#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QGridLayout>
#include <QFont>

#include "utility/Printer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setStyleSheet(R"(
    QWidget {
        background-color: #111;
    }

    QPushButton {
        background-color: #222;
        color: white;
        border: 1px solid #333;
        border-radius: 4px;
        font-size: 14px;
    }

    QPushButton:pressed {
        background-color: #444;
    }

    QLineEdit, QTextEdit {
        background-color: black;
        color: lime;
        font-family: Consolas;
        font-size: 16px;
        border: 1px solid #333;
    }

    QLabel {
        color: orange;
        font-weight: bold;
    }
)");

    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    // Mode

    auto *modeLayout = new QHBoxLayout();

    modeLabel = new QLabel("DEG");
    modeLabel->setAlignment(Qt::AlignCenter);

    modeLayout->addWidget(modeLabel);
    mainLayout->addLayout(modeLayout);

    // Display

    inputBox = new QLineEdit();
    outputBox = new QTextEdit();

    inputBox->setReadOnly(true);
    outputBox->setReadOnly(true);

    inputBox->setAlignment(Qt::AlignRight);
    outputBox->setAlignment(Qt::AlignRight);

    inputBox->setMinimumHeight(40);
    outputBox->setMinimumHeight(40);

    QFont displayFont;
    displayFont.setPointSize(14);
    inputBox->setFont(displayFont);
    outputBox->setFont(displayFont);

    mainLayout->addWidget(inputBox);
    mainLayout->addWidget(outputBox);

    // Buttons

    auto *grid = new QGridLayout();

    QString keys[7][4] = {
        {"SHIFT", "ALPHA", "MODE", "DEL"},
        {"sin", "cos", "tan", "^"},
        {"log", "ln", "√", "x²"},
        {"7", "8", "9", "÷"},
        {"4", "5", "6", "×"},
        {"1", "2", "3", "−"},
        {"0", ".", "=", "+"}};

    for (int i = 0; i < 7; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            QPushButton *btn = new QPushButton(keys[i][j]);
            btn->setMinimumHeight(45);

            grid->addWidget(btn, i, j);

            connect(btn, &QPushButton::clicked, this, [=]()
                    { handleButton(keys[i][j]); });
        }
    }

    mainLayout->addLayout(grid);

    setCentralWidget(central);
    setWindowTitle("Scientific Calculator");
    resize(600, 500);
}

void MainWindow::onEvaluate()
{
    QString input = inputBox->text();

    if (input.isEmpty())
        return;

    try
    {
        Value result = calc.evaluate(input.toStdString());
        std::string resultStr = Printer::toString(result);

        outputBox->setText(input + " = " + QString::fromStdString(resultStr));
    }
    catch (const CalcError &e)
    {
        outputBox->setText(QString::fromStdString(
            Printer::formatCalcError(input.toStdString(), e)));
    }
    catch (const std::exception &e)
    {
        outputBox->setText(QString::fromStdString(
            Printer::formatRuntimeError(e)));
    }
}

void MainWindow::handleButton(const QString &text)
{
    if (text == "=")
    {
        onEvaluate();
        return;
    }

    if (text == "DEL")
    {
        inputBox->backspace();
        return;
    }

    if (text == "SHIFT")
    {
        shiftMode = !shiftMode;
        modeLabel->setText(shiftMode ? "SHIFT" : "DEG");
        return;
    }

    if (text == "MODE")
    {
        // later: DEG/RAD toggle
        return;
    }

    // Function handling
    if (text == "sin" || text == "cos" || text == "tan")
    {
        if (shiftMode)
        {
            if (text == "sin")
                inputBox->insert("asin(");
            if (text == "cos")
                inputBox->insert("acos(");
            if (text == "tan")
                inputBox->insert("atan(");
        }
        else
        {
            inputBox->insert(text + "(");
        }
        return;
    }

    if (text == "√")
    {
        inputBox->insert("sqrt(");
        return;
    }

    if (text == "x²")
    {
        inputBox->insert("^2");
        return;
    }

    // Replace symbols with real operators
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
