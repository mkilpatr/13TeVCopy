#/bin/bash
for i in "datasets_QCD1.conf" "datasets_QCD2.conf" "datasets_QCD3.conf" "datasets_QCD4.conf" "datasets_QCD5.conf" "datasets_QCD6.conf" "datasets_QCD7.conf" "datasets_QCD8.conf" "datasets_QCD9.conf" "datasets_QCD10.conf" "datasets_QCD11.conf" "datasets_QCD12.conf" "datasets_QCD13.conf" "datasets_QCD14.conf" "datasets_QCD15.conf" "datasets_QCD16.conf" "datasets_QCD17.conf" "datasets_QCD18.conf" "datasets_QCD19.conf";
do
	./makejobs.py -i $i -n 1 -o /store/user/mkilpatr/13TeV/ntuples/outputs_crab/ -j jobs_crab -t crab
	source submitall.sh
done
