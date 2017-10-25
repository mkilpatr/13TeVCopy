#!/bin/bash

workdir=$1
targetdir=$2

mkdir -p $targetdir

cd $workdir

# sr
#cd sr/
#hadd $targetdir/ttbarplusw_tree.root ttbar_tree.root wjets_tree.root tW_tree.root ttW_tree.root
#hadd $targetdir/diboson_tree.root ww_tree.root wz_tree.root zz_tree.root tWZ_tree.root
#hadd $targetdir/nonqcd_qcdcr_tree.root $targetdir/ttbarplusw_tree.root $targetdir/diboson_tree.root znunu_tree.root ttZ_tree.root
#cd ..

## photoncr
#cd photoncr
#hadd $targetdir/gjets_photoncr_tree.root gjets_tree.root qcd-fake_tree.root qcd-frag_tree.root ttg_tree.root
#hadd $targetdir/data_photoncr_tree.root singlephoton_tree.root jetht_tree.root
#cd ..

# link trees
cd $targetdir
# sr
#cp $workdir/met_tree.root data_tree.root
cp $workdir/znunu_tree.root ./
cp $workdir/qcd_orig_tree.root ./qcd_tree.root
cp $workdir/qcd_orig_tree.root ./qcd_qcdcr_tree.root
cp $workdir/ttZ_tree.root ./
#ln -s $workdir/signals/*.root ./
# tradlepcr: traditional (2015) lepcr
ln -s data_tree.root data_tradlepcr_tree.root
ln -s ttbarplusw_tree.root ttbarplusw_tradlepcr_tree.root
# qcdcr
ln -s data_tree.root data_qcdcr_tree.root
# photoncr
#ln -s $workdir/photoncr/singlepho_tree.root  data_photoncr_tree.root

# onelepcr: for MET+Lep method
#cd $targetdir/onelepcr
#ln -s ../ttbarplusw_tree.root
#ln -s $workdir/onelepcr/signals/*.root ./

echo "link sr signals from $workdir/signals/ by yourself"
#echo "link lepcr signals from $workdir/onelepcr/signals by yourself"

return

