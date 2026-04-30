#include "btchoosewindow.h"
#include "ui_btchoosewindow.h"

btchoosewindow::btchoosewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::btchoosewindow)
{
    ui->setupUi(this);

    //返回
    connect(ui->pushButton_back,&QPushButton::clicked,[=](){
        this->hide();
        this->parentWidget()->show();
    });
}

btchoosewindow::~btchoosewindow()
{
    delete ui;
}
