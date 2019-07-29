#ifndef SIMPLEPLAYER_H_
#define SIMPLEPLAYER_H_

#include <QMainWindow>
#include <QSet>
namespace Ui {
    class SimplePlayer;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class SimplePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();

protected:
    void flash(int ms);

private slots:
    void openLocal();

    void on_actionOpen_dir_triggered();

    void on_next_clicked();

    void on_play_clicked();

    void on_undo_clicked();

    void on_replay_clicked();

    void on_action1_clicked();

    void on_action2_clicked();

    void on_action3_clicked();

    void on_action4_clicked();

    void on_action5_clicked();

    void on_action6_clicked();

    void on_action7_clicked();

    void on_action8_clicked();

private:
    Ui::SimplePlayer *ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

private:
    void keyPressEvent(QKeyEvent *event);
    bool isPlaying = false;
    bool isOnOpenDir;
    bool isPressing = false;
    QStringList currentVideoPath;
    int currentVideoIndex;
    int currentActionIndex;
    bool actionBegin;
    void setVideo(QString filePath);
    void nextVideo();
    bool eventFilter(QObject *watched, QEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void paint();
    QVector<float> xs;
    QVector<float> positions;
    QVector<int> actionTypes;
    QVector<QColor> colors;
    void reset();
    void writeToFile();
    void setAllEnable();
    void writPostion();
};

#endif // SIMPLEPLAYER_H_
