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
json=${10}

workdir=`pwd`

echo `hostname`
echo "${_CONDOR_SCRATCH_DIR}"
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"
ls -l

source /cvmfs/cms.cern.ch/cmsset_default.sh
cd $scramdir/src/
SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
cd $workdir

### done in the transfer_input_files ###
#cp $scramdir/rootlogon.C .
#cp $scramdir/$runmacro .
#if [ ! -z "$json" ]; then
#  cp $scramdir/$json .
#fi

root -l -b -q $runmacro+\(\"${sname}\",$index,$ismc,\"${filename}\",$xsec,\"${outputdir}\",\"${prefix}\",\"${json}\"\)

status=`echo $?`
echo "Status = $status"

exit $status
