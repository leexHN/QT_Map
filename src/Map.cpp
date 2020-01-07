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
 ** Class S_Map
 *********************************************************************/

std::ostream& operator << (std::ostream &os, const S_Map& rhs){
//    rhs.Convert2D();
    os<< "The Map Is : \n";
    os << rhs.Convert2DStr();
    return os;
}

void S_Map::Convert2D() {
    std::string map_str;
    std::stringstream ss;



//    for (size_t i = 0; i < map_2d_.size(); i++) {
//        for (size_t j = 0; j < map_2d_[i].size(); j++) {
//            if(((i==0) + (i==map_2d_.size()-1) + (j==0) + (j==map_2d_[0].size()-1)) >= 2)
//                ss << cross;
//            if( (IsEven(i)) &&  (IsEven(j)) && map_2d_[i][j] == 0)
//                ss << cross;
//            else if()
//
//        }
//    }
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
    auto map = *map_pr_;
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