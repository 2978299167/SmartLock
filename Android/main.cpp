#include "mainwindow.h"
#include <QScreen>
#include <QApplication>
#include <QDebug>

//QRect rScreenSize;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    QScreen *iScreen = a.primaryScreen();
//    rScreenSize = iScre*/en->geometry();


//    qDebug() << src_w << endl;
//    qDebug() << src_h << endl;
//    QIcon icon = QIcon(":/pic/hzcbt.ico");//使用相对路径
//    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}
