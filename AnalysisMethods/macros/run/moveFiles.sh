#!/bin/bash

end=$((SECONDS+3600))
count=$(./checkCondor.sh)

user=${CMSSW_BASE%/CMSSW*}
user=${user##*/}

if [[ $# -eq 0 ]] ; then
    echo 'You need to give directory where files are located'
    return
fi

while [ $SECONDS -lt $end -a $count -gt 0 ]; do

	watch -n 600 count=$(./checkCondor.sh)
done

if [ $count -eq 0 ]
then
	mv /eos/uscms/store/user/${user}/13TeV/$1 $CMSSW_BASE/src/AnalysisMethods/macros/run/
fi


