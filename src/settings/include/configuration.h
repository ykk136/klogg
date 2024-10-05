/*
 * Copyright (C) 2009, 2010, 2011, 2013, 2015 Nicolas Bonnefon and other
 * contributors
 *
 * This file is part of glogg.
 *
 * glogg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glogg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glogg.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#ifndef KLOGG_CONFIGURATION_H
#define KLOGG_CONFIGURATION_H

#include <QColor>
#include <QFont>
#include <QSettings>
#include <qcolor.h>
#include <string>
#include <string_view>

#include "persistable.h"

// Type of regexp to use for searches
enum class SearchRegexpType {
    ExtendedRegexp,
    Wildcard,
    FixedString,
};

enum class RegexpEngine { Hyperscan, QRegularExpression };
static constexpr int MAX_RECENT_FILES = 25;

// Configuration class containing everything in the "Settings" dialog
class Configuration final : public Persistable<Configuration> {
  public:
    inline static const char* persistableName()
    {
        return "Configuration";
    }
    Configuration();

    // Accesses the main font used for display
    QFont mainFont() const;
    void setMainFont( QFont newFont );

    inline QString language() const
    {
        return language_;
    }

    inline void setLanguage( QString lang )
    {
        language_ = lang;
    }

    // Accesses the regexp types
    inline SearchRegexpType mainRegexpType() const
    {
        return mainRegexpType_;
    }
    inline SearchRegexpType quickfindRegexpType() const
    {
        return quickfindRegexpType_;
    }
    inline bool isQuickfindIncremental() const
    {
        return quickfindIncremental_;
    }
    inline void setMainRegexpType( SearchRegexpType type )
    {
        mainRegexpType_ = type;
    }
    inline void setQuickfindRegexpType( SearchRegexpType type )
    {
        quickfindRegexpType_ = type;
    }
    inline void setQuickfindIncremental( bool isIncremental )
    {
        quickfindIncremental_ = isIncremental;
    }

    // "Advanced" settings
    inline bool anyFileWatchEnabled() const
    {
        return nativeFileWatchEnabled() || pollingEnabled();
    }

    inline bool nativeFileWatchEnabled() const
    {
        return nativeFileWatchEnabled_;
    }
    inline void setNativeFileWatchEnabled( bool enabled )
    {
        nativeFileWatchEnabled_ = enabled;
    }
    inline bool pollingEnabled() const
    {
        return pollingEnabled_;
    }
    inline void setPollingEnabled( bool enabled )
    {
        pollingEnabled_ = enabled;
    }
    inline int pollIntervalMs() const
    {
        return pollIntervalMs_;
    }
    inline void setPollIntervalMs( int interval )
    {
        pollIntervalMs_ = interval;
    }

    inline bool fastModificationDetection() const
    {
        return fastModificationDetection_;
    }

    inline void setFastModificationDetection( bool fastDetection )
    {
        fastModificationDetection_ = fastDetection;
    }

    inline bool loadLastSession() const
    {
        return loadLastSession_;
    }
    inline void setLoadLastSession( bool enabled )
    {
        loadLastSession_ = enabled;
    }
    inline bool followFileOnLoad() const
    {
        return followFileOnLoad_;
    }
    inline void setFollowFileOnLoad( bool enabled )
    {
        followFileOnLoad_ = enabled;
    }
    inline bool allowMultipleWindows() const
    {
        return allowMultipleWindows_;
    }
    inline void setAllowMultipleWindows( bool enabled )
    {
        allowMultipleWindows_ = enabled;
    }

    // perf settings
    inline bool useParallelSearch() const
    {
        return useParallelSearch_;
    }
    inline void setUseParallelSearch( bool enabled )
    {
        useParallelSearch_ = enabled;
    }
    inline bool useSearchResultsCache() const
    {
        return useSearchResultsCache_;
    }
    inline void setUseSearchResultsCache( bool enabled )
    {
        useSearchResultsCache_ = enabled;
    }
    inline unsigned searchResultsCacheLines() const
    {
        return searchResultsCacheLines_;
    }
    inline void setSearchResultsCacheLines( unsigned lines )
    {
        searchResultsCacheLines_ = lines;
    }
    inline int indexReadBufferSizeMb() const
    {
        return indexReadBufferSizeMb_;
    }
    inline void setIndexReadBufferSizeMb( int bufferSizeMb )
    {
        indexReadBufferSizeMb_ = bufferSizeMb;
    }
    inline int searchReadBufferSizeLines() const
    {
        return searchReadBufferSizeLines_;
    }
    inline void setSearchReadBufferSizeLines( int lines )
    {
        searchReadBufferSizeLines_ = lines;
    }
    inline int searchThreadPoolSize() const
    {
        return searchThreadPoolSize_;
    }
    inline void setSearchThreadPoolSize( int threads )
    {
        searchThreadPoolSize_ = threads;
    }
    inline bool keepFileClosed() const
    {
        return keepFileClosed_;
    }
    inline void setKeepFileClosed( bool shouldKeepClosed )
    {
        keepFileClosed_ = shouldKeepClosed;
    }

    inline RegexpEngine regexpEngine() const
    {
        return regexpEngine_;
    }

    inline void setRegexpEnging( RegexpEngine engine )
    {
        regexpEngine_ = engine;
    }

    // Accessors
    inline bool versionCheckingEnabled() const
    {
        return enableVersionChecking_;
    }
    inline void setVersionCheckingEnabled( bool enabled )
    {
        enableVersionChecking_ = enabled;
    }

    // View settings
    inline bool isOverviewVisible() const
    {
        return overviewVisible_;
    }
    inline void setOverviewVisible( bool isVisible )
    {
        overviewVisible_ = isVisible;
    }
    inline bool mainLineNumbersVisible() const
    {
        return lineNumbersVisibleInMain_;
    }
    inline bool filteredLineNumbersVisible() const
    {
        return lineNumbersVisibleInFiltered_;
    }
    inline bool minimizeToTray() const
    {
        return minimizeToTray_;
    }
    inline QString style() const
    {
        return style_;
    }
    inline void setMainLineNumbersVisible( bool lineNumbersVisible )
    {
        lineNumbersVisibleInMain_ = lineNumbersVisible;
    }
    inline void setFilteredLineNumbersVisible( bool lineNumbersVisible )
    {
        lineNumbersVisibleInFiltered_ = lineNumbersVisible;
    }
    inline void setMinimizeToTray( bool minimizeToTray )
    {
        minimizeToTray_ = minimizeToTray;
    }
    inline void setStyle( const QString& style )
    {
        style_ = style;
    }

    inline bool enableLogging() const
    {
        return enableLogging_;
    }
    inline int loggingLevel() const
    {
        return loggingLevel_;
    }

    inline void setEnableLogging( bool enableLogging )
    {
        enableLogging_ = enableLogging;
    }
    inline void setLoggingLevel( int level )
    {
        loggingLevel_ = level;
    }

    // Default settings for new views
    inline bool isSearchAutoRefreshDefault() const
    {
        return searchAutoRefresh_;
    }
    inline void setSearchAutoRefreshDefault( bool autoRefresh )
    {
        searchAutoRefresh_ = autoRefresh;
    }
    inline bool isSearchIgnoreCaseDefault() const
    {
        return searchIgnoreCase_;
    }
    inline void setSearchIgnoreCaseDefault( bool ignoreCase )
    {
        searchIgnoreCase_ = ignoreCase;
    }
    inline QList<int> splitterSizes() const
    {
        return splitterSizes_;
    }
    inline void setSplitterSizes( QList<int> sizes )
    {
        splitterSizes_ = std::move( sizes );
    }

    inline bool extractArchives() const
    {
        return extractArchives_;
    }
    inline void setExtractArchives( bool extract )
    {
        extractArchives_ = extract;
    }

    inline bool extractArchivesAlways() const
    {
        return extractArchivesAlways_;
    }
    inline void setExtractArchivesAlways( bool extract )
    {
        extractArchivesAlways_ = extract;
    }

    inline bool verifySslPeers() const
    {
        return verifySslPeers_;
    }
    inline void setVerifySslPeers( bool verify )
    {
        verifySslPeers_ = verify;
    }

    inline bool forceFontAntialiasing() const
    {
        return forceFontAntialiasing_;
    }
    inline void setForceFontAntialiasing( bool force )
    {
        forceFontAntialiasing_ = force;
    }

    inline bool enableQtHighDpi() const
    {
        return enableQtHighDpi_;
    }
    inline void setEnableQtHighDpi( bool enable )
    {
        enableQtHighDpi_ = enable;
    }

    inline int scaleFactorRounding() const
    {
        return scaleFactorRounding_;
    }
    inline void setScaleFactorRounding( int rounding )
    {
        scaleFactorRounding_ = rounding;
    }

    inline bool mainSearchHighlight() const
    {
        return enableMainSearchHighlight_;
    }
    inline void setEnableMainSearchHighlight( bool enable )
    {
        enableMainSearchHighlight_ = enable;
    }

    inline bool variateMainSearchHighlight() const
    {
        return enableMainSearchHighlightVariance_;
    }
    inline void setVariateMainSearchHighlight( bool enable )
    {
        enableMainSearchHighlightVariance_ = enable;
    }

    inline QColor mainSearchBackColor() const
    {
        return mainSearchBackColor_;
    }
    inline void setMainSearchBackColor( QColor color )
    {
        mainSearchBackColor_ = color;
    }

    inline QColor qfBackColor() const
    {
        return qfBackColor_;
    }
    inline void setQfBackColor( QColor color )
    {
        qfBackColor_ = color;
    }

    inline bool qfIgnoreCase() const
    {
        return qfIgnoreCase_;
    }
    inline void setQfIgnoreCase( bool ignore )
    {
        qfIgnoreCase_ = ignore;
    }

    inline std::map<std::string, QStringList> shortcuts() const
    {
        return shortcuts_;
    }
    inline void setShortcuts( const std::map<std::string, QStringList>& shortcuts )
    {
        shortcuts_ = shortcuts;
    }

    inline bool allowFollowOnScroll() const
    {
        return allowFollowOnScroll_;
    }
    inline void setAllowFollowOnScroll( bool enable )
    {
        allowFollowOnScroll_ = enable;
    }

    inline bool useTextWrap() const
    {
        return useTextWrap_;
    }
    inline void setUseTextWrap( bool enable )
    {
        useTextWrap_ = enable;
    }

    inline bool autoRunSearchOnPatternChange() const
    {
        return autoRunSearchOnPatternChange_;
    }
    inline void setAutoRunSearchOnPatternChange( bool enable )
    {
        autoRunSearchOnPatternChange_ = enable;
    }

    inline bool optimizeForNotLatinEncodings() const
    {
        return optimizeForNotLatinEncodings_;
    }
    inline void setOptimizeForNotLatinEncodings( bool enable )
    {
        optimizeForNotLatinEncodings_ = enable;
    }

    inline bool hideAnsiColorSequences() const
    {
        return hideAnsiColorSequences_;
    }
    inline void setHideAnsiColorSequences( bool hide )
    {
        hideAnsiColorSequences_ = hide;
    }

    inline int defaultEncodingMib() const
    {
        return defaultEncodingMib_;
    }
    inline void setDefaultEncodingMib( int mib )
    {
        defaultEncodingMib_ = mib;
    }

    inline std::map<QString, QString> darkPalette() const {
        return darkPalette_;
    }

    // Reads/writes the current config in the QSettings object passed
    void saveToStorage( QSettings& settings ) const;
    void retrieveFromStorage( QSettings& settings );

  private:
    // Configuration settings
    mutable QFont mainFont_ = { "DejaVu Sans Mono", 10 };
    SearchRegexpType mainRegexpType_ = SearchRegexpType::ExtendedRegexp;
    SearchRegexpType quickfindRegexpType_ = SearchRegexpType::FixedString;
    bool quickfindIncremental_ = true;

    QString language_{ "en" };

    bool nativeFileWatchEnabled_ = true;
#ifdef Q_OS_WIN
    bool pollingEnabled_ = true;
#else
    bool pollingEnabled_ = false;
#endif

    int pollIntervalMs_ = 2000;

    bool fastModificationDetection_ = false;

    bool loadLastSession_ = true;
    bool followFileOnLoad_ = false;
    bool allowMultipleWindows_ = false;

    // View settings
    bool overviewVisible_ = true;
    bool lineNumbersVisibleInMain_ = false;
    bool lineNumbersVisibleInFiltered_ = true;
    bool minimizeToTray_ = false;
    QString style_;

    // Default settings for new views
    bool searchAutoRefresh_ = false;
    bool searchIgnoreCase_ = false;
    QList<int> splitterSizes_;

    // Performance settings
    bool useSearchResultsCache_ = true;
    unsigned searchResultsCacheLines_ = 1000000;
    bool useParallelSearch_ = true;
    int indexReadBufferSizeMb_ = 16;
    int searchReadBufferSizeLines_ = 10000;
    int searchThreadPoolSize_ = 0;
    bool keepFileClosed_ = false;

    bool enableLogging_ = false;
    int loggingLevel_ = 4;

    bool enableVersionChecking_ = true;

    bool extractArchives_ = true;
    bool extractArchivesAlways_ = false;

    bool verifySslPeers_ = true;

    bool forceFontAntialiasing_ = false;
    bool enableQtHighDpi_ = true;

    int scaleFactorRounding_ = 1;

    RegexpEngine regexpEngine_ = RegexpEngine::Hyperscan;

    QColor qfBackColor_ = Qt::yellow;
    QColor mainSearchBackColor_ = Qt::lightGray;
    bool enableMainSearchHighlight_ = false;
    bool enableMainSearchHighlightVariance_ = false;

    bool allowFollowOnScroll_ = true;
    bool autoRunSearchOnPatternChange_ = false;

    bool optimizeForNotLatinEncodings_ = false;

    bool hideAnsiColorSequences_ = false;

    int defaultEncodingMib_ = -1;

    bool qfIgnoreCase_ = false;

    bool useTextWrap_ = false;

    std::map<std::string, QStringList> shortcuts_;

    // based on https://gist.github.com/QuantumCD/6245215
    std::map<QString, QString> darkPalette_ = {
        {"Window", "#353535"},
        {"WindowText", "#FFFFFF"},
        {"Base", "#282828"},
        {"AlternateBase", "#353535"},
        {"ToolTipBase", "#2a82da"},
        {"ToolTipText", "#FFFFFF"},
        {"Text", "#FFFFFF"},
        {"Button", "#353535"},
        {"ButtonText", "#FFFFFF"},
        {"Link", "#2a82da"},
        {"Highlight", "#2a82da"},
        {"HighlightedText", "#212121"},
        {"ActiveButton", "#303030"},
        {"DisabledButtonText", "#757575"},
        {"DisabledWindowText", "#808080"},
        {"DisabledText", "#808080"},
        {"DisabledLight", "#353535"},
    };
};

#endif
