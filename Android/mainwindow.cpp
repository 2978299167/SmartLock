#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutmainwindow.h"
#include "SettingWindow.h"
#include <QScreen>
#include <string>
#include <string.h>
#include <QFile>
#include <QDataStream>
#include <iostream>
#include <QButtonGroup>
#include <QTime>

using namespace std;



//时间
QString current_date;
int hour,minute,second;


#define MAXSIZE 256
typedef struct
{
    int curr_color;
    int color_tab[10];
}FileInfoStruct;


QBluetoothDeviceDiscoveryAgent *discoveryAgent;
QBluetoothLocalDevice *localDevice;
QBluetoothSocket *socket;
unsigned char comBuffer[15];
unsigned int  comCount;
QString comStr;
QTimer *ptimer;
int num;
int set_time=10;
int Connect_Flag = 0,SaveColorFlag=0;
float speed=10.0;
float P=0.71,I=0.12,D = 0.01;



FileInfoStruct *FileInfo;

const int color_tab[10] = {0xffff33,0x99ff33,0xff0000,0xffcccc,0x99ffcc,
                            0xcc66ff,0x33ffff,0xFF7F00,0xFFFFFF,0x000000};
const int mode_tab[10] = {
    0xF5FFFA,0x2E8B57,0x90EE90,0x808000,0xBDB76B,0xFFDAB9,
    0x8B4513,0xE9967A,0xFF6347,0xFFE4E1,
};
const QString ModeStr[7] = {"静态","呼吸","闪烁","渐变","蹦迪","流水","自定义"};
extern QColor Color_sum;

//const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
QTimer * timer_conflag;
//extern QRect rScreenSize;

int src_w,src_h;
int ColorSlider_paint_Flag = 1;
int Mainwondow_paint_Flag = 1;



BTMainWindow * btcwindow;
AboutMainWindow * aboutwindow;
SettingWindow * settingwindow;

template<class T>
bool bin_save(T* p_s)
{
    QFile file("BTinfo.dat");
        file.open(QIODevice::WriteOnly);
        int len = sizeof(T);
        file.write((char*)p_s,len);
        file.close();
        return true;
}

template<class T>
int bin_read(T*& p_r)
{/** 记住函数的参数传递是值复制传递，如果不用&传过来是指针的内容，即指向内存空间的地址，
    而且是复制一份地址，所以只能改变指针的指向内存位置的内容，不能改变指针的指向，
    传地址相当于引用，改变了指针的内容，即他指向的具体位置 */
    QFile tmpfile("BTinfo.dat");
    tmpfile.open(QIODevice::ReadOnly);

    p_r = new T[MAXSIZE];
    int len = tmpfile.read((char*)p_r, MAXSIZE);

    return len;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->Start_Init();
    this->File_Init();
    this->BuleTooth_Init();
    this->Label_Init();
    this->PushButton_Init();
    this->ColorSlider_Init();
    this->setFocus();
}

MainWindow::~MainWindow()
{
    bin_save(FileInfo);
    delete ui;
}

//起始代码
void MainWindow::Start_Init()
{
    //设置背景图片
    this->setStyleSheet("QMainWindow{border-image: url(:/pic/btmenuv2.jpg);}");
    //获取屏幕大小
    QScreen *screen = QApplication::screens().at(0);
    src_w = screen->size().width();
    src_h = screen->size().height();
    if(src_w <= 0  || src_h <= 0)
    {
        src_h = 2267;src_w = 1080;
        this->setGeometry(0,0,src_w,src_h);//1080   2267
        qDebug() << "src get err ======== "<< src_w <<src_h << endl;
    }
    else
    {
        this->setGeometry(0,0,src_w,src_h);//1080   2267
        qDebug() << "src get ok ======== "  << src_w <<src_h << endl;
    }
}

//蓝牙初始化
void MainWindow::BuleTooth_Init(void)
{
    //蓝牙连接初始化代码
    timer_conflag = new QTimer;
    ptimer = new QTimer;
    //QBluetoothDeviceDiscoveryAgent 这个是指扫描周围蓝牙设备！
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    //QBluetoothLocalDevice 是指配置获取设备的蓝牙状态信息等！
    localDevice = new QBluetoothLocalDevice();
    //QBluetoothSocket指进行链接蓝牙设备，读写信息！
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    //多窗口初始化
    btcwindow = new BTMainWindow(this);
    btcwindow->hide();
    aboutwindow = new AboutMainWindow(this);
    aboutwindow->hide();
    //settingwindow  = new SettingWindow(this);
    //settingwindow->hide();

    connect(socket,
           SIGNAL(readyRead()),
           this,
           SLOT(readBluetoothDataEvent())
           );
    connect(socket,
           SIGNAL(connected()),
           this,
           SLOT(bluetoothConnectedEvent())
           );
    connect(socket,
           SIGNAL(disconnected()),
           this,
           SLOT(bluetoothDisconnectedEvent())
           );

    localDevice->powerOn();//打开蓝牙
    discoveryAgent->start();//开始扫描
}




