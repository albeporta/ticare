// bitcoinexchange.cpp

#include <bitcoinexchange.h>
#include <QObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


static const QString BitcoinExchangeUrl = "https://www.bitstamp.net/api/v2/ticker/btceur/";
static const int BitcoinExchangeRefreshTime = 10;


BitcoinExchange::BitcoinExchange(QObject* parent):
 QObject(parent), debug(false), netManager(this), timer(this)
{
    timer.setInterval(BitcoinExchangeRefreshTime * 1000);

    // set proxy for debug
    if (debug)
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(8888);
        netManager.setProxy(proxy);
    }
    connect(&netManager, &QNetworkAccessManager::finished, this, &BitcoinExchange::finishedSlot);
    connect(&netManager, &QNetworkAccessManager::sslErrors, this, &BitcoinExchange::networkSslErrors);

    connect(&timer, &QTimer::timeout, this, &BitcoinExchange::doCurrentExchange);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::startCurrentExchange()
{
    // launch now
    doCurrentExchange();

    // launch wvery N seconds
    timer.start();
}

void BitcoinExchange::stopCurrentExchange()
{
    timer.stop();
}

void BitcoinExchange::doCurrentExchange()
{
    QUrl request_url(BitcoinExchangeUrl);
    QNetworkRequest request(request_url);

    QNetworkReply* reply = netManager.get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
}

void BitcoinExchange::finishedSlot(QNetworkReply* reply)
{
   reply->deleteLater();

   if (reply->error())
   {
       reply = nullptr;
       return;
   }

   QString data = (QString) reply->readAll();

   QJsonDocument json_response = QJsonDocument::fromJson(data.toUtf8());
   QJsonObject json_object = json_response.object();
   QJsonValue value = json_object.value("last");

    BitcoinExchangeData bed_data;
    bed_data.currentExchange = value.toString();
    bed_data.dateExchange = QTime::currentTime();

    emit currentExchangeChanged(bed_data);
}

void BitcoinExchange::networkSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors(errors);
}

void BitcoinExchange::networkError(QNetworkReply::NetworkError)
{
    BitcoinExchangeData bed_data;
    bed_data.currentExchange = "";
    bed_data.dateExchange = QTime::currentTime();

    emit currentExchangeChanged(bed_data);
}
