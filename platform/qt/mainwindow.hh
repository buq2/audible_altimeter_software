#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include "axlib/displays/display_buffer.hh"
#include "ui/ui_main.hh"
#include "sensors.hh"

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
public slots:
    void Tick100ms();
private:
    QPixmap GetPixmap();
private:
    Ui::MainWindow *qt_ui;
    QGraphicsView *view_;
    QGraphicsPixmapItem *graphics_pixmap_;
    QGraphicsScene *scene_;
    UiMain ui_main_;
    DisplayBuffer display_buffer_;
    Sensors sensors_;
    MiscInformation misc_;
    Config config_;
    friend void UpdateConfig(Config *conf);
};

#endif // MAINWINDOW_HH
