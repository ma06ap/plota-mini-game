#include "GameServer.h"
#include <QDebug>
#include <string>

GameServer::GameServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    gameLogic = nullptr;
}

bool GameServer::start(int port) {
    if (server->listen(QHostAddress::Any, port)) {
        emit logMessage("Server started on port " + QString::number(port));
        return true;
    } else {
        emit logMessage("Error: " + server->errorString());
        return false;
    }
}

void GameServer::stop() {
    for(QTcpSocket* client : clients) {
        client->disconnectFromHost();
    }
    server->close();
    emit logMessage("Server stopped.");
}

void GameServer::onNewConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    clients.append(socket);

    connect(socket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);

    emit logMessage("Client connected. IP: " + socket->peerAddress().toString());
    emit playerCountChanged(clients.size());

    if (gameLogic != nullptr && clients.size() == 2) {
        broadcastGameStart();
    }
}

void GameServer::onReadyRead() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    while (senderSocket->canReadLine()) {
        QString data = QString::fromUtf8(senderSocket->readLine()).trimmed();

        if(data.startsWith("SELECT_GAME") || data.startsWith("CLICK"))
            emit logMessage("Cmd from client: " + data);

        if (data.startsWith("SELECT_GAME:")) {
            QString gameName = data.mid(12);
            resetGame(gameName);
            if (clients.size() == 2) broadcastGameStart();
        }
        else if (data.startsWith("CLICK:")) {
            if (!gameLogic) return;

            QStringList parts = data.split(":");
            if (parts.size() == 3) {
                std::string r = parts[1].toStdString();
                std::string c = parts[2].toStdString();
                std::string result;
                std::string name = gameLogic->getName();

                try {
                    if (name == "Checkers") {
                        Checkers* chk = dynamic_cast<Checkers*>(gameLogic);
                        if (chk->isPieceSelected()) result = gameLogic->input("move " + r + " " + c);
                        else result = gameLogic->input("select " + r + " " + c);
                    } else if (name == "Othello") {
                        result = gameLogic->input("put " + r + " " + c);
                    } else if (name == "Connect-4") {
                        result = gameLogic->input("put " + c);
                    }

                    emit logMessage("Logic Result: " + QString::fromStdString(result));
                    broadcastBoard();
                } catch (const std::exception& e) {
                    emit logMessage("Logic Error: " + QString(e.what()));
                }
            }
        }
    }
}

void GameServer::resetGame(QString gameName) {
    if (gameLogic) delete gameLogic;

    if (gameName == "Checkers") gameLogic = new Checkers();
    else if (gameName == "Othello") gameLogic = new Othello();
    else if (gameName == "Connect-4") gameLogic = new ConnectFour();

    emit logMessage("Game switched to: " + gameName);
}

void GameServer::broadcastGameStart() {
    if (!gameLogic) return;
    emit logMessage("Broadcasting Game Start...");
    QString type = QString::fromStdString(gameLogic->getName());
    for (QTcpSocket* client : clients) {
        sendToClient(client, "START_GAME:" + type);
    }
    broadcastBoard();
}

void GameServer::broadcastBoard() {
    if (!gameLogic) return;
    QString boardStr = QString::fromStdString(gameLogic->input("getboard"));
    for (QTcpSocket* client : clients) {
        sendToClient(client, "BOARD:" + boardStr);
    }
}

void GameServer::sendToClient(QTcpSocket* socket, QString msg) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write((msg + "\n").toUtf8());
        socket->flush();
    }
}

void GameServer::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(socket);
    socket->deleteLater();
    emit logMessage("Client disconnected.");
    emit playerCountChanged(clients.size());

    if (clients.isEmpty() && gameLogic) {
        delete gameLogic;
        gameLogic = nullptr;
        emit logMessage("All players left. Game logic reset.");
    }
}
