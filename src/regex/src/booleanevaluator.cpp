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

#include "booleanevaluator.h"

#include "log.h"
#include <algorithm>
#include <string>

BooleanExpressionEvaluator::BooleanExpressionEvaluator(
    const std::string& expression, const std::vector<RegularExpressionPattern>& patterns )
{
    variables_.reserve( patterns.size() );

    for ( const auto& p : patterns ) {
        if ( symbols_.create_variable( p.id() ) ) {
            variables_.push_back( &symbols_.get_variable( p.id() )->ref() );
        }
    }

    expression_.register_symbol_table( symbols_ );
    isValid_ = parser_.compile( expression, expression_ );
    if ( !isValid_ && parser_.error_count() > 0 ) {
        auto error = parser_.get_error( 0 );
        exprtk::parser_error::update_error( error, expression );
        errorString_ = error.diagnostic + " at " + std::to_string( error.column_no );
    }
}

bool BooleanExpressionEvaluator::evaluate( const std::vector<unsigned char>& variables )
{
    if ( !isValid() ) {
        return false;
    }

    if ( variables_.size() != variables.size() ) {
        LOG_ERROR << "Wrong number of matched patterns";
        return false;
    }

    for ( auto index = 0u; index < variables_.size(); ++index ) {
        *variables_[ index ] = variables[ index ];
    }

    return expression_.value() > 0;
}
