#include "tools.hpp"

QStringList ls(QString path, QStringList filters)
{
    QDir dir(path);
    // in case dir is not exist.
    if (!dir.exists())
    {
        chen_debug << "the dir \'path\' is not exists";
        return QStringList();
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    QStringList pathList;

    auto list = dir.entryInfoList();
    for(auto i : list)
    {
        QString file_path = i.filePath();
        pathList.append(file_path);
    }

    return pathList;
}


#include <QMessageBox>

