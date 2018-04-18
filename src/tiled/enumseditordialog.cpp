/*
* enumseditordialog.cpp
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

#include "enumseditordialog.h"

using namespace Tiled::Internal;

EnumsEditorDialog::EnumsEditorDialog(QWidget *parent) :
    QDialog(parent),
    mUi(new Ui::EnumsEditorDialog)
{
    mUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    
}


void EnumsEditorDialog::ShowEnums(QMap<QString,QStringList> enums)
{
    mEnums = enums;

    for (auto enumPairs : enums.toStdMap())
    {
        mUi->enumsList->addItem(enumPairs.first);
    }
}