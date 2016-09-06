#!/usr/bin/env python
import os
import sys
import re
import argparse
import subprocess
from ROOT import gROOT, TFile, TTree, TH2F, TChain
gROOT.SetBatch(True)

# script to help with submission of ntupling jobs, either interactively or using a batch system (LSF on lxplus CERN or condor on cmslpc at FNAL)
# use ./makejobs.py -h to explore the options and usage
# edit datasets.conf to choose samples to submit
# run ./makejobs.py with the appropriate options
# then, submit jobs using ./submitall.sh
# to run the merging after the ntupling is done, run ./makejobs.py --runmerge --inputdir </path/to/dir/with/unmerged/files> -o </path/to/dir/with/merged/files>
# if you want to split the merged output into multiple files (for large samples), add the --splitmerge option, you'll be asked how many merged files you want and how many input files you want to merge per output file
# a script called submitmerge.sh will be produced which will contain the commands needed to run the merging
# to run the postprocessing (adding cross section weights), run ./makejobs.py --postprocess -o </path/to/dir/with/merged/files> -c <conffile> -t <condor|lsf|interactive>
# to break up an SMS scan into the individual mass points, run ./makejobs.py --makegrid --inputdir </path/to/dir/with/postprocessed/files> -o </path/to/dir/with/final/files> -t <condor|lsf|interactive> --postsuffix postproc

parser = argparse.ArgumentParser(description='Prepare and submit ntupling jobs')
parser.add_argument("--makegrid", dest="makegrid", action='store_true', help="Make jobs for producing mass grid from SMS scans. [Default: False]")
parser.add_argument("--mstopmin", dest="mstopmin", type=int, default=100, help="Minimum m(stop) in GeV for making grid. [Default: 100]")
parser.add_argument("--mstopmax", dest="mstopmax", type=int, default=1250, help="Maximum m(stop) in GeV for making grid. [Default: 1250]")
parser.add_argument("--mlspmin", dest="mlspmin", type=int, default=0, help="Minimum m(lsp) in GeV for making grid. [Default: 0]")
parser.add_argument("--mlspmax", dest="mlspmax", type=int, default=800, help="Maximum m(lsp) in GeV for making grid. [Default: 800]")
parser.add_argument("--postprocess", dest="postprocess", action='store_true', help="Run postprocessing instead of job submission. [Default: False]")
parser.add_argument("--treename", dest="treename", default="TestAnalyzer/Events", help="Name of trees in merged input files, needed for postprocessing. [Default: TestAnalyzer/Events]")
parser.add_argument("--lumi", dest="lumi", type=float, default=1., help="Integrated luminosity to be used for calculating cross section weights in postprocessing. [Default: 1.]")
parser.add_argument("--postsuffix", dest="postsuffix", default="postproc", help="Suffix to add to output files from postprocessing. [Default: postproc]")
parser.add_argument("--runmerge", dest="runmerge", action='store_true', help="Run file merging instead of job submission. [Default: False]")
parser.add_argument("--splitmerge", dest="splitmerge", action='store_true', help="Split file merging into multiple output files. [Default: False]")
parser.add_argument("--inputdir", dest="inputdir", default="/eos/uscms/store/user/${USER}/13TeV/ntuples", help="Input directory with unmerged ntuples. [Default: \"/eos/uscms/store/user/${USER}/13TeV/ntuples\"]")
parser.add_argument("--checkfailed", dest="checkfailed", action='store_true', help="Check for failed jobs which result in corrupted files. Important for data. [Default: False]")
parser.add_argument("--resubmit", dest="resubmit", action='store_true', help="Resubmit jobs for which output files do not exist. [Default: False]")
parser.add_argument("-i", "--input", dest="input", default="datasets.conf", help="Input configuration file with list of datasets. [Default: datasets.conf]")
parser.add_argument("-s", "--submit", dest="submit", default="submitall", help="Name of shell script to run for job submission. [Default: submitall]")
parser.add_argument("-n", "--numperjob", dest="numperjob", type=int, default=5, help="Number of files or events per job. Splittype determines whether splitting is by number of files (default) or by number of events. [Default: 5]")
parser.add_argument("-m", "--maxevents", dest="maxevents", type=int, default=-1, help="Maximum number of events to run over. [Default: -1 (all)]")
parser.add_argument("-p", "--pathtocfg", dest="path", default="../test", help="Path to directory with python configuration file to be run using cmsRun. [Default: \"../test/\"]")
parser.add_argument("-c", "--config", dest="config", default="runTestAnalyzer.py", help="Configuration file to be run using cmsRun to run. [Default: runTestAnalyzer.py]")
parser.add_argument("-d", "--dbsinstance", dest="dbsinstance", default="prod/global", help="DBS instance to query for dataset. [Default: prod/global]")
parser.add_argument("-o", "--outdir", dest="outdir", default="/eos/uscms/store/user/${USER}/13TeV/ntuples", help="Output directory for ntuples. [Default: \"/eos/uscms/store/user/${USER}/13TeV/ntuples\"]")
parser.add_argument("-j", "--jobdir", dest="jobdir", default="jobs", help="Directory for job files. [Default: jobs]")
parser.add_argument("-r", "--runscript", dest="script", default="runjobs", help="Shell script to be run by the jobs, [Default: runjobs]")
parser.add_argument("-t", "--submittype", dest="submittype", default="condor", choices=["interactive", "lsf", "condor", "crab"], help="Method of job submission. [Options: interactive, lsf, condor, crab. Default: condor]")
parser.add_argument("-l", "--splittype", dest="splittype", default="file", choices=["file", "event"], help="Split jobs by number of files or events. [Options: file, event. Default: file]")
parser.add_argument("-q", "--queue", dest="queue", default="8nh", help="LSF submission queue. [Default: 8nh]")
parser.add_argument("-a", "--arrangement", dest="arrangement", default="das", choices=["das", "local"], help="(ntuplizing only) Specifies if samples' paths are das, or local eos space (format: /eos/uscms/store/... or /eos/cms/store/...). If local, then file-based splitting required, and sample name will be used to discover files (eg \'find /eos/uscms/store/...\') [Options: das, local. Default: das]")
parser.add_argument("-e", "--redir", dest="redir", default="root://cmsxrootd.fnal.gov/", help="Url of redirector to be added to file names. [Default: root://cmsxrootd.fnal.gov/]")
parser.add_argument("--storagesite", dest="site", default="T3_US_FNALLPC", help="Crab storage site. [Default: T3_US_FNALLPC]")
parser.add_argument("--crabcmd", dest="crabcommand", default=None, help="Crab command. [Default: None]")
parser.add_argument("--fromcrab", dest="fromcrab", action='store_true', help="The files to be merged are produced by CRAB. [Default: False]")
args = parser.parse_args()

