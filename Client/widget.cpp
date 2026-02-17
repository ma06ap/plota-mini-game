#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    ted = new QTextEdit("Press button to connect");
    pbnConnect = new QPushButton("Connect");

    mainLayout->addWidget(pbnConnect);
    mainLayout->addWidget(ted);

    ted->setReadOnly(true);
    pbnConnect->setEnabled(true);


    pbnConnect->setMinimumHeight(50);
    pbnConnect->setMaximumHeight(50);
    pbnConnect->setStyleSheet(
        "QPushButton { background-color: green; }"
        "QPushButton:pressed { background-color: darkgreen; }"
        );
    pbnConnect->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(pbnConnect, SIGNAL(clicked()), this, SLOT(connectingToServer()));




}

Widget::~Widget()
{
    delete ui;
}

void Widget::readingData()
{
    QByteArray data = ClientSocket->readAll();

    ted->setTextColor(Qt::white);
    ted->append(data);

}

void Widget::writingData()
{
    ted->setTextColor(Qt::green);
    ted->append("written successfully\n");

}

void Widget::connectedToServer()
{
    ted->setTextColor(Qt::green);
    ted->append("connected successfully");

    ClientSocket->write("hello\n");
}

void Widget::disconnectingFromServer()
{
    ted->setTextColor(Qt::red);
    ted->append("connection lost\n");

}

void Widget::connectingToServer()
{
    ClientSocket = new QTcpSocket();
    ClientSocket->connectToHost("127.0.0.1", 1025);
    ted->setTextColor(Qt::red);
    ted->append("connecting...\r\n");

    connect(ClientSocket, SIGNAL(connected()), this, SLOT(connectedToServer()));
    connect(ClientSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(writingData()));
    connect(ClientSocket, SIGNAL(readyRead()), this, SLOT(readingData()));
    connect(ClientSocket, SIGNAL(disconnected()), this, SLOT(disconnectingFromServer()));




}
