#!/bin/bash

CMSSW=${CMSSW_BASE##*/}
user=${CMSSW_BASE%/CMSSW*}
user=${user##*/}

cd ../../../../../
eosrm /store/user/${user}/CMSSW926.tgz
tar --exclude-caches-all -zcvf CMSSW926.tgz $CMSSW --exclude="$CMSSW/src/AnalysisMethods/macros/run/plots_*" --exclude="$CMSSW/src/AnalysisBase/Analyzer/*.root" --exclude="*.pdf" --exclude="*.png" --exclude="*.log" --exclude="*.err" --exclude="*.out" --exclude="*.txt" --exclude="*.sh"
xrdcp CMSSW926.tgz root://cmseos.fnal.gov//store/user/${user}/CMSSW926.tgz

rm -f CMSSW926.tgz
cd -
