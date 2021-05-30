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

#ifndef KLOGG_SHORTCUTS_H
#define KLOGG_SHORTCUTS_H

#include <map>
#include <qobject.h>
#include <qwidget.h>
#include <string>
#include <string_view>

#include <QKeySequence>
#include <QShortcut>
#include <QStringList>

struct ShortcutAction {
    static constexpr auto CrawlerChangeVisibility = "crawler.change_visibility_type";
    static constexpr auto CrawlerIncreseTopViewSize = "crawler.increase_top_view_size";
    static constexpr auto CrawlerDecreaseTopViewSize = "crawler.decrease_top_view_size";

    static constexpr auto QfFindNext = "qf.find_next";
    static constexpr auto QfFindPrev = "qf.find_prev";

    static constexpr auto MainWindowOpenFile = "mainwindow.open_file";
    static constexpr auto MainWindowNewWindow = "mainwindow.new_window";
    static constexpr auto MainWindowCloseFile = "mainwindow.close_file";
    static constexpr auto MainWindowCloseAll = "mainwindow.close_all";
    static constexpr auto MainWindowQuit = "mainwindow.quit";
    static constexpr auto MainWindowSelectAll = "mainwindow.select_all";
    static constexpr auto MainWindowOpenQf = "mainwindow.open_qf";
    static constexpr auto MainWindowOpenQfForward = "mainwindow.open_qf_forward";
    static constexpr auto MainWindowOpenQfBackward = "mainwindow.open_qf_backward";
    static constexpr auto MainWindowFocusSearchInput = "qf.focus_search";
    static constexpr auto MainWindowClearFile = "mainwindow.clear_file";
    static constexpr auto MainWindowFollowFile = "mainwindow.follow_file";
    static constexpr auto MainWindowReload = "mainwindow.reload";
    static constexpr auto MainWindowSelectOpenFile = "mainwindow.select_open_file";

    static constexpr auto LogViewMark = "logview.mark";
    static constexpr auto LogViewNextMark = "logview.next_mark";
    static constexpr auto LogViewPrevMark = "logview.prev_mark";
    static constexpr auto LogViewSelectionUp = "logview.selection_up";
    static constexpr auto LogViewSelectionDown = "logview.selection_down";
    static constexpr auto LogViewScrollUp = "logview.scroll_up";
    static constexpr auto LogViewScrollDown = "logview.scroll_down";
    static constexpr auto LogViewScrollLeft = "logview.scroll_left";
    static constexpr auto LogViewScrollRight = "logview.scroll_right";
    static constexpr auto LogViewJumpToTop = "logview.jump_to_top";
    static constexpr auto LogViewJumpToButtom = "logview.jump_to_buttom";
    static constexpr auto LogViewJumpToStartOfLine = "logview.jump_to_start_of_line";
    static constexpr auto LogViewJumpToEndOfLine = "logview.jump_to_end_of_line";
    static constexpr auto LogViewJumpToRightOfScreen = "logview.jump_to_right";
    static constexpr auto LogViewJumpToLine = "logview.jump_to_line";
    static constexpr auto LogViewJumpToLineNumber = "logview.jump_to_line_number";

    static constexpr auto LogViewQfSelectedForward = "logview.qf_selected_forward";
    static constexpr auto LogViewQfSelectedBackward = "logview.qf_selected_backward";
    static constexpr auto LogViewQfForward = "logview.open_qf_forward";
    static constexpr auto LogViewQfBackward = "logview.open_qf_backward";

    static constexpr auto LogViewExitView = "logview.exit_view";

