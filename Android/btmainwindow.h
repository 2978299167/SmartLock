#ifndef BTMAINWINDOW_H
#define BTMAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
//#include <QtBluetooth/qbluetoothglobal.h>
#include <QtBluetooth/qtbluetoothglobal.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothsocket.h>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QListWidget>

#include "mypushbutton.h"
#include <QListWidgetItem>

namespace Ui {
class BTMainWindow;
}

class BTMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BTMainWindow(QWidget *parent = nullptr);
    ~BTMainWindow();


private slots:

    void addBlueToothDevicesToList(const QBluetoothDeviceInfo&);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


private:
    Ui::BTMainWindow *ui;
    QListWidget * listwidget;
    //返回按钮
    myPushButton * button_back;

    //QLabel * Infolabel;
    QTimer * time1;
};

#endif // BTMAINWINDOW_H
