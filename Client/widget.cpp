#include "widget.h"
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("Plota Game Hub - Online");
    this->setMinimumSize(500, 750);

    setupUI();
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Widget::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Widget::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Widget::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &Widget::onErrorOccurred);
}

Widget::~Widget() {}

QString Widget::hashPassword(const QString& pass) {
    return QString(QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256).toHex());
}

void Widget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QString appStyle =
        "QWidget { background-color: #2C3E50; color: #ECF0F1; font-family: 'Segoe UI', sans-serif; }"
        "QLineEdit, QComboBox { background-color: #34495E; border: 2px solid #5D6D7E; border-radius: 8px; padding: 10px; font-size: 11pt; color: white; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #3498DB; }"
        "QPushButton { border-radius: 8px; padding: 10px; font-weight: bold; font-size: 11pt; border: none; }"
        "QGroupBox { border: 2px solid #34495E; border-radius: 10px; margin-top: 25px; font-weight: bold; font-size: 12pt; color: #3498DB; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; }";

    this->setStyleSheet(appStyle);

    stackedWidget = new QStackedWidget(this);

    setupLoginPage();
    setupSelectPage();
    setupGamePage();

    stackedWidget->addWidget(pageLogin);
    stackedWidget->addWidget(pageSelect);
    stackedWidget->addWidget(pageGame);

    mainLayout->addWidget(stackedWidget);
    stackedWidget->setCurrentIndex(0);
}

void Widget::setupLoginPage() {
    pageLogin = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageLogin);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setAlignment(Qt::AlignTop);

    QLabel *lblTitle = new QLabel("PLOTA GAME HUB");
    lblTitle->setStyleSheet("font-size: 24pt; font-weight: bold; color: #E67E22; margin-bottom: 10px;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QGroupBox *grpConn = new QGroupBox("Connection");
    QVBoxLayout *lConn = new QVBoxLayout(grpConn);
    leIp = new QLineEdit("127.0.0.1"); leIp->setPlaceholderText("IP Address");
    lePort = new QLineEdit("1234");    lePort->setPlaceholderText("Port");
    btnConnect = new QPushButton("Connect to Server");
    btnConnect->setCursor(Qt::PointingHandCursor);
    btnConnect->setStyleSheet("background-color: #27AE60; color: white;");
    connect(btnConnect, &QPushButton::clicked, this, &Widget::onBtnConnectClicked);
    lConn->addWidget(leIp);
    lConn->addWidget(lePort);
    lConn->addWidget(btnConnect);

    authTabs = new QTabWidget();
    authTabs->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #34495E; border-radius: 5px; margin-top: 10px; } "
        "QTabBar::tab { background: #2C3E50; color: white; padding: 10px 20px; border-top-left-radius: 5px; border-top-right-radius: 5px; } "
        "QTabBar::tab:selected { background: #2980B9; font-weight: bold; }");

    tabLogin = new QWidget();
    QVBoxLayout *lLogin = new QVBoxLayout(tabLogin);
    lLogin->setSpacing(15);
    leUsername = new QLineEdit(); leUsername->setPlaceholderText("Username");
    lePassword = new QLineEdit(); lePassword->setPlaceholderText("Password");
    lePassword->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton("Login");
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet("background-color: #2980B9; color: white;");
    connect(btnLogin, &QPushButton::clicked, this, &Widget::onBtnLoginClicked);

    btnForgotPassword = new QPushButton("Forgot Password?");
    btnForgotPassword->setStyleSheet("background: transparent; color: #3498DB; text-decoration: underline; border: none; font-size: 10pt;");
    btnForgotPassword->setCursor(Qt::PointingHandCursor);
    connect(btnForgotPassword, &QPushButton::clicked, this, &Widget::onBtnForgotClicked);

    lLogin->addWidget(leUsername);
    lLogin->addWidget(lePassword);
    lLogin->addWidget(btnLogin);
    lLogin->addWidget(btnForgotPassword);
    lLogin->addStretch();

    tabSignup = new QWidget();
    QVBoxLayout *lSignup = new QVBoxLayout(tabSignup);
    lSignup->setSpacing(10);
    leSignupName = new QLineEdit(); leSignupName->setPlaceholderText("Full Name");
    leSignupUsername = new QLineEdit(); leSignupUsername->setPlaceholderText("Username");
    leSignupPhone = new QLineEdit(); leSignupPhone->setPlaceholderText("Phone (e.g. 09123456789)");
    leSignupEmail = new QLineEdit(); leSignupEmail->setPlaceholderText("Email");
    leSignupPassword = new QLineEdit(); leSignupPassword->setPlaceholderText("Password (Min 8 chars)");
    leSignupPassword->setEchoMode(QLineEdit::Password);

    btnSignup = new QPushButton("Sign Up");
    btnSignup->setCursor(Qt::PointingHandCursor);
    btnSignup->setStyleSheet("background-color: #8E44AD; color: white;");
    connect(btnSignup, &QPushButton::clicked, this, &Widget::onBtnSignupClicked);

    lSignup->addWidget(leSignupName);
    lSignup->addWidget(leSignupUsername);
    lSignup->addWidget(leSignupPhone);
    lSignup->addWidget(leSignupEmail);
    lSignup->addWidget(leSignupPassword);
    lSignup->addWidget(btnSignup);
    lSignup->addStretch();

    authTabs->addTab(tabLogin, "Login");
    authTabs->addTab(tabSignup, "Sign Up");
    authTabs->setEnabled(false);

    lblStatus = new QLabel("Not Connected");
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setStyleSheet("color: #95A5A6; margin-top: 10px;");

    layout->addWidget(lblTitle);
    layout->addWidget(grpConn);
    layout->addWidget(authTabs);
    layout->addWidget(lblStatus);
}

