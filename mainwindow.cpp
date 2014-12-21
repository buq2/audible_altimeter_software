#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    view_(new QGraphicsView(this)),
    megaui_(128,128)
{
    ui->setupUi(this);


    ui->centralWidget->setLayout(new QHBoxLayout);
    ui->centralWidget->layout()->addWidget(view_);

    scene_ = new QGraphicsScene(view_);
    view_->setScene(scene_);
    view_->setFocusPolicy( Qt::NoFocus ); //otherwise arrow keys will be captured
    graphics_pixmap_ = scene_->addPixmap(megaui_.GetPixmap());

    view_->ensureVisible(0,0,megaui_.GetWidth(),megaui_.GetHeight(),5);
    view_->scale(1,1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch(event->key()) {
    case Qt::Key_Up:
        megaui_.KeyUp(true);
        break;
    case Qt::Key_Down:
        megaui_.KeyDown(true);
        break;
    case Qt::Key_Left:
        megaui_.KeyLeft(true);
        break;
    case Qt::Key_Right:
        megaui_.KeyRight(true);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch(event->key()) {
    case Qt::Key_Up:
        megaui_.KeyUp(false);
        break;
    case Qt::Key_Down:
        megaui_.KeyDown(false);
        break;
    case Qt::Key_Left:
        megaui_.KeyLeft(false);
        break;
    case Qt::Key_Right:
        megaui_.KeyRight(false);
        break;
    }
}
