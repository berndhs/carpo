#!/bin/bash

NAME=carpo
CHANGELOG=${NAME}.changes
DESKTOP=linux/${NAME}.desktop
VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
        | awk '{print $3;}' \
        | sed s/[\(\"\;\)]//g`
PACKDIR=rpm_packaging

makearchive.sh ${NAME}-${VERSION} master
cp ${NAME}-${VERSION}.tar.gz ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
cp ${DESKTOP} ${PACKDIR}
echo ${NAME} > ${PACKDIR}/pack-name
echo ${VERSION} > ${PACKDIR}/pack-version
ls -l ${PACKDIR}/${NAME}-${VERSION}.tar.gz
ls -l ${PACKDIR}/pack-*

if [ x$2 == "xfresh" ]
then
  rm ${PACKDIR}/pack-count
fi
  

if [ x$1 == "xmake" ]
then
  cd ${PACKDIR}
  make
fi