def get_num_events(filelist, prefix='', wgtsign=1, treename='TestAnalyzer/Events', selection=''):
    totentries = 0
    for filename in filelist :
        filepath = '/'.join(['%s' % prefix, '%s' % filename])
        file = TFile.Open(filepath)
        tree = file.Get(treename)
        if wgtsign > 0 :
            totentries += tree.GetEntries('genweight>0' + selection)
        else :
            totentries += tree.GetEntries('genweight<0' + selection)
    return totentries

def get_all_masspoints(filelist, mstopmin, mstopmax, mlspmin, mlspmax, sigbase, sigsuffix='', treename='Events') :
    mstopbins = int(mstopmax - mstopmin)
    mlspbins = int(mlspmax - mlspmin)
    massgrid = TH2F('massgrid', '', mstopbins, mstopmin, mstopmax, mlspbins, mlspmin, mlspmax)
    chain = TChain(treename)
    for filename in filelist :
        chain.Add(filename)

    chain.SetBranchStatus('*',0)
    chain.SetBranchStatus('massparams',1)

    chain.Draw('massparams[1]:massparams[0]>>massgrid')

    snames = []
    masspoints = []

    for ibinx in range(mstopbins+1) :
        for ibiny in range(mlspbins+1) :
            if massgrid.GetBinContent(ibinx,ibiny) > 0 :
                mstop = int(massgrid.GetXaxis().GetBinLowEdge(ibinx))
                mlsp = int(massgrid.GetYaxis().GetBinLowEdge(ibiny))
                snames.append('_'.join([sigbase, str(mstop), str(mlsp)]))
                masspoints.append(('_').join([sigbase, str(mstop), str(mlsp)])) if mlsp != 0 else masspoints.append(('_').join([sigbase, str(mstop), '1']))
                if len(sigsuffix) :
                    snames[-1] += '_' + sigsuffix
                    masspoints[-1] += '_' + sigsuffix

    return (snames,masspoints)

processes = []
samples = []
xsecs = []
datasets = []
totnposevents = []
totnnegevents = []
xsecfiles = []
filterfiles = []
wgtscalefactors = []

def parseConfig(removeExtension=False, removeGenJets5=False):
    with open(args.input, "r") as f :
        for line in f :
            content = line.split()
            if "#" in content[0] :
                continue
            if removeExtension and re.search(r'-ext[0-9]*$', content[1]) and re.sub(r'-ext[0-9]*$', '', content[1]) in samples:
                print content[1], 'is an extension sample, will be treated together with the non-extension sample...'
                continue
            if removeGenJets5 and re.search(r'-genjets5$', content[1]) and re.sub(r'-genjets5$', '', content[1]) in samples:
                print content[1], 'is a genjets5 filtered sample, will be treated together with the inclusive sample...'
                continue
            processes.append(content[0])
            samples.append(content[1])
            xsecs.append(content[2])
            if len(content) > 3 :
                datasets.append(content[3])
            else:
                datasets.append(None)
            if len(content) > 4 :
                xsecfiles.append(content[4])
            else :
                xsecfiles.append('None')
            if len(content) > 5 :
                filterfiles.append(content[5])
            else:
                filterfiles.append('None')

eos = "/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select"

os.system("mkdir -p %s" % args.jobdir)

