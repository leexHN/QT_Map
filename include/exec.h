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
#include "Maze.h"
#include "signals.h"
#include "maze_factory.h"
#include <SteadyTimer.hpp>



class Exec{
public:
    explicit Exec(int &arc,char** argv):app_(arc,argv),
        row_(10),col_(10){
        // connect message
        G_SetRowColSig.connect([this](int row, int col){ResetMap(row,col);});
        G_IsShowStackSig.connect([this](bool flag){ShowStack(flag);});
        G_RunSig.connect([this](bool flag){Start(flag);});
        G_SetDelayTimeSig.connect([this](int ms){SetDelayTime(ms);});
        G_SetMazeGeneratorSig.connect([this](int type){ChangeGenerator(type);});

        // maze_factory
        maze_fac_pr_ = std::make_shared<DFS_MazeGenFactory>();
        maze_gen_ = maze_fac_pr_->CreateMazeGen(row_,col_);

        // draw basic map
        win_.ConvertToImg(maze_gen_->Maze().MapImg().Bits(),
                          (int) maze_gen_->Maze().MapImg().H(), (int) maze_gen_->Maze().MapImg().W());
        //run map process thread
        flags_.run_status = true;
        maze_process_thread_ = std::thread([this](){ MazeProcessThread();});
    };

    ~Exec(){
        S_Flags end_flag{};
        flags_ = end_flag;
        if(maze_process_thread_.joinable())
            maze_process_thread_.join();
    }

    inline int Run(){
        win_.show();
        return app_.exec();
    }

    void ResetMap(int row, int col);

    inline void ShowStack(bool flag){flags_.show_stack=flag;}

    inline void Start(bool flag){flags_.start = flag;}

    inline void SetDelayTime(int ms){delay_time_ = ms;}

    inline void ChangeGenerator(int type){ flags_.cur_maze_gen = (MAZE_GENERATOR)type;};

private:
    struct S_Flags{
        bool start = false;// true start false pause
        bool reset_row_col = false;
        bool show_stack = false;
        bool run_status = false;
        MAZE_GENERATOR cur_maze_gen = DFS;
    };
    QApplication app_;
    MainWindow win_;
    int row_, col_;
    int delay_time_ = 0; // ms
    std::thread maze_process_thread_;
    S_Flags flags_;
    AbstractMazeGen* maze_gen_ = nullptr;
    std::shared_ptr<AbstractMazeFactory> maze_fac_pr_;
    SteadyTimer timer_;

    void MazeProcessThread();
    
    void ChangeMazeFac(MAZE_GENERATOR type);
};