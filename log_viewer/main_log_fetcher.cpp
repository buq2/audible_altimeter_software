#include "log_fetcher.hh"
#include <QApplication>
#include "jump_viewer.hh"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LogFetcher log("/dev/ttyACM0");
    if (!log.Open()) {
        std::cerr << "Failed to open serial port" << std::endl;
        return -1;
    }
    const size_t num_jumps = log.GetNumberOfJumps();
    if (num_jumps > 0) {

        JumpViewer viewer(&log);
        viewer.SetNumberOfJumps(num_jumps);
        viewer.show();

        return app.exec();
}
    return -1;
}
