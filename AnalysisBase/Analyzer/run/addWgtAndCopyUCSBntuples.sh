#!/bin/bash

eos='/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select'
filedir=/eos/cms/store/user/gouskos/13TeV/Phys14/20150503/merged
conf=test.conf

IFS=$'\n'

declare -a process
declare -a sample
declare -a xsec


let count=0
for line in `cat ${conf} | grep -v ^#` ;
do
    process[count]=`echo ${line} | awk '{print $1}'`
    sample[count]=`echo ${line} | awk '{print $2}'`
    xsec[count]=`echo ${line} | awk '{print $3}'`
    ((count++))
done < ${conf}


for ((i=0; i<${#sample[*]}; i++));
do
    echo "##dummy line for code to work" > ${sample[i]}.txt 
    $eos find -f ${filedir} | grep ${sample[i]} >> ${sample[i]}.txt
    path_=`$eos find -f ${filedir} | grep ${sample[i]}`
    tmp=$(root -l -q -b GetNumOfEvents.C+\(\"${sample[i]}.txt\"\))
    totalEvents=`echo ${tmp} | awk '{print $3}'`

    let count2=0
    for line2 in `cat ${sample[i]}.txt | grep -v ^#` ;
    do
	path="root://eoscms/"${line2}
	com='AddWgt2UCSBntuples.C+("'${path}'","'${process[i]}'",'${xsec[i]}',1.,'${totalEvents}',"TestAnalyzer/Events","wgtXSec")'
	root -l -q -b ${com}
	currentdir="/tmp/your_userid/cache/"${sample[i]}"_ntuple_wgtXSec.root"
	finaldir=${filedir}"/"${sample[i]}"_ntuple_wgtXSec_"${count2}".root"
	$eos cp ${currentdir} ${finaldir}
	rm ${currentdir}
	$eos rm ${line2}
	rm ${sample[i]}.txt
	((count2++))
    done

done
