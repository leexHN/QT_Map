//
// Created by lix on 2020/1/6.
//

#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <bits/shared_ptr.h>
#include <stack>

typedef unsigned char uchar;


// The array map_ is going to hold the array information for each cell.
// The first four coordinates tell if walls exist on those sides(1 means dont have wall)
// and the fifth indicates if the cell has been visited in the search.
// map[row, col, (LEFT, UP, RIGHT, DOWN, CHECK_IF_VISITED)]
struct S_Map{
    explicit S_Map(int row, int col)
    :map_(row,std::vector<std::vector<bool>>(col,std::vector<bool>(5, 0))),
    map_2d_(row + row + 1,std::vector<char>(col + col + 1, ' ')){
        for(size_t i=0; i<= 2*row ;i++){
            for(size_t j=0;j<= 2*col; j++){
                if(!IsEven(i) && !IsEven(j))
                    continue;
                else if(IsEven(i) && IsEven(j))
                    map_2d_[i][j] = cross;
                else if(!IsEven(i))
                    map_2d_[i][j] = v_w;
                else if(!IsEven(j))
                    map_2d_[i][j] = h_w;
            }
        }
        map_2d_[1][0] = ' ';
        map_2d_[2*row - 1][2*col] = ' ';
    };
    ~S_Map() = default;

    std::vector<std::vector<bool>>& operator[] (int index){
        return map_[index];
    }

    std::vector<bool>::reference operator()(int row, int col, int check){
        return map_[row][col][check];
    }

    std::vector<std::vector<std::vector<bool>>> &Map(){ return map_;}
    const std::vector<std::vector<char>> &Map2D(){return map_2d_;};
private:
    std::vector<std::vector<std::vector<bool>>> map_;
    std::vector<std::vector<char>> map_2d_;
    const char cross = '+', v_w = '|', h_w = '-', space = ' '; //v_w vertical wall

    void Convert2D();

    std::string Convert2DStr() const;

//    std::vector<char>::reference operator()(int row, int col){
//        return map_2d_[row][col];
//    }

    static inline bool IsEven(int num){
        return num%2==0;
    }

    friend std::ostream& operator << (std::ostream &os, const S_Map& rhs);
};

class MapGen{
public:
    MapGen(int row, int col);

    void Reset(int row, int col);

    bool IsFinish(){ return history_.empty();};

    void Step();

    int StepCount(){ return step_count_;};

    void Loop(){while(!IsFinish()) Step();};

    S_Map& Map(){ return *map_pr_;}

private:
    enum DIRECTION{L=0,U,R,D};
    std::shared_ptr<S_Map> map_pr_;
    int step_count_;
    int r_,c_;
    int num_cols,num_rows;
    std::stack<std::pair<int,int>> history_;  // The history is the stack of visited locations (row,col)

    void _Reset(int row, int col);

    friend inline std::ostream &operator << (std::ostream &os, const MapGen& rhs);
};

std::ostream& operator << (std::ostream &os, const S_Map& rhs);
inline std::ostream& operator << (std::ostream &os, const MapGen& rhs){
    return os << (*(rhs.map_pr_)) ;
}


