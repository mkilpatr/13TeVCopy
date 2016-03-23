#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

// Procedure: Entries 9-108 in the systweights vector correspond to pdf variations at fixed alpha_S, 109 and 110 correspond to variations of alpha_S. For the pdf variations, we take the standard deviation of the 100 variations, then add in quadrature the maximum alpha_S variation. In order to factor out the effects on the cross section, yields should be normalized to the inclusive yield. To be done properly, input trees should have no pre-selection
//

void getPdfUncertaintiesTTZ(const TString inputdir = "/uscms_data/d3/hqu/trees/050316/sr",
                            const TString outputdir = "plots_bkgest/sysvars",
                            const TString lumistr = "2.318",
                            const TString format = "pdf",
                            vector<TString> samples = {"ttZ_only","tZq"},
                            const bool    combinesamples = true,
                            const bool    plotlog = true)
{

  gSystem->mkdir(outputdir,true);

  //TString basewgt = lumistr + "*weight*truePUWeight*btagWeight*leptnpweight*lepvetoweight";
  TString basewgt = lumistr + "*weight";

  map<TString,TString> sel;

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["njets"]        = " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["highmt"]       = " && mtcsv12met>175";
  sel["mednj"]        = " && njets>=5 && njets<7";
  sel["highnj"]       = " && njets>=7";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  sel["lowmt_mednj"]        = sel["sr"] + sel["lowmt"] + sel["mednj"];
  sel["lowmt_highnj"]       = sel["sr"] + sel["lowmt"] + sel["highnj"];
  sel["highmt_mednj_nt0"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"];
  sel["highmt_highnj_nt0"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"];
  sel["highmt_nt1"]         = sel["sr"] + sel["highmt"] + sel["nt1"];
  sel["lowmt_mednj_nb1"]        = sel["sr"] + sel["lowmt"] + sel["mednj"] + sel["nb1"];
  sel["lowmt_mednj_nb2"]        = sel["sr"] + sel["lowmt"] + sel["mednj"] + sel["nb2"];
  sel["lowmt_highnj_nb1"]       = sel["sr"] + sel["lowmt"] + sel["highnj"] + sel["nb1"];
  sel["lowmt_highnj_nb2"]       = sel["sr"] + sel["lowmt"] + sel["highnj"] + sel["nb2"];
  sel["highmt_mednj_nt0_nb1"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_mednj_nt0_nb2"]   = sel["sr"] + sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_highnj_nt0_nb1"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_highnj_nt0_nb2"]  = sel["sr"] + sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_nt1_nb1"]         = sel["sr"] + sel["highmt"] + sel["nt1"] + sel["nb1"];
  sel["highmt_nt1_nb2"]         = sel["sr"] + sel["highmt"] + sel["nt1"] + sel["nb2"];

  vector<TString> srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "lowmt_highnj_nb1", "lowmt_highnj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_highnj_nt0_nb1", "highmt_highnj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};

  const int nbins = 5;
  double metbins[nbins+1] = {250.,300.,400.,500.,600.,1000.};

  vector<TString> binlabels = {"5_1_0_0", "5_2_0_0", "7_1_0_0", "7_2_0_0", "5_1_0_175", "5_2_0_175", "7_1_0_175", "7_2_0_175", "5_1_1_175", "5_2_1_175"};

  TFile* infile = 0;
  TTree* intree = 0;

  TFile* outfile = new TFile(outputdir + "/pdfvars.root","RECREATE");

  FILE * txtfile = fopen(outputdir + "/pdfuncs.txt","w");

  for(auto sample : samples) {
    infile = TFile::Open(inputdir + "/" + sample + "_tree.root");
    assert(infile);
    intree = (TTree*)infile->Get("Events");
    assert(intree);
    TH1F* hbase = new TH1F("base_"+sample,"",1,0,1000);
    hbase->Sumw2();
    intree->Project("base_"+sample,"met","weight");
    double baseint = hbase->Integral(0,2);
    cout << sample.Data() << "\n\tBase Norm = " << baseint << endl;
    vector<double> varnorms;
    for(int iwgt = 9; iwgt < 111; ++iwgt) {
      TH1F* hvarbase = new TH1F(TString::Format("base_var%d_%s", iwgt, sample.Data()),"",1,0,1000);
      hvarbase->Sumw2();
      intree->Project(hvarbase->GetName(),"met",TString::Format("weight*systweights[%d]",iwgt));
      double varint = hvarbase->Integral(0,2);
      cout << "\tVar " << iwgt << " Norm = " << varint << endl;
      varnorms.push_back(baseint/varint);
    }
    int isrbin = 0;
    for(auto srbin : srbins) {
      cout << sample << ": " << srbin << endl;
      TString hname = "nominal_"+srbin+"_"+sample;
      TH1F* h = new TH1F(hname,"",5,metbins);
      h->Sumw2();
      intree->Project(hname,"met",basewgt+"*("+sel[srbin]+")");
      Plot::addOverFlow(h);
      outfile->cd();
      h->Write();
      vector<TH1F*> hvars;
      TH1F* pdfvar = 0;
      cout << "\tNominal: " << h->Integral(0,-1) << endl;
      for(int iwgt = 9; iwgt < 111; ++iwgt) {
        TString cutstr = TString::Format("%s*systweights[%d]*(%s)", basewgt.Data(), iwgt, sel[srbin].Data());
        TString hvarname = TString::Format("var%d_%s_%s", iwgt, srbin.Data(), sample.Data());
        TH1F* hvar = new TH1F(hvarname,"",5,metbins);
        hvar->Sumw2();
        intree->Project(hvarname,"met",cutstr);
        hvar->Scale(varnorms[iwgt-9]);
        Plot::addOverFlow(hvar);
        outfile->cd();
        hvar->Write();
        hvars.push_back(hvar);
        if(!pdfvar) {
          pdfvar = (TH1F*)hvar->Clone("pdfvar_"+srbin+"_"+sample);
        } else if(iwgt<109) {
          pdfvar->Add(hvar);
        }
        cout << "\tVar " << iwgt << ": " << hvar->Integral(0,-1) << endl;
      }
      cout << hvars.size() << " variations" << endl;
      pdfvar->Scale(1.0/float(hvars.size()));
      for(int ibin = 1; ibin < pdfvar->GetNbinsX()+1; ++ibin) {
        double y_imean = pdfvar->GetBinContent(ibin);
        cout << "Bin " << ibin << ": " << y_imean << "\t";
        double variance = 0.0;
        for(unsigned int ivar=0; ivar < hvars.size()-2; ++ivar) {
          double y_in = hvars[ivar]->GetBinContent(ibin);
          variance += pow((y_in-y_imean),2);
        }
        cout << variance << "/" << hvars.size()-3 << " = ";
        variance /= float(hvars.size()-3);
        cout << variance << "\t";
        double asvar = max(fabs(hvars[hvars.size()-1]->GetBinContent(ibin) - h->GetBinContent(ibin)),fabs(hvars[hvars.size()-2]->GetBinContent(ibin) - h->GetBinContent(ibin)));
        cout << hvars[hvars.size()-1]->GetBinContent(ibin) << ", " << hvars[hvars.size()-2]->GetBinContent(ibin) << " - " << h->GetBinContent(ibin) << ": " << asvar << "\t";
        pdfvar->SetBinContent(ibin, h->GetBinContent(ibin));
        pdfvar->SetBinError(ibin, sqrt(variance + (asvar*asvar)));
        cout << pdfvar->GetBinContent(ibin) << " +/- " << pdfvar->GetBinError(ibin) << endl;
      }

      outfile->cd();
      TGraphAsymmErrors* gr = new TGraphAsymmErrors(pdfvar);
      for(int ibin = 0; ibin < gr->GetN(); ++ibin) {
        double varup = gr->GetEYhigh()[ibin];
        double vardown = gr->GetEYlow()[ibin];
        if(varup != vardown) cout << varup << " != " << vardown << "!" << endl;
        double maxsys = 1.0 + (varup/h->GetBinContent(ibin+1));
        printf("bin_%d_%s\tpdfunc\t%s\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sample.Data(), maxsys);
        fprintf(txtfile, "bin_%d_%s\tpdfunc\t%s\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), sample.Data(), maxsys);
      }
      outfile->cd();
      gr->Write(TString(h->GetName()).Append("_varenvelope"));
      isrbin++;
    }
  }

  //outfile->Close();

  if(combinesamples) {
    cout << "Combined" << endl;
    int isrbin = 0;
    for(auto srbin : srbins) {
      TH1F *hcomb = 0;
      for(auto sample : samples) {
        if(!hcomb)  hcomb    = (TH1F*)outfile->Get("pdfvar_"+srbin+"_"+sample)->Clone("combined_"+srbin);
        else        hcomb->Add((TH1F*)outfile->Get("pdfvar_"+srbin+"_"+sample));
      }
      hcomb->Write();
      TGraphAsymmErrors* gr = new TGraphAsymmErrors(hcomb);
      for(int ibin = 0; ibin < gr->GetN(); ++ibin) {
        double var = 1.0 + (gr->GetEYhigh()[ibin]/gr->GetY()[ibin]);
        printf("bin_%d_%s\tpdfunc\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), var);
        fprintf(txtfile, "bin_%d_%s\tpdfunc\t%4.3f\n", int(gr->GetX()[ibin] - gr->GetEXlow()[ibin]), binlabels[isrbin].Data(), var);
      }
      outfile->cd();
      gr->Write(TString(hcomb->GetName()).Append("_varenvelope"));
      isrbin++;
    }
  }

  fclose(txtfile);

  PlotStuff* plots = new PlotStuff(outfile, outputdir);
  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::COMP);
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setYTitle("Events");
  if(plotlog) plots->setLogy();
  plots->setFormat(format);
  plots->setGraphDrawOption("P2");

  for(auto sample : samples) {
    for(auto srbin : srbins) {
      plots->addCompSet(sample+"_"+srbin+"_pdfvar", {"nominal_"+srbin+"_"+sample+"_varenvelope"},{"Max. PDF variation"},-1.,PlotStuff::PlotComp::GRAPHCOMP);
    }
  }
  if(combinesamples) {
    for(auto srbin : srbins) {
      plots->addCompSet("combined_"+srbin+"_pdfvar", {"combined_"+srbin+"_varenvelope"},{"Max. PDF variation"},-1.,PlotStuff::PlotComp::GRAPHCOMP);
    }
  }
  plots->plot();

}
