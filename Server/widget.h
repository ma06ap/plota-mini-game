#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void newConnection();
    void readData();
    void clientDisconnected();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QList<QTcpSocket*> clients;

    void logMessage(const QString &msg);
};
#endif // WIDGET_H
