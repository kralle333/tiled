/*
 * modifyenumlist.h
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

#include "undocommands.h"

#include <QUndoCommand>
#include <QMap>
#include "tileset.h"

namespace Tiled {

class GroupLayer;
class Layer;

namespace Internal {

class MapDocument;

class ModifyEnumList : public QUndoCommand
{
public:

    ModifyEnumList(Tileset *tileset,
                   const QMap<QString, QStringList>& newEnums,
                   const QMap<QString, QStringList>& oldEnums);

    void undo() override
    {
        mTileset->setEnums(mOldEnums);
    }

    void redo() override
    {
        mTileset->setEnums(mNewEnums);
    }

private:
    Tileset *mTileset;
    QMap<QString, QStringList> mNewEnums;
    QMap<QString, QStringList> mOldEnums;
};
} // namespace Internal
} // namespace Tiled