if args.makegrid :
    parseConfig(True)

    if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
        os.system("%s mkdir -p %s" % (eos, args.outdir))
    else :
        os.system("mkdir -p %s" % args.outdir)

    masspoints = {}
    files = {}
    prefix = ""
    snames = {}
    for sample in samples :
        filelist = []
        snames[sample] = []
        masspoints[sample] = []
        if args.inputdir.startswith("/eos/cms/store/user") or args.inputdir.startswith("/store/user") :
            cmd = ("%s find -f %s | egrep '%s(-ext[0-9]*|)(_[0-9]+|)_ntuple_%s.root'" % (eos, args.outdir, sample, args.postsuffix))
            ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            result = ps.communicate()
            filelist = result[0].rstrip('\n').split('\n')
            prefix = "root://eoscms/"
        else :
            filelist = [os.path.join(args.inputdir, f) for f in os.listdir(args.inputdir) if re.match(r'%s(-ext[0-9]*|)(_[0-9]+|)_ntuple_%s.root' % (sample, args.postsuffix), f)]
            if args.inputdir.startswith("/eos/uscms/store/user") :
                prefix = "root://cmseos:1094/"
        mstopmin = sample.split('_')[1][:3] if len(sample.split('_')) > 1 and len(sample.split('_')[1]) > 2 and sample.split('_')[1][:3].isdigit() else args.mstopmin
        mstopmax = args.mstopmax
        if len(sample.split('_')) > 1 :
            mstopmax = sample.split('_')[1][-4:] if len(sample.split('_')[1]) > 3 and sample.split('_')[1][-4].isdigit() else sample.split('_')[1][-3:]
        sigbase = sample.split('_')[0]
        sigsuffix = sample.split('_')[3] if len(sample.split('_')) > 3 else ''
        infiles = [prefix + f for f in filelist]
        files[sample] = infiles
        mstopmin = int(mstopmin)
        mstopmax = int(mstopmax)+1
        print mstopmin, mstopmax
        (snames[sample],masspoints[sample]) = get_all_masspoints(infiles, mstopmin, mstopmax, args.mlspmin, args.mlspmax, sigbase, sigsuffix)

    print 'Creating submission file: submit_makegrid.sh'
    script = open('submit_makegrid.sh', 'w')
    script.write("""#!/bin/bash
outputdir={outputdir}
runmacro=GetMassPointFromScan.C
prefix={prefix}
suffix={suffix}
""".format(outputdir=args.outdir, prefix=prefix, suffix=args.postsuffix))

    if args.submittype == 'lsf' or args.submittype == 'condor' :
        script.write("""
workdir=$CMSSW_BASE
runscript=runmakegrid{stype}.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp GetMassPointFromScan.C $workdir
cp rootlogon.C $workdir

echo "$runscript $runmacro $workdir $outputdir"
""".format(stype=args.submittype))

    for sample in samples :
        for masspoint in snames[sample] :
            submitfile = '%s/filelist_%s.txt' % (args.jobdir, masspoint)
            with open(submitfile, 'w') as f :
                f.write('\n'.join(files[sample]))
            outfilename = '_'.join([masspoint, 'ntuple', args.postsuffix + '.root'])
            if int(masspoint.split('_')[2]) == 0 :
                outfilename = outfilename.replace('_0_', '_1_')
            if args.submittype == 'interactive' :
                script.write("""root -l -q -b $runmacro+\(\\"{infile}\\",\\"{outfile}\\",{mstop},{mlsp}\)\nmv {outfile} {outdir}/\n""".format(
                infile=submitfile, outfile=outfilename, mstop=int(masspoint.split('_')[1]), mlsp=int(masspoint.split('_')[2]), outdir=args.outdir
                ))
            elif args.submittype == 'lsf' :
                script.write("""cp {jobdir}/{infile} $workdir\nbsub -q {queue} $runscript $runmacro {infile} {outfile} {mstop} {mlsp} {outdir} $workdir\n""".format(
                jobdir=args.jobdir, queue=args.queue, infile=submitfile.split('/')[1], outfile=outfilename, mstop=int(masspoint.split('_')[-1]), mlsp=int(masspoint.split('_')[2]), outdir=args.outdir
                ))
            elif args.submittype == 'condor' :
                os.system("mkdir -p %s/logs" % args.jobdir)
                jobscript = open('%s/submit_%s_makegrid.sh' % (args.jobdir, masspoint), 'w')
                jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
Executable              = runmakegridcondor.sh
Arguments               = {macro} {infile} {outfile} {mstop} {mlsp} {outdir} {workdir}
Output                  = logs/{sname}_makegrid.out
Error                   = logs/{sname}_makegrid.err
Log                     = logs/{sname}_makegrid.log
use_x509userproxy       = true
initialdir              = {jobdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{macro},{workdir}/{infile},{workdir}/rootlogon.C
WhenToTransferOutput    = ON_EXIT
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format(
                macro="GetMassPointFromScan.C", infile=submitfile.split('/')[-1], outfile=outfilename, mstop=int(masspoint.split('_')[1]), mlsp=int(masspoint.split('_')[2]), outdir=args.outdir, workdir="${CMSSW_BASE}", jobdir=args.jobdir, sname=masspoint
                ))
                jobscript.close()
                script.write("cp {jobdir}/{infile} $workdir\n./{jobdir}/submit_{sname}_makegrid.sh\n".format(jobdir=args.jobdir, infile=submitfile.split('/')[-1], sname=masspoint))
                os.system("chmod +x %s/submit_%s_makegrid.sh" % (args.jobdir, masspoint))


    script.close()
    os.system("chmod +x submit_makegrid.sh")

    print "Done!"

    for sname in masspoints:
        print sname
        print ', '.join(masspoints[sname])

    exit()

