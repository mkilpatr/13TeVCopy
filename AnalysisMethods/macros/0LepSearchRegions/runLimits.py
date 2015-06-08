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

"""This takes all the per-bin datacards from makeDatacards.py, combines them into a single
datacard and then runs the limits. At the moment it assumes that the datacards for each
signal point are in their own folder and combines all files in each signal point's folder.

It currently runs the "ExpSign" mode from Loukas's code (changing '-t -1' to '-t 100'; see 
below), but may be updated with the other methods at some point.
"""

import os
import commands
  
# ===== user defined stuff =====

sigPoints = ['T2tt_850_100','T2tt_650_325',] # 'T2tt_500_325' ]
saveLocation = 'datacards/150602/' # this should be the same as the one in makeDatacards.py

##### ##### ##### ##### ##### ##### ##### 
##### you should not need to touch  ##### 
##### anything below this point!!!  ##### 
##### ##### ##### ##### ##### ##### ##### 

for sigPoint in sigPoints:
  datacardSaveLocation = saveLocation + sigPoint+'/'
  datacards = os.listdir(datacardSaveLocation)
  combinedDatacard = 'combined_'+sigPoint+'.txt'
  
  combineDatacardsCommand = 'combineCards.py'
  for datacard in datacards:
    combineDatacardsCommand += ' ' + datacardSaveLocation+datacard
  combineDatacardsCommand += ' > ' + combinedDatacard
  
  print 'now running:' #DEBUGGING ONLY
  print combineDatacardsCommand, '\n' #DEBUGGING ONLY
  output = commands.getoutput(combineDatacardsCommand)
  print output # probably keep this in case of datacard syntax problems
  
  #=== run the limits ===
  # this currently uses "ExpSign" from Loukas's code but should be 
  # expanded with the other methods soon
  
  limitsOutputFile = 'limitsOutput_' + sigPoint 
  runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance --pvalue -t 100 --expectSignal=1 -n '+limitsOutputFile
  # note: the default for -t is -1 (Asimov dataset) see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit
  print 'now running:' #DEUBGGING ONLY 
  print runLimitsCommand, '\n' # keep this in some kind of log file?
  output = commands.getoutput(runLimitsCommand)
  print output # maybe redirect this to some kind of log file?

# for sigPoint in sigPoints



