#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#include <zconf.h>
#include "Map.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    unsigned int row = 300,col = 300;
//    S_MapIMg img(row,col);
//    for(size_t i=0; i< row; i++){
//        for (size_t j = 0; j < col; ++j) {
//            for(int k=0;k<4;k++)
//                img.RemoveWall(i,j,(DIRECTION)k);
//        }
//    }
//    img.FullArea(0,0,img.H()-1,img.W()-20);
//    w.ConvertToImg(img.Bits(),img.H(),img.W());

//    std::thread bb;
//    auto func = [&w](){
//        int j(100000);
//        w.SetUpdateFrequency(2);
//        while(j) {
//            for (int i = 0; i < 256; ++i) {
//                std::vector<uchar> tttt(410 * 410, i);
//                w.ConvertToImg(tttt,410,410);
//            }
//            j--;
//        }
//    };
//
//    bb = std::thread(func);
//
//    bb.detach();

    std::thread MapGenTest;
    auto MapGenTestFunc = [&w](){
        MapGen generator(150,150);
        do{
            // draw image
//            w.ConvertToImg(generator.Map().MapImg().Bits(),generator.Map().MapImg().H(),generator.Map().MapImg().W());
            std::cout << "Time : " << generator.StepCount() << std::endl;
//            usleep(50000);
            generator.Step();
        }while (!generator.IsFinish());
        w.ConvertToImg(generator.Map().MapImg().Bits(),generator.Map().MapImg().H(),generator.Map().MapImg().W());
    };
    MapGenTest = std::thread(MapGenTestFunc);
    MapGenTest.detach();
    return a.exec();
}