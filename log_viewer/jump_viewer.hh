#ifndef AUDIBLEA_JUMP_VIEWER_HH
#define AUDIBLEA_JUMP_VIEWER_HH

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QListWidget>
#include "jump.hh"
#include <QSpinBox>
#include <QPushButton>
#include "log_fetcher.hh"

class JumpViewer
        :
        public QMainWindow
{
    Q_OBJECT
 public:
    JumpViewer(LogFetcher *fetcher);
    void Clear();
    void SetJump(const Jump &jump);
    void SetNumberOfJumps(int number_of_jumps);
 private slots:
    void RebuildPlot();
    void CurveSelectionChanged();
    void ClearCurves();
    void FetchSelectedJump();
    void WriteJumpToCsv();
 private:
    QString GetOutputFilename();
 private:
    QListWidget *list_selection_;
    QwtPlot *plot_;
    std::vector<QwtPlotCurve*> curves_;
    Jump jump_;
    QSpinBox *spin_selected_jump_;
    LogFetcher *log_fetcher_;
}; //class JumpViewer

#endif //ifndef AUDIBLEA_JUMP_VIEWER_HH
