#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "AuthHandler.h"
#include "Backend/Games/Checkers/Checkers.h"
#include "Backend/Games/Othello/Othello.h"
#include "Backend/Games/Connect-4/Connect-4.h"

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);
    bool start(int port);
    void stop();

signals:
    void logMessage(QString msg);
    void playerCountChanged(int count);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    Game *gameLogic;
    AuthHandler *auth;

    void broadcastGameStart();
    void broadcastBoard();
    void sendToClient(QTcpSocket* socket, QString msg);
    void resetGame(QString gameName);
};

#endif // GAMESERVER_H
