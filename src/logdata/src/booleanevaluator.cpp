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

BooleanExpressionEvaluator::BooleanExpressionEvaluator(
    const std::string& expression, const std::vector<RegularExpressionPattern>& patterns )
{
    for ( const auto& p : patterns ) {
        symbols_.create_variable( p.id() );
    }
    expression_.register_symbol_table( symbols_ );
    isValid_ = parser_.compile( expression, expression_ );
    if ( !isValid_ ) {
        errorString_ = parser_.error();
    }
}

bool BooleanExpressionEvaluator::evaluate(
    const robin_hood::unordered_flat_map<std::string, bool>& variables )
{
    if ( !isValid() ) {
        return false;
    }

    for ( const auto& result : variables ) {
        symbols_.get_variable( result.first )->ref() = result.second ? 1 : 0;
    }

    return expression_.value() > 0;
}