elif args.postprocess :
    parseConfig(True, True)

    if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
        os.system("%s mkdir -p %s" % (eos, args.outdir))
    else :
        os.system("mkdir -p %s" % args.outdir)

    files = []
    prefix = ""
    for isam in range(len(samples)) :
        filelist = []
        extrafilelist = []
        if args.inputdir.startswith("/eos/cms/store/user") or args.inputdir.startswith("/store/user") :
            cmd = ("%s find -f %s | egrep '%s(-ext[0-9]*|)(_[0-9]+|)_ntuple.root'" % (eos, args.outdir, samples[isam]))
            ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            result = ps.communicate()
            filelist = result[0].rstrip('\n').split('\n')
            prefix = "root://eoscms/"
            if 'qcd' in samples[isam]:
                cmd = ("%s find -f %s | egrep '%s-genjets5(-ext[0-9]*|)(_[0-9]+|)_ntuple.root'" % (eos, args.outdir, samples[isam]))
                ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                result = ps.communicate()
                extrafilelist = result[0].rstrip('\n').split('\n')
        else :
            filelist = [os.path.join(args.inputdir, f) for f in os.listdir(args.inputdir) if re.match(r'%s(-ext[0-9]*|)(_[0-9]+|)_ntuple.root' % samples[isam], f)]
            if args.inputdir.startswith("/eos/uscms/store/user") :
                prefix = "root://cmseos:1094/"
            if 'qcd' in samples[isam]:
                extrafilelist = [os.path.join(args.inputdir, f) for f in os.listdir(args.inputdir) if re.match(r'%s-genjets5(-ext[0-9]*|)(_[0-9]+|)_ntuple.root' % samples[isam], f)]
        files.append(filelist + extrafilelist)
        if re.search('-[0-9]{4}[a-zA-Z]-', samples[isam]):
            print samples[isam] + ' is data! No need to get total number of events.'
            nposevents = 1
            nnegevents = 0
        else:
            nposevents = get_num_events(filelist, prefix)
            nnegevents = get_num_events(filelist, prefix, -1)
            print "Sample " + samples[isam] + " has " + str(nposevents) + " positive and " + str(nnegevents) + " negative weight events"
        totnposevents.append(nposevents)
        totnnegevents.append(nnegevents)
        if 'qcd' in samples[isam]:
            ngenjets5events = get_num_events(filelist, prefix, selection=' && nstdgenjets>=5')
            nextragenjets5events = get_num_events(extrafilelist, prefix, selection=' && nstdgenjets>=5')
            wgtsf = float(ngenjets5events) / (ngenjets5events + nextragenjets5events)
            wgtscalefactors.append(wgtsf)
            print "Sample " + samples[isam] + " has " + str(nposevents) + " events, " + str(ngenjets5events) + " of which have ngenjets>=5; Extra genjets5 sample has " + str(nextragenjets5events) + " events; wgtSF for ngenjets>=5 = " + str(wgtsf)
        else:
            wgtscalefactors.append(1)

    print "Creating submission file: submit_addweight.sh"
    script = open("submit_addweight.sh", "w")
    script.write("""#!/bin/bash
outputdir={outdir}
runmacro=AddWgt2UCSBntuples.C
treename={tname}
prefix={prefixs}
suffix={suffixs}
""".format(outdir=args.outdir, tname=args.treename, prefixs=prefix, suffixs=args.postsuffix))

    if args.submittype == "lsf" or args.submittype == "condor" :
        script.write("""
workdir=$CMSSW_BASE
runscript=runaddweight{stype}.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp AddWgt2UCSBntuples.C $workdir
cp rootlogon.C $workdir

echo "$runscript $runmacro $workdir $outputdir"
""".format(stype=args.submittype))

    for isam in range(len(samples)) :
        for ifile in range(len(files[isam])) :
            filename = files[isam][ifile].split('/')[-1]
            outfilename = filename.replace(".root", "_{}.root".format(args.postsuffix))
            if args.submittype == "interactive" :
                script.write("""root -l -q -b $runmacro+\(\\"$prefix{fname}\\",\\"{process}\\",{xsec},{lumi},{nposevts},{nnegevts},\\"$treename\\",\\"$suffix\\",\\"{xsecfile}\\",\\"{filterfile}\\",{wgtsf}\)\n""".format(
                fname=files[isam][ifile], process=processes[isam], xsec=xsecs[isam], lumi=args.lumi, nposevts=totnposevents[isam], nnegevts=totnnegevents[isam], xsecfile=xsecfiles[isam], filterfile=filterfiles[isam], wgtsf=wgtscalefactors[isam]
                ))
            elif args.submittype == "lsf" :
                script.write("""bsub -q {queue} $runscript $runmacro $prefix{fname} {process} {xsec} {lumi} {nposevts} {nnegevts} $treename $suffix {xsecfile} {filterfile} {wgtsf} {outname} {outdir} $workdir\n""".format(
                queue=args.queue, fname=files[isam][ifile], process=processes[isam], xsec=xsecs[isam], lumi=args.lumi, nposevts=totnposevents[isam], nnegevts=totnegevents[isam], xsecfile=xsecfiles[isam], filterfile=filterfiles[isam], wgtsf=wgtscalefactors[isam], outname=outfilename, outdir=args.outdir
                ))
            elif args.submittype == "condor" :
                os.system("mkdir -p %s/logs" % args.jobdir)
                jobscript = open("{}/submit_{}_{}_addwgt.sh".format(args.jobdir, samples[isam], ifile), "w")
                jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
