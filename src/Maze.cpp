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
#include <future>

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
    std::vector<std::future<void>> fu_vec;
    for(size_t i = 0; i < maze_.size(); ++i){
        auto func_ = [i,this](){
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
        };
        fu_vec.push_back(std::async(std::launch::async, func_));
    }

    for(const auto & fu:fu_vec){
        fu.wait();
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

std::vector<std::vector<uchar>> S_Maze::GetAdjacencyMap() {
    size_t num_rows = maze_.size(), num_cols = maze_[0].size();
    // map : distance == 2(N_ACC) means is block; distance == means access; distance == 0 same dot
    AdjacencyMapType map(num_rows* num_cols, std::vector<uchar>(num_rows* num_cols,N_ACC));
    // according row to get id:(0,0) id 0; (0,1) id 1; (1,0) id col; (m,n) id col*m+n
    std::vector<std::future<void>> fu_vec;
    for(size_t i = 0; i < num_rows; i++){
        auto func = [i,&map,this](){
            for(size_t j = 0; j < maze_[0].size(); j++){
                auto map_cell = maze_[i][j];
                if(map_cell[DIRECTION::L] == 1)
                    ChangeAdjacencyMapConnect(i, j, DIRECTION::L, map);
                if(map_cell[DIRECTION::U] == 1)
                    ChangeAdjacencyMapConnect(i, j, DIRECTION::U, map);
                if(map_cell[DIRECTION::R] == 1)
                    ChangeAdjacencyMapConnect(i, j, DIRECTION::R, map);
                if(map_cell[DIRECTION::D] == 1)
                    ChangeAdjacencyMapConnect(i, j, DIRECTION::D, map);
            }
        };
        fu_vec.push_back(std::async(std::launch::async, func));
    }

    for(size_t i = 0; i < map.size(); i++)
        map[i][i] = LOC;

    for(const auto & fu:fu_vec){
        fu.wait();
    }

    return map;
}

std::pair<size_t, size_t> S_Maze::ID2R_C(size_t id) {
    size_t num_rows = maze_.size(), num_cols = maze_[0].size();
    size_t col = id % num_cols;
    size_t row = id / num_cols;
    return {row,col};
}

void S_Maze::ChangeAdjacencyMapConnect(size_t row, size_t col, DIRECTION dir, AdjacencyMapType &map) {
    size_t cur_id = R_C2ID(row,col);
    size_t next_id;
    switch (dir){
        case L:
            next_id = R_C2ID(row,col-1);
            break;
        case U:
            next_id = R_C2ID(row - 1, col);
            break;
        case R:
            next_id = R_C2ID(row,col+1);
            break;
        case D:
            next_id = R_C2ID(row + 1,col);
            break;
        default:
            assert(false);
    }
    map[cur_id][next_id] = ACC;
    map[next_id][cur_id] = ACC;
}


