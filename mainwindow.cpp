#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QtGlobal>
#include <QKeyEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

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
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(Qt::BottomToolBarArea, toolBar);
    toolBar->setMovable(false);
    toolBar->addAction(ui->actionStart);
    toolBar->addAction(ui->actionReset);
    toolBar->addAction(ui->actionPause);
    toolBar->addAction(ui->actionSave);
    toolBar->addAction(ui->actionLoad);
    toolBar->addAction(ui->actionExit);
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
    interval = 150;
    eating = 0;
    eggExist = false;
    mouseLocation = 0;
    ui->step->setText(QString::number(step));
    ui->score->setText(QString::number(score));
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
    if (nodes[mouseLocation] == 0) {
        QPen pen(QColor(128, 138, 135));
        QBrush brush(QColor(128, 138, 135));
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(QRect(100 + 25 * (mouseLocation % 42), 100 + 25 * (mouseLocation / 42), 25, 25));
    }
    mouseLocation = 0;
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
        ui->Start->setEnabled(false);
        ui->Reset->setEnabled(false);
        ui->Pause->setEnabled(true);
        ui->Save->setEnabled(false);
        ui->Load->setEnabled(false);
        ui->actionStart->setEnabled(false);
        ui->actionReset->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->actionSave->setEnabled(false);
        ui->actionLoad->setEnabled(false);
    } else if (state == 0) {
        ui->Start->setEnabled(true);
        ui->Reset->setEnabled(false);
        ui->Pause->setEnabled(false);
        ui->Save->setEnabled(false);
        ui->Load->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionReset->setEnabled(false);
        ui->actionPause->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionLoad->setEnabled(true);
    } else if (state == 2) {
        ui->Start->setEnabled(true);
        ui->Reset->setEnabled(true);
        ui->Pause->setEnabled(false);
        ui->Save->setEnabled(true);
        ui->Load->setEnabled(false);
        ui->actionStart->setEnabled(true);
        ui->actionReset->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionSave->setEnabled(true);
        ui->actionLoad->setEnabled(false);
    } else if (state == 3) {
        ui->Start->setEnabled(false);
        ui->Reset->setEnabled(true);
        ui->Pause->setEnabled(false);
        ui->Save->setEnabled(false);
        ui->Load->setEnabled(false);
        ui->actionStart->setEnabled(false);
        ui->actionReset->setEnabled(true);
        ui->actionPause->setEnabled(true);
        ui->actionSave->setEnabled(false);
        ui->actionLoad->setEnabled(false);
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
            if (nodes[mouseLocation] == 0)
                nodes[mouseLocation] = 3;
            else if (nodes[mouseLocation] == 3)
                nodes[mouseLocation] = 0;
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
        score += 5;
        ui->score->setText(QString::number(score));
    }
    if (nodes[*(snake.begin())] == 1 || nodes[*(snake.begin())] == 3) {
        state = 3;
        QMessageBox::information(this, "GameOver", "Game Over!");
    }
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

void MainWindow::save() {
    QString path = QFileDialog::getSaveFileName(this, "Save", ".", "JSON(*.json)");
    if (!path.isEmpty()) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject job;
            job.insert("state", state);
            job.insert("step", step);
            job.insert("score", score);
            job.insert("direction", direction);
            job.insert("interval", interval);
            job.insert("eating", eating);
            job.insert("eggExist", eggExist);
            job.insert("mousLocation", mouseLocation);
            QJsonArray jarrNodes;
            for (int i = 0; i <= 1763; i++)
                jarrNodes.append(nodes[i]);
            job.insert("nodes", jarrNodes);
            QJsonArray jarrSnake;
            for (auto x: snake)
                jarrSnake.append(x);
            job.insert("snake", jarrSnake);
            QJsonDocument jd;
            jd.setObject(job);
            file.write(jd.toJson());
            file.close();
        }
    }
}

void MainWindow::load() {
    QString path = QFileDialog::getOpenFileName(this, "Open", ".", "JSON(*.json)");
    if (!path.isEmpty()) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray allData = file.readAll();
            file.close();
            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
            if (json_error.error != QJsonParseError::NoError) {
                QMessageBox::warning(this, "Error", "Json Error!");
                return;
            }
            QJsonObject job = jsonDoc.object();
            state = job["state"].toInt();
            step = job["step"].toInt();
            score = job["score"].toInt();
            direction = job["direction"].toInt();
            interval = job["interval"].toInt();
            eating = job["eating"].toInt();
            eggExist = job["eggExist"].toBool();
            mouseLocation = job["mouseLocation"].toInt();
            QJsonArray jarrNodes = job["nodes"].toArray();
            for (int i = 0; i <= 1763; i++) {
                nodes[i] = jarrNodes.at(i).toInt();
            }
            QJsonArray jarrSnake = job["snake"].toArray();
            int l = jarrSnake.size();
            snake.clear();
            for (int i = 0; i < l; i++) {
                snake.push_back(jarrSnake.at(i).toInt());
            }
            ui->score->setText(QString::number(score));
            ui->step->setText(QString::number(step));
        }
    }
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

void MainWindow::on_Pause_clicked()
{
    state = 2;
}

void MainWindow::on_Exit_clicked()
{
    this->close();
}

void MainWindow::on_actionStart_triggered()
{
    state = 1;
}

void MainWindow::on_actionReset_triggered()
{
    init();
    update();
}

void MainWindow::on_actionPause_triggered()
{
    state = 2;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_Save_clicked()
{
    save();
}

void MainWindow::on_Load_clicked()
{
    load();
}

void MainWindow::on_actionSave_triggered()
{
    save();
}

void MainWindow::on_actionLoad_triggered()
{
    load();
}
