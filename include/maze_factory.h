//
// Created by lix on 2020/1/15.
//

#pragma once

#include <stack>
#include "Maze.h"

typedef unsigned char uchar;

enum MazeImgFlag{
    MAZE_DEFAULT = 0x00,
    SHOW_STACK = 0x01
};

class AbstractMazeGen{
public:
    explicit AbstractMazeGen(int row, int col):num_rows(row),num_cols(col),step_count_(0){}

    virtual ~AbstractMazeGen() = default;

    virtual void Reset(int row, int col) = 0;

    virtual bool IsFinish() = 0;

    virtual void Step() = 0;

    int StepCount(){ return step_count_;};

    virtual void Loop() = 0;

    S_Maze& Maze(){ return *maze_pr_;}

    virtual const uchar* MazeImgBits(MazeImgFlag setting) = 0;

    const uchar* MazeImgBits(){ return MazeImgBits(MAZE_DEFAULT);};
protected:
    int num_cols,num_rows;
    int step_count_;
    std::shared_ptr<S_Maze> maze_pr_;
};


class AbstractMazeFactory{
public:
    virtual ~AbstractMazeFactory() = default;
    virtual AbstractMazeGen* CreateMazeGen(int row, int col) = 0;

protected:
    virtual void DeleteMazeGen() = 0;

    std::stack<AbstractMazeGen*> maze_gen_pr_stack_;
};

/*********************************************************************
 ** Concrete Products
 *********************************************************************/

class DFS_MazeGen:public AbstractMazeGen{
public:
    explicit DFS_MazeGen(int row, int col):AbstractMazeGen(row,col),r_(0),c_(0){
        Reset(row,col);
    }

    ~DFS_MazeGen() override = default;;

    void Reset(int row, int col) override;

    bool IsFinish() override { return history_.empty();};

    void Step() override;

    void Loop() override {while(!IsFinish()) Step();};

    const uchar* MazeImgBits(MazeImgFlag setting) override;

private:
    int r_,c_; // current process location
    std::stack<std::pair<int,int>> history_;  // The history is the stack of visited locations (row,col)
    std::stack<std::pair<int,int>> pre_history_;
};


/*********************************************************************
 ** Concrete Factories
 *********************************************************************/

class DFS_MazeGenFactory:public AbstractMazeFactory{
public:
    DFS_MazeGenFactory() = default;
    ~DFS_MazeGenFactory() override{
        DeleteMazeGen();
    };

    AbstractMazeGen* CreateMazeGen(int row, int col)override {
        maze_gen_pr_stack_.push(new DFS_MazeGen(row,col));
        return maze_gen_pr_stack_.top();
    }

private:
    void DeleteMazeGen() override {
        while (!maze_gen_pr_stack_.empty()){
            delete maze_gen_pr_stack_.top();
            maze_gen_pr_stack_.pop();
        }
    }

};