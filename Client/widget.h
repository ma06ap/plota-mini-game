#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QList>
#include <QPair>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

    void onBtnConnectClicked();
    void onBtnLoginClicked();
    void onBtnSignupClicked();
    void onBtnBackToMenuClicked();

private:
    QTcpSocket *socket;

    QStackedWidget *stackedWidget;
    QWidget *pageLogin;
    QWidget *pageSelect;
    QWidget *pageGame;

    QLineEdit *leIp;
    QLineEdit *lePort;
    QPushButton *btnConnect;
    QLineEdit *leUsername;
    QLineEdit *lePassword;
    QPushButton *btnLogin;
    QPushButton *btnSignup;
    QLabel *lblStatus;

    QLineEdit *leJoinId;

    QPushButton *btnBack;
    QLabel *lblGameTitle;

    QLabel *lblRoomId;
    QLabel *lblHostTime;
    QLabel *lblGuestTime;

    QWidget *boardContainer;
    QGridLayout *boardGrid;
    QTextEdit *txtGameLog;

    QString activeGame;
    QPair<int,int> selectedCell = {-1, -1};
    QList<QPair<int,int>> validMoveCells;
    QList<QPair<int,int>> selectablePieces;

    void setupUI();
    void setupLoginPage();
    void setupSelectPage();
    void setupGamePage();

    void sendCommand(QString cmd);

    void initBoardGrid(int rows, int cols);
    void renderBoard(QString data);
    void handleStateMessage(QString state);
    void applyCheckersHighlights();
    void clearCheckersHighlights();

    void clearLayout(QLayout *layout);
    QString getCellStyle(QString color, QString role = "");
    QString getEmptyCellStyle(int r, int c);

    void onBoardCellClicked(int r, int c);
};

#endif // WIDGET_H
