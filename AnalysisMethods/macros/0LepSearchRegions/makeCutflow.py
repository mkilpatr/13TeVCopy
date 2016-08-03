#! /usr/bin/python
import os
import sys
from ROOT import gROOT,TFile,TTree,TH1D
gROOT.SetBatch(True)

#from collections import OrderedDict

lumi       = 12.9
weight     = 'weight*truePUWeight*qcdRespTailWeight*leptnpweightHM*lepvetoweightHM*btagWeight*btagFastSimWeight*isrWeightTight*(1.0*(mtcsv12met<=175)+sdtopFastSimWeight*sdwFastSimWeight*(mtcsv12met>175))'
projvar    = 'met'
treename   = 'Events'
filesuffix = '_tree.root'
#treeDir    = '/eos/uscms/store/user/mullin/13TeV/lepCor/trees/160217_noPreSel'
treeDir    = 'hqu_20160728_13ifb/'

#signals = ['T2tt_500_200','T2tt_500_250','T2tt_600_200','T2tt_850_1','ttbarplusw','znunu','ttZ']
signals = ['T2tt_850_100','T2tt_500_325','T2tt_425_325']

baselines = [
              ('inclusive'             , ''                         ),
              #('0 gen e/$\\mu$'        , 'ngoodgenmu+nvetolele==0'  ),
              #('$\geq1$ gen e/$\\mu$'  , 'ngoodgenmu+nvetolele>=1'  ),
            ]

#nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0)) && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && passjson && (passmetmht100 || ismc) 
#&& met>250 && njets>=5 && nbjets>=1 && nlbjets>=2 && dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5

cuts = [
         ('Trigger preselection ($\\met > 250$, $N_{jets}>=2$)'  , 'met>250 && njets>=2 && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && passjson && (passmetmht100 || ismc) '  ),
         #('$\\met > 200$'          , 'met>200'                                    ),
         #('$\\met > 250$'          , 'met>250'                                    ),
         #('$\pt (J_{1,2})>75$'     , 'j1pt>75 && j2pt>75'                         ),
         ('lepton veto'            , 'nvetolep==0'                                ),
         ('$\\tau$ veto'           , '(nvetotau==0 || (ismc && npromptgentau>0))' ),
         #('$\\tau$ veto'           , 'nvetotau==0'                                ),
         ('$N_{J} \\geq 5$'        , 'njets>=5'                                   ),
         ('$\\nb^{l} \\geq 2$, $\\nb \\geq 1$'     , 'nlbjets>=2 && nbjets>=1'    ),
         #('$\\nb^{l} \\geq 2$, '     , 'nlbjets>=2'                                 ),
         #('$\\nb \\geq 1$'         , 'nbjets>=1'                                  ),
         ('$\\dphij_{1234} > 0.5$' , 'dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5'),
         #('$\\dphij_{12} > 0.5$'   , 'dphij12met>0.5'                             ),
         #('$\\dphij_{3} > 0.5$'    , 'dphij3met>0.5'                              ),
         #('$\\dphij_{4} > 0.5$'    , 'dphij4met>0.5'                              ),
         #
         #('$\\tau$ veto'           , '(nvetotau==0 || (ismc && npromptgentau>0))' ),
         #('$\\tau$ veto'           , 'nvetotau==0'                                ),
       ]

