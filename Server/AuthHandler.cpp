#include "AuthHandler.h"

AuthHandler::AuthHandler(QString filename) : m_filename(filename) {
    m_historyFile = "history.csv";

    QFile file(m_filename);
    if (!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "username,password,score,name,phone,email\n";
            file.close();
        }
    }

    QFile hFile(m_historyFile);
    if (!hFile.exists()) {
        if(hFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&hFile);
            out << "username,gameType,opponent,date,role,result,scoreChange\n";
            hFile.close();
        }
    }
}

bool AuthHandler::userExists(QString username) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() > 0 && parts[0] == username) return true;
    }
    return false;
}

bool AuthHandler::signup(QString username, QString password, QString name, QString phone, QString email) {
    if (userExists(username)) return false;
    QFile file(m_filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << username << "," << password << ",0," << name << "," << phone << "," << email << "\n";
    file.close();
    return true;
}

bool AuthHandler::login(QString username, QString password) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() >= 2 && parts[0] == username && parts[1] == password) return true;
    }
    return false;
}

bool AuthHandler::resetPassword(QString username, QString phone, QString newPassword) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QStringList lines;
    bool userFound = false;
    QTextStream in(&file);
    if (!in.atEnd()) lines.append(in.readLine());
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 5 && parts[0] == username && parts[4] == phone) {
            parts[1] = newPassword;
            line = parts.join(",");
            userFound = true;
        }
        lines.append(line);
    }
    file.close();
    if (!userFound) return false;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&file);
    for (const QString& l : lines) out << l << "\n";
    file.close();
    return true;
}

QString AuthHandler::getUserProfile(QString username) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() >= 6 && parts[0] == username) {
            return parts[3] + ":" + parts[4] + ":" + parts[5];
        }
    }
    return "";
}

bool AuthHandler::editProfile(QString username, QString newPassword, QString name, QString phone, QString email) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QStringList lines;
    bool userFound = false;
    QTextStream in(&file);
    if (!in.atEnd()) lines.append(in.readLine());
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 6 && parts[0] == username) {
            if (newPassword != "KEEP_OLD") parts[1] = newPassword;
            parts[3] = name;
            parts[4] = phone;
            parts[5] = email;
            line = parts.join(",");
            userFound = true;
        }
        lines.append(line);
    }
    file.close();
    if (!userFound) return false;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&file);
    for (const QString& l : lines) out << l << "\n";
    file.close();
    return true;
}

int AuthHandler::getTotalScore(QString username) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() >= 3 && parts[0] == username) return parts[2].toInt();
    }
    return 0;
}

void AuthHandler::updateScore(QString username, int scoreChange) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QStringList lines;
    QTextStream in(&file);
    if (!in.atEnd()) lines.append(in.readLine());
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 3 && parts[0] == username) {
            int currentScore = parts[2].toInt();
            parts[2] = QString::number(currentScore + scoreChange);
            line = parts.join(",");
        }
        lines.append(line);
    }
    file.close();
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const QString& l : lines) out << l << "\n";
        file.close();
    }
}

void AuthHandler::recordMatch(QString username, QString gameType, QString opponent, QString date, QString role, QString result, int scoreChange) {
    QFile file(m_historyFile);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString scoreStr = (scoreChange > 0 ? "+" : "") + QString::number(scoreChange);
        out << username << "," << gameType << "," << opponent << "," << date << "," << role << "," << result << "," << scoreStr << "\n";
        file.close();
    }
}

QString AuthHandler::getMatchHistory(QString username, QString gameType) {
    QFile file(m_historyFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";
    QString history;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(",");
        if (parts.size() >= 7 && parts[0] == username && parts[1] == gameType) {
            history += parts[2] + "," + parts[3] + "," + parts[4] + "," + parts[5] + "," + parts[6] + "|";
        }
    }
    if (history.endsWith("|")) history.chop(1);
    return history;
}
