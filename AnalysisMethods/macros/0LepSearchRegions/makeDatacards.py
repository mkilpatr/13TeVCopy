#! /usr/bin/python
#
# Note: if you want to copy this to a different location to run, you will also need 
# to copy getBinNumbers.C and datacard.txt_template to the same directory.

import os
import commands
  
# ===== user defined stuff =====

baselineSelection = 'ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&NCTTstd>0&&(DphiTopMET>1||DphiTopMET<-1)&&(DphiTopMET<2.8&&DphiTopMET>-2.8)' 
saveLocation = 'datacards/150608_defaultLepMVAvetos-binNCTTstd/' 
ttreeLocation = 'ttrees/150608/'
lumi = 5 # this assumes that the trees are filled with weights that assume 1/fb, so use this to scale as desired

# All filenames need to have the format <sampleName><fileNameTail> e.g. ttbar + _tree.root.
# The list of signal and background samples is used both to get the filenames and 
# to label things in the datacards and datacard names.
sigPoints = ['T2tt_850_100','T2tt_650_325','T1tttt_1200_800','T1tttt_1500_100'] 
backgrounds = ['ttbar','ttZ','znunu','ttW'] # add wjets
fileNameTail = '_tree.root'

# This will produce one datacard per bin combination. 
# Bins must be defined as a list with [ binVarName, bin1, bin2, ... ].
# Bins are tuples (low,high) where low is inclusive and high is exclusive
# such that (1,2) is >=1 && <2. Note that the high limit on the last bin 
# should be absurdly high for inclusive cuts, e.g. >=500 -> (500,100000000000).
varBins = [#['ptMet',(200,300),(300,400),(400,999999)],
           ['nmBtag',(1,2),(2,100)],
           ['MT2tp0_000',(200,300),(300,400),(400,500),(500,600),(600,1000000000)],   
           #['NCTTstd',(0,1),(1,100)],       
           ]

##### ##### ##### ##### ##### ##### ##### 
##### you should not need to touch  ##### 
##### anything below this point!!!  ##### 
##### ##### ##### ##### ##### ##### ##### 


##### ##### ##### ##### ##### ##### 
#####  define some functions  #####
##### ##### ##### ##### ##### ##### 

def expandBins(bins):
  """Expands the bins from the input above to a list of all possible bin combinations. Each
  entry is a list of the variable bins e.g. [['nmBtag', 1, 2], ['dPhiMET12', 0, 1]]  
  """
  allBinCombos = [[]] # 'running' list to hold each iteration of expansion 
  for varBin in bins:
    variable = varBin[0]
    cuts = varBin[1:]
    tempBins = allBinCombos[:] #temporary list to store list from last iteration
    allBinCombos = [] # reset 'running' list to this iteration of expansion 
    for tempBin in tempBins: # loop through current list of bin combinations
      for iCut in cuts: # loop through cuts in the bin to add
        iCut = [variable] + list(iCut)
        allBinCombos.append(tempBin[:]+[iCut])
  return allBinCombos

def getNumEvents(filename,bin): 
  """Returns the number of events in the given file for the given bin. All the necessary 
  formatting to get the proper cut string for root is done here. This includes the
  baselineSelection and lumi sacling defined by the user and the cuts to get the current bin.
  """
  binCuts = baselineSelection
  for i in range(len(bin)):
    binCuts += '&&'+bin[i][0]+'>='+str(bin[i][1]) + '&&'+bin[i][0]+'<'+str(bin[i][2])
  binCuts = '('+str(lumi)+'*scaleFactor)*('+binCuts+')'
  rootCommand = 'root -l -b -q "getBinNumbers.C( \\"'+filename+'\\" , \\"'+binCuts+'\\" )"'
  #print rootCommand #DEBUGGING ONLY
  output = commands.getoutput(rootCommand)
  output = output.split('\n')[-1]  # only look at the last line which should have the number of events
  # note: it's possible the next line may need tweaking if your version of root formats things too differently
  output = output.replace('(double)','') # (double)123.4e+00 -> 123.4e+00
  numEvents = float(output)
  return numEvents
  
def getFileName(bins):
  """makes the name of the template datacard file from the bins
  """
  name = ''
  for bin in bins:
    name += '-' + bin[0] + '-' + str(bin[1])
  name += '.txt'
  return name

def getUncertanties(filename,bins):
  """Dummy function to get the uncertanties. It returns
  static numbers for now. Will need to be filled in later. 
  """
  if filename=='sig': return 1.5
  if filename=='bkg': return 1.3
  return '-'

