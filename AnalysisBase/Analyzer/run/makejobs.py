#!/usr/bin/env python
import os
import sys
import argparse
import subprocess

# script to help with submission of ntupling jobs, either interactively or using a batch system (LSF on lxplus CERN or condor on cmslpc at FNAL)
# use ./makejobs.py -h to explore the options and usage
# edit datasets.conf to choose samples to submit
# run ./makejobs.py with the appropriate options
# then, submit jobs using ./submitall.sh

parser = argparse.ArgumentParser(description='Prepare and submit ntupling jobs')
parser.add_argument("-i", "--input", dest="input", default="datasets.conf", help="Input configuration file with list of datasets. [Default: datasets.conf]")
parser.add_argument("-s", "--submit", dest="submit", default="submitall", help="Name of shell script to run for job submission. [Default: submitall]")
parser.add_argument("-n", "--numperjob", dest="numperjob", type=int, default=5, help="Number of files or events per job. Splittype determines whether splitting is by number of files (default) or by number of events. [Default: 5]")
parser.add_argument("-m", "--maxevents", dest="maxevents", type=int, default=-1, help="Maximum number of events to run over. [Default: -1 (all)]")
parser.add_argument("-p", "--pathtocfg", dest="path", default="../test", help="Path to directory with python configuration file to be run using cmsRun. [Default: \"../test/\"]")
parser.add_argument("-c", "--config", dest="config", default="runTestAnalyzer.py", help="Configuration file to be run using cmsRun to run. [Default: runTestAnalyzer.py]")
parser.add_argument("-o", "--outdir", dest="outdir", default="/store/user/${USER}/13TeV/ntuples", help="Output directory for ntuples. [Default: \"/store/user/${USER}/13TeV/ntuples\"]")
parser.add_argument("-j", "--jobdir", dest="jobdir", default="jobs", help="Directory for job files. [Default: jobs]")
parser.add_argument("-r", "--runscript", dest="script", default="runjobs", help="Shell script to be run by the jobs, [Default: runjobs]")
parser.add_argument("-t", "--submittype", dest="submittype", default="condor", choices=["interactive","lsf","condor"], help="Method of job submission. [Options: interactive, lsf, condor. Default: condor]")
parser.add_argument("-l", "--splittype", dest="splittype", default="file", choices=["file","event"], help="Split jobs by number of files or events. [Options: file, event. Default: file]")
parser.add_argument("-q", "--queue", dest="queue", default="8nh", help="LSF submission queue. [Default: 8nh]")
#parser.print_help()
args = parser.parse_args()

samples = []
datasets = []
with open(args.input,"r") as f :
    for line in f :
        content = line.split()
        if "#" in content[0] :
            continue
        samples.append(content[0])
        datasets.append(content[1])

os.system("mkdir -p %s" % args.jobdir)

eos="/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select"
if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
    os.system("%s mkdir -p %s" % (eos,args.outdir))
else :
    os.system("mkdir -p %s" % args.outdir)

maxevts = args.maxevents if args.splittype == "file" else args.numperjob

print "Creating submission file: ",args.submit+".sh"
script = open(args.submit+".sh","w")
script.write("""#!/bin/bash
outputdir={outdir}
jobdir={jobdir}
cfgfile={cfg}
numperjob={numperjob}
maxevents={maxevents}
""".format(outdir=args.outdir, jobdir=args.jobdir, pathtocfg=args.path, cfg=args.config, numperjob=args.numperjob, maxevents=maxevts))

if args.submittype == "lsf" or args.submittype == "condor" :
    script.write("""
workdir=$CMSSW_BASE
runscript={runscript}{stype}.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp {pathtocfg}/$cfgfile $workdir

echo "$cfgfile $runscript $workdir $outputdir"
""".format(pathtocfg=args.path,runscript=args.script,stype=args.submittype))

