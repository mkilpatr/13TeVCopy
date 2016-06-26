//+______________________________________________________________________________
// Generate weights based on two input distributions, intended and generated


#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>


void reweight_gen() {

  TH1D* pu_generated = new TH1D("pu_generated","Generated pileup distribution (i.e., MC)",50,0,50);

  TFile* data_pufile = new TFile("pileup_data_2016b_4ifb_50bins_69p735mb.root");
  TH1D* data_puhist = (TH1D*)data_pufile->Get("pileup");

  double x;

  double probdistSpring1625ns[50] = {
		0.000829312873542,
 		0.00124276120498,
 		0.00339329181587,
 		0.00408224735376,
 		0.00383036590008,
		0.00659159288946,
 		0.00816022734493,
 		0.00943640833116,
 		0.0137777376066,
 		0.017059392038,
 		0.0213193035468,
 		0.0247343174676,
 		0.0280848773878,
 		0.0323308476564,
 		0.0370394341409,
 		0.0456917721191,
 		0.0558762890594,
 		0.0576956187107,
 		0.0625325287017,
 		0.0591603758776,
 		0.0656650815128,
 		0.0678329011676,
 		0.0625142146389,
 		0.0548068448797,
 		0.0503893295063,
 		0.040209818868,
 		0.0374446988111,
 		0.0299661572042,
 		0.0272024759921,
 		0.0219328403791,
 		0.0179586571619,
 		0.0142926728247,
 		0.00839941654725,
 		0.00522366397213,
 		0.00224457976761,
 		0.000779274977993,
 		0.000197066585944,
 		7.16031761328e-05,
 		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
 		0.0,
 		0.0,
		0.0};

  double probdistSummer1525ns[52] = {
                           4.8551E-07,
                           1.74806E-06,
                           3.30868E-06,
                           1.62972E-05,
                           4.95667E-05,
                           0.000606966,
                           0.003307249,
                           0.010340741,
                           0.022852296,
                           0.041948781,
                           0.058609363,
                           0.067475755,
                           0.072817826,
                           0.075931405,
                           0.076782504,
                           0.076202319,
                           0.074502547,
                           0.072355135,
                           0.069642102,
                           0.064920999,
                           0.05725576,
                           0.047289348,
                           0.036528446,
                           0.026376131,
                           0.017806872,
                           0.011249422,
                           0.006643385,
                           0.003662904,
                           0.001899681,
                           0.00095614,
                           0.00050028,
                           0.000297353,
                           0.000208717,
                           0.000165856,
                           0.000139974,
                           0.000120481,
                           0.000103826,
                           8.88868E-05,
                           7.53323E-05,
                           6.30863E-05,
                           5.21356E-05,
                           4.24754E-05,
                           3.40876E-05,
                           2.69282E-05,
                           2.09267E-05,
                           1.5989E-05,
                           4.8551E-06,
                           2.42755E-06,
                           4.8551E-07,
                           2.42755E-07,
                           1.21378E-07,
                           4.8551E-08};

  //Fill histograms with one generated, one desired distribution
  
  for (int i=1;i<51;i++) {
    pu_generated->SetBinContent(i,probdistSpring1625ns[i-1]);
  }

  pu_generated->Scale(1.0/pu_generated->Integral(0,pu_generated->GetNbinsX()+1));
  data_puhist->Scale(1.0/data_puhist->Integral(0,data_puhist->GetNbinsX()+1));

  data_puhist->Divide(pu_generated);

  std::cout << "Bin Contents" << std::endl;

  for(int ibin=1; ibin<51; ibin++){

    x = data_puhist->GetBinContent(ibin);

    std::cout << ibin-1 << " " << x << std::endl;

  }

  data_puhist->SetNameTitle("puWeight","puWeight");

  TFile* outfile = new TFile("puWeights_2016b_4ifb_50bins_69p735mb.root","RECREATE");
  outfile->cd();
  data_puhist->Write("puWeight");
  outfile->Close();

}

