#include "mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), process(new QProcess(this)) {
  // Set window geometry
  setGeometry(0, 0, 400, 200);
  setWindowTitle("Open Redatam");

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

  // Donate button
  QPushButton *buyMeACoffeeButton = new QPushButton("Donate", this);

  // Create a horizontal layout for the buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(convertButton);
  buttonLayout->addWidget(buyMeACoffeeButton);

  // Set stretch factors to make each button take 50% of the width
  buttonLayout->setStretch(0, 1);
  buttonLayout->setStretch(1, 1);

  // Add the button layout to the main vertical layout
  vbox->addLayout(buttonLayout);

  // Progress bar and status label
  progressBar = new QProgressBar(this);
  progressBar->setRange(0, 100);
  progressBar->setValue(0);
  statusLabel = new QLabel("", this);
  vbox->addWidget(progressBar);
  vbox->addWidget(statusLabel);

  // Footer
  QHBoxLayout *footerSizer = new QHBoxLayout();
  QLabel *footerText = new QLabel(this);
  footerText->setTextFormat(Qt::RichText);
  footerText->setTextInteractionFlags(Qt::TextBrowserInteraction);
  footerText->setOpenExternalLinks(true);
  footerText->setText(
      "The documentation and source code is available at: "
      "<a href=\"https://github.com/litalbarkai/open-redatam\">https://"
      "github.com/litalbarkai/open-redatam</a>");
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
  connect(process, &QProcess::readyReadStandardOutput, this,
          &MainWindow::onReadyReadStandardOutput);
  connect(process, &QProcess::readyReadStandardError, this,
          &MainWindow::onReadyReadStandardError);
  connect(buyMeACoffeeButton, &QPushButton::clicked, this,
          &MainWindow::onBuyMeACoffee);
}

MainWindow::~MainWindow() { delete process; }

void MainWindow::onSelectInputFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Select Input File"), "",
      tr("Dictionary Files (*.dic *.dicx *.dicX *.DIC *.DICX)"));
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

#ifdef _WIN32
  // Replace forward slashes with backslashes for Windows
  inputFilePath.replace("/", "\\");
  outputDirPath.replace("/", "\\");
#endif

  QString program = "redatam";
  QStringList arguments;

#ifdef _WIN32
  arguments << inputFilePath << outputDirPath + "\\";
#else
  arguments << inputFilePath << outputDirPath + "/";
#endif

  process->start(program, arguments);
  if (!process->waitForStarted()) {
    QMessageBox::critical(this, "Error",
                          "Failed to start the conversion process. Please "
                          "ensure 'redatam' is installed and accessible.");
    return;
  }

  progressBar->setValue(0);
  statusLabel->setText("Starting conversion...");
}

void MainWindow::onProcessFinished(int exitCode,
                                   QProcess::ExitStatus exitStatus) {
  if (exitCode != 0) {
    QString error = process->readAllStandardError();
    QMessageBox::critical(this, "Error",
                          "Conversion failed. Please check the input file and "
                          "output directory.\n" +
                              error);
  }
  // else {
  //   QMessageBox::information(this, "Success", "Conversion successful!");
  // }
}

void MainWindow::onReadyReadStandardOutput() {
  QString output = process->readAllStandardOutput();

  // The output contains lines like "Exporting ENTITY..."
  QRegExp regex("Exporting (\\w+)...");
  if (regex.indexIn(output) != -1) {
    QString entity = regex.cap(1);
    int progress = 0;
    if (progress < 100) {
      progress += 10;
    }
    updateProgress(progress, entity);
  }
}

void MainWindow::onReadyReadStandardError() {
  QString error = process->readAllStandardError();
}

void MainWindow::updateProgress(int value, const QString &entity) {
  progressBar->setValue(value);
  statusLabel->setText("Exporting " + entity + "...");

  if (value == 100) {
    statusLabel->setText("Conversion successful!");
  }
}

void MainWindow::onBuyMeACoffee() {
  QDesktopServices::openUrl(QUrl("https://www.buymeacoffee.com/pacha"));
}
