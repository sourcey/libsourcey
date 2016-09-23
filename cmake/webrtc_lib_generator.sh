#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

usage ()
{
cat << EOF

Usage:
   $0 [OPTIONS]

WebRTC static library conbiner script.

OPTIONS:
   -h             Show this message
   -o OUTDIR      WebRTC output library directory.
   -n LIBNAME     Output library name. Default: 'libwebrtc_scy.a'
   -x EXCLUSIONS  Exclusion grep patters. Default: 'jsoncpp|unittest|examples|main.o'
EOF
}

while getopts :o:n:x: OPTION; do
  case $OPTION in
  o) OUTDIR=$OPTARG ;;
  n) LIBNAME=$OPTARG ;;
  x) EXCLUSIONS=$OPTARG ;;
  ?) usage; exit 1 ;;
  esac
done


OUTDIR=${OUTDIR:-}
LIBNAME=${LIBNAME:-libwebrtc_scy.a}
EXCLUSIONS=${EXCLUSIONS:-jsoncpp|unittest|examples|main.o}


OBJECTS=$(find $OUTDIR -name '*.o' | grep -v -E $EXCLUSIONS)
# echo $OBJECTS
# echo $EXCLUSIONS
ar -rcT $OUTDIR/$LIBNAME $OBJECTS