binnings = {
  '\\met~bins' : [
    ('\\met~250-300'     , 'met>=250 && met<300'),
    ('\\met~300-400'     , 'met>=300 && met<400'),
    ('\\met~400-500'     , 'met>=400 && met<500'),
    ('\\met $\\geq$ 500' , 'met>=500'           ),
  ],
  '\\mtb, \\nj, \\nt, \\nb~bins' : [
    ('low  $\\mtb$, low  $\\nj$, $\\nb=1$' , 'mtcsv12met<175  && njets<=6               && nbjets==1' ),
    ('low  $\\mtb$, low  $\\nj$, $\\nb \\geq 2$' , 'mtcsv12met<175  && njets<=6               && nbjets>=1' ),
    ('low  $\\mtb$, high $\\nj$, $\\nb=1$' , 'mtcsv12met<175  && njets>=7               && nbjets==1' ),
    ('low  $\\mtb$, high $\\nj$, $\\nb \\geq 2$' , 'mtcsv12met<175  && njets>=7               && nbjets>=1' ),

    ('high $\\mtb$, $N_{t}=0$, $N_{W}=0$, $\\nb=1$, low $\\nj$' , 'mtcsv12met>=175 && njets<=6 && nsdtoploose==0 && nsdwloose==0 && nbjets==1' ),
    ('high $\\mtb$, $N_{t}=0$, $N_{W}=0$, $\\nb \\geq 2$, low $\\nj$' , 'mtcsv12met>=175 && njets<=6 && nsdtoploose==0 && nsdwloose==0 && nbjets>=2' ),
    ('high $\\mtb$, $N_{t}=0$, $N_{W}=0$, $\\nb=1$, high $\\nj$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose==0 && nsdwloose==0 && nbjets==1' ),
    ('high $\\mtb$, $N_{t}=0$, $N_{W}=0$, $\\nb \\geq 2$, high $\\nj$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose==0 && nsdwloose==0 && nbjets>=2' ),
    ('high $\\mtb$, $N_{t} \\geq 1$, $N_{W}=0$, $\\nb=1$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose>=1 && nsdwloose==0 && nbjets==1' ),
    ('high $\\mtb$, $N_{t} \\geq 1$, $N_{W}=0$, $\\nb \\geq 2$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose>=1 && nsdwloose==0 && nbjets>=2' ),
    ('high $\\mtb$, $N_{t}=0$, $N_{W} \\geq 1$, $\\nb=1$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose==0 && nsdwloose>=1 && nbjets==1' ),
    ('high $\\mtb$, $N_{t}=0$, $N_{W} \\geq 1$, $\\nb \\geq 2$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose==0 && nsdwloose>=1 && nbjets>=2' ),
    ('high $\\mtb$, $N_{t} \\geq 1$, $N_{W} \\geq 1$, $\\nb=1$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose>=1 && nsdwloose>=1 && nbjets==1' ),
    ('high $\\mtb$, $N_{t} \\geq 1$, $N_{W} \\geq 1$, $\\nb \\geq 2$' , 'mtcsv12met>=175 && njets>=7 && nsdtoploose>=1 && nsdwloose>=1 && nbjets>=2' ),
  ],
  #'binning 1: \\mtb, \\nj, \\nt' : [
  #  ('low  $\\mtb$, low  $\\nj$' , 'mtcsv12met<175  && njets<=6'               ),
  #  ('low  $\\mtb$, high $\\nj$' , 'mtcsv12met<175  && njets>=7'               ),
  #  ('high $\\mtb$, low  $\\nj$' , 'mtcsv12met>=175 && njets<=6 && ncttstd==0' ),
  #  ('high $\\mtb$, high $\\nj$' , 'mtcsv12met>=175 && njets>=7 && ncttstd==0' ),
  #  ('high $\\mtb$, high $\\nt$' , 'mtcsv12met>=175             && ncttstd>=1' ),
  #],
  #'binning 2: \\nb' : [
  #  ('$\\nb = 1$'     , 'nbjets==1' ),
  #  ('$\\nb \\geq 2$' , 'nbjets>=2' ),
  #],
}

hline  = '\\hline\n'
endrow = ' \\\\\n'

yields  = {}
xsecs   = {'ttbarplusw' : 1,
           'znunu' : 1,
           'ttZ' : 1,
           'T2tt_850_100': 18.96,
           'T2tt_500_325': 518.5,
           'T2tt_425_325': 1312,
           'T2tt_600_200': 174.59900000000002, 
           'T2tt_500_200': 518.48000000000002, 
           'T2tt_500_250': 518.48000000000002, 
           'T2tt_850_1': 18.961200000000002
          } #{}

