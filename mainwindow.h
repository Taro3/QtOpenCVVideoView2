#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QMutex;
class ViewThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void frameUpdated(cv::Mat* mat);

private:
    Ui::MainWindow *ui;
    QMutex* _lock;
    ViewThread* _vThread;
};
#endif // MAINWINDOW_H
