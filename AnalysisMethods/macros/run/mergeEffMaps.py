#!/usr/bin/env python

# mergeEffMaps.py
# created Sept 13, 2015 by Alex Patterson
#
# part of b-tagging efficiency calculations:
#   BTaggingEffMapMaker.hh ---------> mergeEffMaps.py -----> jetCorrectionSet
#   numerator/denominator histos      merged eff histos      application of eff histos
#
# takes maps created by BTaggingEffMapMaker.hh (numerator/denominator of b-tagging efficiency),
#   rebins in variable-sized pt/eta bins, and calculates efficiency = num/denom histos which
#   are ready for jetCorrectionSet.
#
# input (maps) format is _effmap.root files (indicated by usual config file) labeled as so:
#   ttbar-50ns_2_effmap.root
# they contain T2HDs labeled by process name (defaults::PROCESS_NAMES), operating point, and parton flavor as so:
#   h2_BTaggingEfficiency_Num_ttbar_MEDIUM_b
# histograms created by BTaggingEffMapMaker.hh MUST be left evenly binned, and span eta<0 as well.
# easy to switch input assumption to |eta| -- see 'lookhere:abs' labels far below.
#
# output format is single file, jetCorr.root, containing TH2Ds labeled again by PN, OP, and PF as so:
#   h2_BTaggingEfficiency_ttbar_MEDIUM_b
# these are then ready to use by the b-tagging efficiency correction code. output eta bins are positive, |eta|.
#
# I also threw in a stat uncertainty histogram to help with binning, in h2_BTaggingStatistics_...
#   stat uncer in typical ratio eff=num/denom is eff*sqrt( (~num/num)^2 + (~denom/denom)^2 )
#   so with random fluctuations, ~X = sqrt(X), we have
#   (stat uncer of eff) = eff*sqrt(1/num + 1/denom)
#
# quick way to draw the output histos in jetCorr.root and tune pt, eta regions:
#   root -l jetCorr.root
#   _file0->ls()
#   TH2D *ttbar_eff = h2_BTaggingEfficiency_ttbar_MEDIUM_b
#   ttbar_eff->Draw("colz")
#
# VERY useful script to draw the four output histos for a sample:
# // Derived from Rene @ ROOT
# // puts four on a pad. to separate, follow the comments.
# {
#    const Int_t nhist=4;
#    char * hnames[nhist] = {"h2_BTaggingNumerator_signal_MEDIUM_b"
#                           ,"h2_BTaggingDenominator_signal_MEDIUM_b"
#                           ,"h2_BTaggingStatistics_signal_MEDIUM_b"
#                           ,"h2_BTaggingEfficiency_signal_MEDIUM_b"};
#    TFile * f = new TFile("jetCorr.root");
#    TH2D * hist[nhist];
#    TCanvas c1("c1", "canvas", 1000, 750); // comment
#    c1.Divide(2,2); // comment
# //   TCanvas * c[nhist]; // uncomment 
#    for (Int_t i=0;i<nhist;i++) {
#       c1.cd(i+1); // comment
#       hist[i] = (TH2D*)f->Get(hnames[i]);
#       hist[i]->GetXaxis()->SetTitle("pT");
#       hist[i]->GetXaxis()->CenterTitle();
#       hist[i]->GetYaxis()->SetTitle("|#eta|");
#       hist[i]->GetYaxis()->CenterTitle();
# //      c[i] = new TCanvas(Form("c%d",i)); //uncomment 
# //      hist[i]->SetStats(0); 
#       hist[i]->Draw("colz");
#    }
# }
#
# derived in part from 
#   https://github.com/rappoccio/usercode/blob/Dev_53x/EDSHyFT/test/bTaggingEfficiency/makeBTaggingEfficiencyMapAK5PF.py
#   and (UCSBSusy) process.py

import os, sys, argparse
from ROOT import gROOT, TFile, TH2D
from array import array
from math import sqrt

gROOT.SetBatch(1)

