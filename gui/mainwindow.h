#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QProcess>

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

 private:
  QProcess *process;
  QLineEdit *inputFileText;
  QLineEdit *outputDirText;
};

#endif  // MAINWINDOW_H
