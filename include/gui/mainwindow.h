#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QtWidgets/QLabel>
#include <mutex>
#include "gui/image_frme.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ConvertToImg(const uchar* const data, int row = GNumRowMap, int col = GNumColMap);
    void ConvertToImg(const std::vector<uchar>& data, int row = GNumRowMap, int col = GNumColMap);
    void SetUpdateFrequency(uint hz);
    int MapH(){ return GNumRowMap;}
    int MapW(){ return GNumColMap;}

private:
    struct S_Map{
        std::vector<uchar> map_data_;
        int col,row;
    };
    Ui::MainWindow *ui;
    static const int GNumRowMap;
    static const int GNumColMap;
    mutable QMutex map_lock_;
    ImageFrame *image_frame_;
    uint update_frequency_ = 30; // if 0 real time update
    int timer_id_ = 0;
    int timer_count_ = 0;
    S_Map map_;

    void AdjustMainWindowsAccordingMap(uint width,uint height);

protected:
    void timerEvent( QTimerEvent *event ) override;
    void paintEvent(QPaintEvent* event) override;

private slots:

signals:
    void StopTimerSig(int timer_id);
    void delayed_update();

};
#endif // MAINWINDOW_H
