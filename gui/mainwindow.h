#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
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

private:
  QLineEdit *inputFileText;
  QLineEdit *outputDirText;
  QPushButton *inputFileButton;
  QPushButton *outputDirButton;
  QPushButton *convertButton;
  QLabel *footerLabel;
  QLabel *footerLink;
};

#endif // MAINWINDOW_H