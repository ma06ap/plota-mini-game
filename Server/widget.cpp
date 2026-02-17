#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QDateTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &Widget::newConnection);
}

Widget::~Widget()
{
    if(server->isListening()) {
        server->close();
    }
    delete ui;
}

void Widget::on_btnStart_clicked()
{
    int port = ui->txtPort->text().toInt();

    if(!server->listen(QHostAddress::Any, port)) {
        logMessage("Error: Could not start server. " + server->errorString());
        return;
    }

    logMessage("Server started on port " + QString::number(port));
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->txtPort->setEnabled(false);
}

void Widget::on_btnStop_clicked()
{
    foreach(QTcpSocket *socket, clients) {
        socket->close();
    }
    server->close();

    logMessage("Server stopped.");
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->txtPort->setEnabled(true);
}

void Widget::newConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        clients.append(socket);

        logMessage("Client connected: " + QString::number(socket->socketDescriptor()));

        connect(socket, &QTcpSocket::readyRead, this, &Widget::readData);
        connect(socket, &QTcpSocket::disconnected, this, &Widget::clientDisconnected);

        socket->write("Welcome to Qt Server!\r\n");
    }
}

void Widget::readData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    logMessage("Data from client " + QString::number(socket->socketDescriptor()) + ": " + QString(data));

}

void Widget::clientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    logMessage("Client disconnected: " + QString::number(socket->socketDescriptor()));

    clients.removeAll(socket);
    socket->deleteLater();
}

void Widget::logMessage(const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->txtLog->append("[" + timestamp + "] " + msg);
}