parser = argparse.ArgumentParser(description='Merge b-tagging efficiency maps of samples split by config file. No arguments prints help.')
parser.add_argument("-c", "--conf", dest="conf", default="run1lep.conf", help="Input configuration file. [Default: run1lep.conf]")
parser.add_argument("-d", "--pathtoconfig", dest="confpath", default="..", help="Path to directory with config. [Default: \"../\"]")
parser.add_argument("-p", "--pathtoeffmaps", dest="indir", default="${PWD}/plots", help="Path to directory with effmap roots. [Default: \"${PWD}/plots\"]")
parser.add_argument("-o", "--outdir", dest="outdir", default="${PWD}/plots", help="Output directory for (single) merged effmap file jetCorr.root. [Default: \"${PWD}/plots\"]")
#parser.print_help()
if len(sys.argv)==1:
    parser.print_help()
    sys.exit(1)
args = parser.parse_args()

# custom pt and eta binning, per sample. values SHOULD be divisible by the (evenly-binned) input bin sizes, eg input[0,10,20,..] and output[0,40,60,...].
datasets = {
  # background
  'qcd':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 60., 80., 100., 150., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'ttbar':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'w':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'z':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'g':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  't':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'ttZ':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  'ttW':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  },
  # signal
  'signal':{ # dataset name (defaults::PROCESS_NAMES)
     'b':    [[0., 40., 80., 100., 120., 140., 160., 200., 300., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for b jets
     'c':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for c jets
     'uds':  [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]],  # jet Pt and |eta| bins for uds jets
     'g':    [[0., 40., 60., 80., 100., 150., 200., 1000.],[0., 0.6, 1.2, 2.4]]  # jet Pt and |eta| bins for g jets
  }
}

# jet flavors whose effmaps were calculated
mcJetFlavors = ['b', 'c', 'uds', 'g']

# operating points whose effmaps were calculated
operatingPoints = ['LOOSE', 'MEDIUM', 'TIGHT']

# translate conf file process names into default::PROCESS_NAMES
confToFramework = {	 'wjets' : 'w'
		 	,'tW'    : 't'
			,'tbar'  : 't' # tbar_tW gets cut to tbar
			,'t'     : 't' # t_X gets cut to t
                        ,'znunu' : 'z'
			,'dyjetstoll' : 'z'
			,'gjets' : 'g'
			,'ttbar' : 'ttbar'
			,'ttW'   : 'ttW'
			,'ttZ'   : 'ttZ'
			,'qcd'   : 'qcd'
			,'signal': 'signal'}

# parse config file into samples
samples = []
files = []
xsecs = []
types = []
state = 0
snum = 0
with open((args.confpath+"/"+args.conf),"r") as f :
    for line in f :
        content = line.split()
        if "#" in content[0] :
            continue
        if content[0] == "%" :
            state += 1
            continue
        if content[0] == "$" :
            samples.append(content[1])
            files.append([])
            xsecs.append([])
            types.append([])
            snum += 1
            continue
        if state == 0 :
            files[snum-1].append(content[0])
            xsecs[snum-1].append(0)
            types[snum-1].append(content[1])
        elif state == 1 :
            files[snum-1].append(content[0])
            xsecs[snum-1].append(content[1])
            types[snum-1].append(0)

print 'Creating merged effmap file: jetCorr.root'

if len(samples) == 0 :
  raise ValueError('no samples found in file')

# set up the single output file (make directory if doesn't exist)
outputFilename = args.outdir + '/' + 'jetCorr.root'
if not os.path.exists(args.outdir) : 
  os.makedirs(args.outdir)
outputFile = TFile(outputFilename, 'RECREATE')

# iterate over samples, eg ttbar-madgraphmlm-50ns
for isam in range(len(samples)) :
         
# skip over data, we only correct mc
  ismc = 1 if types[isam][0] == 0 else 0
  if not ismc :
    continue

# extract process name (sproc, eg ttbar) from sample name (sname, eg ttbar_0_...)
# NOTE: sproc MUST match the entry in ucsbsusy::defaults::PROCESS_NAMES.
#   we run over file names, so just hack up a dictionary to translate these into
#   defaults::PROCESS_NAMES strings. Eg ttW in a filename is really process 't'.
  sname = samples[isam]
  if sname.startswith( 'T2' ) or sname.startswith( 'T1' ): # catches T2tt, T1ttt, leaves possible Ttbar alone
    sproc = 'signal' # opportunity to use exact name of signal .. but FW isn't ready for this.
  else :
    sproc = sname.split( '_' )[0] # ttbar-madgraphmlm-50ns_5_X -> ttbar-madgraphmlm-50ns, also cuts off things like t_barW, which is OK (see confToFramework)
    sproc = sproc.split( '-' )[0] # ttbar-madgraphmlm-50ns -> ttbar
  sproc = confToFramework[sproc] 
  print 'Process name (MUST match the entry in ucsbsusy::defaults::PROCESS_NAMES: %s' % sproc

