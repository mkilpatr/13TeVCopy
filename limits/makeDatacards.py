#! /usr/bin/python
import os
import sys
import re
import time
from shutil import copy  
from ConfigParser import SafeConfigParser
from collections import OrderedDict
from ROOT import gROOT,TFile,TTree,TH1,TH1D,Double
from math import sqrt
gROOT.SetBatch(True)
TH1.SetDefaultSumw2(True)

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

    dcconfig = DatacardConfig(configFile,configparser)
    dcconfig.produceDatacards()

class DatacardConfig:
    # setup
    def __init__(self,conf_file,config_parser):
        self.conf_file      = conf_file
        config_parser.read(self.conf_file)
        self.treebase       = config_parser.get('config','treelocation')
        self.subdir         = config_parser.get('config','subdir')
        self.datacarddir    = os.path.join( config_parser.get('config','datacarddir'), self.subdir )
        self.setupbase      = config_parser.get('config','setuplocation')
        self.treename       = config_parser.get('config','treename')
        self.weightname     = config_parser.get('config','weightname')
        self.lumiscale      = config_parser.get('config','lumiscale')
        self.filesuffix     = config_parser.get('config','filesuffix')
        self.template       = config_parser.get('config','template')
        self.uncdefinitions = config_parser.get('config','uncertainty_definitions')
        self.uncfileprefix  = config_parser.get('config','uncertainty_fileprefix')
        self.basesel        = config_parser.get('config','basesel')
        self.usedummyuncs   = config_parser.getboolean('config','dummyuncertainties')
        self.has_data       = config_parser.getboolean('config','havedata')
        self.signals        = config_parser.get('signals','samples').replace(' ','').split(',')
        self.backgrounds    = config_parser.get('backgrounds','samples').replace(' ','').split(',')
        self.yieldwidth     = 18
        self.colwidth       = 30
        self.evtwidth       =  7
        self.uncwidth       = 12

        # get expanded binning with all allowed combinations
        self.varnames = []
        self.binedges = []
        for var in config_parser.options('bins') :
            self.varnames.append(var)
            self.binedges.append(config_parser.get('bins',var).replace(' ','').split(','))
        self.allbins = self.expandBins(self.varnames,self.binedges)

        # assign shorthand bin labels to all of them
        self.compactbinlist = []
        self.binmap = {}
        for bin in self.allbins :
            label = 'bin'
            label += self.getBinName(bin,True)
            self.compactbinlist.append(label)
            self.binmap[label] = bin

        # get information about control regions
        self.controlregions = {}
        for cr in config_parser.options('control_regions') :
            crinput  = config_parser.get('control_regions',cr).replace(' ','').split(',')
            datafile = os.path.join( self.treebase, crinput[0] + self.filesuffix )
            mcfile   = os.path.join( self.treebase, crinput[1] + self.filesuffix )
            label    = crinput[0].split('_')[-1]
            selection = crinput[2] if len(crinput) > 2 else self.basesel
            self.controlregions[cr] = ControlRegion(datafile,mcfile,label,selection)

        # load uncertainty files
        if not self.usedummyuncs:
            self.uncvalfiles = [f for f in os.listdir(self.setupbase) if not f.find(self.uncfileprefix) == -1]
            self.uncnames = []
            self.uncertainties = {}
            self.compileUncertainties()

    # Make datacards, one for every bin combination.
    # First the template card is made with all backgrounds,
    # then the signal points are looped to get the sig numbers.
    def produceDatacards(self):
        if os.path.exists(self.datacarddir) :
            print 'WARNING!', self.datacarddir, 'already exists!'
            t = time.localtime()
            moveStr = '_moveTime'+str(t.tm_year)+'-'+str(t.tm_mon)+'-'+str(t.tm_mday)+'-'+str(t.tm_hour*10000+t.tm_min*100+t.tm_sec)
            print 'renaming existing directory to', self.datacarddir+moveStr
            os.rename(self.datacarddir,self.datacarddir+moveStr) 
        os.makedirs(self.datacarddir)
        copy(self.conf_file,self.datacarddir)

        # loop over all bins
        ibin = -1
        for bins in self.allbins :
            print bins
            ibin += 1
            binFileBaseName = self.getFileName(bins)
            binLabel = self.compactbinlist[ibin]
            templateFile = os.path.join( self.datacarddir, 'template'+binFileBaseName+'_template' )

            # first create the template file for this combination of bins
            fdatacard = open( os.path.join(self.setupbase,self.template), 'r' )
            templateDatacard  = fdatacard.read()
            fdatacard.close()

            # compute data yield if applicable
            datayield = 0
            if self.has_data :
                dataFile = os.path.join( self.treebase, 'data' + self.filesuffix )
                datayield = self.getNumEvents(dataFile,bins,True)

            # lines to replace placeholders for current bin's template datacard
            # placeholders are left for signal numbers
            lineSBin     = 'bin'.ljust(self.yieldwidth)         + str(ibin)  
            lineSObs     = 'observation'.ljust(self.yieldwidth) + str(datayield)
            lineSBBin    = 'bin'.ljust(self.yieldwidth)         + str(ibin).ljust(self.yieldwidth)
            lineProcess1 = 'process'.ljust(self.yieldwidth)     + 'susy'.ljust(self.yieldwidth)
            lineProcess2 = 'process'.ljust(self.yieldwidth)     + '0'.ljust(self.yieldwidth)
            lineRate     = 'rate'.ljust(self.yieldwidth)        + 'SIGRATE'

            # loop through backgrounds to get yields
            ibkg = -1
            nBkgEvts = []
            for background in self.backgrounds :
                ibkg+=1
                bkgFile = os.path.join( self.treebase, background+self.filesuffix )
                lineSBBin    += str(ibin).ljust(self.yieldwidth)
                lineProcess1 += background.ljust(self.yieldwidth)
                lineProcess2 += str(ibkg+1).ljust(self.yieldwidth)
                nevts = self.getNumEvents(bkgFile,bins)
                nBkgEvts.append(nevts)
                lineRate     += str(nevts).ljust(self.yieldwidth)

            # fill uncertainties
            # for now ignore differences in systematics for different signal samples
            lineSys = ''
            nUncs = 0
            if self.usedummyuncs :
            # flat uncertainties
                lineSysSig = 'sysSig\tlnN\t' + str(self.getDummyUncertainties('signal',bins)) + '\t'
                nUncs += 1
                ibkg = -1
                lineSysBkg = ''
                for background in self.backgrounds :
                    ibkg += 1
                    lineSysSig += '-\t'
                    lineSysBkg += 'sys'+background+'bin'+str(ibin)+'\tlnN\t-\t'
                    if ibkg > 0 :
                        for ibef in range(ibkg) :
                            lineSysBkg += '-\t'
                    lineSysBkg += str(self.getDummyUncertainties('bkg',bins))+'\t'
                    if ibkg < len(self.backgrounds) :
                        for iaft in range(len(self.backgrounds) - ibkg - 1) :
                            lineSysBkg += '-\t'
                    lineSysBkg += '\n'
                    nUncs += 1
                lineSys += lineSysSig + '\n' + lineSysBkg
            else :
            # fill uncertainties according to values defined in uncertainty config files
                for uncname,unc in self.uncertainties.iteritems() :
                    if not unc.vals.has_key(binLabel) and not unc.vals.has_key('all') :
                        continue
                    uncline = self.getUncertaintyLine(uncname,unc,binLabel,nBkgEvts)
                    lineSys += uncline
                    if not uncline == '' :
                        nUncs += 1

            # put current lines into the placeholders in the template datacard
            templateDatacard = templateDatacard.replace('IMAX'    ,'1'         ) # need to change if more bins are put into one datacard
            templateDatacard = templateDatacard.replace('JMAX'    ,str(len(self.backgrounds))  )
            templateDatacard = templateDatacard.replace('KMAX'    ,str(nUncs)  )
            templateDatacard = templateDatacard.replace('BKGLIST' ,', '.join(self.backgrounds) )
            templateDatacard = templateDatacard.replace('SBIN'    ,lineSBin    )
            templateDatacard = templateDatacard.replace('SOBS'    ,lineSObs    )
            templateDatacard = templateDatacard.replace('SBBIN'   ,lineSBBin   )
            templateDatacard = templateDatacard.replace('PROCESS1',lineProcess1)
            templateDatacard = templateDatacard.replace('PROCESS2',lineProcess2)
            templateDatacard = templateDatacard.replace('RATE'    ,lineRate    )
            templateDatacard = templateDatacard.replace('SYS\n'   ,lineSys     )

            # save the template datacard 
            f = open(templateFile,'w')
            f.write(templateDatacard)
            f.close()

            # now loop through the signal files to get the actual datacards
            for signal in self.signals:
                sigFile = os.path.join( self.treebase, signal+self.filesuffix )
                nSig    = self.getNumEvents(sigFile,bins)

                # put signal numbers into the placeholders in the template datacard
                fdatacard = open(templateFile,'r')
                datacard  = fdatacard.read()
                fdatacard.close()
                datacard = datacard.replace( 'SIGRATE', str(nSig).ljust(self.yieldwidth) )

                # save the current datacard
                datacardSaveLocation = os.path.join( self.datacarddir, signal )
                if not os.path.exists(datacardSaveLocation) :
                    os.makedirs(datacardSaveLocation)
                datacardName = os.path.join( datacardSaveLocation, signal+binFileBaseName )
                f = open(datacardName,'w')
                f.write(datacard)
                f.close()


    def expandBins(self,varnames,binedges):
        """Expands a list of variable names and corresponding bins to a list of all possible bin combinations. Each entry is a list of the combined variable bins e.g. [['nbtag', 1, 2], ['met', 400, 500]]  
        """
        allBinCombos = [[]]
        
        for ivar in range(len(varnames)) :
            cuts = binedges[ivar]
            tempBins = allBinCombos[:]
            allBinCombos = []
            for bin in tempBins :
                for icut in range(len(cuts)) :
                    cut = [varnames[ivar]]
                    if icut < len(cuts) :
                        cut.append(cuts[icut][:-1]) if cuts[icut][-1:] == '+' else cut.append(cuts[icut])
                    if icut < len(cuts)-1 :
                        cut.append(cuts[icut+1][:-1]) if cuts[icut+1][-1:] == '+' else cut.append(cuts[icut+1])
                    allBinCombos.append(bin[:]+[cut])
        return allBinCombos

    def getCutString(self,bins):
        """Converts list of variables and bins into a cutstring
        """
        cutstr = ''
        for ivar in range(len(bins)) :
            var = bins[ivar][0]
            lowcut = bins[ivar][1]
            cutstr += ' && ' + var + ' >= ' + lowcut
            if len(bins[ivar]) > 2 :
                cutstr += ' && ' + var + ' < ' + bins[ivar][2]
        return cutstr
    

    def getFileName(self,bins):
        """makes the name of the template datacard file from the bins
        """
        name = self.getBinName(bins,False) + '.txt'
        return name
    
    def getNumEvents(self,filename,bins,isdata=False,basestr=''):
        """Returns the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins)
        projectvar = bins[0][0]
        if not isdata :
            cutstr = '('+str(self.lumiscale)+'*'+self.weightname+'*('+cutstr+'))'

        file = TFile(filename)
        tree = file.Get(self.treename)
        htmp = TH1D('htmp','htmp',1,0,10000)
        tree.Project('htmp',projectvar,cutstr)
        rate = htmp.Integral(0,2)

        if isdata :
            return int(rate)
        return max([rate,0])
    
    def getNumEventsError(self,filename,bins,isdata=False,basestr=''):
        """Returns the error on the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins)
        projectvar = bins[0][0]
        if not isdata :
            cutstr = '('+str(self.lumiscale)+'*'+self.weightname+'*('+cutstr+'))'

        file = TFile(filename)
        tree = file.Get(self.treename)
        htmp = TH1D('htmp','htmp',1,0,10000)
        tree.Project('htmp',projectvar,cutstr)
        error = Double()
        rate = htmp.IntegralAndError(0,2,error)

        return ( max([rate,0]), abs(error) )

    def getDummyUncertainties(self,procname,bins):
      """Function to get dummy uncertanties. 
      """
      if procname=='sig' :
          return 1.5
      else :
          return 1.3
      return '-'

    def getBinName(self,bins,compact=False):
        """Get compressed bin label, relevant for uncertainty names
        """
        name = ''
        for bin in bins :
            name += '_'
            if not compact :
                name += bin[0].replace('*','')
            name += bin[1]
        return name

    def compileUncertainties(self):
        """Get list of all uncertainty names and values and samples to which they must be applied
        """
        with open('%s/%s' % (self.setupbase,self.uncdefinitions),'r') as f :
            for line in f :
                if line.startswith('#') or line.startswith('%') or line.strip()=='' :
                    continue
                line = line.strip().rstrip('\n')
                entries = re.split('\s+',line)
                name = entries[0]
                type = entries[1]
                if name.find('$BIN') == -1 :
                    if self.uncertainties.has_key(name) :
                        continue
                    self.uncertainties[name] = Uncertainty(name,type)
                    self.uncnames.append(name)
                else :
                    for binlabel in self.compactbinlist :
                        binuncname = name
                        binuncname = binuncname.replace('$BIN',binlabel)
                        if self.uncertainties.has_key(binuncname) :
                            continue
                        self.uncertainties[binuncname] = Uncertainty(binuncname,type)
                        self.uncnames.append(binuncname)
        self.fillUncertaintyValues()
        # printout for debugging
        print 'printing uncertanties by bin: uncName('+'type ::: '+'cr_nevts :: '+'vals) [-1 means it will be calculated later]'
        for k in sorted(self.uncertainties.keys()):
            unc = self.uncertainties[k]
            printStr = unc.name + '(' + unc.type + ' ::: '
            for k in unc.cr_nevts.keys():
                printStr += str(k)+':'+str(unc.cr_nevts[k])+','
            printStr += ' ::: '
            for k in unc.vals.keys(): 
                printStr += str(k)+':'+str(unc.vals[k])+','
            printStr += ')'
            print  printStr 

    def fillUncertaintyValues(self):
        """Get values of each designated uncertainty
        """
        for file in self.uncvalfiles :
            with open('%s/%s' % (self.setupbase,file),'r') as f :
                for line in f :
                    if line.startswith('#') or line.startswith('%') or line.strip()=='' :
                        continue
                    line = line.strip().rstrip('\n')
                    entries = re.split('\s+',line)
                    binnames = [entries[0]] if entries[0] == 'all' else self.compactbinlist[:] 
                    uncnames = [entries[1]] if entries[0] == 'all' else [ entries[1]+'_'+bin for bin in self.compactbinlist ]
                    uncVal   = None
                    if len(entries)>=4:
                        uncVal = entries[3]
                    for i in range(len(binnames)):
                        binname = binnames[i]
                        uncname = uncnames[i]
                        if not self.uncertainties.has_key(uncname) :
                            print 'Didn\'t find uncertainty ',uncname
                            continue
                        unc = self.uncertainties[uncname]
                        samples = entries[2].split(',')
                        for samp in samples :
                            if samp == 'signal' or samp in self.signals or samp in self.backgrounds :
                                if not unc.vals.has_key(binname) :
                                    unc.vals[binname] = {}
                                if unc.type == 'gmN' :
                                    crname = uncname.split('_')[0]
                                    if not self.controlregions.has_key(crname) :
                                        continue
                                    cr = self.controlregions[crname]
                                    crnevts = 0
                                    if self.has_data :
                                        crnevts = self.getNumEvents(cr.datafile, self.binmap[binname], True, cr.selection)
                                    else :
                                        crnevts = int(self.getNumEvents(cr.datafile, self.binmap[binname], False, cr.selection)) 
                                    #change to cr_nmcevets TODO
                                    if not unc.cr_nevts.has_key(binname):
                                        unc.cr_nevts[binname] = {}
                                    unc.cr_nevts[binname]['data'] = max(crnevts,1)
                                    unc.cr_nevts[binname]['mc']   = max( 1, self.getNumEvents(cr.mcfile, self.binmap[binname], False, cr.selection) )
                                    unc.vals[binname][samp] = -1
                                else :
                                    if binname=='all' or uncVal:
                                        unc.vals[binname][samp] = float(uncVal)
                                    else: 
                                      srfile = os.path.join( self.treebase, samp             +self.filesuffix )
                                      (srevts,srunc) = self.getNumEventsError(srfile   , self.binmap[binname], False )
                                      (crevts,crunc) = self.getNumEventsError(cr.mcfile, self.binmap[binname], False, cr.selection)
                                      if crevts==0:
                                          unc.vals[binname][samp] = 1.00
                                      elif srevts==0:
                                           unc.vals[binname][samp] = 2.00
                                      else:
                                          unc.vals[binname][samp] = 1 + sqrt( (srunc/srevts)**2 + (crunc/crevts)**2 )
                                       
                                    

    def getUncertaintyLine(self,uncname,unc,binlabel,nbkgevts):
        """Get line with uncertainty name, type, and values correctly formatted
        """
        isglobal = unc.vals.has_key('all')
        uncline = uncname.ljust(self.colwidth)
        uncline += unc.type + ' '
        if unc.type == 'gmN' :
            if not unc.cr_nevts.has_key(binlabel) :
                print 'Control region yields not loaded for ',uncname
                return ''
            uncline += str(unc.cr_nevts[binlabel]['data']).ljust(self.evtwidth)
        else: 
            uncline += ' '*self.evtwidth 
        # susy uncert
        if not isglobal and unc.vals[binlabel].has_key('signal') : 
            uncline += ('%4.2f' % unc.vals[binlabel]['signal']).ljust(self.uncwidth)
        elif isglobal and unc.vals['all'].has_key('signal') :
            uncline += ('%4.2f' % unc.vals['all']['signal']).ljust(self.uncwidth)
        else :
            uncline += '-'.ljust(self.uncwidth)
        ibkg = -1
        # background uncerts
        for background in self.backgrounds :
            ibkg += 1
            if not isglobal and unc.vals[binlabel].has_key(background) : 
                if unc.type == 'gmN' :
                    uncline += ('%6.5f' % float(nbkgevts[ibkg]/float(unc.cr_nevts[binlabel]['mc']))).ljust(self.uncwidth)
                else :
                    uncline += ('%4.2f' % unc.vals[binlabel][background]).ljust(self.uncwidth)                        
            elif isglobal and unc.vals['all'].has_key(background) :
                uncline += ('%4.2f' % unc.vals['all'][background]).ljust(self.uncwidth)
            else :
                uncline += '-'.ljust(self.uncwidth)
        uncline += '\n'
        return uncline

class ControlRegion:
    def __init__(self,datafile,mcfile,label,selection):
        self.datafile  = datafile
        self.mcfile    = mcfile
        self.label     = label
        self.selection = selection

class Uncertainty:
    def __init__(self,name,type):
        self.name = name
        self.type = type
        self.cr_nevts = {}
        self.vals = {}


if __name__=='__main__': main()
