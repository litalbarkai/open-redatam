#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>

#include <string>

#define ID_SELECT_INPUT_FILE 1
#define ID_SELECT_OUTPUT_DIR 2
#define ID_CONVERT 3

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSelectInputFile(HWND hwnd);
void OnSelectOutputDirectory(HWND hwnd);
void OnConvert(HWND hwnd);

HWND hInputFileText;
HWND hOutputDirText;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow) {
  const wchar_t CLASS_NAME[] = L"RedatamConverter";

  WNDCLASSW wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  RegisterClassW(&wc);

  HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"Redatam Converter",
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                              800, 250, NULL, NULL, hInstance, NULL);

  if (hwnd == NULL) {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE: {
      CreateWindowW(L"BUTTON", L"Select Input File",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                    10, 150, 30, hwnd, (HMENU)ID_SELECT_INPUT_FILE,
                    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
      hInputFileText = CreateWindowW(
          L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 10, 600, 30,
          hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

      CreateWindowW(L"BUTTON", L"Select Output Directory",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10,
                    50, 150, 30, hwnd, (HMENU)ID_SELECT_OUTPUT_DIR,
                    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
      hOutputDirText = CreateWindowW(
          L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 50, 600, 30,
          hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

      CreateWindowW(L"BUTTON", L"Convert",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 350,
                    90, 100, 30, hwnd, (HMENU)ID_CONVERT,
                    (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
      break;
    }
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {
        case ID_SELECT_INPUT_FILE:
          OnSelectInputFile(hwnd);
          break;
        case ID_SELECT_OUTPUT_DIR:
          OnSelectOutputDirectory(hwnd);
          break;
        case ID_CONVERT:
          OnConvert(hwnd);
          break;
      }
      break;
    }
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSelectInputFile(HWND hwnd) {
  OPENFILENAMEW ofn;
  wchar_t szFile[260] = {0};

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = L"Dictionary Files\0*.dic;*.dicx\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileNameW(&ofn) == TRUE) {
    SetWindowTextW(hInputFileText, ofn.lpstrFile);
  }
}

void OnSelectOutputDirectory(HWND hwnd) {
  BROWSEINFOW bi = {0};
  bi.lpszTitle = L"Select Output Directory";
  LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

  if (pidl != 0) {
    wchar_t path[MAX_PATH];
    if (SHGetPathFromIDListW(pidl, path)) {
      SetWindowTextW(hOutputDirText, path);
    }
    CoTaskMemFree(pidl);
  }
}

void OnConvert(HWND hwnd) {
  wchar_t inputFilePath[260];
  wchar_t outputDirPath[260];

  GetWindowTextW(hInputFileText, inputFilePath, 260);
  GetWindowTextW(hOutputDirText, outputDirPath, 260);

  if (wcslen(inputFilePath) == 0 || wcslen(outputDirPath) == 0) {
    MessageBoxW(hwnd, L"Please select both input file and output directory.",
                L"Error", MB_OK | MB_ICONERROR);
    return;
  }

  std::wstring command = L"redatam.exe ";
  command += inputFilePath;
  command += L" ";
  command += outputDirPath;

  STARTUPINFOW si = {sizeof(si)};
  PROCESS_INFORMATION pi;

  if (!CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si,
                      &pi)) {
    MessageBoxW(hwnd,
                L"Failed to start the conversion process. Please ensure "
                L"'redatam' is installed and accessible.",
                L"Error", MB_OK | MB_ICONERROR);
    return;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exitCode;
  GetExitCodeProcess(pi.hProcess, &exitCode);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  if (exitCode != 0) {
    MessageBoxW(
        hwnd,
        L"Conversion failed. Please check the input file and output directory.",
        L"Error", MB_OK | MB_ICONERROR);
  } else {
    MessageBoxW(hwnd, L"Conversion successful!", L"Success",
                MB_OK | MB_ICONINFORMATION);
  }
}
