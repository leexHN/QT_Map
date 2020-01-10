#include "gui/mainwindow.h"
#include "gui/ui_mainwindow.h"
#include <algorithm>
#include <iostream>
#include "signals.h"


const int MainWindow::GNumRowMap = 400;
const int MainWindow::GNumColMap = 400;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(delayed_update()), this, SLOT(update()), Qt::QueuedConnection);
    connect(ui->row_spinBox,SIGNAL(valueChanged(int)), this, SLOT(SetRow(int)));
    connect(ui->col_spinBox,SIGNAL(valueChanged(int)), this, SLOT(SetCol(int)));
    connect(ui->delay_time_spinBox,SIGNAL(valueChanged(int)),this, SLOT(SetDelayTime(int)));
    connect(ui->show_stack_checkBox,SIGNAL(stateChanged(int)),this,SLOT(ShowStack(int)));

    connect(ui->r_p_Button,SIGNAL(pressed()), this, SLOT(RunOrPause()));
    connect(ui->reset_pushButton,SIGNAL(pressed()), this, SLOT(Reset()));


    timer_id_ = startTimer(1);
    SetUpdateFrequency(update_frequency_);

    image_frame_ = new ImageFrame(this);
    image_frame_->setMinimumSize(QSize(GNumRowMap, GNumRowMap));
    ui->image_layout->addWidget(image_frame_,0,0);

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

void MainWindow::paintEvent(QPaintEvent *event) {

}

void MainWindow::SetDelayTime(int time){
    G_SetDelayTimeSig(time);
}

void MainWindow::RunOrPause() {
    is_run_ = !is_run_;
    is_run_?G_RunSig(true):G_RunSig(false);
}

void MainWindow::Reset() {
    is_run_ = false;
    G_SetRowColSig(set_row_,set_col_);
}

void MainWindow::ShowStack(int state){
    G_IsShowStackSig(state == Qt::Checked);
};