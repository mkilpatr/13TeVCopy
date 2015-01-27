#!/bin/bash

cfgfile=$1
filename=$2
outputdir=$3
outputname=$4
maxevents=$5
skipevents=$6
scramdir=$7

workdir=`pwd`

echo `hostname`
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"

cd $scramdir/src/
SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
cd $workdir

cp ${scramdir}/${cfgfile} .
cp ${scramdir}/${filename} .

cmsRun $cfgfile print inputFiles_clear inputFiles_load=${filename} outputFile=${outputname} maxEvents=${maxevents} skipEvents=${skipevents}

status=`echo $?`
echo "Status = $status"

if [ $maxevents -gt -1 ]; then
  outputname=`echo ${outputname} | sed "s/.root/_numEvent${maxevents}.root/"`
fi

cmsStage -f ${outputname} ${outputdir}

exit $status
