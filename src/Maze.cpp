//
// Created by lix on 2020/1/6.
//

#include <set>
#if !defined(_WIN32)
#include <sys/time.h>
#else
#include <Windows.h>
#include <time.h>
#endif
#include "Maze.h"
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
 ** Class S_MapImg
 *********************************************************************/

void S_MazeIMg::Init(unsigned int row, unsigned int col, uchar wall, uchar edge, uchar space, bool is_auto_adjust) {
    if(!map_pr)
        delete map_pr;
    if(is_auto_adjust){
        edge = edge >= 8 ? 8 :edge;
        wall = wall >= 10 ? 10 :wall;
        space = space >= 16 ? 16 :space;
    }
    edge_pix_ = edge;
    wal_pix_ = wall;
    space_pix_ = space;
    row_ = row;
    col_ = col;

    h_ = 2*edge_pix_ + wal_pix_ * (row_ - 1) + row_ * space_pix_;
    w_ = 2*edge_pix_ + wal_pix_ * (col_ - 1) + col_ * space_pix_;
    map_pr = new uchar[h_*w_];

    memset(map_pr, 0, h_*w_);

    // Draw inlet and outlet
    FullAreaRef(edge_pix_ ,0,space_pix_,edge_pix_);
    FullAreaRef(h_ - edge_pix_ -space_pix_,w_ -edge_pix_,space_pix_,edge_pix_);
    // Draw every space
    auto DrawSpace = [this](size_t row, size_t col){
        FullAreaRef(row,col,space_pix_,space_pix_);
    };
    for (size_t r = edge_pix_;  r <= h_- edge_pix_ - space_pix_ ; r+= space_pix_+wal_pix_)
        for(size_t c = edge_pix_; c <= w_- edge_pix_ - space_pix_; c+= space_pix_+wal_pix_)
            DrawSpace(r,c);

}

void S_MazeIMg::FullArea(size_t r1, size_t c1, size_t r2, size_t c2, uchar value) {
    for(size_t r= r1; r <= r2; r++)
        for(size_t c = c1; c <= c2; c++)
            I(r, c) = value;
}

void S_MazeIMg::FullAreaRef(size_t r1, size_t c1, unsigned int h, unsigned int w, uchar value) {
    FullArea(r1,c1,r1+h -1,c1+w -1,value);
}

void S_MazeIMg::RemoveWall(size_t row, size_t col, DIRECTION dir) {
    size_t s_row,s_col; // space left top corner location of img
    s_row = edge_pix_ + (space_pix_ + wal_pix_) * row;
    s_col = edge_pix_ + (space_pix_ + wal_pix_) * col;
    switch (dir){
        case DIRECTION::L:
            if(col == 0)
                return;
            FullAreaRef(s_row,s_col - wal_pix_,space_pix_,wal_pix_);
            break;
        case DIRECTION::U:
            if(row == 0)
                return;
            FullAreaRef(s_row - wal_pix_, s_col, wal_pix_, space_pix_);
            break;
        case DIRECTION::R:
            if(col >= map_col_ - 1)
                return;
            FullAreaRef(s_row, s_col + space_pix_, space_pix_, wal_pix_);
            break;
        case DIRECTION::D:
            if(row >= map_row_ - 1)
                return;
            FullAreaRef(s_row + space_pix_, s_col,wal_pix_,space_pix_);
            break;
        default:
            assert(false);
    }
}

void S_MazeIMg::ChangeSpaceDepth(unsigned int row, unsigned col, uchar depth) {
    row = edge_pix_ + row * (space_pix_+wal_pix_);
    col = edge_pix_ + col * (space_pix_+wal_pix_);
    if(I(row,col) == depth)
        return;
    FullAreaRef(row,col,space_pix_,space_pix_,depth);
}


/*********************************************************************
 ** Class S_Map
 *********************************************************************/

std::ostream& operator << (std::ostream &os, const S_Maze& rhs){
//    rhs.Convert2D();
    os<< "The Maze Is : \n";
    os << rhs.Convert2DStr();
    return os;
}

void S_Maze::Convert2D() {
    for (size_t i = 0; i < maze_.size(); ++i) {
        for (size_t j = 0; j < maze_[0].size(); ++j) {
            auto map_cell = maze_[i][j];
            if(map_cell[DIRECTION::L] == 1)
                maze_img_.RemoveWall(i, j, DIRECTION::L);
            if(map_cell[DIRECTION::U] == 1)
                maze_img_.RemoveWall(i, j, DIRECTION::U);
            if(map_cell[DIRECTION::R] == 1)
                maze_img_.RemoveWall(i, j, DIRECTION::R);
            if(map_cell[DIRECTION::D] == 1)
                maze_img_.RemoveWall(i, j, DIRECTION::D);
        }
    }
}

std::string S_Maze::Convert2DStr() const {
    std::stringstream ss;
    for(const auto& sub_map: map_2d_){
        for(const auto& ss_map : sub_map)
            ss<<ss_map;
        ss<<std::endl;
    }
    return ss.str();
}


