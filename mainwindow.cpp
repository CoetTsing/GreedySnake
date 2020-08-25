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
    this->resize(1550, 1250);
    init();
    startTimer(interval);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    state = 0;
    step = 0;
    score = 0;
    direction = 4;
    interval = 200;
    eating = 0;
    eggExist = false;
    mouseLocation = 0;
    for (int i = 0; i <= 1763; i++) {
        if ((i <= 41) || (i >= 1722 && i <= 1763) || (i % 42 == 0) || (i % 42 == 41))
            nodes[i] = 3;
        else
            nodes[i] = 0;
    }
    nodes[902] = 1;
    nodes[903] = 4;
    snake.clear();
    snake.push_back(903);
    snake.push_back(902);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i <= 1763; i++) {
        if (nodes[i] == 0) { // 平地
            QPen pen(QColor(250, 240, 230));
            QBrush brush(QColor(250, 240, 230));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 42), 100 + 25 * (i / 42), 25, 25));
        } else if (nodes[i] == 1 || nodes[i] == 4) { //蛇身&蛇头
            QPen pen(QColor(107, 142, 35));
            QBrush brush(QColor(107, 142, 35));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 42), 100 + 25 * (i / 42), 25, 25));
        } else if (nodes[i] == 2) { //egg
            QPen pen(QColor(176, 23, 31));
            QBrush brush(QColor(176, 23, 31));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 42), 100 + 25 * (i / 42), 25, 25));
        } else if (nodes[i] == 3) { //障碍
            QPen pen(QColor(128, 128, 105));
            QBrush brush(QColor(128, 128, 105));
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(QRect(100 + 25 * (i % 42), 100 + 25 * (i / 42), 25, 25));
        }
    }
    if (mouseLocation > 0) {
        QPen pen(QColor(128, 138, 135));
        QBrush brush(QColor(128, 138, 135));
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(QRect(100 + 25 * (mouseLocation % 42), 100 + 25 * (mouseLocation / 42), 25, 25));
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (state == 1) {
        ui->step->setText(QString::number(++step));
        go();
        tailDrop();
        if (!eggExist) {
            nodes[egg()] = 2;
            eggExist = true;
        }
        update();
    }
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

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (state == 0) {
        int x = event->x();
        int y = event->y();
        if (x > 125 && x < 1125 && y > 125 && y <1125) {
            mouseLocation = (x - 125) / 25 + 42 * ((y - 125) / 25 + 1) + 1;
            update();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (state == 0) {
        int x = event->x();
        int y = event->y();
        if (x > 125 && x < 1125 && y > 125 && y <1125) {
            mouseLocation = (x - 125) / 25 + 42 * ((y - 125) / 25 + 1) + 1;
            nodes[mouseLocation] = 3;
            update();
        }
    }
}

void MainWindow::go() {
    int now = *(snake.begin());
    if (direction == 1)
        snake.push_front(now - 42);
    else if (direction == 2)
        snake.push_front(now + 42);
    else if (direction == 3)
        snake.push_front(now - 1);
    else
        snake.push_front(now + 1);
    if (nodes[*(snake.begin())] == 2) {
        eating = 3;
        eggExist = false;
    }
    if (nodes[*(snake.begin())] == 1 || nodes[*(snake.begin())] == 3)
        state = 3;
    for (auto x: snake)
        nodes[x] = 1;
    nodes[*snake.begin()] = 4;
}

void MainWindow::tailDrop() {
    if (eating == 0) {
        nodes[*(snake.end() - 1)] = 0;
        snake.pop_back();
    } else
        eating--;
}

int MainWindow::egg() {
    int a = qrand() % 1764;
    if (nodes[a] == 0)
        return a;
    else
        return egg();
}

void MainWindow::on_Start_clicked()
{
    state = 1;
}

void MainWindow::on_Reset_clicked()
{
    init();
    update();
}
