#include "MainWindow.h"

#include <wx/hyperlink.h>
#include <wx/sstream.h>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_BUTTON(ID_SelectInputFile, MainWindow::OnSelectInputFile)
        EVT_BUTTON(ID_SelectOutputDirectory,
                   MainWindow::OnSelectOutputDirectory)
            EVT_BUTTON(ID_Convert, MainWindow::OnConvert)
                EVT_END_PROCESS(wxID_ANY, MainWindow::OnProcessTerminated)
                    wxEND_EVENT_TABLE()

                        MainWindow::MainWindow(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 250)) {
  wxPanel *panel = new wxPanel(this, wxID_ANY);

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  // Input file selection
  wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
  inputFileButton =
      new wxButton(panel, ID_SelectInputFile, wxT("Select Input File"));
  inputFileText = new wxTextCtrl(panel, wxID_ANY);
  hbox1->Add(inputFileButton, 0);
  hbox1->Add(inputFileText, 1, wxEXPAND | wxLEFT, 5);
  vbox->Add(hbox1, 0, wxEXPAND | wxALL, 10);

  // Output directory selection
  wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
  outputDirButton = new wxButton(panel, ID_SelectOutputDirectory,
                                 wxT("Select Output Directory"));
  outputDirText = new wxTextCtrl(panel, wxID_ANY);
  hbox2->Add(outputDirButton, 0);
  hbox2->Add(outputDirText, 1, wxEXPAND | wxLEFT, 5);
  vbox->Add(hbox2, 0, wxEXPAND | wxALL, 10);

  // Convert button
  convertButton = new wxButton(panel, ID_Convert, wxT("Convert"));
  vbox->Add(convertButton, 0, wxALIGN_CENTER | wxALL, 10);

  // Footer
  wxBoxSizer *footerSizer = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *footerText = new wxStaticText(
    panel, wxID_ANY,
    wxT("The documentation and source code is available at: "));
  wxHyperlinkCtrl *footerLink = new wxHyperlinkCtrl(
    panel, wxID_ANY, wxT("https://github.com/litalbarkai/redatam-converter"),
    wxT("https://github.com/litalbarkai/redatam-converter"));

  footerSizer->Add(footerText, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
  footerSizer->Add(footerLink, 0, wxALIGN_CENTER_VERTICAL);

  vbox->Add(footerSizer, 0, wxALIGN_CENTER | wxALL, 10);

  panel->SetSizer(vbox);
}

void MainWindow::OnSelectInputFile(wxCommandEvent &event) {
  wxFileDialog openFileDialog(
      this, _("Select Input File"), "", "",
      "Dictionary Files (*.dic;*.dicx)|*.dic;*.dicx;*.dicX|All Files (*.*)|*.*",
      wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openFileDialog.ShowModal() == wxID_CANCEL) return;

  inputFileText->SetValue(openFileDialog.GetPath());
}

void MainWindow::OnSelectOutputDirectory(wxCommandEvent &event) {
  wxDirDialog openDirDialog(this, _("Select Output Directory"), "",
                            wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (openDirDialog.ShowModal() == wxID_CANCEL) return;

  outputDirText->SetValue(openDirDialog.GetPath());
}

void MainWindow::OnConvert(wxCommandEvent &event) {
  wxString inputFilePath = inputFileText->GetValue();
  wxString outputDirPath = outputDirText->GetValue();

  if (inputFilePath.IsEmpty() || outputDirPath.IsEmpty()) {
    wxMessageBox("Please select both input file and output directory.", "Error",
                 wxOK | wxICON_ERROR);
    return;
  }

  wxString program = "redatam";
  wxArrayString arguments;
  arguments.Add(inputFilePath);
  arguments.Add(outputDirPath + "/");

  wxProcess *process = new wxProcess(this);
  process->Redirect();
  long pid = wxExecute(program + " " + arguments[0] + " " + arguments[1],
                       wxEXEC_ASYNC, process);
  if (!pid) {
    wxMessageBox(
        "Failed to start the conversion process. Please ensure 'redatam' is "
        "installed and accessible.",
        "Error", wxOK | wxICON_ERROR);
    delete process;
    return;
  }

  wxInputStream *stdoutStream = process->GetInputStream();
  wxInputStream *stderrStream = process->GetErrorStream();

  wxString output;
  wxString error;
  if (stdoutStream) {
    wxStringOutputStream stdoutStr(&output);
    stdoutStream->Read(stdoutStr);
  }
  if (stderrStream) {
    wxStringOutputStream stderrStr(&error);
    stderrStream->Read(stderrStr);
  }

  // Store the process and output for later use
  this->process = process;
  this->output = output;
  this->error = error;
}

void MainWindow::OnProcessTerminated(wxProcessEvent &event) {
  int exitCode = event.GetExitCode();
  delete process;

  if (exitCode != 0) {
    wxMessageBox(
        "Conversion failed. Please check the input file and output "
        "directory.\n" +
            error,
        "Error", wxOK | wxICON_ERROR);
  } else {
    wxMessageBox("Conversion successful!", "Success",
                 wxOK | wxICON_INFORMATION);
  }
}
