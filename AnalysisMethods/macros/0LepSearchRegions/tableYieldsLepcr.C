#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include <iostream>
#include <iomanip>
#endif

vector<TString> regions = {"lowmt_mednj", "lowmt_highnj", "highmt_mednj_nt0", "highmt_highnj_nt0", "highmt_nt1"};
vector<TString> bkgs      = {/*"ttZ"      , "qcd", "znunu"             ,*/ "ttW"      , "tW", "wjets" , "ttbar"   };
vector<TString> bkglabels = {/*"t#bar{t}Z", "QCD", "Z#rightarrow#nu#nu",*/ "t#bar{t}W", "tW", "W+jets", "t#bar{t}"};
vector<TString> sigs      = {"nb1"         , "nb2"            };
vector<TString> siglabels = {"t#bar{t}/W (SR, N_{b}=1)", "t#bar{t}/W (SR, N_{b}#geq2)"};

vector<TString> regionHeaders = { "$\\mtb < 175$~\\GeV, $5-6$ jets",
                                  "$\\mtb < 175$~\\GeV, $\\geq 7$ jets",
                                  "$\\mtb > 175$~\\GeV, $\\nt=0$, $5-6$ jets",
                                  "$\\mtb > 175$~\\GeV, $\\nt=0$, $\\geq 7$ jets",
                                  "$\\mtb > 175$~\\GeV, $\\nt \\geq 1$, $\\geq 5$ jets"
                                };

vector<TString> metRows = { "", "250-300", "300-400", "400-500", "500-600", "$>$600" };

/*
 * Makes the bulk of the "yields_lepcr.tex" table.
 *
 * To make the input histograms to this macro, run getZeroLeptonPrediction after uncommenting
 * the ttbar, wjets, tW, and ttW lines in run0lepbkgpred.conf (note that the root files for these
 * trees will need to be linked from <treeDir>/sr/ to <treeDir> similar to how the other trees
 * are). Then hadd the 0L and onelepcr trees together to get the input file for this macro:
 *
 * $ hadd output_0l_plus_lepcr.root output_0l.root output_lepcr.root
 */
void tableYieldsLepcr(const TString inputDir      = "plots_bkgest_split_ttbarplusw",
                      const TString inputFileName = "output_0l_plus_lepcr.root"
                     )
{

  TFile* infile = new TFile(inputDir + "/" + inputFileName);

  for(unsigned int ireg=0; ireg<regions.size(); ++ireg) {
    auto region = regions[ireg];
    cout << "\\hline\n"
         << "\\multicolumn{8}{c}{" << regionHeaders[ireg] << "} \\\\\n"
         << "\\hline" << endl;

    TH1F* data  = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_data;1");
    TH1F* ttbar = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_ttbar;1");
    TH1F* wjets = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_wjets;1");
    TH1F* tw    = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_tW;1");
    TH1F* ttw   = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_ttW;1");
    TH1F* ttz   = (TH1F*)infile->Get("met_lepcr_nbgeq1_"+region+"_znunu;1");


    double int_data=0, int_ttbar=0, int_wjets=0, int_tw=0, int_ttw=0, int_ttz=0, int_bkg=0;
    double             err_ttbar=0, err_wjets=0, err_tw=0, err_ttw=0, err_ttz=0, err_bkg=0;
    for(int i=1; i<=5; ++i) {
      int_data  += data ->GetBinContent(i);
      int_ttbar += ttbar->GetBinContent(i); err_ttbar += ttbar->GetBinError(i) * ttbar->GetBinError(i);
      int_wjets += wjets->GetBinContent(i); err_wjets += wjets->GetBinError(i) * wjets->GetBinError(i);
      int_tw    += tw   ->GetBinContent(i); err_tw    += tw   ->GetBinError(i) * tw   ->GetBinError(i);
      int_ttw   += ttw  ->GetBinContent(i); err_ttw   += ttw  ->GetBinError(i) * ttw  ->GetBinError(i);
      int_ttz   += ttz  ->GetBinContent(i); err_ttz   += ttz  ->GetBinError(i) * ttz  ->GetBinError(i);
      double bkg = ttbar->GetBinContent(i) + wjets->GetBinContent(i) + tw   ->GetBinContent(i) + ttz  ->GetBinContent(i);
      double err =sqrt( ttbar->GetBinError(i) * ttbar->GetBinError(i)
                      + wjets->GetBinError(i) * wjets->GetBinError(i)
                      + tw   ->GetBinError(i) * tw   ->GetBinError(i)
                      + ttw  ->GetBinError(i) * ttw  ->GetBinError(i)
                      + ttz  ->GetBinError(i) * ttz  ->GetBinError(i) );
      int_bkg += bkg;
      err_bkg += err*err;
      if(ireg==4) continue;
      cout << metRows[i] << " & "
           << fixed << setprecision(0)
           << data ->GetBinContent(i) << " &   "
           << setprecision(2)
           << ttbar->GetBinContent(i) << " $\\pm$ " << ttbar->GetBinError(i) << " & "
           << wjets->GetBinContent(i) << " $\\pm$ " << wjets->GetBinError(i) << " & "
           << tw   ->GetBinContent(i) << " $\\pm$ " << tw   ->GetBinError(i) << " & "
           << ttw  ->GetBinContent(i) << " $\\pm$ " << ttw  ->GetBinError(i) << " & "
           << ttz  ->GetBinContent(i) << " $\\pm$ " << ttz  ->GetBinError(i) << " & "
           << bkg                     << " $\\pm$ " << err                   << " \\\\"
           << endl;
    }

    if(ireg==4) cout << "$>$250 & "
                     << fixed << setprecision(0)
                     << int_data  << " & "
                     << setprecision(2)
                     << int_ttbar << " $\\pm$ " << sqrt(err_ttbar) << " & "
                     << int_wjets << " $\\pm$ " << sqrt(err_wjets) << " & "
                     << int_tw    << " $\\pm$ " << sqrt(err_tw   ) << " & "
                     << int_ttw   << " $\\pm$ " << sqrt(err_ttw  ) << " & "
                     << int_ttz   << " $\\pm$ " << sqrt(err_ttz  ) << " & "
                     << int_bkg   << " $\\pm$ " << sqrt(err_bkg  ) << " \\\\"
                     << endl;
  } // regions
  cout << "\\hline" << endl;

}
