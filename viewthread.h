#ifndef VIEWTHREAD_H
#define VIEWTHREAD_H

#include <QThread>
#include <QObject>

#include <opencv2/opencv.hpp>

class QMutex;

class ViewThread : public QThread
{
    Q_OBJECT
public:
    ViewThread(int cameraIndex, QMutex *lock);
    ~ViewThread();
    void stop();

protected:
    void run() override;

signals:
    void frameUpdated(cv::Mat *mat);

private:
    bool _isStop;
    int _cameraId;
    QMutex *_lock;
    cv::Mat _frame;
};

#endif // VIEWTHREAD_H
