/*
 * newmapfromtemplatedialog.cpp
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


#include "newmapfromtemplatedialog.h"
#include "ui_newmapfromtemplatedialog.h"
#include "mapformat.h"
#include "mapdocument.h"
#include "objectgroup.h"
#include "grouplayer.h"
#include "preferences.h"
#include "tmxmapformat.h"

#include <QFileDialog>
#include <QMessageBox>
#include "documentmanager.h"

using namespace Tiled;
using namespace Tiled::Internal;


NewMapFromTemplateDialog::NewMapFromTemplateDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::NewMapFromTemplateDialog)
{
    mUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(mUi->browseButton, SIGNAL(clicked()), SLOT(openFile()));
    connect(mUi->overrideSize, SIGNAL(toggled(bool)), SLOT(updateMapSizeWidgets(bool)));
    connect(mUi->openedFileRadioButton, SIGNAL(toggled(bool)), SLOT(updateMapFileWidgets(bool)));
    connect(mUi->openfilesComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(comboBoxIndexChanged(QString)));
    updateMapSizeWidgets(false);
    updateMapFileWidgets(true);

    for (auto document : DocumentManager::instance()->documents()) {
        if (document->type() == Document::MapDocumentType)
            mUi->openfilesComboBox->addItem(document->displayName());
    }
}

NewMapFromTemplateDialog::~NewMapFromTemplateDialog()
{
    delete mUi;
}


void NewMapFromTemplateDialog::openFile()
{
    TmxMapFormat tmxp;
    QString filter = tmxp.nameFilter();
    FormatHelper<FileFormat> helper(FileFormat::Read, filter);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Map File Template"),
                                                    Preferences::instance()->fileDialogStartLocation(), filter);



    if (!fileName.isEmpty()) {
        if (fileName != mPath) {
            mUi->mappath->setText(fileName);
            mPath = fileName;
            mMap = tmxp.read(fileName);
            mUi->mapWidth->setValue(mMap->width());
            mUi->mapHeight->setValue(mMap->height());
        }
    }
}

void NewMapFromTemplateDialog::eraseLayerContents(const QList<Layer*> &layers)
{
    for (auto layerItr = layers.begin(); layerItr != layers.end(); ++layerItr) {
        Layer *layer = *layerItr;
        if (mUi->objectsClearBox->isChecked() && layer->isObjectGroup()) {
            ObjectGroup *a = layer->asObjectGroup();
            while (!a->isEmpty()) {
                a->removeObjectAt(0);
            }
        } else if (mUi->tilesClearBox->isChecked() && layer->isTileLayer()) {
            TileLayer *a = layer->asTileLayer();
            a->erase(QRegion(0, 0, a->width(), a->height()));
        } else if (mUi->groupsClearBox->isChecked() && layer->isGroupLayer()) {
            eraseLayerContents((layer->asGroupLayer()->layers()));
        }
    }

}

void NewMapFromTemplateDialog::comboBoxIndexChanged(const QString &text)
{
    DocumentPtr selectedDocument = nullptr;
    for (DocumentPtr const document : DocumentManager::instance()->documents()) {
        if (document->displayName() == text) {
            selectedDocument = document;
            break;
        }
    }
    Q_ASSERT(selectedDocument != nullptr);

    const QString fileName = selectedDocument->fileName();
    if (fileName != mPath) {
        TmxMapFormat tmxp;
        mUi->mappath->setText(fileName);
        mPath = fileName;
        mMap = tmxp.read(fileName);
        if (mMap != nullptr) {
            mUi->mapWidth->setValue(mMap->width());
            mUi->mapHeight->setValue(mMap->height());
        }
    }
}

MapDocumentPtr NewMapFromTemplateDialog::createMap()
{
    if (exec() != QDialog::Accepted)
        return nullptr;

    if (mMap == nullptr) {
        QMessageBox::warning(this, tr("Error"),
                             tr("No Map Template selected!"));
        return nullptr;
    }

    MapDocument *newDocument = new MapDocument(mMap);

    QSize newSize(mUi->mapWidth->value(), mUi->mapHeight->value());
    eraseLayerContents(newDocument->map()->layers());
    QPoint offset;
    newDocument->resizeMap(newSize, offset, false);
    return MapDocumentPtr::create(mMap);
}
void NewMapFromTemplateDialog::updateMapFileWidgets(bool checked)
{
    mUi->browseButton->setEnabled(!checked);
    mUi->mappath->setEnabled(!checked);
    mUi->openfilesComboBox->setEnabled(checked);
}

void NewMapFromTemplateDialog::updateMapSizeWidgets(bool checked)
{
    mUi->mapHeight->setEnabled(checked);
    mUi->mapWidth->setEnabled(checked);
    mUi->heightLabel->setEnabled(checked);
    mUi->widthLabel->setEnabled(checked);
}