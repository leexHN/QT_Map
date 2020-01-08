#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>
#include "Map.h"


//std::thread bb;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    S_MapIMg img(3,3);
//    img.FullArea(0,0,img.H()-1,img.W()-20);
    w.ConvertToImg(img.Bits(),img.H(),img.W());

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
    return a.exec();
}