Executable              = runaddweight{stype}.sh
Arguments               = {macro} {prefixs}{fname} {process} {xsec} {lumi} {nposevts} {nnegevts} {tname} {suffix} {xsecfile} {filterfile} {wgtsf} {outname} {outdir} {workdir}
Output                  = logs/{sname}_{num}_addweight.out
Error                   = logs/{sname}_{num}_addweight.err
Log                     = logs/{sname}_{num}_addweight.log
use_x509userproxy       = true
initialdir              = {jobdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{macro},{workdir}/rootlogon.C
WhenToTransferOutput    = ON_EXIT
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format(
                stype=args.submittype, macro="AddWgt2UCSBntuples.C", prefixs=prefix, workdir="${CMSSW_BASE}", fname=files[isam][ifile], process=processes[isam], xsec=xsecs[isam], lumi=args.lumi, nposevts=totnposevents[isam], nnegevts=totnnegevents[isam], tname=args.treename, suffix=args.postsuffix, xsecfile=xsecfiles[isam], filterfile=filterfiles[isam], wgtsf=wgtscalefactors[isam], sname=samples[isam], num=ifile, jobdir=args.jobdir, outname=outfilename, outdir=args.outdir
                ))
                jobscript.close()
                script.write("./{jobdir}/submit_{name}_{j}_addwgt.sh\n".format(jobdir=args.jobdir, name=samples[isam], j=ifile))
                os.system("chmod +x %s/submit_%s_%d_addwgt.sh" % (args.jobdir, samples[isam], ifile))


    script.close()
    os.system("chmod +x submit_addweight.sh")

    print "Done!"
    exit()

