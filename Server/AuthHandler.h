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
    bool signup(QString username, QString password, QString name, QString phone, QString email);
    bool resetPassword(QString username, QString phone, QString newPassword);

    QString getUserProfile(QString username);
    bool editProfile(QString username, QString newPassword, QString name, QString phone, QString email);
    int getTotalScore(QString username);
    void updateScore(QString username, int scoreChange);
    void recordMatch(QString username, QString gameType, QString opponent, QString date, QString role, QString result, int scoreChange);
    QString getMatchHistory(QString username, QString gameType);

private:
    QString m_filename;
    QString m_historyFile;
    bool userExists(QString username);
};

#endif // AUTHHANDLER_H
