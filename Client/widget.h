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
#include <QTabWidget>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QDialog>
#include <QFormLayout>
#include <QTableWidget>
#include <QHeaderView>

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
    void onBtnForgotClicked();
    void onBtnBackToDashboardClicked();

    void openGameDashboard(QString gameName);
    void onBtnEditProfileClicked();
    void onBtnSaveProfileClicked();

private:
    QTcpSocket *socket;

    QStackedWidget *stackedWidget;
    QWidget *pageLogin;         // Index 0
    QWidget *pageMainMenu;      // Index 1
    QWidget *pageGameDashboard; // Index 2
    QWidget *pageEditProfile;   // Index 3
    QWidget *pageGame;          // Index 4

    QLineEdit *leIp;
    QLineEdit *lePort;
    QPushButton *btnConnect;
    QTabWidget *authTabs;
    QWidget *tabLogin;
    QWidget *tabSignup;
    QLineEdit *leUsername;
    QLineEdit *lePassword;
    QPushButton *btnLogin;
    QPushButton *btnForgotPassword;
    QLineEdit *leSignupName;
    QLineEdit *leSignupUsername;
    QLineEdit *leSignupPhone;
    QLineEdit *leSignupEmail;
    QLineEdit *leSignupPassword;
    QPushButton *btnSignup;
    QLabel *lblStatus;

    QPushButton *btnMenuCheckers;
    QPushButton *btnMenuOthello;
    QPushButton *btnMenuConnect4;
    QPushButton *btnMenuEditProfile;
    QPushButton *btnMenuLogout;

    QLabel *lblDashTitle;
    QLabel *lblDashScore;
    QTableWidget *tblHistory;
    QComboBox *comboTime;
    QComboBox *comboColor;
    QPushButton *btnDashCreate;
    QLineEdit *leDashJoin;
    QPushButton *btnDashJoin;
    QPushButton *btnDashBack;

    QLineEdit *leEditName;
    QLineEdit *leEditUsername;
    QLineEdit *leEditPhone;
    QLineEdit *leEditEmail;
    QLineEdit *leEditPassword;
    QPushButton *btnSaveProfile;
    QPushButton *btnEditBack;

    QPushButton *btnBack;
    QLabel *lblGameTitle;
    QLabel *lblRoomId;
    QLabel *lblHostTime;
    QLabel *lblGuestTime;
    QWidget *boardContainer;
    QGridLayout *boardGrid;
    QTextEdit *txtGameLog;

    QString activeGame;
    QString currentUser;

    QPair<int,int> selectedCell = {-1, -1};
    QList<QPair<int,int>> validMoveCells;
    QList<QPair<int,int>> selectablePieces;

    void setupUI();
    void setupLoginPage();
    void setupMainMenuPage();
    void setupGameDashboardPage();
    void setupEditProfilePage();
    void setupGamePage();

    void sendCommand(QString cmd);
    QString hashPassword(const QString& pass);

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
