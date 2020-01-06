#include <iostream>
#include <QApplication>
#include <gui/mainwindow.h>
#include <thread>


//std::thread bb;

int main(int argc, char *argv[]) {
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