/*
 * setallowedtilesetsdialog.cpp
 * Copyright 2008-2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

using namespace Tiled::Internal;

SetAllowedTilesetsDialog::SetAllowedTilesetsDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::SetAllowedTilesetsDialog)
{
    mUi->setupUi(this);
/*
    connect(enableTilesetButton, SIGNAL(clicked()), SLOT(enableTilesets()));
    connect(enableAllTilesetsButton, SIGNAL(clicked()), SLOT(enableAllTilesets()));
    connect(disableTilesetButton, SIGNAL(clicked()), SLOT(disableTilesets()));
    connect(disableAllTilesetsButton, SIGNAL(clicked()), SLOT(disableAllTilesets()));*/
}
SetAllowedTilesetsDialog::~SetAllowedTilesetsDialog()
{
    delete mUi;
}
void SetAllowedTilesetsDialog::enableTilesets()
{
    QVector<QListWidgetItem*> itemsToRemove;
    for (int i = 0; i < mUi->disabledTilesetsList->selectedItems().count(); ++i)
    {
        QListWidgetItem* item = mUi->disabledTilesetsList->selectedItems().at(i);
        mUi->enabledTilesetsList->addItem(item->text());
        itemsToRemove.append(item);
    }

    for each (QListWidgetItem *item in itemsToRemove)
    {
        mUi->disabledTilesetsList->removeItemWidget(item);
    }
}
void SetAllowedTilesetsDialog::enableAllTilesets()
{
    for (int i = 0; i < mUi->disabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->disabledTilesetsList->item(i);
        mUi->enabledTilesetsList->addItem(item->text());
    }
    mUi->disabledTilesetsList->clear();
}


void SetAllowedTilesetsDialog::disableTilesets()
{
    QVector<QListWidgetItem*> itemsToRemove;
    for (int i = 0; i < mUi->enabledTilesetsList->selectedItems().count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->selectedItems().at(i);
        mUi->disabledTilesetsList->addItem(item->text());
        itemsToRemove.append(item);
    }

    for each (QListWidgetItem *item in itemsToRemove)
    {
        mUi->enabledTilesetsList->removeItemWidget(item);
    }
}
void SetAllowedTilesetsDialog::disableAllTilesets()
{
    for (int i = 0; i < mUi->enabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->item(i);
        mUi->disabledTilesetsList->addItem(item->text());
    }
    mUi->enabledTilesetsList->clear();
}

QVector<QString> SetAllowedTilesetsDialog::getAllowedTilesets()
{
    QVector<QString> allowedTilesets;

    for (int i = 0; i < mUi->enabledTilesetsList->count(); ++i)
    {
        QListWidgetItem* item = mUi->enabledTilesetsList->item(i);
        allowedTilesets.append(item->text());
    }

    return allowedTilesets;
}

void SetAllowedTilesetsDialog::populateTilesetLists(const QVector<Tiled::SharedTileset> allTilesets, const QVector<Tiled::SharedTileset> tilesetsLockedToLayer)
{
    for each (SharedTileset tileset in tilesetsLockedToLayer)
    {
        mUi->enabledTilesetsList->addItem(tileset->name());
    }
    for each (SharedTileset tileset in allTilesets)
    {
        if (!tilesetsLockedToLayer.contains(tileset))
        {
            mUi->enabledTilesetsList->addItem(tileset->name());
        }
    }
}