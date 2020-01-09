//
// Created by lix on 2020/1/6.
//

#include <set>
#if !defined(_WIN32)
#include <sys/time.h>
#endif
#include "Map.h"
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
    srand(time(nullptr));
#endif
};

/*********************************************************************
 ** Class S_MapImg
 *********************************************************************/

void S_MapIMg::Init(unsigned int row, unsigned int col, uchar wall, uchar edge, uchar space,bool is_auto_adjust) {
    if(!map_pr)
        delete map_pr;
    if(is_auto_adjust){
        edge = edge >= 8 ? 8 :edge;
        wall = wall >= 12 ? 12 :wall;
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
    auto DrawSpace = [this](unsigned int row, unsigned col){
        FullAreaRef(row,col,space_pix_,space_pix_);
    };
    for (size_t r = edge_pix_;  r <= h_- edge_pix_ - space_pix_ ; r+= space_pix_+wal_pix_)
        for(size_t c = edge_pix_; c <= w_- edge_pix_ - space_pix_; c+= space_pix_+wal_pix_)
            DrawSpace(r,c);

}

void S_MapIMg::FullArea(unsigned int r1, unsigned int c1, unsigned int r2, unsigned int c2, uchar value) {
    for(size_t r= r1; r <= r2; r++)
        for(size_t c = c1; c <= c2; c++)
            I(r, c) = value;
}

void S_MapIMg::FullAreaRef(unsigned int r1, unsigned int c1, unsigned int h, unsigned int w, uchar value) {
    FullArea(r1,c1,r1+h -1,c1+w -1,value);
}

void S_MapIMg::RemoveWall(unsigned int row, unsigned col, DIRECTION dir) {
    unsigned int s_row,s_col; // space left top corner location of img
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

void S_MapIMg::ChangeSpaceDepth(unsigned int row, unsigned col, uchar depth) {
    row = edge_pix_ + row * (space_pix_+wal_pix_);
    col = edge_pix_ + col * (space_pix_+wal_pix_);
    FullAreaRef(row,col,space_pix_,space_pix_,depth);
}


/*********************************************************************
 ** Class S_Map
 *********************************************************************/

std::ostream& operator << (std::ostream &os, const S_Map& rhs){
//    rhs.Convert2D();
    os<< "The Map Is : \n";
    os << rhs.Convert2DStr();
    return os;
}

void S_Map::Convert2D() {
    for (size_t i = 0; i < map_.size(); ++i) {
        for (size_t j = 0; j < map_[0].size(); ++j) {
            auto map_cell = map_[i][j];
            if(map_cell[DIRECTION::L] == 1)
                map_img_.RemoveWall(i,j,DIRECTION::L);
            if(map_cell[DIRECTION::U] == 1)
                map_img_.RemoveWall(i,j,DIRECTION::U);
            if(map_cell[DIRECTION::R] == 1)
                map_img_.RemoveWall(i,j,DIRECTION::R);
            if(map_cell[DIRECTION::D] == 1)
                map_img_.RemoveWall(i,j,DIRECTION::D);
        }
    }
}

std::string S_Map::Convert2DStr() const {
    std::stringstream ss;
    for(const auto& sub_map: map_2d_){
        for(const auto& ss_map : sub_map)
            ss<<ss_map;
        ss<<std::endl;
    }
    return ss.str();
}

MapGen::MapGen(int row, int col):
        step_count_(0){
    _Reset(row,col);
}

void MapGen::Reset(int row, int col) {
    _Reset(row,col);
}

void MapGen::Step() {
    S_Map& map = *map_pr_.get();
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
        history_.push({r_,c_});
        int range = check.size(); // [0~ range)
        seed_rand();
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

void MapGen::_Reset(int row, int col) {
    step_count_ = 0;
    num_rows = row;
    num_cols = col;
    r_ = 0;
    c_ = 0;
    map_pr_.reset(new S_Map(row, col));
    history_.push({r_,c_});
}


