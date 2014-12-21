#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include "display_buffer.hh"
#include "ui_main.hh"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
    void Refresh();
public slots:
    void Tick100ms();
private:
    Ui::MainWindow *qt_ui;
    QGraphicsView *view_;
    QGraphicsPixmapItem *graphics_pixmap_;
    QGraphicsScene *scene_;
    UiMain ui_main_;
    DisplayBuffer display_buffer_;
};

#endif // MAINWINDOW_HH
