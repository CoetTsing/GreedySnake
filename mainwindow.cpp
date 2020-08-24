#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i <= 1599; i++) {
        if (nodes[i] == 0) {
            QPen pen(QColor(46, 139, 87));
            QBrush brush(QColor(46, 139, 87));
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
            QPen pen(QColor(255, 215, 0));
            QBrush brush(QColor(255, 215, 0));
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

