#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <iostream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    qt_ui(new Ui::MainWindow),
    view_(new QGraphicsView(this)),
    display_buffer_(128,128),
    ui_main_(&sensors_)
{
    qt_ui->setupUi(this);
    qt_ui->centralWidget->setLayout(new QHBoxLayout);
    qt_ui->centralWidget->layout()->addWidget(view_);

    scene_ = new QGraphicsScene(view_);
    view_->setScene(scene_);
    view_->setFocusPolicy( Qt::NoFocus ); //otherwise arrow keys will be captured
    graphics_pixmap_ = scene_->addPixmap(display_buffer_.GetPixmap());

    view_->ensureVisible(0,0,display_buffer_.GetWidth(),display_buffer_.GetHeight(),5);
    view_->scale(1,1);

    QTimer *timer_100ms = new QTimer;
    connect(timer_100ms, SIGNAL(timeout()), this, SLOT(Tick100ms()));
    timer_100ms->start(100);
}

MainWindow::~MainWindow()
{
    delete qt_ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch(event->key()) {
    case Qt::Key_Up:
        ui_main_.KeyPress(UiBase::KEY_UP, true);
        break;
    case Qt::Key_Down:
        ui_main_.KeyPress(UiBase::KEY_DOWN, true);
        break;
    case Qt::Key_Left:
        ui_main_.KeyPress(UiBase::KEY_LEFT, true);
        break;
    case Qt::Key_Right:
        ui_main_.KeyPress(UiBase::KEY_RIGHT, true);
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
        ui_main_.KeyPress(UiBase::KEY_UP, false);
        break;
    case Qt::Key_Down:
        ui_main_.KeyPress(UiBase::KEY_DOWN, false);
        break;
    case Qt::Key_Left:
        ui_main_.KeyPress(UiBase::KEY_LEFT, false);
        break;
    case Qt::Key_Right:
        ui_main_.KeyPress(UiBase::KEY_RIGHT, false);
        break;
    }
    Refresh();
}

void MainWindow::Refresh()
{
}

void MainWindow::Tick100ms()
{
    ui_main_.Tick100ms();
    ui_main_.Render(&display_buffer_);
    graphics_pixmap_->setPixmap(display_buffer_.GetPixmap());
}
