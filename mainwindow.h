#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

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
    void timerEvent(QTimerEvent *);
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

private:
    Ui::MainWindow *ui;
    int nodes[1764];
    QList<int> snake;
    int state;
    int step;
    int score;
    int direction;
    int interval;
    int eating;
    bool eggExist;
    int mouseLocation = 0;

};
#endif // MAINWINDOW_H
