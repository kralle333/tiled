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
#include <QInputDialog>

using namespace Tiled::Internal;

EnumsEditorDialog::EnumsEditorDialog(QWidget *parent) :
    QDialog(parent),
    mEnumsWereChanged(false),
    mUi(new Ui::EnumsEditorDialog)
{
    mUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(mUi->addEnumButton, SIGNAL(clicked()), SLOT(addEnumButtonClicked()));
    connect(mUi->removeEnumButton, SIGNAL(clicked()), SLOT(removeEnumButtonClicked()));
    connect(mUi->addEnumValueButton, SIGNAL(clicked()), SLOT(addEnumValueButtonClicked()));
    connect(mUi->removeEnumValueButton, SIGNAL(clicked()), SLOT(removeEnumValueButtonClicked()));
    connect(mUi->enumsList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));

}

EnumsEditorDialog::~EnumsEditorDialog()
{
    delete mUi;
}

bool EnumsEditorDialog::wereEnumsChanged()
{
    return mEnumsWereChanged;
}

QMap<QString, QStringList> EnumsEditorDialog::getEnums()
{
    return mEnums;
}

void EnumsEditorDialog::removeEnumButtonClicked()
{
    QListWidgetItem* currentItem = selectedItem();
    if(currentItem)
    {        
        mEnums.remove(currentItem->text());
        while (mUi->enumValuesList->count()>0) {
            mUi->enumValuesList->takeItem(0);
        }
        qDeleteAll(mUi->enumsList->selectedItems());
        mEnumsWereChanged = true;
    }
}
void EnumsEditorDialog::addEnumButtonClicked()
{
    bool ok;
    QString newEnum = QInputDialog::getText(this, tr("Add New Enum"), tr("Enum Name"), QLineEdit::Normal,tr("name"), &ok);

    if(ok && !newEnum.isEmpty() && mUi->enumsList->findItems(newEnum,Qt::MatchExactly).isEmpty())
    {
        mUi->enumsList->addItem(newEnum);
        mEnums[newEnum] = QStringList();
        mEnumsWereChanged = true;
    }
}

void EnumsEditorDialog::removeEnumValueButtonClicked()
{
    QListWidgetItem* currentItem = mUi->enumValuesList->currentItem();
    if (currentItem != nullptr)
    {
        mEnums[mUi->enumsList->currentItem()->text()].removeOne(currentItem->text());
        qDeleteAll(mUi->enumValuesList->selectedItems());
        mEnumsWereChanged = true;
    }
}
void EnumsEditorDialog::addEnumValueButtonClicked()
{
    bool ok;
    QString newEnumValue = QInputDialog::getText(this, tr("Add New Enum Value"), tr("Enum Value Name"), QLineEdit::Normal, tr("name"), &ok);

    if (ok && !newEnumValue.isEmpty()) {
        QString selectedEnum = selectedItemText();
        QStringList values = mEnums[selectedEnum];
        if(!values.contains(newEnumValue))
        {
            mUi->enumValuesList->addItem(newEnumValue);        
            mEnums[selectedEnum].append(newEnumValue);
            mEnumsWereChanged = true;
        }
    }
}

void EnumsEditorDialog::currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    while (mUi->enumValuesList->count()>0) 
        mUi->enumValuesList->takeItem(0);

    if(current)
    {        
        for (QString enumValue : mEnums[current->text()])
        {
            mUi->enumValuesList->addItem(enumValue);
        }
    }

}

QString EnumsEditorDialog::selectedItemText()
{
    if(mUi->enumsList->count()>0 && mUi->enumsList->currentItem() != nullptr) {
        return mUi->enumsList->currentItem()->text();
    }
    return QString();
}

QListWidgetItem* EnumsEditorDialog::selectedItem()
{
    if (mUi->enumsList->count() > 0) {
        return mUi->enumsList->currentItem();
    }
    return nullptr;
}

void EnumsEditorDialog::setEnums(QMap<QString,QStringList> enums)
{
    mEnums = enums;

    for (auto enumPairs : enums.toStdMap())
    {
        mUi->enumsList->addItem(enumPairs.first);
    }
    mEnumsWereChanged = false;
}
