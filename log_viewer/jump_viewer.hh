#ifndef AUDIBLEA_JUMP_VIEWER_HH
#define AUDIBLEA_JUMP_VIEWER_HH

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QListWidget>
#include "jump.hh"

class JumpViewer
        :
        public QMainWindow
{
    Q_OBJECT
 public:
    JumpViewer();
    void Clear();
    void SetJump(const Jump &jump);

 private slots:
    void RebuildPlot();
    void CurveSelectionChanged();
    void ClearCurves();
 private:
    QListWidget *list_selection_;
    QwtPlot *plot_;
    std::vector<QwtPlotCurve*> curves_;
    Jump jump_;
}; //class JumpViewer

#endif //ifndef AUDIBLEA_JUMP_VIEWER_HH
