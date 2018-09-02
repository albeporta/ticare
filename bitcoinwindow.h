#ifndef BITCOINWINDOW_H
#define BITCOINWINDOW_H

#include "bitcoinexchange.h"
#include <QProgressDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QNetworkReply;


class BitcoinWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BitcoinWindow(QWidget *parent = nullptr);

private slots:
    void startBitcointExchange();

public slots:
    void updateCurrentExchange(const BitcoinExchangeData& data);

private:
    QLineEdit *statusLineEdit;
    QPushButton *startButton;
    QLineEdit *currentBitcoinExchangeLineEdit;
    BitcoinExchange bcExchange;
    bool start;
};

#endif