void Widget::setupSelectPage() {
    pageSelect = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageSelect);
    layout->setSpacing(30);
    layout->setContentsMargins(40, 60, 40, 60);

    QLabel *lblMode = new QLabel("Select Game Mode");
    lblMode->setStyleSheet("font-size: 20pt; color: white; font-weight: bold;");
    lblMode->setAlignment(Qt::AlignCenter);

    QGroupBox *grpCreate = new QGroupBox("Create New Room");
    QVBoxLayout *lCreate = new QVBoxLayout(grpCreate);

    QComboBox *comboGame = new QComboBox();
    comboGame->addItems({"Connect-4", "Checkers", "Othello"});
    comboGame->setStyleSheet("padding: 8px; font-size: 12pt;");

    QPushButton *btnCreate = new QPushButton("Create Room & Get Code");
    btnCreate->setCursor(Qt::PointingHandCursor);
    btnCreate->setStyleSheet("background-color: #E67E22; color: white; height: 40px;");

    connect(btnCreate, &QPushButton::clicked, [this, comboGame](){
        activeGame = comboGame->currentText();
        sendCommand("CREATE_GAME:" + activeGame);
    });

    lCreate->addWidget(new QLabel("Choose Game Type:"));
    lCreate->addWidget(comboGame);
    lCreate->addWidget(btnCreate);

    QGroupBox *grpJoin = new QGroupBox("Join Friend's Room");
    QVBoxLayout *lJoin = new QVBoxLayout(grpJoin);

    leJoinId = new QLineEdit();
    leJoinId->setPlaceholderText("Enter 4-Digit Room ID");
    leJoinId->setAlignment(Qt::AlignCenter);
    leJoinId->setMaxLength(4);

    QPushButton *btnJoin = new QPushButton("Join Room");
    btnJoin->setCursor(Qt::PointingHandCursor);
    btnJoin->setStyleSheet("background-color: #8E44AD; color: white; height: 40px;");

    connect(btnJoin, &QPushButton::clicked, [this](){
        if(leJoinId->text().isEmpty()) return;
        sendCommand("JOIN_GAME:" + leJoinId->text());
    });

    lJoin->addWidget(leJoinId);
    lJoin->addWidget(btnJoin);

    layout->addWidget(lblMode);
    layout->addWidget(grpCreate);
    layout->addWidget(grpJoin);
    layout->addStretch();
}

