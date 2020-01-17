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
    int MapH(){ return image_frame_->height();}
    int MapW(){ return image_frame_->width();}
    void SetTextBrowser(const QString &context);
    void ResetIsRun(){is_run_ = false;}

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
    int set_row_ = 10, set_col_=10;
    bool is_run_ = false;


protected:
    void timerEvent( QTimerEvent *event ) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void SetRow(int row){set_row_ = row;};
    void SetCol(int col){set_col_ = col;};
    void ShowStack(int state);
    void SetDelayTime(int time);
    void SetMazeGenerator(int type);
    void RunOrPause();
    void Reset();

signals:
    void StopTimerSig(int timer_id);
    void delayed_update();
    void textBrowserSig(const QString &text);
    void clearBrowserSig();

};
#endif // MAINWINDOW_H
