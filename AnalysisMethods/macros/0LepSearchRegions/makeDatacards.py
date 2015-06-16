#! /usr/bin/python
import os
import sys
import re
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
        self.colwidth       = 25

        # get expanded binning with all allowed combinations
        self.varnames = []
        self.binedges = []
        for var in config_parser.options('bins') :
            self.varnames.append(var)
            self.binedges.append(config_parser.get('bins',var).replace(' ','').split(','))
        self.allbins = self.expandBins(self.varnames,self.binedges)

        # assign shorthand bin labels to all of them
        self.compactbinlist = []
        for bin in self.allbins :
            label = 'bin'
            label += self.getBinName(bin,True)
            self.compactbinlist.append(label)

        # load uncertainty files
        self.uncvalfiles = [f for f in os.listdir(self.setupbase) if not f.find(self.uncfileprefix) == -1]
        self.uncnames = []
        self.uncertainties = {}
        self.compileUncertainties()

    # Make datacards, one for every bin combination.
    # First the template card is made with all backgrounds,
    # then the signal points are looped to get the sig numbers.
    def produceDatacards(self):
        if not os.path.exists(self.datacarddir) :
            os.makedirs(self.datacarddir)

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
            lineSBin     = 'bin\t\t'       + str(ibin)+'\t'  
            lineSObs     = 'observation\t' + str(datayield)
            lineSBBin    = 'bin\t'         + str(ibin)+'\t\t'
            lineProcess1 = 'process\t'     + 'susy\t\t'
            lineProcess2 = 'process\t'     + '0\t\t'
            lineRate     = 'rate\t'        + 'SIGRATE'

            # loop through backgrounds to get yields
            ibkg = -1
            nBkgEvts = []
            for background in self.backgrounds :
                ibkg+=1
                bkgFile = os.path.join( self.treebase, background+self.filesuffix )
                lineSBBin    += str(ibin)+'\t\t'
                lineProcess1 += background+'\t\t'
                lineProcess2 += str(ibkg+1)+'\t\t'
                nevts = self.getNumEvents(bkgFile,bins)
                nBkgEvts.append(nevts)
                lineRate     += str(nevts)+'\t'

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
                    nUncs += 1
                    lineSys += self.getUncertaintyLine(uncname,unc,binLabel,nBkgEvts)

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
                datacard = datacard.replace('SIGRATE',str(nSig   )+'\t')

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
    
    def getNumEvents(self,filename,bins,isdata=False):
        """Returns the number of events in the given file for the given bin. All the necessary formatting to get the proper cut string for root is done here. This includes the baseline selection and lumi scaling defined by the user and the cuts to get the current bin.
        """
        cutstr = self.basesel
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
        return rate

    def getDummyUncertainties(self,procname,bins):
      """Dummy function to get the uncertanties. It returns static numbers for now. Will need to be filled in later. 
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
                name += bin[0]
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
                    binname = entries[0]
                    uncname = entries[1]
                    if not self.uncertainties.has_key(uncname) :
                        print "Didn't find uncertainty ",uncname
                        continue
                    unc = self.uncertainties[uncname]
                    samples = entries[2].split(',')
                    for samp in samples :
                        if samp == 'signal' or samp in self.signals or samp in self.backgrounds :
                            if not unc.vals.has_key(binname) :
                                unc.vals[binname] = {}
                            if unc.type == 'gmN' :
                                unc.cr_nevts[binname] = int(entries[3])
                                unc.vals[binname][samp] = float(entries[4])
                            else :
                                unc.vals[binname][samp] = float(entries[3])

    def getUncertaintyLine(self,uncname,unc,binlabel,nbkgevts):
        """Get line with uncertainty name, type, and values correctly formatted
        """
        isglobal = unc.vals.has_key('all')
        uncline = uncname.ljust(self.colwidth)
        uncline += unc.type
        if unc.type == 'gmN' :
            uncline += ' '+str(unc.cr_nevts[binlabel])
        uncline += '\t\t'
        if not isglobal and unc.vals[binlabel].has_key('signal') :
            uncline += ('%4.2f\t\t' % unc.vals[binlabel]['signal'])
        elif isglobal and unc.vals['all'].has_key('signal') :
            uncline += ('%4.2f\t\t' % unc.vals['all']['signal'])
        else :
            uncline += '-\t\t'
        ibkg = -1
        for background in self.backgrounds :
            ibkg += 1
            if not isglobal and unc.vals[binlabel].has_key(background) :
                if unc.type == 'gmN' :
                    uncline += ('%6.5f\t\t' % float(nbkgevts[ibkg]/float(unc.cr_nevts[binlabel])))
                else :
                    uncline += ('%4.2f\t\t' % unc.vals[binlabel][background])
            elif isglobal and unc.vals['all'].has_key(background) :
                uncline += ('%4.2f\t\t' % unc.vals['all'][background])
            else :
                uncline += '-\t\t'
        uncline += '\n'
        return uncline

class Uncertainty:
    def __init__(self,name,type):
        self.name = name
        self.type = type
        self.cr_nevts = {}
        self.vals = {}


if __name__=='__main__': main()
