#include "jump_viewer.hh"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QPen>
#include <QFileDialog>

JumpViewer::JumpViewer(LogFetcher *fetcher)
    :
      QMainWindow(0),
      list_selection_(new QListWidget(this)),
      plot_(new QwtPlot(this)),
      spin_selected_jump_(new QSpinBox(this)),
      log_fetcher_(fetcher)
{
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    central_widget->setLayout(new QHBoxLayout());

    QSplitter *splitter = new QSplitter(central_widget);
    central_widget->layout()->addWidget(splitter);

    QWidget *left_side = new QWidget(this);
    splitter->addWidget(left_side);

    QVBoxLayout *layout_left = new QVBoxLayout(left_side);

    layout_left->addWidget(list_selection_);

    spin_selected_jump_->setMinimum(0);
    layout_left->addWidget(spin_selected_jump_);

    QPushButton *button_fetch_jump = new QPushButton;
    button_fetch_jump->setText("Fetch jump");
    layout_left->addWidget(button_fetch_jump);

    QPushButton *button_csv = new QPushButton;
    button_csv->setText("Save to CSV");
    layout_left->addWidget(button_csv);

    QSizePolicy sizep;
    sizep.setHorizontalStretch(10);
    plot_->setSizePolicy(sizep);
    plot_->setAutoReplot(true);
    plot_->setAxisAutoScale(QwtPlot::xBottom,true);
    plot_->setAxisAutoScale(QwtPlot::yLeft,true);

    splitter->addWidget(plot_);

    connect(list_selection_, SIGNAL(itemSelectionChanged()), this, SLOT(CurveSelectionChanged()));
    connect(button_fetch_jump, &QPushButton::clicked, this, &JumpViewer::FetchSelectedJump);
    connect(button_csv, &QPushButton::clicked, this, &JumpViewer::WriteJumpToCsv);
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

void JumpViewer::SetNumberOfJumps(int number_of_jumps)
{
    spin_selected_jump_->setMaximum(number_of_jumps-1);
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

void JumpViewer::FetchSelectedJump()
{
    const int jump_idx = spin_selected_jump_->value();
    jump_ = log_fetcher_->GetJump(jump_idx);
    RebuildPlot();
}

void JumpViewer::WriteJumpToCsv()
{
    QString fname = GetOutputFilename();
    if (fname.isEmpty()) {
        return;
    }

    jump_.SaveToCsv(fname.toStdString().c_str());
}

QString JumpViewer::GetOutputFilename()
{
    return QFileDialog::getSaveFileName(this, "Output filename");
}
