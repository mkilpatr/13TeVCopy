#/bin/bash
for i in "datasets_QCD_post4.conf" "datasets_QCD_post5.conf";
do
	./makejobs.py --postprocess -i $i --inputdir /eos/uscms/store/user/mkilpatr/13TeV/ntuples/outputs_crab_merged/ -o /eos/uscms/store/user/mkilpatr/13TeV/230916/merged/
	source submit_addweight.sh
done
