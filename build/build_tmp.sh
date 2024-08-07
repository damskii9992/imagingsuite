#!/bin/bash
if [ `uname` == 'Linux' ]; then
    SPECSTR="-spec linux-g++"
else
    SPECSTR="-spec macx-clang CONFIG+=x86_64"
fi

REPOSPATH=$WORKSPACE/imagingsuite

DEST=$WORKSPACE/builds


mkdir -p $DEST/build-StdBackProjectors
cd $DEST/build-StdBackProjectors

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../imagingsuite/frameworks/tomography/Backprojectors/StdBackProjectors/qt/StdBackProjectors/StdBackProjectors.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile


mkdir -p $DEST/build-StdPreprocModulesGUI
cd $DEST/build-StdPreprocModulesGUI

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../imagingsuite/frameworks/tomography/Preprocessing/StdPreprocModulesGUI/StdPreprocModulesGUI.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile



if [ -e "$REPOSPATH/frameworks/tomography/UnitTests" ]
then
    echo "Build tests"

    for f in `ls $REPOSPATH/frameworks/tomography/UnitTests`
    do
        echo "$REPOSPATH/frameworks/tomography/UnitTests/$f/$f.pro"
        if [ -e "$REPOSPATH/frameworks/tomography/UnitTests/$f/$f.pro" ]
        then
            mkdir -p $DEST/build-$f
            cd $DEST/build-$f

            $QTBINPATH/qmake -makefile $SPECSTR -o Makefile ../../imagingsuite/frameworks/tomography/UnitTests/$f/$f.pro
            make -f Makefile clean
            make -f Makefile mocables all
            make -f Makefile
        fi

    done

    echo "Tests built"
fi