elif args.runmerge :
    parseConfig(False)

    if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
        os.system("%s mkdir -p %s" % (eos, args.outdir))
    else :
        os.system("mkdir -p %s" % args.outdir)

    prefix = ""
    mergefilelist = []
    for isam in range(len(samples)) :
        filelist = []
        if args.inputdir.startswith("/eos/cms/store/user") or args.inputdir.startswith("/store/user") :
            prefix = "root://eoscms/"
            if args.fromcrab:
                cmd = ("%s find -f %s | egrep 'evttree_[0-9]+(_numEvent[0-9]+|).root' | grep -v 'failed'" % (eos, os.path.join(args.inputdir, datasets[isam].split("/")[1], 'crab_' + samples[isam])))
            else:
                cmd = ("%s find -f %s | egrep 'output_[0-9]+_%s(-ext[0-9]*|)(_numEvent[0-9]+|).root'" % (eos, args.inputdir, samples[isam]))
            ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            result = ps.communicate()
            filelist = result[0].rstrip('\n').split('\n')
            filelist = ["".join([prefix, file]) for file in filelist]
            outname = "%s%s/%s_ntuple.root" % (prefix, args.outdir, samples[isam])
            filelist.insert(0, outname)
        else :
            if args.fromcrab:
                input_dir = os.path.join(args.inputdir, datasets[isam].split("/")[1], 'crab_' + samples[isam])
                filelist = [os.path.join(dp, f) for dp, dn, filenames in os.walk(input_dir) if 'failed' not in dp for f in filenames if re.match(r'evttree_[0-9]+(_numEvent[0-9]+|).root', f)]
            else:
                filelist = [os.path.join(args.inputdir, f) for f in os.listdir(args.inputdir) if re.match(r'output_[0-9]+_%s(-ext[0-9]*|)(_numEvent[0-9]+|).root' % samples[isam], f)]
            if args.inputdir.startswith("/eos/uscms/store/user") :
                prefix = "root://cmseos:1094/"
            filelist = ["".join([prefix, file]) for file in filelist]
            outname = "%s%s/%s_ntuple.root" % (prefix, args.outdir, samples[isam])
            filelist.insert(0, outname)
        mergefile = os.path.join(args.jobdir, "merge_%s.txt" % samples[isam])
        with open(mergefile, "w") as f:
            f.write('\n'.join(filelist))
        print "There are %d files to merge for %s\n" % (len(filelist) - 1, samples[isam])
        if args.splitmerge :
            numfilespermerge = int(input("How many input files per merge? "))
            nummergedfiles = 0 if numfilespermerge==0 else max(int((len(filelist)-1) / numfilespermerge), 1)
            if ((len(filelist)-1) % numfilespermerge) > numfilespermerge/2: nummergedfiles = nummergedfiles+1
            print "Will produce %d merged files." % nummergedfiles
            for imerge in range(1, nummergedfiles + 1) :
                start = ((imerge - 1) * numfilespermerge) + 2
                end = (imerge * numfilespermerge) + 1
                outfile = os.path.join(args.jobdir, "merge_%s_%d.txt" % (samples[isam], imerge))
                os.system("sed -n 1p %s | sed \"s/ntuple.root/%d_ntuple.root/\" > %s" % (mergefile, imerge, outfile))
                os.system("sed -n %d,%dp %s >> %s" % (start, end, mergefile, outfile))
                mergefilelist.append(os.path.basename(outfile))
            rem = len(filelist) - 1 - (nummergedfiles * numfilespermerge)
            if rem > 0 :
                print "Will add last %d files to last merged file" % rem
                os.system("tail -n %d %s >> %s/merge_%s_%d.txt" % (rem, mergefile, args.jobdir, samples[isam], nummergedfiles))
        else :
            mergefilelist.append(os.path.basename(mergefile))

    print 'Creating submission file: submit_runmerge.sh'
    script = open('submit_runmerge.sh', 'w')
    script.write("""#!/bin/bash
outputdir={outputdir}
runmacro=MergeNtuples.C
prefix={prefix}
""".format(outputdir=args.outdir, prefix=prefix))

    if args.submittype == 'lsf' or args.submittype == 'condor' :
        script.write("""
workdir=$CMSSW_BASE
runscript=runmerge{stype}.sh

if [ ! "$CMSSW_BASE" ]; then
  echo "-------> error: define cms environment."
  exit 1
fi

cp MergeNtuples.C $workdir
cp rootlogon.C $workdir

echo "$runscript $runmacro $workdir $outputdir"
""".format(stype=args.submittype))

    for submitfile in mergefilelist :
        outfilename = submitfile[6:].replace('.txt', '_ntuple.root')
        if args.submittype == 'interactive' :
            script.write("""root -l -q -b MergeNtuples.C+\\(\\\"{infile}\\\",0\\)\n""".format(
            infile = os.path.join(args.jobdir, submitfile)
            ))
        elif args.submittype == 'lsf' :
            script.write("""cp {jobdir}/{infile} $workdir\nbsub -q {queue} $runscript $runmacro {infile} {outfile} {outdir} $workdir\n""".format(
            queue=args.queue, jobdir=args.jobdir, infile=submitfile, outfile=outfilename, outdir=args.outdir
            ))
        elif args.submittype == 'condor' :
            os.system("mkdir -p %s/logs" % args.jobdir)
            jobname = outfilename.replace('_ntuple.root', '')
            jobscript = open('%s/submit_%s_runmerge.sh' % (args.jobdir, jobname), 'w')
            jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
Executable              = runmergecondor.sh
Arguments               = {macro} {infile} {outfile} {outdir} {workdir}
Output                  = logs/{sname}_runmerge.out
Error                   = logs/{sname}_runmerge.err
Log                     = logs/{sname}_runmerge.log
use_x509userproxy       = true
initialdir              = {jobdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{macro},{workdir}/{infile},{workdir}/rootlogon.C
WhenToTransferOutput    = ON_EXIT
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format(
            macro="MergeNtuples.C", infile=submitfile, outfile=outfilename, outdir=args.outdir, workdir="${CMSSW_BASE}", jobdir=args.jobdir, sname=jobname
            ))
            jobscript.close()
            script.write("cp {jobdir}/{infile} $workdir\n./{jobdir}/submit_{sname}_runmerge.sh\n".format(jobdir=args.jobdir, infile=submitfile, sname=jobname))
            os.system("chmod +x %s/submit_%s_runmerge.sh" % (args.jobdir, jobname))

    script.close()
    os.system("chmod +x submit_runmerge.sh")

    print "Done!"
    exit()

elif args.submittype == 'crab':
    parseConfig(False)

    if args.crabcommand:
        print "Creating submission file: ", args.submit + ".sh"
        with open(args.submit + ".sh", "w") as script:
            script.write("#!/bin/bash\n\n")
            for isamp in range(len(samples)):
                samp = samples[isamp]
                script.write("crab %s -d crab_projects/crab_%s\n\n" % (args.crabcommand, samp))
        os.system("chmod +x %s.sh" % args.submit)
        print "Done!"
        exit()

    print "Creating crab config files: "
    crab_configs = []
    for isamp in range(len(samples)):
        samp = samples[isamp]
        dataset = datasets[isamp]
        if not dataset: continue
        isData = re.match(r'^/[a-zA-Z]+/Run[0-9]{4}[A-Z]', dataset)
