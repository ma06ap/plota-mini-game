#include "GameServer.h"
#include <QRandomGenerator>
#include <utility>

GameServer::GameServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    auth = new AuthHandler("users.csv");
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

bool GameServer::start(int port) {
    if (server->listen(QHostAddress::Any, port)) {
        emit logMessage("Server started on port " + QString::number(port));
        return true;
    }
    emit logMessage("Error: " + server->errorString());
    return false;
}

void GameServer::stop() {
    for(auto session : sessions) {
        if(session->gameLogic) delete session->gameLogic;
        if(session->turnTimer) session->turnTimer->stop();
        delete session;
    }
    sessions.clear();
    for(auto client : std::as_const(allClients)) {
        client->disconnectFromHost();
    }
    allClients.clear();

    server->close();
    emit logMessage("Server stopped.");
}

void GameServer::onNewConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    allClients.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &GameServer::onDisconnected);
    emit logMessage("Client connected: " + socket->peerAddress().toString());
}

void GameServer::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while (clientSocket->canReadLine()) {
        QString line = QString::fromUtf8(clientSocket->readLine()).trimmed();

        if (line.startsWith("LOGIN:")) {
            QStringList parts = line.split(":");
            if(parts.size() >= 3) {
                if(auth->login(parts[1], parts[2])) sendToClient(clientSocket, "LOGIN_SUCCESS");
                else sendToClient(clientSocket, "LOGIN_FAIL");
            }
        }
        else if (line.startsWith("SIGNUP:")) {
            QStringList parts = line.split(":");
            if(parts.size() >= 3) {
                if(auth->signup(parts[1], parts[2])) sendToClient(clientSocket, "SIGNUP_SUCCESS");
                else sendToClient(clientSocket, "SIGNUP_FAIL");
            }
        }
        else if (line.startsWith("CREATE_GAME:")) {
            QString gameType = line.mid(12);
            handleCreateGame(clientSocket, gameType);
        }
        else if (line.startsWith("JOIN_GAME:")) {
            QString roomId = line.mid(10);
            handleJoinGame(clientSocket, roomId);
        }
        else if (line.startsWith("CLICK:")) {
            QStringList parts = line.split(":");
            if (parts.size() == 3) {
                handleMove(clientSocket, parts[1].toInt(), parts[2].toInt());
            }
        }
        else if (line == "LEAVE_GAME") {
            handleLeave(clientSocket);
        }
    }
}

void GameServer::handleCreateGame(QTcpSocket* senderSocket, QString gameType) {
    QString roomId = generateRoomId();

    GameSession* session = new GameSession();
    session->roomId = roomId;
    session->gameType = gameType;
    session->host = senderSocket;
    session->hostTimeLeft = 180;
    session->guestTimeLeft = 180;

    if (gameType == "Checkers")    session->gameLogic = new Checkers();
    else if (gameType == "Othello")    session->gameLogic = new Othello();
    else if (gameType == "Connect-4")  session->gameLogic = new ConnectFour();

    session->turnTimer = new QTimer(this);
    connect(session->turnTimer, &QTimer::timeout, this, &GameServer::onTurnTimerTick);

    sessions.insert(roomId, session);

    sendToClient(senderSocket, "GAME_CREATED:" + roomId);
    emit logMessage("Game created. Room: " + roomId + " (" + gameType + ")");
}

void GameServer::handleJoinGame(QTcpSocket* senderSocket, QString roomId) {
    if (!sessions.contains(roomId)) {
        sendToClient(senderSocket, "JOIN_FAIL:Invalid Room ID");
        return;
    }

    GameSession* session = sessions[roomId];
    if (session->guest != nullptr) {
        sendToClient(senderSocket, "JOIN_FAIL:Room Full");
        return;
    }

    session->guest = senderSocket;
    sendToClient(senderSocket, "JOIN_SUCCESS:" + session->gameType);

    QString startMsg = "START_GAME:" + session->gameType + ":" + session->roomId;
    broadcastToSession(session, startMsg);

    if(session->gameLogic) {
        QString board = QString::fromStdString(session->gameLogic->input("getboard"));
        broadcastToSession(session, "BOARD:" + board);

        QString state = QString::fromStdString(session->gameLogic->input("start"));
        broadcastToSession(session, "STATE:" + state);
    }

    if(session->turnTimer) session->turnTimer->start(1000);
}

