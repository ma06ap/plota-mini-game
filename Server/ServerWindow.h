#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "GameServer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void updateLog(QString msg);
    void updatePlayerCount(int count);

private:
    Ui::ServerWindow *ui;
    GameServer *gameServer;
};
#endif // SERVERWINDOW_H
