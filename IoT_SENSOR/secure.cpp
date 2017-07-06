#include "secure.h"
#include "ui_secure.h"
#include <QTextCodec>
#include <QString>
#include <QDebug>
#include <QDesktopWidget>

Secure::Secure(const QString &ip, quint8 id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Secure),
    funcID(id),
    wsnSrvUser(NULL),
    curNwkAddr(0xFFFF),
    nodeData(0)
{
    backend = QString("172.18.71.17");
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    this->setGeometry(10,40,this->width(),this->height());

    PicTimer = new QTimer();
    connect(PicTimer,SIGNAL(timeout()),this,SLOT(PicTimeout()));
    pic[0].load(":/pic/safe.png");
    pic[1].load(":/pic/human.png");
    ui->labelPic->setPixmap(pic[1]);
    ui->labelPic->setScaledContents(true);

    connect(this, SIGNAL(nodeInfoChanged(unsigned short,unsigned short,unsigned char*)), this, SLOT(updateNodeInfo(unsigned short,unsigned short,unsigned char*)));
    connect(this, SIGNAL(dataChanged(char)), this, SLOT(updateNodeData(char)));

    reconnect(ip, id);
}

Secure::~Secure()
{
    if(PicTimer)
        delete PicTimer;
    delete ui;
}

void Secure::reconnect(const QString &ip, quint8 id)
{
    wsncomm_unregister(wsnSrvUser);
    wsnSrvIp = ip;
    funcID = id;
    wsnSrvUser = wsncomm_register(wsnSrvIp.toUtf8().constData(),
                NULL, cbNewFunc, cbNewData, cbNodeGone,
                NULL, (void *)this);
    WSNCOMM_NODE *node = wsncomm_getNode_byType(ip.toUtf8().constData(), DevIRPers, funcID);
    if(node != NULL)
    {
        curNwkAddr = node->nwkAddr;
        updateNodeInfo(node->nwkAddr, node->parAddr, node->hwAddr);
        char *data = NULL;
        int len = wsncomm_getNodeData_byType(ip.toUtf8().constData(), DevIRPers, funcID, &data);
        if(data != NULL)
        {
            updateNodeData(data[0]);
            free(data);
        }
        wsncomm_delete_node(node);
    }
    else
    {
        curNwkAddr = 0xFFFF;
        updateNodeInfo(0xFFFF, 0xFFFF, NULL);
    }
    //QString title = QTextCodec::codecForName("UTF-8")->toUnicode(wsncomm_find_nodeTypeString(DevIRPers));
    QString title = "红外";
    title += " - ";
    if(funcID != 0xFF)
        title += QString::number(funcID);
    else
        title += tr("Any");
    this->setWindowTitle(title);
}

void Secure::updateNodeInfo(unsigned short nwkAddr, unsigned short parAddr, unsigned char *mac)
{
    if(nwkAddr == 0xFFFF)
    {
        ui->nwkAddrEdit->setText(tr("Invalid"));
        ui->parAddrEdit->setText(tr("Invalid"));
        ui->hwAddrEdit->setText(tr("Invalid"));
        ui->labelPic->setPixmap(QPixmap());
    }
    else
    {
        char tmpString[20];
        sprintf(tmpString, "%04X", nwkAddr);
        ui->nwkAddrEdit->setText(tmpString);
        sprintf(tmpString, "%04X", parAddr);
        ui->parAddrEdit->setText(tmpString);
        ui->hwAddrEdit->setText(QByteArray((char *)mac, 8).toHex());
#if 0
        if(data != NULL)
        {
            if(data[0] == 0)
            {
                ui->labelPic->setPixmap(pic[1]);
                ui->labelPic->setScaledContents(true);
                PicTimer->stop();
            }
            else
            {
                ui->labelPic->setPixmap(pic[1]);
                ui->labelPic->setScaledContents(true);
                PicTimer->start(100);
            }
        }
        else
            ui->labelPic->setPixmap(QPixmap());
#endif
    }
}

