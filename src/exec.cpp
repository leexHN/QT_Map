//
// Created by lix on 2020/1/9.
//

#include "exec.h"

void Exec::ResetMap(int row, int col) {
    flags_.reset_row_col = true;
    row_ = row;
    col_ = col;
}

void Exec::MazeProcessThread() {
    static bool print_flag = true;
    while (flags_.run_status){
        if(flags_.cur_maze_gen != maze_gen_->CurMazeGenRateType()){
            S_Flags new_flags{};
            new_flags.cur_maze_gen = flags_.cur_maze_gen;
            new_flags.run_status = true;
            flags_ = new_flags;
            ChangeMazeFac(flags_.cur_maze_gen);
            print_flag = true;
            win_.ConvertToImg(maze_gen_->MazeImgBits(),
                              (int) maze_gen_->Maze().MapImg().H(), (int) maze_gen_->Maze().MapImg().W());
            win_.SetTextBrowser(QString("Now Change The Maze Generator to ") +
                                MAZE_GENERATOR_NAME[flags_.cur_maze_gen] + QString("; Auto reset the maze and status. "));
            win_.ResetIsRun();
            continue;
        }

        if(flags_.reset_row_col){
            maze_gen_->Reset(row_,col_);
            flags_.reset_row_col = false;
            flags_.start = false;
            print_flag = true;
            win_.ConvertToImg(maze_gen_->MazeImgBits(),
                              (int) maze_gen_->Maze().MapImg().H(), (int) maze_gen_->Maze().MapImg().W());
            continue;
        }

        if(flags_.start){
            if(!maze_gen_->IsFinish()){
                print_flag = true;
                maze_gen_->Step();
                if(flags_.show_stack)
                    win_.ConvertToImg(maze_gen_->MazeImgBits(MazeImgFlag::SHOW_STACK),
                                      (int) maze_gen_->Maze().MapImg().H(), (int) maze_gen_->Maze().MapImg().W());
                else
                    win_.ConvertToImg(maze_gen_->MazeImgBits(),
                                      (int) maze_gen_->Maze().MapImg().H(), (int) maze_gen_->Maze().MapImg().W());
#if !defined(_WIN32)
                usleep(delay_time_*1000);
#else
                Sleep(delay_time_);
#endif

            }
            else
                if(print_flag){
                    print_flag = false;
                    flags_.start = false;

                    win_.SetTextBrowser("Mapping Finished!!!");
                    win_.SetTextBrowser("Mapping cost steps : "+ QString::number(maze_gen_->StepCount()));
                    win_.SetTextBrowser("This Loop Cost Time: " + QString::number(timer_.ElapseMs(),'f',2) + " ms");

                    maze_gen_->Reset(row_,col_);
                    win_.ResetIsRun();
                }
        }
        else
            timer_.Reset();
    }
}

void Exec::ChangeMazeFac(MAZE_GENERATOR type) {
    switch (type){
        case DFS:
            maze_fac_pr_.reset(new DFS_MazeGenFactory());
            maze_gen_ = maze_fac_pr_->CreateMazeGen(row_,col_);
            break;
        case RANDOM_PRIM:
            maze_fac_pr_.reset(new RandomPrimGenFactory());
            maze_gen_ = maze_fac_pr_->CreateMazeGen(row_,col_);
            break;
        default:
            assert(false);
    }
}
