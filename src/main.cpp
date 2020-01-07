#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#include "Map.h"


//std::thread bb;

int main(int argc, char *argv[]) {
    MapGen aa(3,4);
    std::cout << aa;
    aa.Reset(4,5);
    std::cout << aa;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    auto func = [&w](){
//        int j(100000);
//        w.SetUpdateFrequency(2);
//        while(j) {
//            for (int i = 0; i < 256; ++i) {
//                std::vector<uchar> tttt(400 * 400, i);
//                w.ConvertToImg(tttt,400,400);
//            }
//            j--;
//        }
//    };
//
//    bb = std::thread(func);
//
//    bb.detach();
    return a.exec();
}