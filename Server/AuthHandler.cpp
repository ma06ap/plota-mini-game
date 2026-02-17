#include "AuthHandler.h"

AuthHandler::AuthHandler(QString filename) : m_filename(filename)
{
    QFile file(m_filename);
    if (!file.exists()) {
        if(file.open(QIODevice::WriteOnly)) {
            file.close();
        }
    }
}

QString AuthHandler::hashPassword(QString password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool AuthHandler::userExists(QString username)
{
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2) {
            if (parts[0] == username) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

bool AuthHandler::signup(QString username, QString password)
{
    if (userExists(username)) {
        qDebug() << "Signup failed: User exists.";
        return false;
    }

    QFile file(m_filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << username << "," << hashPassword(password) << "\n";

    file.close();
    return true;
}

bool AuthHandler::login(QString username, QString password)
{
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString hashedInput = hashPassword(password);
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2) {
            if (parts[0] == username && parts[1] == hashedInput) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}