#         crab_template_file = 'template_runCrabOnData.py' if isData else 'template_runCrabOnMC.py'
        crab_template_file = 'template_runCrab.py'
        with open(crab_template_file, 'r') as crfile:
            crabconfig = crfile.read()
        replace_dict = {'_requestName_':samp,
                        '_psetName_':os.path.join(args.path, args.config),
                        '_inputDataset_':dataset,
                        '_unitsPerJob_':str(args.numperjob),
                        '_outLFNDirBase_':re.sub(r'/eos/[a-z]+/store', '/store', args.outdir),
                        '_storageSite_':args.site}
        for key in replace_dict:
            crabconfig = crabconfig.replace(key, replace_dict[key])
        cfgfilename = 'crab_submit_%s.py' % samp
        crab_configs.append(cfgfilename)
        with open(cfgfilename, 'w') as cfgfile:
            cfgfile.write(crabconfig)
        print cfgfilename

    print "Creating submission file: ", args.submit + ".sh"
    with open(args.submit + ".sh", "w") as script:
        script.write("#!/bin/bash\n\n")
        for cfg in crab_configs:
            script.write("crab submit -c %s\n\n" % cfg)

    os.system("chmod +x %s.sh" % args.submit)
    print "Done!"
    exit()

elif args.checkfailed :
    parseConfig(False)
    for isam in range(len(samples)) :
        cmd = ""
        if args.splittype == "file" :
            cmd = "for i in `grep -cH ERROR %s/logs/%s*.err | grep -v :0 | sed 's/.err:.*/.out/'` ; do for j in `grep -cH \"Status = 0\" $i | grep -v :1 | sed 's/:0//'` ; do file=`cat $j | grep \"args:\" | awk '{print $4\"/\"$5}'` ; logfile=`echo $i | sed 's/out/err/'` ; echo \"log file: \"$logfile\", output file:\"; ls -lrth $file ; done ; done" % (args.jobdir,samples[isam])
        else :
            cmd = "for i in `grep -cH ERROR %s/logs/%s*.err | grep -v :0 | sed 's/.err:.*/.out/'` ; do for j in `grep -cH \"Status = 0\" $i | grep -v :1 | sed 's/:0//'` ; do file=`cat $j | grep \"args:\" | awk '{print $4\"/\"$5}' | sed 's/.root/_numEvent%d.root/'` ; logfile=`echo $i | sed 's/out/err/'` ; echo \"log file: \"$logfile\", output file:\"; ls -lrth $file ; done ; done" % (args.jobdir,samples[isam],args.numperjob)
        ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        output= ps.communicate()[0].replace(':\n',': ')
        result = output.split('\n')
        if len(result) > 1 :
            print '\n'.join(result)
            files = [line.split()[-1] for line in result if len(line) > 1]
            removefiles = raw_input("Do you want to delete these output files? [y/n] ").lower()
            yes = ['yes','y']
            no = ['no','n']
            if removefiles in yes :
                print 'Will remove files!'
                rmcmd = 'rm ' + ' '.join(files)
                os.system(rmcmd)
            elif not removefiles in no :
                print 'Didn\'t get a y/n answer! Doing nothing ...'

