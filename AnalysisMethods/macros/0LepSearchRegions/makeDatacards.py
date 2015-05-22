#! /usr/bin/python

import os
import commands
  
# user defined stuff
baselineSelection = 'ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1'
runDate = 'testing' # subfolder to put datacards in under <saveLocation>/
sigPoints = ['T2tt_850_100','T2tt_650_325','T2tt_500_325']
backgrounds = ['ttbar','ttZ','znunu']
fileNameTail = '_tree.root'
backgroundFileName = 'bkg_tree.root'
ttreeLocation = 'ttrees/150501/'
saveLocation = 'datacards/'+runDate+'/'

# met bins will be printed in each datacard (MUST be seperate from other bins)
# all bins will have one datacard per bin combination 
varBins = [['ptMet',(200,300),(300,400),(400,999999)],
           ['nmBtag',(1,2),(2,100)],
           ['dPhiMET12',(0,1),(1,4)],          
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
  """Returns the number of events in the given file for the given bin. All the 
  necessary formatting to get the proper cut string for root is done here. This includes
  the baselineSelection defined by the user and the cuts to get the passed bin.
  """
  binCuts = baselineSelection
  for i in range(len(bin)):
    binCuts += '&&'+bin[i][0]+'>='+str(bin[i][1]) + '&&'+bin[i][0]+'<'+str(bin[i][2])
  binCuts = '(scaleFactor)*('+binCuts+')'
  rootCommand = 'root -l -b -q "getBinNumbers.C( \\"'+filename+'\\" , \\"'+binCuts+'\\" )"'
  output = commands.getoutput(rootCommand)
  output = output.split('\n')[-1]
  output = output.replace('(double)','')
  numEvents = float(output)
  return numEvents
  
def getTemplateFileName(bins):
  """creates the name of the template datacard file from the bins
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

saveDir = 'datacards/'+runDate+'/'
if not os.path.exists(saveDir): os.makedirs(saveDir)

# expand variable bins to a list of all combiations using [variable, low, high]
# e.g. (other): [['ptMet', 200, 300], ['nmBtag', 1, 2], ['dPhiMET12', 0, 1]]
allBinCombos = expandBins(varBins)[:]

# make datacards, one for every bin combination,
# if the template datacard does not exist, make that first
for bins in allBinCombos:
  print bins #DEBUGGING ONLY

  #=== first create the template file for this set of bins
  binFileBaseName = getTemplateFileName(bins)
  templateFile = saveLocation + 'template'+binFileBaseName+'_template'
  fdatacard = open('datacard.txt_template','r')
  templateDatacard  = fdatacard.read()
  fdatacard.close()
  
  #lines to replace placeholders for current bin template datacard
  #placeholders are left for signal numbers
  lineSBin     = 'bin\t\t'       + 'METs\t'  
  lineSObs     = 'observation\t' + 'SIGOBS'
  lineSBBin    = 'bin\t'         + 'METs\t\t'
  lineProcess1 = 'process\t'     + 'susy\t\t'
  lineProcess2 = 'process\t'     + '0\t\t'
  lineRate     = 'rate\t'        + 'SIGRATE'
  lineSysSig   = 'tosSysS lnN\t' + 'SIGSYS'
  lineSysBkg   = 'tosSysB lnN\t' + '-\t'
  
  #loop through background inputs ##########TODO#################
  for background in backgrounds:
    bkgFile = ttreeLocation + background + fileNameTail
    lineSBBin    += 'METb\t\t'
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
    
    #put signal numbers into the placeholders in the template datacard for this set of bins
    fdatacard = open(templateFile,'r')
    datacard  = fdatacard.read()
    fdatacard.close()
    datacard = datacard.replace('SIGOBS' ,str(nSigObs)+'\t'  )
    datacard = datacard.replace('SIGRATE',str(nSig   )+'\t')
    datacard = datacard.replace('SIGSYS' ,str(uncSig )+'\t'  )
    
    #save the current datacard
    datacardName = saveLocation + sigPoint+binFileBaseName
    f = open(datacardName,'w')
    f.write(datacard)
    f.close()
    
    print '\n', datacardName, '\n', '='*60 #DEBUGGING ONLY
    print datacard #DEBUGGING ONLY
    #break #DEBUGGING ONLY
  #for sigPoint in sigPoints
  #break
#for bins in allBinCombo

