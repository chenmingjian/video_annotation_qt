#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#define chen_debug qDebug()<<"[FILE:"<<__FILE__<<",LINE"<<__LINE__<<",FUNC"<<__FUNCTION__<<"]"

QStringList ls(QString path, QStringList filters = {"*"});

template<typename T, typename T_2>
void savetxt(QString savePath, T content, T_2 content_2)

{
    QFile saveFile(savePath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
     {
        chen_debug << savePath;
        QMessageBox::critical(nullptr,"错误", "保存失败", QMessageBox::Ok | QMessageBox::Default , QMessageBox::Cancel | QMessageBox::Escape , 0);
        return;
    }
    QTextStream textStream(&saveFile);
    for (int i = 0; i < content.length(); ++i)
    {
        QString sContent = QString::number(content.at(i));
        sContent += " ";
        sContent += QString::number(content_2.at(i));
        textStream << sContent <<"\n";
    }
    saveFile.close();
}

#endif // TOOLS_H
