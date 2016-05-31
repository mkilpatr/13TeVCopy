#!/bin/bash

workdir=$1

cd $workdir


# sr
cd sr/
hadd ttbarplusw_tree.root ttbar-mg_tree.root wjets-ht_tree.root tW_tree.root ttW_tree.root
mv ttZ_tree.root ttZ_only_tree.root && hadd ttZ_tree.root ttZ_only_tree.root tZq_tree.root
hadd nonqcd_qcdcr_tree.root ttbarplusw_tree.root znunu_tree.root ttZ_tree.root
cd ..
ln -s sr/htmht_tree.root data_tree.root
ln -s sr/znunu_tree.root
ln -s sr/ttbarplusw_tree.root
ln -s sr/qcd_tree.root
ln -s sr/ttZ_tree.root

# onelepcr
ln -s sr/htmht_tree.root data_onelepcr_tree.root
ln -s sr/ttbarplusw_tree.root ttbarplusw_onelepcr_tree.root

# qcdcr
ln -s sr/htmht_tree.root data_qcdcr_tree.root
ln -s sr/qcd_tree.root qcd_qcdcr_tree.root
ln -s sr/nonqcd_qcdcr_tree.root

# photoncr
cd photoncr
hadd photon_tree.root gjets_tree.root qcd-fake_tree.root qcd-frag_tree.root ttg_tree.root
ln -s singlepho_tree.root data_tree.root
cd ..
ln -s photoncr/singlepho_tree.root  data_photoncr_tree.root
ln -s photoncr/photon_tree.root gjets_photoncr_tree.root

# zllcr
cd zllcr
ln -s doublelep_tree.root data_tree.root
hadd zll_tree.root dyjetstoll-madgraph_tree.root tZq_tree.root ttZ_tree.root
hadd ttbar_tree.root ttbar-mg_tree.root ttW_tree.root tW_tree.root
cd ..


exit 0

####### ignore ttz for now ########
# ttzcr
cd ../ttzcr/pieces
mkdir data
mv *2015d* data/
mkdir dy_mg
mv dyjetstoll-madgraph*.root dy_mg
mkdir dy_amc
mv *amcatnlo*.root dy_amc
mkdir other
mv *.root other

hadd ../data_tree.root data/*root
hadd ../ttz-all_amc.root other/*root dy_amc/*root
hadd ../ttz-all_mg.root other/*root dy_mg/*root
cd ..
ln -s ttz-all_mg.root ttz-all_tree.root
