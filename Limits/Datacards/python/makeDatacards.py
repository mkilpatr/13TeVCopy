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
        self.binmapfile     = config_parser.get('config','crtosr_bin_map')
        self.basesel        = config_parser.get('config','basesel')
        self.usedummyuncs   = config_parser.getboolean('config','dummyuncertainties')
        self.printuncs      = config_parser.getboolean('config','printuncertainties')
        self.has_data       = config_parser.getboolean('config','havedata')
        self.blind_sr       = config_parser.getboolean('config','blindsr')
        self.saveoverwrites = config_parser.getboolean('config','saveoverwrites')
        self.signals        = config_parser.get('signals','samples').replace(' ','').split(',')
        self.backgrounds    = config_parser.get('backgrounds','samples').replace(' ','').split(',')
        self.yieldwidth     = 20
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

        # define signal region
        self.signalregion = FitRegion('data',self.backgrounds,'sr',self.weightname,self.basesel,'signal',self.allbins,self.compactbinlist,self.binmap)

        # fill mapping of signal region bins to control region bins
        self.srtocrbinmaps = {}
        self.fillBinMaps()

        # get information about control regions
        self.extcontrolregions = {}
        self.fitcontrolregions = {}
        for cr in config_parser.options('control_regions') :
            crinput  = config_parser.get('control_regions',cr).replace(' ','').split(';')
            label    = crinput[1].split('_')[-1]
            wgtexpr  = crinput[3] if len(crinput) > 3 else self.weightname
            selection = crinput[4] if len(crinput) > 4 else self.basesel
            crvarnames = []
            crbinedges = []
            for var in config_parser.options(cr+'_bins') :
                crvarnames.append(var)
                crbinedges.append(config_parser.get(cr+'_bins',var).replace(' ','').split(','))
            crallbins = self.expandBins(crvarnames,crbinedges)
            crcompactbinlist = []
            crbinmap = {}
            for bin in crallbins :
                crbinlabel = 'bin_' + label
                crbinlabel += self.getBinName(bin,True)
                crcompactbinlist.append(crbinlabel)
                crbinmap[crbinlabel] = bin
            if crinput[0] == 'ext' :
                self.extcontrolregions[cr] = FitRegion(crinput[1],crinput[2].split(','),label,wgtexpr,selection,'external',crallbins,crcompactbinlist,crbinmap,self.srtocrbinmaps[label])
            else :
                self.fitcontrolregions[cr] = FitRegion(crinput[1],crinput[2].split(','),label,wgtexpr,selection,'control',crallbins,crcompactbinlist,crbinmap,self.srtocrbinmaps[label])

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
            if self.saveoverwrites :
                t = time.localtime()
                moveStr = '_moveTime'+str(t.tm_year)+'-'+str(t.tm_mon)+'-'+str(t.tm_mday)+'-'+str(t.tm_hour*10000+t.tm_min*100+t.tm_sec)
                print 'renaming existing directory to', self.datacarddir+moveStr
                os.rename(self.datacarddir,self.datacarddir+moveStr) 
                os.makedirs(self.datacarddir)
            else :
                print 'will be overwritten' 
                os.popen('rm -rf '+self.datacarddir+'/*')
        else :
            os.makedirs(self.datacarddir)
        copy(self.conf_file,self.datacarddir)
        # write datacards for control regions
        for cr in self.fitcontrolregions.values() :
            self.writeDatacards(cr)
        # write datacards for signal region
        self.writeDatacards(self.signalregion)

    def writeDatacards(self,fitregion):
        """Write out datacards for given fit region, one per for each defined bin and signal point
        """
        print 'making datacards for ',fitregion.type,' region ',fitregion.label
        # loop over all bins
        ibin = -1
        for bins in fitregion.allbins :
            print bins
            ibin += 1
            binFileBaseName = self.getFileName(bins)
            if fitregion.type == 'control' :
                binFileBaseName = '_' + fitregion.label + binFileBaseName
            binLabel = fitregion.compactbinlist[ibin]
            templateFile = os.path.join( self.datacarddir, 'template'+binFileBaseName+'_'+fitregion.label+'_template' )

            # first create the template file for this combination of bins
            fdatacard = open( os.path.join(self.setupbase,self.template), 'r' )
            templateDatacard  = fdatacard.read()
            fdatacard.close()

            # compute data yield if applicable
            datayield = 0
            if self.has_data and not self.blind_sr :
                dataFile = os.path.join( self.treebase, fitregion.dataname + self.filesuffix )
                datayield = self.getNumEvents(dataFile,bins,self.weightname,True)

            # lines to replace placeholders for current bin's template datacard
            # placeholders are left for signal numbers
            lineSBin     = 'bin'.ljust(self.yieldwidth)         + str(ibin)  
            lineSObs     = 'observation'.ljust(self.yieldwidth) + str(datayield)
            lineSBBin    = 'bin'.ljust(self.yieldwidth)         + str(ibin).ljust(self.yieldwidth)
            lineProcess1 = 'process'.ljust(self.yieldwidth)     + 'signal'.ljust(self.yieldwidth)
            lineProcess2 = 'process'.ljust(self.yieldwidth)     + '0'.ljust(self.yieldwidth)
            lineRate     = 'rate'.ljust(self.yieldwidth)        + 'SIGRATE'

            # loop through backgrounds to get yields
            ibkg = -1
            nBkgEvts = []
            for background in fitregion.backgrounds :
                ibkg+=1
                bkgFile = os.path.join( self.treebase, background+self.filesuffix )
                lineSBBin    += str(ibin).ljust(self.yieldwidth)
                if fitregion.type == 'control' :
                    background = background.split('_')[0]
                lineProcess1 += background.ljust(self.yieldwidth)
                lineProcess2 += str(ibkg+1).ljust(self.yieldwidth)
                nevts = self.getNumEvents(bkgFile, bins, fitregion.weight, False, fitregion.selection)
                for frname,fr in self.extcontrolregions.iteritems() :
                    if background == fr.backgrounds[0] :
                         for uncname,unc in self.uncertainties.iteritems() :
                             if uncname.split('_')[0] == frname and unc.type == 'gmN' :
                                 nevts *= float(unc.cr_nevts[binLabel]['data']/unc.cr_nevts[binLabel]['mc'])
                                 if unc.cr_nevts[binLabel]['mcsub'] > 0.0 :
                                     nevts *= (1.0 - unc.cr_nevts[binLabel]['mcsub']/float(unc.cr_nevts[binLabel]['data']))
                                 break
                         break
                nBkgEvts.append(nevts)
                lineRate     += str(nevts).ljust(self.yieldwidth)

            # fill uncertainties
            # for now ignore differences in systematics for different signal samples
            lineSys = ''
            nUncs = 0
            if self.usedummyuncs or not self.uncertainties :
            # flat uncertainties
                lineSysSig = 'sysSig\tlnN\t' + str(self.getDummyUncertainties('signal',bins)) + '\t'
                nUncs += 1
                ibkg = -1
                lineSysBkg = ''
                for background in fitregion.backgrounds :
                    if fitregion.type == 'control' :
                        background = background.split('_')[0]
                    ibkg += 1
                    lineSysSig += '-\t'
                    lineSysBkg += 'sys'+background+'bin'+str(ibin)+'\tlnN\t-\t'
                    if ibkg > 0 :
                        for ibef in range(ibkg) :
                            lineSysBkg += '-\t'
                    lineSysBkg += str(self.getDummyUncertainties('bkg',bins))+'\t'
                    if ibkg < len(fitregion.backgrounds) :
                        for iaft in range(len(fitregion.backgrounds) - ibkg - 1) :
                            lineSysBkg += '-\t'
                    lineSysBkg += '\n'
                    nUncs += 1
                lineSys += lineSysSig + '\n' + lineSysBkg
            else :
            # fill uncertainties according to values defined in uncertainty config files
                for uncname,unc in self.uncertainties.iteritems() :
                    if not unc.vals.has_key(binLabel) and not unc.vals.has_key('all') :
                        continue
                    backgrounds = [b.split('_')[0] if fitregion.type=='control' else b for b in fitregion.backgrounds]
                    uncline = self.getUncertaintyLine(uncname,unc,binLabel,backgrounds,nBkgEvts)
                    lineSys += uncline
                    if not uncline == '' :
                        nUncs += 1

            # put current lines into the placeholders in the template datacard
            templateDatacard = templateDatacard.replace('IMAX'    ,'1'         ) # need to change if more bins are put into one datacard
            templateDatacard = templateDatacard.replace('JMAX'    ,str(len(fitregion.backgrounds))  )
            templateDatacard = templateDatacard.replace('KMAX'    ,str(nUncs)  )
            templateDatacard = templateDatacard.replace('BKGLIST' ,', '.join([b.split('_')[0] if fitregion.type=='control' else b for b in fitregion.backgrounds]) )
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
                nSig    = self.getNumEvents(sigFile, bins, fitregion.weight, False, fitregion.selection)

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
        """Makes the name of the template datacard file from the bins
        """
        name = self.getBinName(bins,False) + '.txt'
        return name
    
    def getNumEvents(self,filename,bins,wgtexpr,isdata=False,basestr=''):
        """Returns the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins)
        projectvar = bins[0][0]
        if not isdata :
            cutstr = '('+str(self.lumiscale)+'*'+wgtexpr+'*('+cutstr+'))'

        file = TFile(filename)
        tree = file.Get(self.treename)
        htmp = TH1D('htmp','htmp',1,0,10000)
        tree.Project('htmp',projectvar,cutstr)
        rate = htmp.Integral(0,2)

        if isdata :
            return int(rate)
        return max([rate,0])
    
    def getNumEventsError(self,filename,bins,wgtexpr,isdata=False,basestr=''):
        """Returns the error on the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins)
        projectvar = bins[0][0]
        if not isdata :
            cutstr = '('+str(self.lumiscale)+'*'+wgtexpr+'*('+cutstr+'))'

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

    def fillBinMaps(self):
        """Fill maps from signal region to control region bins for transfer factors
        """
        with open('%s/%s' % (self.setupbase,self.binmapfile),'r') as f :
            for line in f :
                if line.startswith('#') or line.startswith('%') or line.strip()=='' :
                    continue;
                line = line.strip().rstrip('\n')
                entries = re.split('\s+:\s+',line)
                srbins = entries[0]
                crbins = entries[1]
                crlabel = entries[1].split('_')[1]
                if not self.srtocrbinmaps.has_key(crlabel) :
                    self.srtocrbinmaps[crlabel] = {}
                self.srtocrbinmaps[crlabel][srbins] = crbins


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
                    binlist = self.compactbinlist[:]
                    crname = name.split('_')[0]
                    if not crname in self.extcontrolregions.keys() :
                        for cr in self.fitcontrolregions.values() :
                            binlist += cr.compactbinlist
                    for binlabel in binlist :
                        uncname = name
                        uncname = uncname.replace('$BIN',binlabel)
                        if name.find('$BKG') == -1 :
                            if self.uncertainties.has_key(name) :
                                continue
                            self.uncertainties[uncname] = Uncertainty(uncname,type)
                            self.uncnames.append(uncname)
                        else :
                            for sample in self.backgrounds :
                                sampuncname = uncname
                                sampuncname = sampuncname.replace('$BKG',sample)
                                if self.uncertainties.has_key(name) :
                                    continue
                                self.uncertainties[sampuncname] = Uncertainty(sampuncname,type)
                                self.uncnames.append(sampuncname)
        self.fillUncertaintyValues()
        if self.printuncs :
            # printout for debugging
            print 'printing uncertanties by bin: uncName('+'type ::: '+'cr_nevts :: '+'vals) [-1 means it will be calculated later]'
            for k in sorted(self.uncertainties.keys()):
                print self.uncertainties[k]

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
                    binnames = []
                    uncnames = []
                    if not entries[0] == 'perbin' :
                        binnames = [entries[0]]
                        uncnames = [entries[1]]
                    else :
                        binlist = self.compactbinlist[:]
                        for cr in self.fitcontrolregions.values() :
                            binlist += cr.compactbinlist
                        binnames = binlist[:]
                        uncnames = [ entries[1]+'_'+bin for bin in binlist ]
                    uncVal   = None
                    if len(entries)>=4:
                        uncVal = entries[3]
                    for i in range(len(binnames)):
                        binname = binnames[i]
                        uncname = uncnames[i]
                        if not self.uncertainties.has_key(uncname) :
                            if self.printuncs :
                                print 'Didn\'t find uncertainty ',uncname
                            continue
                        if len(uncname) > self.colwidth :
                            self.colwidth = len(uncname) + 5
                        unc = self.uncertainties[uncname]
                        samples = entries[2].split(',')
                        for samp in samples :
                            if samp == 'signal' or samp in self.signals or samp in self.backgrounds :
                                if not unc.vals.has_key(binname) :
                                    unc.vals[binname] = {}
                                if unc.type == 'gmN' :
                                    crname = uncname.split('_')[0]
                                    if not self.extcontrolregions.has_key(crname) :
                                        continue
                                    if not self.binmap.has_key(binname) :
                                        continue
                                    cr = self.extcontrolregions[crname]
                                    crnevts = 0
                                    datafile = os.path.join( self.treebase, cr.dataname + self.filesuffix )
                                    mcfileadd= os.path.join( self.treebase, cr.backgrounds[0] + self.filesuffix )
                                    mcfilesub= os.path.join( self.treebase, cr.backgrounds[1] + self.filesuffix ) if len(cr.backgrounds) > 1 else ''
                                    srbin = self.binmap[binname]
                                    crbin = cr.binmap[cr.srtocrbinmap[binname]]
                                    if self.has_data :
                                        crnevts = self.getNumEvents(datafile, crbin, cr.weight, True, cr.selection)
                                    else :
                                        crnevts = int(self.getNumEvents(datafile, crbin, cr.weight, False, cr.selection)) 
                                    if not unc.cr_nevts.has_key(binname):
                                        unc.cr_nevts[binname] = {}
                                    unc.cr_nevts[binname]['data'] = max(crnevts, 1)
                                    unc.cr_nevts[binname]['mc']   = max(self.getNumEvents(mcfileadd, crbin, cr.weight, False, cr.selection), 0.00000001)
                                    unc.cr_nevts[binname]['mcsub'] = max(self.getNumEvents(mcfilesub, crbin, cr.weight, False, cr.selection), 0.00000001) if mcfilesub != '' else 0.0
                                    unc.vals[binname][samp] = -1
                                    unc.label = uncname.replace(binname,cr.srtocrbinmap[binname])
                                else :
                                    if binname=='all' or uncVal:
                                        unc.vals[binname][samp] = float(uncVal)
                                    else: 
                                        crname = uncname.split('_')[0]
                                        if self.extcontrolregions.has_key(crname) :
                                            if not self.binmap.has_key(binname) :
                                                continue
                                            cr = self.extcontrolregions[crname]
                                            crfileadd = os.path.join( self.treebase, cr.backgrounds[0]+self.filesuffix )
                                            # uncertainty on subtraction of other backgrounds needs to be put in separately
                                            #crfilesub = os.path.join( self.treebase, cr.backgrounds[1]+self.filesuffix ) if len(cr.backgrounds) > 1 else ''
                                            srfile = os.path.join( self.treebase, samp             +self.filesuffix )
                                            srbin = self.binmap[binname]
                                            crbin = cr.binmap[cr.srtocrbinmap[binname]]
                                            (srevts,srunc) = self.getNumEventsError(srfile, srbin, cr.weight, False )
                                            (crevts,crunc) = self.getNumEventsError(crfileadd, crbin, cr.weight, False, cr.selection)
                                            #if crfilesub != '' :
                                            #    (crsubevts,crsubunc) = self.getNumEventsError(crfilesub, crbin, cr.weight, False, cr.selection)
                                            #    crunc *= crunc
                                            #    crunc += crsubunc*crsubunc*crevts*crevts/(crsubevts*crsubevts)
                                            #    crunc = sqrt(crunc)
                                            crstatuncname = uncname.replace(binname,cr.srtocrbinmap[binname])
                                            if not self.uncertainties.has_key(crstatuncname) :
                                                self.uncertainties[crstatuncname] = Uncertainty(crstatuncname,unc.type)
                                                self.uncnames.append(crstatuncname)
                                            if not self.uncertainties[crstatuncname].vals.has_key(binname) :
                                                self.uncertainties[crstatuncname].vals[binname] = {}
                                            self.uncertainties[crstatuncname].vals[binname][samp] = 1 + (crunc/crevts) if crevts > 0 else 2.00
                                            if srevts==0:
                                                unc.vals[binname][samp] = 1.00
                                            else:
                                                unc.vals[binname][samp] = 1 + (srunc/srevts)
                                        else :
                                            mcfile = os.path.join( self.treebase, samp+self.filesuffix )
                                            selection = self.basesel
                                            bin = None
                                            if not binname.split('_')[1].isdigit() :
                                                cr = None
                                                for fcr in self.fitcontrolregions.values() :
                                                    if fcr.label == binname.split('_')[1] :
                                                        cr = fcr
                                                        selection = cr.selection
                                                        bin = cr.binmap[binname]
                                                        break
                                                sampname = '_'.join([samp,cr.label])
                                                if not sampname in cr.backgrounds :
                                                    continue
                                                mcfile = mcfile.replace(samp, sampname)
                                            else :
                                                bin = self.binmap[binname]
                                            (mcevts,mcunc) = self.getNumEventsError(mcfile, bin, cr.weight, False, selection)
                                            if not mcevts==0 :
                                                unc.vals[binname][samp] = 1 + (mcunc/mcevts)

                                    

    def getUncertaintyLine(self,uncname,unc,binlabel,backgrounds,nbkgevts):
        """Get line with uncertainty name, type, and values correctly formatted
        """
        isglobal = unc.vals.has_key('all')
        uncline = unc.label.ljust(self.colwidth)
        uncline += unc.type + ' '
        # don't count uncertainties with no entries for any of the processes
        hasEntry = False
        if unc.type == 'gmN' :
            if not unc.cr_nevts.has_key(binlabel) :
                print 'Control region yields not loaded for ',uncname
                return ''
            uncline += str(unc.cr_nevts[binlabel]['data']).ljust(self.evtwidth)
            hasEntry = True
        else: 
            uncline += ' '*self.evtwidth 
        if not isglobal and unc.vals[binlabel].has_key('signal') : 
            uncline += ('%4.2f' % unc.vals[binlabel]['signal']).ljust(self.uncwidth)
            hasEntry = True
        elif isglobal and unc.vals['all'].has_key('signal') :
            uncline += ('%4.2f' % unc.vals['all']['signal']).ljust(self.uncwidth)
            hasEntry = True
        else :
            uncline += '-'.ljust(self.uncwidth)
        ibkg = -1
        # background uncerts
        for background in backgrounds :
            ibkg += 1
            if not isglobal and unc.vals[binlabel].has_key(background) : 
                if unc.type == 'gmN' :
                    uncline += ('%6.5f' % float(nbkgevts[ibkg]/float(unc.cr_nevts[binlabel]['data']))).ljust(self.uncwidth)
                else :
                    uncline += ('%4.2f' % unc.vals[binlabel][background]).ljust(self.uncwidth)                        
                hasEntry = True
            elif isglobal and unc.vals['all'].has_key(background) :
                uncline += ('%4.2f' % unc.vals['all'][background]).ljust(self.uncwidth)
                hasEntry = True
            else :
                uncline += '-'.ljust(self.uncwidth)
        uncline += '\n'
        if not hasEntry :
            return ''
        return uncline

class FitRegion:
    def __init__(self,dataname,backgrounds,label,weight,selection,type,allbins,compactbinlist,binmap,srtocrbinmap={}):
        self.dataname       = dataname
        self.backgrounds    = backgrounds
        self.label          = label
        self.weight         = weight
        self.selection      = selection
        self.type           = type
        self.allbins        = allbins
        self.compactbinlist = compactbinlist
        self.binmap         = binmap
        self.srtocrbinmap   = srtocrbinmap

class Uncertainty:
    def __init__(self,name,type):
        self.name = name
        self.type = type
        self.label = name
        self.cr_nevts = {}
        self.vals = {}
    def __str__(self):
        printStr = self.name + '(' + self.type + ' ::: '
        for k in self.cr_nevts.keys():
            printStr += str(k)+':'+str(self.cr_nevts[k])+','
        printStr += ' ::: '
        for k in self.vals.keys(): 
            printStr += str(k)+':'+str(self.vals[k])+','
        printStr += ')'
        return printStr 


if __name__=='__main__': main()
