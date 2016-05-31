#!/bin/bash
outputdir=OUTPUT_DIR
runmacro=MACRO_NAME

workdir=SCRAM_DIR
runscript=runjobscondor.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp ../../0LepSearchRegions/qcdPredictionMacros//rootlogon.C $workdir
cp ../$runmacro $workdir

echo "$runscript $runmacro $workdir $outputdir"    
