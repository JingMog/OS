#include "app.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    app w;
    w.setStyleSheet("app{border-image:url(:/images/2.png);}");
    w.test();
    w.show();
    return a.exec();
}
