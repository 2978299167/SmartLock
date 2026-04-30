#include "btmainwindow.h"
#include "ui_btmainwindow.h"


extern QBluetoothDeviceDiscoveryAgent *discoveryAgent;
extern QBluetoothLocalDevice *localDevice;
extern QBluetoothSocket *socket;
extern unsigned char comBuffer[15];
extern unsigned int  comCount;
extern QString comStr;
extern QTimer *ptimer;
extern int num;

extern int src_w,src_h;
const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

BTMainWindow::BTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BTMainWindow)
{

    this->setGeometry(0,0,src_w,src_h);//1080   2267


    listwidget = new QListWidget(this);
    listwidget->setGeometry(0,150,1080*src_w/1080,1900 *src_h/2267);
    listwidget->setStyleSheet("background-color: rgba(255, 255, 255,150);"
                              "font-size:60px;color:black");



    //返回按钮
    button_back=new myPushButton(":/pic/back.png","",200*src_w/1080,200 *src_h/2267);
    time1= new QTimer(this);
    button_back->setParent(this);
    button_back->move(850*src_w/1080,2050 *src_h/2267);

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



    connect(listwidget,
            SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,
            SLOT(on_listWidget_itemDoubleClicked(QListWidgetItem*))
            );

    connect(discoveryAgent,
            SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this,
            SLOT(addBlueToothDevicesToList(QBluetoothDeviceInfo))
            );



}

BTMainWindow::~BTMainWindow()
{
    delete ui;
}






/***/
void BTMainWindow::addBlueToothDevicesToList( const QBluetoothDeviceInfo &info )
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    QList<QListWidgetItem *> items = this->listwidget->findItems(label, Qt::MatchExactly);

    //自动连接
    if(info.name() == "JDY-31-SPP")
    {
        QBluetoothAddress address(info.address().toString());
        //QMessageBox::information(this,tr("提示"),tr("设备自动连接中，请稍等！"));
        socket->connectToService(address, QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
    }
    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
            item->setTextColor(QColor(Qt::green));
        else
            item->setTextColor(QColor(Qt::black));
        this->listwidget->addItem(item);
    }

}



void BTMainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString text = item->text();
    int index = text.indexOf(' ');

    if (index == -1)
        return;

    QBluetoothAddress address(text.left(index));
    QString name(text.mid(index + 1));


    qDebug() << "You has choice the bluetooth address is " << address;
    if(socket->state() != QBluetoothSocket::ConnectingState )/* &&
            socket->state() != QBluetoothSocket::ConnectedState)*/
    {
        qDebug() << "The device is connneting.... ";
        //QMessageBox::information(this,tr("提示"),tr("设备正在连接中，请稍等！"));
        socket->connectToService(address, QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);
    }
}



