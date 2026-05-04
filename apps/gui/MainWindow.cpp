#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

#include "utility/Printer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    // History
    historyList = new QListWidget();

    // Input row
    auto *inputLayout = new QHBoxLayout();
    inputBox = new QLineEdit();
    evalButton = new QPushButton("Evaluate");

    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(evalButton);

    // Output
    outputBox = new QTextEdit();
    outputBox->setReadOnly(true);

    // Assemble layout
    mainLayout->addWidget(historyList);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(outputBox);

    setCentralWidget(central);

    // Connections
    connect(evalButton, &QPushButton::clicked,
            this, &MainWindow::onEvaluate);

    connect(inputBox, &QLineEdit::returnPressed,
            this, &MainWindow::onEvaluate);

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

        // Add to history
        historyList->addItem(input + " = " + QString::fromStdString(resultStr));

        // Show output
        outputBox->setText(QString::fromStdString(resultStr));
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
    inputBox->clear();
}