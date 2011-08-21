#!/bin/bash

function pack_archive () {
  local PREFIX BRANCH FILES
  
  FILES=$(git ls-files)
  tar  \
    --transform="s+^+${PREFIX}+"  \
    -zcf  ${TARFILE} \
    ${FILES}
}

NAME=carpo
CHANGELOG=${NAME}.changes
DESKTOP=linux/${NAME}.desktop
VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
        | awk '{print $3;}' \
        | sed s/[\(\"\;\)]//g`
PACKDIR=rpm_packaging

#makearchive.sh ${NAME}-${VERSION} master
pack_archive ${NAME}-${VERSION} 

cp ${NAME}-${VERSION}.tar.gz ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
cp ${DESKTOP} ${PACKDIR}
echo ${NAME} > ${PACKDIR}/pack-name
echo ${VERSION} > ${PACKDIR}/pack-version
ls -l ${PACKDIR}/${NAME}-${VERSION}.tar.gz
ls -l ${PACKDIR}/pack-*

MAKEIT="no"
for ARG in $*; do
  if [ x$ARG == "xfresh" ]
  then
    rm ${PACKDIR}/pack-count
  fi
  if [ x$ARG == "xmake" ]
  then
    MAKEIT="yes"
  fi
done

if [ "x$MAKEIT" == "xyes" ]; then
  cd ${PACKDIR}
  make
fi
