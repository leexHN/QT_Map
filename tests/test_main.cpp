//
// Created by lix on 2020/1/9.
//
#include <gtest/gtest.h>
#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#include <zconf.h>
#include "Map.h"


MainWindow * pr_w;

TEST(MapGenTest,test1){
    std::thread MapGenTest;
    auto MapGenTestFunc = [](){
        MainWindow & w = *pr_w;
        MapGen generator(150,200);
        do{
            // draw image
            w.ConvertToImg(generator.Map().MapImg().Bits(),generator.Map().MapImg().H(),generator.Map().MapImg().W());
            std::cout << "Time : " << generator.StepCount() << std::endl;
            usleep(0);
            generator.Step();
        }while (!generator.IsFinish());
        w.ConvertToImg(generator.Map().MapImg().Bits(),generator.Map().MapImg().H(),generator.Map().MapImg().W());
    };
    MapGenTest = std::thread(MapGenTestFunc);
    MapGenTest.detach();
    SUCCEED();
}

TEST(S_MAP_IMG_TEST, remove_all_wall){
    MainWindow & w = *pr_w;
    unsigned int row = 10,col = 10;
    S_MapIMg img(row,col);
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
    S_MapIMg img(row,col);
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