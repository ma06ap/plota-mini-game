#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QDebug>

class AuthHandler
{
public:
    AuthHandler(QString filename = "users.csv");

    bool login(QString username, QString password);

    bool signup(QString username, QString password);

private:
    QString m_filename;

    QString hashPassword(QString password);

    bool userExists(QString username);
};

#endif // AUTHHANDLER_H
