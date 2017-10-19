#!/bin/bash

runmacro=$1
inputfile=$2
inputdir=$3
outputname=$4
outputdir=$5
scramdir=$6

workdir=`pwd`

echo `hostname`
echo "${_CONDOR_SCRATCH_DIR}"
echo "scramdir: $scramdir"
echo "workdir: $workdir"
echo "args: $*"
ls -l

source /cvmfs/cms.cern.ch/cmsset_default.sh
#cd $scramdir/src/
SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 project CMSSW CMSSW_9_2_6`
cd CMSSW_9_2_6/src/
eval `scramv1 runtime -sh`
echo "CMSSW: "$CMSSW_BASE
cd ../../
#cd $workdir

xrdcp root://cmseos.fnal.gov//store/user/${USER}/CMSSW926.tgz .
tar -xf CMSSW926.tgz
rm CMSSW926.tgz

#cp ${scramdir}/${runmacro} .
#cp ${scramdir}/rootlogon.C .
#cp ${scramdir}/${inputfile} .

cd ${_CONDOR_SCRATCH_DIR}
echo $outputdir

xrdcp root://cmseos.fnal.gov/${inputdir}${inputfile} .

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