def fillXsecs():
  xsecfile = TFile('$CMSSW_BASE/src/data/xsecs/stop.root')
  xsechist = TH1D()
  xsechist = xsecfile.Get('xsecs')
  for sig in signals:
    if not sig in xsecs.keys():
      mstop = int(sig.split('_')[1])
      xsecs[sig] = xsechist.Interpolate(mstop)*1000 # pb->fb (lumi is in 1/fb)

def getNumEvents(sample,cutstr):
  cutstr = '('+str(lumi)+'*'+weight+')*('+cutstr+')'
  file = TFile(os.path.join(treeDir,sample+filesuffix))
  tree = file.Get(treename)
  htmp = TH1D('htmp','htmp',1,0,10000)
  tree.Project('htmp',projvar,cutstr)
  rate = htmp.Integral(0,2)
  return round(rate,2)

def rndYield(n):
  if n >= 20 : return int(round(n,0))
  if n >=  1 : return round(n,1)
  return round(n,2)

def startTable():
  s  = '\n\\begin{tabular}{|c|' + '| c c '*len(signals) + '|}\n' + hline
  for sig in signals:
    if 'T' in sig:
      (model,mstop,mlsp) = sig.split('_')
      s += ' & \\multicolumn{2}{c|}{'+model+'('+mstop+','+mlsp+')}'
    else:
      s += ' & \\multicolumn{2}{c|}{'+sig+'}'
  s += endrow + hline + hline
  return s

def newSection(header):
  return '\\multicolumn{'+str(1+2*len(signals))+'}{c}{'+header+'}' + endrow + hline 

def finishTable():
  return '\\end{tabular}\n'

def fullSigLine(baseline):
  s = 'lumi*cross-section'
  for sig in signals:
    n = lumi*xsecs[sig]
    s += ' & '+str(rndYield(n))+' &'
    yields[sig] = n
  s += endrow + hline
  if(len(baseline[1])>0):
    s += baseline[0]
    for sig in signals:
      n = getNumEvents(sig,baseline[1])
      s += ' & '+str(rndYield(n))+' & '
      s += str(int(round(100*n/yields[sig],0)))+'\\%' if sig in yields.keys() else ''
      yields[sig] = n
    s += endrow + hline
  return s

def getCutLine(baseline,n):
  cutlable = cuts[n][0]
  cutstr  = baseline+' && ' if len(baseline)>0 else ''
  cutstr += ' && '.join(zip(*cuts[:n+1])[1])
  s = cutlable
  for sig in signals:
    n  = getNumEvents(sig, cutstr)
    s += ' & ' + str(rndYield(n)) + ' & '
    if sig in yields.keys():
      prev = yields[sig]
      if prev==0 : s += '-'
      else       : s += str(int(round(100*n/prev,0)))+'\\%'
    yields[sig] = n
  s += endrow + hline
  return s

def getBinLine(baseline,bin):
  binlabel = bin[0]
  cutstr  = baseline+' && ' if len(baseline)>0 else ''
  cutstr += ' && '.join(zip(*cuts)[1]+(bin[1],))
  s = binlabel
  for sig in signals:
    n  = getNumEvents(sig, cutstr)
    s += ' & ' + str(rndYield(n)) + ' & '
    if sig in yields.keys():
      prev = yields[sig]
      if prev==0 : s += '-'
      else       : s += str(int(round(100*n/prev,0)))+'\\%'
  s += endrow + hline
  return s

def makeTable(baseline):
  print startTable(),
  print fullSigLine(baseline),
  if(len(cuts)>0):
    print newSection('cutflow' + (' ('+baseline[0]+')' if len(baseline[1])>0 else '')),
    for n in range(len(cuts)):
      print getCutLine(baseline[1],n),
  if(len(binnings)>0):
    for lab in sorted(binnings.keys()):
      binning = binnings[lab]
      if(len(binning)>0):
        print newSection(lab),
        for bin in binning:
          print getBinLine(baseline[1],bin),
  print finishTable()

def main():
  fillXsecs()
  for baseline in baselines:
    print '\n\nMaking Table for baseline:', baseline[0], '\n'
    makeTable(baseline)

if __name__=='__main__': main()
