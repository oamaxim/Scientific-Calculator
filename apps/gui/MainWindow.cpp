#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QGridLayout>
#include <QFont>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QFontDatabase>

#include "utility/Printer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet(R"(
    QPushButton {
        background-color: #222;
        color: white;
    }

    QPushButton[role="function"] {
        background-color: #2a2a2a;
        color: cyan;
    }

    QPushButton[role="operator"] {
        background-color: #333;
        color: orange;
    }

    QPushButton[role="control"] {
        background-color: #444;
        color: yellow;
    })");

    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);

    auto *topBar = new QHBoxLayout();

    angleLabel = new QLabel("DEG");
    shiftLabel = new QLabel("");

    topBar->addWidget(angleLabel);
    topBar->addStretch();
    topBar->addWidget(shiftLabel);

    root->addLayout(topBar);

    // History
    historyList = new QListWidget();
    historyList->setMinimumHeight(70);
    historyList->setAutoScroll(true);
    historyList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(historyList, &QListWidget::customContextMenuRequested,
            this, &MainWindow::showHistoryContextMenu);

    // Input
    inputBox = new QLineEdit();
    inputBox->setAlignment(Qt::AlignRight);
    inputBox->setMinimumHeight(50);
    connect(inputBox, &QLineEdit::returnPressed,
            this, &MainWindow::onEvaluate);

    inputBox->setStyleSheet(R"(
    background-color: black;
    color: #00ff90;
    font-family: Consolas;
    font-size: 22px;
    padding: 10px;
    )");

    // Buttons

    softMenu = new QGridLayout();

    createButton("SHIFT", "", 0, 0, ButtonType::Control, softMenu);
    createButton("ANGLE", "VAR/CON", 0, 1, ButtonType::Control, softMenu);

    createButton("mat", "identity", 0, 3, ButtonType::Function, softMenu);
    createButton("det", "", 0, 4, ButtonType::Function, softMenu);
    createButton("inv", "transpose", 0, 5, ButtonType::Function, softMenu);

    createButton("sin", "asin", 1, 0, ButtonType::Function, softMenu);
    createButton("cos", "acos", 1, 1, ButtonType::Function, softMenu);
    createButton("tan", "atan", 1, 2, ButtonType::Function, softMenu);
    createButton("csc", "acsc", 1, 3, ButtonType::Function, softMenu);
    createButton("sec", "asec", 1, 4, ButtonType::Function, softMenu);
    createButton("cot", "acot", 1, 5, ButtonType::Function, softMenu);

    createButton("sinh", "asinh", 2, 0, ButtonType::Function, softMenu);
    createButton("cosh", "acosh", 2, 1, ButtonType::Function, softMenu);
    createButton("tanh", "atanh", 2, 2, ButtonType::Function, softMenu);
    createButton("^", "root", 2, 3, ButtonType::Function, softMenu);
    createButton("^2", "√", 2, 4, ButtonType::Function, softMenu);
    createButton("ans", "", 2, 5, ButtonType::Input, softMenu);

    createButton("log", "log10", 3, 0, ButtonType::Function, softMenu);
    createButton("ln", "e^", 3, 1, ButtonType::Function, softMenu);
    createButton("e", "", 3, 2, ButtonType::Input, softMenu);
    createButton("π", "", 3, 3, ButtonType::Input, softMenu);
    createButton("(", "x", 3, 4, ButtonType::Input, softMenu);
    createButton(")", "y", 3, 5, ButtonType::Input, softMenu);

    grid = new QGridLayout();
    grid->setSpacing(2);

    createButton("7", "", 0, 0, ButtonType::Input, grid);
    createButton("8", "", 0, 1, ButtonType::Input, grid);
    createButton("9", "", 0, 2, ButtonType::Input, grid);
    createButton("DEL", "", 0, 3, ButtonType::Control, grid);
    createButton("AC", "", 0, 4, ButtonType::Control, grid);

    createButton("4", "", 1, 0, ButtonType::Input, grid);
    createButton("5", "", 1, 1, ButtonType::Input, grid);
    createButton("6", "", 1, 2, ButtonType::Input, grid);
    createButton("×", "", 1, 3, ButtonType::Operator, grid);
    createButton("÷", "", 1, 4, ButtonType::Operator, grid);

    createButton("1", "", 2, 0, ButtonType::Input, grid);
    createButton("2", "", 2, 1, ButtonType::Input, grid);
    createButton("3", "", 2, 2, ButtonType::Input, grid);
    createButton("+", "", 2, 3, ButtonType::Operator, grid);
    createButton("−", "", 2, 4, ButtonType::Operator, grid);

    createButton("0", "", 3, 0, ButtonType::Input, grid);
    createButton(".", "", 3, 1, ButtonType::Input, grid);
    createButton("×10^", "abs", 3, 2, ButtonType::Function, grid);
    createButton("=", "", 3, 3, ButtonType::Operator, grid);
    createButton("EXE", "", 3, 4, ButtonType::Control, grid);

    QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    QFont f("Segoe UI Mono");
    inputBox->setFont(f);
    historyList->setFont(f);

    root->addWidget(historyList);
    root->addWidget(inputBox);
    root->addLayout(softMenu);
    root->addLayout(grid);

    setCentralWidget(central);
    setWindowTitle("Scientific Calculator");
    setFixedSize(420, 600);
    setWindowFlags(
        windowFlags() &
        ~Qt::WindowMaximizeButtonHint);
}

