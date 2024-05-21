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
#include <iostream>
#include <cmath>


class CalculatorButton : public QPushButton{
public:
    CalculatorButton(const QString label, QWidget* parent = nullptr, int width = 80)
        :QPushButton(label, parent)
    {
        setFixedSize(width, 80);
        setStyleSheet(baseStyleSheet());
    }

protected:
    QString baseStyleSheet(){
        return "QPushButton {"
               "font-size: 38px;"
               "padding: 10px;"
               "border-radius: 40px;"
               "}";
    }
};

class NumberButton: public CalculatorButton{
public:
    NumberButton(const QString label, QWidget* parent = nullptr, int width = 80)
        :CalculatorButton(label, parent, width)
    {
        setStyleSheet(baseStyleSheet() + "QPushButton {"
                                         "color: white;"
                                         "background-color: #404040;"
                                         "}"
                                         "QPushButton::pressed {"
                                         "background-color: #C4C1C1;"
                                         "}");
    }
};

class OperatorButton : public CalculatorButton{
public:
    OperatorButton(const QString& label, const QString& color, QWidget* parent = nullptr)
        :CalculatorButton(label, parent, 80)
    {
        QString textColor = (color == "#FF9933") ? "white" : "black";
        QString pressedColor = (color == "#FF9933") ? "#FFCC99" : "#D5CFCF";
        setStyleSheet(baseStyleSheet() + QString("QPushButton {"
                                         "color: %1;"
                                         "background-color: %2"
                                         "}"
                                         "QPushButton::pressed {"
                                         "color: %3;"
                                         "background-color: %4"
                                         "}").arg(textColor, color, textColor, pressedColor));
    }
};

class Calculator : public QWidget{

public:
    Calculator(QWidget* parent = nullptr)
        :QWidget (parent)
    {
        QVBoxLayout* vlayout = new QVBoxLayout();
        QGridLayout* glayout = new QGridLayout();
        glayout->setSpacing(10);
        vlayout->setSpacing(10);
        glayout->setContentsMargins(10, 10, 10, 10);


        display = new QLineEdit();
        display->setFixedSize(350, 70);
        display->setAlignment(Qt::AlignRight);
        display->setReadOnly(true);
        display->setStyleSheet("color: white; font-size: 68px; /*border: 2px solid black*/");
        display->setText("0");
        vlayout->addWidget(display);

        int number = 1;
        for (int i = 3; i >0; --i) {
                for (int j = 0; j < 3; ++j) {
                    NumberButton* btn = new NumberButton(QString::number(number), this);
                    glayout->addWidget(btn, i, j);
                    connect(btn, &QPushButton::clicked, this, &Calculator::handleNumber);
                    ++number;
                }
        }

        addOperator(glayout, "+", "#FF9933", 3, 4, this);
        addOperator(glayout, "-", "#FF9933", 2, 4, this);
        addOperator(glayout, "*", "#FF9933", 1, 4, this);
        addOperator(glayout, "÷", "#FF9933", 0, 4, this);
        addOperator(glayout, "AC", "#B9B3B3", 0, 0, this);
        addOperator(glayout, "+/-", "#B9B3B3", 0, 1, this);
        addOperator(glayout, "%", "#B9B3B3", 0, 2, this);

        NumberButton* zero = new NumberButton("0", this, 160);
        glayout->addWidget(zero, 4, 0, 1, 2);
        connect(zero, &QPushButton::clicked, this, &Calculator::handleNumber);

        NumberButton* dot = new NumberButton(",", this, 80);
        glayout->addWidget(dot, 4, 2);
        connect(dot, &QPushButton::clicked, this, &Calculator::handleNumber);

        addOperator(glayout, "=", "#B9B3B3", 4, 4, this);



    vlayout->addLayout(glayout);
    setLayout(vlayout);
    }

