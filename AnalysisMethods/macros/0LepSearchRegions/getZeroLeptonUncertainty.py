#! /usr/bin/python

'''
Calculate the full (stat.+syst.) uncertainty of the background prediction.
Predicted yields and stat. uncertainties (MC stat. and CR data stat.) are taken from the output of background prediction macro.
[see: AnalysisMethods/macros/0LepSearchRegions/getZeroLeptonPrediction.C] 
Syst. uncertainties are calculate based on the config files for datacard production.
[see: Limits/Datacards/setup/values_0l_unc_*.conf] 
'''

import os
import math
import json
import argparse
import ROOT as rt

uncfiles=['values_0l_unc_btag.conf',
 'values_0l_unc_jets.conf',
 'values_0l_unc_lepcorr.conf',
 'values_0l_unc_llbnt1metint.conf',
#  'values_0l_unc_mcstat.conf',
# 'values_0l_unc_perbin.conf',
 'values_0l_unc_pu.conf',
 'values_0l_unc_qcd.conf',
 'values_0l_unc_ttz.conf',
 'values_0l_unc_wtopfrac.conf',
 'values_0l_unc_znunu.conf']

ttzuncfile = 'values_0l_unc_all.conf'

all_samples=('ttbarplusw', 'znunu', 'ttZ', 'qcd')
hist_names=('lostlep_pred_sr', 'znunu_pred_sr', 'ttZ_pred_sr', 'qcd_pred_sr')

# ordered bin list
binlist=("bin_250_5_1_0_0", "bin_300_5_1_0_0", "bin_400_5_1_0_0", "bin_500_5_1_0_0", "bin_600_5_1_0_0",
         "bin_250_5_2_0_0", "bin_300_5_2_0_0", "bin_400_5_2_0_0", "bin_500_5_2_0_0", "bin_600_5_2_0_0",
         "bin_250_7_1_0_0", "bin_300_7_1_0_0", "bin_400_7_1_0_0", "bin_500_7_1_0_0", "bin_600_7_1_0_0",
         "bin_250_7_2_0_0", "bin_300_7_2_0_0", "bin_400_7_2_0_0", "bin_500_7_2_0_0", "bin_600_7_2_0_0",
         "bin_250_5_1_0_175", "bin_300_5_1_0_175", "bin_400_5_1_0_175", "bin_500_5_1_0_175", "bin_600_5_1_0_175",
         "bin_250_5_2_0_175", "bin_300_5_2_0_175", "bin_400_5_2_0_175", "bin_500_5_2_0_175", "bin_600_5_2_0_175",
         "bin_250_7_1_0_175", "bin_300_7_1_0_175", "bin_400_7_1_0_175", "bin_500_7_1_0_175", "bin_600_7_1_0_175",
         "bin_250_7_2_0_175", "bin_300_7_2_0_175", "bin_400_7_2_0_175", "bin_500_7_2_0_175", "bin_600_7_2_0_175",
         "bin_250_5_1_1_175", "bin_300_5_1_1_175", "bin_400_5_1_1_175", "bin_500_5_1_1_175", "bin_600_5_1_1_175",
         "bin_250_5_2_1_175", "bin_300_5_2_1_175", "bin_400_5_2_1_175", "bin_500_5_2_1_175", "bin_600_5_2_1_175")


def sumUnc(unc_list):
    '''Add uncertainties in quadrature.'''
    return math.sqrt(sum(err*err for err in unc_list))


# relUnc[type][bin][sample] : percentage syst. uncertainty of each 'type' per 'bin' per 'sample'
# absUnc[bin][type] : absolute uncertainty per 'bin' of each 'type'
# yields[bin][sample] : predicted bkg yields per bin per sample 
# statUnc[bin] : stat. (MC + data CR) unc. per bin
# systUnc[bin] : syst. unc. per bin
# fullUnc[bin] : full unc. per bin

relUnc={}
absUnc={}
absUnc_pieces={'ttbarplusw':{}, 'znunu':{}, 'qcd':{}, 'ttZ':{}}
yields={}
yields_data={}
statUnc={}
statUnc_pieces={}
systUnc={}
systUnc_pieces={'ttbarplusw':{}, 'znunu':{}, 'qcd':{}, 'ttZ':{}}
fullUnc={}
fullUnc_pieces={'ttbarplusw':{}, 'znunu':{}, 'qcd':{}, 'ttZ':{}}
allVals = {}

