#ifndef TGASSTATION_H
#define TGASSTATION_H

#include "state.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TGasStation; }
QT_END_NAMESPACE

class TGasStation : public QMainWindow
{
    Q_OBJECT

public:
    TGasStation(QWidget *parent = nullptr);
    ~TGasStation();

private slots:
    void on_run_clicked();

    void on_stop_clicked();

private:
    //! Sleeps for `msec' milliseconds without blocking the thread.
    void Delay(int msec) const;

    Ui::TGasStation* ui;

    constexpr static int GasStationWidth = 25;
    constexpr static int GasStationHeight = 25;
    constexpr static int CarWidth = 20;
    constexpr static int CarHeight = 40;
    constexpr static int Offset = 5;

    TState State_;
    bool Running_ = false;
};
#endif // TGASSTATION_H