    static const std::map<std::string, QStringList>& defaultShortcuts()
    {
        static const std::map<std::string, QStringList> defaultShortcuts = []() {
            std::map<std::string, QStringList> shortcuts;

            auto getKeyBindings = []( QKeySequence::StandardKey standardKey ) {
                auto bindings = QKeySequence::keyBindings( standardKey );
                QStringList stringBindings;
                std::transform( bindings.begin(), bindings.end(),
                                std::back_inserter( stringBindings ),
                                []( const auto& keySequence ) { return keySequence.toString(); } );

                return stringBindings;
            };

            shortcuts.emplace( CrawlerChangeVisibility,
                               QStringList() << QKeySequence( Qt::Key_V ).toString() );
            shortcuts.emplace( CrawlerIncreseTopViewSize,
                               QStringList() << QKeySequence( Qt::Key_Plus ).toString() );
            shortcuts.emplace( CrawlerDecreaseTopViewSize,
                               QStringList() << QKeySequence( Qt::Key_Minus ).toString() );

            shortcuts.emplace( QfFindNext, getKeyBindings( QKeySequence::FindNext ) );
            shortcuts.emplace( QfFindPrev, getKeyBindings( QKeySequence::FindPrevious ) );

            shortcuts.emplace( LogViewMark, QStringList() << QKeySequence( Qt::Key_M ).toString() );

            shortcuts.emplace( LogViewNextMark,
                               QStringList() << QKeySequence( Qt::Key_BracketRight ).toString() );
            shortcuts.emplace( LogViewPrevMark,
                               QStringList() << QKeySequence( Qt::Key_BracketLeft ).toString() );
            shortcuts.emplace( LogViewSelectionUp, QStringList()
                                                       << QKeySequence( Qt::Key_Up ).toString()
                                                       << QKeySequence( Qt::Key_K ).toString() );
            shortcuts.emplace( LogViewSelectionDown, QStringList()
                                                         << QKeySequence( Qt::Key_Down ).toString()
                                                         << QKeySequence( Qt::Key_J ).toString() );
            shortcuts.emplace( LogViewScrollUp, QStringList() << "Ctrl+Up" );
            shortcuts.emplace( LogViewScrollDown, QStringList() << "Ctrl+Down" );
            shortcuts.emplace( LogViewScrollLeft, QStringList()
                                                      << QKeySequence( Qt::Key_Left ).toString()
                                                      << QKeySequence( Qt::Key_H ).toString() );
            shortcuts.emplace( LogViewScrollRight, QStringList()
                                                       << QKeySequence( Qt::Key_Right ).toString()
                                                       << QKeySequence( Qt::Key_L ).toString() );
            shortcuts.emplace( LogViewJumpToStartOfLine,
                               QStringList() << QKeySequence( Qt::Key_Home ).toString()
                                             << QKeySequence( Qt::Key_AsciiCircum ).toString() );
            shortcuts.emplace( LogViewJumpToEndOfLine,
                               QStringList() << QKeySequence( Qt::Key_Dollar ).toString() );
            shortcuts.emplace( LogViewJumpToRightOfScreen,
                               QStringList() << QKeySequence( Qt::Key_End ).toString() );
            shortcuts.emplace( LogViewJumpToButtom, QStringList() << "Ctrl+End"
                                                                  << "Shift+G" );
            shortcuts.emplace( LogViewJumpToTop, QStringList() << "Ctrl+Home" );
            shortcuts.emplace( LogViewJumpToLine, QStringList() << "Alt+G" );
            shortcuts.emplace( LogViewQfForward, getKeyBindings( QKeySequence::FindNext )
                                                     << QKeySequence( Qt::Key_N ).toString()
                                                     << "Ctrl+G" );
            shortcuts.emplace( LogViewQfBackward, getKeyBindings( QKeySequence::FindPrevious )
                                                      << "Shift+N"
                                                      << "Ctrl+Shift+G" );

            shortcuts.emplace( LogViewQfSelectedForward,
                               QStringList() << QKeySequence( Qt::Key_Asterisk ).toString()
                                             << QKeySequence( Qt::Key_Period ).toString() );
            shortcuts.emplace( LogViewQfSelectedBackward,
                               QStringList() << QKeySequence( Qt::Key_Slash ).toString()
                                             << QKeySequence( Qt::Key_Comma ).toString() );
            shortcuts.emplace( LogViewExitView, QStringList()
                                                    << QKeySequence( Qt::Key_Space ).toString() );

            return shortcuts;
        }();

        return defaultShortcuts;
    }

