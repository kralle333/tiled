/*
* newmapdialog.cpp
* Copyright 2009-2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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

using namespace Tiled;
using namespace Tiled::Internal;


NewMapFromTemplateDialog::NewMapFromTemplateDialog(QWidget *parent) :
	QDialog(parent),
	mUi(new Ui::NewMapFromTemplateDialog)
{
	mUi->setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	mUi->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save As..."));
	connect(mUi->browseButton, SIGNAL(clicked()), SLOT(openFile()));
	connect(mUi->overrideSize, SIGNAL(toggled(bool)), SLOT(updateWidgets(bool)));
	updateWidgets(false);
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

	auto preferences = Preferences::instance();
	QString f = QFileDialog::getOpenFileName(this, tr("Map File Template"), preferences->fileDialogStartLocation(), filter);

	if (!f.isEmpty()) {
		if (f != mPath)
		{
			mUi->mappath->setText(f);
			mPath = f;
			mMap = tmxp.read(f);
			mUi->mapWidth->setValue(mMap->width());
			mUi->mapHeight->setValue(mMap->height());
		}
	}
}

void NewMapFromTemplateDialog::eraseLayerContents(const QList<Layer*> &layers)
{
	foreach(Layer *layer, layers)
	{
		if (mUi->objectsClearBox->isChecked() && layer->isObjectGroup())
		{
			ObjectGroup *a = layer->asObjectGroup();
			while (!a->isEmpty())
			{
				a->removeObjectAt(0);
			}
		}
		else if (mUi->tilesClearBox->isChecked() && layer->isTileLayer())
		{
			TileLayer *a = layer->asTileLayer();
			a->erase(QRegion(0, 0, a->width(), a->height()));
		}
		else if (mUi->groupsClearBox->isChecked() && layer->isGroupLayer())
		{
			eraseLayerContents((layer->asGroupLayer()->layers()));
		}
	}

}

bool NewMapFromTemplateDialog::openFile(const QString &fileName, FileFormat *fileFormat)
{
	/*if (fileName.isEmpty())
		return false;


	if (!fileFormat) {
		// Try to find a plugin that implements support for this format
		const auto formats = PluginManager::objects<FileFormat>();
		for (FileFormat *format : formats) {
			if (format->supportsFile(fileName)) {
				fileFormat = format;
				break;
			}
		}
	}

	if (!fileFormat) {
		QMessageBox::critical(this, tr("Error Opening File"), tr("Unrecognized file format."));
		return false;
	}

	QString error;
	Document *document = nullptr;

	if (MapFormat *mapFormat = qobject_cast<MapFormat*>(fileFormat)) {
		document = MapDocument::load(fileName, mapFormat, &error);
	}
	else if (TilesetFormat *tilesetFormat = qobject_cast<TilesetFormat*>(fileFormat)) {
		// It could be, that we have already loaded this tileset while loading some map.
		if (TilesetDocument *tilesetDocument = mDocumentManager->findTilesetDocument(fileName)) {
			document = tilesetDocument;
		}
		else {
			document = TilesetDocument::load(fileName, tilesetFormat, &error);
		}
	}

	if (!document) {
		QMessageBox::critical(this, tr("Error Opening File"), error);
		return false;
	}

	mDocumentManager->addDocument(document);

	if (MapDocument *mapDocument = qobject_cast<MapDocument*>(document))
		mDocumentManager->checkTilesetColumns(mapDocument);

	Preferences::instance()->addRecentFile(fileName);*/
	return true;
}

MapDocument *NewMapFromTemplateDialog::createMap()
{
	if (exec() != QDialog::Accepted)
		return nullptr;

	if (mMap == nullptr)
	{
		QMessageBox::warning(this, tr("Error"),
			tr("No Map Template selected!"));
		return nullptr;
	}

	MapDocument *newDocument = new MapDocument(mMap);

	QSize newSize(mUi->mapWidth->value(), mUi->mapHeight->value());
    eraseLayerContents(newDocument->map()->layers());
	QPoint offset;
	newDocument->resizeMap(newSize, offset, false);
	return newDocument;
}

void NewMapFromTemplateDialog::updateWidgets(bool checked)
{
	mUi->mapHeight->setEnabled(checked);
	mUi->mapWidth->setEnabled(checked);
	mUi->heightLabel->setEnabled(checked);
	mUi->widthLabel->setEnabled(checked);
}