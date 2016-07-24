import ROOT
from optparse import OptionParser

def findBins(array, var):
    size = len(array)
    bin = "dump";
    for i in xrange(size-1):
        low = array[i]
        hi  = array[i+1]
        if (low <= var and hi > var):
            bin = str(low)+"To"+str(hi)
  
    return bin;

def main(options):
    
    pts  = []
    for v in options.ptbins.split(","):
        pts.append(float(v))
    etas = []
    for v in options.etabins.split(","):
        etas.append(float(v))

    inFile = ROOT.TFile(options.input)
    #inFile.cd(options.directory)
    #fDir = inFile.Get(options.directory)
    fChain = inFile.Get("Events")

    histos = dict()

    print "Doing templates for All"
    histNameSt = "hMass_All"
    hp = histNameSt+"_Pass"
    hf = histNameSt+"_Fail"
    histos[hp] = ROOT.TH1D(hp, hp, 120, 60, 120)
    histos[hf] = ROOT.TH1D(hf, hf, 120, 60, 120)
    cuts = "("+options.idprobe+"==1"+")*"+options.weightVarName
    fChain.Draw("mass>>"+histos[hp].GetName(), cuts, "goff")
    cuts = options.idprobe+"==0"
    fChain.Draw("mass>>"+histos[hf].GetName(), cuts, "goff")

    for binPt in xrange(len(pts)-1):
        print "Doing templates for "+str(pts[binPt])+"To"+str(pts[binPt+1])
        histNameSt = "hMass_%4.3fTo%4.3f" % (pts[binPt],pts[binPt+1])
        hp = histNameSt+"_Pass"
        hf = histNameSt+"_Fail"
        histos[hp] = ROOT.TH1D(hp, hp, 120, 60, 120)
        histos[hf] = ROOT.TH1D(hf, hf, 120, 60, 120)
        
        binning = options.ptVarName +">"+str(pts[binPt])+" && "+options.ptVarName +"<"+str(pts[binPt+1])
        cuts = "("+binning + " && "+options.idprobe+"==1"+")*"+options.weightVarName
        fChain.Draw("mass>>"+histos[hp].GetName(), cuts, "goff")
        cuts = binning + " && "+options.idprobe+"==0"
        fChain.Draw("mass>>"+histos[hf].GetName(), cuts, "goff")
    
    for binEta in xrange(len(etas)-1):
        print "Doing templates for "+str(etas[binEta])+"To"+str(etas[binEta+1])
        histNameSt = "hMass_%4.3fTo%4.3f" % (etas[binEta],etas[binEta+1])
        hp = histNameSt+"_Pass"
        hf = histNameSt+"_Fail"
        histos[hp] = ROOT.TH1D(hp, hp, 120, 60, 120)
        histos[hf] = ROOT.TH1D(hf, hf, 120, 60, 120)
        
        binning = options.etaVarName +">"+str(etas[binEta])+" && "+options.etaVarName +"<"+str(etas[binEta+1])            
        cuts = "("+binning + " && "+options.idprobe+"==1"+")*"+options.weightVarName
        fChain.Draw("mass>>"+histos[hp].GetName(), cuts, "goff")
        cuts = binning + " && "+options.idprobe+"==0"
        fChain.Draw("mass>>"+histos[hf].GetName(), cuts, "goff")
    
    for binPt in xrange(len(pts)-1):
        for binEta in xrange(len(etas)-1):
            print "Doing templates for "+str(pts[binPt])+"To"+str(pts[binPt+1])+"_"+str(etas[binEta])+"To"+str(etas[binEta+1])
            histNameSt = "hMass_%4.3fTo%4.3f_%4.3fTo%4.3f"% (pts[binPt],pts[binPt+1],etas[binEta],etas[binEta+1])
            hp = histNameSt+"_Pass"
            hf = histNameSt+"_Fail"
            histos[hp] = ROOT.TH1D(hp, hp, 120, 60, 120)
            histos[hf] = ROOT.TH1D(hf, hf, 120, 60, 120)
            
            binning = options.ptVarName +">"+str(pts[binPt])+" && "+options.ptVarName +"<"+str(pts[binPt+1])+" && "+options.etaVarName +">"+str(etas[binEta])+" && "+options.etaVarName +"<"+str(etas[binEta+1])            
            cuts = "("+binning + " && "+options.idprobe+"==1"+")*"+options.weightVarName
            fChain.Draw("mass>>"+histos[hp].GetName(), cuts, "goff")
            cuts = binning + " && "+options.idprobe+"==0"
            fChain.Draw("mass>>"+histos[hf].GetName(), cuts, "goff")
    
    outFile = ROOT.TFile(options.output, "RECREATE")
    for k in histos:
        histos[k].Write()
    outFile.Close()


if __name__ == "__main__":  
    parser = OptionParser()
    parser.add_option("-i", "--input", default="../trees/TnPTree_MC.root", help="Input filename")
    parser.add_option("-o", "--output", default="mc_templates.root", help="Output filename")
    #parser.add_option("-d", "--directory", default="GsfElectronToRECO", help="Directory with fitter_tree")
    parser.add_option("", "--idprobe", default="pass", help="String identifying ID WP to measure")
    parser.add_option("", "--ptbins", default="10,20,30,40,60,100,1000", help="Binning to use in pT")
    parser.add_option("", "--etabins", default="0.0,0.8,1.4442,1.566,2.0,2.5", help="Binning to use in eta")
    parser.add_option("", "--etaVarName", default="abseta", help="Eta variable branch name")
    parser.add_option("", "--ptVarName", default="pt", help="Pt variable branch name")
    parser.add_option("", "--weightVarName", default="weight", help="Weight variable branch name")

    (options, arg) = parser.parse_args()
     
    main(options)