def readRelUnc(config_path):
    '''Read in percentage syst. uncertainty from the config files.'''
    for unc_fn in uncfiles:
        with open(os.path.join(config_path, unc_fn), 'r') as f:
            for line in f.readlines():
                try:
                    bin, type, sample, value = line.split()
                except ValueError:
                    continue
                if bin not in binlist or sample not in all_samples:
                    # ignore CR syst., e.g., "bin_onelepcr_250_5_1_0_0"
                    continue
                if type not in relUnc:
                    relUnc[type] = {}
                if bin not in relUnc[type]:
                    relUnc[type][bin] = {}
                if sample in all_samples:
                    relUnc[type][bin][sample] = float(value)-1
    
    # only for ttZ: need special treatment for lumi and ttZNorm unc.
    with open(os.path.join(config_path, ttzuncfile), 'r') as f:
        for line in f.readlines():
            try:
                bin, type, sample, value = line.split()
            except ValueError:
                continue
            sample = 'ttZ'
            if type not in relUnc:
                relUnc[type] = {}
            # bin=='all' in config file
            for bin in binlist:
                if bin not in relUnc[type]:
                    relUnc[type][bin] = {sample : float(value)-1}

    # if a type of unc. is not define for some samples/bins, then fill zero
    for type in relUnc:
        for bin in binlist:
            if bin not in relUnc[type]:
                # for bin-specific unc. (e.g., qcd-tf) : set other bins to 0
                relUnc[type][bin] = {sample:0 for sample in all_samples}
            else:
                for sample in all_samples:
                    # for sample specific unc.: set other samples to 0
                    if sample not in relUnc[type][bin]:
                        relUnc[type][bin][sample] = 0


def readYields(pred_file):
    ''' Read in predicted bkg yields and the stat. unc. 
    
    pred_file -- input root file 
    '''
    f = rt.TFile(pred_file)
    for hname, sample in zip(hist_names, all_samples):
        h = f.Get(hname)
        for ibin in xrange(0, h.GetNbinsX()):
            bin = binlist[ibin]
            if bin not in yields:
                yields[bin] = {}
                statUnc_pieces[bin] = {}
            y = h.GetBinContent(ibin+1)
            e = h.GetBinError(ibin+1)
            yields[bin][sample] = y
            if sample == 'ttZ': statUnc_pieces[bin][sample] = min(e,y) # don't want MC stat unc > 100%
            else :              statUnc_pieces[bin][sample] = e
    h = f.Get('data_sr')
    for ibin in xrange(0, h.GetNbinsX()):
        bin = binlist[ibin]
        yields_data[bin] = (h.GetBinContent(ibin+1), h.GetBinError(ibin+1))
    f.Close()
    for bin in binlist:
        statUnc[bin] = sumUnc(statUnc_pieces[bin].values())


def calcAbsUnc():
    ''' Calculate syst. uncertainty.
    For uncertainties of the same type, add linearly for each bin.
    For uncertainties of different types, add in quadrature for each bin.
    '''
    absUnc = {bin:{} for bin in binlist}
    for sample in all_samples: absUnc_pieces[sample] = {bin:{} for bin in binlist}
    for bin in binlist:
        absUnc[bin] = {type:0 for type in relUnc.keys()}
        for sample in all_samples: absUnc_pieces[sample][bin] = {type:0 for type in relUnc.keys()}
        for type in relUnc:
            for sample in all_samples:
                # Add the same type of unc. linearly
                tempUnc = relUnc[type][bin][sample] * yields[bin][sample]
                absUnc[bin][type]                += tempUnc
                absUnc_pieces[sample][bin][type] += tempUnc

    for bin in absUnc:
        # Add different types of unc. in quadrature
        systUnc[bin] = sumUnc(absUnc[bin].values())
        fullUnc[bin] = sumUnc([statUnc[bin], systUnc[bin]])
        for sample in all_samples:
            systUnc_pieces[sample][bin] = sumUnc(absUnc_pieces[sample][bin].values())
            fullUnc_pieces[sample][bin] = sumUnc([statUnc_pieces[bin][sample], systUnc_pieces[sample][bin]])


def writeFullUnc(pred_file):
    ''' Update the input root file, add a hist with total prediction and full uncertainty. '''
    f = rt.TFile(pred_file, 'UPDATE')
    h = rt.TH1F('bkgtotal_unc_sr', '', len(binlist), 0, len(binlist))
    h_pieces = {}
    for sample in all_samples : h_pieces[sample] = rt.TH1F(sample+'_unc_sr', '', len(binlist), 0, len(binlist))
    print "%30s %10s %8s" % ('bin', 'total pred', 'total unc.')
    for ibin in xrange(0, h.GetNbinsX()):
        bin = binlist[ibin]
        val = sum(yields[bin].values())
        err = fullUnc[bin]
        h.SetBinContent(ibin+1, val)
        h.SetBinError(ibin+1, err)
        print "%30s %10.2f %8.2f" % (bin, val, err)
        allVals[bin] = {'bkg':(val,err)}
        for sample in all_samples:
            val = yields[bin][sample]
            err = fullUnc_pieces[sample][bin]
            h_pieces[sample].SetBinContent(ibin+1, val)
            h_pieces[sample].SetBinError(ibin+1, err)
            allVals[bin][sample] = (val,err)  
    h.Write('bkgtotal_unc_sr', rt.TObject.kOverwrite)
    for sample in all_samples : h_pieces[sample].Write(sample+'_unc_sr', rt.TObject.kOverwrite)
    f.Close()

