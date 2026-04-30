#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H




#include <QMainWindow>
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
#include <QTextBrowser>
#include <QPushButton>
#include "mypushbutton.h"
#include <QListWidgetItem>
#include <QCheckBox>



class SettingWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SettingWindow(QWidget *parent = nullptr);

signals:

private:
    QTextBrowser * textBrowser;
    //返回按钮
    myPushButton * button_back;
    QPushButton * button_Update;
    QPushButton * button_DisVer;
    QTimer * time1;

};




#endif // SETTINGWINDOW_H
