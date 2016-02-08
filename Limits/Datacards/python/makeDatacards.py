#! /usr/bin/python
import os
import sys
import re
import time
import ast
from shutil import copy  
from ConfigParser import SafeConfigParser
from collections import OrderedDict
from ROOT import gROOT,TFile,TTree,TH1,TH1D,TH2D,Double
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

def natural_sort(list, key=lambda s:s):
    """
    Sort the list into natural alphanumeric order.
    https://stackoverflow.com/questions/4836710/does-python-have-a-built-in-function-for-string-natural-sort
    """
    def get_alphanum_key_func(key):
        convert = lambda text: int(text) if text.isdigit() else text
        return lambda s: [convert(c) for c in re.split('([0-9]+)', key(s))]
    sort_key = get_alphanum_key_func(key)
    list.sort(key=sort_key)

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
        self.scalesigtoacc  = config_parser.getboolean('config','scalesigtoacc')
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

        # combine bins if so desired
        for binlist in config_parser.get('combine_bins','bins').replace(' ','').split(';') :
            combinebins = binlist.replace(' ','').split(',')
            if len(combinebins) > 1 :
                self.combineBins(self.allbins, self.compactbinlist, self.binmap, combinebins)
        

        # define signal region
        self.signalregion = FitRegion('data',self.backgrounds,'sr',self.weightname,self.basesel,self.weightname,self.basesel,'','','','signal',self.allbins,self.compactbinlist,self.binmap)

        # fill mapping of signal region bins to control region bins
        self.srtocrbinmaps = {}
        self.fillBinMaps()

        # get information about control regions
        self.extcontrolregions = {}
        self.fitcontrolregions = {}
        for cr in config_parser.options('control_regions') :
            crinput  = ast.literal_eval(config_parser.get('control_regions',cr))
            label    = crinput['dataFile'].split('_')[-1]
            wgtexpr     = crinput['crwgt'] if crinput.has_key('crwgt') else self.weightname
            selection   = crinput['crsel'] if crinput.has_key('crsel') else self.basesel
            crSubSel    = crinput['crsubsel'] if crinput.has_key('crsubsel') else ''
            subNormSel  = crinput['crnormsel'] if crinput.has_key('crnormsel') else ''
            subNormWgt  = crinput['crnormwgt'] if crinput.has_key('crnormwgt') else ''
            srwgtexpr   = crinput['srwgt'] if crinput.has_key('srwgt') else self.weightname
            srselection = crinput['srsel'] if crinput.has_key('srsel') else self.basesel
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
            for binlist in config_parser.get('combine_bins',cr+'_bins').replace(' ','').split(';') :
                combinebins = binlist.replace(' ','').split(',')
                if len(combinebins) > 1 :
                    self.combineBins(crallbins, crcompactbinlist, crbinmap, combinebins)
        
            if crinput['type'] == 'ext' :
                self.extcontrolregions[cr] = FitRegion(crinput['dataFile'],crinput['mcFiles'],label,wgtexpr,selection,crSubSel,subNormSel,subNormWgt,srwgtexpr,srselection,'external',crallbins,crcompactbinlist,crbinmap,self.srtocrbinmaps[label])
            else :
                self.fitcontrolregions[cr] = FitRegion(crinput['dataFile'],crinput['mcFiles'],label,wgtexpr,selection,crSubSel,subNormSel,subNormWgt,srwgtexpr,srselection,'control',crallbins,crcompactbinlist,crbinmap,self.srtocrbinmaps[label])

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
            print '\nWARNING!', self.datacarddir, 'already exists!'
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
        print '\n~~~Making datacards for ',fitregion.type,' region ',fitregion.label,'~~~'
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

            # lines to replace placeholders for current bin's template datacard
            # placeholders are left for signal numbers
            lineSBin     = 'bin'.ljust(self.yieldwidth)         + str(ibin)  
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
                if fitregion.type == 'control' :
                    background = background.split('_')[0]
                    if ibkg > 0 :
                        continue
                lineSBBin    += str(ibin).ljust(self.yieldwidth)
                lineProcess1 += background.ljust(self.yieldwidth)
                lineProcess2 += str(ibkg+1).ljust(self.yieldwidth)
                nevts = self.getNumEvents(bkgFile, bins, fitregion.weight, 1, fitregion.selection)
                for frname,fr in self.extcontrolregions.iteritems() :
                    if background == fr.backgrounds[2] :
                         for uncname,unc in self.uncertainties.iteritems() :
                             if uncname.split('_')[0] == frname and binLabel in uncname and unc.type == 'gmN' :
                                 nevts = float(unc.cr_nevts[binLabel]['data']*unc.cr_nevts[binLabel]['mcsr']/unc.cr_nevts[binLabel]['mc'])
                                 if unc.cr_nevts[binLabel]['mcsub'] > 0.0 :
                                     nevts *= unc.cr_nevts[binLabel]['mcsub']
                                 break
                         break
                nBkgEvts.append(nevts)
                lineRate     += str(nevts).ljust(self.yieldwidth)

            # compute data yield if applicable
            datayield = 0
            if self.has_data and not (self.blind_sr and fitregion.type == 'signal') :
                dataFile = os.path.join( self.treebase, fitregion.dataname + self.filesuffix )
                datayield = self.getNumEvents(dataFile,bins,fitregion.weight,0,fitregion.selection)
            else :
                datayield = int(sum(nBkgEvts))

            lineSObs     = 'observation'.ljust(self.yieldwidth) + str(datayield)

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
                        if ibkg > -1 :
                            continue
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
                for uncname in sorted(self.uncertainties.keys()) :
                    unc = self.uncertainties[uncname]
                    if not unc.vals.has_key(binLabel) and not unc.vals.has_key('all') :
                        continue
                    backgrounds = [fitregion.backgrounds[0].split('_')[0]] if fitregion.type=='control' else [b for b in fitregion.backgrounds]
                    uncline = self.getUncertaintyLine(uncname,unc,binLabel,backgrounds,nBkgEvts)
                    lineSys += uncline
                    if not uncline == '' :
                        nUncs += 1

            # put current lines into the placeholders in the template datacard
            templateDatacard = templateDatacard.replace('IMAX'    ,'1'         ) # need to change if more bins are put into one datacard
            templateDatacard = templateDatacard.replace('JMAX'    ,str(1 if fitregion.type=='control' else len(fitregion.backgrounds))  )
            templateDatacard = templateDatacard.replace('KMAX'    ,str(nUncs)  )
            templateDatacard = templateDatacard.replace('BKGLIST' ,', '.join([fitregion.backgrounds[0].split('_')[0]] if fitregion.type=='control' else [b for b in fitregion.backgrounds]) )
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
                nSig    = self.getNumEvents(sigFile, bins, fitregion.weight, 2, fitregion.selection)
                if nSig == 0.0 :
                    nSig = 0.00000001

                # put signal numbers into the placeholders in the template datacard
                fdatacard = open(templateFile,'r')
                datacard  = fdatacard.read()
                fdatacard.close()
                datacard = datacard.replace( 'SIGRATE', str(nSig).ljust(self.yieldwidth) )
                dclines = datacard.split('\n')
                sigunclines = []
                for iline in range(len(dclines)) :
                    line = dclines[iline]
                    if '$SIG' in line or 'SIGUNC' in line :
                        uncname = line.split()[0]
                        # any of the following if cases are problematic!
                        if not self.uncertainties.has_key(uncname) :
                            print 'Didn\'t find uncertainty',uncname,'in uncertainty list'
                        elif not self.uncertainties[uncname].vals.has_key(binLabel) :
                            print 'Uncertainty',uncname,'not defined for bin',binLabel
                        elif not self.uncertainties[uncname].vals[binLabel].has_key(signal) :
                            print 'Uncertainty',uncname,'not filled for',signal,'in bin',binLabel
                        else :
                            newline = line.replace('$SIG','signal').replace('SIGUNC','%4.2f' % self.uncertainties[uncname].vals[binLabel][signal])
                            dclines[iline] = newline
                datacard = '\n'.join(dclines)

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


    def combineBins(self, allbins, compactbinlist, binmap, combinebins):
        combinebinlist = []
        for binlabel in combinebins :
            combinebinlist.append(binmap[binlabel])

        varindices = {}
        for index in range(len(combinebinlist[0])) :
            varindices[combinebinlist[0][index][0]] = index

        combinebinedges = {}
        for bins in combinebinlist :
            for ivar in range(len(bins)) :
                var = bins[ivar][0]
                lowedge = bins[ivar][1]
                if not combinebinedges.has_key(var) :
                    combinebinedges[var] = [lowedge]
                if not lowedge in combinebinedges[var] :
                    combinebinedges[var].append(lowedge)
                else :
                    if len(bins[ivar]) == 2 :
                        combinebinedges[var].remove(lowedge)
                if len(bins[ivar]) > 2 :
                    highedge = bins[ivar][2]
                    if not highedge in combinebinedges[var] :
                        combinebinedges[var].append(highedge)
                if len(combinebinedges[var]) == 3 :
                    # need sorting to ensure removing the value in between
                    natural_sort(combinebinedges[var])
                    combinebinedges[var].pop(1)
                if not combinebinedges[var]:
                    # need this if it is the highest bin in "var", but we are not merging in "var"
                    combinebinedges[var] = [lowedge]
        newbins = []
        for var,binedges in combinebinedges.iteritems() :
            binedges.insert(0,var)
            newbins.insert(varindices[var], binedges)

        firstbin = True
        indices_to_remove = []
        for bins in allbins :
            if bins in combinebinlist :
                if firstbin :
                    allbins[allbins.index(bins)] = newbins
                    firstbin = False
                    binlabel = binmap.keys()[binmap.values().index(bins)]
                    binmap[binlabel] = newbins
                else :
                    indices_to_remove.append(allbins.index(bins))
                    binlabel = binmap.keys()[binmap.values().index(bins)]
                    binmap.pop(binlabel)

        for index in sorted(indices_to_remove, reverse=True) :
            allbins.pop(index)
            compactbinlist.pop(index)


    def getCutString(self,bins,strip = -1):
        """Converts list of variables and bins into a cutstring
        """
        cutstr = ''
        for ivar in range(max(0,strip),len(bins)) :
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
    
    def getNumEvents(self,filename,bins,wgtexpr,process=1,basestr='', strip = -1):
        """Returns the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins,strip)
        projectvar = bins[0][0]
        if process > 0 :
            cutstr = '('+str(self.lumiscale)+'*'+wgtexpr+'*('+cutstr+'))'

        file = TFile(filename)
        tree = file.Get(self.treename)
        htmp = TH1D('htmp','htmp',1,0,10000)
        tree.Project('htmp',projectvar,cutstr)
        rate = htmp.Integral(0,2)

        if process == 0 :
            return int(rate)

        if process == 2 and self.scalesigtoacc :
            mstop = int(filename.split('_')[-3])
            xsecfile = TFile('../data/xsecs/stop.root')
            xsechist = TH1D()
            xsechist = xsecfile.Get('xsecs')
            xsec = xsechist.Interpolate(mstop)
            rate = rate/xsec

        return max([rate,0])
    
    def getNumEventsError(self,filename,bins,wgtexpr,process=1,basestr=''):
        """Returns the error on the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel if basestr == '' else basestr
        cutstr += self.getCutString(bins)
        projectvar = bins[0][0]
        if process > 0 :
            cutstr = '('+str(self.lumiscale)+'*'+wgtexpr+'*('+cutstr+'))'

        file = TFile(filename)
        tree = file.Get(self.treename)
        htmp = TH1D('htmp','htmp',1,0,10000)
        tree.Project('htmp',projectvar,cutstr)

        if process == 2 and self.scalesigtoacc :
            mstop = int(filename.split('_')[-3])
            xsecfile = TFile('../data/xsecs/stop.root')
            xsechist = TH2D()
            xsechist = xsecfile.Get('xsecs')
            xsec = xsechist.Interpolate(mstop)
            htmp.Divide(xsec)

        error = Double()
        rate = htmp.IntegralAndError(0,2,error)

        return ( max([rate,0]), abs(error) )

    def getDummyUncertainties(self,procname,bins):
      """Function to get dummy uncertanties. 
      """
      if procname=='sig' :
          return 1.1
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
        print '\n~~~Compiling uncertainty list~~~'
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
        print '\n~~~Filling uncertainty values~~~'
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
            print 'Opening ',file
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
                    samples = entries[2].split(',')
                    if '$SIG' in entries[1] and entries[2] == 'signal' :
                        samples = self.signals
                    uncVal   = None
                    if len(entries)>=4:
                        uncVal = entries[3]
                    cryields_bins_filled = {}
                    for i in range(len(binnames)):
                        binname = binnames[i]
                        uncname = uncnames[i]
                        hasunc  = False
                        if not self.uncertainties.has_key(uncname) :
                            for name,unc in self.uncertainties.iteritems() :
                                if name in uncname :
                                    self.uncertainties[uncname] = Uncertainty(uncname,unc.type)
                                    self.uncnames.append(uncname)
                                    hasunc = True
                                    break
                        else :
                            hasunc = True
                        if not hasunc :
                            if self.printuncs :
                                print 'Didn\'t find uncertainty ',uncname
                            continue
                        if len(uncname) > self.colwidth :
                            self.colwidth = len(uncname) + 5
                        unc = self.uncertainties[uncname]
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
                                    mcfilesr = os.path.join( self.treebase, cr.backgrounds[2] + self.filesuffix )
                                    mcfilesub= os.path.join( self.treebase, cr.backgrounds[1] + self.filesuffix ) if cr.backgrounds[1] != '' else ''
                                    srbin = self.binmap[binname]
                                    crbin = cr.binmap[cr.srtocrbinmap[binname]]
                                    if not unc.cr_nevts.has_key(binname):
                                        unc.cr_nevts[binname] = {}
                                    unc.cr_nevts[binname]['mcsr']  = max(self.getNumEvents(mcfilesr, srbin, cr.orweight, 1, cr.orselection), 0.00000001)
                                    if cryields_bins_filled.has_key(cr.srtocrbinmap[binname]) :
                                        unc.cr_nevts[binname]['data'] = cryields_bins_filled[cr.srtocrbinmap[binname]]['data']
                                        unc.cr_nevts[binname]['mc'] = cryields_bins_filled[cr.srtocrbinmap[binname]]['mc']
                                        unc.cr_nevts[binname]['mcsub'] = cryields_bins_filled[cr.srtocrbinmap[binname]]['mcsub']
                                    else :
                                        if self.has_data :
                                            crnevts = self.getNumEvents(datafile, crbin, cr.weight, 0, cr.selection)
                                        else :
                                            crnevts = int(self.getNumEvents(datafile, crbin, cr.weight, 1, cr.selection)) 
                                        unc.cr_nevts[binname]['data'] = max(crnevts, 1)
                                        unc.cr_nevts[binname]['mc']   = max(self.getNumEvents(mcfileadd, crbin, cr.weight, 1, cr.selection), 0.00000001)
                                        unc.cr_nevts[binname]['mcsub'] = max(self.getNumEvents(mcfilesub, crbin, cr.weight, 1, cr.selection), 0.00000001) if mcfilesub != '' else 0.0
                                        
                                        #Normalize the subtracted MC
                                        if cr.subNormSel != '' and mcfilesub != '':
                                            normData = self.getNumEvents(datafile, crbin, cr.subNormWgt, 0, cr.subNormSel,1)
                                            normSub  =  self.getNumEvents(mcfilesub, crbin, cr.subNormWgt, 1, cr.subNormSel,1)
                                            normMC   =  self.getNumEvents(mcfileadd, crbin, cr.subNormWgt, 1, cr.subNormSel,1)
                                            if normData > 0 and normSub + normMC > 0 :
                                                unc.cr_nevts[binname]['mcsub'] = unc.cr_nevts[binname]['mcsub']* normData/(normSub+normMC)                                        
                                        #Get the subtraction correction
                                        if unc.cr_nevts[binname]['mcsub'] > 0 :
                                            if unc.cr_nevts[binname]['data'] < 10 :
                                                tempMC = self.getNumEvents(mcfileadd, crbin, cr.weight, 1, cr.crSubSel)
                                                unc.cr_nevts[binname]['mcsub'] = (1.0 - (unc.cr_nevts[binname]['mcsub']/(tempMC + unc.cr_nevts[binname]['mcsub'])))
                                            else :                                                
                                                unc.cr_nevts[binname]['mcsub'] = (1.0 - (unc.cr_nevts[binname]['mcsub']/float(unc.cr_nevts[binname]['data'])))                                            
                                                
                                    unc.vals[binname][samp] = -1
                                    unc.label = uncname.replace(binname,cr.srtocrbinmap[binname])
                                    if not cryields_bins_filled.has_key(cr.srtocrbinmap[binname]) : 
                                        cryields_bins_filled[cr.srtocrbinmap[binname]] = unc.cr_nevts[binname]
                                elif unc.type == 'lnU' :
                                    crname = uncname.split('_')[0]
                                    if not self.fitcontrolregions.has_key(crname) :
                                        continue
                                    cr = self.fitcontrolregions[crname]
                                    crbin = cr.binmap[cr.srtocrbinmap[binname]] if self.binmap.has_key(binname) else cr.binmap[binname]
                                    datafile = os.path.join( self.treebase, cr.dataname + self.filesuffix )
                                    crnevts = 0
                                    if self.has_data :
                                        crnevts = self.getNumEvents(datafile, crbin, cr.weight, 0, cr.selection)
                                    else :
                                        crnevts = int(self.getNumEvents(datafile, crbin, cr.weight, 1, cr.selection))
                                    crnevts = max(crnevts, 1)
                                    unc.vals[binname][samp] = float(uncVal) if uncVal else 2
                                    #if not self.has_data or self.blind_sr :
                                    #    unc.vals[binname][samp] = 1 + sqrt(crnevts)/crnevts
                                    #    unc.type = 'lnN'
                                    if self.binmap.has_key(binname) :
                                        unc.label = uncname.replace(binname,cr.srtocrbinmap[binname])
                                    elif not cr.binmap.has_key(binname) :
                                        continue
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
                                            srfile = os.path.join( self.treebase, samp             +self.filesuffix )
                                            srbin = self.binmap[binname]
                                            crbin = cr.binmap[cr.srtocrbinmap[binname]]
                                            (srevts,srunc) = self.getNumEventsError(srfile, srbin, cr.weight, 1 )
                                            (crevts,crunc) = self.getNumEventsError(crfileadd, crbin, cr.weight, 1, cr.selection)
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
                                            cr = None
                                            if not binname.split('_')[1].isdigit() :
                                                for fcr in self.fitcontrolregions.values() :
                                                    if fcr.label == binname.split('_')[1] :
                                                        cr = fcr
                                                        selection = cr.selection
                                                        bin = cr.binmap[binname]
                                                        break
                                                bkgsampname = '_'.join([samp,cr.label])
                                                if not bkgsampname in cr.backgrounds and not samp in self.signals :
                                                    continue
                                                if not samp in self.signals :
                                                    mcfile = mcfile.replace(samp, bkgsampname)
                                            else :
                                                bin = self.binmap[binname]
                                                cr = self.signalregion
                                            (mcevts,mcunc) = self.getNumEventsError(mcfile, bin, cr.weight, 1, selection)
                                            if not mcevts==0 :
                                                if mcunc > mcevts :
                                                    print 'Warning! %s has %s uncertainty %4.2f in bin %s, setting to 100' % (samp,uncname,100.0*mcunc/mcevts,binname)
                                                    unc.vals[binname][samp] = 2.0
                                                else :
                                                    unc.vals[binname][samp] = 1 + (mcunc/mcevts)
                                            else :
                                                unc.vals[binname][samp] = 2.0

    def getUncertaintyLine(self,uncname,unc,binlabel,backgrounds,nbkgevts):
        """Get line with uncertainty name, type, and values correctly formatted
        """
        isglobal = unc.vals.has_key('all')
        if '$SIG' in unc.label :
            uncline = unc.label.ljust(self.colwidth-2)
        else :
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
        elif not isglobal and unc.vals[binlabel].has_key(self.signals[0]) :
            uncline += ('SIGUNC').ljust(self.uncwidth-2)
            hasEntry = True
        elif isglobal and unc.vals['all'].has_key('signal') :
            if 'lumi' in unc.label :
                uncline += ('%4.3f' % unc.vals['all']['signal']).ljust(self.uncwidth)
            else :
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
                if 'lumi' in unc.label :
                    uncline += ('%4.3f' % unc.vals['all'][background]).ljust(self.uncwidth)
                else :
                    uncline += ('%4.2f' % unc.vals['all'][background]).ljust(self.uncwidth)
                hasEntry = True
            else :
                uncline += '-'.ljust(self.uncwidth)
        uncline += '\n'
        if not hasEntry :
            return ''
        return uncline

class FitRegion:
    def __init__(self,dataname,backgrounds,label,weight,selection,crSubSel,subNormSel,subNormWgt,orweight,orselection,type,allbins,compactbinlist,binmap,srtocrbinmap={}):
        self.dataname       = dataname
        self.backgrounds    = backgrounds
        self.label          = label
        self.weight         = weight
        self.selection      = selection
        self.crSubSel       = crSubSel
        self.subNormSel     = subNormSel
        self.subNormWgt     = subNormWgt
        self.orweight       = orweight
        self.orselection    = orselection
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
