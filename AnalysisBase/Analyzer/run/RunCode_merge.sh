#/bin/bash
for i in "datasets_QCD12.conf" "datasets_QCD13.conf" "datasets_QCD14.conf" "datasets_QCD15.conf" "datasets_QCD16.conf" "datasets_QCD17.conf" "datasets_QCD18.conf" "datasets_QCD19.conf";
do
	./makejobs.py --runmerge -i $i --inputdir /eos/uscms/store/user/mkilpatr/13TeV/ntuples/outputs_crab/ -o /eos/uscms/store/user/mkilpatr/13TeV/ntuples/outputs_crab_merged/ --splitmerge --fromcrab
	source submit_runmerge.sh
done
