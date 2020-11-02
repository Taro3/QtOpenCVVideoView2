#include <QMutex>
#include <QMutexLocker>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewthread.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _lock(new QMutex)
  , _vThread(new ViewThread(0, _lock))
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));
    connect(_vThread, &ViewThread::frameUpdated, this, &MainWindow::frameUpdated);
}

MainWindow::~MainWindow()
{
    if (_vThread->isRunning()) {
        _vThread->stop();
        while (_vThread->isRunning()) qApp->processEvents();
    }
    delete ui;
    delete _lock;
    delete _vThread;
}

void MainWindow::on_pushButton_clicked()
{
    if (_vThread->isRunning()) {
        ui->pushButton->setText(tr("&Start"));
        _vThread->stop();
    } else {
        ui->pushButton->setText(tr("&Stop"));
        _vThread->start();
    }
}

void MainWindow::frameUpdated(cv::Mat *mat)
{
    QMutexLocker locker(_lock);
    cv::Mat data = *mat;
    locker.unlock();

    QImage frame(data.data, data.cols, data.rows, data.step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);
    QGraphicsView* view = ui->graphicsView;
    QGraphicsScene* scene = view->scene();
    scene->clear();
    view->resetTransform();
    scene->addPixmap(image);
    scene->update();
    view->setSceneRect(image.rect());
}