##### ##### ##### #####  
##### actual work #####
##### ##### ##### #####  

if not os.path.exists(saveLocation): os.makedirs(saveLocation)

# expand variable bins to a list of all combiations using [variable, low, high]
# e.g. (other): [['ptMet', 200, 300], ['nmBtag', 1, 2], ['dPhiMET12', 0, 1]]
allBinCombos = expandBins(varBins)[:]

# Make datacards, one for every bin combination.
# First the template card is made with all backgrounds,
# then the signal points are looped to get the sig numbers.
for bins in allBinCombos:
  #print bins #DEBUGGING ONLY
  binFileBaseName = getFileName(bins)
  templateFile = saveLocation + 'template'+binFileBaseName+'_template'

  #=== first create the template file for this combination of bins
  fdatacard = open('datacard.txt_template','r')
  templateDatacard  = fdatacard.read()
  fdatacard.close()
  
  # lines to replace placeholders for current bin's template datacard
  # placeholders are left for signal numbers
  lineSBin     = 'bin\t\t'       + 'METs\t'  
  lineSObs     = 'observation\t' + 'SIGOBS'
  lineSBBin    = 'bin\t'         + 'METs\t\t'
  lineProcess1 = 'process\t'     + 'susy\t\t'
  lineProcess2 = 'process\t'     + '0\t\t'
  lineRate     = 'rate\t'        + 'SIGRATE'
  lineSysSig   = 'tosSysS lnN\t' + 'SIGSYS'
  lineSysBkg   = 'tosSysB lnN\t' + '-\t'
  
  #loop through backgrounds to get numbers 
  for background in backgrounds:
    bkgFile = ttreeLocation + background + fileNameTail
    lineSBBin    += 'METs\t\t'
    lineProcess1 += background+'\t\t'
    lineProcess2 += '1\t\t'
    lineRate     += str(getNumEvents(bkgFile,bins))+'\t'
    lineSysSig   += '-\t'
    lineSysBkg   += str(getUncertanties('bkg',bins))+'\t'
  
  #put current lines into the placeholders in the template datacard
  templateDatacard = templateDatacard.replace('IMAX'    ,'1'         ) # need to change if more bins are put into one datacard
  templateDatacard = templateDatacard.replace('JMAX'    ,str(len(backgrounds))  )
  templateDatacard = templateDatacard.replace('BKGLIST' ,', '.join(backgrounds) )
  templateDatacard = templateDatacard.replace('SBIN'    ,lineSBin    )
  templateDatacard = templateDatacard.replace('SOBS'    ,lineSObs    )
  templateDatacard = templateDatacard.replace('SBBIN'   ,lineSBBin   )
  templateDatacard = templateDatacard.replace('PROCESS1',lineProcess1)
  templateDatacard = templateDatacard.replace('PROCESS2',lineProcess2)
  templateDatacard = templateDatacard.replace('RATE'    ,lineRate    )
  templateDatacard = templateDatacard.replace('SYSSIG'  ,lineSysSig  )
  templateDatacard = templateDatacard.replace('SYSBKG'  ,lineSysBkg  )
  
  #save the template datacard 
  f = open(templateFile,'w')
  f.write(templateDatacard)
  f.close()

  #=== now loop through the signal files to get the actual datacards
  for sigPoint in sigPoints:
    sigFile = ttreeLocation+sigPoint+fileNameTail
    nSig    = getNumEvents(sigFile,bins)
    nSigObs = int(nSig+0.5)
    uncSig  = getUncertanties('sig',bins)
    
    #put signal numbers into the placeholders in the template datacard
    fdatacard = open(templateFile,'r')
    datacard  = fdatacard.read()
    fdatacard.close()
    datacard = datacard.replace('SIGOBS' ,str(nSigObs)+'\t')
    datacard = datacard.replace('SIGRATE',str(nSig   )+'\t')
    datacard = datacard.replace('SIGSYS' ,str(uncSig )+'\t')
    
    #save the current datacard
    datacardSaveLocation = saveLocation + sigPoint+'/'
    if not os.path.exists(datacardSaveLocation): os.makedirs(datacardSaveLocation)
    datacardName = datacardSaveLocation + sigPoint+binFileBaseName
    f = open(datacardName,'w')
    f.write(datacard)
    f.close()
    
    #print '\n', datacardName, '\n', '='*60 #DEBUGGING ONLY
    #print datacard #DEBUGGING ONLY
  #for sigPoint in sigPoints
  #break
#for bins in allBinCombo

