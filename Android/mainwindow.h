#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "mypushbutton.h"
#include "btmainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QLineEdit>
#include <QProgressBar>
#include <QSlider>
#include "colorslider.h"
#include <QPainter>
#include "SettingWindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    //定时器
    QTimer * time1;
    QTimer * time2;
    QTimer * time3;
    QTimer * time4;
    QTimer * time5;
    QTimer * time6;
    QTimer * time7;
    QTimer * time8;
    QTimer * time9;
    QTimer * time10;
    QTimer * time11;
    QTimer * time12;
    QTimer * time_info;
    //时间加按钮
    myPushButton * button_add;
    //时间减按钮
    myPushButton * button_sub;
    //开始按钮
    myPushButton * button_start;
    //连接按钮
    myPushButton * button_connect;
    //关于按钮
    myPushButton * button_about;
    //设置按钮
    myPushButton * button_setting;

    //链接状态显示label
    QLabel * label_status;
    QLabel * label_info;

    QLabel * lable1;
    QLabel * lable2;
    QLabel * lable3;
    QLabel * lablePic;
    QLineEdit * lable4;
    QLineEdit * lable5;
    QLineEdit * lable6;


    //色条
    ColorSlider *colorslider_R;
    ColorSlider *colorslider_G;
    ColorSlider *colorslider_B;
    ColorSlider *colorslider_A;

    //色块按钮
    QPushButton *phbutton[10];


    //模式按钮
    QPushButton *pushbotton[20];
    //保存按钮
    QPushButton *SavePushbutton;


private slots:
    //槽函数
    void Paint_Color(QColor color);
    void readBluetoothDataEvent();
    void bluetoothConnectedEvent();
    void bluetoothDisconnectedEvent();

    void get_bt(int num);

private:
    Ui::MainWindow *ui;

    //开始代码
    void Start_Init(void);
    //蓝牙初始化
    void BuleTooth_Init(void);
    //按钮初始化
    void PushButton_Init(void);
    //label初始化
    void Label_Init(void);
    //文件初始化
    void File_Init(void);
    //颜色调初始化
    void ColorSlider_Init(void);
    //绘画事件重写
    void paintEvent(QPaintEvent *);
};
#endif // MAINWINDOW_H