//颜色条初始化
void MainWindow::ColorSlider_Init(void)
{

}


//按钮初始化
void MainWindow::PushButton_Init(void)
{
    //刷新定时器
    static QColor last_Color;
    time1= new QTimer(this);
    time1->start(1000);
    connect(time1,&QTimer::timeout,[=](){

       time1->start(100);
       if(Connect_Flag == 1)//连接指示
       {
           Connect_Flag = 0;
           btcwindow->hide();
           this->show();
           QMessageBox::information(this,tr("提示"),tr("蓝牙连接成功!"));

           QByteArray arrayData;    //发送空指令
           QString s = QString("NONE\r\n");
           qDebug() << s << endl;
           arrayData = s.toUtf8();
           socket->write(arrayData);
           s.clear();
           arrayData.clear();
       }
       if(last_Color != Color_sum)//发送指令
       {
           update();//更新
           unsigned int color_d =   ((Color_sum.alpha()/16)<<24)+(Color_sum.red()<<16) + (Color_sum.green()<<8)
                         + (Color_sum.blue()<<0) ;
           QByteArray arrayData;
           QString s = QString("COLOR:%1\r\n").arg(color_d);
           qDebug() << s << endl;
           arrayData = s.toUtf8();
           socket->write(arrayData);
           s.clear();
           arrayData.clear();
       }
       last_Color = Color_sum;

    });



    //连接按钮
    button_connect = new myPushButton(":/pic/connect.png","",120*src_w/1080,120 *src_h/2267);
    time10= new QTimer(this);
    button_connect->setParent(this);
    button_connect->move(800*src_w/1080,2 *src_h/2267);
    //连接钮按下处理
    connect(button_connect,&myPushButton::clicked,[=](){
        button_connect->zoom1();//弹跳
        button_connect->zoom2();
        time10->start(500);

        localDevice->powerOn();//打开蓝牙
        discoveryAgent->start();//


        connect(time10,&QTimer::timeout,[=](){
            time10->stop();
            this->hide();
            this->setStyleSheet("QMainWindow{border-image: url(:/pic/btmenu1.jpg);}");
            btcwindow->show();
        });
    });

    //关于按钮
    button_about = new myPushButton(":/pic/about.png","",120*src_w/1080,120 *src_h/2267);
    time11= new QTimer(this);
    button_about->setParent(this);
    button_about->move(950*src_w/1080,2 *src_h/2267);

    //关于钮按下处理
    connect(button_about,&myPushButton::clicked,[=](){
        button_about->zoom1();//弹跳
        button_about->zoom2();
        time11->start(500);
        connect(time11,&QTimer::timeout,[=](){
            time11->stop();
            this->hide();
            aboutwindow->show();
            this->setStyleSheet("QMainWindow{border-image: url(:/pic/btmenu2.jpg);}");
        });
    });

}


