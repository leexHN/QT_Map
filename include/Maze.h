//
// Created by lix on 2020/1/6.
//

#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <exception>
#include <cstring>

typedef unsigned char uchar;
enum DIRECTION{L=0,U,R,D};
enum MAP_DIS{LOC=0,ACC,N_ACC};

struct S_MazeIMg{
    explicit S_MazeIMg(unsigned int row, unsigned int col){
        Reset(row,col);
    };
    ~S_MazeIMg(){
        delete map_pr;
    }
    inline unsigned int W() const { return w_;}
    inline unsigned int H() const { return h_;}

    inline void Reset(unsigned int row,unsigned int col){
        map_row_ = row;
        map_col_ = col;
        edge_pix_ = (row + col) * 2;
        wal_pix_ = (row + col) * 4;
        space_pix_ = (row + col) * 6;
        Init(row, col, wal_pix_, edge_pix_ , space_pix_);
    }

    void SetPix(uchar wall, uchar edge, uchar space){
        Init(row_,col_,wall,edge,space,false);
    }

    inline uchar &operator() (size_t row, size_t col){
        if(row >= h_ || col >= w_)
            throw std::range_error("[S_MazeIMg] : image index out of range!");
        return map_pr[w_ * row + col];
    }

    uchar* Bits(){ return map_pr;}

    /**
     * @brief remove wall according the location of !!!S_MAP!!!(is not the loc of image)
     */
    void RemoveWall(size_t row, size_t col, DIRECTION dir);

    /**
     * @brief set space color according the location of !!!S_MAP!!!(is not the loc of image)
     */
    void ChangeSpaceDepth(unsigned int row, unsigned col, uchar depth);
private:
    uchar edge_pix_,wal_pix_, space_pix_;
    unsigned int map_row_, map_col_;
    unsigned int row_,col_;
    unsigned int h_,w_;
    uchar * map_pr = nullptr;

    inline uchar & I(size_t row, size_t col){
        return (*(this))(row, col);
    }

    /**
     * @brief Full an area using same @param value (c1,r1) top-left corner (r2,c2) bottom-right corner
     */
    void FullArea(size_t r1, size_t c1, size_t r2, size_t c2, uchar value = 255);

    void FullAreaRef(size_t r1, size_t c1, unsigned int h, unsigned int w, uchar value = 255);

    void Init(unsigned int row,unsigned int col, uchar wall, uchar edge, uchar space, bool is_auto_adjust = true);

    S_MazeIMg& operator = (const S_MazeIMg&); // prohibit copy or assigment
    S_MazeIMg(const S_MazeIMg&);
};


// The array maze_ is going to hold the array information for each cell.
// The first four coordinates tell if walls exist on those sides(1 means dont have wall)
// and the fifth indicates if the cell has been visited in the search.
// map[row, col, (LEFT, UP, RIGHT, DOWN, CHECK_IF_VISITED)]
struct S_Maze{
    typedef std::string str_t;
    typedef std::vector<std::vector<uchar>> AdjacencyMapType;
    explicit S_Maze(int row, int col)
    : cross("+"), v_w("|"), h_w("--"), space("  "),
      maze_(row, std::vector<std::vector<uchar>>(col, std::vector<uchar>(5, 0))),
      map_2d_(row + row + 1,std::vector<str_t>(col + col + 1, space)),
      maze_img_(row, col)
    {
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
    ~S_Maze() = default;

    std::vector<std::vector<uchar>>& operator[] (int index){
        return maze_[index];
    }

    std::vector<uchar>::reference operator()(int row, int col, int check){
        return maze_[row][col][check];
    }

    S_MazeIMg &MapImg(){
//        Convert2D();
        return maze_img_;}

    void Convert2D(); // turn maze_ to img

    std::vector<std::vector<uchar>> GetAdjacencyMap();

    std::pair<size_t ,size_t> ID2R_C(size_t id);

    std::vector<std::vector<std::vector<uchar>>> &Map(){ return maze_;}
    const std::vector<std::vector<std::string>> &MapStr(){return map_2d_;};
private:
    const std::string cross, v_w, h_w, space; //v_w vertical wall
    std::vector<std::vector<std::vector<uchar>>> maze_;
    std::vector<std::vector<str_t>> map_2d_;
    S_MazeIMg maze_img_;

    std::string Convert2DStr() const;

//    std::vector<char>::reference operator()(int row, int col){
//        return map_2d_[row][col];
//    }
    template<typename T>
    static inline bool IsEven(T num){
        return num%2==0;
    }

    inline size_t R_C2ID(size_t row, size_t col){
        if(row < 0 || col < 0 )
            throw std::invalid_argument("index out of range!!!");
        return maze_[0].size()*row+col;
    }

    void ChangeAdjacencyMapConnect(size_t row, size_t col, DIRECTION dir, AdjacencyMapType &map);

    friend std::ostream& operator << (std::ostream &os, const S_Maze& rhs);
};

std::ostream& operator << (std::ostream &os, const S_Maze& rhs);


