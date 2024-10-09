#!/bin/bash

mkdir -p package/usr/local/bin
mkdir -p package/usr/share/applications
cp redatam package/usr/local/bin/
cp redatamgui package/usr/local/bin/
chmod +x package/usr/local/bin/redatam
chmod +x package/usr/local/bin/redatamgui
echo "[Desktop Entry]
Version=1.0
Name=Redatam GUI
Exec=/usr/local/bin/redatamgui
Icon=utilities-terminal
Type=Application
Categories=Utility;" > package/usr/share/applications/redatamgui.desktop
mkdir -p package/DEBIAN
echo "Package: redatam
Version: 0.0.1
Section: base
Priority: optional
Architecture: amd64
Maintainer: Mauricio Pacha Vargas Sepulveda <m.sepulveda@mail.utoronto.ca>
Depends: libxerces-c-dev, qtbase5-dev
Description: Redatam and Redatam GUI" > package/DEBIAN/control
dpkg-deb --build package redatam_0.0.1_amd64.deb
