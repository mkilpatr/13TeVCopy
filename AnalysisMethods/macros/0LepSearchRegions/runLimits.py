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

It currently runs HybridNew, ProfileLikelihood, and Asymptotic.
"""

import os
import commands
import sys 
import time 

# to get the subdirectory
args = sys.argv[1:] 
if len(args)<1:
  print "You need to give me a subdirectory! (the same one from makeDatacards.py)"
  print "$ ./runLimits subdir_from_makeDatacards"
  sys.exit(1)
subdir = args[0]

  
# ===== user defined stuff =====

sigPoints = ['T2tt_850_100','T2tt_650_325','T1tttt_1500_100','T1tttt_1200_800'] 

# method choices: HybridNew, ProfileLikelihood, Asymptotic
limitMethod = 'Asymptotic'

# you probably don't need to change this unless you want different directory names
saveLocation = 'datacards/'+subdir+'/'  # this should be the same as the one in makeDatacards.py
outputLocation = 'limitRootFiles/'+subdir+'_'+limitMethod+'/' # place to move all the output files from running the limits

##### ##### ##### ##### ##### ##### ##### 
##### you should not need to touch  ##### 
##### anything below this point!!!  ##### 
##### ##### ##### ##### ##### ##### ##### 

##### ##### ##### ##### ##### #####
#####  define some functions  #####
##### ##### ##### ##### ##### #####

def getLimit(rootFile):
  rootCommand = 'root -l -q "../getLimitNumbers.C(\\"'+rootFile+'\\")"'
  output = commands.getoutput(rootCommand)
  meanAndError = output.split('\n')[-1]
  return meanAndError

##### ##### ##### #####
##### actual work #####
##### ##### ##### #####

if not os.path.exists(saveLocation):
  print saveLocation, 'does not exist!' 
  print 'Are you sure', subdir, 'is the one you used in makeDatacards.py?'
  sys.exit(1)

currentDir = os.getcwd()
significances = [] # store the Significance from each sig point to print at the end

for sigPoint in sigPoints: 
  # create and move into a dummy directory (using the timestamp in the name for uniqueness). At the end of 
  # each sigPoint, all remaining files will be moved to an actual storage location 
  dummyRunDir = 'dummy_' + str(int(time.time()*1e8))
  print 'creating a dummy directory to run in:', dummyRunDir 
  os.makedirs(dummyRunDir)
  os.chdir(dummyRunDir)
    
  datacardSaveLocation = os.path.join(currentDir,saveLocation + sigPoint+'/')
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
  
  # ===== run the limits ===== 
  #
  ### notes from previous versions:
  # runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance -t 500 --expectSignal=1 -n '+limitsOutputFile
  # for now, remove '--pvalue' (between --significance and -t 500)
  # note: the default for -t is -1 (Asimov dataset) see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHiggsAnalysisCombinedLimit
  
  # === Asymptotic
  if limitMethod=='Asymptotic':
    runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance -t -1 --expectSignal=1 -n '+sigPoint
    # run the limit command and figure out what the output root file is
    print 'now running:' #DEUBGGING ONLY 
    print runLimitsCommand, '\n' # keep this in some kind of log file?
    output = commands.getoutput(runLimitsCommand)
    print output # maybe redirect this to some kind of log file?
    
    # pull the Significance out and store
    tempSig = ''
    for line in output.split('\n'): 
      if 'Significance' in line: 
        tempSig = line.replace('Significance',sigPoint) 
    if tempSig=='': 
      significances.append(sigPoint+': no significance found..')
    else:
      significances.append(tempSig)  
  #limitMethod=='Asymptotic'
  
  # === ProfileLikelihood
  elif limitMethod=='ProfileLikelihood':
    runLimitsCommand =  'combine -M ProfileLikelihood '+combinedDatacard+' --significance -t 500 --expectSignal=1 -n '+sigPoint
    # run the limit command and figure out what the output root file is
    print 'now running:' #DEUBGGING ONLY 
    print runLimitsCommand, '\n' # keep this in some kind of log file?
    output = commands.getoutput(runLimitsCommand)
    print output # maybe redirect this to some kind of log file?
    
    # get the significance and store
    rootFile = ''
    dummyFiles = os.listdir('./')
    for df in dummyFiles:
      if 'higgsCombine' in df: rootFile = df
    if rootFile=='':
      significances.append(sigPoint+': no significance found..')
    else:
      significances.append( sigPoint + ': ' + getLimit(rootFile) )
  # limitMethod=='ProfileLikelihood'
  
  # === HybridNew method
  elif limitMethod=='HybridNew':
    # run the first limit command and figure out what the output root file is
    runLimitsCommand =  'combine -M HybridNew --frequentist '+combinedDatacard+' --significance --saveToys --fullBToys --saveHybridResult -T 500 -i 10 -n '+sigPoint
    print 'now running:' #DEUBGGING ONLY 
    print runLimitsCommand, '\n' # keep this in some kind of log file?
    output = commands.getoutput(runLimitsCommand)
    print output # maybe redirect this to some kind of log file?
    # now need to figure out what the root output of the previous file is
    dummyFiles = os.listdir('./')
    outputRootFile = ''
    for f in dummyFiles:
      if 'higgsCombine' in f:
        outputRootFile = f
    # now run the second limit command with the above root file 
    runLimitsCommand =  'combine -M HybridNew --frequentist '+combinedDatacard+' --significance --readHybridResult --toysFile='+outputRootFile+' --expectedFromGrid=0.5 -n '+sigPoint
    print 'now running:' #DEUBGGING ONLY 
    print runLimitsCommand, '\n' # keep this in some kind of log file?
    output = commands.getoutput(runLimitsCommand)
    print output # maybe also redirect this to some kind of log file?
    
    # pull the Significance out and store
    tempSig = ''
    for line in output.split('\n'): 
      if 'Significance' in line: 
        tempSig = line.replace('Significance',sigPoint) 
    if tempSig=='': 
      significances.append(sigPoint+': no significance found..')
    else:
      significances.append(tempSig)  
  #limitMethod=='HybridNew':
  
  # === non-existant choice of limit method
  else:
    print limitMethod, 'is not one of the currently implemented methods! You need to pick one of the allowed ones!'
    sys.exit(1)
  #non-existant limit method
    
  # move any output files to the correct directory
  os.chdir(currentDir)
  dummyFiles = os.listdir(dummyRunDir)
  if not os.path.exists(outputLocation): os.makedirs(outputLocation)
  print '\n'
  print 'moving files to:', outputLocation
  for f in dummyFiles:
    print ' '*3, f
    if 'roostats' in f:
      os.remove(os.path.join(dummyRunDir,f))
      continue
    os.rename(os.path.join(dummyRunDir,f),os.path.join(outputLocation,f)) 
  #for f in dummyFiles
  os.removedirs(dummyRunDir)
  print '\n', '='*60, '\n'
  
#for sigPoint in sigPoints

# print the significances
print '='*5, 'SIGNIFICANCES', '('+limitMethod+')', '='*5
print '\n'.join(significances)
print '\n'


