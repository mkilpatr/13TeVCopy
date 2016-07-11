#!/usr/bin/env python
import argparse
import subprocess
import os
import re
from collections import OrderedDict

def natural_sort(list, key=lambda s:s):
    """
    Sort the list into natural alphanumeric order.
    https://stackoverflow.com/questions/4836710/does-python-have-a-built-in-function-for-string-natural-sort
    """
    def get_alphanum_key_func(key):
        convert = lambda text: int(text) if text.isdigit() else text
        return lambda s: [convert(c) for c in re.split('([0-9]+)', key(s))]
    sort_key = get_alphanum_key_func(key)
    list.sort(key=sort_key)

parser = argparse.ArgumentParser(description='Create ntuple file list.')
parser.add_argument('path', metavar='PATH', help='Location of ntuples')
parser.add_argument("-c", "--conf", dest="conf", default="ntuples.conf", help="Output configuration file.")
parser.add_argument("-s", "--suffix", dest="suffix", default="postproc", help="Suffix of the ntuple file.")
parser.add_argument("--split", dest="split", action='store_true', default=False, help="Split different ht bins.")
args = parser.parse_args()

data_samples = OrderedDict()
mc_samples = OrderedDict()
signal_samples = OrderedDict()

eos = "/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select"
if args.path.startswith("/eos/cms/store/user") or args.path.startswith("/store/user") :
    cmd = ("%s find -f %s | egrep '%s.root'" % (eos, args.path, args.suffix))
    ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    result = ps.communicate()
    filelist = result[0].rstrip('\n').replace('/eos/cms','').split('\n')
else :
    filelist = [os.path.realpath(os.path.join(dp, f)).replace('/eos/uscms', '') for dp, dn, filenames in os.walk(args.path) for f in filenames if ('%s.root'%args.suffix) in f]

natural_sort(filelist)

for filepath in filelist:
    file = os.path.basename(filepath)
    if re.search(r'-[0-9]{4}[a-z]-', file):
        samples = data_samples
    elif re.search(r'T2[a-zA-Z]+_[0-9]+_[0-9]+', file):
        samples = signal_samples
    else:
        samples = mc_samples
    if args.split:
        sname = re.sub(r'(-ext[0-9]*|)(-genjets5|)(_[0-9]+|)_ntuple_%s.root'%args.suffix, '', file)
    else:
        sname = re.sub(r'(_ht[0-9]+to(inf|[0-9]+)|)(-ext[0-9]*|)(-genjets5|)(_[0-9]+|)_ntuple_%s.root'%args.suffix, '', file)
        sname = re.sub(r'-[0-9]{4}[a-z]-.+$', '', sname)
    if sname.startswith('T2'):
        r = re.search('(T2.+_[0-9]+_[0-9]+.*)_ntuple', file)
        if r:
            sname = r.group(1)
        else:
            continue
    if sname not in samples:
        samples[sname] = [filepath]
    else:
        samples[sname].append(filepath)
        
def writeSamples(fout, samples):
    for sname in samples:
        fout.write('$ %s 100 @%s\n'%(sname, sname))
        for filepath in samples[sname]:
            fout.write(filepath+'\n')

def printSignalList(samples):
    sigtype = []
    for sname in samples:
        sig = sname.split('_')[0]
        if sig not in sigtype:
            sigtype.append(sig)
            print '\n\n'+sig+':\n\n'
        print sname+',',
    print '\n\n'

with open(args.conf, 'w') as fout:
    sep = '#'*70
    fout.write('%s\n# data\n%s\n'%(sep, sep))
    writeSamples(fout, data_samples)
    fout.write(sep+'\n%\n')
    fout.write('%s\n# backgrounds\n%s\n'%(sep, sep))
    writeSamples(fout, mc_samples)
    fout.write('%s\n# signals\n%s\n'%(sep, sep))
    writeSamples(fout, signal_samples)
    printSignalList(signal_samples)


