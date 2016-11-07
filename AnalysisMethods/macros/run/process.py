#!/usr/bin/env python
import os
import sys
import argparse

parser = argparse.ArgumentParser(description='Process config file')
parser.add_argument("-c", "--conf", dest="conf", default="run0lep.conf", help="Input configuration file. [Default: run0lep.conf]")
parser.add_argument("-s", "--submit", dest="submit", default="submitall", help="Name of shell script to run for job submission. [Default: submitall]")
parser.add_argument("-p", "--pathtomacro", dest="path", default="..", help="Path to directory with run macro and configuration file. [Default: \"../\"]")
parser.add_argument("-m", "--runmacro", dest="macro", default="makeZeroLeptonSRTrees.C", help="ROOT macro to run. [Default: makeZeroLeptonSRTrees.C]")
parser.add_argument("-x", "--prefix", dest="prefix", default="root://cmseos:1094/", help="Prefix needed to access files over xrootd, [Default: \"root://cmseos:1094/\"]")
parser.add_argument("-o", "--outdir", dest="outdir", default="${PWD}/plots", help="Output directory for plots, [Default: \"${PWD}/plots\"]")
parser.add_argument("-r", "--runscript", dest="script", default="runjobs", help="Shell script to be run by the jobs, [Default: runjobs]")
parser.add_argument("-t", "--submittype", dest="submittype", default="condor", choices=["interactive","lsf","condor"], help="Method of job submission. [Options: interactive, lsf, condor. Default: condor]")
parser.add_argument("-q", "--queue", dest="queue", default="1nh", help="LSF submission queue. [Default: 1nh]")
parser.add_argument("-j", "--json", dest="json", default="Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt", help="json file to use. [Default: %(default)s]")
parser.add_argument("--output-suffix", dest="suffix", default="_tree.root", help="Suffix of output file. [Default: %(default)s. Use '.json' with dumpJSON.C.]")
#parser.print_help()
args = parser.parse_args()

samples = []
files = []
types = []
state = 0
snum = 0
with open((args.path+"/"+args.conf),"r") as f :
    for line in f :
        content = line.split()
        if "#" in content[0] :
            continue
        if content[0] == "%" :
            state += 1
            continue
        if content[0] == "$" :
            samples.append(content[1])
            files.append([])
            types.append([])
            snum += 1
            continue
        if state == 0 :
            files[snum-1].append(content[0])
            types[snum-1].append(1)
        elif state == 1 :
            files[snum-1].append(content[0])
            types[snum-1].append(0)


print "Creating submission file: ",args.submit+".sh"
script = open(args.submit+".sh","w")
script.write("""#!/bin/bash
outputdir={outdir}
runmacro={macro}
prefix={prefix}
""".format(outdir=args.outdir, pathtomacro=args.path, macro=args.macro, prefix=args.prefix))

if args.submittype == "lsf" or args.submittype == "condor" :
    script.write("""
workdir=$CMSSW_BASE
runscript={runscript}{stype}.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp {pathtomacro}/rootlogon.C $workdir
cp {pathtomacro}/$runmacro $workdir
""".format(pathtomacro=args.path,runscript=args.script,stype=args.submittype)) 
    if args.json != '' : script.write("cp $workdir/src/data/JSON/{jsonfile} $workdir".format(jsonfile=args.json))
    script.write("""
    
echo "$runscript $runmacro $workdir $outputdir"    
""")

for isam in range(len(samples)) :
    for ifile in range(len(files[isam])) :
        findex = ifile if len(files[isam]) > 1 else -1
        ismc = 1 if types[isam][ifile] == 0 else 0
        if args.submittype == "interactive" :
            script.write("""root -l -q -b {pathtomacro}/rootlogon.C {pathtomacro}/$runmacro+\(\\"{sname}\\",{index},{mc},\\"{file}\\",\\"$outputdir\\",\\"$prefix\\",\\"{json}\\"\)\n""".format(
            pathtomacro=args.path, sname=samples[isam], index=findex, mc=ismc, file=files[isam][ifile], json="$CMSSW_BASE/src/data/JSON/"+args.json if args.json!='' else '' 
            ))
        elif args.submittype == "lsf" :
            script.write("""bsub -q {queue} $runscript $runmacro {sname} {index} {mc} {file} $outputdir $prefix $workdir {json}\n""".format(
            queue=args.queue, sname=samples[isam], index=findex, mc=ismc, file=files[isam][ifile], json=args.json
            ))
        elif args.submittype == "condor" :
            os.system("mkdir -p %s/logs" % args.outdir)
            jobscript = open("submit_{}_{}.sh".format(samples[isam],ifile),"w")
            outputname = samples[isam]+args.suffix if findex == -1 else samples[isam]+"_{}".format(findex)+args.suffix
            addJSON = ',${CMSSW_BASE}/'+args.json if args.json !='' else ''
            jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
Executable              = {runscript}{stype}.sh
Arguments               = {macro} {sname} {index} {mc} {file} . {prefix} {workdir} {json}
Output                  = logs/{sname}_{num}.out
Error                   = logs/{sname}_{num}.err
Log                     = logs/{sname}_{num}.log
use_x509userproxy       = true
initialdir              = {outdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{macro},{workdir}/rootlogon.C{addjson}
transfer_output_files   = {outname}
WhenToTransferOutput    = ON_EXIT
Queue
EOF

  condor_submit submit.cmd;
  rm submit.cmd""".format(
            runscript=args.script, stype=args.submittype, pathtomacro=args.path, macro=args.macro, prefix=args.prefix, workdir="${CMSSW_BASE}", sname=samples[isam], index=findex, mc=ismc, file=files[isam][ifile], num=ifile, outdir=args.outdir, abspathtomacro=os.path.abspath(args.path+"/"+args.macro), abspathtorlogon=os.path.abspath(args.path+"/rootlogon.C"), outname=outputname, addjson=addJSON, json=args.json
            ))
            jobscript.close()
            script.write("./submit_{name}_{j}.sh\n".format(name=samples[isam], j=ifile))
            os.system("chmod +x submit_%s_%d.sh" %(samples[isam], ifile))


script.close()
os.system("chmod +x %s.sh" % args.submit)

print "Done!"
