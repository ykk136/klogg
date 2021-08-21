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

#include "logger.h"
#include "log.h"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

namespace logging {

void kloggFileMessageHandler( QtMsgType type, const QMessageLogContext& context,
                              const QString& msg );
void kloggConsoleMessageHandler( QtMsgType type, const QMessageLogContext& context,
                                 const QString& msg );
void kloggNoopMessageHandler( QtMsgType, const QMessageLogContext&, const QString& ) {}

class Logger {
  public:
    static Logger& instance()
    {
        static Logger l;
        return l;
    }

    void fileMessageHandler( QtMsgType type, const QMessageLogContext& context, const QString& msg )
    {
        if ( !needLogging( type ) ) {
            return;
        }

        ScopedLock lock( mutex_ );

        const auto formattedMessage = qPrintable( qFormatLogMessage( type, context, msg ) );
        QTextStream ts( logFile_.get() );
        ts << formattedMessage << '\n';

        std::cout << formattedMessage << '\n';
    }

    void consoleMessageHandler( QtMsgType type, const QMessageLogContext& context,
                                const QString& msg )
    {
        if ( !needLogging( type ) ) {
            return;
        }

        ScopedLock lock( mutex_ );

        const auto formattedMessage = qPrintable( qFormatLogMessage( type, context, msg ) );
        std::cout << formattedMessage << '\n';
    }

    void enableLogging( bool isEnabled, uint8_t logLevel, bool logToFile )
    {
        ScopedLock lock( mutex_ );

        qSetMessagePattern( "%{time} %{type} [%{threadid}] [%{function}@%{line}] %{message}" );

        isEnabled_ = isEnabled;
        logLevel_ = logLevel;

        if ( logToFile && !logFile_ ) {
            auto logFileName
                = QString( "klogg_%1_%2.log" )
                      .arg( QDateTime::currentDateTime().toString( "yyyy-MM-dd_HH-mm-ss" ) )
                      .arg( QCoreApplication::applicationPid() );

            logFile_ = std::make_unique<QFile>( QDir::temp().filePath( logFileName ) );
            if ( !logFile_->open( QIODevice::WriteOnly | QIODevice::Append ) ) {
                logFile_.release();
            }
        }

        if ( !isEnabled_ ) {
            qInstallMessageHandler( logging::kloggNoopMessageHandler );
        }
        else if ( logFile_ ) {
            qInstallMessageHandler( logging::kloggFileMessageHandler );
        }
        else {
            qInstallMessageHandler( logging::kloggConsoleMessageHandler );
        }
    }

    bool needLogging( QtMsgType type ) const
    {
        if ( !isEnabled_ || static_cast<uint8_t>( logLevel( type ) ) > logLevel_ ) {
            return false;
        }
        return true;
    }

  private:
    Logger() {}

  private:
    static LogLevel logLevel( QtMsgType type )
    {
        switch ( type ) {
        case QtFatalMsg:
            return LogLevel::Fatal;
        case QtCriticalMsg:
            return LogLevel::Error;

        case QtWarningMsg:
            return LogLevel::Warning;

        case QtInfoMsg:
            return LogLevel::Info;
        case QtDebugMsg:
            return LogLevel::Debug;
        default:
            return LogLevel::Info;
        }
    }

  private:
    mutable std::shared_mutex mutex_;
    using ScopedLock = std::unique_lock<std::shared_mutex>;

    std::atomic_bool isEnabled_ = false;
    std::atomic_int logLevel_ = 0;

    QString logFileName_;
    std::unique_ptr<QFile> logFile_;
};

void enableLogging( bool isEnabled, LogLevel logLevel, bool logToFile )
{
    Logger::instance().enableLogging( isEnabled, static_cast<uint8_t>( logLevel ), logToFile );
}

void kloggFileMessageHandler( QtMsgType type, const QMessageLogContext& context,
                              const QString& msg )
{
    Logger::instance().fileMessageHandler( type, context, msg );
}

void kloggConsoleMessageHandler( QtMsgType type, const QMessageLogContext& context,
                                 const QString& msg )
{
    Logger::instance().consoleMessageHandler( type, context, msg );
}

bool needLogging( QtMsgType type )
{
    return Logger::instance().needLogging( type );
}

} // namespace logging