#! /usr/bin/python
#
# Note: if you want to copy this to a different location to run, you will also need 
# to copy getLimitNumbers to the same place 

import os
import commands

# ===== user defined stuff =====

sigPoints = ['T2tt_850_100','T2tt_650_325','T1tttt_1200_800','T1tttt_1500_100'] 
rootFileLocation = 'limitRootFiles/150608_defaultLepMVAvetos-binNCTTstd/' # location of the output files from runLimits ('outputLocation' from runLimits.py)

##### ##### ##### ##### ##### ##### ##### 
##### you should not need to touch  ##### 
##### anything below this point!!!  ##### 
##### ##### ##### ##### ##### ##### ##### 

rootFiles = os.listdir(rootFileLocation)

for sigPoint in sigPoints:
  rootFile = ''
  for rf in rootFiles:
    if 'higgsCombine' not in rf: continue # this assmes you kept the default name
    if sigPoint in rf: rootFile = rootFileLocation + rf
  if rootFile == '': 
    print "The file for", sigPoint, "doesn't exist! skipping..."
    continue
  #print 'getting mean, rms for', rootFile 
  
  rootCommand = 'root -l -q "getLimitNumbers.C(\\"'+rootFile+'\\")"'
  output = commands.getoutput(rootCommand)
  meanAndError = output.split('\n')[-1]
  print sigPoint, '\t', meanAndError

# for sigPoint in sigPoints



