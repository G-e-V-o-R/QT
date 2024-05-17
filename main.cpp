#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>


class numberButton : public QWidget{
public:
    numberButton(QString &&label, QGridLayout* glayout, int row, int column)
    {
        QPushButton* button = new QPushButton(label);
        button->setFixedSize(80, 80);
        button->setStyleSheet("QPushButton {"
                              "font-size: 38px; padding: 10px;"
                              "background-color: #404040;"
                              "color: white;"
                              "border-radius: 40px"
                              "}"
                              "QPushButton:pressed {"
                              "background-color: #C4C1C1;"                                      "}"
                               );
        glayout->addWidget(button, row, column);
    }

    numberButton(QString &&label, QHBoxLayout* hlayout, int width)
    {
        QPushButton* button = new QPushButton(label);
        button->setFixedSize(width, 80);
        button->setStyleSheet("QPushButton {"
                             "font-size: 38px;"
                             "padding: 10px;"
                             "background-color: #404040;"
                             "color: white;"
                             "border-radius: 40px;"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #C4C1C1;"  // Change color when pressed
                             "}");
        hlayout->addWidget(button);
    }
};




class operatorButton : public QWidget{
public:
    operatorButton(QString &&label, QGridLayout* glayout,
                 int row, int column,
                   QString color)
    {
        QPushButton* button = new QPushButton(label);
        QString styleSheet = QString("QPushButton {"
                                     "font-size: 38px; padding: 10px;"
                                     "background-color: %1; color: %2;"
                                     "border-radius: 40px"
                                     "}"
                                     "QPushButton:pressed {"
                                     "background-color: %3;"
                                     "color: %4"  // Change color when pressed
                                     "}")
                                     .arg(color, color == "#FF9933" ? "white" : "black")
                                     .arg(color == "#FF9933" ? "white" : "#ECE9E9",
                                      color == "#FF9933" ? "#FF9933" : "black");
        button->setFixedSize(80, 80);
        button->setStyleSheet(styleSheet);
        glayout->addWidget(button, row, column);
    }

    operatorButton(QString &&label, QHBoxLayout* hlayout)
    {
        QPushButton* button = new QPushButton(label);
        button->setFixedSize(80, 80);
        button->setStyleSheet("QPushButton {"
                              "font-size: 38px; padding: 10px;"
                              "background-color: #FF9933;"
                              "color: white;"
                              "border-radius: 40px"
                              "}"
                              "QPushButton:pressed {"
                              "background-color: #FFCC99;"
                              "}");
        hlayout->addWidget(button);
    }
};




class Calculator : public QWidget{
public:
    Calculator(){
        QGridLayout* glayout = new QGridLayout();
        QVBoxLayout* vlayout = new QVBoxLayout();
        QHBoxLayout* hlayout = new QHBoxLayout();
        glayout->setSpacing(10);

        display = new QLineEdit();
        display->setFixedSize(410, 250);
        display->setReadOnly(true);
        display->setAlignment(Qt::AlignRight);
        display->setStyleSheet("QLineEdit { border: 2px solid black; }");
        vlayout->addWidget(display);

        int number = 1;
        for (int i = 3; i >0; --i) { // Loop for rows
                for (int j = 0; j < 3; ++j) { // Loop for columns
                    numberButton(QString::number(number), glayout, i, j);
                    ++number;
                }
            }

        operatorButton("+", glayout, 3, 3, "#FF9933");
        operatorButton("-", glayout, 2, 3, "#FF9933");
        operatorButton("*", glayout, 1, 3, "#FF9933");
        operatorButton("÷", glayout, 0, 3, "#FF9933");
        operatorButton("AC", glayout, 0, 0, "#A0A0A0");
        operatorButton("%", glayout, 0, 2, "#A0A0A0");
        numberButton("0", hlayout, 180);
        numberButton(",", hlayout, 80);
        operatorButton("=", hlayout);
        operatorButton("+/-", glayout, 0, 1, "#A0A0A0");

        setLayout(vlayout);
        vlayout->addLayout(glayout);
        vlayout->addLayout(hlayout);

    }
 private:
    QLineEdit* display;

};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget w;
    w.setFixedSize(410, 750);
    w.setStyleSheet("background-color: #000000");

    QWidget* calculator = new Calculator();
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addWidget(calculator);
    w.setLayout(mainlayout);
    w.show();

    return a.exec();
}

