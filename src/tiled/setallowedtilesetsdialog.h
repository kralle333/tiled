/*
 * setallowedtilesetsdialog.h
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

#pragma once

#include <QDialog>
#include "ui_setallowedtilesetsdialog.h"
#include "layer.h"

namespace Ui {
class SetAllowedTilesetsDialog;
}

namespace Tiled {
namespace Internal {

class SetAllowedTilesetsDialog : public QDialog
{
    Q_OBJECT

public:
    SetAllowedTilesetsDialog(QWidget *parent = nullptr);
    ~SetAllowedTilesetsDialog();

    inline bool wasListChanged() { return mWasListChanged; }
    QVector<QString> getAllowedTilesets();
    void populateTilesetLists(const QVector<SharedTileset> allTileSets, const QVector<SharedTileset> tilesetsLockedToLayer);

private slots:
    void enableTilesets();
    void enableAllTilesets();
    void disableTilesets();
    void disableAllTilesets();

private:
    Ui::SetAllowedTilesetsDialog *mUi;
    bool mWasListChanged = false;
};

} // namespace Internal
} // namespace Tiled

