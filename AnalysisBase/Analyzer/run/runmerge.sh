#!/bin/bash

eos='/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select'
filedir=/eos/uscms/store/user/vdutta/13TeV/150715
conf=datasets.conf
prefix=""

if [[ "$filedir" =~ ^/eos/cms/.* && "$HOSTNAME" =~ .*cern.ch ]]; then
  $eos mkdir -p ${filedir}/merged
  prefix="root://eoscms/"
else
  mkdir -p ${filedir}/merged
  if [[ "$filedir" =~ ^/eos/uscms/.* && "$HOSTNAME" =~ .*fnal.gov ]]; then
    prefix="root://cmseos:1094/"
  fi
fi

for sname in `cat ${conf} | grep -v ^# | awk '{print $2}'` ; do
  echo ${prefix}${filedir}/merged/${sname}_ntuple.root > merge.txt
  file=""
  if [[ "$filedir" =~ ^/eos/cms/.* && "$HOSTNAME" =~ .*cern.ch ]]; then
    filelist=`$eos find -f ${filedir} | egrep "output_[0-9]+_${sname}(_numEvent[0-9]+|).root"`
  else
    filelist=`find ${filedir} | egrep "output_[0-9]+_${sname}(_numEvent[0-9]+|).root"`
  fi
  for file in ${filelist} ; do 
    echo ${prefix}${file} >> merge.txt
  done
  root -l -q -b MergeNtuples.C+\(\"merge.txt\"\)
done

rm -f merge.txt
