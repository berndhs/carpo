#!/bin/bash

PROJECT=carpo
NAME=$PROJECT
CHANGELOG=${NAME}.changes
DESKTOP=${NAME}.desktop
VERSION=`grep "ProgramVersion::VersionNumber" src/version.cpp \
        | awk '{print $3;}' \
        | sed s/[\(\"\;\)]//g`
PACKDIR=rpm_packaging

COMPRESS=z
TARFILE=${PROJECT}-${VERSION}.tar.gz

BRANCH=master

if [ $# -ge 1 ]; then
  BRANCH = $1
fi

FILES=$(git ls-files)
PREFIX=${PROJECT}-${VERSION}/

tar  \
    --transform="s+^+${PREFIX}+"  \
    -${COMPRESS}cf  ${TARFILE} \
    ${FILES}
#git archive --prefix=${PROJECT}-${VERSION}/ $BRANCH | $COMPRESS > $TARFILE
set -x
cp ${TARFILE} ${PACKDIR}
cp ${CHANGELOG} ${PACKDIR}
cp ${DESKTOP} ${PACKDIR}
set +x
