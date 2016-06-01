Assume you use condor at FNAL.

[**Step 1**] Run over the MiniAOD:

`./makejobs.py -i [your config, e.g., ttbar.conf] -n [# file per job, e.g., 1] -o [output path, e.g., /eos/uscms/store/user/${USER}/13TeV/ntuples/outputs]`

[**Step 2**] Merge the files:

`./makejobs.py --runmerge -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/outputs] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] --splitmerge`

Here you need to specify how many output files you want, and how many input files to take for each output file.

[**Step 3**] Add weights:

`./makejobs.py --postprocess -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] -o /eos/uscms/store/user/${USER}/13TeV/ntuples/merged`

The extension samples will be combined with the original samples (*This is handled by makejobs.py now, so no need to comment out the -ext lines. However, make sure you have the -ext lines AFTER the original samples in your .conf file.*). 

[**Step 4**] Split files (only for signal scans):

`./makejobs.py --makegrid -i [T2fbd.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged] --mlspsteps [mlsp step]`