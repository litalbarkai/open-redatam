cd Z:\redatamguiwindows\build

# install qt from https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-windows-x64-4.8.0.exe
# it has to be Qt 5 for MSVC 2019 64-bit

# install directx
choco install directx -y

# then

# 1. Open Qt Creator.
# 2. Create a New Project: Create Project -> Application -> Qt Widgets Application
# 3. Copy sources from gui/: mainwindow.h, mainwindow.cpp, and main.cpp
# 4. Build: Build -> Build Project "redatamguiwindows"

# copy C:\Program Files (x86)\Windows Kits\10\Redist\D3D\x64\dxil.dll to Z:\redatamguiwindows\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug

mkdir build
cd Z:\redatamguiwindows\build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
& "C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe" --release .\Release\redatamgui.exe
