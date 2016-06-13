#!/bin/bash

runmacro=$1
inputfile=$2
outputname=$3
outputdir=$4
scramdir=$5

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

#cp ${scramdir}/${runmacro} .
#cp ${scramdir}/rootlogon.C .
#cp ${scramdir}/${inputfile} .


root -l -b -q $runmacro+\(\"${inputfile}\"\)

status=`echo $?`
echo "Status = $status"

if [[ "$outputdir" =~ ^/eos/uscms/.* ]]; then
  xrdcp -np ${outputname} root://cmseos:1094/${outputdir}/
  rm ${outputname}
else
  mv ${outputname} ${outputdir}/
fi

exit $status
