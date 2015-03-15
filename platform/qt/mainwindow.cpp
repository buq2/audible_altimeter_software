#include "mainwindow.hh"
#include "platform/qt/ui_mainwindow.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

// Ugly, but works for the testing purposes
MainWindow *mainwin;
void UpdateConfig(Config *conf)
{
    mainwin->display_buffer_.SetRotation(conf->display_orientation);
    mainwin->ui_main_.GetAltimeterUi()->SetUiMode(conf->default_altimeter_ui_mode_);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    qt_ui(new Ui::MainWindow),
    view_(new QGraphicsView(this)),
    ui_main_(&config_, &sensors_, &misc_),
    display_buffer_(128,128)
{
    sensors_.SetMiscInformation(&misc_);
    ui_main_.SetConfigChangedFunction(UpdateConfig);
    // Ugly, but works similar to MCU code
    mainwin = this;

    qt_ui->setupUi(this);
    qt_ui->centralWidget->setLayout(new QHBoxLayout);
    qt_ui->centralWidget->layout()->addWidget(view_);

    scene_ = new QGraphicsScene(view_);
    view_->setScene(scene_);
    view_->setFocusPolicy(Qt::NoFocus); //otherwise arrow keys will be captured

    graphics_pixmap_ = scene_->addPixmap(GetPixmap());

    view_->ensureVisible(0,0,display_buffer_.GetWidth(),display_buffer_.GetHeight(),5);
    view_->scale(3,3);

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
}

void MainWindow::Tick100ms()
{
    ui_main_.Tick100ms();
    ui_main_.Render(&display_buffer_);
    graphics_pixmap_->setPixmap(GetPixmap());
}

QPixmap MainWindow::GetPixmap()
{
    QImage img((uchar*)display_buffer_.GetBuffer(),
               display_buffer_.GetWidth(),
               display_buffer_.GetHeight(),
               QImage::Format_MonoLSB);
    return QPixmap::fromImage(img);
}
