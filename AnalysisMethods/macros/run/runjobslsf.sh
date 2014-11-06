#!/bin/bash

runmacro=$1
sname=$2
index=$3
ismc=$4
filename=$5
xsec=$6
outputdir=$7
prefix=$8
scramdir=$9

workdir=`pwd`

echo `hostname`
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"

cd $scramdir/src/
SCRAM_ARCH=slc6_amd64_gcc481
eval `scramv1 runtime -sh`
cd $workdir

cp $scramdir/rootlogon.C .
cp $scramdir/$runmacro .

root -l -b -q $runmacro+\(\"${sname}\",$index,$ismc,\"${filename}\",$xsec,\"${outputdir}\",\"${prefix}\"\)

status=`echo $?`
echo "Status = $status"

#outputname=`echo ${sname}_plots.root`
#if [ $index -gt -1 ]; then
#  outputname=`echo ${sname}_${index}_plots.root`
#fi
#mv ${sname}_${index}_plots.root ${outputdir}/

exit $status
