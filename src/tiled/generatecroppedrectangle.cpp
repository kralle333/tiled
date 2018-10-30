#include "generatecroppedrectangle.h"
#include "tile.h"

#include <QProgressDialog>
#include <QApplication>

namespace Tiled {
namespace Internal {

//CroppedBoundsX: Iterate left to right, top down to find the first pixel where alpha!=0
//Example: Iterating from left to right we find that every column of pixels have alpha=0 until we reach x=3,
//Therefore for this tile the cropped rectangle bounds starts at x=3
GenerateCroppedRectangle::GenerateCroppedRectangle(Tileset *tileset) :
    mTileset(tileset)
{
    setText(QCoreApplication::translate("Undo Commands", "Generating Cropped Rectangles"));

    if (!tileset)
        return;

    const int length = tileset->tiles().size();

    QProgressDialog progress(QLatin1String("Generating Cropped Rectangles..."), QLatin1String("Abort Cropping"), 0, length);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.show();

    QRectF croppedBounds(0, 0, 0, 0);
    int i = 0;
    for (Tile* element : tileset->tiles()) {
        const QPixmap tilePixmap = element->image();
        QImage image = tilePixmap.toImage();

        //X
        for (int x = 0, nonAlphaPixelFound = false; x < tilePixmap.width() && !nonAlphaPixelFound; ++x) {
            for (int y = 0; y < tilePixmap.height(); ++y) {
                nonAlphaPixelFound = image.pixel(x, y) >> 24 != 0;
                if (nonAlphaPixelFound) {
                    croppedBounds.setX(x);
                    break;
                }
            }
        }
        //Width
        for (int x = tilePixmap.width() - 1, nonAlphaPixelFound = false; x >= 0 && !nonAlphaPixelFound; --x) {
            for (int y = 0; y < tilePixmap.height(); ++y) {
                nonAlphaPixelFound = image.pixel(x, y) >> 24 != 0;
                if (nonAlphaPixelFound) {
                    croppedBounds.setWidth(x - croppedBounds.x());
                    break;
                }
            }
        }
        //Y
        for (int y = 0, nonAlphaPixelFound = false; y < tilePixmap.height() && !nonAlphaPixelFound; ++y) {
            for (int x = 0; x < tilePixmap.width(); ++x) {
                nonAlphaPixelFound = image.pixel(x, y) >> 24 != 0;
                if (nonAlphaPixelFound) {
                    croppedBounds.setY(y);
                    break;
                }
            }
        }
        //Height
        for (int y = tilePixmap.height() - 1, nonAlphaPixelFound = false; y >= 0 && !nonAlphaPixelFound; --y) {
            for (int x = 0; x < tilePixmap.width(); ++x) {
                nonAlphaPixelFound = image.pixel(x, y) >> 24 != 0;
                if (nonAlphaPixelFound) {
                    croppedBounds.setHeight(y - croppedBounds.y());
                    break;
                }
            }
        }
        oldCroppedRectangles.insert(element, element->croppedRectangle());
        newCroppedRectangles.insert(element, croppedBounds);
        i++;
        QApplication::processEvents();
    }
}

void GenerateCroppedRectangle::redo()
{
    for (Tile* tile : newCroppedRectangles.keys()) {
        tile->setCroppedRectangle(newCroppedRectangles.value(tile));
    }
}

void GenerateCroppedRectangle::undo()
{
    for (Tile* tile : oldCroppedRectangles.keys()) {
        tile->setCroppedRectangle(oldCroppedRectangles.value(tile));
    }
}

}
}