void Widget::setupGamePage() {
    pageGame = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(pageGame);
    layout->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout *header = new QHBoxLayout();
    btnBack = new QPushButton("Quit Game");
    btnBack->setFixedSize(120, 40);
    btnBack->setStyleSheet("background-color: #C0392B; color: white;");
    connect(btnBack, &QPushButton::clicked, this, &Widget::onBtnBackToMenuClicked);

    lblGameTitle = new QLabel("Waiting...");
    lblGameTitle->setStyleSheet("font-size: 16pt; font-weight: bold; color: #F1C40F;");
    lblGameTitle->setAlignment(Qt::AlignCenter);

    header->addWidget(btnBack);
    header->addWidget(lblGameTitle);
    header->addStretch();

    QHBoxLayout *infoLayout = new QHBoxLayout();
    lblHostTime = new QLabel("Host: 03:00");
    lblHostTime->setStyleSheet("color: #FF5252; font-weight: bold; font-size: 12pt;");

    lblRoomId = new QLabel("Room: ----");
    lblRoomId->setStyleSheet("color: white; font-weight: bold; font-size: 14pt; border: 1px solid white; padding: 5px; border-radius: 5px;");
    lblRoomId->setAlignment(Qt::AlignCenter);

    lblGuestTime = new QLabel("Guest: 03:00");
    lblGuestTime->setStyleSheet("color: #FFEB3B; font-weight: bold; font-size: 12pt;");

    infoLayout->addWidget(lblHostTime);
    infoLayout->addStretch();
    infoLayout->addWidget(lblRoomId);
    infoLayout->addStretch();
    infoLayout->addWidget(lblGuestTime);

    boardContainer = new QWidget();
    boardGrid = new QGridLayout(boardContainer);
    boardGrid->setSpacing(4);
    boardContainer->setLayout(boardGrid);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(boardContainer);
    centerLayout->addStretch();

    txtGameLog = new QTextEdit();
    txtGameLog->setMaximumHeight(100);
    txtGameLog->setReadOnly(true);
    txtGameLog->setStyleSheet("background-color: #1C2833; color: #2ECC71; border: 1px solid #5D6D7E;");

    layout->addLayout(header);
    layout->addLayout(infoLayout);
    layout->addSpacing(10);
    layout->addLayout(centerLayout);
    layout->addWidget(txtGameLog);
}

void Widget::sendCommand(QString cmd) {
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->write((cmd + "\n").toUtf8());
        socket->flush();
    } else {
        txtGameLog->append("Error: No connection.");
    }
}

void Widget::onBtnConnectClicked() {
    lblStatus->setText("Connecting...");
    lblStatus->setStyleSheet("color: yellow;");
    socket->connectToHost(leIp->text(), lePort->text().toInt());
}

void Widget::onConnected() {
    lblStatus->setText("Connected! Please Login or Sign Up.");
    lblStatus->setStyleSheet("color: #2ECC71;");
    btnConnect->setEnabled(false);
    btnConnect->setText("Connected");
    authTabs->setEnabled(true);
}

void Widget::onDisconnected() {
    lblStatus->setText("Disconnected.");
    lblStatus->setStyleSheet("color: red;");
    btnConnect->setEnabled(true);
    btnConnect->setText("Connect to Server");
    authTabs->setEnabled(false);
    stackedWidget->setCurrentIndex(0);
}

void Widget::onErrorOccurred(QAbstractSocket::SocketError) {
    lblStatus->setText("Error: " + socket->errorString());
}

void Widget::onBtnLoginClicked() {
    if (leUsername->text().isEmpty() || lePassword->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and Password cannot be empty.");
        return;
    }
    QString hashedPass = hashPassword(lePassword->text());
    sendCommand("LOGIN:" + leUsername->text() + ":" + hashedPass);
}

void Widget::onBtnSignupClicked() {
    QString name = leSignupName->text();
    QString user = leSignupUsername->text();
    QString phone = leSignupPhone->text();
    QString email = leSignupEmail->text();
    QString pass = leSignupPassword->text();

    if (name.isEmpty() || user.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name and Username are required.");
        return;
    }

    QRegularExpression phoneRegex("^09\\d{9}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid phone number. Must start with 09 and be 11 digits.");
        return;
    }

    QRegularExpression emailRegex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid email address.");
        return;
    }

    if (pass.length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters.");
        return;
    }

    QString hashedPass = hashPassword(pass);
    sendCommand("SIGNUP:" + user + ":" + hashedPass + ":" + name + ":" + phone + ":" + email);
}

