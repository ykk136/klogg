/*
 * Copyright (C) 2009, 2010 Nicolas Bonnefon and other contributors
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
 * Copyright (C) 2019 Anton Filimonov and other contributors
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

#include "predefinedfiltersdialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QTimer>
#include <QToolButton>
#include <qabstractitemmodel.h>

#include "dispatch_to.h"
#include "iconloader.h"
#include "log.h"
#include "predefinedfilters.h"

PredefinedFiltersDialog::PredefinedFiltersDialog( QWidget* parent )
    : QDialog( parent )
{
    setupUi( this );

    filters_ = PredefinedFiltersCollection::getSynced().getFilters();

    populateFiltersTable();

    connect( addFilterButton, &QToolButton::clicked, this, &PredefinedFiltersDialog::addFilter );
    connect( removeFilterButton, &QToolButton::clicked, this,
             &PredefinedFiltersDialog::removeFilter );
    connect( upButton, &QToolButton::clicked, this, &PredefinedFiltersDialog::moveFilterUp );
    connect( downButton, &QToolButton::clicked, this, &PredefinedFiltersDialog::moveFilterDown );
    connect( importFilterButton, &QToolButton::clicked, this,
             &PredefinedFiltersDialog::importFilters );
    connect( exportFilterButton, &QToolButton::clicked, this,
             &PredefinedFiltersDialog::exportFilters );

    connect( buttonBox, &QDialogButtonBox::clicked, this,
             &PredefinedFiltersDialog::resolveStandardButton );

    dispatchToMainThread( [ this ] {
        IconLoader iconLoader( this );

        addFilterButton->setIcon( iconLoader.load( "icons8-plus-16" ) );
        removeFilterButton->setIcon( iconLoader.load( "icons8-minus-16" ) );
        upButton->setIcon( iconLoader.load( "icons8-up-16" ) );
        downButton->setIcon( iconLoader.load( "icons8-down-arrow-16" ) );
    } );
}

PredefinedFiltersDialog::PredefinedFiltersDialog( const QString& newFilter, QWidget* parent )
    : PredefinedFiltersDialog( parent )
{
    if ( newFilter != "" ) {
        addFilterFromSearchLine( newFilter );
    }
}

void PredefinedFiltersDialog::populateFiltersTable()
{
    filtersTableWidget->clear();

    filtersTableWidget->setRowCount( static_cast<int>( filters_.size() ) );
    filtersTableWidget->setColumnCount( 2 );

    filtersTableWidget->setHorizontalHeaderLabels( QStringList() << "Name"
                                                                 << "Pattern" );

    int filterIndex = 0;
    for ( const auto& filter : filters_ ) {
        filtersTableWidget->setItem( filterIndex, 0, new QTableWidgetItem( filter.name ) );
        filtersTableWidget->setItem( filterIndex, 1, new QTableWidgetItem( filter.pattern ) );

        filterIndex++;
    }

    filtersTableWidget->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch );
}

void PredefinedFiltersDialog::saveSettings()
{
    readFiltersTable();

    PredefinedFiltersCollection::getSynced().saveToStorage( filters_ );
}

void PredefinedFiltersDialog::readFiltersTable()
{
    const auto rows = filtersTableWidget->rowCount();

    filters_.clear();

    for ( auto i = 0; i < rows; ++i ) {
        if ( nullptr == filtersTableWidget->item( i, 0 )
             || nullptr == filtersTableWidget->item( i, 1 ) ) {
            continue;
        }

        const auto name = filtersTableWidget->item( i, 0 )->text();
        const auto value = filtersTableWidget->item( i, 1 )->text();

        if ( !name.isEmpty() && !value.isEmpty() ) {
            filters_.push_back( { name, value } );
        }
    }
}

void PredefinedFiltersDialog::addFilter()
{
    filtersTableWidget->setRowCount( filtersTableWidget->rowCount() + 1 );
}

void PredefinedFiltersDialog::addFilterFromSearchLine( const QString& newFilter )
{
    addFilter();

    const auto row = filtersTableWidget->rowCount() - 1;

    filtersTableWidget->setItem( row, 1, new QTableWidgetItem( newFilter ) );
    filtersTableWidget->setItem( row, 0, new QTableWidgetItem( "" ) );

    filtersTableWidget->scrollToItem( filtersTableWidget->item( row, 0 ) );
    filtersTableWidget->editItem( filtersTableWidget->item( row, 0 ) );
}

void PredefinedFiltersDialog::removeFilter()
{
    filtersTableWidget->removeRow( filtersTableWidget->currentRow() );
}

void PredefinedFiltersDialog::moveFilterUp()
{
    const auto* currentItem = filtersTableWidget->currentItem();

    if ( currentItem->row() > 0 ) {
        filters_.move( currentItem->row(), currentItem->row() - 1 );

        dispatchToMainThread( [ this, row = currentItem->row(), column = currentItem->column() ] {
            populateFiltersTable();
            filtersTableWidget->setCurrentItem( filtersTableWidget->item( row - 1, column ) );
        } );
    }
}

void PredefinedFiltersDialog::moveFilterDown()
{
    const auto* currentItem = filtersTableWidget->currentItem();

    if ( currentItem && currentItem->row() < filters_.size() - 1 ) {
        filters_.move( currentItem->row(), currentItem->row() + 1 );

        dispatchToMainThread( [ this, row = currentItem->row(), column = currentItem->column() ] {
            populateFiltersTable();
            filtersTableWidget->setCurrentItem( filtersTableWidget->item( row + 1, column ) );
        } );
    }
}

void PredefinedFiltersDialog::importFilters()
{
    const auto file = QFileDialog::getOpenFileName( this, "Select file to import", "",
                                                    "Predefined filters (*.conf)" );

    if ( file.isEmpty() ) {
        return;
    }

    LOG_DEBUG << "Loading predefined filters from " << file;
    QSettings settings{ file, QSettings::IniFormat };

    PredefinedFiltersCollection collection;
    collection.retrieveFromStorage( settings );

    filters_ = collection.getFilters();
    populateFiltersTable();
}

void PredefinedFiltersDialog::exportFilters()
{
    const auto file = QFileDialog::getSaveFileName( this, "Export predefined filters", "",
                                                    tr( "Predefined filters (*.conf)" ) );

    if ( file.isEmpty() ) {
        return;
    }

    QSettings settings{ file, QSettings::IniFormat };
    readFiltersTable();

    PredefinedFiltersCollection collection;
    collection.setFilters( filters_ );
    collection.saveToStorage( settings );
}

void PredefinedFiltersDialog::resolveStandardButton( QAbstractButton* button )
{
    LOG_DEBUG << "PredefinedFiltersDialog::resolveStandardButton";

    const auto role = buttonBox->buttonRole( button );

    switch ( role ) {
    case QDialogButtonBox::RejectRole:
        reject();
        return;

    case QDialogButtonBox::ApplyRole:
        saveSettings();
        break;

    case QDialogButtonBox::AcceptRole:
        saveSettings();
        accept();
        break;
    default:
        LOG_ERROR << "PredefinedFiltersDialog::resolveStandardButton unhandled role: " << role;
        return;
    }

    emit optionsChanged();
}