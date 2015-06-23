#! /usr/bin/python
import os
import sys

from ConfigParser import SafeConfigParser
from collections import OrderedDict
from ROOT import gROOT,TFile,TTree,TH1D
gROOT.SetBatch(True)

hline  = '\\hline\n'
endrow = '\\\\\n'

def main():
  # to get the config file
  configFile = 'dc_0l_setup.conf'
  args = sys.argv[1:] 
  if len(args)>=1:
    configFile = args[0]
  if os.path.exists(configFile):
    print 'running with config file', configFile
  else:
    print 'you are trying to use a config file ('+configFile+') that does not exist!'
    sys.exit(1)
  configparser = SafeConfigParser(dict_type=OrderedDict)
  configparser.optionxform = str
  
  latexFile = open('cutflowTables.tex','w')
  cfconfig = cutflowConfig(configFile,configparser)
  latexFile.write(cfconfig.produceTables())
  latexFile.close()
  del latexFile

class cutflowConfig:
  # setup
  def __init__(self,conf_file,config_parser):
    self.conf_file      = conf_file
    config_parser.read(self.conf_file)
    self.treebase       = config_parser.get('config','treelocation')
    self.treename       = config_parser.get('config','treename')
    self.projvar        = config_parser.get('config','projvar')
    self.weightname     = config_parser.get('config','weightname')
    self.lumiscale      = config_parser.get('config','lumiscale')
    self.filesuffix     = config_parser.get('config','filesuffix')
    self.signals        = config_parser.get('signals','samples').replace(' ','').split(',')
    self.backgrounds    = config_parser.get('backgrounds','samples').replace(' ','').split(',')
  
  def produceTables(self):
    latexFileStart  = '\\documentclass{article}\n'
    latexFileStart += '\\usepackage{slashed}\n'
    latexFileStart += '\\usepackage[a4paper,margin=.05in,landscape]{geometry}\n'
    latexFileStart += '\n'
    latexFileStart += '\\begin{document}\n'
    latexFileStart += '\\small\n'
    latexFileStart += '\n'
    latexFileStart += 'preselection: $MET>200, N_\\mathrm{j60} \\geq 2$\n'
    latexFileStart += '\n'
    
    latexFileEnd  = '\n'
    latexFileEnd += '\\end{document}\n'
    latexFileEnd += '\n'
    
    tableInclusive = self.makeTable('','nj60>=2')
    
    return latexFileStart + tableInclusive + latexFileEnd
    
  def makeTable(self,binName='',baseCut=''):
    nCols = 1 + len(self.backgrounds) + 1 + len(self.signals)
    cutstr = baseCut
    
    tableStr = ''
    if binName != '': tableStr += binName + '\n'
    
    tableStr += '\\begin{tabular}{|' +  'c|'*nCols +'}\n'
    tableStr += hline
    
    # table column lables
    tableStr += 'Process\t'
    for bkg in self.backgrounds:
      tableStr += '& ' + bkg + '\t'
    tableStr += '& total bkg\t'
    for sig in self.signals:
      tableStr += '& ' + sig.replace('_',' ') + '\t'
    tableStr += endrow + hline
    
    previousNums = {}
    
    # preselection
    nBkg = 0
    tableStr += 'presel\t'
    for bkg in self.backgrounds:
      n = self.getNumEvents(bkg, cutstr)
      tableStr += '& ' + str(n) + ' (1.00)\t'
      nBkg += n 
      previousNums[bkg] = n 
    tableStr += '& ' + str(nBkg) + ' (1.00)\t'
    previousNums['allBkg'] = nBkg
    for sig in self.signals:
      n = self.getNumEvents(sig, cutstr)
      tableStr += '& ' + str(n) + ' (1.00)\t'
      previousNums[sig] = n 
    tableStr += endrow + hline
    
    # lepton veto
    cutstr += ' && nVetoedLeptons<1'
    tableStr += self.addCut('$e \\mu$ veto',cutstr,previousNums)
    
    # n jets
    cutstr += ' && nJ20>=5'
    tableStr += self.addCut('$N_\\mathrm{j20} \\geq 5$',cutstr,previousNums)
    
    # b jets
    cutstr += ' && nmBtag>=1'
    tableStr += self.addCut('$N_\\mathrm{b} \\geq 1$',cutstr,previousNums)
    
    # tau veto
    cutstr += ' && nVetoedTaus<1'
    tableStr += self.addCut('$\\tau$ veto',cutstr,previousNums)
    
    # delta phi cuts
    cutstr += ' && dPhiMET12>1&&dPhiMET3>0.5'
    tableStr += self.addCut('$\\Delta \\phi \\mathrm(cuts)$',cutstr,previousNums)
    
    # mtB01MET
    cutstr += ' && mtB01MET>175'
    tableStr += self.addCut('$\\mathrm{mtB01MET}>175$',cutstr,previousNums)
     
    # ===== now look at bins ===== 
    tableStr += hline
    
    # mtB01MET
    cutstrTop0 = cutstr + ' && NCTTstd==0'
    tableStr += self.addCut('$\\mathrm{NCTTstd}=0$',cutstrTop0,previousNums,False)
    
    # mtB01MET
    cutstrTop1 = cutstr + ' && NCTTstd>=1'
    tableStr += self.addCut('$\\mathrm{NCTTstd}\\geq1$',cutstrTop1,previousNums,False)
    
    
    tableStr += '\\end{tabular}\n'
    
    return tableStr
  
  def getNumEvents(self,sample,cutstr):
    cutstr = '('+str(self.lumiscale)+'*'+self.weightname+'*('+cutstr+'))'
    filename = os.path.join( self.treebase, sample+self.filesuffix )
    
    file = TFile(filename)
    tree = file.Get(self.treename)
    htmp = TH1D('htmp','htmp',1,0,10000)
    tree.Project('htmp',self.projvar,cutstr)
    rate = htmp.Integral(0,2)
    
    return round(rate,2)
  
  def addCut(self,cutName,cutstr,previous,updateNumbers=True):
    nBkg = 0
    newRow = ''
    newRow += cutName+'\t'
    for bkg in self.backgrounds:
      n = self.getNumEvents(bkg, cutstr)
      newRow += '& ' + str(n) + ' ('+ str(round(n/previous[bkg],2)) +')\t'
      nBkg += n 
      if updateNumbers: previous[bkg] = n 
    newRow += '& ' + str(nBkg) + ' ('+ str(round(nBkg/previous['allBkg'],2)) +')\t'
    if updateNumbers: previous['allBkg'] = nBkg
    for sig in self.signals:
      n = self.getNumEvents(sig, cutstr)
      newRow += '& ' + str(n) + ' ('+ str(round(n/previous[sig],2)) +')\t'
      if updateNumbers: previous[sig] = n 
    newRow += endrow + hline
    return newRow

if __name__=='__main__': main()