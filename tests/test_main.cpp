//
// Created by lix on 2020/1/9.
//
#include <gtest/gtest.h>
#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#if !defined(_WIN32)
#include <zconf.h>
#else
#include<Windows.h>
#endif
#include "Maze.h"
#include "maze_factory.h"


MainWindow * pr_w;

TEST(MapGenTest,DFS){
    std::thread MapGenTest;
    auto MapGenTestFunc = [](){
        MainWindow & w = *pr_w;
        DFS_MazeGenFactory dfs_maze_fac_;
        AbstractMazeGen* generator = dfs_maze_fac_.CreateMazeGen(50,70);
        do{
            // draw image
            w.ConvertToImg(generator->Maze().MapImg().Bits(),generator->Maze().MapImg().H(),generator->Maze().MapImg().W());
            std::cout << "Time : " << generator->StepCount() << std::endl;
            generator->Step();
        }while (!generator->IsFinish());
        w.ConvertToImg(generator->Maze().MapImg().Bits(),generator->Maze().MapImg().H(),generator->Maze().MapImg().W());
    };
    MapGenTest = std::thread(MapGenTestFunc);
    MapGenTest.detach();
    SUCCEED();
}

TEST(MapGenTest,dfs_show_stack){
    std::thread MapGenTest;
    auto MapGenTestFunc = [](){
        MainWindow & w = *pr_w;
        DFS_MazeGenFactory dfs_maze_fac_;
        AbstractMazeGen* generator = dfs_maze_fac_.CreateMazeGen(25,30);
        do{
            // draw image
            w.ConvertToImg(generator->MazeImgBits(SHOW_STACK),generator->Maze().MapImg().H(),generator->Maze().MapImg().W());
            generator->Step();
        }while (!generator->IsFinish());
        w.ConvertToImg(generator->Maze().MapImg().Bits(),generator->Maze().MapImg().H(),generator->Maze().MapImg().W());
    };
    MapGenTest = std::thread(MapGenTestFunc);
    MapGenTest.detach();
    SUCCEED();
}

TEST(S_MAP_IMG_TEST, remove_all_wall){
    MainWindow & w = *pr_w;
    unsigned int row = 10,col = 10;
    S_MazeIMg img(row,col);
    for(size_t i=0; i< row; i++){
        for (size_t j = 0; j < col; ++j) {
            for(int k=0;k<4;k++)
                img.RemoveWall(i,j,(DIRECTION)k);
        }
    }
    w.ConvertToImg(img.Bits(),img.H(),img.W());
    SUCCEED();
}

TEST(S_MAP_IMG_TEST, set_all_space_grey){
    MainWindow & w = *pr_w;
    unsigned int row = 10,col = 10;
    S_MazeIMg img(row,col);
    for(size_t i=0; i< row; i++)
        for (size_t j = 0; j < col; ++j)
            img.ChangeSpaceDepth(i,j,128);
    w.ConvertToImg(img.Bits(),img.H(),img.W());
    SUCCEED();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc,argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    pr_w = &w;
    RUN_ALL_TESTS();
    return a.exec();
}
