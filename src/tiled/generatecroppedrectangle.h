/*
* generatecroppedrectangle.h
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
#include <QUndoCommand>
#include "tileset.h"

namespace Tiled {

namespace Internal {


class GenerateCroppedRectangle : public QUndoCommand
{
public:
    GenerateCroppedRectangle(Tileset *tileset);

    void undo() override;
    void redo() override;

private:
    Tileset *mTileset;
    QMap<Tile*, QRectF> newCroppedRectangles;
    QMap<Tile*, QRectF> oldCroppedRectangles;
};

} // namespace Internal
} // namespace Tiled
