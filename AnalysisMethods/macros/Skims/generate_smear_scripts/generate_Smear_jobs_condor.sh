#!/bin/bash
runmacro=${2}
smeartemplatename=${3}
outputdir=${4}

scramdir='${CMSSW_BASE}'

if [[ -e ../"${outputdir}" ]]; then
  rm -r ../"${outputdir}"
fi
mkdir ../"${outputdir}"
mkdir ../"${outputdir}"/"${outputdir}"
mkdir ../"${outputdir}"/"${outputdir}"/logs
cp template_files/runjobscondor.sh ../"${outputdir}"

SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
FILE_LIST=( `cat "$1"`)
IFS=$SAVEIFS

index=0
for file_list_entry in "${FILE_LIST[@]}"; do
  if [[ $index -eq 0 ]]; then
    filedir=`echo "$file_list_entry" | awk '{ print $1 }'`
    treename=`echo "$file_list_entry" | awk '{ print $2 }'`
    suffix=`echo "$file_list_entry" | awk '{ print $3 }'`
    ismc=`echo "$file_list_entry" | awk '{ print $4 }'`
    prefix=`echo "$file_list_entry" | awk '{ print $5 }'`
    outputpath=`echo "$file_list_entry" | awk '{ print $6 }'`
    eosmkdir /eos/uscms/"${outputpath}"
    sed -e s:MACRO_NAME:"${runmacro}":g -e s:OUTPUT_DIR:"${outputdir}":g -e s:SCRAM_DIR:"${scramdir}":g template_files/submit_all_template.sh > tempfile && mv tempfile ../"${outputdir}"/submit_all.sh
  else
    filename=`echo "$file_list_entry" | awk '{ print $1 }'`
    outfilename="${filename%.*}"_"${index}"_"${suffix}".root
    sed -e s:MACRO_NAME:"${runmacro}":g -e s:INFILE_PATH_NAME:"${filedir}"/"${filename}":g -e s:INFILE_NAME:"${filename%.*}":g -e s:INDEX:"${index}":g -e s:TREE_NAME:"${treename}":g -e s:SUFFIX:"${suffix}":g -e s:SMEAR_TEMPLATE_NAME:"${smeartemplatename}":g -e s:IS_MC:"${ismc}":g -e s:PWD:.:g -e s:SCRAM_DIR:"${scramdir}":g -e s:OUTFILE_NAME:"${outfilename}":g -e s:OUTFILE_DIR:"${outputpath}":g -e s%PREFIX%"${prefix}"%g -e s:OUTPUT_DIR:"${outputdir}":g template_files/submit_script_template.sh > tempfile && mv tempfile ../"${outputdir}"/submit_"${outputdir}"_"${index}".sh
    chmod +x ../"${outputdir}"/submit_"${outputdir}"_"${index}".sh
    echo "./submit_${outputdir}_${index}.sh" >> ../"${outputdir}"/submit_all.sh
  fi
  let index++
done
