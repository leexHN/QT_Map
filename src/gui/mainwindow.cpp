#include "gui/mainwindow.h"
#include "gui/ui_mainwindow.h"
#include <algorithm>


const int MainWindow::GNumRowMap = 400;
const int MainWindow::GNumColMap = 400;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(StopTimerSig(int)),this,SLOT(StopTimerSlot(int)), Qt::DirectConnection);
    timer_id_ = startTimer(1);
    SetUpdateFrequency(update_frequency_);

    auto verticalLayoutWidget = new QWidget(this);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(10,10,
            geometry().width()-20,geometry().height()-15);

    auto verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    image_frame_ = new ImageFrame(this);
    image_frame_->setGeometry(0,0,GNumRowMap + 10 ,GNumColMap + 10);
    image_frame_->setMinimumSize(QSize(GNumRowMap + 10, GNumRowMap + 10));

    verticalLayout->addWidget(image_frame_,0,Qt::AlignHCenter|Qt::AlignTop);

    map_lock_.lock();
    map_.map_data_.reserve(GNumRowMap*GNumColMap);
    map_.map_data_.resize(1*1,125);
    map_.row = 1;
    map_.col = 1;
    image_frame_ -> setImage(map_.map_data_,map_.row,map_.col);
    map_lock_.unlock();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::ConvertToImg(const uchar *const data, int row, int col) {
    ConvertToImg(std::vector<uchar>(data, data + row*col),row,col);
}

void MainWindow::ConvertToImg(const std::vector<uchar> &data,  int row, int col) {
    map_lock_.lock();
    map_.map_data_ = data;
    map_.row = row;
    map_.col = col;
    if(!update_frequency_)
        image_frame_ -> setImage(map_.map_data_,map_.row,map_.col);
    map_lock_.unlock();
}

void MainWindow::SetUpdateFrequency(uint hz) {
    update_frequency_ = hz;
    if(update_frequency_ == 0){
        timer_count_ = -1;
        return;
    }
    timer_count_ = static_cast<int>(1000.f/update_frequency_);
}

void MainWindow::StopTimerSlot(int timer_id) {
    killTimer(timer_id);
}

void MainWindow::TimerUpdate() {
    image_frame_ -> setImage(map_.map_data_,map_.row,map_.col);
}

void MainWindow::timerEvent(QTimerEvent *event) {
    static int count = 0;
    if(event->timerId() == timer_id_){
        if(timer_count_ > 0)
            if(count >= timer_count_){
                map_lock_.lock();
                count = 0;
                image_frame_ -> setImage(map_.map_data_,map_.row,map_.col);
                map_lock_.unlock();
            }
        count++;
    }
}