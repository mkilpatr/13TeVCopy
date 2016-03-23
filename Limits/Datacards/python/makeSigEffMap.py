#! /usr/bin/python
import os
import sys
import ROOT as rt

""" creates root files (one for each input directory) with the efficiency plots for
the baseline selection (relitive to full yield from the xsec) and for each bin 
(relitive to the baseline selection) 
"""

bins_met = (250,300,400,500,600)
bins_nb  = (1,2)
bins_mega = ( (0,5,0), (0,7,0), (175,5,0), (175,7,0), (175,5,1) ) # mtb, njets, ntops
binlist = []

lumi = 2.26
xsecs = {}

xmin =  150.0
xmax = 1000.0
xbin =   25.0
ymin =    0.0
ymax =  500.0
ybin =   25.0


def getXbin(mstop): return int((int(mstop)-xmin)/xbin)+1
def getYbin(mlsp) : return int((int(mlsp )-ymin)/ybin)+1 


def makeEffMap(inDir):
  sigPts = {}
  for d in os.listdir(inDir):
    if '.' in d : continue
    sigPts[d] = 0
    updateXsecs(d)

  model = sigPts.keys()[0].split('_')[0]
  f = rt.TFile('effMap_'+model+'.root','RECREATE')
  sumAllBins(inDir,sigPts)
  
  print '\n'+'='*5, 'making eff map for', model, '='*5
  for bin in binlist:
    print bin
    savebin = bin.replace('ncttstd','ntop').replace('mtcsv12met','mtb')
    h = rt.TH2F(savebin,savebin, int((xmax-xmin)/xbin),xmin,xmax, int((ymax-ymin)/ybin),ymin,ymax)
    for sigPt in sigPts:
      (mstop,mlsp) = sigPt.split('_')[1:]
      sigDir = os.path.join(inDir,sigPt)
      nevents = getYield(os.path.join(sigDir,sigPt+'_'+bin+'.txt'))
      eff = nevents / sigPts[sigPt]
      h.SetBinContent(getXbin(mstop),getYbin(mlsp),eff)
    h.Write(savebin, rt.TObject.kOverwrite)
  f.Close()


def sumAllBins(inDir,sigPts):
  h = rt.TH2F('baseline','baseline', int((xmax-xmin)/xbin),xmin,xmax, int((ymax-ymin)/ybin),ymin,ymax)
  for sigPt in sigPts:
    (mstop,mlsp) = sigPt.split('_')[1:]
    sigDir = os.path.join(inDir,sigPt)
    nevents = 0
    for bin in binlist:
      nevents += getYield(os.path.join(sigDir,sigPt+'_'+bin+'.txt'))
    eff = nevents / (lumi*xsecs[int(mstop)]*1000)
    h.SetBinContent(getXbin(mstop),getYbin(mlsp),eff)
    sigPts[sigPt] = nevents
  h.Write('baseline', rt.TObject.kOverwrite)


def getYield(datacard):
  if not os.path.exists(datacard):
    print '!!!', 'unknown datacard:', datacard
    return 0
  mstop = int(datacard.split('/')[-2].split('_')[1])
  f = open(datacard)
  n = ''
  for l in f:
    if len(l) < 1 : continue
    if not 'rate' in l : continue
    n = l.split()[1]
  f.close()
  n = float(n)
  return n*xsecs[mstop]


def compileBinList():
  for met in bins_met:
    for nb in bins_nb:
      for (mtb, njets, nt) in bins_mega:
        binlist.append(getBinString(met,njets,nb,nt,mtb))


def getBinString(met,njets,nb,nt,mtb):
  return 'met'+str(met)+'_njets'+str(njets)+'_nbjets'+str(nb)+'_ncttstd'+str(nt)+'_mtcsv12met'+str(mtb)


def updateXsecs(d):
  mstop = int(d.split('_')[1])
  if mstop not in xsecs.keys():
    xsecfile = rt.TFile('../data/xsecs/stop.root')
    xsechist = rt.TH1D()
    xsechist = xsecfile.Get('xsecs')
    xsec = xsechist.Interpolate(mstop)
    xsecs[mstop] = xsec


def main(): 
  args = sys.argv[1:]
  if not args:
    print('usage: makeYieldTables.py <directory[s] with template datacards>')
    sys.exit(1)
  inDirs = args[:]
  
  compileBinList()
  
  for inDir in inDirs:
    makeEffMap(inDir)


if __name__=='__main__' : main()
