#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    backend = QString("172.18.71.17");
    Rain::showOut("127.0.0.1", 0xFF);
    Secure::showOut("127.0.0.1", 0xFF);
}

Widget::~Widget()
{
    delete ui;
}

// Rain sensor
void Widget::on_pushButton_clicked()
{
    Rain::showWidget();
}

// IRDA sensor
void Widget::on_pushButton_2_clicked()
{
    Secure::showWidget();
}
