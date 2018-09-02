#ifndef BITCOINEXCHANGE_H
#define BITCOINEXCHANGE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSsl>

class BitcoinExchangeData : public QObject
{
    Q_OBJECT

public:
    QString currentExchange;
    QString statusMessage;
    QTime dateExchange;
};


class BitcoinExchange : public QObject
{
    Q_OBJECT

private:
    bool debug;
    QNetworkAccessManager netManager;
    QTimer timer;

    virtual void doCurrentExchange();

public:
    BitcoinExchange(QObject* parent = 0);
    ~BitcoinExchange();

    virtual void startCurrentExchange();
    virtual void stopCurrentExchange();

public slots:
    void finishedSlot(QNetworkReply* reply);
    void networkSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void networkError(QNetworkReply::NetworkError error);

signals:
    void currentExchangeChanged(const BitcoinExchangeData& data);
};

#endif // BITCOINEXCHANGE_H
