#!/bin/bash

runmacro=$1
sname=$2
index=$3
ismc=$4
filename=$5
outputdir=$6
prefix=$7
scramdir=$8
json=$9

workdir=`pwd`

### Do copy CMSSW tarball
tar -xf CMSSW.tar.gz

echo `hostname`
echo "${_CONDOR_SCRATCH_DIR}"
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"
ls -l

source /cvmfs/cms.cern.ch/cmsset_default.sh

### Don't copy CMSSW tarball
#cd $scramdir/src/
#SCRAM_ARCH=slc6_amd64_gcc491
#eval `scramv1 runtime -sh`

### Do copy CMSSW tarball
cd CMSSW_*/src/
SCRAM_ARCH=slc6_amd64_gcc491
eval `scramv1 runtime -sh`
scramdir=`pwd`
echo "New scramdir: $scramdir"

cd $workdir

### done in the transfer_input_files ###
#cp $scramdir/rootlogon.C .
#cp $scramdir/$runmacro .
#if [ ! -z "$json" ]; then
#  cp $scramdir/$json .
#fi

root -l -b -q $runmacro+\(\"${sname}\",$index,$ismc,\"${filename}\",\"${outputdir}\",\"${prefix}\",\"${json}\"\)

status=`echo $?`
echo "Status = $status"

exit $status