void MainWindow::onEvaluate()
{
    QString input = inputBox->text();

    if (input.isEmpty())
    {
        if (lastExpression.isEmpty())
            return;

        input = lastExpression;
    }

    try
    {
        Value result = calc.evaluate(input.toStdString());

        std::string resultStr =
            Printer::toString(result);

        lastExpression = input;
        lastAnswer = QString::fromStdString(resultStr);

        historyList->addItem(
            "[" + input + "] = " +
            QString::fromStdString(resultStr));

        inputBox->clear();
    }
    catch (const CalcError &e)
    {
        historyList->addItem(
            QString::fromStdString(
                Printer::formatCalcError(
                    input.toStdString(),
                    e)));
    }
    catch (const std::exception &e)
    {
        historyList->addItem(
            QString::fromStdString(
                Printer::formatRuntimeError(e)));
    }
}

QPushButton *MainWindow::createButton(const QString &text, const QString &shiftText, int r, int c, ButtonType type, QGridLayout *layout)
{
    QPushButton *btn = new QPushButton(text);
    btn->setMinimumSize(55, 45);
    btn->setProperty("role",
                     type == ButtonType::Function ? "function" : type == ButtonType::Operator ? "operator"
                                                             : type == ButtonType::Control    ? "control"
                                                                                              : "input");

    btn->style()->unpolish(btn);
    btn->style()->polish(btn);

    layout->addWidget(btn, r, c);

    connect(btn, &QPushButton::clicked, this, [=]()
            { handleButton(btn->text(), type); });

    if (!shiftText.isEmpty())
    {
        shiftButtons.push_back({btn,
                                text,
                                shiftText});
    }

    return btn;
}

void MainWindow::handleButton(const QString &text, ButtonType type)
{
    // Control
    if (type == ButtonType::Control)
    {
        if (text == "EXE")
        {
            onEvaluate();
            return;
        }

        if (text == "DEL")
        {
            inputBox->backspace();
            return;
        }

        if (text == "AC")
        {
            inputBox->clear();
            return;
        }

        if (text == "SHIFT")
        {
            shiftMode = !shiftMode;
            updateModeLabel();
            return;
        }

        if (text == "ANGLE")
        {

            if (calc.getAngleMode() == AngleMode::RAD)
            {
                calc.setAngleMode(AngleMode::DEG);
            }
            else
            {
                calc.setAngleMode(AngleMode::RAD);
            }

            updateAngleLabel();
            return;
        }
        if (text == "VAR/CON")
        {
            historyList->addItem(
            QString::fromStdString(calc.getSymbolTable()));
            return;
        }
    }

    // Functions
    if (type == ButtonType::Function)
    {
        handleFunction(text);
        return;
    }

    // Operators
    if (type == ButtonType::Operator)
    {
        handleOperator(text);
        return;
    }

    // Input
    if (type == ButtonType::Input)
    {
        handleInput(text);
        return;
    }
    inputBox->insert(text);
}

void MainWindow::handleFunction(const QString &text)
{
    if (text == "√")
    {
        inputBox->insert("sqrt(");
        return;
    }

    if (text == "^2")
    {
        inputBox->insert("^2");
        return;
    }
    inputBox->insert(text + "(");
}

void MainWindow::handleOperator(const QString &text)
{
    if (text == "÷")
    {
        inputBox->insert("/");
        return;
    }
    if (text == "×")
    {
        inputBox->insert("*");
        return;
    }
    if (text == "−")
    {
        inputBox->insert("-");
        return;
    }

    inputBox->insert(text);
}

void MainWindow::handleInput(const QString &text)
{
    inputBox->insert(text);
}

void MainWindow::updateModeLabel()
{
    shiftLabel->setText(shiftMode ? "SHIFT" : "");

    for (auto &entry : shiftButtons)
    {
        entry.button->setText(
            shiftMode
                ? entry.shiftText
                : entry.normalText);
    }
}

void MainWindow::updateAngleLabel()
{
    QString angle = "DEG";

    switch (calc.getAngleMode())
    {
    case AngleMode::RAD:
        angle = "RAD";
        break;

    case AngleMode::DEG:
        angle = "DEG";
        break;

    default:
        break;
    }

    angleLabel->setText(angle);
}

void MainWindow::showHistoryContextMenu(const QPoint &pos)
{
    QListWidgetItem *item =
        historyList->itemAt(pos);

    if (!item)
        return;

    QMenu menu(this);

    QAction *copyAction =
        menu.addAction("Copy");

    QAction *deleteAction =
        menu.addAction("Delete");

    QAction *selected =
        menu.exec(historyList->viewport()->mapToGlobal(pos));

    if (selected == copyAction)
    {
        QApplication::clipboard()->setText(item->text());
    }

    if (selected == deleteAction)
    {
        delete historyList->takeItem(
            historyList->row(item));
    }
}