//label初始化
void MainWindow::Label_Init(void)
{

    //链接状态显示label
    label_status = new QLabel(this);
    label_status->setGeometry(5*src_w/1080,10 *src_h/2267,200*src_w/1080,80 *src_h/2267);//设置坐标
    label_status->setStyleSheet("font-size:60px;color:white");//设置大小颜色

    //信息显示label
    label_info = new QLabel(this);
    label_info->setGeometry(0*src_w/1080,2180 *src_h/2267,src_w*src_w/1080,87 *src_h/2267);//设置坐标
    label_info->setStyleSheet("font-size:70px;color:white;background-color: rgba(255, 255, 255, 150);");//设置大小颜色

    lable1 = new QLabel(this);
    lable1->setGeometry(320*src_w/1080,920 *src_h/2267,450*src_w/1080,100 *src_h/2267);//设置坐标
    lable1->setStyleSheet("font-size:100px;color:white;background-color: rgba(255, 255, 255, 150);");//设置大小颜色

    lable2 = new QLabel(this);
    lable2->setGeometry(10*src_w/1080,800 *src_h/2267,850*src_w/1080,100 *src_h/2267);//设置坐标
    lable2->setStyleSheet("font-size:100px;color:white;background-color: rgba(0, 0, 0, 255);");//设置大小颜色
    lable2->setText("请输入6位密码：");
    if(socket->state() != QBluetoothSocket::ConnectedState)
    {
        this->label_status->setText("未连接");
    }
    else
    {
        this->label_status->setText("已连接");
    }


    /**************************时间***************************/
    lable3 = new QLabel(this);
    lable3->setGeometry(312*src_w/1080,2100 *src_h/2267,800*src_w/1080,50 *src_h/2267);//设置坐标
    lable3->setStyleSheet("font-size:50px;color:grey;background-color: rgba(0, 0, 0, 255);");//设置大小颜色
    time2 = new QTimer(this);
    time2->start(500);
    connect(time2,&QTimer::timeout,[=](){
        time2->start(500);
        //获取时间
        QTime current_time =QTime::currentTime();
        QDateTime current_date_time =QDateTime::currentDateTime();
        current_date =current_date_time.toString("yyyy-MM-dd");
        hour = current_time.hour();//当前的小时
        minute = current_time.minute();//当前的分
        second = current_time.second();//当前的秒

        QString s = current_date+ " " + QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
        lable3->setText(s);
    });


    /**************************信息***************************/
    QWidget *widget1 = new QWidget(this);
    widget1->setGeometry(0*src_w/1080,150 *src_h/2267,src_w-1,650 *src_h/2267);
    widget1->setStyleSheet("QWidget{border-image: url(:/pic/smartku.jpg);}");



    time_info = new QTimer(this);
    this->label_info->setText(" ");
    time_info->start(2000);
    connect(time_info,&QTimer::timeout,[=](){
        time_info->stop();
        this->label_info->clear();
    });

    pushbotton[16] = new QPushButton(this);
    pushbotton[16]->setStyleSheet("QPushButton{border-image: url(:/pic/eye.png);}");
    pushbotton[16]->setGeometry(780*src_w/1080,920 *src_h/2267,100*src_w/1080,100 *src_h/2267);

    QButtonGroup *btgroup = new QButtonGroup(this);
    for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
    {
        pushbotton[i*4+j] = new QPushButton(this);
        pushbotton[i*4+j]->setGeometry(40+j*270*src_w/1080,1100+i*270 *src_h/2267,200*src_w/1080,200 *src_h/2267);
    }
    pushbotton[0]->setText("1");
    pushbotton[1]->setText("2");
    pushbotton[2]->setText("3");
    pushbotton[3]->setText("Lock");
    pushbotton[4]->setText("4");
    pushbotton[5]->setText("5");
    pushbotton[6]->setText("6");
    pushbotton[7]->setText("Del");
    pushbotton[8]->setText("7");
    pushbotton[9]->setText("8");
    pushbotton[10]->setText("9");
    pushbotton[11]->setText("Clr");
    pushbotton[12]->setText("<");
    pushbotton[13]->setText("0");
    pushbotton[14]->setText("SetTime");
    pushbotton[15]->setText("OK");

    btgroup->addButton(pushbotton[0],'1');
    btgroup->addButton(pushbotton[1],'2');
    btgroup->addButton(pushbotton[2],'3');
    btgroup->addButton(pushbotton[3],'l');
    btgroup->addButton(pushbotton[4],'4');
    btgroup->addButton(pushbotton[5],'5');
    btgroup->addButton(pushbotton[6],'6');
    btgroup->addButton(pushbotton[7],'d');
    btgroup->addButton(pushbotton[8],'7');
    btgroup->addButton(pushbotton[9],'8');
    btgroup->addButton(pushbotton[10],'9');
    btgroup->addButton(pushbotton[11],'c');
    btgroup->addButton(pushbotton[12],'<');
    btgroup->addButton(pushbotton[13],'0');
    btgroup->addButton(pushbotton[14],'>');
    btgroup->addButton(pushbotton[15],'y');
    btgroup->addButton(pushbotton[16],'e');

    connect(btgroup,SIGNAL(buttonClicked(int )),this,SLOT(get_bt(int)));
}


