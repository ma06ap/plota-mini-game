#ifndef CHANNELS_H
#define CHANNELS_H

#include <QObject>
#include <QTcpSocket>

class channels : public QObject
{
    Q_OBJECT
public:
    explicit channels(QTcpSocket* _socket, QObject *parent = nullptr);

private:
    QTcpSocket *socket;

private slots:
    void onReadyRead();
    void onDisconnected();
};

#endif // CHANNELS_H