void GameServer::handleMove(QTcpSocket* senderSocket, int row, int col) {
    GameSession* session = findSessionBySocket(senderSocket);
    if (!session || !session->gameLogic || !session->guest) return;

    bool isHost = (senderSocket == session->host);
    std::string currentTurn = session->gameLogic->getCurrentPlayer();
    bool isHostTurn = false;
    if (session->gameType == "Connect-4")  isHostTurn = (currentTurn == "Red");
    else if (session->gameType == "Checkers")  isHostTurn = (currentTurn == "Red");
    else if (session->gameType == "Othello")   isHostTurn = (currentTurn == "Black");

    if (isHost && !isHostTurn) return;
    if (!isHost && isHostTurn) return;

    try {
        std::string res = session->gameLogic->input(
            "select " + std::to_string(row) + " " + std::to_string(col));
        QString board = QString::fromStdString(session->gameLogic->input("getboard"));
        broadcastToSession(session, "BOARD:" + board);
        if (!res.empty() && res != "Invalid Command" && res != "Invalid command") {
            broadcastToSession(session, "STATE:" + QString::fromStdString(res));
        }
        if (res.find("Win") != std::string::npos) {
            endGame(session, isHost ? "HOST_WON" : "GUEST_WON");
            return;
        }
        if (res.find("Draw") != std::string::npos) {
            endGame(session, "DRAW");
            return;
        }

    } catch (...) {
        emit logMessage("Exception in handleMove for session " + session->roomId);
    }
}

void GameServer::handleLeave(QTcpSocket* senderSocket) {
    GameSession* session = findSessionBySocket(senderSocket);
    if (!session) return;

    QString winner = (senderSocket == session->host) ? "GUEST_WON_OPPONENT_LEFT" : "HOST_WON_OPPONENT_LEFT";
    endGame(session, winner);
}

void GameServer::onTurnTimerTick() {
    for (auto session : sessions) {
        if (!session->gameLogic || !session->guest) continue;

        std::string turn = session->gameLogic->getCurrentPlayer();

        bool isHostTurn = false;
        if (session->gameType == "Connect-4")  isHostTurn = (turn == "Red");
        else if (session->gameType == "Checkers")  isHostTurn = (turn == "Red");
        else if (session->gameType == "Othello")   isHostTurn = (turn == "Black");

        if (isHostTurn) {
            session->hostTimeLeft--;
            if (session->hostTimeLeft <= 0) { endGame(session, "GUEST_WON_TIMEOUT"); return; }
        } else {
            session->guestTimeLeft--;
            if (session->guestTimeLeft <= 0) { endGame(session, "HOST_WON_TIMEOUT"); return; }
        }

        QString timeMsg = "TIME:" + QString::number(session->hostTimeLeft) + ":" + QString::number(session->guestTimeLeft);
        broadcastToSession(session, timeMsg);
    }
}

void GameServer::endGame(GameSession* session, QString reason) {
    if(!session) return;
    if(session->turnTimer) session->turnTimer->stop();

    broadcastToSession(session, "GAME_OVER:" + reason);

    sessions.remove(session->roomId);
    delete session->gameLogic;
    delete session->turnTimer;
    delete session;
}

QString GameServer::generateRoomId() {
    int code = QRandomGenerator::global()->bounded(1000, 9999);
    return QString::number(code);
}

GameSession* GameServer::findSessionBySocket(QTcpSocket* socket) {
    for (auto session : std::as_const(sessions)) {
        if (session->host == socket || session->guest == socket) return session;
    }
    return nullptr;
}

void GameServer::broadcastToSession(GameSession* s, QString msg) {
    if(s->host) sendToClient(s->host, msg);
    if(s->guest) sendToClient(s->guest, msg);
}

void GameServer::sendToClient(QTcpSocket* socket, QString msg) {
    if(socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->write((msg + "\n").toUtf8());
        socket->flush();
    }
}

void GameServer::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;

    allClients.removeAll(socket);
    handleLeave(socket);

    socket->deleteLater();
    emit logMessage("Client disconnected.");
}
