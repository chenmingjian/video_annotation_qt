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
#include <QtTest/QTest>

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
    QTest::qSleep(ms);
    this->setWindowOpacity(1.0);
    _player->play();
}


void SimplePlayer::keyPressEvent(QKeyEvent *event)
{
//    if(event->key() == Qt::Key_Control && this->isPlaying)
    if(event->key() == Qt::Key_Control)
    {
        isPressing = true;
        qDebug() << _player->time() << _player->position();
//        this->flash(500);
        auto position = this->_player->position();
        this->xs << (ui->seek->width() - 90) * position + 45;
        this->positions << position;
        ui->seek->update();
        this->writeToFile();

    }
    else if(event->key() == Qt::Key_U)
    {
        this->on_undo_clicked();

    }
    if (this->xs.isEmpty())
        ui->undo->setEnabled(false);
    else
        ui->undo->setEnabled(true);
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
    QString save_path = "./output_txt/";
    QDir dir(save_path);
    if(!dir.exists())
        dir.mkpath(save_path);

    QFileInfo fi(this->currentVideoPath.at(this->currentVideoIndex));
    auto baseName = fi.baseName();
    save_path += baseName + ".txt";
    savetxt<QVector<float>>(save_path, this->positions);
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
    }
    else
    {
        this->_player->play();
        this->isPlaying = true;
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
    QPen pen(Qt::blue);
    pen.setWidth(2);
    painter.setPen(pen);
    for (auto x: this->xs)
        painter.drawLine(QLineF(QPointF(x, 0), QPointF(x, 200)));

    if(!this->xs.isEmpty())
    {
        pen.setColor(Qt::red);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawLine(QLineF(QPointF(this->xs.back(), 0), QPointF(this->xs.back(), 200)));
    }
}


void SimplePlayer::on_undo_clicked()
{
    if(!this->xs.isEmpty())
    {
        this->xs.pop_back();
        ui->seek->update();

    }
    if(this->xs.isEmpty())
        ui->undo->setEnabled(false);
    else
        ui->undo->setEnabled(true);
}


void SimplePlayer::reset()
{
    this->xs.clear();
    ui->seek->update();
}
