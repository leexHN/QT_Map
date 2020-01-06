//
// Created by lix on 2020/1/6.
//

#include "gui/image_frme.h"

ImageFrame::ImageFrame(QWidget *parent, Qt::WindowFlags flags)
        : QFrame(parent),
          qImage_pr_(nullptr){
    connect(this, SIGNAL(delayed_update()), this, SLOT(update()), Qt::QueuedConnection);
}

const QImage &ImageFrame::getImage() const {
    return qimage_;
}

QImage ImageFrame::getImageCopy() const {
    QImage img;
    qimage_mutex_.lock();
    img = qimage_.copy();
    qimage_mutex_.unlock();
    return img;
}

void ImageFrame::setImage(const QImage &image) {
    qimage_mutex_.lock();
    qimage_ = image.copy();
    qimage_mutex_.unlock();
    emit delayed_update();
}

void ImageFrame::setImage(const std::vector<uchar> &data, uint row,uint col) {
    qimage_mutex_.lock();
    qimage_ = QImage(&data[0],row,col,row,QImage::Format_Indexed8);
    if(qimage_.bits()[0] != qimage_.bits()[col*row-1]){
//        printf("!!!!!\n");
    }
    qimage_mutex_.unlock();
    emit delayed_update();
}

void ImageFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    qimage_mutex_.lock();
    if (!qimage_.isNull())
    {
        if (contentsRect().width() == qimage_.width()) {
            painter.drawImage(contentsRect(), qimage_);
        } else {
            QImage image = qimage_.scaled(contentsRect().width(), contentsRect().height(),
                                          Qt::KeepAspectRatio, Qt::SmoothTransformation);
            painter.drawImage(contentsRect(), image);
        }
    }
    qimage_mutex_.unlock();
}

