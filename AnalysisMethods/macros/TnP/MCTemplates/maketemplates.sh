#bin: pt, eta
python getTemplatesFromMC.py --input trees/dyjetstoll-mu_tree.root -o zmm_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etabins=0,0.8,1.6,2.4
python getTemplatesFromMC.py --input trees/dyjetstoll-el_tree.root -o zee_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etabins=0.0,0.8,1.5,2.4

#bin: pt, htalong
#python getTemplatesFromMC.py --input trees/dyjetstoll-mu_tree.root -o zmm_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etaVarName htalong --etabins=0,50,100,300
#python getTemplatesFromMC.py --input trees/dyjetstoll-el_tree.root -o zee_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etaVarName htalong --etabins=0,50,100,300

#bin: pt, annulus
#python getTemplatesFromMC.py --input trees/dyjetstoll-mu_tree.root -o zmm_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etaVarName annulus --etabins=0,1,2,3,10
#python getTemplatesFromMC.py --input trees/dyjetstoll-el_tree.root -o zee_templates.root --idprobe pass --weightVarName weight --ptbins=10,15,20,30,40,60,100,1000 --etaVarName annulus --etabins=0,1,2,3,10
