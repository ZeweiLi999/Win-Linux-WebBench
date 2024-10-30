/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *labelURL;
    QPushButton *pushButtonRun;
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEditTime;
    QLineEdit *lineEditClient;
    QLabel *labelClient;
    QComboBox *comboBoxProtocol;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *comboBoxMethod;
    QProgressBar *progressBar;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        labelURL = new QLabel(centralwidget);
        labelURL->setObjectName("labelURL");
        labelURL->setGeometry(QRect(110, 40, 54, 16));
        labelURL->setAlignment(Qt::AlignCenter);
        pushButtonRun = new QPushButton(centralwidget);
        pushButtonRun->setObjectName("pushButtonRun");
        pushButtonRun->setGeometry(QRect(470, 40, 80, 24));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(157, 110, 61, 20));
        label->setAlignment(Qt::AlignCenter);
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(160, 40, 291, 23));
        lineEditTime = new QLineEdit(centralwidget);
        lineEditTime->setObjectName("lineEditTime");
        lineEditTime->setGeometry(QRect(217, 110, 91, 23));
        lineEditClient = new QLineEdit(centralwidget);
        lineEditClient->setObjectName("lineEditClient");
        lineEditClient->setGeometry(QRect(427, 110, 91, 23));
        labelClient = new QLabel(centralwidget);
        labelClient->setObjectName("labelClient");
        labelClient->setGeometry(QRect(330, 110, 91, 20));
        labelClient->setAlignment(Qt::AlignCenter);
        comboBoxProtocol = new QComboBox(centralwidget);
        comboBoxProtocol->addItem(QString());
        comboBoxProtocol->addItem(QString());
        comboBoxProtocol->addItem(QString());
        comboBoxProtocol->setObjectName("comboBoxProtocol");
        comboBoxProtocol->setGeometry(QRect(320, 180, 121, 24));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(260, 180, 54, 16));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(230, 220, 91, 16));
        label_3->setAlignment(Qt::AlignCenter);
        comboBoxMethod = new QComboBox(centralwidget);
        comboBoxMethod->addItem(QString());
        comboBoxMethod->addItem(QString());
        comboBoxMethod->addItem(QString());
        comboBoxMethod->addItem(QString());
        comboBoxMethod->setObjectName("comboBoxMethod");
        comboBoxMethod->setGeometry(QRect(320, 220, 121, 24));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(140, 300, 431, 23));
        progressBar->setValue(24);
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(140, 360, 431, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelURL->setText(QCoreApplication::translate("MainWindow", "URL", nullptr));
        pushButtonRun->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\265\213\350\257\225", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\265\213\350\257\225\346\227\266\351\227\264", nullptr));
        labelClient->setText(QCoreApplication::translate("MainWindow", "\346\250\241\346\213\237\345\256\242\346\210\267\347\253\257\346\225\260\351\207\217", nullptr));
        comboBoxProtocol->setItemText(0, QCoreApplication::translate("MainWindow", "HTTP/0.9", nullptr));
        comboBoxProtocol->setItemText(1, QCoreApplication::translate("MainWindow", "HTTP/1.0", nullptr));
        comboBoxProtocol->setItemText(2, QCoreApplication::translate("MainWindow", "HTTP/1.1", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\345\215\217\350\256\256", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\350\277\236\346\216\245\346\226\271\345\274\217", nullptr));
        comboBoxMethod->setItemText(0, QCoreApplication::translate("MainWindow", "GET", nullptr));
        comboBoxMethod->setItemText(1, QCoreApplication::translate("MainWindow", "HEAD", nullptr));
        comboBoxMethod->setItemText(2, QCoreApplication::translate("MainWindow", "OPTIONS", nullptr));
        comboBoxMethod->setItemText(3, QCoreApplication::translate("MainWindow", "TRACE", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
