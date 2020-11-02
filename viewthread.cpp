#include <QThread>
#include <QMutexLocker>

#include "viewthread.h"

ViewThread::ViewThread(int cameraIndex, QMutex *lock) : QThread(), _isStop(true), _cameraId(cameraIndex)
  , _lock(lock), _frame()
{
}

ViewThread::~ViewThread()
{
}

void ViewThread::stop()
{
    _isStop = true;
}

void ViewThread::run()
{
    _isStop = false;
    cv::VideoCapture vc(_cameraId);
    cv::Mat tempFrame;

    while (!_isStop) {
        vc >> tempFrame;
        if (tempFrame.empty())
            break;
        cv::cvtColor(tempFrame, tempFrame, cv::COLOR_BGR2RGB);
        QMutexLocker locker(_lock);
        _frame = tempFrame;
        locker.unlock();
        emit frameUpdated(&_frame);
    }
    vc.release();
    _isStop = true;
}
