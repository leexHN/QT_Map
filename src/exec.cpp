//
// Created by lix on 2020/1/9.
//

#include "exec.h"

void Exec::ResetMap(int row, int col) {
    flags_.reset_row_col = true;
    row_ = row;
    col_ = col;
}

void Exec::MapProcessThread() {
    static bool print_flag = true;
    while (flags_.run_status){
        if(flags_.reset_row_col){
            map_generator_.Reset(row_,col_);
            flags_.reset_row_col = false;
            flags_.start = false;
            print_flag = true;
            continue;
        }

        if(flags_.start){
            if(!map_generator_.IsFinish()){
                map_generator_.Step();
                if(flags_.show_stack)
                    win_.ConvertToImg(map_generator_.MapImgBits(true),
                                      (int)map_generator_.Map().MapImg().H(),(int)map_generator_.Map().MapImg().W());
                else
                    win_.ConvertToImg(map_generator_.MapImgBits(),
                                      (int)map_generator_.Map().MapImg().H(),(int)map_generator_.Map().MapImg().W());
                usleep(delay_time_*1000);

            }
            else
                if(print_flag){
                    win_.SetTextBrowser("Mapping Finished!!!");
                    print_flag = false;
                    win_.SetTextBrowser("Mapping cost steps : "+ QString::number(map_generator_.StepCount()));
                    flags_.reset_row_col = true;
                    win_.ResetIsRun();
                }
        }
    }
}
