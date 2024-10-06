#include "mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), process(new QProcess(this)) {
  // Set window geometry
  setGeometry(0, 0, 400, 120);
  setWindowTitle("MainWindow");

  // Central widget and layout
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *vbox = new QVBoxLayout(centralWidget);

  // Input file selection
  QHBoxLayout *hbox1 = new QHBoxLayout();
  QPushButton *inputFileButton = new QPushButton("Select Input File", this);
  inputFileText = new QLineEdit(this);
  hbox1->addWidget(inputFileButton);
  hbox1->addWidget(inputFileText);
  vbox->addLayout(hbox1);

  // Output directory selection
  QHBoxLayout *hbox2 = new QHBoxLayout();
  QPushButton *outputDirButton =
      new QPushButton("Select Output Directory", this);
  outputDirText = new QLineEdit(this);
  hbox2->addWidget(outputDirButton);
  hbox2->addWidget(outputDirText);
  vbox->addLayout(hbox2);

  // Convert button
  QPushButton *convertButton = new QPushButton("Convert", this);
  vbox->addWidget(convertButton);

  // Footer
  QHBoxLayout *footerSizer = new QHBoxLayout();
  QLabel *footerText = new QLabel(this);
  footerText->setTextFormat(Qt::RichText);
  footerText->setTextInteractionFlags(Qt::TextBrowserInteraction);
  footerText->setOpenExternalLinks(true);
  footerText->setText(
      "The documentation and source code is available at: "
      "<a href=\"https://github.com/litalbarkai/redatam-converter\">https://"
      "github.com/litalbarkai/redatam-converter</a>");
  footerSizer->addWidget(footerText);
  vbox->addLayout(footerSizer);

  setCentralWidget(centralWidget);

  // Connect signals and slots
  connect(inputFileButton, &QPushButton::clicked, this,
          &MainWindow::onSelectInputFile);
  connect(outputDirButton, &QPushButton::clicked, this,
          &MainWindow::onSelectOutputDirectory);
  connect(convertButton, &QPushButton::clicked, this, &MainWindow::onConvert);
  connect(process,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
          &MainWindow::onProcessFinished);
}

MainWindow::~MainWindow() { delete process; }

void MainWindow::onSelectInputFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Select Input File"), "",
      tr("Dictionary Files (*.dic *.dicx);;All Files (*)"));
  if (!fileName.isEmpty()) {
    inputFileText->setText(fileName);
  }
}

void MainWindow::onSelectOutputDirectory() {
  QString dirName = QFileDialog::getExistingDirectory(
      this, tr("Select Output Directory"), "",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dirName.isEmpty()) {
    outputDirText->setText(dirName);
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

  process->start(program, arguments);
  if (!process->waitForStarted()) {
    QMessageBox::critical(this, "Error",
                          "Failed to start the conversion process. Please "
                          "ensure 'redatam' is installed and accessible.");
    return;
  }
}

void MainWindow::onProcessFinished(int exitCode,
                                   QProcess::ExitStatus exitStatus) {
  if (exitCode != 0) {
    QString error = process->readAllStandardError();
    QMessageBox::critical(this, "Error",
                          "Conversion failed. Please check the input file and "
                          "output directory.\n" +
                              error);
  } else {
    QMessageBox::information(this, "Success", "Conversion successful!");
  }
}
