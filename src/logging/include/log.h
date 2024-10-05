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

#ifndef KLOGG_LOG_H
#define KLOGG_LOG_H

#include <QDebugStateSaver>
#include <QMessageLogContext>
#include <QString>

#include <chrono>
#include <optional>
#include <string>

#include <cassert>

// Undefine QT_MESSAGELOG_FILE to get real filename rather than full file path
#undef QT_MESSAGELOG_FILE
#define QT_MESSAGELOG_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_IF_( severity )                                                                        \
    if ( !logging::needLogging( severity ) ) {                                                     \
        ;                                                                                          \
    }                                                                                              \
    else

#define LOG_DEBUG LOG_IF_( QtDebugMsg ) qDebug().nospace()
#define LOG_INFO LOG_IF_( QtInfoMsg ) qInfo().nospace()
#define LOG_WARNING LOG_IF_( QtWarningMsg ) qWarning().nospace()
#define LOG_ERROR LOG_IF_( QtCriticalMsg ) qCritical().nospace()
#define LOG_FATAL LOG_IF_( QtFatalMsg ) qFatal().nospace()

namespace logging {
bool needLogging( QtMsgType type );
} // namespace logging

template <typename T>
QDebug operator<<( QDebug dbg, const std::optional<T>& opt )
{
    QDebugStateSaver saver( dbg );
    opt ? dbg << *opt : dbg << "<empty>";

    return dbg;
}

inline QDebug operator<<( QDebug dbg, const std::chrono::microseconds& duration )
{
    QDebugStateSaver saver( dbg );
    dbg << static_cast<float>( duration.count() ) / 1000.f << " ms";

    return dbg;
}

inline QDebug operator<<( QDebug dbg, const std::string& str )
{
    QDebugStateSaver saver( dbg );
    dbg << QString::fromStdString( str );

    return dbg;
}

#endif