/*
 * Copyright (C) 2020 Anton Filimonov and other contributors
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

#ifndef KLOGG_STYLES
#define KLOGG_STYLES

#include <QApplication>
#include <QColor>
#include <QPalette>
#include <QStyleFactory>

#include "log.h"

struct StyleManager {

    static constexpr QLatin1String DarkStyleKey = QLatin1String( "Dark", 4 );
    static constexpr QLatin1String VistaKey = QLatin1String( "WindowsVista", 12 );
    static constexpr QLatin1String FusionKey = QLatin1String( "Fusion", 6 );
    static constexpr QLatin1String WindowsKey = QLatin1String( "Windows", 7 );
    static constexpr QLatin1String MacintoshKey = QLatin1String( "macintosh", 9 );

    static constexpr QLatin1String Gtk2Key = QLatin1String( "Gtk2", 4 );
    static constexpr QLatin1String Bb10Key = QLatin1String( "bb10", 4 );

    static QStringList availableStyles()
    {
        QStringList styles;
#ifdef Q_OS_WIN
        styles << VistaKey;
        styles << WindowsKey;
        styles << FusionKey;
#else
        styles << QStyleFactory::keys();
#endif

        auto removedStyles
            = std::remove_if( styles.begin(), styles.end(), []( const QString& style ) {
                  return style.startsWith( Gtk2Key, Qt::CaseInsensitive )
                         || style.startsWith( Bb10Key, Qt::CaseInsensitive );
              } );

        styles.erase( removedStyles, styles.end() );

        styles << DarkStyleKey;

        std::sort( styles.begin(), styles.end(), []( const auto& lhs, const auto& rhs ) {
            return lhs.compare( rhs, Qt::CaseInsensitive ) < 0;
        } );

        return styles;
    }

    static QString defaultPlatformStyle()
    {
#if defined( Q_OS_WIN )
        return VistaKey;
#elif defined( Q_OS_MACOS )
        return MacintoshKey;
#else
        return FusionKey;
#endif
    }

    static void applyStyle( const QString& style )
    {
        LOG_INFO << "Setting style to " << style;

        if ( style == DarkStyleKey ) {

            // based on https://gist.github.com/QuantumCD/6245215

            QColor darkGray( 53, 53, 53 );
            QColor gray( 128, 128, 128 );
            QColor black( 40, 40, 40 );
            QColor white( 240, 240, 240 );
            QColor blue( 42, 130, 218 );

            QPalette darkPalette;
            darkPalette.setColor( QPalette::Window, darkGray );
            darkPalette.setColor( QPalette::WindowText, white );
            darkPalette.setColor( QPalette::Base, black );
            darkPalette.setColor( QPalette::AlternateBase, darkGray );
            darkPalette.setColor( QPalette::ToolTipBase, blue );
            darkPalette.setColor( QPalette::ToolTipText, white );
            darkPalette.setColor( QPalette::Text, white );
            darkPalette.setColor( QPalette::Button, darkGray );
            darkPalette.setColor( QPalette::ButtonText, white );
            darkPalette.setColor( QPalette::Link, blue );
            darkPalette.setColor( QPalette::Highlight, blue );
            darkPalette.setColor( QPalette::HighlightedText, black.darker() );

            darkPalette.setColor( QPalette::Active, QPalette::Button, gray.darker() );
            darkPalette.setColor( QPalette::Disabled, QPalette::ButtonText, gray );
            darkPalette.setColor( QPalette::Disabled, QPalette::WindowText, gray );
            darkPalette.setColor( QPalette::Disabled, QPalette::Text, gray );
            darkPalette.setColor( QPalette::Disabled, QPalette::Light, darkGray );

            qApp->setStyle( QStyleFactory::create( FusionKey ) );
            qApp->setPalette( darkPalette );
        }
        else {
            qApp->setStyle( style );
            qApp->setStyleSheet( "" );
        }
    }
};

#endif
