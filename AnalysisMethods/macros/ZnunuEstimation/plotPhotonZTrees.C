#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotPhotonZTrees(const TString conffile="runPhotonZ.conf",
    const TString inputdir="../run/plots/",
    const TString outputdir="../run/plots/")
{

  TString all          = "1==1";
  TString preselection = "NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1 && MET>=200";
  TString baseline     = preselection + " && dPhiMET12>1 && dPhiMET3>0.5 && mtB1MET>175 && mtB2MET>175";


  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::NORMCOMP);
    myPlots->setWriteHists();
    myPlots->setOutfile("gzratio.root");
    myPlots->setFormat("pdf");
    myPlots->setWgtVar("ScaleFactor");
    myPlots->setDrawOption("histE");


    myPlots->addTreeVar("genPT_NJ_0to1", "genBosonPT", "NGenJets<=1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NJ_2to3", "genBosonPT", "NGenJets==2 || NGenJets==3", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NJ_4to5", "genBosonPT", "NGenJets==4 || NGenJets==5", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NJ_6to7", "genBosonPT", "NGenJets==6 || NGenJets==7", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NJ_ge8", "genBosonPT", "NGenJets>=8", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_inc", "genBosonPT", "1==1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NB_0", "genBosonPT", "NGenBJets==0", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NB_1", "genBosonPT", "NGenBJets==1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genPT_NB_ge2", "genBosonPT", "NGenBJets>=2", "boson PT [GeV]", 18, 100, 1000);

    myPlots->addTreeVar("pre_genPT_NJ_0to1", "genBosonPT", preselection+"&& NGenJets<=1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NJ_2to3", "genBosonPT", preselection+"&& NGenJets==2 || NGenJets==3", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NJ_4to5", "genBosonPT", preselection+"&& NGenJets==4 || NGenJets==5", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NJ_6to7", "genBosonPT", preselection+"&& NGenJets==6 || NGenJets==7", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NJ_ge8", "genBosonPT", preselection+"&& NGenJets>=8", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_inc", "genBosonPT", preselection+"&& 1==1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NB_0", "genBosonPT", preselection+"&& NGenBJets==0", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NB_1", "genBosonPT", preselection+"&& NGenBJets==1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_genPT_NB_ge2", "genBosonPT", preselection+"&& NGenBJets>=2", "boson PT [GeV]", 18, 100, 1000);


    myPlots->plot();
    delete myPlots;
  }


  {
    TFile *inFile = new TFile(inputdir+"gzratio.root");
    PlotStuff* myPlots = new PlotStuff(inFile, outputdir, 0, true);
    myPlots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
    myPlots->setPlotType(PlotStuff::COMP);
    myPlots->setFormat("pdf");
    myPlots->setDrawOption("histE");


    vector<TString> njlabels{"NJ_2to3", "NJ_4to5", "NJ_6to7", "NJ_ge8"};
    vector<TString> nblabels{"inc", "NB_0", "NB_1", "NB_ge2"};
    vector<TString> njnames, nbnames;
    for (auto l : njlabels) njnames.push_back(TString("genPT_")+l+"_znunuovergjets");
    for (auto l : nblabels) nbnames.push_back(TString("genPT_")+l+"_znunuovergjets");

    myPlots->addCompSet("genPT_NJets", njnames, njlabels);
    myPlots->addCompSet("genPT_NBJets", nbnames, nblabels);

    njnames.clear();
    nbnames.clear();
    for (auto l : njlabels) njnames.push_back(TString("pre_genPT_")+l+"_znunuovergjets");
    for (auto l : nblabels) nbnames.push_back(TString("pre_genPT_")+l+"_znunuovergjets");

    myPlots->addCompSet("pre_genPT_NJets", njnames, njlabels);
    myPlots->addCompSet("pre_genPT_NBJets", nbnames, nblabels);

    myPlots->plot();
    delete myPlots;
  }



  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::COMP);
    myPlots->setFormat("pdf");
    myPlots->setWgtVar("NewSF");
    myPlots->setDrawOption("histE");

    myPlots->addTreeVar("genBosonPT", "genBosonPT", all, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("numGenJet", "NGenJets", all, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("numGenBJets", "NGenBJets", all, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("MET", "MET", all, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("numJets", "NJets", all, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("numBJets", "NBJets", all, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("dPhiMET12","dPhiMET12", all, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("dPhiMET3","dPhiMET3", all, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("mtB1MET", "mtB1MET", all, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("mtB2MET", "mtB2MET", all, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);


    myPlots->addTreeVar("pre_genBosonPT", "genBosonPT", preselection, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_numGenJet", "NGenJets", preselection, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("pre_numGenBJets", "NGenBJets", preselection, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("pre_MET", "MET", preselection, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_numJets", "NJets", preselection, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("pre_numBJets", "NBJets", preselection, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("pre_dPhiMET12","dPhiMET12", preselection, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("pre_dPhiMET3","dPhiMET3", preselection, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("pre_mtB1MET", "mtB1MET", preselection, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("pre_mtB2MET", "mtB2MET", preselection, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);

    myPlots->addTreeVar("baseline_genBosonPT", "genBosonPT", baseline, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numGenJet", "NGenJets", baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numGenBJets", "NGenBJets", baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_MET", "MET", baseline, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numJets", "NJets", baseline, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numBJets", "NBJets", baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_dPhiMET12","dPhiMET12", baseline, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("baseline_dPhiMET3","dPhiMET3", baseline, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("baseline_mtB1MET", "mtB1MET", baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_mtB2MET", "mtB2MET", baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);


//    myPlots->plot();
    delete myPlots;
  }

  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::NORMCOMP);
    myPlots->setFormat("pdf");
    myPlots->setWgtVar("NewSF");
    myPlots->setDrawOption("histE");

    myPlots->addTreeVar("genBosonPT", "genBosonPT", all, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("numGenJet", "NGenJets", all, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("numGenBJets", "NGenBJets", all, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("MET", "MET", all, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("numJets", "NJets", all, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("numBJets", "NBJets", all, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("dPhiMET12","dPhiMET12", all, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("dPhiMET3","dPhiMET3", all, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("mtB1MET", "mtB1MET", all, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("mtB2MET", "mtB2MET", all, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);


    myPlots->addTreeVar("pre_genBosonPT", "genBosonPT", preselection, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_numGenJet", "NGenJets", preselection, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("pre_numGenBJets", "NGenBJets", preselection, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("pre_MET", "MET", preselection, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("pre_numJets", "NJets", preselection, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("pre_numBJets", "NBJets", preselection, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("pre_dPhiMET12","dPhiMET12", preselection, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("pre_dPhiMET3","dPhiMET3", preselection, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("pre_mtB1MET", "mtB1MET", preselection, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("pre_mtB2MET", "mtB2MET", preselection, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);

    myPlots->addTreeVar("baseline_genBosonPT", "genBosonPT", baseline, "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numGenJet", "NGenJets", baseline, "N(gen ak4 Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numGenBJets", "NGenBJets", baseline, "N(gen ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_MET", "MET", baseline, "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("baseline_numJets", "NJets", baseline, "N(ak4Jets)", 10, -0.5, 9.5);
    myPlots->addTreeVar("baseline_numBJets", "NBJets", baseline, "N(ak4 BJets)", 5, -0.5, 4.5);
    myPlots->addTreeVar("baseline_dPhiMET12","dPhiMET12", baseline, "dPhiMET12", 30, 0, 3);
    myPlots->addTreeVar("baseline_dPhiMET3","dPhiMET3", baseline, "dPhiMET3", 30, 0, 3);
    myPlots->addTreeVar("baseline_mtB1MET", "mtB1MET", baseline, "m_{T}(B1,MET) [GeV]", 20, 0, 1000);
    myPlots->addTreeVar("baseline_mtB2MET", "mtB2MET", baseline, "m_{T}(B2,MET) [GeV]", 20, 0, 1000);

//    myPlots->plot();
    delete myPlots;
  }




}