   // vlayout->addLayout(glayout);
    //setLayout(vlayout);

private slots:
    void handleNumber(){
        QPushButton *button = qobject_cast<QPushButton *>(sender());
        QString text = display->text();
        QString lastSymbol = text.at(text.length() - 1);
        if (button){
            if (display->text() == "0" && button->text() != ".")
                display->setText(button->text());
            else if (button->text() == "," && lastSymbol == ".")
                return;
            else if (button->text() == ",")
                display->setText(text+".");
            else
                display->setText(display->text()+button->text());

        }
    }

    void handleOperator()
    {
        QPushButton* btn = qobject_cast<QPushButton*>(sender());
        if(btn){
            QString oper = btn->text();
            if (oper == "AC")
            {
                display->setText("0");
                return;
            }else if (oper == "+/-")
            {
                QString text = display->text();
                if (text[0] == "-"){
                     text = text.mid(1);
                     display->setText(text);
                }else{
                    text = "-"+text;
                    display->setText(text);
                }
                return;
            }else
                handleAction(oper);
        }
    }

    void handleAction(const QString &oper){


        QString text = display->text();
        std::cout << "HANDLE ACTION: " << std::endl;
        qDebug() << "text: " << text ;

        QString lastSymbol = text.at(text.length() - 1);
        QString updatedText;

        if (text.contains(QRegExp("[+\\-*÷%]")) && !QString("+\\-*÷%").contains(lastSymbol)){
            updatedText = QString::number(calculate());
            if (oper != "=")
                display->setText(updatedText+oper);
            else
                display->setText(updatedText);
        }else if (text.contains(QRegExp("[+\\-*÷%]")) && QString("+\\-*÷%").contains(lastSymbol) && oper == "="){
            updatedText = QString::number(calculate());
            display->setText(updatedText);
        }else if(!text.contains(QRegExp("[+-*÷]"))){
            display->setText(text + oper);
        }
    }


private:
    QLineEdit* display = nullptr;

    void addOperator(QGridLayout* glayout, const QString &&label, const QString &color, int &&row, int &&col,  QWidget* parent = nullptr){
        OperatorButton* btn = new OperatorButton(label, color, this);
        glayout->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, &Calculator::handleOperator);
    }


    double calculate(){

        QString text = display->text();
        qDebug() << text;
        QStringList operand;
        QString oper;
        double operand1;
        double operand2;

        if (text.contains("+")) {
            operand = text.split("+");
            oper = "+";
        }else if (text.contains("*")) {
            operand = text.split("*");
            oper = "*";
        }else if (text.contains("÷")) {
            operand = text.split("÷");
            oper = "÷";
        }else if (text.contains("%")) {
            operand = text.split("%");
            oper = "%";
        }else if (text.contains("-")) {
            int minusCount = text.count('-');
            operand = text.split("-");
            if(minusCount == 2){
                oper = "-";
                operand[0] = "-"+operand[1];
                qDebug() << "1: " << operand[0];
                operand[1] = operand[2];
                qDebug() << "2: " << operand[1];
                operand.removeLast();
            }else{
                operand = text.split("-");
                oper = "-";
            }
        }

        if (operand.length() == 2 && operand[1] == ""){
            operand1 = operand[0].toDouble();
            operand2 = operand1;
        }else if(operand.length() == 2){
            operand1 = operand[0].toDouble();
            operand2 = operand[1].toDouble();
        }



        if (oper == "+"){
            return operand1+operand2;
        }else if (oper == "-"){
            return operand1-operand2;
        }else if (oper == "*"){
            return operand1*operand2;
        }else if (oper == "÷"){
            std::cout << operand1 << " / " << operand2 << std::endl;
            return operand1/operand2;
        }else if (oper == "%"){
            return std::fmod(operand1, operand2);
        }


    }
};



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(410, 600);
    window.setStyleSheet("background-color: #000000");

    Calculator* calculator = new Calculator(&window);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addWidget(calculator);
    window.setLayout(mainlayout);
    window.show();

    return app.exec();
}

