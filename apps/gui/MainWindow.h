#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <vector>

#include "calculator/Calculator.h"

enum class ButtonType
{
    Input,
    Function,
    Operator,
    Control
};

struct ShiftButton
{
    QPushButton *button;
    QString normalText;
    QString shiftText;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onEvaluate();
    void showHistoryContextMenu(const QPoint &pos);

private:
    QPushButton *createButton(const QString &text, const QString &shiftText, int r, int c, ButtonType type, QGridLayout *layout);

    void handleButton(
        const QString &text,
        ButtonType type);

    void handleFunction(const QString &text);
    void handleOperator(const QString &text);
    void handleInput(const QString &text);
    void updateModeLabel();
    void updateAngleLabel();

private:
    QLineEdit *inputBox;
    QListWidget *historyList;

    QLabel *angleLabel;
    QLabel *shiftLabel;

    QGridLayout *softMenu;
    QGridLayout *grid;
    std::vector<ShiftButton> shiftButtons;

    QString lastAnswer = "0";
    QString lastExpression = "";

    Calculator calc;

    bool shiftMode = false;
};
