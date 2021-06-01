#ifndef TIMEOUTTIMER_H
#define TIMEOUTTIMER_H

#include <QTimer>

class TimeoutTimer : public QObject {
    Q_OBJECT
public:
    TimeoutTimer(int const&& msec, QObject* parent = nullptr);
    ~TimeoutTimer();

    void startTimer();
    void stopTimer();
    void emitTimeout();

signals:
    void timeoutSignal(void);

private:
    QTimer m_timer;
    int m_msec;
};

#endif // TIMEOUTTIMER_H
