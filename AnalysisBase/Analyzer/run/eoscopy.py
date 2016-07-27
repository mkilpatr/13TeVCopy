#!/usr/bin/env python
import os
import argparse

parser = argparse.ArgumentParser(description='Prepare and submit ntupling jobs')
parser.add_argument("-i", "--inputdir", dest="inputdir", help="Input dir.")
parser.add_argument("-n", "--numperjob", dest="numperjob", type=int, default=5, help="Number of files or events per job. [Default: 5]")
parser.add_argument("-o", "--outdir", dest="outdir", default="/eos/uscms/store/user/${USER}/13TeV/ntuples", help="Output directory for ntuples. [Default: \"/eos/uscms/store/user/${USER}/13TeV/ntuples\"]")
args = parser.parse_args()

cmdtmpl = 'xrdcp -np %s %s\n'
eos_prefix = 'root://cmseos.fnal.gov/'

indir = args.inputdir.replace('/eos/uscms', eos_prefix)
outdir = args.outdir.replace('/eos/uscms', eos_prefix)
if not os.path.exists(args.outdir):
    os.system("mkdir -p %s" % args.outdir)

filelist = os.listdir(args.inputdir)
numfiles = len(filelist)
numjobs = numfiles/args.numperjob
if numfiles % args.numperjob != 0: numjobs = numjobs+1
print '%d files to be copied in %d jobs'%(numfiles, numjobs)

jobdir = 'jobs/eoscopy'
os.system("mkdir -p %s/logs" % jobdir)

print 'Creating submission file: submit_copy.sh'
with open('submit_copy.sh', 'w') as script:
    script.write('#!/bin/bash\n\n')
    
    for ijob in range(numjobs):
        with open(os.path.join(jobdir, 'eoscopy_%d.sh'%ijob), 'w') as f:
            f.write('#!/bin/bash\n')
            nstart = ijob*args.numperjob
            for idx in range(args.numperjob):
                ifile = nstart+idx
                if ifile == numfiles: break
                fn = filelist[ifile]
                cmd = cmdtmpl % (os.path.join(indir, fn), os.path.join(outdir, fn))
                f.write("echo '%s'\n"%cmd)
                f.write(cmd)
                f.write('echo Status="$?"\n\n')
        os.system("chmod +x %s" % os.path.join(jobdir, 'eoscopy_%d.sh'%ijob))

        with open(os.path.join(jobdir, 'submit_copy_%d.sh'%ijob), 'w') as jobscript:
            jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Executable              = {dir}/eoscopy_{num}.sh
Output                  = logs/eoscopy_{num}.out
Error                   = logs/eoscopy_{num}.err
Log                     = logs/eoscopy_{num}.log
use_x509userproxy       = true
initialdir              = {dir}
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format( num=ijob, dir=jobdir))
        
        script.write('./%s\n'%os.path.join(jobdir, 'submit_copy_%d.sh'%ijob))
        os.system("chmod +x %s" % os.path.join(jobdir, 'submit_copy_%d.sh'%ijob))
    
os.system("chmod +x submit_copy.sh")
print 'Done'
