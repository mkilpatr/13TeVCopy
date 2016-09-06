import json
import argparse
import ROOT as rt
rt.gROOT.SetBatch(True)

def calcGenEfficiency(filename, treename):
    filter_info = {} # fomat: {mass point: {pass:n_pass, total:n_total}} 
    
    f = rt.TFile.Open(filename)
    t = f.Get(treename)
    for i in range(t.GetEntries()):
        t.GetEntry(i)
        mass_str = '_'.join([str(m) for m in t.massparams])
#         print '%s\t%f\t%f'%(mass_str, t.sumPassWeights, t.sumWeights)
        if mass_str not in filter_info:
            filter_info[mass_str] = {'pass':t.sumPassWeights, 'total':t.sumWeights}
        else:
            filter_info[mass_str]['pass'] += t.sumPassWeights
            filter_info[mass_str]['total'] += t.sumWeights
    
    filter_eff = {}
    for mass_str in filter_info:
        filter_eff[mass_str] = filter_info[mass_str]['pass']/filter_info[mass_str]['total']
    
    return filter_eff


def writeJson(input, outputfile):
    output = json.dumps(input, sort_keys=True, indent=2)
    with open(outputfile, 'wb') as f:
        f.write(output)



def main():
    parser = argparse.ArgumentParser(description='Calcuate gen-filter efficiency')
    parser.add_argument('filename',
        metavar='filename',
        help='Input file name'
    )
    parser.add_argument('-o', '--output',
        dest='output',
        default = 'filter_efficiency.json',
        help='Output json file. Default: %(default)s.'
    )
    parser.add_argument('-t', '--treename',
        dest = 'treename',
        default = 'analyzer/GenFilter',
        help = 'Tree name. Default: %(default)s.',
    )
    args = parser.parse_args()
    
    eff = calcGenEfficiency(args.filename, args.treename)
    writeJson(eff, args.output)

    
if __name__ == '__main__':
    main()
