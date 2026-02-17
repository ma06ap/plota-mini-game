#include "ServerWindow.h"
#include "ui_ServerWindow.h"
#include <QDateTime>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    gameServer = new GameServer(this);

    connect(gameServer, &GameServer::logMessage, this, &ServerWindow::updateLog);
    connect(gameServer, &GameServer::playerCountChanged, this, &ServerWindow::updatePlayerCount);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_btnStart_clicked()
{
    int port = ui->txtPort->text().toInt();
    if (gameServer->start(port)) {
        ui->btnStart->setEnabled(false);
        ui->txtPort->setEnabled(false);
        ui->btnStop->setEnabled(true);
    }
}

void ServerWindow::on_btnStop_clicked()
{
    gameServer->stop();
    ui->btnStart->setEnabled(true);
    ui->txtPort->setEnabled(true);
    ui->btnStop->setEnabled(false);
    updatePlayerCount(0);
}

void ServerWindow::updateLog(QString msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("[HH:mm:ss] ");
    ui->txtLog->append(timestamp + msg);
}

void ServerWindow::updatePlayerCount(int count)
{
    ui->lblPlayers->setText("Connected Players: " + QString::number(count));
}
