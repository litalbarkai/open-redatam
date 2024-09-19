cd Z:\redatamguiwindows\build

# install qt from https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-online-installer-windows-x64-4.8.0.exe
# it has to be Qt 5 for MSVC 2019 64-bit

# install directx
choco install directx -t

mkdir build
cd Z:\redatamguiwindows\build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
& "C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe" --release .\Release\redatamgui.exe
