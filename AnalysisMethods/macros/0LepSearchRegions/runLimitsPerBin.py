#! /usr/bin/python
#
## Note on running:
## 
## In order to run this, you need to set up CMSSW_7_1_5 as follows
## (for more info see: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit#How_to_prepare_the_datacard)
#
# export SCRAM_ARCH=slc6_amd64_gcc481 # for csh use: setenv SCRAM_ARCH slc6_amd64_gcc481
# cmsrel CMSSW_7_1_5                  # must be a 7_1_X release >= 7_1_5;  (7.0.X and 7.2.X are NOT supported either) 
# cd CMSSW_7_1_5/src
# cmsenv
# git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
#
## Updating to a tag (both the first time and whenever there are updates):
#
# cd HiggsAnalysis/CombinedLimit
# git fetch origin
# git checkout v5.0.1
# scramv1 b clean; scramv1 b # always make a clean build, as scram doesn't always see updates to src/LinkDef.h
# 

"""This takes all the per-bin datacards from makeDatacards.py, and runs the limits on 
them individually. At the moment it assumes that the datacards for each
signal point are in their own folder .

It currently runs HybridNew, ProfileLikelihood, and Asymptotic.
"""

import commands
import os
import sys 
import time 
from ConfigParser import SafeConfigParser
from ROOT import gROOT,TFile,TTree,TH1D
gROOT.SetBatch(True)

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
  configparser = SafeConfigParser()
  configparser.optionxform = str
  
  limconfig = LimitConfig(configFile,configparser)
  limconfig.runLimits()
  
class LimitConfig:
  # setup
  def __init__(self,conf_file,config_parser):
    self.conf_file      = conf_file
    config_parser.read(self.conf_file)
    self.limitmethod    = config_parser.get('config','limitmethod')
    self.subdir         = config_parser.get('config','subdir')
    self.datacarddir    = os.path.join( config_parser.get('config','datacarddir'), self.subdir                      )
    self.limitdir       = os.path.join( config_parser.get('config','limitdir'   ), self.subdir+'_'+self.limitmethod )
    self.signals        = config_parser.get('signals','samples').replace(' ','').split(',') 
  
  
  def getLimit(self,rootFile):
    file = TFile(rootFile)
    tree = file.Get('limit')
    htmp = TH1D('htmp','htmp',1,0,10000)
    mean = htmp.GetMean()
    error = htmp.GetError()
    output = 'mean=' + str(mean) + '\terror=' + str(error) + '\n'
    return output

  # for each signal, combine the datacards for it, run the
  # chosen limit method, then print the significances for
  # eachpoint at the end. 
  def runLimits(self):
    if not os.path.exists(self.datacarddir):
      print self.datacarddir, 'does not exist!' 
      print 'Are you sure you already ran makeDatacards.py?'
      sys.exit(1)
    
    currentDir = os.getcwd()
    significances = [] # store the Significance from each sig point to print at the end
    
    for signal in self.signals: 
      # create and move into a dummy directory (using the timestamp in the name for uniqueness). At the end of 
      # each signal loop, all remaining files will be either deleted or moved to the actual output directory. 
      dummyRunDir = 'dummy_' + str(int(time.time()*1e8))
      print 'creating a dummy directory to run in:', dummyRunDir 
      os.makedirs(dummyRunDir)
      os.chdir(dummyRunDir)
        
      datacardSaveLocation = os.path.join(currentDir,self.datacarddir,signal)
      datacards = os.listdir(datacardSaveLocation)
      combinedDatacard = 'combined_'+signal+'.txt' # REMOVE
      
      sigSignificances = []
      
      # ===== run the limits ===== 
      #
      ### notes from previous versions:
      # runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance -t 500 --expectSignal=1 -n '+limitsOutputFile
      # for now, remove '--pvalue' (between --significance and -t 500)
      # note: the default for -t is -1 (Asimov dataset) see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit
      #####
      
      # === Asymptotic
      if self.limitmethod=='Asymptotic':
        for datacard in datacards:
          #runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance --saveWorkspace -t -1 --expectSignal=1 -n '+signal
          runLimitsCommand =  'combine -M ProfileLikelihood '+os.path.join(datacardSaveLocation,datacard)+' --significance -t -1 --expectSignal=1 -n '+datacard.replace('.txt','')
          # run the limit command and figure out what the output root file is
          print 'now running:' #DEUBGGING ONLY 
          print runLimitsCommand, '\n' # keep this in some kind of log file?
          output = commands.getoutput(runLimitsCommand)
          print output # maybe redirect this to some kind of log file?
          
          # pull the Significance out and store
          tempSig = ''
          for line in output.split('\n'): 
            if 'Significance' in line: 
              tempSig = ( float(line.replace('Significance: ','')), datacard.replace('.txt','') )  
          if tempSig=='': 
            sigSignificances.append( (-1.0, datacard.replace('.txt','')) )
          else:
            sigSignificances.append(tempSig)
      
      # === non-existant choice of limit method
      else:
        print self.limitmethod, 'is not one of the currently implemented methods! You need to pick one of the allowed ones!'
        sys.exit(1)
        
      # sort by append significances for current signal point to full list of lists
      sigSignificances = sorted( sigSignificances, reverse=True, key=lambda s: s[0])
      significances.append(sigSignificances)
      
      # move any output files to the correct directory
      os.chdir(currentDir)
      dummyFiles = os.listdir(dummyRunDir)
      outputLocation = os.path.join(currentDir,self.limitdir,signal+'_perBin')
      if not os.path.exists(outputLocation): os.makedirs(outputLocation)
      print '\n'
      print 'moving files to:', outputLocation
      for f in dummyFiles:
        print ' '*3, f
        if 'roostats' in f:
          os.remove(os.path.join(dummyRunDir,f))
          continue
        os.rename(os.path.join(dummyRunDir,f),os.path.join(outputLocation,f)) 
      os.removedirs(dummyRunDir)
      print '\n', '='*60, '\n'
    
    # print the significances
    print '='*5, 'SIGNIFICANCES', '('+self.limitmethod+')', '='*5
    for pointSigs in significances:
      print '\n'
      for sig in pointSigs:
        print str(sig[0]) + ' \t' + sig[1]
      print '\n'


if __name__=='__main__': main()
