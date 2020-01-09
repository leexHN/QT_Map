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
    while (flags_.run_status){
        if(flags_.reset_row_col){
            map_generator_.Reset(row_,col_);
            flags_.reset_row_col = false;
            continue;
        }

        if(flags_.start){
            map_generator_.Step();
            if(flags_.show_stack)
                win_.ConvertToImg(map_generator_.MapImgBits(true),
                                  (int)map_generator_.Map().MapImg().H(),(int)map_generator_.Map().MapImg().W());
            else
                win_.ConvertToImg(map_generator_.MapImgBits(),
                                  (int)map_generator_.Map().MapImg().H(),(int)map_generator_.Map().MapImg().W());
            usleep(delay_time_*1000);
        }
    }
}