# iterate over parton flavors and operating points (needed to set up output histo)
  for partonFlavor in mcJetFlavors :
    for operatingPoint in operatingPoints :

# set up output histos for this sample, to which all files in the sample will contribute
      binsX = array('d', datasets[sproc][partonFlavor][0])
      binsY = array('d', datasets[sproc][partonFlavor][1])

      denominatorOut = TH2D('h2_BTaggingDenominator_'  + sproc + '_' + operatingPoint + '_' + partonFlavor, '', (len(binsX)-1), binsX, (len(binsY)-1), binsY)
      numeratorOut   = TH2D('h2_BTaggingNumerator_'  + sproc + '_' + operatingPoint + '_' + partonFlavor, '', (len(binsX)-1), binsX, (len(binsY)-1), binsY)
      efficiencyOut  = TH2D('h2_BTaggingEfficiency_'  + sproc + '_' + operatingPoint + '_' + partonFlavor, '', (len(binsX)-1), binsX, (len(binsY)-1), binsY)
      statisticsOut  = TH2D('h2_BTaggingStatistics_'  + sproc + '_' + operatingPoint + '_' + partonFlavor, '', (len(binsX)-1), binsX, (len(binsY)-1), binsY)

# loop over all output bins to fill them. note that all of a sample's sample-parts contribute to each output bin.
      for i in range(1,denominatorOut.GetXaxis().GetNbins()+1):
        for j in range(1,denominatorOut.GetYaxis().GetNbins()+1):

# start output bin contents at zero (additive)
          numerator   = 0
          denominator = 0

# for each sample-part in the sample, eg ttbar_5_X
          for ifile in range(len(files[isam])) :
            findex = ifile if len(files[isam]) > 1 else -1

# open the sample-part's effmap file and get its histos
            inputSuffix   = str(findex) + '_effmap.root' if findex != -1 else 'effmap.root'
            inputFilename = args.indir + '/' + samples[isam] + '_' + inputSuffix
            inputFile     = TFile(inputFilename, 'READ')

            denominatorHisto = 'h2_BTaggingEfficiency_Denom_' + sproc + '_' + operatingPoint + '_' + partonFlavor
            numeratorHisto   = 'h2_BTaggingEfficiency_Num_' + sproc + '_' + operatingPoint + '_' + partonFlavor
		
            denominatorIn = inputFile.Get(denominatorHisto)
            numeratorIn   = inputFile.Get(numeratorHisto)

            xShift = denominatorIn.GetXaxis().GetBinWidth(1)/2. # half-widths in pt and eta
            yShift = denominatorIn.GetYaxis().GetBinWidth(1)/2.

# get bin range in X and Y of input, which is covered by the current output bin. remember input eta spans negative.
            binXMin    = denominatorIn.GetXaxis().FindBin(denominatorOut.GetXaxis().GetBinLowEdge(i)+xShift)
            binXMax    = denominatorIn.GetXaxis().FindBin(denominatorOut.GetXaxis().GetBinUpEdge(i)-xShift)

            binYMinPos = denominatorIn.GetYaxis().FindBin(denominatorOut.GetYaxis().GetBinLowEdge(j)+yShift) #lookhere:abs -- match Y lines to X lines if input is binned in |eta|
            binYMaxPos = denominatorIn.GetYaxis().FindBin(denominatorOut.GetYaxis().GetBinUpEdge(j)-yShift)
            binYMinNeg = denominatorIn.GetYaxis().FindBin(-denominatorOut.GetYaxis().GetBinUpEdge(j)+yShift)
            binYMaxNeg = denominatorIn.GetYaxis().FindBin(-denominatorOut.GetYaxis().GetBinLowEdge(j)-yShift)

# integrate that range of input bins to get num and denom of current output bin's efficiency
            denominator = denominator + denominatorIn.Integral(binXMin,binXMax,binYMinPos,binYMaxPos) #lookhere:abs
            denominator = denominator + denominatorIn.Integral(binXMin,binXMax,binYMinNeg,binYMaxNeg)
            numerator   = numerator +     numeratorIn.Integral(binXMin,binXMax,binYMinPos,binYMaxPos)
            numerator   = numerator +     numeratorIn.Integral(binXMin,binXMax,binYMinNeg,binYMaxNeg)

