cd Z:

Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
choco install wget -y
choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' --apply-install-arguments-to-dependencies -y

wget https://archive.apache.org/dist/xerces/c/3/sources/xerces-c-3.2.3.tar.gz -O downloads/xerces-c-3.2.5.tar.gz
tar -xzf downloads/xerces-c-3.2.5.tar.gz
cd xerces-c-3.2.5
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX=Z:/xerceswin ..
cmake --build . --config Release
cmake --install . --config Release

cd ..
cd ..
cd redatamwindows
cmake -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH="Z:/xerceswin" .
cmake --build . --config Release
cmake --install . --config Release