void Secure::updateNodeData(char data)
{
    if (data != nodeData) {
        sendData(data);
        nodeData = data;
    }
    switch(data)
    {
    case 0:
        ui->labelPic->setPixmap(pic[1]);
        ui->labelPic->setScaledContents(true);
        PicTimer->stop();
        break;
    case 1:
        ui->labelPic->setPixmap(pic[1]);
        ui->labelPic->setScaledContents(true);
        PicTimer->start(100);
        break;
    default:
        ui->labelPic->setPixmap(QPixmap());
    }
}

void Secure::sendData(char data)
{
    std::cout << "Secure::sendData data = " << (int)data << std::endl;
    QNetworkRequest request;
    QString url = QString("http://%1:8080/IoT/set/redsensor/%2").arg(backend).arg((int)data);
    std::cout << (const char*)url.toLocal8Bit() << std::endl;
    request.setUrl(QUrl(url));
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);
}

void Secure::PicTimeout()
{
    static int stat=1;
    static int orX = 0;
    if(stat == 1)
    {
        orX = ui->labelPic->x();
       ui->labelPic->move(ui->labelPic->x()+5,ui->labelPic->y());
       stat++;
    }
    else if(stat < 10)
    {
        ui->labelPic->move(ui->labelPic->x() + 5, ui->labelPic->y());
        stat++;
    }
    else
    {
        ui->labelPic->move(orX,ui->labelPic->y());
        stat = 1;
    }

}

void Secure::showEvent(QShowEvent *)
{
    if(wsnSrvUser == NULL)
    {
        wsnSrvUser = wsncomm_register(wsnSrvIp.toUtf8().constData(),
                    NULL, cbNewFunc, cbNewData, cbNodeGone,
                    NULL, (void *)this);
    }
}

void Secure::hideEvent(QHideEvent *)
{
    wsncomm_unregister(wsnSrvUser);
    wsnSrvUser = NULL;
}

Secure *secure = NULL;
void Secure::showOut(const QString &ip, quint8 id)
{
    if (secure == NULL)
        secure = new Secure(ip, id);
    else
        secure->reconnect(ip, id);
}

void Secure::showWidget()
{
    secure->show();
}

void Secure::cbNewFunc(void *arg, unsigned short nwkAddr, int funcNum, WSNCOMM_NODE_FUNC *funcList)
{
    Secure *This = qobject_cast<Secure *>((QObject *)arg);
    if(This == NULL)
        return;
    int i;
    for(i = 0; i < funcNum; i++)
    {
        if((funcList[i].funcCode == DevIRPers) && ((This->funcID == 0xFF) || (This->funcID == funcList[i].funcID)))
        {
            WSNCOMM_NODE *node = wsncomm_getNode_byAddr(This->wsnSrvIp.toUtf8().constData(), nwkAddr);
            if(node != NULL)
            {
                This->curNwkAddr = nwkAddr;
                emit This->nodeInfoChanged(node->nwkAddr, node->parAddr, node->hwAddr);
                wsncomm_delete_node(node);
            }
        }
    }
}

void Secure::cbNewData(void *arg, unsigned short nwkAddr, int endPoint, int funcCode, int funcID, char *data, int len)
{
    if(funcCode != DevIRPers)
        return;
    Secure *This = qobject_cast<Secure *>((QObject *)arg);
    if(This == NULL)
        return;
    if((This->funcID != 0xFF) && (funcID != This->funcID))
        return;
    if(This->curNwkAddr == 0xFFFF)
    {
        WSNCOMM_NODE *node = wsncomm_getNode_byAddr(This->wsnSrvIp.toUtf8().constData(), nwkAddr);
        if(node != NULL)
        {
            This->curNwkAddr = nwkAddr;
            emit This->nodeInfoChanged(node->nwkAddr, node->parAddr, node->hwAddr);
            wsncomm_delete_node(node);
        }
        else
            emit This->nodeInfoChanged(0xFFFF, 0xFFFF, NULL);
    }
    emit This->dataChanged((char)data[0]);
}

void Secure::cbNodeGone(void *arg, unsigned short nwkAddr)
{
    Secure *This = qobject_cast<Secure *>((QObject *)arg);
    if(This == NULL)
        return;
    if(nwkAddr != This->curNwkAddr)
        return;
    This->curNwkAddr = 0xFFFF;
    emit This->nodeInfoChanged(0xFFFF, 0xFFFF, NULL);
}
