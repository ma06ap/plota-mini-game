#include "AuthHandler.h"

AuthHandler::AuthHandler(QString filename) : m_filename(filename) {
    qDebug() << "Database Path:" << QDir::current().absoluteFilePath(m_filename);

    QFile file(m_filename);
    if (!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "username,password,score,name,phone,email\n";
            file.close();
        }
    }
}

bool AuthHandler::userExists(QString username) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    if (!in.atEnd()) in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() > 0 && parts[0] == username) {
            return true;
        }
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
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2) {
            if (parts[0] == username && parts[1] == password) {
                return true;
            }
        }
    }
    return false;
}

bool AuthHandler::resetPassword(QString username, QString phone, QString newPassword) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QStringList lines;
    bool userFound = false;

    QTextStream in(&file);
    if (!in.atEnd()) {
        lines.append(in.readLine());
    }

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
    for (const QString& l : lines) {
        out << l << "\n";
    }
    file.close();

    return true;
}
