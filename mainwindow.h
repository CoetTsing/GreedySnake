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
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void go();
    void tailDrop();
    int egg();
    void test();

private:
    Ui::MainWindow *ui;
    int nodes[1609];
    QList<int> snake;
    int state = 1;
    int step = 0;
    int score = 0;
    int direction = 4;
    int interval = 200;
    int eating = 0;
    bool eggExist = false;

};
#endif // MAINWINDOW_H
