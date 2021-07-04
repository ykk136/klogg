/*
 * Copyright (C) 2021 Anton Filimonov and other contributors
 *
 * This file is part of klogg.
 *
 * klogg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * klogg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with klogg.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <iterator>
#include <numeric>
#include <qregularexpression.h>
#include <string_view>
#include <vector>

#include <QRegularExpression>

#ifdef KLOGG_HAS_HS
#include "hsregularexpression.h"

#include "log.h"

namespace {

struct MatcherContext {
    const std::size_t patternsCount = 0;
    MatchedPatterns& matchingPatterns;
    bool hasMatch = false;
};

int matchCallback( unsigned int id, unsigned long long from, unsigned long long to,
                   unsigned int flags, void* context )
{
    Q_UNUSED( from );
    Q_UNUSED( to );
    Q_UNUSED( flags );

    MatcherContext* matchContext = static_cast<MatcherContext*>( context );

    if ( matchContext->patternsCount == 1 ) {
        matchContext->hasMatch = true;
        return 1;
    }
    else {
        matchContext->matchingPatterns[ id ] = true;
    }

    return 0;
}

} // namespace

HsMatcher::HsMatcher( HsDatabase db, HsScratch scratch, const std::vector<std::string>& patternIds )
    : database_{ std::move( db ) }
    , scratch_{ std::move( scratch ) }
    , patternIds_{ patternIds }
{
}

MatchingResult HsMatcher::match( const std::string_view& utf8Data ) const
{

    if ( !scratch_ || !database_ ) {
        return {};
    }
    MatchedPatterns matchingPatterns( patternIds_.size() );

    MatcherContext context{ patternIds_.size(), matchingPatterns };
    hs_scan( database_.get(), utf8Data.data(), static_cast<unsigned int>( utf8Data.size() ), 0,
             scratch_.get(), matchCallback, static_cast<void*>( &context ) );

    if ( context.patternsCount == 1 ) {
        return context.hasMatch;
    }
    else {
        return matchingPatterns;
    }
}

HsRegularExpression::HsRegularExpression( const RegularExpressionPattern& pattern )
    : HsRegularExpression( std::vector<RegularExpressionPattern>{ pattern } )
{
}

HsRegularExpression::HsRegularExpression( const std::vector<RegularExpressionPattern>& patterns )
    : patterns_( patterns )
{
    std::transform( patterns.cbegin(), patterns.cend(), std::back_inserter( patternIds_ ),
                    []( const auto& p ) { return p.id(); } );

    database_ = HsDatabase{ makeUniqueResource<hs_database_t, hs_free_database>(
        []( const std::vector<RegularExpressionPattern>& expressions,
            QString& errorMessage ) -> hs_database_t* {
            hs_database_t* db = nullptr;
            hs_compile_error_t* error = nullptr;

            std::vector<unsigned> flags( expressions.size() );
            std::transform( expressions.cbegin(), expressions.cend(), flags.begin(),
                            []( const auto& expression ) {
                                auto expressionFlags
                                    = HS_FLAG_UTF8 | HS_FLAG_UCP | HS_FLAG_SINGLEMATCH;
                                if ( !expression.isCaseSensitive ) {
                                    expressionFlags |= HS_FLAG_CASELESS;
                                }
                                return expressionFlags;
                            } );

            std::vector<QByteArray> utf8Patterns( expressions.size() );
            std::transform( expressions.cbegin(), expressions.cend(), utf8Patterns.begin(),
                            []( const auto& expression ) {
                                auto p = expression.pattern;
                                if ( expression.isPlainText ) {
                                    p = QRegularExpression::escape( expression.pattern );
                                }
                                return p.toUtf8();
                            } );

            std::vector<const char*> patternPointers( utf8Patterns.size() );
            std::transform( utf8Patterns.cbegin(), utf8Patterns.cend(), patternPointers.begin(),
                            []( const auto& utf8Pattern ) { return utf8Pattern.data(); } );

            std::vector<unsigned> expressionIds( expressions.size() );
            std::iota( expressionIds.begin(), expressionIds.end(), 0u );

            const auto compileResult = hs_compile_multi(
                patternPointers.data(), flags.data(), expressionIds.data(),
                static_cast<unsigned>( expressions.size() ), HS_MODE_BLOCK, nullptr, &db, &error );

            if ( compileResult != HS_SUCCESS ) {
                LOG_ERROR << "Failed to compile pattern " << error->message;
                errorMessage = error->message;
                hs_free_compile_error( error );
                return nullptr;
            }

            return db;
        },
        patterns, errorMessage_ ) };

    if ( database_ ) {
        scratch_ = makeUniqueResource<hs_scratch_t, hs_free_scratch>(
            []( hs_database_t* db ) -> hs_scratch_t* {
                hs_scratch_t* scratch = nullptr;

                const auto scratchResult = hs_alloc_scratch( db, &scratch );
                if ( scratchResult != HS_SUCCESS ) {
                    LOG_ERROR << "Failed to allocate scratch";
                    return nullptr;
                }

                return scratch;
            },
            database_.get() );
    }

    if ( !isHsValid() ) {
        const auto validationResult = std::transform_reduce(
            patterns_.cbegin(), patterns_.cend(), std::make_pair( true, QString{} ),
            []( const auto& acc, const auto& next ) { return acc.first ? next : acc; },
            []( const auto& pattern ) {
                const auto regex = static_cast<QRegularExpression>( pattern );
                return std::make_pair( regex.isValid(), regex.errorString() );
            } );

        isValid_ = validationResult.first;
        errorMessage_ = validationResult.second;
    }
}

bool HsRegularExpression::isValid() const
{
    return isValid_;
}

bool HsRegularExpression::isHsValid() const
{
    return database_ != nullptr && scratch_ != nullptr;
}

QString HsRegularExpression::errorString() const
{
    return errorMessage_;
}

MatcherVariant HsRegularExpression::createMatcher() const
{
    if ( !isHsValid() ) {
        return MatcherVariant{ DefaultRegularExpressionMatcher( patterns_ ) };
    }

    auto matcherScratch = makeUniqueResource<hs_scratch_t, hs_free_scratch>(
        []( hs_scratch_t* prototype ) -> hs_scratch_t* {
            hs_scratch_t* scratch = nullptr;

            const auto err = hs_clone_scratch( prototype, &scratch );
            if ( err != HS_SUCCESS ) {
                LOG_ERROR << "hs_clone_scratch failed";
                return nullptr;
            }

            return scratch;
        },
        scratch_.get() );

    return MatcherVariant{ HsMatcher{ database_, std::move( matcherScratch ), patternIds_ } };
}
#endif
