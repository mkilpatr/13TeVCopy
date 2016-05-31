#!/bin/bash

runmacro=${1}
filename=${2}
index=${3}
treename=${4}
suffix=${5}
smeartemplatename=${6}
ismc=${7}
outputdir=${8}
scramdir=${9}
outfilename=${10}
outputpath=${11}
prefix=${12}

echo ${runmacro}
echo ${filename}
echo ${index}
echo ${treename}
echo ${suffix}
echo ${smeartemplatename}
echo ${ismc}
echo ${outputdir}
echo ${scramdir}
echo ${outfilename}
echo ${outputpath}
echo ${prefix}

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

root -l -b -q $runmacro+\(\"${prefix}${filename}\",$index,\"${treename}\",\"${suffix}\",\"${smeartemplatename}\",$ismc\)

pwd
ls -l
echo xrdcp "${outfilename}" "${prefix}""${outputpath}"/"${outfilename}"
xrdcp "${outfilename}" "${prefix}""${outputpath}"/"${outfilename}"
echo rm "${outfilename}"
rm "${outfilename}"

status=`echo $?`
echo "Status = $status"

exit $status
