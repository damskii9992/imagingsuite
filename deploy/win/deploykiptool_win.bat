﻿@echo ----------------- Deploy copy --------------
#set REPOS=C:\Users\kaestner\repos
#set DEST=C:\Users\kaestner\kiptool

set REPOS=%WORKSPACE%
set DEST=%HOMEPATH%\kiptool

IF NOT EXIST %DEST% mkdir %DEST%
pushd .
cd %DEST%

copy %REPOS%\Applications\KipTool.exe .
copy %REPOS%\Applications\KipToolCLI.exe .
copy %REPOS%\lib\ImagingAlgorithms.dll .
copy %REPOS%\lib\ModuleConfig.dll .
copy %REPOS%\lib\ReaderConfig.dll .
copy %REPOS%\lib\ReaderGUI.dll .
copy %REPOS%\lib\QtAddons.dll .
copy %REPOS%\lib\QtModuleConfigure.dll .
copy %REPOS%\lib\kipl.dll .
copy %REPOS%\lib\ProcessFramework.dll .
copy %REPOS%\lib\AdvancedFilterModules.dll .
copy %REPOS%\lib\AdvancedFilterModulesGUI.dll .
copy %REPOS%\lib\BaseModules.dll .
copy %REPOS%\lib\BaseModulesGUI.dll .
copy %REPOS%\lib\ClassificationModules.dll .
copy %REPOS%\lib\ClassificationModulesGUI.dll . 
copy %REPOS%\lib\ImagingModules.dll .
copy %REPOS%\lib\ImagingModulesGUI.dll .

copy %REPOS%\imagingsuite\external\lib64\libtiff.dll .
copy %REPOS%\imagingsuite\external\lib64\libjpeg-62.dll .
copy %REPOS%\imagingsuite\external\lib64\zlib1.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3f-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libxml2-2.dll .
copy %REPOS%\imagingsuite\external\lib64\libiconv.dll .
copy %REPOS%\imagingsuite\external\lib64\cfitsio.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NeXus.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NexusCPP.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5_cpp.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\zlib.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\szip.dll .

copy %REPOS%\imagingsuite\external\lib64\libtiff.dll .
copy %REPOS%\imagingsuite\external\lib64\libjpeg-62.dll .
copy %REPOS%\imagingsuite\external\lib64\zlib1.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3f-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libxml2-2.dll .
copy %REPOS%\imagingsuite\external\lib64\libiconv.dll .
copy %REPOS%\imagingsuite\external\lib64\cfitsio.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NeXus.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NexusCPP.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5_cpp.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\zlib.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\szip.dll .

cd %QTBINPATH%
#cd C:\Qt\5.11.1\msvc2017_64\bin

windeployqt %DEST%\KipTool.exe
windeployqt %DEST%\KipToolCLI.exe
copy Qt5PrintSupport.dll %DEST%
copy Qt5Charts.dll %DEST%

popd