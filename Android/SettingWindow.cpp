#include "SettingWindow.h"
#include "update.h"
#include <string.h>
#include <cstring>
#include <QThread>
#include <QLabel>
extern int src_w,src_h;
#include <QProgressDialog>
#include <QSpinBox>
#include <QPushButton>

/*
Ver  :0x01010000
Check:0x000E0988
Size :0x000027B4
Prom :0x00980706

libAndroid.so: 版本： 16842752 1010000
09-07 21:39:02.962 25624 25655 D libAndroid.so:
D libAndroid.so: 大小： 10164  27b4
09-07 21:39:02.962 25624 25655 D libAndroid.so:
D libAndroid.so: 标志： 9963270 980706
09-07 21:39:02.962 25624 25655 D libAndroid.so:
D libAndroid.so: 校验： 919944  E0988
*/


extern QBluetoothDeviceDiscoveryAgent *discoveryAgent;
extern QBluetoothLocalDevice *localDevice;
extern QBluetoothSocket *socket;



SettingWindow::SettingWindow(QWidget *parent) : QMainWindow(parent)
{

    this->setGeometry(0,0,src_w,src_h);//1080   2267

    //返回按钮
    button_back=new myPushButton(":/pic/back.png","",200,200);
    time1= new QTimer(this);
    button_back->setParent(this);
    button_back->move(850*src_w/1080,2050);

    //返回按钮按下处理
    connect(button_back,&myPushButton::clicked,[=](){
        button_back->zoom1();//弹跳
        button_back->zoom2();
        time1->start(500);

        connect(time1,&QTimer::timeout,[=](){
            time1->stop();
            this->parentWidget()->setStyleSheet("QMainWindow{border-image: url(:/pic/btmenuv2.jpg);}");
            this->hide();
            this->parentWidget()->show();
        });
    });
    QLabel    *lable1 = new QLabel("控制模式",this);
    lable1->setGeometry(20,150,500,50);
    QCheckBox *checkbox1 = new QCheckBox("光控模式", this);
    QCheckBox *checkbox2 = new QCheckBox("声控模式", this);
    QCheckBox *checkbox3 = new QCheckBox("人体感应模式", this);
    checkbox1->setGeometry(20,200,500,50);
    checkbox2->setGeometry(20,250,500,50);
    checkbox3->setGeometry(20,300,500,50);
    QPushButton *pushbutton1 = new QPushButton("设置",this);
    pushbutton1->setGeometry(20,400,200,100);
    connect(pushbutton1,&QPushButton::clicked,[=](){
        QByteArray arrayData;
        QString s = QString("SET_CTRLMODE:1%1%2%3\r\n").arg(checkbox1->isChecked()).arg(checkbox2->isChecked()).arg(checkbox3->isChecked());
        qDebug() << s << endl;
        arrayData = s.toUtf8();
        socket->write(arrayData);
        s.clear();
        arrayData.clear();
    });

    QLabel    *lable2 = new QLabel("定时关闭            分钟",this);
    lable2->setGeometry(20,600,500,50);
    QSpinBox *SpinBox1 = new QSpinBox(this);
    SpinBox1->setGeometry(220,600,100,50);
    QPushButton *pushbutton2 = new QPushButton("设置",this);
    pushbutton2->setGeometry(20,700,200,100);
    connect(pushbutton2,&QPushButton::clicked,[=](){
        QByteArray arrayData;
        QString s = QString("SET_OFFTIME:%1\r\n").arg(SpinBox1->value());
        qDebug() << s << endl;
        arrayData = s.toUtf8();
        socket->write(arrayData);
        s.clear();
        arrayData.clear();
    });



}
