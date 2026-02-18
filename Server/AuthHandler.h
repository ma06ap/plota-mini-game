#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QDir>

class AuthHandler {
public:
    AuthHandler(QString filename);
    bool login(QString username, QString password);
    bool signup(QString username, QString password);

private:
    QString m_filename;
    bool userExists(QString username);
};

#endif // AUTHHANDLER_H
