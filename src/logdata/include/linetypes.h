/*
 * Copyright (C) 2016 -- 2019 Anton Filimonov and other contributors
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

#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <qglobal.h>
#include <string_view>

#include <type_safe/strong_typedef.hpp>
#include <type_safe/narrow_cast.hpp>

#include <QMetaType>
#include <QString>
#include <type_traits>

#include "log.h"
#include "containers.h"

template <typename StrongType>
constexpr StrongType maxValue()
{
    return StrongType( std::numeric_limits<typename StrongType::UnderlyingType>::max() );
}

// Qt file reading api has qint64 type offsets
struct OffsetInFile : type_safe::strong_typedef<OffsetInFile, int64_t>,
                      type_safe::strong_typedef_op::addition<OffsetInFile>,
                      type_safe::strong_typedef_op::subtraction<OffsetInFile>,
                      type_safe::strong_typedef_op::increment<OffsetInFile>,
                      type_safe::strong_typedef_op::relational_comparison<OffsetInFile>,
                      type_safe::strong_typedef_op::equality_comparison<OffsetInFile> {
    using strong_typedef::strong_typedef;

    using UnderlyingType = int64_t;

    template<typename T = UnderlyingType>
    T get() const {
        if constexpr(std::is_same_v<T, UnderlyingType>) {
            return type_safe::get(*this);
        }
        else {
           return type_safe::narrow_cast<T>(type_safe::get(*this)); 
        }
    }
};

struct LinesCount : type_safe::strong_typedef<LinesCount, uint64_t>,
                      type_safe::strong_typedef_op::addition<LinesCount>,
                      type_safe::strong_typedef_op::subtraction<LinesCount>,
                      type_safe::strong_typedef_op::increment<LinesCount>,
                      type_safe::strong_typedef_op::decrement<LinesCount>,
                      type_safe::strong_typedef_op::relational_comparison<LinesCount>,
                      type_safe::strong_typedef_op::equality_comparison<LinesCount> {
    using strong_typedef::strong_typedef;

    using UnderlyingType = uint64_t;

    template<typename T = UnderlyingType>
    T get() const {
        if constexpr(std::is_same_v<T, UnderlyingType>) {
            return type_safe::get(*this);
        }
        else {
           return type_safe::narrow_cast<T>(type_safe::get(*this)); 
        }
    }
};
Q_DECLARE_METATYPE( LinesCount )

struct LineNumber : type_safe::strong_typedef<LineNumber, uint64_t>,
                    type_safe::strong_typedef_op::increment<LineNumber>,
                    type_safe::strong_typedef_op::decrement<LineNumber>,
                    type_safe::strong_typedef_op::relational_comparison<LineNumber>,
                    type_safe::strong_typedef_op::equality_comparison<LineNumber> {
    using strong_typedef::strong_typedef;

    using UnderlyingType = uint64_t;

    template<typename T = UnderlyingType>
    T get() const {
        if constexpr(std::is_same_v<T, UnderlyingType>) {
            return type_safe::get(*this);
        }
        else {
           return type_safe::narrow_cast<T>(type_safe::get(*this)); 
        }
    }

};
Q_DECLARE_METATYPE( LineNumber )

struct LineLength : type_safe::strong_typedef<LineLength, int>,
                      type_safe::strong_typedef_op::relational_comparison<LineLength>,
                      type_safe::strong_typedef_op::equality_comparison<LineLength> {
    using strong_typedef::strong_typedef;

    using UnderlyingType = int;

    template<typename T = UnderlyingType>
    T get() const {
        if constexpr(std::is_same_v<T, UnderlyingType>) {
            return type_safe::get(*this);
        }
        else {
           return type_safe::narrow_cast<T>(type_safe::get(*this)); 
        }
    }
};
Q_DECLARE_METATYPE( LineLength )

inline constexpr OffsetInFile operator"" _offset( unsigned long long int value )
{
    return OffsetInFile( static_cast<OffsetInFile::UnderlyingType>( value ) );
}
inline constexpr LineNumber operator"" _lnum( unsigned long long int value )
{
    return LineNumber( static_cast<LineNumber::UnderlyingType>( value ) );
}
inline constexpr LinesCount operator"" _lcount( unsigned long long int value )
{
    return LinesCount( static_cast<LinesCount::UnderlyingType>( value ) );
}
inline constexpr LineLength operator"" _length( unsigned long long int value )
{
    return LineLength( static_cast<LineLength::UnderlyingType>( value ) );
}


inline LineNumber& operator+=( LineNumber& number, const LinesCount& count )
{
    number = ( number.get() <= maxValue<LineNumber>().get() - count.get() )
               ? LineNumber( number.get() + count.get() )
               : maxValue<LineNumber>();
    return number;
}
inline LineNumber operator+( const LineNumber& number, const LinesCount& count )
{
    return ( number.get() <= maxValue<LineNumber>().get() - count.get() )
               ? LineNumber( number.get() + count.get() )
               : maxValue<LineNumber>();
}

inline LineNumber operator-( const LineNumber& number, const LinesCount& count )
{
    return number.get() >= count.get() ? LineNumber( number.get() - count.get() )
                                       : LineNumber( 0u );
}

inline LinesCount operator-( const LineNumber& n1, const LineNumber& n2 )
{
    return n1.get() >= n2.get() ? LinesCount( n1.get() - n2.get() ) : LinesCount( 0u );
}

inline bool operator<( const LineNumber& number, const LinesCount& count )
{
    return number.get() < count.get();
}
inline bool operator>=( const LineNumber& number, const LinesCount& count )
{
    return !( number < count );
}


using OptionalLineNumber = std::optional<LineNumber>;

template <typename Tag, typename T>
QDebug operator<<( QDebug dbg, type_safe::strong_typedef<Tag, T> const& object )
{
    QDebugStateSaver saver( dbg );
    dbg << type_safe::get(object);

    return dbg;
}

// Represents a position in a file (line, column)
class FilePosition {
  public:
    FilePosition()
        : column_{ -1 }
    {
    }
    FilePosition( LineNumber line, int column )
        : line_{ line }
        , column_{ column }
    {
    }

    LineNumber line() const
    {
        return line_;
    }
    int column() const
    {
        return column_;
    }

  private:
    LineNumber line_;
    int column_;
};

// Length of a tab stop
constexpr int TabStop = 8;

inline QString untabify( QString&& line, int initialPosition = 0 )
{
    LineLength::UnderlyingType totalSpaces = 0;
    line.replace( QChar::Null, QChar::Space );

    LineLength::UnderlyingType position = 0;
    position
        = static_cast<LineLength::UnderlyingType>( line.indexOf( QChar::Tabulation, position ) );
    while ( position >= 0 ) {
        const auto spaces = TabStop - ( ( initialPosition + position + totalSpaces ) % TabStop );
        line.replace( position, 1, QString( spaces, QChar::Space ) );
        totalSpaces += spaces - 1;
        position = static_cast<LineLength::UnderlyingType>(
            line.indexOf( QChar::Tabulation, position ) );
    }

    return std::move( line );
}

template <typename LineType>
LineLength getUntabifiedLength( const LineType& utf8Line )
{
    size_t totalSpaces = 0;

    auto tabPosition = utf8Line.find( '\t' );
    while ( tabPosition != std::string_view::npos ) {
        const auto spaces = TabStop - ( ( tabPosition + totalSpaces ) % TabStop ) - 1;
        totalSpaces += spaces;

        tabPosition = utf8Line.find( '\t', tabPosition + 1 );
    }

    return LineLength( type_safe::narrow_cast<LineLength::UnderlyingType>( utf8Line.size() + totalSpaces ) );
}
