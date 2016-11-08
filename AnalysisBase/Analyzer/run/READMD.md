Assume you use condor at FNAL.

[**Step 1**] Run over the MiniAOD:

`./makejobs.py -i [your config, e.g., ttbar.conf] -n [# file per job, e.g., 1] -o [output path, e.g., /eos/uscms/store/user/${USER}/13TeV/ntuples/outputs]`

> If you experience `[SSL: CERTIFICATE_VERIFY_FAILED]` issue with `das_client.py`, do

> `export SSL_CERT_DIR='/etc/pki/tls/certs:/etc/grid-security/certificates'`

> before running `makejobs.py`, or consider add this line into your `.bashrc`.

[**Step 2**] Merge the files:

`./makejobs.py --runmerge -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/outputs] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] --splitmerge`

Here you only need to specify how many input files to include per output file, and the number of output files will be calculated automatically (the remaining files will be added to the last output file).

[**Step 3**] Add weights:

`./makejobs.py --postprocess -i [ttbar.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/after_merge] -o /eos/uscms/store/user/${USER}/13TeV/ntuples/merged`

The extension samples will be combined with the original samples (*This is handled by makejobs.py now, so no need to comment out the -ext lines. However, make sure you have the -ext lines AFTER the original samples in your .conf file.*). 

The QCD genjets5 samples will be patched together in this step (by scaling down the weights of N(genjets>=5) events). For it to work properly, make sure you have, e.g., the lines `qcd-ht300to500`, `qcd-ht300to500-ext`, `qcd-ht300to500-genjets5`, all in your .conf file, and they appear in this order.

[*NOTE*] For gen-filtered samples, gen-filter efficiency has to be calculated before adding weights.

[**Step 4**] Split files (only for signal scans):

`./makejobs.py --makegrid -i [T2fbd.conf] --inputdir [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged] -o [/eos/uscms/store/user/${USER}/13TeV/ntuples/merged]`


==============

For Step 1, you can also use CRAB:

[**Step 1**] Run over the MiniAOD via CRAB, e.g.,:

`./makejobs.py -i ttbar.conf -n 1 -o /eos/uscms/store/user/${USER}/13TeV/010616 -t crab`

Remember to setup the CRAB environment before submitting:

`source /cvmfs/cms.cern.ch/crab3/crab_light.(c)sh`

You can check the status of, resubmit, or kill **all** the jobs by

`./makejobs.py -i ttbar.conf -t crab --crabcmd (status|resubmit|kill|...)`

(Or run any crab command that works in this format: `crab [command] -d crab_projects/[name]`)

[**Step 2**] Merge CRAB output files:

For Step 2, you need to add the `--fromcrab` option, because the output files are named differently.

`./makejobs.py --runmerge -i ttbar.conf --inputdir /eos/uscms/store/user/${USER}/13TeV/010616 -o /eos/uscms/store/user/${USER}/13TeV/010616/after_merge --splitmerge --fromcrab`

Step 3 and 4 is the same as in the above section and uses Condor.

==============

**Calculate gen-filter efficiency**

The gen-filter info can only be accessed in `endLuminosityBlock()`, so it cannot be filled during normal ntuplization process. A dedicated simple analyzer `GenFilterAnalyzer` was created for this purpose.

[**Step 1**] Run this analyzer over the MiniAOD files, via condor (recommended, since the processing time is very short):

`./makejobs.py -c runGenFilterEfficiency.py -i signal.conf -n 10 -o /eos/uscms/store/user/${USER}/genEff/outputs`

[**Step 2**] Merge the output (can simply use hadd)

[**Step 3**] Run the script to calcuate gen-filter efficiencies, and upload the output json file to `data/xsec` directory.

`python calcGenEfficiency.py [merged_output.root] -o [Sample]_Filter_Efficiency.json`