# if last bin in pT, add to it the statistics of higher-pT input bins
            if(i==denominatorOut.GetXaxis().GetNbins()):
              denominator = denominator + denominatorIn.Integral(binXMax+1,denominatorIn.GetXaxis().GetNbins()+1,binYMinPos,binYMaxPos) #lookhere:abs
              denominator = denominator + denominatorIn.Integral(binXMax+1,denominatorIn.GetXaxis().GetNbins()+1,binYMinNeg,binYMaxNeg)
              numerator   = numerator +     numeratorIn.Integral(binXMax+1,  numeratorIn.GetXaxis().GetNbins()+1,binYMinPos,binYMaxPos)
              numerator   = numerator +     numeratorIn.Integral(binXMax+1,  numeratorIn.GetXaxis().GetNbins()+1,binYMinNeg,binYMaxNeg)

# fill output bin after all sample-parts have contributed to this output bin
          denominatorOut.SetBinContent(i,j,denominator)
          numeratorOut.SetBinContent(i,j,numerator)
          efficiency = numerator/denominator if denominator > 0. else 0.
          efficiencyOut.SetBinContent(i,j,efficiency)
          statUncer = efficiency*sqrt(1./denominator + 1./numerator) if ((denominator > 0.) and (numerator > 0.)) else 0.
          statisticsOut.SetBinContent(i,j,statUncer)

# check if 0 or 100% efficiency in this output bin
          if(efficiency == 0. or efficiency == 1.):
            print 'Warning! Bin(%i,%i) for %s process, %s jets, and %s operating point has a b-tagging efficiency of %.3f'%(i,j,sproc,partonFlavor,operatingPoint,efficiency)
            print '  Corresponds to pt [%.3f, %.3f] and eta [%.3f,%.3f]' % (denominatorOut.GetXaxis().GetBinLowEdge(i), denominatorOut.GetXaxis().GetBinUpEdge(i), denominatorOut.GetYaxis().GetBinLowEdge(j), denominatorOut.GetYaxis().GetBinUpEdge(j))
# check if statistical uncertainty is ... prohibitive
          if(statUncer > efficiency) :  
            print 'Warning! Bin(%i,%i) for %s process, %s jets, and %s operating point has a stat error of %.3f on an efficiency of %.3f' % (i,j,sproc,partonFlavor,operatingPoint,statUncer,efficiency)
            print '  Corresponds to pt [%.3f, %.3f] and eta [%.3f,%.3f]' % (denominatorOut.GetXaxis().GetBinLowEdge(i), denominatorOut.GetXaxis().GetBinUpEdge(i), denominatorOut.GetYaxis().GetBinLowEdge(j), denominatorOut.GetYaxis().GetBinUpEdge(j))
                 
# set efficiencies of pt and eta overflow bins equal to those of nearest (highest) bin (ignore stat histo here -- it's for human inspection)
      for i in range(1,denominatorOut.GetXaxis().GetNbins()+1): # eta overflow
        efficiencyOut.SetBinContent(i, denominatorOut.GetYaxis().GetNbins()+1, efficiencyOut.GetBinContent(i, denominatorOut.GetYaxis().GetNbins()))
      for j in range(1,denominatorOut.GetYaxis().GetNbins()+2): # pt overflow
        efficiencyOut.SetBinContent(denominatorOut.GetXaxis().GetNbins()+1, j, efficiencyOut.GetBinContent(denominatorOut.GetXaxis().GetNbins(), j))

# write to the output histo file and move on to next parton flavor and operating point (and beyond that, next sample)
      outputFile.cd()
      denominatorOut.Write()
      numeratorOut.Write()
      efficiencyOut.Write()
      statisticsOut.Write()

# after all sample-parts, jet flavors, and operating points done for this sample, let user know
  print '-------------------------------------------------------------------------------------------'
  print 'b-tagging efficiency map (numerator count, denominator count, efficiency, and stat uncertainty) for'
  print sproc
  print 'successfully created and stored in %s' % outputFilename
  print ''

# after all samples done, close output file
outputFile.Close()  
