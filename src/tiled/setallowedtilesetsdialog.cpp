/*
 * setallowedtilesetsdialog.cpp
 * Copyright 2018, Kristian Pilegaard Jensen <kralle@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "setallowedtilesetsdialog.h"

using namespace Tiled;

SetAllowedTilesetsDialog::SetAllowedTilesetsDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::SetAllowedTilesetsDialog)
{
    mUi->setupUi(this);

    connect(mUi->enableTilesetButton, SIGNAL(clicked()), SLOT(enableTilesets()));
    connect(mUi->enableAllTilesetsButton, SIGNAL(clicked()), SLOT(enableAllTilesets()));
    connect(mUi->disableTilesetButton, SIGNAL(clicked()), SLOT(disableTilesets()));
    connect(mUi->disableAllTilesetsButton, SIGNAL(clicked()), SLOT(disableAllTilesets()));
}
SetAllowedTilesetsDialog::~SetAllowedTilesetsDialog()
{
    delete mUi;
}
void SetAllowedTilesetsDialog::enableTilesets()
{
    for (int i = 0; i < mUi->disabledTilesetsList->selectedItems().count(); ++i)
    {
        QListWidgetItem* item = mUi->disabledTilesetsList->selectedItems().at(i);
        mUi->enabledTilesetsList->addItem(item->text());
    }
    qDeleteAll(mUi->disabledTilesetsList->selectedItems());
    mWasListChanged = true;
}
void SetAllowedTilesetsDialog::enableAllTilesets()
{
    for (int i = 0; i < mUi->disabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->disabledTilesetsList->item(i);
        mUi->enabledTilesetsList->addItem(item->text());
    }
    while (mUi->disabledTilesetsList->count()>0) {
        mUi->disabledTilesetsList->takeItem(0);
    }
    mWasListChanged = true;
}


void SetAllowedTilesetsDialog::disableTilesets()
{
    for (int i = 0; i < mUi->enabledTilesetsList->selectedItems().count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->selectedItems().at(i);
        mUi->disabledTilesetsList->addItem(item->text());
    }
    qDeleteAll(mUi->enabledTilesetsList->selectedItems());
    mWasListChanged = true;
}
void SetAllowedTilesetsDialog::disableAllTilesets()
{
    for (int i = 0; i < mUi->enabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->item(i);
        mUi->disabledTilesetsList->addItem(item->text());
    }
    while (mUi->enabledTilesetsList->count()>0) {
        mUi->enabledTilesetsList->takeItem(0);
    }
    mWasListChanged = true;
}

bool SetAllowedTilesetsDialog::wasListChanged() const
{
    return mWasListChanged;
}

QVector<QString> SetAllowedTilesetsDialog::getAllowedTilesets() const
{
    QVector<QString> allowedTilesets;

    for (int i = 0; i < mUi->enabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->item(i);
        QString str = item->text();
        allowedTilesets.append(item->text());
    }

    return allowedTilesets;
}

void SetAllowedTilesetsDialog::populateTilesetLists(const QVector<Tiled::SharedTileset> &allTilesets, const QVector < Tiled::SharedTileset > &tilesetsLockedToLayer) const
{
    for each (SharedTileset tileset in tilesetsLockedToLayer)
    {
        mUi->enabledTilesetsList->addItem(tileset->name());
    }
    for each (SharedTileset tileset in allTilesets)
    {
        if (!tilesetsLockedToLayer.contains(tileset))
        {
            mUi->disabledTilesetsList->addItem(tileset->name());
        }
    }
}
