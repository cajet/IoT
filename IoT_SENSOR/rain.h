#ifndef RAIN_H
#define RAIN_H

#include <QWidget>
#include <iostream>
#include <QString>
#include <libwsncomm.h>
#include <node_config.h>

#include <widget.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

namespace Ui {
    class Rain;
}

class Rain : public QWidget
{
    Q_OBJECT

public:
    explicit Rain(const QString &ip, quint8 id = 0xFF, QWidget *parent = 0);
    ~Rain();

public slots:
    void reconnect(const QString &ip, quint8 id = 0xFF);

private:
    Ui::Rain *ui;
    QPixmap pic[2];
    QString wsnSrvIp;
    quint8 funcID;
    void *wsnSrvUser;
    unsigned short curNwkAddr;
    char nodeData;
    QString backend;

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    static void cbNewFunc(void *arg, unsigned short nwkAddr, int funcNum, WSNCOMM_NODE_FUNC *funcList);
    static void cbNewData(void *arg, unsigned short nwkAddr, int endPoint, int funcCode, int funcID, char *data, int len);
    static void cbNodeGone(void *arg, unsigned short nwkAddr);

    void sendData(char data);

signals:
    void nodeInfoChanged(unsigned short nwkAddr, unsigned short parAddr, unsigned char *mac);
    void gotNewNodeData(char v);

protected slots:
    void updateNodeInfo(unsigned short nwkAddr, unsigned short parAddr, unsigned char *hwAddr);
    void updateNodeData(char v);

public:
    static void showOut(const QString &ip, quint8 id = 0xFF);
    static void showWidget();
};

#endif // RAIN_H
