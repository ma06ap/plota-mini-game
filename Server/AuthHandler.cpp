#include "AuthHandler.h"

AuthHandler::AuthHandler(QString filename) : m_filename(filename) {
    qDebug() << "Database Path:" << QDir::current().absoluteFilePath(m_filename);

    QFile file(m_filename);
    if (!file.exists()) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "username,password,score\n";
            file.close();
        }
    }
}

bool AuthHandler::userExists(QString username) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() > 0 && parts[0] == username) {
            return true;
        }
    }
    return false;
}

bool AuthHandler::signup(QString username, QString password) {
    if (userExists(username)) return false;

    QFile file(m_filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << username << "," << password << ",0\n";
    file.close();
    return true;
}

bool AuthHandler::login(QString username, QString password) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
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
