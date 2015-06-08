#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#endif

void plotSplits(const TString conffile="plottops.conf",
                const TString inputdir="trees",
                const TString outputdir="plots/splits")
{
  gSystem->mkdir(outputdir, true);

  // will take ages to run over all cuts
  vector<double> mcuts = {20.0, 40.0, 50.0, 55.0}; //{10.0, 20.0, 30.0, 40.0, 50.0, 55.0};
  vector<double> rmins = {0.10}; //{0.05, 0.10, 0.15, 0.20};
  vector<double> ycuts = {0.05}; //{0.05, 0.10, 0.15, 0.20};

  vector<double> fatjetptbins = {20.0, 100.0, 300.0, 500.0, 1000.0};

  PlotStuff* plots1 = new PlotStuff(conffile, inputdir, outputdir);

  plots1->setPlotSource(PlotStuff::TREES);
  plots1->setPlotType(PlotStuff::COMP);
  plots1->setTree("Jets");
  plots1->setWgtVar("1.0");
  plots1->setUnitScale();
  plots1->setWriteHists();
  plots1->setHeaderText("","","");
  plots1->setDrawOption("hist");
  plots1->setDrawOption2D("COLZTEXT");

  // single parton
  // should clean
  plots1->addTreeVar("singlepart_shouldsplit_superjetm",      "superJet_mass", "splitAnalysis==2 || splitAnalysis==4", "superjet mass [GeV]", 100, 0, 500);
  plots1->addTreeVar("singlepart_shouldsplit_superjetm_zoom", "superJet_mass", "splitAnalysis==2 || splitAnalysis==4", "superjet mass [GeV]", 100, 0, 100);
  plots1->addTreeVar("singlepart_shouldsplit_subjetdrmin",    "subJet_dr",     "splitAnalysis==2 || splitAnalysis==4", "#DeltaR(j1,j2)", 100, 0, 1);
  plots1->addTreeVar("singlepart_shouldsplit_superjetm_subjetfailpt",   "superJet_mass", "splitAnalysis==3 || splitAnalysis==5", "superjet mass [GeV]", 100, 0, 100);
  plots1->addTreeVar("singlepart_shouldsplit_subjetdrmin_subjetfailpt", "subJet_dr",     "splitAnalysis==3 || splitAnalysis==5", "#DeltaR(j1,j2)", 100, 0, 1);
  plots1->addTreeVar("singlepart_shouldsplittofirst_subjetptratio",     "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==2", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
  plots1->addTreeVar("singlepart_shouldsplittosecond_subjetptratio",    "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==4", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
  // should not clean
  plots1->addTreeVar("singlepart_badsplit_superjetm",      "superJet_mass", "splitAnalysis==6", "superjet mass [GeV]", 100, 0, 500);
  plots1->addTreeVar("singlepart_badsplit_superjetm_zoom", "superJet_mass", "splitAnalysis==6", "superjet mass [GeV]", 100, 0, 100);
  plots1->addTreeVar("singlepart_badsplit_subjetdrmin",    "subJet_dr",     "splitAnalysis==6", "#DeltaR(j1,j2)", 100, 0, 1);
  plots1->addTreeVar("singlepart_badsplit_subjetptratio",  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==6", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);

  // multi parton
  // should split
  plots1->addTreeVar("multipart_shouldsplit_superjetm",      "superJet_mass", "splitAnalysis>6 && splitAnalysis<12", "superjet mass [GeV]", 100, 0, 500);
  plots1->addTreeVar("multipart_shouldsplit_superjetm_zoom", "superJet_mass", "splitAnalysis>6 && splitAnalysis<12", "superjet mass [GeV]", 100, 0, 100);
  plots1->addTreeVar("multipart_shouldsplit_subjetdrmin",    "subJet_dr",     "splitAnalysis>6 && splitAnalysis<12", "#DeltaR(j1,j2)", 100, 0, 1);
  plots1->addTreeVar("multipart_shouldsplittoboth_subjetptratio",       "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==7 || splitAnalysis==10 || splitAnalysis==11 || splitAnalysis==13 || splitAnalysis==16", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
  plots1->addTreeVar("multipart_shouldsplittofirstonly_subjetptratio",  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==8 || splitAnalysis==12 || splitAnalysis==14", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
  plots1->addTreeVar("multipart_shouldsplittosecondonly_subjetptratio", "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==9 || splitAnalysis==15 || splitAnalysis==17", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
  // should not split
  plots1->addTreeVar("multipart_badsplit_superjetm",      "superJet_mass", "splitAnalysis==18", "superjet mass [GeV]", 100, 0, 500);
  plots1->addTreeVar("multipart_badsplit_superjetm_zoom", "superJet_mass", "splitAnalysis==18", "superjet mass [GeV]", 100, 0, 100);
  plots1->addTreeVar("multipart_badsplit_subjetdrmin",    "subJet_dr",     "splitAnalysis==18", "#DeltaR(j1,j2)", 100, 0, 1);
  plots1->addTreeVar("multipart_badsplit_subjetptratio",  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "splitAnalysis==18", "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);

  // plot algorithm choice vs gen split analysis
  for(auto mcut : mcuts) {
    for(auto rmin : rmins) {
      for(auto ycut : ycuts) {
        // 1 = don't split
        // 2 = split, iterate on first only
        // 3 = split, iterate on second only
        // 4 = split, iterate on both
        TString cutlabel = TString::Format("_%2.0f_%4.2f_%4.2f", mcut, rmin, ycut);
        TString splitDecision = TString::Format("(1*(superJet_mass<%4.2f || subJet_dr<%4.2f || splitDecision==0) + 2*(superJet_mass>%4.2f && subJet_dr>%4.2f && subJet_1_pt>20.0 && (subJet_2_pt<20.0 || subJet_2_pt/(subJet_1_pt+subJet_2_pt)<%4.2f)) + 4*(superJet_mass>%4.2f && subJet_dr>%4.2f && subJet_2_pt>20.0 && subJet_2_pt/(subJet_1_pt+subJet_2_pt)>%4.2f))", mcut, rmin, mcut, rmin, ycut, mcut, rmin, ycut);

        // single
        TString singleSplitAnalysis = "(1*(splitAnalysis==6) + 2*(splitAnalysis==2) + 3*(splitAnalysis==4))";
        plots1->addTreeVar("singlepart_decisionvsanalysis"+cutlabel, splitDecision+":"+singleSplitAnalysis, "splitAnalysis==2 || splitAnalysis==4 || splitAnalysis==6", "Gen choice; Algorithm choice", 4, 0.5, 4.5, 4, 0.5, 4.5);

        // multi
        TString multiSplitAnalysis = "(1*(splitAnalysis==18) + 2*(splitAnalysis==8 || splitAnalysis==12 || splitAnalysis==14) + 3*(splitAnalysis==9 || splitAnalysis==15 || splitAnalysis==17) + 4*(splitAnalysis==7 || splitAnalysis==10 || splitAnalysis==11 || splitAnalysis==13 || splitAnalysis==16))";
        plots1->addTreeVar("multipart_decisionvsanalysis"+cutlabel,  splitDecision+":"+multiSplitAnalysis,  "splitAnalysis>6 && splitAnalysis<19", "Gen choice; Algorithm choice", 4, 0.5, 4.5, 4, 0.5, 4.5);
      }
    }
  }

  // in bins of fatjet pt
  for(unsigned int ibin = 0; ibin < fatjetptbins.size()-1; ++ibin) {
    TString ptcut = TString::Format(" && fatJet_pt > %5.2f && fatJet_pt < %5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]);
    TString label = TString::Format("_fatjetpt%5.2fto%5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]);

    // single parton
    // should clean
    plots1->addTreeVar("singlepart_shouldsplit_superjetm"+label,      "superJet_mass", "(splitAnalysis==2 || splitAnalysis==4)"+ptcut, "superjet mass [GeV]", 100, 0, 500);
    plots1->addTreeVar("singlepart_shouldsplit_superjetm_zoom"+label, "superJet_mass", "(splitAnalysis==2 || splitAnalysis==4)"+ptcut, "superjet mass [GeV]", 100, 0, 100);
    plots1->addTreeVar("singlepart_shouldsplit_subjetdrmin"+label,    "subJet_dr", "(splitAnalysis==2 || splitAnalysis==4)"+ptcut, "#DeltaR(j1,j2)", 100, 0, 1);
    plots1->addTreeVar("singlepart_shouldsplit_superjetm_subjetfailpt"+label,   "superJet_mass", "(splitAnalysis==3 || splitAnalysis==5)"+ptcut, "superjet mass [GeV]", 100, 0, 100);
    plots1->addTreeVar("singlepart_shouldsplit_subjetdrmin_subjetfailpt"+label, "subJet_dr",     "(splitAnalysis==3 || splitAnalysis==5)"+ptcut, "#DeltaR(j1,j2)", 100, 0, 1);
    plots1->addTreeVar("singlepart_shouldsplittofirst_subjetptratio"+label,     "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==2)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
    plots1->addTreeVar("singlepart_shouldsplittosecond_subjetptratio"+label,    "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==4)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
    // should not clean
    plots1->addTreeVar("singlepart_badsplit_superjetm"+label,      "superJet_mass", "(splitAnalysis==6)"+ptcut, "superjet mass [GeV]", 100, 0, 500);
    plots1->addTreeVar("singlepart_badsplit_superjetm_zoom"+label, "superJet_mass", "(splitAnalysis==6)"+ptcut, "superjet mass [GeV]", 100, 0, 100);
    plots1->addTreeVar("singlepart_badsplit_subjetdrmin"+label,    "subJet_dr",     "(splitAnalysis==6)"+ptcut, "#DeltaR(j1,j2)", 100, 0, 1);
    plots1->addTreeVar("singlepart_badsplit_subjetptratio"+label,  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==6)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);

    // multi parton
    // should split
    plots1->addTreeVar("multipart_shouldsplit_superjetm"+label,           "superJet_mass", "(splitAnalysis>6 && splitAnalysis<12)"+ptcut, "superjet mass [GeV]", 100, 0, 500);
    plots1->addTreeVar("multipart_shouldsplit_superjetm_zoom"+label,      "superJet_mass", "(splitAnalysis>6 && splitAnalysis<12)"+ptcut, "superjet mass [GeV]", 100, 0, 100);
    plots1->addTreeVar("multipart_shouldsplit_subjetdrmin"+label,         "subJet_dr",     "(splitAnalysis>6 && splitAnalysis<12)"+ptcut, "#DeltaR(j1,j2)", 100, 0, 1);
    plots1->addTreeVar("multipart_shouldsplittoboth_subjetptratio"+label, "subJet_2_pt/(subJet_1_pt+subJet_2_pt)",       "(splitAnalysis==7 || splitAnalysis==10 || splitAnalysis==11 || splitAnalysis==13 || splitAnalysis==16)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
    plots1->addTreeVar("multipart_shouldsplittofirstonly_subjetptratio"+label,  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==8 || splitAnalysis==12 || splitAnalysis==14)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
    plots1->addTreeVar("multipart_shouldsplittosecondonly_subjetptratio"+label, "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==9 || splitAnalysis==15 || splitAnalysis==17)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);
    // should not split
    plots1->addTreeVar("multipart_badsplit_superjetm"+label,      "superJet_mass", "(splitAnalysis==18)"+ptcut, "superjet mass [GeV]", 100, 0, 500);
    plots1->addTreeVar("multipart_badsplit_superjetm_zoom"+label, "superJet_mass", "(splitAnalysis==18)"+ptcut, "superjet mass [GeV]", 100, 0, 100);
    plots1->addTreeVar("multipart_badsplit_subjetdrmin"+label,    "subJet_dr",     "(splitAnalysis==18)"+ptcut, "#DeltaR(j1,j2)", 100, 0, 1);
    plots1->addTreeVar("multipart_badsplit_subjetptratio"+label,  "subJet_2_pt/(subJet_1_pt+subJet_2_pt)", "(splitAnalysis==18)"+ptcut, "p_{T}(j2)/(p_{T}(j1) + p_{T}(j2)", 100, 0, 0.5);

    // plot algorithm choice vs gen split analysis
    for(auto mcut : mcuts) {
      for(auto rmin : rmins) {
        for(auto ycut : ycuts) {
          // 1 = don't split
          // 2 = split, iterate on first only
          // 3 = split, iterate on second only
          // 4 = split, iterate on both
          TString cutlabel = TString::Format("_%2.0f_%4.2f_%4.2f", mcut, rmin, ycut);
          TString splitDecision = TString::Format("(1*(superJet_mass<%4.2f || subJet_dr<%4.2f || splitDecision==0) + 2*(superJet_mass>%4.2f && subJet_dr>%4.2f && subJet_1_pt>20.0 && (subJet_2_pt<20.0 || subJet_2_pt/(subJet_1_pt+subJet_2_pt)<%4.2f)) + 4*(superJet_mass>%4.2f && subJet_dr>%4.2f && subJet_2_pt>20.0 && subJet_2_pt/(subJet_1_pt+subJet_2_pt)>%4.2f))", mcut, rmin, mcut, rmin, ycut, mcut, rmin, ycut);

          // single
          TString singleSplitAnalysis = "(1*(splitAnalysis==6) + 2*(splitAnalysis==2) + 3*(splitAnalysis==4))";
          plots1->addTreeVar("singlepart_decisionvsanalysis"+cutlabel+label, splitDecision+":"+singleSplitAnalysis, "(splitAnalysis==2 || splitAnalysis==4 || splitAnalysis==6)"+ptcut, "Gen choice; Algorithm choice", 4, 0.5, 4.5, 4, 0.5, 4.5);

          // multi
          TString multiSplitAnalysis = "(1*(splitAnalysis==18) + 2*(splitAnalysis==8 || splitAnalysis==12 || splitAnalysis==14) + 3*(splitAnalysis==9 || splitAnalysis==15 || splitAnalysis==17) + 4*(splitAnalysis==7 || splitAnalysis==10 || splitAnalysis==11 || splitAnalysis==13 || splitAnalysis==16))";
          plots1->addTreeVar("multipart_decisionvsanalysis"+cutlabel+label,  splitDecision+":"+multiSplitAnalysis,  "(splitAnalysis>6 && splitAnalysis<19)"+ptcut, "Gen choice; Algorithm choice", 4, 0.5, 4.5, 4, 0.5, 4.5);
        }
      }
    }
  }

  plots1->plot();

  // comparison plots
  TFile* infile = new TFile(plots1->outfileName());

  PlotStuff* plots2 = new PlotStuff(infile, outputdir);

  plots2->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots2->setPlotType(PlotStuff::COMP);
  plots2->setHeaderText("","","");
  plots2->setYTitle("A.U.");
  plots2->setUnitScale();
  plots2->setHeaderText("","","");

  // ROC curves for cut variables
  PlotROCCurves* rocplots = new PlotROCCurves();

  vector<TString> splittypelabels = {"single parton, good split", "single parton, bad split", "multi parton, good split", "multi parton, bad split"};
  vector<TString> itertypelabels  = {"split, keep both", "split, drop subjet 2", "split, drop subjet 1"};
  vector<TString>           superjetm_names, subjetdrmin_names, ptratio_names;
  vector<vector<TString > > superjetm_names_byptbin, subjetdrmin_names_byptbin, ptratio_names_byptbin;

  vector<TString>           comprocnames_mass, comprocnames_drmin, comprocnames_ptratio;
  vector<TString>           comprocnames_mass_byptbin, comprocnames_drmin_byptbin, comprocnames_ptratio_byptbin;

  vector<Sample*> samples;
  PlotTools::initSamples(conffile, samples);

  TString siglabel1 = "#varepsilon(good splits, multi-parton)", bkglabel1 = "#varepsilon(bad splits, single-parton)";
  TString siglabel2 = "#varepsilon(good splits, multi-parton)", bkglabel2 = "#varepsilon(bad splits, multi-parton)";

  for(auto* sample : samples) {
    superjetm_names   = {"singlepart_shouldsplit_superjetm", "singlepart_badsplit_superjetm", "multipart_shouldsplit_superjetm", "multipart_badsplit_superjetm"};
    subjetdrmin_names = {"singlepart_shouldsplit_subjetdrmin", "singlepart_badsplit_subjetdrmin", "multipart_shouldsplit_subjetdrmin", "multipart_badsplit_subjetdrmin"};
    ptratio_names     = {"multipart_shouldsplittoboth_subjetptratio", "multipart_shouldsplittofirstonly_subjetptratio", "multipart_shouldsplittosecondonly_subjetptratio"};

    vector<TString> tmp_vec (splittypelabels.size(), "");
    for(unsigned int ibin = 0; ibin < fatjetptbins.size()-1; ++ibin) {
      superjetm_names_byptbin.push_back(tmp_vec);
      subjetdrmin_names_byptbin.push_back(tmp_vec);
      TString ptlabel = TString::Format("_fatjetpt%5.2fto%5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]);
      for(unsigned int in = 0; in < splittypelabels.size(); ++in) {
        superjetm_names_byptbin[ibin][in]   = superjetm_names[in]+ptlabel+"_"+sample->name;
        subjetdrmin_names_byptbin[ibin][in] = subjetdrmin_names[in]+ptlabel+"_"+sample->name;
      }
    }

    for(unsigned int in = 0; in < splittypelabels.size(); ++in) {
      superjetm_names[in]   += "_"+sample->name;
      subjetdrmin_names[in] += "_"+sample->name;
    }

    tmp_vec.resize(itertypelabels.size(), "");
    for(unsigned int ibin = 0; ibin < fatjetptbins.size()-1; ++ibin) {
      ptratio_names_byptbin.push_back(tmp_vec);
      TString ptlabel = TString::Format("_fatjetpt%5.2fto%5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]);
      for(unsigned int in = 0; in < itertypelabels.size(); ++in) {
        ptratio_names_byptbin[ibin][in] = ptratio_names[in]+ptlabel+"_"+sample->name;
      }
    }

    for(unsigned int in = 0; in < itertypelabels.size(); ++in) {
      ptratio_names[in] += "_"+sample->name;
    }

    plots2->addCompSet("superjetm_by_splittype_"+sample->name,   superjetm_names,   splittypelabels);
    plots2->addCompSet("subjetdrmin_by_splittype_"+sample->name, subjetdrmin_names, splittypelabels);
    plots2->addCompSet("ptratio_by_itertype_"+sample->name,      ptratio_names,     itertypelabels);

    rocplots->addROCVariable("superjet_mass_"+sample->name, "m(J)", plots1->outfileName(),
                             "multipart_shouldsplit_superjetm_"+sample->name, siglabel1,
                             "singlepart_badsplit_superjetm_"+sample->name, bkglabel1,
                             StyleTools::DefaultColors()["comp1"]);
    comprocnames_mass.push_back("superjet_mass_"+sample->name);

    rocplots->addROCVariable("subjet_drmin_"+sample->name, "#Delta R(j1, j2)", plots1->outfileName(),
                             "multipart_shouldsplit_subjetdrmin_"+sample->name, siglabel1,
                             "singlepart_badsplit_subjetdrmin_"+sample->name, bkglabel1,
                             StyleTools::DefaultColors()["comp1"]);
    comprocnames_drmin.push_back("subjet_drmin_"+sample->name);

    rocplots->addROCVariable("subjet_ptratio_"+sample->name, "p_{T}(j1)/(p_{T}(j1) + p_{T}(j2))", plots1->outfileName(),
                             "multipart_shouldsplittoboth_subjetptratio_"+sample->name, siglabel2,
                             "multipart_shouldsplittofirstonly_subjetptratio_"+sample->name, bkglabel2,
                             StyleTools::DefaultColors()["comp1"]);
    comprocnames_ptratio.push_back("subjet_ptratio_"+sample->name);

    comprocnames_mass_byptbin.clear(); comprocnames_drmin_byptbin.clear(); comprocnames_ptratio_byptbin.clear();

    for(unsigned int ibin = 0; ibin < fatjetptbins.size()-1; ++ibin) {
      TString ptlabel = TString::Format("_fatjetpt%5.2fto%5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]);
      TString colorname = TString::Format("comp%d", ibin+1);

      plots2->addCompSet("superjetm_by_splittype_"+ptlabel+"_"+sample->name,   superjetm_names_byptbin[ibin],   splittypelabels);
      plots2->addCompSet("subjetdrmin_by_splittype_"+ptlabel+"_"+sample->name, subjetdrmin_names_byptbin[ibin], splittypelabels);
      plots2->addCompSet("ptratio_by_itertype_"+ptlabel+"_"+sample->name,      ptratio_names_byptbin[ibin],     itertypelabels);

      rocplots->addROCVariable("superjet_mass_"+ptlabel+"_"+sample->name,
                               TString::Format("m(J), %5.2f < p_{T} < %5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]),
                               plots1->outfileName(),
                               "multipart_shouldsplit_superjetm"+ptlabel+"_"+sample->name, siglabel1,
                               "singlepart_badsplit_superjetm"+ptlabel+"_"+sample->name, bkglabel1,
                               StyleTools::DefaultColors()[colorname]);
      comprocnames_mass_byptbin.push_back("superjet_mass_"+ptlabel+"_"+sample->name);

      rocplots->addROCVariable("subjet_drmin_"+ptlabel+"_"+sample->name,
                               TString::Format("#Delta R(j1, j2), %5.2f < p_{T} < %5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]),
                               plots1->outfileName(),
                               "multipart_shouldsplit_subjetdrmin"+ptlabel+"_"+sample->name, siglabel1,
                               "singlepart_badsplit_subjetdrmin"+ptlabel+"_"+sample->name, bkglabel1,
                               StyleTools::DefaultColors()[colorname]);
      comprocnames_drmin_byptbin.push_back("subjet_drmin_"+ptlabel+"_"+sample->name);

      rocplots->addROCVariable("subjet_ptratio_"+ptlabel+"_"+sample->name,
                               TString::Format("p_{T}(j1)/(p_{T}(j1) + p_{T}(j2)), %5.2f < p_{T} < %5.2f", fatjetptbins[ibin], fatjetptbins[ibin+1]),
                               plots1->outfileName(),
                               "multipart_shouldsplittoboth_subjetptratio"+ptlabel+"_"+sample->name, siglabel2,
                               "multipart_shouldsplittofirstonly_subjetptratio"+ptlabel+"_"+sample->name, bkglabel2,
                               StyleTools::DefaultColors()[colorname]);
      comprocnames_ptratio_byptbin.push_back("subjet_ptratio_"+ptlabel+"_"+sample->name);
    }

    rocplots->addCompPlot("roccomp_ptbin_mass",    comprocnames_mass_byptbin);
    rocplots->addCompPlot("roccomp_ptbin_drmin",   comprocnames_drmin_byptbin);
    rocplots->addCompPlot("roccomp_ptbin_ptratio", comprocnames_ptratio_byptbin);

    superjetm_names.clear(); subjetdrmin_names.clear(); ptratio_names.clear();
    superjetm_names_byptbin.clear(); subjetdrmin_names_byptbin.clear(); ptratio_names_byptbin.clear();

  }
  rocplots->addCompPlot("roc_mass",    comprocnames_mass);
  rocplots->addCompPlot("roc_drmin",   comprocnames_drmin);
  rocplots->addCompPlot("roc_ptratio", comprocnames_ptratio);

  plots2->plot();

  rocplots->plotAll();

  delete plots1;
  delete plots2;
  delete rocplots;

}
