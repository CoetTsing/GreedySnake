#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QQueue>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void go();
    void tailDrop();
    int egg();
    void save();
    void load();

private slots:
    void on_Start_clicked();

    void on_Reset_clicked();

    void on_Pause_clicked();

    void on_Exit_clicked();

    void on_actionStart_triggered();

    void on_actionReset_triggered();

    void on_actionPause_triggered();

    void on_actionExit_triggered();

    void on_Save_clicked();

    void on_Load_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_speed_valueChanged(int value);

    void timeToGo();

private:
    Ui::MainWindow *ui;
    int nodes[1764];
    QList<int> snake;
    QQueue<int> keys;
    int state;
    int step;
    int score;
    int direction;
    int interval;
    int eating;
    bool eggExist;
    bool decided = true;
    int mouseLocation = 0;
    QTimer *timer = new QTimer(this);

};
#endif // MAINWINDOW_H
