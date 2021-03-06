/*
 * mapobjectitem.cpp
 * Copyright 2008, Roderic Morris <roderic@ccs.neu.edu>
 * Copyright 2008-2011, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2010, Jeff Bland <jksb@member.fsf.org>
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

#include "mapobjectitem.h"

#include "mapdocument.h"
#include "mapobject.h"
#include "maprenderer.h"
#include "mapscene.h"
#include "mapview.h"
#include "objectgroup.h"
#include "objectgroupitem.h"
#include "preferences.h"
#include "tile.h"
#include "zoomable.h"

#include <QPainter>

using namespace Tiled;
using namespace Tiled::Internal;

MapObjectItem::MapObjectItem(MapObject *object, MapDocument *mapDocument,
                             ObjectGroupItem *parent):
    QGraphicsItem(parent),
    mObject(object),
    mMapDocument(mapDocument)
{
    setAcceptedMouseButtons(Qt::MouseButtons());
    syncWithMapObject();
}

void MapObjectItem::syncWithMapObject()
{
    const QColor color = objectColor(mObject);

    // Update the whole object when the name, polygon or color has changed
    if (mName != mObject->name() || mPolygon != mObject->polygon() || mColor != color) {
        mName = mObject->name();
        mPolygon = mObject->polygon();
        mColor = color;
        update();
    }

    QString toolTip = mName;
    const QString &type = mObject->type();
    if (!type.isEmpty())
        toolTip += QLatin1String(" (") + type + QLatin1String(")");
    setToolTip(toolTip);

    MapRenderer *renderer = mMapDocument->renderer();
    const QPointF pixelPos = renderer->pixelToScreenCoords(mObject->position());
    QRectF bounds = renderer->boundingRect(mObject);

    bounds.translate(-pixelPos);

    setPos(pixelPos);
    setRotation(mObject->rotation());

    if (ObjectGroup *objectGroup = mObject->objectGroup())
        if (objectGroup->drawOrder() == ObjectGroup::TopDownOrder)
            setZValue(pixelPos.y());

    if (mBoundingRect != bounds) {
        // Notify the graphics scene about the geometry change in advance
        prepareGeometryChange();
        mBoundingRect = bounds;
    }

    setVisible(mObject->isVisible());
}

QRectF MapObjectItem::boundingRect() const
{
    return mBoundingRect;
}

QPainterPath MapObjectItem::shape() const
{
    QPainterPath path = mMapDocument->renderer()->shape(mObject);
    path.translate(-pos());
    return path;
}

void MapObjectItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *,
                          QWidget *widget)
{
    qreal scale = static_cast<MapView*>(widget->parent())->zoomable()->scale();
    painter->translate(-pos());
    mMapDocument->renderer()->setPainterScale(scale);
    mMapDocument->renderer()->drawMapObject(painter, mObject, mColor);
}

void MapObjectItem::resizeObject(const QRectF &bounds)
{
    // Not using the MapObjectModel because it is used during object creation,
    // when the object is not actually part of the map yet.
    mObject->setBounds(bounds);
    syncWithMapObject();
}

void MapObjectItem::setPolygon(const QPolygonF &polygon)
{
    // Not using the MapObjectModel because it is used during object creation,
    // when the object is not actually part of the map yet.
    mObject->setPolygon(polygon);
    syncWithMapObject();
}

QColor MapObjectItem::objectColor(const MapObject *object)
{
    const QString effectiveType = object->effectiveType();

    if(object->shape() == MapObject::Rectangle || object->shape() == MapObject::Polygon)
    {        
        if (effectiveType == tr("trigger")) {
            return QColor(0x8094FF);
        }
        if (effectiveType == tr("walkable")) {
            return QColor(0x80FF8E);
        }
        if (effectiveType == tr("")) {
            return QColor(0xFF9f9f);
        }
        if (effectiveType == tr("respawnarea")) {
            return QColor(0xCD80FF);
        }

        if (effectiveType == tr("encounter")) {
            return QColor(0xFF1010);
        }
    }

    // See if this object type has a color associated with it
    for (const ObjectType &type : Object::objectTypes()) {
        if (type.name.compare(effectiveType, Qt::CaseInsensitive) == 0)
            return type.color;
    }

    // If not, get color from object group
    const ObjectGroup *objectGroup = object->objectGroup();
    if (objectGroup && objectGroup->color().isValid())
        return objectGroup->color();

    // Fallback color
    return Qt::gray;
}

bool MapObjectItem::isAlphaZeroAt(const QPointF& pos) const
{
    if (mObject->cell().tile() == nullptr ||
        mObject->cell().tile()->imageSource().isEmpty())
    {
        return true;
    }
    //Only return object if object's alpha value is not 0 at the given position
    QPixmap pixmap = mObject->cell().tile()->image();
    float scaleX = pixmap.width() / mObject->width();
    float scaleY = pixmap.height() / mObject->height();
    QPointF imageLocalPosition = sceneTransform().inverted().map(pos);
    int x = imageLocalPosition.x() * scaleX;
    int y = (mObject->height() - -imageLocalPosition.y()) * scaleY;

    QImage pixMapImage = pixmap.toImage();
    if (mObject->cell().flippedHorizontally())
    {
        pixMapImage = pixMapImage.mirrored(true, false);
    }
    if (mObject->cell().flippedVertically())
    {
        pixMapImage = pixMapImage.mirrored(false, true);
    }

    return pixMapImage.pixel(x, y) >> 24 == 0;
}
