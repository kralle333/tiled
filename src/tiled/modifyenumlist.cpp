#include "modifyenumlist.h"
#include <QCoreApplication>

namespace Tiled {


ModifyEnumList::ModifyEnumList(Tileset* tileset,
                                                const QMap<QString, QStringList>& newEnums,
                                                const QMap<QString, QStringList>& oldEnums) :
    mTileset(tileset),
    mNewEnums(newEnums),
    mOldEnums(oldEnums)
{
    setText(QCoreApplication::translate("Undo Commands", "Modify Enums"));
}

}