def makeYieldTable():
    ''' Make a Latex-formatted table with each bkg plus unc, total bkg plus unc, and observed data for every bin. '''
    print '\nprinting yield table...\n'
    s  = '\\hline\n'
    s += '\\met [GeV]  &  \\ttbar, \\W+jets  &  \\znunu  &  QCD  &  \\ttZ  &  total SM  &  $N_{\\rm data}$  \\\\ \n'
    s += '\\hline\n'
    s += makeTableChunk(5,1,  0,0)
    s += makeTableChunk(5,2,  0,0)
    s += makeTableChunk(7,1,  0,0)
    s += makeTableChunk(7,2,  0,0)
    s += makeTableChunk(5,1,175,0)
    s += makeTableChunk(5,2,175,0)
    s += makeTableChunk(7,1,175,0)
    s += makeTableChunk(7,2,175,0)
    s += makeTableChunk(5,1,175,1)
    s += makeTableChunk(5,2,175,1)
    s += '\\hline\n'
    print s
    
def makeTableChunk(nj,nb,mtb,nt):
    ''' Put together the table chunk for the given nj,nb,mtb,nt mega-bin. '''
    s = chunkHeader(nj,nb,mtb,nt,7)
    for binMet in [(250,'250-300'), (300,'300-400'), (400,'400-500'), (500,'500-600'), (600,'$>$600')] :
        bin = '_'.join(['bin',str(binMet[0]),str(nj),str(nb),str(nt),str(mtb)])
        s += binMet[1]
        for bkg in ('ttbarplusw', 'znunu', 'qcd', 'ttZ', 'bkg') :
            n, e = allVals[bin][bkg]
            s += formatPrediction(n,e)
        n, e = yields_data[bin]
        s += ' & ' + str(int(n))
        s += ' \\\\ \n'
    return s

# formats the prediction nEvents +/- error
def formatPrediction(n,e):
  if n>=10:
    return ' & ' + str(int(round(n,0))) + ' $\\pm$ ' + str(int(round(e,0)))
  if n>=1:
    return ' & ' + str(round(n,1)) + ' $\\pm$ ' + str(round(e,1))
  return ' & ' + str(round(n,2)) + ' $\\pm$ ' + str(round(e,2))

# puts together the bin header for bins of nJets, mtb, nTop (no selection on nB)
def chunkHeader(nj,nb,mtb,nt,columns):
    ''' Put together the mega-bin chunk header. '''
    s  = '\\hline\n'
    s += '\\multicolumn{'+str(columns)+'}{c}{'
    if  (mtb==  0 and nj==5           and nb==1) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$, $\\nb = 1$'
    elif(mtb==  0 and nj==7           and nb==1) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$, $\\nb = 1$'
    elif(mtb==175 and nj==5 and nt==0 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$, $\\nb = 1$'
    elif(mtb==175 and nj==7 and nt==0 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$, $\\nb = 1$'
    elif(mtb==175 and nj==5 and nt==1 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$, $\\nb = 1$'
    elif(mtb==  0 and nj==5           and nb==2) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$, $\\nb \\geq 2$'
    elif(mtb==  0 and nj==7           and nb==2) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$, $\\nb \\geq 2$'
    elif(mtb==175 and nj==5 and nt==0 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$, $\\nb \\geq 2$'
    elif(mtb==175 and nj==7 and nt==0 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$, $\\nb \\geq 2$'
    elif(mtb==175 and nj==5 and nt==1 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$, $\\nb \\geq 2$'
    else : s += 'UNKNOWN HEADER (mtb='+str(mtb)+',nj='+str(nj)+',nt='+str(nt)+'nb='+str(nb)+')'
    #nJets $NJETS, \mtb MTB$~\\GeV, $N_\\mathrm{t} NT$
    s += '} \\\\ \n'
    s += '\\hline\n' 
    return s


def main():
    parser = argparse.ArgumentParser('Calculate the full (stat.+syst.) uncertainty of the background prediction.')
    parser.add_argument('predfile',
        default='srpred.root',
        help='Input prediction file. Default: %(default)s'
    )
    parser.add_argument('-c', '--configdir',
        dest='configdir',
        default='../../../Limits/Datacards/setup',
        help='Uncertainties configs directory. Default: %(default)s'
    )
    parser.add_argument('-t', '--printTable',
        dest='printTable',
        action='store_true',
        help='print the full bkg prediction table: %(default)s'
    )
    args = parser.parse_args()
    
    readRelUnc(args.configdir)
    readYields(args.predfile)
    calcAbsUnc()
    writeFullUnc(args.predfile)
    if args.printTable : makeYieldTable()


if __name__=='__main__':
    main()
