#include "ShadowDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShadowDialog w;
    w.show();

    return a.exec();
}
