#include "gas_station.h"
#include "options.h"
#include "request_generator.h"
#include "./ui_gas_station.h"

#include <iostream>
#include <QTimer>
#include <QThread>
#include <QPainter>

TGasStation::TGasStation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TGasStation)
{
    TOptions options;
    ui->setupUi(this);
    ui->petrols->setRowCount(5);
    ui->petrols->setColumnCount(2);
    ui->petrols->setHorizontalHeaderLabels({"price", "markup"});
    ui->petrols->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int row = 0; row < ui->petrols->horizontalHeader()->count(); ++row) {
        ui->petrols->horizontalHeader()->setSectionResizeMode(row, QHeaderView::Stretch);
    }
}

TGasStation::~TGasStation()
{
    delete ui;
}


void TGasStation::on_run_clicked()
{
    if (Running_) {
        return;
    }
    Running_ = true;

    TOptions options;
    options.QueueCount = ui->queue_count->text().toInt();
    options.MaxQueueSize = ui->max_queue_size->text().toInt();
    options.SimulationPeriod = ui->simulation_period->text().toInt();
    for (int petrolIndex = 0; petrolIndex < ui->petrols->rowCount(); ++petrolIndex) {
        auto priceItem = ui->petrols->item(petrolIndex, 0);
        auto markupItem = ui->petrols->item(petrolIndex, 1);
        if (!priceItem || !markupItem) {
            break;
        }
        auto price = priceItem->text();
        auto markup = markupItem->text();
        if (price.size() == 0 || markup.size() == 0) {
            break;
        }
        TPetrol petrol;
        petrol.Price = price.toDouble();
        petrol.Markup = markup.toDouble();
        options.Petrols.push_back(petrol);
    }

    TRequestGenerator requestGenerator(options);
    auto requests = requestGenerator.GenerateRequests();
    State_ = TState(requestGenerator.GenerateRequests(), options);

    int time = 0;
    while (time < Week) {
        if (!Running_) {
            break;
        }
        State_.Simulate(time);

        ui->Statistics->setText(QString::fromUtf8(State_.GetStatistics().Print(time).c_str()));
        auto queueSizes = State_.GetQueueSizes();
        QPixmap pixmap(ui->image->width(), ui->image->height());
        QPainter painter(&pixmap);
        painter.fillRect(pixmap.rect(), Qt::white);
        for (size_t queueIndex = 0; queueIndex < queueSizes.size(); ++queueIndex) {
            int x = (GasStationWidth + Offset) * queueIndex;
            painter.drawPixmap(x, 0, GasStationWidth, GasStationHeight, QPixmap(":shop.png"));
            for (int carIndex = 0; carIndex < queueSizes[queueIndex]; ++carIndex) {
                int y = GasStationHeight + Offset + (CarHeight + Offset) * carIndex;
                painter.drawPixmap(x, y, CarWidth, CarHeight, QPixmap(":car.png"));
            }
        }
        painter.end();
        ui->image->setPixmap(pixmap);

        if (time != Week - 1 && time + options.SimulationPeriod > Week) {
            time = Week - 1;
        } else {
            time += options.SimulationPeriod;
        }
        repaint();
        Delay(500);
    }

    Running_ = false;
}

void TGasStation::Delay(int msec) const
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, &QEventLoop::quit);
    loop.exec();
}

void TGasStation::on_stop_clicked()
{
    Running_ = false;
}
