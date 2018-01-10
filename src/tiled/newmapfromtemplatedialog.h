/*
* newmapdialog.h
* Copyright 2009, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
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
#include "map.h"

namespace Ui {
	class NewMapFromTemplateDialog;
}

namespace Tiled {
	class FileFormat;
	namespace Internal {

		class MapDocument;

		/**
		* A dialog for the creation of a new map.
		*/
		class NewMapFromTemplateDialog : public QDialog
		{
			Q_OBJECT

		public:
			NewMapFromTemplateDialog(QWidget *parent = nullptr);
			virtual ~NewMapFromTemplateDialog();
			/**
			* Shows the dialog and returns the created map. Returns null if the dialog
			* was cancelled.
			*/
			MapDocument *createMap();

			private slots:
			void eraseLayerContents(const QList<Layer*> &layers);
			void openFile();
			bool openFile(const QString &fileName, FileFormat *fileFormat);

			void updateWidgets(bool checked);

		private:
			QString mPath;
			Ui::NewMapFromTemplateDialog *mUi;
			Map *mMap = nullptr;
		};

	} // namespace Internal
} // namespace Tiled
