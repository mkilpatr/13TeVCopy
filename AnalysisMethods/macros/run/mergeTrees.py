#!/usr/bin/env python
import argparse
import subprocess
import os

parser = argparse.ArgumentParser(description='Merges trees from indir to outdir according to config file. To use FNAL EOS locations, prepend LFNs as in root://cmseos.fnal.gov///store...')
parser.add_argument("-c", "--conf", dest="conf", default="run1lep.conf", help="Input configuration file.")
parser.add_argument("-i", "--indir", dest="indir", default="${PWD}/trees", help="Input directory.")
parser.add_argument("-o", "--outdir", dest="outdir", default="${PWD}/trees", help="Output directory.")
parser.add_argument("-k", "--kamikaze", dest="kamikaze", action="store_true", help="Kamikaze mode (hadd ignores corrupted files).") 
#parser.print_help()
args = parser.parse_args()

samples = []
files = []
xsecs = []
types = []
state = 0
snum = 0
with open((args.conf),"r") as f :
    for line in f :
        content = line.split()
        if "#" in content[0] :
            continue
        if content[0] == "$" :
            samples.append(content[1])

eosfnal = "root://cmseos.fnal.gov/"

for samp in samples:  
  if args.indir.startswith(eosfnal) :
    #fnal eos is now unmounted from lpc workers ... workaround over xrootd
    outname=samp + "_tree.root"
    inname =samp+"_[0-9]*_tree.root"
    print "*** Ignore any Unable to Stat error ..., output file will be created"
    if subprocess.Popen(["eos root://cmseos.fnal.gov ls /store/user/"+args.indir.split("/store/user/",1)[1]+"/"+outname], stdout=subprocess.PIPE,shell=True).communicate()[0] :
        # just move
        cmd = args.indir + "/" + outname + " " + args.outdir  + "/" + outname
        cmd = "xrdcp -sf " + cmd
    elif subprocess.Popen(["eos root://cmseos.fnal.gov ls /store/user/"+args.indir.split("/store/user/",1)[1]+"/"+samp+"_1_tree.root"], stdout=subprocess.PIPE, shell=True).communicate()[0] :
        # no regex with hadd over xrootd. workaround: form entire list of input files explicitely.
        lscmd = "eos root://cmseos.fnal.gov ls /store/user/"+args.indir.split("/store/user/",1)[1] + " | grep \"" + inname + "\" | sed -e \'s|^|"+args.indir + "/" + "|\'"
        infiles = subprocess.Popen([lscmd, ""], stdout=subprocess.PIPE, shell=True).communicate()[0]
        infiles = infiles.replace('\n',' ')
        cmd = args.outdir + "/" + outname + " " + infiles
        if args.kamikaze :          
          cmd = "hadd -k -f " + cmd
        else : cmd = "hadd -f " + cmd
    else:
	print "File of %s do not exist: Bypassing..."%samp
        continue
  else :
    outname=samp + "_tree.root"
    inname =samp+"_[0-9]*_tree.root"
    if os.path.isfile(args.indir + "/" + outname):
        # just move
        cmd="mv "+ args.indir + "/" + outname + " " + args.outdir  + "/" + outname
    elif os.path.isfile(args.indir + "/" + samp + "_1_tree.root"):       
        cmd = args.outdir + "/" + outname + " " + args.indir  + "/" + inname
        if args.kamikaze :
          cmd = "hadd -k -f " + cmd
        else : cmd = "hadd -f " + cmd
    else:
        print "File of %s do not exist: Bypassing..."%samp
        continue
#  print cmd
  subprocess.call(cmd, shell=True)
    
