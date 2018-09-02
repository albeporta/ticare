#include "bitcoinwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BitcoinWindow bc_win;
    const QRect availableSize = QApplication::desktop()->availableGeometry(&bc_win);
    bc_win.resize(availableSize.width() / 5, availableSize.height() / 5);
    bc_win.move((availableSize.width() - bc_win.width()) / 2, (availableSize.height() - bc_win.height()) / 2);

    bc_win.show();
    return app.exec();
}
