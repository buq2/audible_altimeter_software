#include "jump_viewer.hh"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QPen>

JumpViewer::JumpViewer()
    :
      QMainWindow(0),
      list_selection_(new QListWidget(this)),
      plot_(new QwtPlot(this))
{
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    central_widget->setLayout(new QHBoxLayout());

    QSplitter *splitter = new QSplitter(central_widget);
    central_widget->layout()->addWidget(splitter);
    splitter->addWidget(list_selection_);

    splitter->addWidget(list_selection_);

    QSizePolicy sizep;
    sizep.setHorizontalStretch(10);
    plot_->setSizePolicy(sizep);
    plot_->setAutoReplot(true);
    plot_->setAxisAutoScale(QwtPlot::xBottom,true);
    plot_->setAxisAutoScale(QwtPlot::yLeft,true);

    splitter->addWidget(plot_);

    connect(list_selection_, SIGNAL(itemSelectionChanged()), this, SLOT(CurveSelectionChanged()));
}

void JumpViewer::Clear()
{
    ClearCurves();
    list_selection_->clear();
}

void JumpViewer::SetJump(const Jump &jump)
{
    jump_ = jump;
    RebuildPlot();
}

void JumpViewer::RebuildPlot()
{
    Clear();
    auto names = jump_.GetDataNames();
    for(auto name : names) {
        list_selection_->addItem(name.c_str());
    }
}

void JumpViewer::CurveSelectionChanged()
{
    ClearCurves();
    const auto selected = list_selection_->selectedItems();
    for (auto sel : selected) {
        auto curve_name_qstr = sel->text();

        auto data = jump_.GetData(curve_name_qstr.toStdString());

        QVector< QPointF > qdata;
        for (auto p : data) {
            qdata.push_back(QPointF(p.first,p.second));
        }

        QwtPointSeriesData *plot_data = new QwtPointSeriesData(qdata);
        QwtPlotCurve *curve = new QwtPlotCurve(curve_name_qstr);
        curve->setData(plot_data);
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(32,227,32));
        curve->setPen(pen);
        curve->attach(plot_);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curves_.push_back(curve);
    }
}

void JumpViewer::ClearCurves()
{
    for(auto c : curves_) {
        delete c;
    }
    curves_.clear();
}