else :
    parseConfig(False)

    if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
        os.system("%s mkdir -p %s" % (eos, args.outdir))
    else :
        os.system("mkdir -p %s" % args.outdir)

    maxevts = args.maxevents if args.splittype == "file" else args.numperjob

    print "Creating submission file: ", args.submit + ".sh"
    script = open(args.submit + ".sh", "w")
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
""".format(
        pathtocfg=args.path, runscript=args.script, stype=args.submittype
        ))
    for isam in range(len(samples)) :
        if not datasets[isam]: continue
        if not args.resubmit :
            if args.arrangement == "das" :
                cmd = ("das_client --query \"file dataset=%s instance=%s\" --limit=0 | grep store | sed 's;/store;%s/store;' > %s/filelist_%s.txt" % (datasets[isam], args.dbsinstance, args.redir, args.jobdir, samples[isam]))
                subprocess.call(cmd, shell=True)
            elif args.arrangement == "local" and args.splittype == "file" :
                cmd = ("find %s | grep .root | sort -V > %s/filelist_%s.txt" % (datasets[isam], args.jobdir, samples[isam]))
                subprocess.call(cmd, shell=True)
            else :
                print "Fatal error: File-based splitting (splittype == \"file\") required if local."
        elif args.resubmit and not os.path.isfile("%s/filelist_%s.txt" % (args.jobdir, samples[isam])) :
            print "Error: Trying to resubmit jobs without filelist!"
        if args.arrangement == "das" :
            cmd = ("das_client --query \"dataset=%s instance=%s | grep dataset.nevents\" | sed -n 4p | tr -d '\n'" % (datasets[isam], args.dbsinstance))
            ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            numevents = int(ps.communicate()[0])
            if args.maxevents > -1  and args.maxevents < numevents :
                numevents = args.maxevents
        numperjob = args.numperjob
        numlines = 0
        infilename = "%s/filelist_%s.txt" % (args.jobdir, samples[isam])
        print "Creating jobs for %s" % samples[isam]
        with open(infilename, "r") as infile :
            numlines = len(infile.readlines())
        if args.splittype == "file" :
            numjobs = int(numlines) / int(numperjob)
            rem = numlines % numperjob
        elif args.splittype == "event" :
            numjobs = int(numevents) / int(numperjob)
            rem = numevents % numperjob
        if rem == 0 :
            numjobs -= 1
        if args.splittype == "file" :
            print "\t %d files to process --> will produce %d jobs with %d files per job" % (numlines, (numjobs + 1), numperjob)
        elif args.splittype == "event" :
            print "\t %d events to process --> will produce %d jobs with %d events per job" % (numevents, (numjobs + 1), numperjob)
        for ijob in range(numjobs + 1) :
            start = (ijob * numperjob) + 1
            end = numperjob * (ijob + 1)
            jobfile = "filelist_%s.txt" % (samples[isam])
            skipevts = 0
            if args.splittype == "file" and args.arrangement == "local" :
                jobfile = "job_%d_%s.txt" % (ijob, samples[isam])
                os.system("sed -n %d,%dp %s | awk \'{print \"root://cmseos:1094/\" $0}\' > %s/%s" % (start, end, infilename, args.jobdir, jobfile))
            elif args.splittype == "file" :
                jobfile = "job_%d_%s.txt" % (ijob, samples[isam])
                os.system("sed -n %d,%dp %s > %s/%s" % (start, end, infilename, args.jobdir, jobfile))
            elif args.splittype == "event" :
                skipevts = start - 1
            outfile = "output_%d_%s.root" % (ijob, samples[isam])
            suffix = "_numEvent{}".format(maxevts) if maxevts > -1 else ""
            cmd = ""
            if args.outdir.startswith("/eos/cms/store/user") or args.outdir.startswith("/store/user") :
                cmd = ("%s ls %s | grep -c output_%d_%s%s.root" % (eos, args.outdir, ijob, samples[isam], suffix))
            else :
                cmd = "ls %s | grep -c output_%d_%s%s.root" % (args.outdir, ijob, samples[isam], suffix)
            ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            output = ps.communicate()[0][0]
            if int(output) > 0 :
                print "Output file output_%d_%s%s.root exists already! Skipping ..." % (ijob, samples[isam], suffix)
                continue

            if args.submittype == "interactive" :
                os.system("mkdir -p %s" % args.outdir)
                script.write("""cmsRun {pathtocfg}/$cfgfile print inputFiles_clear inputFiles_load=$jobdir/{infile} outputFile=$outputdir/{outputname} maxEvents={maxevents} skipEvents={skipevents}\n""".format(
                pathtocfg=args.path, infile=jobfile, outputname=outfile, maxevents=maxevts, skipevents=skipevts
                ))

            elif args.submittype == "lsf" :
                cpinput = ""
                if args.splittype == "file" or (args.splittype == "event" and ijob == 0) :
                    cpinput = "\ncp $jobdir/%s $workdir \n" % (jobfile)
                script.write("""{cptxt}
    bsub -q {queue} $runscript $cfgfile {infile} $outputdir {outputname} {maxevents} {skipevents} $workdir \n""".format(
                cptxt=cpinput, queue=args.queue, infile=jobfile, outputname=outfile, maxevents=maxevts, skipevents=skipevts
                ))

            elif args.submittype == "condor" :
                os.system("mkdir -p %s/logs" % args.jobdir)
                jobscript = open("{0}/submit_{1}_{2}.sh".format(args.jobdir, samples[isam], ijob), "w")
                jobscript.write("""
cat > submit.cmd <<EOF
universe                = vanilla
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
Executable              = {runscript}{stype}.sh
Arguments               = {cfg} {infile} {outputdir} {outputname} {maxevents} {skipevents} {workdir}
Output                  = logs/{sname}_{num}.out
Error                   = logs/{sname}_{num}.err
Log                     = logs/{sname}_{num}.log
use_x509userproxy       = true
initialdir              = {jobdir}
Should_Transfer_Files   = YES
transfer_input_files    = {workdir}/{cfg},{workdir}/{infile}
WhenToTransferOutput    = ON_EXIT
Queue
EOF

condor_submit submit.cmd;
rm submit.cmd""".format(
                runscript=args.script, stype=args.submittype, pathtocfg=args.path, cfg=args.config, infile=jobfile, workdir="${CMSSW_BASE}", sname=samples[isam], num=ijob, jobdir=args.jobdir, outputdir=args.outdir, outputname=outfile, maxevents=maxevts, skipevents=skipevts, evttag=suffix
                ))
                jobscript.close()
                cpinput = ""
                if args.splittype == "file" or (args.splittype == "event" and ijob == 0) :
                    cpinput = "\ncp $jobdir/%s $workdir \n" % (jobfile)
                script.write("{cptxt}./$jobdir/submit_{name}_{j}.sh\n".format(cptxt=cpinput, name=samples[isam], j=ijob))
                os.system("chmod +x %s/submit_%s_%d.sh" % (args.jobdir, samples[isam], ijob))


    script.close()
    os.system("chmod +x %s.sh" % args.submit)

    print "Done!"