for isam in range(len(samples)) :
    cmd = ("./das_client.py --query \"file dataset=%s\" --limit=0 | grep store > %s/filelist_%s.txt" % (datasets[isam],args.jobdir,samples[isam]))
    subprocess.call(cmd,shell=True)
    cmd = ("./das_client.py --query \"dataset=%s | grep dataset.nevents\" | sed -n 4p | tr -d '\n'" % (datasets[isam]))
    ps = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
    numevents=int(ps.communicate()[0])
    if args.maxevents > -1  and args.maxevents < numevents :
        numevents = args.maxevents
    numlines = 0
    numperjob = args.numperjob
    infilename = "%s/filelist_%s.txt" % (args.jobdir,samples[isam])
    print "Creating jobs for %s" % samples[isam]
    with open(infilename,"r") as infile :
       numlines = len(infile.readlines())
    if args.splittype == "file" :
        numjobs = int(numlines)/int(numperjob)
        rem = numlines % numperjob
    elif args.splittype == "event" :
        numjobs = int(numevents)/int(numperjob)
        rem = numevents % numperjob
    if rem == 0 :
        numjobs -= 1
    if args.splittype == "file" :
        print "\t %d files to process --> will produce %d jobs with %d files per job" % (numlines,(numjobs+1),numperjob)
    elif args.splittype == "event" :
        print "\t %d events to process --> will produce %d jobs with %d events per job" % (numevents,(numjobs+1),numperjob)
    for ijob in range(numjobs+1) :
        start = (ijob*numperjob) + 1
        end = numperjob*(ijob+1)
        jobfile = "filelist_%s.txt" % (samples[isam])
        skipevts = 0
        if args.splittype == "file" :
            jobfile = "job_%d_%s.txt" % (ijob,samples[isam])
            os.system("sed -n %d,%dp %s > %s/%s" % (start,end,infilename,args.jobdir,jobfile))
        elif args.splittype == "event" :
            skipevts = start-1
        outfile = "output_%d_%s.root" % (ijob,samples[isam])
        suffix = "_numEvent{}".format(maxevts) if maxevts > -1 else ""
        cmd = ""
        if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
            cmd = ("%s ls %s | grep -c output_%d_%s%s.root" % (eos,args.outdir,ijob,samples[isam],suffix))
        else :
            cmd = "ls %s | grep -c output_%d_%s%s.root" % (args.outdir,ijob,samples[isam],suffix)
        ps = subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE)
        output = ps.communicate()[0][0]
        if int(output) > 0 :
            print "Output file output_%s_%d%s.root exists already! Skipping ..." % (samples[isam],ijob,suffix)
            continue

        if args.submittype == "interactive" :
            os.system("mkdir -p %s" % args.outdir)
            script.write("""cmsRun {pathtocfg}/$cfgfile print inputFiles_clear inputFiles_load=$jobdir/{infile} outputFile=$outputdir/{outputname} maxEvents={maxevents} skipEvents={skipevents}\n""".format(
            pathtocfg=args.path,infile=jobfile,outputname=outfile,maxevents=maxevts,skipevents=skipevts
            ))

        elif args.submittype == "lsf" :
            cpinput = ""
            if args.splittype == "file" or (args.splittype == "event" and ijob == 0) :
                cpinput = "\ncp $jobdir/%s $workdir \n" % (jobfile)
            script.write("""{cptxt}
bsub -q {queue} $runscript $cfgfile {infile} $outputdir {outputname} {maxevents} {skipevents} $workdir \n""".format(
            cptxt=cpinput,queue=args.queue,infile=jobfile,outputname=outfile,maxevents=maxevts,skipevents=skipevts
            ))

        elif args.submittype == "condor" :
            os.system("mkdir -p %s/logs" % args.jobdir)
            jobscript = open("{}/submit_{}_{}.sh".format(args.jobdir,samples[isam],ijob),"w")
            jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
request_memory          = 199
Executable              = {runscript}{stype}.sh
Arguments               = {cfg} {infile} {outputdir} {outputname} {maxevents} {skipevents} {workdir}
Output                  = logs/{sname}_{num}.out
Error                   = logs/{sname}_{num}.err
Log                     = logs/{sname}_{num}.log
use_x509userproxy       = true
initialdir              = {jobdir}
output_destination      = root://cmseos:1094/{outputdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{cfg},{workdir}/{infile}
transfer_output_files   = output_{num}_{sname}{evttag}.root
WhenToTransferOutput    = ON_EXIT
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format(
            runscript=args.script, stype=args.submittype, pathtocfg=args.path, cfg=args.config, infile=jobfile, workdir="${CMSSW_BASE}", sname=samples[isam], num=ijob, jobdir=args.jobdir, outputdir=args.outdir, outputname=outfile, maxevents=maxevts, skipevents=skipevts, evttag=suffix
            ))
            jobscript.close()
            script.write("cp $jobdir/{infile} $workdir \n./$jobdir/submit_{name}_{j}.sh\n".format(infile=jobfile, name=samples[isam], j=ijob))
            os.system("chmod +x %s/submit_%s_%d.sh" %(args.jobdir, samples[isam], ijob))


script.close()
os.system("chmod +x %s.sh" % args.submit)

print "Done!"
