#!/usr/bin/env python
import os
import sys
import argparse
import subprocess

parser = argparse.ArgumentParser(description='Process config file')
parser.add_argument("-c", "--conf", dest="conf", default="run0lep.conf", help="Input configuration file. [Default: run0lep.conf]")
parser.add_argument("-s", "--submit", dest="submit", default="submitskims", help="Name of shell script to run for job submission. [Default: submitskims]")
parser.add_argument("-p", "--pathtomacro", dest="path", default="../Skims", help="Path to directory with run macro and configuration file. [Default: \"../Skims\"]")
parser.add_argument("-m", "--runmacro", dest="macro", default="TwoJetAddMVASkimmer.C", help="ROOT macro to run. [Default: TwoJetAddMVASkimmer.C]")
parser.add_argument("-x", "--prefix", dest="prefix", default="root://cmseos:1094/", help="Prefix needed to access files over xrootd, [Default: \"root://cmseos:1094/\"]")
parser.add_argument("-o", "--outdir", dest="outdir", default="${PWD}/skims", help="Output directory for skims, [Default: \"${PWD}/skims\"]")
parser.add_argument("-r", "--runscript", dest="script", default="runskimjobs", help="Shell script to be run by the jobs, [Default: runskimjobs]")
parser.add_argument("-t", "--submittype", dest="submittype", default="condor", choices=["interactive","lsf","condor"], help="Method of job submission. [Options: interactive, lsf, condor. Default: condor]")
parser.add_argument("-q", "--queue", dest="queue", default="1nh", help="LSF submission queue. [Default: 1nh]")
parser.add_argument("-u", "--postfix", dest="postfix", default="skim", help="Suffix to add to skimmed file name. [Default:\"skim\"]")
#parser.print_help()
args = parser.parse_args()

samples = []
files = []
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
            snum += 1
            continue
        files[snum-1].append(content[0])


print "Creating submission file: ",args.submit+".sh"
script = open(args.submit+".sh","w")
script.write("""#!/bin/bash
outputdir={outdir}
runmacro={macro}
prefix={prefix}
postfix={postfix}
""".format(outdir=args.outdir, pathtomacro=args.path, macro=args.macro, prefix=args.prefix,postfix=args.postfix))

eos="/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select"

if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
    os.system("%s mkdir -p %s" % (eos,args.outdir))
else :
    os.system("mkdir -p %s" % args.outdir)

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
    script.write("""
    
echo "$runscript $runmacro $workdir $outputdir"    
""")

for isam in range(len(samples)) :
    for ifile in range(len(files[isam])) :
        startindex = files[isam][ifile].rfind('/')+1
        endindex = files[isam][ifile].rfind('.')
        outputname = files[isam][ifile][startindex:endindex] + "_" + args.postfix + ".root"
        if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
            cmd = ("%s ls %s | grep -c %s" % (eos,args.outdir,outputname))
        else :
            cmd = "ls %s | grep -c %s" % (args.outdir,outputname)
        ps = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
        output = ps.communicate()[0][0]
        if int(output) > 0 :
            print "Output file %s exists already! Skipping ..." % (outputname)
            continue
        if args.submittype == "interactive" :
            script.write("""root -l -q -b {pathtomacro}/rootlogon.C {pathtomacro}/$runmacro+\(\\"{file}\\",\\"$prefix\\",\\"$outputdir\\",\\"$postfix\\"\)\n""".format(
            pathtomacro=args.path, file=files[isam][ifile] 
            ))
        elif args.submittype == "lsf" :
            script.write("""bsub -q {queue} $runscript $runmacro {file} $prefix $outputdir $postfix\n""".format(
            queue=args.queue, file=files[isam][ifile]
            ))
        elif args.submittype == "condor" :
            os.system("mkdir -p logs")
            jobscript = open("submit_{}_{}.sh".format(samples[isam],ifile),"w")
            startindex = files[isam][ifile].rfind('/')+1
            endindex = files[isam][ifile].rfind('.')
            outputname = files[isam][ifile][startindex:endindex] + "_" + args.postfix + ".root"
            jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
request_memory          = 199
Executable              = {runscript}{stype}.sh
Arguments               = {macro} {file} {prefix} {outdir} {postfix} {outname} {workdir}
Output                  = logs/{sname}_{num}.out
Error                   = logs/{sname}_{num}.err
Log                     = logs/{sname}_{num}.log
use_x509userproxy       = true
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{macro},{workdir}/rootlogon.C
WhenToTransferOutput    = ON_EXIT
Queue
EOF

  condor_submit submit.cmd;
  rm submit.cmd""".format(
            runscript=args.script, stype=args.submittype, pathtomacro=args.path, macro=args.macro, prefix=args.prefix, postfix=args.postfix, workdir="${CMSSW_BASE}", sname=samples[isam], file=files[isam][ifile], num=ifile, outdir=args.outdir, abspathtomacro=os.path.abspath(args.path+"/"+args.macro), abspathtorlogon=os.path.abspath(args.path+"/rootlogon.C"), outname=outputname
            ))
            jobscript.close()
            script.write("./submit_{name}_{j}.sh\n".format(name=samples[isam], j=ifile))
            os.system("chmod +x submit_%s_%d.sh" %(samples[isam], ifile))


script.close()
os.system("chmod +x %s.sh" % args.submit)

print "Done!"
