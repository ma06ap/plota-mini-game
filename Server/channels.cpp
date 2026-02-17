#include "channels.h"
#include <QDebug>

channels::channels(QTcpSocket* _socket, QObject *parent)
    : QObject{parent}
{
    socket = _socket;
    socket->setParent(this);
    connect(socket, &QTcpSocket::readyRead, this, &channels::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &channels::onDisconnected);
}

void channels::onReadyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "data received: " << data;

    socket->write("\nwelcome user\r\n");
}

void channels::onDisconnected()
{
    qDebug() << "Client disconnected";
    this->deleteLater();
}