void Widget::onBtnForgotClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Forgot Password");
    dialog.setStyleSheet(this->styleSheet());
    QFormLayout form(&dialog);

    QLineEdit *txtUser = new QLineEdit(&dialog);
    QLineEdit *txtPhone = new QLineEdit(&dialog);
    QLineEdit *txtNewPass = new QLineEdit(&dialog);
    txtNewPass->setEchoMode(QLineEdit::Password);

    form.addRow("Username:", txtUser);
    form.addRow("Phone Number:", txtPhone);
    form.addRow("New Password (Min 8):", txtNewPass);

    QPushButton *btnSubmit = new QPushButton("Reset Password", &dialog);
    btnSubmit->setStyleSheet("background-color: #E67E22; color: white; margin-top: 10px;");
    form.addRow(btnSubmit);

    connect(btnSubmit, &QPushButton::clicked, [&]() {
        if (txtNewPass->text().length() < 8) {
            QMessageBox::warning(&dialog, "Error", "Password must be at least 8 characters.");
            return;
        }
        QString hashedPass = hashPassword(txtNewPass->text());
        sendCommand("RESET_PASS:" + txtUser->text() + ":" + txtPhone->text() + ":" + hashedPass);
        dialog.accept();
    });

    dialog.exec();
}

void Widget::onBtnBackToMenuClicked() {
    sendCommand("LEAVE_GAME");

    stackedWidget->setCurrentIndex(1);
    txtGameLog->clear();
    activeGame = "";
    clearLayout(boardGrid);
    clearCheckersHighlights();

    lblRoomId->setText("Room: ----");
    lblHostTime->setText("Host: 03:00");
    lblGuestTime->setText("Guest: 03:00");
}

void Widget::onReadyRead() {
    while(socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        if (line == "LOGIN_SUCCESS") {
            stackedWidget->setCurrentIndex(1);
        }
        else if (line == "LOGIN_FAIL") {
            QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        }
        else if (line == "SIGNUP_SUCCESS") {
            QMessageBox::information(this, "Success", "Account created successfully. You can now login.");
            authTabs->setCurrentIndex(0);
            leSignupPassword->clear();
        }
        else if (line == "SIGNUP_FAIL") {
            QMessageBox::warning(this, "Signup Failed", "Username already exists or server error.");
        }
        else if (line == "RESET_SUCCESS") {
            QMessageBox::information(this, "Success", "Password reset successfully. You can now login.");
        }
        else if (line == "RESET_FAIL") {
            QMessageBox::warning(this, "Reset Failed", "Username and Phone number do not match.");
        }
        else if (line.startsWith("GAME_CREATED:")) {
            QString roomId = line.mid(13);
            lblRoomId->setText("Room ID: " + roomId);
            lblGameTitle->setText("Waiting for opponent...");
            int r = 8, c = 8;
            if(activeGame == "Connect-4") { r = 6; c = 7; }
            initBoardGrid(r, c);

            stackedWidget->setCurrentIndex(2);
            boardContainer->setEnabled(false);
            txtGameLog->append("Room created! Share ID " + roomId + " with a friend.");
        }
        else if (line.startsWith("JOIN_SUCCESS:")) {
            activeGame = line.mid(13);

            int r = 8, c = 8;
            if(activeGame == "Connect-4") { r = 6; c = 7; }
            initBoardGrid(r, c);

            stackedWidget->setCurrentIndex(2);
            txtGameLog->append("Joined room successfully.");
        }
        else if (line.startsWith("START_GAME:")) {
            QStringList parts = line.split(":");
            activeGame = parts[1];
            QString roomId = parts[2];

            lblRoomId->setText("Room ID: " + roomId);
            lblGameTitle->setText("Playing: " + activeGame);
            int r = 8, c = 8;
            if(activeGame == "Connect-4") { r = 6; c = 7; }
            initBoardGrid(r, c);

            boardContainer->setEnabled(true);
            txtGameLog->append(">>> GAME STARTED! <<<");

            if (activeGame == "Connect-4")      txtGameLog->append("Red (Host) starts first.");
            else if (activeGame == "Checkers")  txtGameLog->append("Red (Host) starts first. Click a piece to select it.");
            else if (activeGame == "Othello")   txtGameLog->append("Black (Host) starts first. Valid moves are highlighted.");
        }
        else if (line.startsWith("TIME:")) {
            QStringList parts = line.split(":");
            if(parts.size() == 3) {
                int h = parts[1].toInt();
                int g = parts[2].toInt();
                lblHostTime->setText(QString("Host: %1:%2").arg(h/60, 2, 10, QChar('0')).arg(h%60, 2, 10, QChar('0')));
                lblGuestTime->setText(QString("Guest: %1:%2").arg(g/60, 2, 10, QChar('0')).arg(g%60, 2, 10, QChar('0')));
            }
        }
        else if (line.startsWith("GAME_OVER:")) {
            QString reason = line.mid(10);
            clearCheckersHighlights();
            QMessageBox::information(this, "Game Over", "Result: " + reason);
            onBtnBackToMenuClicked();
        }
        else if (line.startsWith("BOARD:")) {
            renderBoard(line.mid(6));
        }
        else if (line.startsWith("STATE:")) {
            handleStateMessage(line.mid(6));
        }
        else if (line.startsWith("JOIN_FAIL:")) {
            QMessageBox::warning(this, "Join Failed", line.mid(10));
        }
    }
}

