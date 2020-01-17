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

template <typename T>
inline  typename T::value_type RandomChoice(const T & container){
    size_t range = container.size();
    if(range == 0)
        throw std::length_error("container size is 0");
    int index = rand()%range;
    auto c_iter = container.cbegin();
    std::advance(c_iter, index);
    return *c_iter;
}



/*********************************************************************
 ** Class AbstractMazeGen
 *********************************************************************/
void AbstractMazeGen::RemoveWall(int &row, int &col, DIRECTION dir) {
    S_Maze& maze = *maze_pr_.get();
    switch (dir){
        case DIRECTION::L: //Remove the wall between the current cell and the chosen cell(!!!the wall do not occupy any cell)
            maze(row, col, DIRECTION::L) = 1; // remove current left wall
            col--; // run left
            maze(row, col, DIRECTION::R) = 1; // right which is origin cell remove wall
            maze_pr_->MapImg().RemoveWall(row,col,DIRECTION::R);
            break;
        case DIRECTION::U:
            maze(row, col, DIRECTION::U) = 1;
            row--;
            maze(row, col, DIRECTION::D) = 1;
            maze_pr_->MapImg().RemoveWall(row,col,DIRECTION::D);
            break;
        case DIRECTION::R:
            maze(row, col, DIRECTION::R) = 1;
            col++;
            maze(row, col, DIRECTION::L) = 1;
            maze_pr_->MapImg().RemoveWall(row,col,DIRECTION::L);
            break;
        case DIRECTION::D:
            maze(row, col, DIRECTION::D) = 1;
            row++;
            maze(row, col, DIRECTION::U) = 1;
            maze_pr_->MapImg().RemoveWall(row,col,DIRECTION::U);
            break;
        default:
            assert(false);
    }
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
        auto move_direction = RandomChoice(check);
        RemoveWall(r_,c_,(DIRECTION)move_direction);
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


/*********************************************************************
 ** Class RandomPrimMazeGen
 *********************************************************************/

void RandomPrimMazeGen::Reset(int row, int col) {
    seed_rand();
    step_count_ = 0;
    num_rows = row;
    num_cols = col;
    r_ = row/2;
    c_ = col/2;
    maze_pr_.reset(new S_Maze(row, col));
    history_.clear();
    history_.insert({r_,c_});
}

void RandomPrimMazeGen::Step() {
    S_Maze& map = *maze_pr_.get();
    //random choose a candidate cell from the cell set histroy
    auto cell_loc = RandomChoice(history_);
    r_ = cell_loc.first; c_ = cell_loc.second;
    map(r_,c_,4) = 1; // this cell is visited
    history_.erase({r_,c_});


    // if this cell has connect the exist maze(the adjacency maze is visited),
    // random choose one to remove the wall ,else if this maze is neither visited nor belong to history
    // (history is a set that contain the maze not be visited), add this cell to history and mark as 2 means the cell
    // is in history(also can using set method "find" or "count",but this method complex is 1 ,and set method is log)
    std::vector<DIRECTION> check;
    if(c_ > 0) { // left check
        if (map(r_, c_ - 1, 4) == 1) // visited
            check.push_back(L);
        else if(map(r_, c_ - 1, 4) == 0){ // is not in the "history" set
            map(r_, c_ - 1, 4) = 2; //mark as in the "history" set
            history_.insert({r_,c_ - 1});
        }
    }

    if(r_ > 0){ // U
        if (map(r_ - 1, c_, 4) == 1)
            check.push_back(U);
        else if(map(r_ - 1, c_, 4) == 0){
            map(r_ - 1, c_, 4) = 2;
            history_.insert({r_-1,c_});
        }
    }

    if(c_ < num_cols -1) { // R
        if(map(r_,c_+1,4) == 1)
            check.push_back(R);
        else if(map(r_,c_+1,4) == 0){
            map(r_,c_+1,4) = 2;
            history_.insert({r_,c_+1});
        }
    }

    if(r_ < num_rows-1){ // D
        if(map(r_ + 1, c_,4) == 1)
            check.push_back(D);
        else if(map(r_ + 1, c_,4) == 0){
            map(r_ + 1, c_,4) = 2;
            history_.insert({r_+1, c_});
        }
    }

    if(!check.empty()) {
        auto move_direction = RandomChoice(check);
        RemoveWall(r_,c_, move_direction);
    }
    step_count_++;
}



