#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QtGlobal>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1500, 1200);
    for (int i = 0; i <= 1599; i++) {
        nodes[i] = 0;
    }
    nodes[800] = nodes[801] = 1;
    snake.push_back(800);
    snake.push_back(801);
    startTimer(interval);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i <= 1599; i++) {
        if (nodes[i] == 0) {
            QPen pen(QColor(250, 240, 230));
            QBrush brush(QColor(250, 240, 230));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 40), 100 + 25 * (i / 40), 25, 25));
        } else if (nodes[i] == 1) {
            QPen pen(QColor(107, 142, 35));
            QBrush brush(QColor(107, 142, 35));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 40), 100 + 25 * (i / 40), 25, 25));
        } else if (nodes[i] == 2) {
            QPen pen(QColor(176, 23, 31));
            QBrush brush(QColor(176, 23, 31));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 40), 100 + 25 * (i / 40), 25, 25));
        } else {
            QPen pen(QColor(135, 51, 36));
            QBrush brush(QColor(135, 51, 36));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 40), 100 + 25 * (i / 40), 25, 25));
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    test();
    ui->step->setText(QString::number(++step));
    go();
    tailDrop();
    if (!eggExist) {
        nodes[egg()] = 2;
        eggExist = true;
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) {
        if (direction != 2)
            direction = 1;
    } else if (event->key() == Qt::Key_Down) {
        if (direction != 1)
            direction = 2;
    } else if (event->key() == Qt::Key_Left) {
        if (direction != 4)
            direction = 3;
    } else if (event->key() == Qt::Key_Right) {
        if (direction != 3)
            direction = 4;
    }
}

void MainWindow::go() {
    int now = *(snake.begin());
    if (direction == 1)
        snake.push_front(now - 40);
    else if (direction == 2)
        snake.push_front(now + 40);
    else if (direction == 3)
        snake.push_front(now - 1);
    else
        snake.push_front(now + 1);
    if (nodes[*(snake.begin())] == 2) {
        eating = 3;
        eggExist = false;
    }
    for (auto x: snake)
        nodes[x] = 1;
}

void MainWindow::tailDrop() {
    if (eating == 0) {
        nodes[*(snake.end() - 1)] = 0;
        snake.pop_back();
    } else
        eating--;
}

int MainWindow::egg() {
    int a = qrand() % 1600;
    if (nodes[a] == 0)
        return a;
    else
        return egg();
}

void MainWindow::test() {
}

