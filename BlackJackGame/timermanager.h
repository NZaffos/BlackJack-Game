#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include "qobject.h"
#include "qtimer.h"
#include <QList>

/**
 * @brief The TimerManager class hands calling and cancelling any single shot callback functions
 *
 * @authors Noah Zaffos, Caleb Standfield, Ethan Perkins, Jas Sandhu, Nash Hawkins
 * @date 4/18/2025
 */
class TimerManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief TimerManager Constructor that initializes the timer manager
     * @param parent
     */
    explicit TimerManager(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~TimerManager();

    /**
     * @brief scheduleSingleShot Schedules a singleShot timer that will execute the callBack function
     * @param delay The delay until running callBack
     * @param callBack The function to run
     */
    void scheduleSingleShot(int delay, std::function<void()> callBack);

    /**
     * @brief cancelAllTimers Cancels all pending timers
     */
    void cancelAllTimers();

private:
    /**
     * @brief timers A list to keep track of all timers
     */
    QList<QTimer *> timers;
};

#endif // TIMERMANAGER_H