    static QStringList defaultShortcuts( const std::string& action )
    {
        const auto& shortcuts = defaultShortcuts();
        const auto actionShortcuts = shortcuts.find( action );
        if ( actionShortcuts == shortcuts.end() ) {
            return {};
        }

        return actionShortcuts->second;
    }

    static QString actionName( const std::string& action )
    {
        static const std::map<std::string, QString> actionNames = []() {
            std::map<std::string, QString> shortcuts;

            shortcuts.emplace( CrawlerChangeVisibility, "Change filtered lines visibility" );
            shortcuts.emplace( CrawlerIncreseTopViewSize, "Increase main view" );
            shortcuts.emplace( CrawlerDecreaseTopViewSize, "Decrease main view" );

            shortcuts.emplace( QfFindNext, "QuickFind: Find next" );
            shortcuts.emplace( QfFindPrev, "QuickFind: Find previous" );

            shortcuts.emplace( LogViewMark, "Add line mark" );

            shortcuts.emplace( LogViewNextMark, "Jump to next mark" );
            shortcuts.emplace( LogViewPrevMark, "Jump to previous mark" );
            shortcuts.emplace( LogViewSelectionUp, "Move selection up" );
            shortcuts.emplace( LogViewSelectionDown, "Move selection down" );
            shortcuts.emplace( LogViewScrollUp, "Scroll up" );
            shortcuts.emplace( LogViewScrollDown, "Scroll down" );
            shortcuts.emplace( LogViewScrollLeft, "Scroll left" );
            shortcuts.emplace( LogViewScrollRight, "Scroll right" );
            shortcuts.emplace( LogViewJumpToStartOfLine,
                               "Jump to the beginning of the current line" );
            shortcuts.emplace( LogViewJumpToEndOfLine,
                               "Jump to the end start of the current line" );
            shortcuts.emplace( LogViewJumpToRightOfScreen, "Jump to the right of the text" );
            shortcuts.emplace( LogViewJumpToButtom, "Jump to the buttom of the text" );
            shortcuts.emplace( LogViewJumpToTop, "Jump to the top of the text" );
            shortcuts.emplace( LogViewJumpToLine, "Jump to line" );
            shortcuts.emplace( LogViewQfForward, "Main view: find next" );
            shortcuts.emplace( LogViewQfBackward, "Main view: find previous" );

            shortcuts.emplace( LogViewQfSelectedForward,
                               "Set selection to QuickFind and find next" );
            shortcuts.emplace( LogViewQfSelectedBackward,
                               "Set selection to QuickFind and find previous" );

            shortcuts.emplace( LogViewExitView, "Release focus from view" );

            return shortcuts;
        }();

        const auto name = actionNames.find( action );

        return name != actionNames.end() ? name->second : QString::fromStdString( action );
    }

    static void registerShortcut( const std::map<std::string, QStringList>& configuredShortcuts,
                                  std::map<QString, QShortcut*>& shortcutsStorage,
                                  QWidget* shortcutsParent, Qt::ShortcutContext context,
                                  const std::string& action, std::function<void()> func )
    {
        auto keysConfiguration = configuredShortcuts.find( action );
        auto keys = keysConfiguration != configuredShortcuts.end()
                        ? keysConfiguration->second
                        : ShortcutAction::defaultShortcuts( action );

        for ( const auto& key : qAsConst( keys ) ) {
            if ( key.isEmpty() ) {
                continue;
            }
            auto shortcut = shortcutsStorage.find( key );
            if ( shortcut != shortcutsStorage.end() ) {
                shortcut->second->setKey( QKeySequence( key ) );
            }
            else {
                shortcutsStorage.emplace(
                    key, new QShortcut( QKeySequence( key ), shortcutsParent, func, context ) );
            }
        }
    }
};

#endif