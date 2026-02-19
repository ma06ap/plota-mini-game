#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QDateTime>
#include "AuthHandler.h"
#include "Backend/Games/Checkers/Checkers.h"
#include "Backend/Games/Othello/Othello.h"
#include "Backend/Games/Connect-4/Connect-4.h"

struct GameSession {
    QString roomId;
    QString gameType;
    QTcpSocket *host = nullptr;
    QTcpSocket *guest = nullptr;
    Game *gameLogic = nullptr;
    QTimer *turnTimer = nullptr;
    int hostTimeLeft = 180;
    int guestTimeLeft = 180;
    QString hostColor;
};

class GameServer : public QObject {
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
    AuthHandler *auth;
    QList<QTcpSocket*> allClients;
    QMap<QString, GameSession*> sessions;

    QMap<QTcpSocket*, QString> loggedInUsers;

    void handleCreateGame(QTcpSocket* senderSocket, QString gameType, int timeLimit = 180, QString hostColor = "");
    void handleJoinGame(QTcpSocket* senderSocket, QString roomId);
    void handleMove(QTcpSocket* senderSocket, int row, int col);
    void handleLeave(QTcpSocket* senderSocket);

    QString generateRoomId();
    GameSession* findSessionBySocket(QTcpSocket* socket);
    void broadcastToSession(GameSession* session, QString msg);
    void sendToClient(QTcpSocket* socket, QString msg);
    void endGame(GameSession* session, QString reason);
};

#endif // GAMESERVER_H
