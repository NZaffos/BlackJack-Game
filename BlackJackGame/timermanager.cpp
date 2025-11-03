/**
 * @brief Implementation of The TimerManager class. It handles calling and cancelling any single shot callback functions
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins, John Chen
 * @date 4/18/2025
 */

#include "timermanager.h"

TimerManager::TimerManager(QObject *parent) : QObject{parent} {}

TimerManager::~TimerManager()
{
    cancelAllTimers();
}

void TimerManager::scheduleSingleShot(int delay, std::function<void()> callBack)
{
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);

    // Creates the timer with the given callBack and has it delte itself afterwards
    connect(timer, &QTimer::timeout, this, [=]()
            {
        callBack();
        timers.removeOne(timer);
        timer->deleteLater(); });

    // Add and start the timer
    timers.append(timer);
    timer->start(delay);
}

void TimerManager::cancelAllTimers()
{
    // Delete all timers
    for (QTimer *timer : timers)
    {
        timer->stop();
        timer->deleteLater();
    }
    timers.clear();
}
