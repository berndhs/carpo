#!/bin/bash

function pack_archive () {
  local PREFIX FILES TARFILE
  PREFIX=$1
  TARFILE=$2
  FILES=$(git ls-files)
  tar  \
    --transform="s+^+${PREFIX}+"  \
    -zcf  ${TARFILE} \
    ${FILES}
}

NAME=carpo
CHANGELOG=${NAME}.changes
DEB_TEMP_DIR=${NAME}.orig
DESKTOP=linux/${NAME}.desktop
VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
        | awk '{print $3;}' \
        | sed s/[\(\"\;\)]//g`

#makearchive.sh ${NAME}-${VERSION} master
ARCHIVE=${NAME}-${VERSION}
BRANCH=master
TARFILE=${ARCHIVE}.tar.gz

#git archive --prefix=${ARCHIVE}/ $BRANCH | gzip > $TARFILE

pack_archive ${ARCHIVE}/ ${TARFILE}
tar zxvf $TARFILE

mad -t harmattan-platform-api dpkg-source -b ${ARCHIVE}

tar zcf debian.tar.gz debian/


