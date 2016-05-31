void mainfunction() {

  TFile *file = TFile::Open("getinclnorm_test/tw.root","READONLY");
  TTree *tree = (TTree*)file->Get("Events");


  float weight_;
  float wpolwgtup_;
  float wpolwgtdn_;
  tree->SetBranchAddress("weight",&weight_);
  tree->SetBranchAddress("wpolwgtup",&wpolwgtup_);
  tree->SetBranchAddress("wpolwgtdn",&wpolwgtdn_);


  double sumwgt      = 0.;
  double sumwgtpolup = 0.;
  double sumwgtpoldn = 0.;
  for (unsigned int i0=0; i0<tree->GetEntries(); ++i0) {
    //for (unsigned int i0=0; i0<100; ++i0) {                                                                                                                                
    tree->GetEntry(i0);

    sumwgt += weight_;
    sumwgtpolup += weight_*wpolwgtup_;
    sumwgtpoldn += weight_*wpolwgtdn_;
    //    cout << weight_ << " " << wpolwgtup_ << " " << weight_*wpolwgtup_ << "\n";                                                                                         
  }
  cout << "sumwgt,sumwgtpolup,sumwgtpoldn final = " << sumwgt << " " << sumwgtpolup << " " << sumwgtpoldn << "\n";
  
  TH1F *hmet_wgt      = new TH1F("hmet_wgt","hmet_wgt",100,0.,1000.);           hmet_wgt->SetLineColor(1);
  TH1F *hmet_wgtpolup = new TH1F("hmet_wgtpolup","hmet_wgtpolup",100,0.,1000.); hmet_wgtpolup->SetLineColor(2);
  TH1F *hmet_wgtpoldn = new TH1F("hmet_wgtpoldn","hmet_wgtpoldn",100,0.,1000.); hmet_wgtpoldn->SetLineColor(4);
  tree->Project("hmet_wgt","met","weight");
  tree->Project("hmet_wgtpolup","met","weight*wpolwgtup");
  tree->Project("hmet_wgtpoldn","met","weight*wpolwgtdn");

  TCanvas *cmet = new TCanvas("cmet","cmet",500,500);
  hmet_wgt->Draw("HIST E0");
  hmet_wgtpolup->Draw("HIST E0 sames");
  hmet_wgtpoldn->Draw("HIST E0 sames");
  

}
