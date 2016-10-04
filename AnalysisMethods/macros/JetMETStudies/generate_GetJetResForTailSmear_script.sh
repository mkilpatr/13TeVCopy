#!/bin/bash
rootLogon_script_path=../rootlogon.C
script_name=$1
output_script=run_"${script_name%.*}"_script.sh
tree_name=Events
suffix=$3
isMC=true

if [[ -e $output_script ]]; then
  rm $output_script 
fi
touch $output_script
echo "#!/bin/bash" >> $output_script

SAVEIFS=$IFS
IFS=$(echo -en "\n\b")
FILE_LIST=( `cat "$2"`)
IFS=$SAVEIFS

file_index=0
for file_list_entry in "${FILE_LIST[@]}"; do
  if [[ $file_index -eq 0 ]]; then
    file_directory=`echo "$file_list_entry" | awk '{ print $1}'`
  else
    file_name=`echo "$file_list_entry" | awk '{ print $1}'`
    echo root -b -q "$rootLogon_script_path" \'"$script_name"+\(\""$file_directory"/"$file_name"\", $file_index , \""$tree_name"\", \""$suffix"\", "$isMC"\)\' \& >> $output_script
  fi
  let file_index++
done
