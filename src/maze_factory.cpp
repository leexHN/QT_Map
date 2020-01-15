//
// Created by lix on 2020/1/15.
//

#include "maze_factory.h"
#include <set>
#if !defined(_WIN32)
#include <sys/time.h>
#else
#include <Windows.h>
#include <time.h>
#endif
#include "iostream"
#include <cassert>
#include <sstream>

/*********************************************************************
 ** Utility Function
 *********************************************************************/
inline void seed_rand()
{
    //Seed random number generator with current microseond count
#if !defined(_WIN32)
    timeval temp_time_struct;
    gettimeofday(&temp_time_struct,NULL);
    srand(temp_time_struct.tv_usec);
#else
    srand((unsigned)time(NULL));
#endif
}

/*********************************************************************
 ** Class DFS_MazeGen
 *********************************************************************/


void DFS_MazeGen::Reset(int row, int col) {
    seed_rand();
    step_count_ = 0;
    num_rows = row;
    num_cols = col;
    r_ = 0;
    c_ = 0;
    maze_pr_.reset(new S_Maze(row, col));
    history_ =  std::stack<std::pair<int,int>>();
    history_.push({r_,c_});
    pre_history_ = history_;
}

void DFS_MazeGen::Step() {
    S_Maze& map = *maze_pr_.get();
    map(r_, c_, 4) = 1; // is visited
    std::vector<int> check;

    if (c_ > 0 && map(r_,c_-1,4) == 0)
        check.push_back(DIRECTION::L);
    if (r_ > 0 && map(r_-1,c_,4) == 0) // the image origin is in the top-left side
        check.push_back(DIRECTION::U);
    if (c_ < num_cols-1 && map(r_,c_+1,4) == 0)
        check.push_back(DIRECTION::R);
    if (r_ < num_rows-1 && map(r_+1,c_,4) == 0)
        check.push_back(DIRECTION::D);

    if(check.empty()){
        std::pair<int,int> temp = history_.top();
        history_.pop();
        r_ = temp.first;
        c_ = temp.second;
    }else{
        if(r_!=0 || c_!= 0)
            history_.push({r_,c_});
        size_t range = check.size(); // [0~ range)
        //seed_rand();
        int index = rand()%range;
        auto move_direction = check[index];
        switch (move_direction){
            case DIRECTION::L: //Remove the wall between the current cell and the chosen cell(!!!the wall do not occupy any cell)
                map(r_,c_,DIRECTION::L) = 1; // remove current left wall
                c_--; // run left
                map(r_,c_,DIRECTION::R) = 1; // right which is origin cell remove wall
                break;
            case DIRECTION::U:
                map(r_,c_,DIRECTION::U) = 1;
                r_--;
                map(r_,c_,DIRECTION::D) = 1;
                break;
            case DIRECTION::R:
                map(r_,c_,DIRECTION::R) = 1;
                c_++;
                map(r_,c_,DIRECTION::L) = 1;
                break;
            case DIRECTION::D:
                map(r_,c_,DIRECTION::D) = 1;
                r_++;
                map(r_,c_,DIRECTION::U) = 1;
                break;
            default:
                assert(false);
        }
    }

    step_count_++;
}

const uchar *DFS_MazeGen::MazeImgBits(MazeImgFlag setting) {
    bool is_show_stack= setting & SHOW_STACK;

    if(is_show_stack){
        if(pre_history_.size() > history_.size()){ //pop
            auto temp = pre_history_.top();
            maze_pr_->MapImg().ChangeSpaceDepth(temp.first, temp.second, 255);
        }else if(pre_history_.size() < history_.size() && !pre_history_.empty()){ // push
            uchar base_color = 170, top_color = 50;
            const auto top = history_.top() , pre_top = pre_history_.top();
            maze_pr_->MapImg().ChangeSpaceDepth(top.first, top.second, top_color);
            maze_pr_->MapImg().ChangeSpaceDepth(pre_top.first, pre_top.second, base_color);
        }
        pre_history_ = history_;
    }

    return maze_pr_->MapImg().Bits();
}
