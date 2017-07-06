#include "widget.h"
#include <QApplication>
#define MAIN_NODE_CONFIG
#include <node_config.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
