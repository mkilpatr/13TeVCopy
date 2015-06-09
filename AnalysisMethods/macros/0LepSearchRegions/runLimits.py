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

It currently runs the "ExpSign" mode from Loukas's code (changing '-t -1' to '-t 500'; see 
below).
"""

import os
import commands
import sys 

# to get the subdirectory
args = sys.argv[1:] 
if len(args)<1:
  print "You need to give me a subdirectory! (the same one from makeDatacards.py; also used for printLimits.py)"
  print "$ ./runLimits subdir_from_makeDatacards"
  sys.exit(1)
subdir = args[0]

  
# ===== user defined stuff =====

sigPoints = ['T2tt_850_100','T2tt_650_325','T1tttt_1200_800','T1tttt_1500_100'] 

# you probably don't need to change this unless you want different directory names
saveLocation = 'datacards/'+subdir+'/'  # this should be the same as the one in makeDatacards.py
outputLocation = 'limitRootFiles/'+subdir+'/' # place to move all the output files from running the limits

##### ##### ##### ##### ##### ##### ##### 
##### you should not need to touch  ##### 
##### anything below this point!!!  ##### 
##### ##### ##### ##### ##### ##### ##### 

if not os.path.exists(saveLocation):
  print saveLocation, 'does not exist!' 
  print 'Are you sure', subdir, 'is the one you used in makeDatacards.py?'
  sys.exit(1)


for sigPoint in sigPoints:
  currentFiles = os.listdir('./') # needed to move the output files correctly at the end of each sigPoint 
  
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
  # this currently uses "ExpSign" from Loukas's code 
  
  limitsOutputFile = 'limitsOutput_' + sigPoint 
  runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance -t 500 --expectSignal=1 -n '+limitsOutputFile
  # for now, remove '--pvalue' (between --significance and -t 500)
  # note: the default for -t is -1 (Asimov dataset) see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit
  print 'now running:' #DEUBGGING ONLY 
  print runLimitsCommand, '\n' # keep this in some kind of log file?
  output = commands.getoutput(runLimitsCommand)
  print output # maybe redirect this to some kind of log file?
  
  # move any output files to the correct directory
  allFiles = os.listdir('./')
  if not os.path.exists(outputLocation): os.makedirs(outputLocation)
  print '\n'
  print 'moving files to:', outputLocation
  for f in allFiles:
    if f not in currentFiles:
      print ' '*3, f
      os.rename(f,os.path.join(outputLocation,f)) 
  # for f in allFiles
  
  print '\n', '='*60, '\n'
  

# for sigPoint in sigPoints



