#include <QFileDialog>
#include <QInputDialog>
#include <QPainter>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>

#include <tools.hpp>

SimplePlayer::SimplePlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimplePlayer),
      _media(nullptr),
      currentVideoIndex(0)
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);

    ui->video->setMediaPlayer(_player);
    ui->seek->setMediaPlayer(_player);

    connect(ui->actionOpenLocal, &QAction::triggered, this, &SimplePlayer::openLocal);
    connect(ui->actionPause, &QAction::toggled, _player, &VlcMediaPlayer::togglePause);
    connect(ui->actionStop, &QAction::triggered, _player, &VlcMediaPlayer::stop);

    ui->next->setEnabled(false);

    ui->seek->installEventFilter(this);

    ui->undo->setEnabled(false);

    ui->play->setEnabled(false);

    ui->replay->setEnabled(false);

    QPixmap p(16,16);

    colors << QColor(255, 0, 0)
           << QColor(255, 165, 0)
           << QColor(255, 255, 0)
           << QColor(0, 255, 0)
           << QColor(0, 127, 255)
           << QColor(0, 0, 255)
           << QColor(139, 0, 255)
           << QColor(0, 0, 0);

    p.fill(QColor(255, 0, 0));
    ui->action1->setIcon(QIcon(p));
    p.fill(QColor(255, 165, 0));
    ui->action2->setIcon(QIcon(p));
    p.fill(QColor(255, 255, 0));
    ui->action3->setIcon(QIcon(p));
    p.fill(QColor(0, 255, 0));
    ui->action4->setIcon(QIcon(p));
    p.fill(QColor(0, 127, 255));
    ui->action5->setIcon(QIcon(p));
    p.fill(QColor(0, 0, 255));
    ui->action6->setIcon(QIcon(p));
    p.fill(QColor(139, 0, 255));
    ui->action7->setIcon(QIcon(p));
    p.fill(QColor(0, 0, 0));
    ui->action8->setIcon(QIcon(p));

    this->currentActionIndex = 1;
    ui->action1->setEnabled(false);
}

SimplePlayer::~SimplePlayer()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}



void SimplePlayer::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;
    this->currentVideoPath << file;
    this->setVideo(file);

}

void SimplePlayer::setVideo(QString filePath)
{
    this->_media = new VlcMedia(filePath, true, this->_instance);

    this->_player->open(_media);

    this->isPlaying = true;
    ui->play->setEnabled(true);
    ui->replay->setEnabled(true);
    reset();
}

void SimplePlayer::flash(int ms)
{
    if(ms < 0)
    {
        return;
    }
    _player->pause();
    this->setWindowOpacity(0.8);
    this->setWindowOpacity(1.0);
    _player->play();
}

void SimplePlayer::writPostion()
{
    auto position = this->_player->position();
    this->xs << (ui->seek->width() - 90) * position + 45;
    this->positions << position;
    this->actionTypes << this->currentActionIndex;
    ui->seek->update();
    this->writeToFile();
}

void SimplePlayer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control && this->isPlaying)
    {
        isPressing = true;
        qDebug() << _player->time() << _player->position();
        writPostion();

    }
    else if(event->key() == Qt::Key_Backspace)
    {
        this->on_undo_clicked();

    }
    if (this->xs.isEmpty())
        ui->undo->setEnabled(false);
    else
        ui->undo->setEnabled(true);

    switch (event->key()) {
        case Qt::Key_1: on_action1_clicked();writPostion();break;
        case Qt::Key_2: on_action2_clicked();writPostion();break;
        case Qt::Key_3: on_action3_clicked();writPostion();break;
        case Qt::Key_4: on_action4_clicked();writPostion();break;
        case Qt::Key_5: on_action5_clicked();writPostion();break;
        case Qt::Key_6: on_action6_clicked();writPostion();break;
        case Qt::Key_7: on_action7_clicked();writPostion();break;
        case Qt::Key_8: on_action8_clicked();writPostion();break;


    }
}

void SimplePlayer::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        isPressing = false;
    }
}

void SimplePlayer::writeToFile()
{
    QString save_dir_path = "./output_txt/";
    QFileInfo save_path_info(save_dir_path);
    QDir dir(save_path_info.absoluteFilePath());
    if(!dir.exists())
        dir.mkdir(dir.path());

    QFileInfo fi(this->currentVideoPath.at(this->currentVideoIndex));
    auto baseName = fi.baseName();
    auto save_path = save_dir_path + baseName + ".txt";
    savetxt<QVector<float>, QVector<int>>(save_path, this->positions, this->actionTypes);
}

void SimplePlayer::on_actionOpen_dir_triggered()
{
    auto dir_path = QFileDialog::getExistingDirectory(this, "Open dir", "./");
//    auto dir_path = "C:/Users/chen/Desktop/test";
    this->currentVideoPath = ls(dir_path);
    this->currentVideoIndex = 0;

    chen_debug << this->currentVideoPath;

    this->setVideo(this->currentVideoPath.at(this->currentVideoIndex));
    this->isOnOpenDir = true;
    ui->next->setEnabled(true);
}

