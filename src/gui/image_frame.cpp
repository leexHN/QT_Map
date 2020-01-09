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
    qimage_ = QImage(&data[0],col,row,col,QImage::Format_Indexed8);
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
        QImage painter_image;
        if (contentsRect().width() == qimage_.width()) {
            if(qimage_.format() ==  QImage::Format_Indexed8)
                painter_image = qimage_.convertToFormat(QImage::Format_RGB32);
            else
                painter_image = qimage_;
        } else {
            painter_image = qimage_.scaled(contentsRect().width(), contentsRect().height(),
                                          Qt::KeepAspectRatio, Qt::SmoothTransformation);
            if(painter_image.format() ==  QImage::Format_Indexed8)
                painter_image = painter_image.convertToFormat(QImage::Format_RGB32);
        }
        painter.drawImage(
                QRect(contentsRect().width() / 2 - painter_image.width() / 2,0,painter_image.width(),painter_image.height()),painter_image);
    }
    qimage_mutex_.unlock();
}

