#!/bin/bash

function pack_archive () {
  local PREFIX TARFILE FILES
  
  PREFIX=$1
  TARFILE=$2
  set -x
  FILES=$(git ls-files)
  tar  \
    --transform="s+^+${PREFIX}+"  \
    -zcf  ${TARFILE} \
    ${FILES}
  set +x
}

NAME=carpo
CHANGELOG=${NAME}.changes
DESKTOP=linux/${NAME}.desktop
VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
        | awk '{print $3;}' \
        | sed s/[\(\"\;\)]//g`
TARBALL=${NAME}_${VERSION}.tar.gz
PACKDIR=rpm_packaging

pack_archive ${NAME}_${VERSION} ${TARBALL}

cp ${TARBALL} ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
cp ${DESKTOP} ${PACKDIR}
echo ${NAME} > ${PACKDIR}/pack-name
echo ${VERSION} > ${PACKDIR}/pack-version
ls -l ${PACKDIR}/${TARBALL}
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
