#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/process.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

class MainWindow : public wxFrame {
 public:
  MainWindow(const wxString &title);

 private:
  void OnSelectInputFile(wxCommandEvent &event);
  void OnSelectOutputDirectory(wxCommandEvent &event);
  void OnConvert(wxCommandEvent &event);
  void OnProcessTerminated(wxProcessEvent &event);

  wxButton *inputFileButton;
  wxTextCtrl *inputFileText;
  wxButton *outputDirButton;
  wxTextCtrl *outputDirText;
  wxButton *convertButton;
  wxStaticText *footerLabel;

  wxProcess *process;
  wxString output;
  wxString error;

  wxDECLARE_EVENT_TABLE();
};

enum { ID_SelectInputFile = 1, ID_SelectOutputDirectory, ID_Convert };

#endif  // MAINWINDOW_H