void Widget::handleStateMessage(QString state) {
    if (activeGame != "Checkers" && activeGame != "Othello") return;

    clearCheckersHighlights();

    QStringList tokens = state.split(' ', Qt::SkipEmptyParts);
    if (tokens.size() < 1) return;

    QString color = tokens[0];

    // --- Othello Logic ---
    if (activeGame == "Othello") {
        if (tokens.size() >= 3 && tokens[1] != "Wins" && tokens[0] != "Draw") {
            for (int k = 1; k + 1 < tokens.size(); k += 2) {
                validMoveCells.append({tokens[k].toInt(), tokens[k+1].toInt()});
            }
            txtGameLog->append(color + "'s turn. Valid moves are highlighted.");
        }
        applyCheckersHighlights();
        return;
    }

    if (tokens.size() >= 2 && (tokens[1] == "selected" || tokens[1] == "continue")) {
        if (tokens.size() < 4) return;
        selectedCell = {tokens[2].toInt(), tokens[3].toInt()};
        for (int k = 4; k + 1 < tokens.size(); k += 2) {
            validMoveCells.append({tokens[k].toInt(), tokens[k+1].toInt()});
        }
        if (tokens[1] == "selected")
            txtGameLog->append(color + " selected piece at (" + QString::number(selectedCell.first) + "," + QString::number(selectedCell.second) + "). Choose destination.");
        else
            txtGameLog->append(color + " continues jump! Choose next jump.");

    } else if (tokens.size() >= 3 && tokens[1] != "Win") {
        for (int k = 1; k + 1 < tokens.size(); k += 2) {
            selectablePieces.append({tokens[k].toInt(), tokens[k+1].toInt()});
        }
        txtGameLog->append(color + "'s turn. Click one of the highlighted pieces.");
    }

    applyCheckersHighlights();
}

void Widget::clearCheckersHighlights() {
    selectedCell = {-1, -1};
    validMoveCells.clear();
    selectablePieces.clear();
}

void Widget::applyCheckersHighlights() {
    if (activeGame != "Checkers" && activeGame != "Othello") return;

    for (const auto& p : selectablePieces) {
        QLayoutItem *item = boardGrid->itemAtPosition(p.first, p.second);
        if (item && item->widget()) {
            QString current = item->widget()->styleSheet();
            item->widget()->setStyleSheet(current + " border: 3px solid #FFFFFF; border-radius: 27px;");
        }
    }

    if (selectedCell.first >= 0) {
        QLayoutItem *item = boardGrid->itemAtPosition(selectedCell.first, selectedCell.second);
        if (item && item->widget()) {
            QString current = item->widget()->styleSheet();
            item->widget()->setStyleSheet(current + " border: 4px solid #FFD700; border-radius: 27px;");
        }
    }

    for (const auto& d : validMoveCells) {
        QLayoutItem *item = boardGrid->itemAtPosition(d.first, d.second);
        if (item && item->widget()) {
            QString bgColor;
            if (activeGame == "Checkers") {
                bgColor = ((d.first + d.second) % 2 == 1) ? "#8D6E63" : "#D7CCC8";
            } else {
                bgColor = "#1B5E20";
            }
            item->widget()->setStyleSheet(
                "background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.35, fx:0.5, fy:0.5, "
                "stop:0 #00E676, stop:0.36 #00E676, stop:0.37 " + bgColor + ", stop:1 " + bgColor + "); "
                                                    "border: 2px solid #00C853; border-radius: 0;");
        }
    }
}

void Widget::onBoardCellClicked(int r, int c) {
    sendCommand("CLICK:" + QString::number(r) + ":" + QString::number(c));
}

