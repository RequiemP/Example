#include "timeouttimer.h"

TimeoutTimer::TimeoutTimer(int const&& msec, QObject* parent) : QObject(parent), m_msec(msec) {
    connect(&m_timer, &QTimer::timeout, this, &TimeoutTimer::emitTimeout);
}

TimeoutTimer::~TimeoutTimer() {
}

void TimeoutTimer::startTimer() {
    m_timer.start(m_msec);
}

void TimeoutTimer::stopTimer() {
    m_timer.stop();
}

void TimeoutTimer::emitTimeout() {
    emit timeoutSignal();
}
