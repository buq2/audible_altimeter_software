#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include "megaui.hh"
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

private:
    Ui::MainWindow *ui;
    QGraphicsView *view_;
    QGraphicsPixmapItem *graphics_pixmap_;
    QGraphicsScene *scene_;
    Megaui megaui_;
};

#endif // MAINWINDOW_HH