void SimplePlayer::nextVideo()
{

    if(this->currentVideoIndex + 1 >= this->currentVideoPath.length())
        return;
    else
        ++this->currentVideoIndex;

    if(this->currentVideoIndex + 1 >= this->currentVideoPath.length())
        ui->next->setEnabled(false);
    else
        ui->next->setEnabled(true);

    this->setVideo(this->currentVideoPath.at(this->currentVideoIndex));

}

void SimplePlayer::on_next_clicked()
{
    this->writeToFile();
    if(this->isOnOpenDir)
    {
        this->nextVideo();
    }
}

void SimplePlayer::on_play_clicked()
{
    if(isPlaying)
     {
        this->_player->pause();
        this->isPlaying = false;
        ui->play->setText("play");
    }
    else
    {
        this->_player->play();
        this->isPlaying = true;
        ui->play->setText("pause");
    }
}

bool SimplePlayer::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->seek && event->type() == QEvent::Paint)
    {
        paint();
    }
    return QWidget::eventFilter(watched,event);
}


void SimplePlayer::paint()
{
    QPainter painter(ui->seek);
    QColor color;

    QPen pen;
    pen.setStyle(Qt::SolidLine);

    auto even = (xs.length() / 2) == xs.length();
    int drawLength;
    if(even)
        drawLength = xs.length();
    else
        drawLength = xs.length() - 1;

    for(int i = 0; i < drawLength; ++i)
    {
        color = Qt::black;
        pen.setColor(color);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(color);
        painter.drawLine(QLineF(QPointF(this->xs.at(i), 0), QPointF(this->xs.at(i), 200)));
        if(i >= 1 && i+1 < drawLength && this->actionTypes.at(i+1) == this->actionTypes.at(i+2))
        {
            continue;
        }
        else
        {
            auto action = this->actionTypes.at(i+1) - 1;
            color = this->colors.at(action);
            pen.setColor(color);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.setBrush(color);

            auto x = this->xs.at(i);
            auto x_end = this->xs.at(i+1);
            chen_debug << this->xs;
            chen_debug << this->actionTypes;
            painter.drawRect(x, 10, x_end - x, 20);
        }
    }


    if(!this->xs.isEmpty())
    {
        pen.setColor(Qt::red);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(QLineF(QPointF(this->xs.back(), 0), QPointF(this->xs.back(), 200)));
    }
}


void SimplePlayer::on_undo_clicked()
{
    if(!this->xs.isEmpty())
    {
        this->xs.pop_back();
        this->positions.pop_back();
        this->actionTypes.pop_back();
        ui->seek->update();
        this->writeToFile();
    }
    if(this->xs.isEmpty())
        ui->undo->setEnabled(false);
    else
        ui->undo->setEnabled(true);
}


void SimplePlayer::reset()
{
    this->xs.clear();
    this->positions.clear();
    this->actionTypes.clear();
    ui->seek->update();
}

void SimplePlayer::on_replay_clicked()
{
    chen_debug << this->currentVideoPath.length() <<  this->currentVideoIndex;
    if(this->currentVideoPath.length() > this->currentVideoIndex)
    {
        this->setVideo(this->currentVideoPath.at(this->currentVideoIndex));
    }
}

void SimplePlayer::setAllEnable()
{
    ui->action1->setEnabled(true);
    ui->action2->setEnabled(true);
    ui->action3->setEnabled(true);
    ui->action4->setEnabled(true);
    ui->action5->setEnabled(true);
    ui->action6->setEnabled(true);
    ui->action7->setEnabled(true);
    ui->action8->setEnabled(true);
}

void SimplePlayer::on_action1_clicked()
{
    setAllEnable();
    ui->action1->setEnabled(false);
    currentActionIndex = 1;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action2_clicked()
{
    setAllEnable();
    ui->action2->setEnabled(false);
    currentActionIndex = 2;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action3_clicked()
{
    setAllEnable();
    ui->action3->setEnabled(false);
    currentActionIndex = 3;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action4_clicked()
{
    setAllEnable();
    ui->action4->setEnabled(false);
    currentActionIndex = 4;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action5_clicked()
{
    setAllEnable();
    ui->action5->setEnabled(false);
    currentActionIndex = 5;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action6_clicked()
{
    setAllEnable();
    ui->action6->setEnabled(false);
    currentActionIndex = 6;
    if(actionBegin == true)
        actionBegin = false;
    else
        actionBegin = true;
}

void SimplePlayer::on_action7_clicked()
{
   setAllEnable();
   ui->action7->setEnabled(false);
   currentActionIndex = 7;
   if(actionBegin == true)
       actionBegin = false;
   else
       actionBegin = true;
}

void SimplePlayer::on_action8_clicked()
{
   setAllEnable();
   ui->action8->setEnabled(false);
   currentActionIndex = 8;
   if(actionBegin == true)
       actionBegin = false;
   else
       actionBegin = true;
}
