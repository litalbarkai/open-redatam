!define PRODUCT_NAME "Open Redatam"
!define PRODUCT_VERSION "0.2"
!define PRODUCT_PUBLISHER "Pacha"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "open-redatam-windows.exe"
InstallDir "$PROGRAMFILES\Open Redatam"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""

Page directory
Page instfiles

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  File /r "Release\*.*"
  CreateDirectory "$SMPROGRAMS\Open Redatam"
  CreateShortCut "$SMPROGRAMS\Open Redatam\Open Redatam.lnk" "$INSTDIR\redatamgui.exe"
  CreateShortCut "$DESKTOP\Open Redatam.lnk" "$INSTDIR\redatamgui.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  Delete "$SMPROGRAMS\Open Redatam\Open Redatam.lnk"
  RMDir "$SMPROGRAMS\Open Redatam"
  Delete "$DESKTOP\Open Redatam.lnk"
  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
SectionEnd
