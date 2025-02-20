#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QProcess>
#include <QProgressBar>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void onSelectInputFile();
  void onSelectOutputDirectory();
  void onConvert();
  void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void onReadyReadStandardOutput();
  void onReadyReadStandardError();
  void onBuyMeACoffee();
  void updateProgress(int value, const QString &entity);

 private:
  QProcess *process;
  QLineEdit *inputFileText;
  QLineEdit *outputDirText;
  QProgressBar *progressBar;
  QLabel *statusLabel;
};

#endif  // MAINWINDOW_H
