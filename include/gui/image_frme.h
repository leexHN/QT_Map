//
// Created by lix on 2020/1/6.
//

#pragma once

#include <QFrame>
#include <QImage>
#include <QLayout>
#include <QLayoutItem>
#include <QMutex>
#include <QPainter>
#include <QRect>
#include <QSize>

class ImageFrame
        : public QFrame
{
Q_OBJECT

public:
    explicit ImageFrame(QWidget* parent, Qt::WindowFlags flags = 0);
    ~ImageFrame() override = default;

    const QImage& getImage() const;

    QImage getImageCopy() const;

    void setImage(const QImage& image);

    void setImage(const std::vector<uchar> &data,uint row,uint col);

signals:

    void delayed_update();

protected:

    void paintEvent(QPaintEvent* event) override;

private:
    QImage qimage_;
    QImage *qImage_pr_;
    mutable QMutex qimage_mutex_;
};