void Widget::clearLayout(QLayout *layout) {
    if (!layout) return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) delete item->widget();
        if (item->layout()) clearLayout(item->layout());
        delete item;
    }
}

void Widget::initBoardGrid(int rows, int cols) {
    clearLayout(boardGrid);
    clearCheckersHighlights();

    QString style = "border-radius: 8px; border: 4px solid #1a252f; ";
    if (activeGame == "Checkers")       style += "background-color: #5D4037;";
    else if (activeGame == "Othello")   style += "background-color: #1B5E20;";
    else if (activeGame == "Connect-4") style += "background-color: #0D47A1; padding: 10px;";

    boardContainer->setStyleSheet(style);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QPushButton *btn = new QPushButton();
            btn->setFixedSize(55, 55);
            btn->setCursor(Qt::PointingHandCursor);
            btn->setStyleSheet(getEmptyCellStyle(i, j));

            connect(btn, &QPushButton::clicked, [this, i, j](){
                onBoardCellClicked(i, j);
            });
            boardGrid->addWidget(btn, i, j);
        }
    }
}

void Widget::renderBoard(QString data) {
    int rows = 8, cols = 8;
    if (activeGame == "Connect-4") { rows = 6; cols = 7; }

    if (boardGrid->count() == 0) initBoardGrid(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QLayoutItem *item = boardGrid->itemAtPosition(i, j);
            if (item && item->widget()) {
                item->widget()->setStyleSheet(getEmptyCellStyle(i, j));
            }
        }
    }

    QStringList tokens = data.split(' ', Qt::SkipEmptyParts);
    if (tokens.isEmpty()) {
        applyCheckersHighlights();
        return;
    }

    QStringList cleanTokens;
    for(const QString &t : tokens) {
        if(!t.trimmed().isEmpty()) cleanTokens.append(t);
    }

    if (activeGame == "Checkers") {
        for (int k = 0; k + 3 < cleanTokens.size(); k += 4) {
            bool ok1, ok2;
            int r = cleanTokens[k].toInt(&ok1);
            int c = cleanTokens[k+1].toInt(&ok2);
            QString color = cleanTokens[k+2];
            QString role  = cleanTokens[k+3];

            if (ok1 && ok2) {
                QLayoutItem *item = boardGrid->itemAtPosition(r, c);
                if (item && item->widget()) {
                    item->widget()->setStyleSheet(getCellStyle(color, role));
                }
            }
        }
    }
    else {
        for (int k = 0; k + 2 < cleanTokens.size(); k += 3) {
            int r = cleanTokens[k].toInt();
            int c = cleanTokens[k+1].toInt();
            QString color = cleanTokens[k+2];

            QLayoutItem *item = boardGrid->itemAtPosition(r, c);
            if (item && item->widget()) {
                item->widget()->setStyleSheet(getCellStyle(color));
            }
        }
    }

    applyCheckersHighlights();
}

QString Widget::getEmptyCellStyle(int r, int c) {
    if (activeGame == "Checkers") {
        if ((r + c) % 2 == 1)
            return "background-color: #8D6E63; border: none; border-radius: 0;";
        else
            return "background-color: #D7CCC8; border: none; border-radius: 0;";
    }
    return "background-color: rgba(0,0,0,0.2); border-radius: 27px; border: 1px solid rgba(255,255,255,0.1);";
}

QString Widget::getCellStyle(QString color, QString role) {
    QString style = "border-radius: 27px; border: 1px solid rgba(0,0,0,0.5); ";
    QString grad;

    if (color == "Red")         grad = "qradialgradient(cx:0.4, cy:0.4, radius:0.6, fx:0.4, fy:0.4, stop:0 #FF5252, stop:1 #B71C1C)";
    else if (color == "Yellow") grad = "qradialgradient(cx:0.4, cy:0.4, radius:0.6, fx:0.4, fy:0.4, stop:0 #FFEB3B, stop:1 #F9A825)";
    else if (color == "Black")  grad = "qradialgradient(cx:0.4, cy:0.4, radius:0.6, fx:0.4, fy:0.4, stop:0 #444, stop:1 #000)";
    else if (color == "White")  grad = "qradialgradient(cx:0.4, cy:0.4, radius:0.6, fx:0.4, fy:0.4, stop:0 #FFF, stop:1 #CCC)";
    else grad = "transparent";

    style += "background-color: " + grad + ";";

    if (role == "King") style += " border: 4px solid #FFD700;";

    return style;
}
