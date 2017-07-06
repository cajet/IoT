#ifndef SECURE_H
#define SECURE_H

#include <QWidget>
#include <widget.h>
#include <iostream>
#include <libwsncomm.h>
#include <node_config.h>
#include <QTimer>
#ifdef _WIN32
#include <winsock2.h>
#endif

namespace Ui {
    class Secure;
}

class Secure : public QWidget
{
    Q_OBJECT

public:
    explicit Secure(const QString &ip, quint8 id = 0xFF, QWidget *parent = 0);
    ~Secure();

public slots:
    void reconnect(const QString &ip, quint8 id = 0xFF);

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    static void cbNewFunc(void *arg, unsigned short nwkAddr, int funcNum, WSNCOMM_NODE_FUNC *funcList);
    static void cbNewData(void *arg, unsigned short nwkAddr, int endPoint, int funcCode, int funcID, char *data, int len);
    static void cbNodeGone(void *arg, unsigned short nwkAddr);

    void sendData(char data);

private:
    Ui::Secure *ui;
    QString wsnSrvIp;
    quint8 funcID;
    QTimer *PicTimer;
    void *wsnSrvUser;
    unsigned short curNwkAddr;
    char nodeData;
    QString backend;

protected:
    QPixmap pic[2];

signals:
    void nodeInfoChanged(unsigned short nwkAddr, unsigned short parAddr, unsigned char *mac);
    void dataChanged(char data);

protected slots:
    void PicTimeout();
    void updateNodeInfo(unsigned short nwkAddr, unsigned short parAddr, unsigned char *mac);
    void updateNodeData(char data);

public:
    static void showOut(const QString &ip, quint8 id = 0xFF);
    static void showWidget();
};

#endif // SECURE_H
