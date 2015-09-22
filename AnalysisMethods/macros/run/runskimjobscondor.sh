#!/bin/bash

runmacro=$1
filename=$2
prefix=$3
outputdir=$4
postfix=$5
outputname=$6
scramdir=$7

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

root -l -b -q $runmacro+\(\"${filename}\",\"${prefix}\",\".\",\"${postfix}\"\)

status=`echo $?`
echo "Status = $status"

if [[ "$outputdir" =~ ^/eos/uscms/.* ]]; then
  xrdcp -np ${outputname} root://cmseos:1094/${outputdir}/
  rm ${outputname}
else
  mv ${outputname} ${outputdir}/
fi

exit $status
