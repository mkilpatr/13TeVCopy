Assume you use condor at FNAL.

[**Step 1**] Run over the MiniAOD:

`./makejobs.py -i [your config, e.g., ttbar.conf] -n [# file per job, e.g., 1] -o [output path, e.g., /eos/uscms/store/user/${USER}/13TeV/ntuples/outputs]`

> If you experience `[SSL: CERTIFICATE_VERIFY_FAILED]` issue with `das_client.py`, do

> `export SSL_CERT_DIR='/etc/pki/tls/certs:/etc/grid-security/certificates'`

> before running `makejobs.py`, or consider add this line into your `.bashrc`.

[**Step 2**] Merge the files:

`./makejobs.py --runmerge -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/outputs] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] --splitmerge`

Here you need to specify how many output files you want, and how many input files to take for each output file.

[**Step 3**] Add weights:

`./makejobs.py --postprocess -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] -o /eos/uscms/store/user/${USER}/13TeV/ntuples/merged`

The extension samples will be combined with the original samples (*This is handled by makejobs.py now, so no need to comment out the -ext lines. However, make sure you have the -ext lines AFTER the original samples in your .conf file.*). 

[**Step 4**] Split files (only for signal scans):

`./makejobs.py --makegrid -i [T2fbd.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged] --mlspsteps [mlsp step]`


==============

For Step 1, you can also use CRAB:

[**Step 1**] Run over the MiniAOD via CRAB, e.g.,:

`./makejobs.py -i ttbar.conf -n 1 -o /eos/uscms/store/user/${USER}/13TeV/010616 -t crab`

Remember to setup the CRAB environment before submitting:

`source /cvmfs/cms.cern.ch/crab3/crab_light.(c)sh`

Then for Step 2, you also need the `-t crab` option, because the output files are named differently (but the merging is running interacitvely. You can use `tmux` or `screen` to keep the script running.)

[**Step 2**] Merge CRAB output files:

`./makejobs.py --runmerge -i ttbar.conf --inputdir /eos/uscms/store/user/${USER}/13TeV/010616 -o /eos/uscms/store/user/${USER}/13TeV/010616/after_merge --splitmerge -t crab`

Step 3 and 4 is the same as in the above section and uses Condor.

