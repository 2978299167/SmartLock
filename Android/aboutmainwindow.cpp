#include "aboutmainwindow.h"
#include "update.h"
#include <string.h>
#include <cstring>
#include <QThread>
extern int src_w,src_h;
#include <QProgressDialog>

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

//typedef struct
//{
//    int Version;    //软件版本号
//    int SizeBin;    //BIN文件大小
//    int CheckSum;   //数据总和，校验数据用
//    int PromFlag;   //程序标志位，检测到特定字节才表示程序可用
//}FW_TypeDef;


//void FWtoChr(FW_TypeDef frmware,char * buf)
//{
//    buf[0] = frmware.Version>>24;
//    buf[1] = frmware.Version>>16%0x100;
//    buf[2] = frmware.Version>>8%0x100;
//    buf[3] = frmware.Version>>0;
//    buf[4] = frmware.SizeBin>>24;
//    buf[5] = frmware.SizeBin>>16%0x100;
//    buf[6] = frmware.SizeBin>>8%0x100;
//    buf[7] = frmware.SizeBin>>0;
//    buf[8] = frmware.CheckSum>>24;
//    buf[9] = frmware.CheckSum>>16%0x100;
//    buf[10] = frmware.CheckSum>>8%0x100;
//    buf[11] = frmware.CheckSum>>0;
//    buf[12] = frmware.PromFlag>>24;
//    buf[13] = frmware.PromFlag>>16%0x100;
//    buf[14] = frmware.PromFlag>>8%0x100;
//    buf[15] = frmware.PromFlag>>0;
//}


//void MCU_Update(void)
//{
//    char buf[16];
//    FW_TypeDef headinfo;
//    QProgressDialog progress;
//     progress.setWindowModality(Qt::WindowModal);
//     progress.setWindowTitle(QObject::tr("提示"));
//     progress.setLabelText(QObject::tr("正在升级单片机..."));
//     progress.setMinimum(0);
//     progress.setMaximum(100);
//     progress.setValue(1);
//     //qApp->processEvents();  //提交事件

////  for (int i=0;i<names.count()+6;i++)
////  {
////   progress.setValue(i);
////  }
////  qApp->processEvents();



//    headinfo.Version = SOFT_VER;
//    headinfo.PromFlag = HZC_FLAG;
//    headinfo.SizeBin = sizeof(update_buf);
//    //校验和
//    headinfo.CheckSum=0;
//    for(int i=0; i<headinfo.SizeBin; i++)
//    {
//        headinfo.CheckSum += update_buf[i];
//    }
//    qDebug() << "版本：" << headinfo.Version << endl;
//    qDebug() << "大小：" << headinfo.SizeBin << endl;
//    qDebug() << "标志：" << headinfo.PromFlag << endl;
//    qDebug() << "校验：" << headinfo.CheckSum << endl;
//    FWtoChr(headinfo,buf);
//    QByteArray Data((const char *)buf,sizeof(buf));
//    socket->write(Data);
//    QThread::msleep(500);
//    QByteArray arrayData((const char *)update_buf,sizeof(update_buf));
//    int i;
//    for(i=0; i<sizeof(update_buf)/256+1; i++)//发送
//    {
//        socket->write((const char *)(update_buf+i*256),256);
//        QThread::msleep(700);
//        progress.setValue(i*100/(sizeof(update_buf)/256));
//    }
//    //发送剩余代码
//    socket->write((const char *)(update_buf+i*256),sizeof(update_buf)%256);

//    arrayData.clear();


//}





AboutMainWindow::AboutMainWindow(QWidget *parent) : QMainWindow(parent)
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

//    //升级按钮
//    QTimer *tim = new QTimer(this);
//    button_Update = new QPushButton(this);
//    button_Update->setGeometry(450*src_w/1080,2100,260,100);
//    button_Update->setText("升级MCU");
//    //button_Update->setEnabled(true);
//    connect(button_Update,&myPushButton::clicked,[=](){
//        QByteArray arrayData;
//        QString s = QString("UPDATE\r\n");
//        qDebug() << s << endl;
//        arrayData = s.toUtf8();
//        socket->write(arrayData);
//        s.clear();
//        arrayData.clear();
//        tim->start(300);
//    });
//    connect(tim,&QTimer::timeout,[=](){
//        tim->stop();
//       // MCU_Update();
//    });

//    //显示版本
//    button_DisVer = new QPushButton(this);
//    button_DisVer->setGeometry(100*src_w/1080,2100,260,100);
//    button_DisVer->setText("显示版本");
//    connect(button_DisVer,&myPushButton::clicked,[=](){
//        QByteArray arrayData;
//        QString s = QString("VERSION\r\n");
//        qDebug() << s << endl;
//        arrayData = s.toUtf8();
//        socket->write(arrayData);
//        s.clear();
//        arrayData.clear();
//    });




    textBrowser = new QTextBrowser(this);
    textBrowser->setGeometry(0,150,1080*src_w/1080,1900);
    textBrowser->setStyleSheet("background-color: rgba(255, 255, 255, 200);font-size:60px;color: rgb(0, 0, 0);");
    textBrowser->setText("项目：智能门锁控制器\r\n"
                         "作者：化作尘\r\n"
                         "更新日期：2021年11月29日\r\n"
                         "版本：V1.00\r\n"
                         "邮箱：2809786963@qq.com\r\n"
                         "CSDN：化作尘\r\n"
                         "哔哩哔哩：化作尘my\r\n"
                         "软件说明：用于控制化作尘指纹锁\r\n"
                         );
    textBrowser->show();


}
