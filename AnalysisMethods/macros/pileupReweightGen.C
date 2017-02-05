//+______________________________________________________________________________
// Generate weights based on two input distributions, intended and generated


#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <vector>

const TH1D* getPUWeight(TString data_pu, TString syst_type) {

  const int N_PU_BINS = 75;

  TH1D* pu_generated = new TH1D("pu_generated","Generated pileup distribution (i.e., MC)",N_PU_BINS,0,N_PU_BINS);

  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData
  TFile* data_pufile = new TFile(data_pu);
  TH1D* data_puhist = (TH1D*)data_pufile->Get("pileup");

  double x;

  // https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/SimGeneral/MixingModule/python/mix_2016_25ns_Moriond17MC_PoissonOOTPU_cfi.py
  std::vector<double> probdist_Moriond17MC_25ns {
    1.78653e-05 ,2.56602e-05 ,5.27857e-05 ,8.88954e-05 ,0.000109362 ,0.000140973 ,0.000240998 ,0.00071209 ,0.00130121 ,0.00245255 ,0.00502589 ,0.00919534 ,0.0146697 ,0.0204126 ,0.0267586 ,0.0337697 ,0.0401478 ,0.0450159 ,0.0490577 ,0.0524855 ,0.0548159 ,0.0559937 ,0.0554468 ,0.0537687 ,0.0512055 ,0.0476713 ,0.0435312 ,0.0393107 ,0.0349812 ,0.0307413 ,0.0272425 ,0.0237115 ,0.0208329 ,0.0182459 ,0.0160712 ,0.0142498 ,0.012804 ,0.011571 ,0.010547 ,0.00959489 ,0.00891718 ,0.00829292 ,0.0076195 ,0.0069806 ,0.0062025 ,0.00546581 ,0.00484127 ,0.00407168 ,0.00337681 ,0.00269893 ,0.00212473 ,0.00160208 ,0.00117884 ,0.000859662 ,0.000569085 ,0.000365431 ,0.000243565 ,0.00015688 ,9.88128e-05 ,6.53783e-05 ,3.73924e-05 ,2.61382e-05 ,2.0307e-05 ,1.73032e-05 ,1.435e-05 ,1.36486e-05 ,1.35555e-05 ,1.37491e-05 ,1.34255e-05 ,1.33987e-05 ,1.34061e-05 ,1.34211e-05 ,1.34177e-05 ,1.32959e-05 ,1.33287e-05
  };

  //Fill histograms with one generated, one desired distribution
  const std::vector<double> &probdist = probdist_Moriond17MC_25ns;

  for (int i=1;i<N_PU_BINS+1;i++) {
    pu_generated->SetBinContent(i,probdist.at(i-1));
  }

  pu_generated->Scale(1.0/pu_generated->Integral(0,pu_generated->GetNbinsX()+1));
  data_puhist->Scale(1.0/data_puhist->Integral(0,data_puhist->GetNbinsX()+1));

  data_puhist->Divide(pu_generated);

  std::cout << "Bin Contents" << std::endl;

  for(int ibin=1; ibin<N_PU_BINS+1; ibin++){

    x = data_puhist->GetBinContent(ibin);

    std::cout << ibin-1 << " " << x << std::endl;

  }

  TString hname = "puWeight_" + syst_type;
  data_puhist->SetNameTitle(hname,hname);

  return data_puhist;

}

void pileupReweightGen(){
  TFile* outfile = new TFile("truePUWeights_2016_36p8ifb_75bins_69p2mb.root","RECREATE");
  const TH1D * data_puhist;

  // nominal
  data_puhist = getPUWeight("pileup_data_2016_36p8ifb_75bins_69p2mb.root", "nominal");
  outfile->cd();
  data_puhist->Write();
  // up (minbias xsec +5%)
  data_puhist = getPUWeight("pileup_data_2016_36p8ifb_75bins_65p74mb.root", "up");
  outfile->cd();
  data_puhist->Write();
  // down (minbias xsec -5%)
  data_puhist = getPUWeight("pileup_data_2016_36p8ifb_75bins_72p66mb.root", "down");
  outfile->cd();
  data_puhist->Write();

  outfile->Close();
}
