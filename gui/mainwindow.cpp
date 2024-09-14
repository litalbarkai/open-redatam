#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *vbox = new QVBoxLayout(centralWidget);

  // Input file selection
  QHBoxLayout *hbox1 = new QHBoxLayout();
  inputFileButton = new QPushButton("Select Input File", this);
  connect(inputFileButton, &QPushButton::clicked, this,
          &MainWindow::onSelectInputFile);
  inputFileText = new QLineEdit(this);
  hbox1->addWidget(inputFileButton);
  hbox1->addWidget(inputFileText);
  vbox->addLayout(hbox1);

  // Output directory selection
  QHBoxLayout *hbox2 = new QHBoxLayout();
  outputDirButton = new QPushButton("Select Output Directory", this);
  connect(outputDirButton, &QPushButton::clicked, this,
          &MainWindow::onSelectOutputDirectory);
  outputDirText = new QLineEdit(this);
  hbox2->addWidget(outputDirButton);
  hbox2->addWidget(outputDirText);
  vbox->addLayout(hbox2);

  // Convert button
  convertButton = new QPushButton("Convert", this);
  connect(convertButton, &QPushButton::clicked, this, &MainWindow::onConvert);
  vbox->addWidget(convertButton);

  // Footer
  QLabel *footerLabel = new QLabel(
      "The documentation and source code is available at: "
      "<a "
      "href=\"https://github.com/litalbarkai/redatam-converter\">https://"
      "github.com/litalbarkai/redatam-converter</a>",
      this);
  footerLabel->setOpenExternalLinks(true);
  vbox->addWidget(footerLabel);

  setCentralWidget(centralWidget);
  setWindowTitle("Redatam Converter");
}

MainWindow::~MainWindow() {}

void MainWindow::onSelectInputFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Select Input File", "",
      "Dictionary Files (*.dic *.dicx);;All Files (*)");
  if (!fileName.isEmpty()) {
    inputFileText->setText(fileName);
  }
}

void MainWindow::onSelectOutputDirectory() {
  QString directory =
      QFileDialog::getExistingDirectory(this, "Select Output Directory", "");
  if (!directory.isEmpty()) {
    outputDirText->setText(directory);
  }
}

void MainWindow::onConvert() {
  QString inputFilePath = inputFileText->text();
  QString outputDirPath = outputDirText->text();

  if (inputFilePath.isEmpty() || outputDirPath.isEmpty()) {
    QMessageBox::critical(
        this, "Error", "Please select both input file and output directory.");
    return;
  }

  QString program = "redatam";
  QStringList arguments;
  arguments << inputFilePath << outputDirPath + "/";

  QProcess process;
  process.start(program, arguments);
  if (!process.waitForStarted()) {
    QMessageBox::critical(this, "Error",
                          "Failed to start the conversion process. Please "
                          "ensure 'redatam' is installed and accessible.");
    return;
  }

  process.waitForFinished();

  QString output(process.readAllStandardOutput());
  QString error(process.readAllStandardError());

  // qDebug() << "Command:" << program << arguments.join(" ");
  // qDebug() << "Standard Output:" << output;
  // qDebug() << "Standard Error:" << error;

  if (process.exitCode() != 0) {
    QMessageBox::critical(this, "Error",
                          "Conversion failed. Please check the input file and "
                          "output directory.\n" +
                              error);
  } else {
    QMessageBox::information(this, "Success",
                             "Conversion successful!");
  }
}
