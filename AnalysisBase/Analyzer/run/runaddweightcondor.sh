#!/bin/bash

runmacro=$1
filename=$2
process=$3
xsec=$4
lumi=$5
totposevents=$6
totnegevents=$7
treename=$8
suffix=$9
xsecfile=${10}
filterfile=${11}
wgtsf=${12}
outputname=${13}
outputdir=${14}
scramdir=${15}

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

cp ${scramdir}/${runmacro} .
cp ${scramdir}/rootlogon.C .

root -l -b -q $runmacro+\(\"${filename}\",\"${process}\",$xsec,$lumi,$totposevents,$totnegevents,\"${treename}\",\"${suffix}\",\"${xsecfile}\",\"${filterfile}\",$wgtsf\)

status=`echo $?`
echo "Status = $status"

if [[ "$outputdir" =~ ^/eos/uscms/.* ]]; then
  xrdcp -np ${outputname} root://cmseos:1094/${outputdir}/
  rm ${outputname}
else
  mv ${outputname} ${outputdir}/
fi

exit $status
