/*
* createtriggerboxobjecttool.cpp
* Copyright 2008-2009, Kristian Pilegaard Jensen <kralle@gmail.com>
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


#include "createtriggerboxobjecttool.h"
#include "changeproperties.h"

#include "mapobject.h"
#include "utils.h"

using namespace Tiled;
using namespace Tiled::Internal;

CreateTriggerBoxObjectTool::CreateTriggerBoxObjectTool(QObject *parent)
    : CreateScalableObjectTool(parent)
{
    QIcon icon(QLatin1String(":images/24x24/insert-trigger-box.png"));
    icon.addFile(QLatin1String(":images/48x48/insert-trigger-box.png"));
    setIcon(icon);
    Utils::setThemeIcon(this, "insert-rectangle");
    languageChangedImpl();
}

void CreateTriggerBoxObjectTool::languageChanged()
{
    CreateScalableObjectTool::languageChanged();
    languageChangedImpl();
}

void CreateTriggerBoxObjectTool::languageChangedImpl()
{
    setName(tr("Insert Trigger box"));
    setShortcut(QKeySequence(tr("B")));
}

MapObject *CreateTriggerBoxObjectTool::createNewMapObject()
{
    MapObject *newMapObject = new MapObject;
    newMapObject->setShape(MapObject::Rectangle);
    newMapObject->setType(tr("trigger"));

    newMapObject->setProperty(tr("condition"), tr("[\n]"));
    newMapObject->setProperty(tr("effect"), tr("[\n]"));

    return newMapObject;
}
