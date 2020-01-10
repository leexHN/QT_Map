/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *main_gridLayout;
    QTextBrowser *textBrowser;
    QGridLayout *set_grid_layout;
    QPushButton *reset_pushButton;
    QCheckBox *show_stack_checkBox;
    QPushButton *r_p_Button;
    QSpinBox *delay_time_spinBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *row_spinBox;
    QSpacerItem *horizontalSpacer;
    QSpinBox *col_spinBox;
    QGridLayout *image_layout;
    QFrame *frame;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(700, 700);
        MainWindow->setMinimumSize(QSize(600, 600));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(600, 600));
        centralwidget->setMaximumSize(QSize(16777215, 16777215));
        main_gridLayout = new QGridLayout(centralwidget);
        main_gridLayout->setObjectName(QString::fromUtf8("main_gridLayout"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMaximumSize(QSize(16777215, 150));

        main_gridLayout->addWidget(textBrowser, 2, 0, 1, 1);

        set_grid_layout = new QGridLayout();
        set_grid_layout->setObjectName(QString::fromUtf8("set_grid_layout"));
        reset_pushButton = new QPushButton(centralwidget);
        reset_pushButton->setObjectName(QString::fromUtf8("reset_pushButton"));

        set_grid_layout->addWidget(reset_pushButton, 0, 9, 1, 1);

        show_stack_checkBox = new QCheckBox(centralwidget);
        show_stack_checkBox->setObjectName(QString::fromUtf8("show_stack_checkBox"));

        set_grid_layout->addWidget(show_stack_checkBox, 0, 12, 1, 1);

        r_p_Button = new QPushButton(centralwidget);
        r_p_Button->setObjectName(QString::fromUtf8("r_p_Button"));

        set_grid_layout->addWidget(r_p_Button, 0, 8, 1, 1);

        delay_time_spinBox = new QSpinBox(centralwidget);
        delay_time_spinBox->setObjectName(QString::fromUtf8("delay_time_spinBox"));
        delay_time_spinBox->setMaximum(999);

        set_grid_layout->addWidget(delay_time_spinBox, 0, 11, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        set_grid_layout->addWidget(label, 0, 2, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        set_grid_layout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        set_grid_layout->addWidget(label_3, 0, 10, 1, 1);

        row_spinBox = new QSpinBox(centralwidget);
        row_spinBox->setObjectName(QString::fromUtf8("row_spinBox"));
        row_spinBox->setMinimum(2);
        row_spinBox->setMaximum(500);
        row_spinBox->setValue(10);

        set_grid_layout->addWidget(row_spinBox, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        set_grid_layout->addItem(horizontalSpacer, 0, 7, 1, 1);

        col_spinBox = new QSpinBox(centralwidget);
        col_spinBox->setObjectName(QString::fromUtf8("col_spinBox"));
        col_spinBox->setMinimum(2);
        col_spinBox->setMaximum(500);
        col_spinBox->setValue(10);

        set_grid_layout->addWidget(col_spinBox, 0, 3, 1, 1);


        main_gridLayout->addLayout(set_grid_layout, 1, 0, 1, 1);

        image_layout = new QGridLayout();
        image_layout->setObjectName(QString::fromUtf8("image_layout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        image_layout->addWidget(frame, 0, 0, 1, 1);


        main_gridLayout->addLayout(image_layout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        textBrowser->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Runtime Status:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">--------------------------</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        reset_pushButton->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        show_stack_checkBox->setText(QApplication::translate("MainWindow", "show stack", nullptr));
        r_p_Button->setText(QApplication::translate("MainWindow", "Run/Pause", nullptr));
        label->setText(QApplication::translate("MainWindow", "    col: ", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "    row:  ", nullptr));
        label_3->setText(QApplication::translate("MainWindow", " delay time: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
