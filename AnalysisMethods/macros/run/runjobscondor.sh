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
outdir=${10}

workdir=`pwd`

echo `hostname`
echo "${_CONDOR_SCRATCH_DIR}"
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"
ls -l

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 project CMSSW CMSSW_9_2_6`
cd CMSSW_9_2_6/src/
eval `scramv1 runtime -sh`
scramv1 b ProjectRename
echo "CMSSW: "$CMSSW_BASE
cd ../../

xrdcp root://cmseos.fnal.gov//store/user/${USER}/CMSSW926.tgz .
tar -xf CMSSW926.tgz
rm CMSSW926.tgz

### done in the transfer_input_files ###
#cp $scramdir/rootlogon.C .
#cp $scramdir/$runmacro .
#if [ ! -z "$json" ]; then
#  cp $scramdir/$json .
#fi

cd ${_CONDOR_SCRATCH_DIR}
echo $outdir

root -l -b -q $runmacro+\(\"${sname}\",$index,$ismc,\"${filename}\",\"${outputdir}\",\"${prefix}\",\"${json}\"\)
#if [ $index -eq -1 ]
#then
#	xrdcp -np ${sname}_tree.root root://cmseos.fnal.gov//store/user/${USER}/13TeV/${outdir}/${sname}_tree.root
#else
#	xrdcp -np ${sname}_${index}_tree.root root://cmseos.fnal.gov//store/user/${USER}/13TeV/${outdir}/${sname}_${index}_tree.root
#fi
ls -a

status=`echo $?`
echo "Status = $status"

exit $status
