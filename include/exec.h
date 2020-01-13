//
// Created by lix on 2020/1/9.
//

#pragma once
#include <gtest/gtest.h>
#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#if !defined(_WIN32)
#include <zconf.h>
#else
#include <Windows.h>
#endif
#include <mutex>
#include "Map.h"
#include "signals.h"



class Exec{
public:
    explicit Exec(int &arc,char** argv):app_(arc,argv),
        row_(10),col_(10),
        map_generator_(row_,col_){
        // connect message
        G_SetRowColSig.connect([this](int row, int col){ResetMap(row,col);});
        G_IsShowStackSig.connect([this](bool flag){ShowStack(flag);});
        G_RunSig.connect([this](bool flag){Start(flag);});
        G_SetDelayTimeSig.connect([this](int ms){SetDelayTime(ms);});

        // draw basic map
        win_.ConvertToImg(map_generator_.Map().MapImg().Bits(),
                map_generator_.Map().MapImg().H(),map_generator_.Map().MapImg().W());
        //run map process thread
        flags_.run_status = true;
        map_process_thread_ = std::thread([this](){MapProcessThread();});
    };

    ~Exec(){
        S_Flags end_flag{};
        flags_ = end_flag;
        if(map_process_thread_.joinable())
            map_process_thread_.join();
    }

    inline int Run(){
        win_.show();
        return app_.exec();
    }

    void ResetMap(int row, int col);

    inline void ShowStack(bool flag){flags_.show_stack=flag;}

    inline void Start(bool flag){flags_.start = flag;}

    inline void SetDelayTime(int ms){delay_time_ = ms;}

private:
    struct S_Flags{
        bool start = false;// true start false pause
        bool reset_row_col = false;
        bool show_stack = false;
        bool run_status = false;
    };
    QApplication app_;
    MainWindow win_;
    int row_, col_;
    int delay_time_ = 0; // ms
    MapGen map_generator_;
    std::thread map_process_thread_;
    S_Flags flags_;

    void MapProcessThread();
};