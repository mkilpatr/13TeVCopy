#!/usr/bin/env python
import argparse
import subprocess
import os

parser = argparse.ArgumentParser(description='Process config file')
parser.add_argument("-c", "--conf", dest="conf", default="run1lep.conf", help="Input configuration file.")
parser.add_argument("-i", "--indir", dest="indir", default="${PWD}/trees", help="Input directory.")
parser.add_argument("-o", "--outdir", dest="outdir", default="${PWD}/trees", help="Output directory.")
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

for samp in samples:
    outname=samp + "_tree.root"
    inname =samp+"_[0-9]*_tree.root"
    if os.path.isfile(args.indir + "/" + outname):
        # just move
        cmd="mv "+ args.indir + "/" + outname + " " + args.outdir  + "/" + outname
    elif os.path.isfile(args.indir + "/" + samp + "_1_tree.root"):
        cmd="hadd -f " + args.outdir + "/" + outname + " " + args.indir  + "/" + inname
    else:
        print "File of %s do not exist: Bypassing..."%samp
        continue
    print cmd
    subprocess.call(cmd, shell=True)
    
