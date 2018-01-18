#!/bin/bash

CMSSW=${CMSSW_BASE##*/}

cd ../../../../../
eosrm /store/user/${USER}/${CMSSW}.tgz
tar --exclude-caches-all -zcvf ${CMSSW}.tgz $CMSSW --exclude="$CMSSW/src/AnalysisMethods/macros/run/plots_*" --exclude="$CMSSW/src/AnalysisBase/Analyzer/run/crab_projects/*" --exclude="$CMSSW/src/AnalysisMethods/EstTools/Moriond2017/tmp/*" --exclude="$CMSSW/src/AnalysisBase/Analyzer/*.root" --exclude="$CMSSW/src/.git/*" --exclude="*.pdf" --exclude="*.png" --exclude="*.log" --exclude="*.err" --exclude="*.out" --exclude="*.txt" --exclude="*.sh"
xrdcp ${CMSSW}.tgz root://cmseos.fnal.gov//store/user/${USER}/${CMSSW}.tgz

rm -f ${CMSSW}.tgz
cd -
