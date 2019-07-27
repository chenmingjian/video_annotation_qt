#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#define chen_debug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

QStringList ls(QString path, QStringList filters = {"*"});

template<typename T>
void savetxt(QString savePath, T content)

{
    QFile saveFile(savePath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
     {
        chen_debug << savePath;
        QMessageBox::critical(nullptr,"错误", "保存失败", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 0);
        return;
    }
    QTextStream textStream(&saveFile);
    for (auto it = content.begin(); it != content.end(); ++it)
    {
        QString sContent = QString::number(*it);

        textStream << sContent <<"\n";
    }
    saveFile.close();
}

#endif // TOOLS_H