void MainWindow::get_bt(int num)
{
    static QString str;
    static QString pswdstr;
    static int PswdFlag=0;
    //开始处理
    char c = (char)num;

    if(c>='0' && c<='9')
    {
        str+=c;
        pswdstr+='*';
    }else
    {
        if(c=='d')//删除
        {
            str = str.left(str.size() - 1);
            pswdstr = pswdstr.left(str.size() - 1);
        }else if(c=='e')//明文、密文切换
        {
            PswdFlag = !PswdFlag;
        }else if(c=='c')//清除
        {
            str.clear();
            pswdstr.clear();
        }else if(c=='y')//确认
        {
            QByteArray arrayData;    //发送指令
            QString s = str+"!:";
            qDebug() << s << endl;
            arrayData = s.toUtf8();
            socket->write(arrayData);
            s.clear();
            arrayData.clear();

            str.clear();
            pswdstr.clear();
        }else if(c=='l')//锁定
        {
            QByteArray arrayData;    //发送指令
            QString s = "LOCK!:";
            qDebug() << s << endl;
            arrayData = s.toUtf8();
            socket->write(arrayData);
            s.clear();
            arrayData.clear();

            str.clear();
            pswdstr.clear();
        }else if(c=='t')//发送时间
        {
            QByteArray arrayData;    //发送指令
            QString s = "TIME!:";
            qDebug() << s << endl;
            arrayData = s.toUtf8();
            socket->write(arrayData);
            s.clear();
            arrayData.clear();

            str.clear();
            pswdstr.clear();
        }
    }
    if(PswdFlag)
    lable1->setText(str);
    else
    lable1->setText(pswdstr);
}




//文件信息读取初始化
void MainWindow::File_Init()
{
//    FileInfo = new FileInfoStruct;
//    int readlen = bin_read(FileInfo);
//    if(readlen != sizeof(FileInfoStruct))//没有数据,恢复默认
//    {
//        FileInfo->curr_color = 0XFFFFFF;
//        for(int i=0; i<sizeof(color_tab); i++)
//        FileInfo->color_tab[i] = color_tab[i];
//        bin_save(FileInfo);
//    }
}



//绘画事件
void MainWindow::paintEvent(QPaintEvent *)
{
//    qDebug() << "绘画事件" << endl;
//    QPainter painter(this);
//    QPen pen;
//    //Color_sum.setAlpha(colorslider_A->);
//    pen.setColor(Color_sum);
//    pen.setWidth(200);
//    painter.setPen(pen);
//    painter.drawEllipse(440*src_w/1080,1100*src_h/2267,200*src_w/1080,200*src_h/2267);


//    pen.setWidth(10);
//    QColor cor(0xff,0xff,0xff);
//    if(SaveColorFlag != 0)
//    {
//        pen.setColor(cor);
//        painter.setPen(pen);
//        QRectF rectangle(40, 1550, 1000, 400);
//        painter.drawRect(rectangle);
//    }else
//    {
//        cor.setAlpha(0);
//        pen.setColor(cor);
//        painter.setPen(pen);
//        QRectF rectangle(40, 1550, 1000, 400);
//        painter.drawRect(rectangle);
//    }

//    painter.end();
}


//槽函数 绘画更新
void MainWindow::Paint_Color(QColor color)
{
   //update();
}

/***/
void MainWindow::readBluetoothDataEvent()
{

//    QByteArray line = socket->readAll();
//    QString rxbuf = line.toStdString().data();

//    if(rxbuf.startsWith("TEMPVAL:"))
//    {
//        rxbuf.remove(0,8);
//        lable1->setText(rxbuf);
//    }else if(rxbuf.startsWith("HUMIVAL:"))
//    {
//        rxbuf.remove(0,8);
//        lable1->setText(rxbuf);
//    }else if(rxbuf.startsWith("AIRVAL:"))
//    {
//        rxbuf.remove(0,7);
//        lable1->setText(rxbuf);
//    }

//    QString str = QString("     接收：%1").arg(QString(line.toStdString().data()));
//    this->label_info->setText(str);
//    time_info->start(2000);

//    line.clear();
}

/***/
void MainWindow::bluetoothConnectedEvent()
{
    qDebug() << "The android device has been connected successfully!";
    //QMessageBox::information(this,tr("提示"),tr("蓝牙连接成功!"));
    Connect_Flag = 1;
    this->label_status->setText("已连接");
}

/***/
void MainWindow::bluetoothDisconnectedEvent()
{
    qDebug() << "The android device has been disconnected successfully!";
    QMessageBox::information(this,tr("提示"),tr("蓝牙已断开!"));
    this->label_status->setText("未连接");
}
