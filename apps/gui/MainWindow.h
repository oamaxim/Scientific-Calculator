#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>

#include "calculator/Calculator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onEvaluate();

private:
    QLineEdit *inputBox;
    QTextEdit *outputBox;
    QPushButton *evalButton;
    QListWidget *historyList;

    Calculator calc;
};