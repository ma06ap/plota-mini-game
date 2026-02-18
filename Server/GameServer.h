#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QTimer>
#include "AuthHandler.h"
#include "Backend/Games/Checkers/Checkers.h"
#include "Backend/Games/Othello/Othello.h"
#include "Backend/Games/Connect-4/Connect-4.h"

struct GameSession {
    QString roomId;
    QString gameType;
    QTcpSocket* host = nullptr;
    QTcpSocket* guest = nullptr;
    Game* gameLogic = nullptr;
    int hostTimeLeft = 180;
    int guestTimeLeft = 180;
    QTimer* turnTimer = nullptr;
};

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
    void onTurnTimerTick();

private:
    QTcpServer *server;
    QList<QTcpSocket*> allClients;
    AuthHandler *auth;

    QMap<QString, GameSession*> sessions;

    void handleCreateGame(QTcpSocket* sender, QString gameType);
    void handleJoinGame(QTcpSocket* sender, QString roomId);
    void handleMove(QTcpSocket* sender, int row, int col);
    void handleLeave(QTcpSocket* sender);

    QString generateRoomId();
    GameSession* findSessionBySocket(QTcpSocket* socket);
    void sendToClient(QTcpSocket* socket, QString msg);
    void broadcastToSession(GameSession* session, QString msg);
    void switchTurn(GameSession* session);
    void endGame(GameSession* session, QString reason);
};

#endif // GAMESERVER_H
