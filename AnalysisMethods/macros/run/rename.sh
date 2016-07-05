#!/bin/bash

workdir=$1
targetdir=$2

mkdir -p $targetdir

cd $workdir

# sr
cd sr/
hadd $targetdir/ttbarplusw_tree.root ttbar-mg_tree.root wjets-ht_tree.root tW_tree.root ttW_tree.root
hadd $targetdir/nonqcd_qcdcr_tree.root $targetdir/ttbarplusw_tree.root znunu_tree.root ttZ_tree.root
cd ..

# onelepcr
cd lepcr/
mkdir -p $targetdir/onelepcr
hadd $targetdir/onelepcr/ttbarplusw_onelepcr_tree.root ttbar-mg_tree.root wjets-ht_tree.root tW_tree.root ttW_tree.root
cd ..

# photoncr
cd photoncr
hadd $targetdir/gjets_photoncr_tree.root gjets_tree.root qcd-fake_tree.root qcd-frag_tree.root ttg_tree.root
cd ..

# link trees
cd $targetdir
# sr
ln -s $workdir/sr/met_tree.root data_tree.root
ln -s $workdir/sr/znunu_tree.root
ln -s $workdir/sr/qcd_tree.root
ln -s $workdir/sr/ttZ_tree.root
ln -s $workdir/signals/*.root ./
# qcdcr
ln -s $workdir/sr/met_tree.root data_qcdcr_tree.root
ln -s $workdir/sr/qcd_tree.root qcd_qcdcr_tree.root
# photoncr
ln -s $workdir/photoncr/singlepho_tree.root  data_photoncr_tree.root

# onelepcr
cd $targetdir/onelepcr
ln -s $workdir/lepcr/singlelep_tree.root data_onelepcr_tree.root
ln -s ../ttbarplusw_tree.root
ln -s $workdir/onelepcr/signals/*.root ./

exit 0

