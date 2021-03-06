//
// Created by lix on 2020/1/15.
//

#pragma once

#include <stack>
#include <set>
#include "Maze.h"

typedef unsigned char uchar;

enum MazeImgFlag{
    MAZE_DEFAULT = 0x00,
    SHOW_STACK = 0x01
};

enum MAZE_GENERATOR{DFS = 0,RANDOM_PRIM};

extern const char* MAZE_GENERATOR_NAME[];;

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

    virtual const uchar* MazeImgBits(MazeImgFlag setting) {
        if(setting != MAZE_DEFAULT)
            throw std::logic_error("If Has Maze Setting, Must Override this Function");
        return maze_pr_->MapImg().Bits();
    };

    const uchar* MazeImgBits(){ return MazeImgBits(MAZE_DEFAULT);};

    virtual MAZE_GENERATOR CurMazeGenRateType() = 0;
protected:
    int num_cols,num_rows;
    int step_count_;
    std::shared_ptr<S_Maze> maze_pr_;

    void RemoveWall(int& r, int& c, DIRECTION dir);
    void RemoveWallOnlyMazeVec(int& r, int& c, DIRECTION dir);
};


class AbstractMazeFactory{
public:
    virtual ~AbstractMazeFactory() {
        DeleteMazeGen();
    };
    virtual AbstractMazeGen* CreateMazeGen(int row, int col) = 0;

private:
    virtual void DeleteMazeGen() {
        while (!maze_gen_pr_stack_.empty()) {
            delete maze_gen_pr_stack_.top();
            maze_gen_pr_stack_.pop();
        };
    }

protected:
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

    void Loop() override;

    const uchar* MazeImgBits(MazeImgFlag setting) override;

    MAZE_GENERATOR CurMazeGenRateType() override { return DFS;}

private:
    int r_,c_; // current process location
    std::stack<std::pair<int,int>> history_;  // The history is the stack of visited locations (row,col)
    std::stack<std::pair<int,int>> pre_history_;
    void _Step(bool is_show_animation = true);
};


class RandomPrimMazeGen: public AbstractMazeGen{
public:
    explicit RandomPrimMazeGen(int row, int col):AbstractMazeGen(row,col){
        Reset(row,col);
    }

    void Reset(int row, int col) override;

    bool IsFinish() override { return history_.empty();};

    void Step() override;

    void Loop() override;

    MAZE_GENERATOR CurMazeGenRateType() override { return RANDOM_PRIM;}
private:
    std::set<std::pair<int,int>> history_;
    int r_ = 0,c_ = 0; // current process location
    void _Step(bool is_show_animation = true);
};

/*********************************************************************
 ** Concrete Factories
 *********************************************************************/

class DFS_MazeGenFactory:public AbstractMazeFactory{
public:
    DFS_MazeGenFactory() = default;
    ~DFS_MazeGenFactory() override = default;

    AbstractMazeGen* CreateMazeGen(int row, int col)override {
        maze_gen_pr_stack_.push(new DFS_MazeGen(row,col));
        return maze_gen_pr_stack_.top();
    }

};

class RandomPrimGenFactory:public AbstractMazeFactory {
public:
    RandomPrimGenFactory() = default;
    ~RandomPrimGenFactory() override = default;

    AbstractMazeGen* CreateMazeGen(int row, int col)override {
        maze_gen_pr_stack_.push(new RandomPrimMazeGen(row,col));
        return maze_gen_pr_stack_.top();
    }

};