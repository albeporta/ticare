#include <QtWidgets>

#include "bitcoinwindow.h"

#define BitcoinExchangeInstance Singleton<BitcoinExchange>::instance()

BitcoinWindow::BitcoinWindow(QWidget *parent)
    : QDialog(parent)
    , statusLineEdit(new QLineEdit("stopped"))
    , startButton(new QPushButton(tr("Start")))
    , currentBitcoinExchangeLineEdit(new QLineEdit(""))
    , start(true)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("BITCOIN EXCHANGE"));

    currentBitcoinExchangeLineEdit->setReadOnly(true);
    statusLineEdit->setReadOnly(true);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Current exchange:"), currentBitcoinExchangeLineEdit);
    formLayout->addRow(tr("Status:"), statusLineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    startButton->setDefault(true);
    connect(startButton, &QAbstractButton::clicked, this, &BitcoinWindow::startBitcointExchange);
    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttonBox);

    connect(&bcExchange, &BitcoinExchange::currentExchangeChanged, this, &BitcoinWindow::updateCurrentExchange);
}

void BitcoinWindow::startBitcointExchange()
{
    if (start)
    {
        statusLineEdit->setText("started");
        startButton->setText("Stop");

        bcExchange.startCurrentExchange();
        start = false;
    }
    else
    {
        statusLineEdit->setText("stopped");
        startButton->setText("Start");

        bcExchange.stopCurrentExchange();
        start = true;
    }
}

void BitcoinWindow::updateCurrentExchange(const BitcoinExchangeData& data)
{
    if (!data.statusMessage.isEmpty())
    {
        statusLineEdit->setText(data.statusMessage);
        currentBitcoinExchangeLineEdit->setText("");
    }
    else
    {
        statusLineEdit->setText("started ... last update at " + data.dateExchange.toString());
        currentBitcoinExchangeLineEdit->setText(data.currentExchange);
    }
}
