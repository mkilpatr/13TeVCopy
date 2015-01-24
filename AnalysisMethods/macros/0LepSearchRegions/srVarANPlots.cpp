{
	Plot::cache("*_ntuple_metPre_plots.root");
   TString T2bWPreselVars[] = {
       "met"               ,
       "num_j30"           ,
       "num_j70"           ,
       "num_tight_btags"   ,
       "min_dphi_j12_met"  ,
       "dphi_j3_met"       ,
       ""
   };

   TString T2bWMVAVars[] = {
       "met"               ,
       "num_j30"           ,
       "dphi_j3_met"       ,
       "num_medium_btags"  ,
       "q2_likeli"         ,
       "q1_likeli"         ,
       "quark_likeli"      ,
       "ht_along_over_away",
       "rms_pt"            ,
       "rms_dphi"          ,
       "bb_mass"           ,
       "mTb"               ,
       "deta_b_rms"        ,
       "leading_jj_mass"   ,
	   "mva"   ,
       ""
   };
   
   TString T2bWMVAVars[] = {
	   "mva"   ,
       ""
   };
   
   TString presel = "nMuETau";
   TString* vars = SRS;
   
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
	   p = new Plot("bkg_.*" + presel + "__"+vars[iV] + "$");
      p->toUnderOverflow();
	  double norm = p->at(0)->Integral();
	  norm += p->at(1)->Integral();
	  norm += p->at(2)->Integral();
      p->scale(1/norm);
	  
	  p->arrange("ttZ > z > t#tbar{t}");
	  
	  TH1 * s = Plot::getCache()->findOne("T2tt_850_100_signal_" + presel + "__"+vars[iV] + "$");
	  double sNorm = s->Integral();
      s->Scale(1/sNorm);
	  p->add(s,"T2tt_850_100");
	  p->rebin(2);
	  p->setYTitle("Normalized # of events");
      // p->normalize();
      o->Add(p);
      
   }
   Pint::drawAll(o,"","stack-{T2tt}",":i(.9,-1)");
   
}


{
	Plot::cache("*_ntuple_0lepSide_plots.root");
   TString T2bWPreselVars[] = {
       "met"               ,
       "num_j30"           ,
       "num_j70"           ,
       "num_tight_btags"   ,
       "min_dphi_j12_met"  ,
       "dphi_j3_met"       ,
       ""
   };

   TString T2bWMVAVars[] = {
       "met"               ,
       "num_j30"           ,
       "dphi_j3_met"       ,
       "num_medium_btags"  ,
       "q2_likeli"         ,
       "q1_likeli"         ,
       "quark_likeli"      ,
       "ht_along_over_away",
       "rms_pt"            ,
       "rms_dphi"          ,
       "bb_mass"           ,
       "mTb"               ,
       "deta_b_rms"        ,
       "leading_jj_mass"   ,
	   "mva"   ,
       ""
   };
   
   TString SRS[] = {
	   "mva"   ,
       ""
   };
   
      TString presel = "selEMu__passDPhi";
   // TString presel = "nMuETau";
   TString* vars = T2bWMVAVars;
   
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
	   p = new Plot("^ttbar_.*" + presel + "__"+vars[iV] + "$");
      p->toUnderOverflow();
	  double norm = p->at(0)->Integral();
	  // norm += p->at(1)->Integral();
	  // norm += p->at(2)->Integral();
	  // p->scale(1/norm);
	  
	  p->arrange("ttZ > z > t#tbar{t}");
	  
	  TH1 * s = Plot::getCache()->findOne("T2tt_850_100_signal_" + presel + "__"+vars[iV] + "$");
	  double sNorm = s->Integral();
	  // s->Scale(1/sNorm);
	  p->add(s,"T2tt_850_100");
	  p->setYTitle("Normalized # of events");
      // p->normalize();
      o->Add(p);
      
   }
   Pint::drawAll(o,"","",":i(.9,-1)");
   
}

{
	Plot::cache("*_plots.root");
	TH1 * v = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMuETau__mva");
	TH1 * s = Plot::getCache()->findOne("ttbar_ntuple_0lepSide_ttbar_selEMu__mva");
	TH1 * sd = Plot::getCache()->findOne("ttbar_ntuple_0lepSide_ttbar_selEMu__passDPhi__mva");
	p = new Plot();
	p->add(v,"MET trigger, leptons vetoed");
	p->add(s,"Lepton trigger, 1 selected lepton");
	p->add(sd,"Lepton trigger, 1 selected lepton + DPhi");
	
	p->rebin(10);
	
	p->draw();
	
	
	new TCanvas();
	p = (Plot*)p->Clone();
	p->normalize();
	p->draw();
	
	new TCanvas();
	p = p->makeRatiogram(0u,"");
	p->draw();
}

{
	Plot::cache("*_plots.root");
	TH1 * v = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMuETau__mva");
	TH1 * sd = Plot::getCache()->findOne("ttbar_ntuple_0lepSide_ttbar_selEMu__passDPhi__mva");
	
	v ->Rebin(10);
	sd->Rebin(10);
	
	
	int binN = 20;
	
	double sLC = sd->GetBinContent(binN);
	double sLE = sd->GetBinError(binN);

	double closureUnc =     TMath::Sqrt(Prescriptions::ratioError2(sLC,sLC,sLC,sLE*sLE)); 
    

    
    

    TH1 * selMu  = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_incl__selMu__lepSF_nEvts");
    TH1 * selE   = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMu__selE__lepSF_nEvts");
    TH1 * selTau = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMuE__selTau__lepSF_nEvts");
    
    double mSFM2 = Prescriptions::ratioError2(selMu->GetBinContent(1),selMu->GetBinContent(1),selMu->GetBinContent(1),selMu->GetBinError(1)*selMu->GetBinError(1));
    double mSFE2 = Prescriptions::ratioError2(selE->GetBinContent(1),selE->GetBinContent(1),selE->GetBinContent(1),selE->GetBinError(1)*selE->GetBinError(1));
    double mSFT2 = Prescriptions::ratioError2(selTau->GetBinContent(1),selTau->GetBinContent(1),selTau->GetBinContent(1),selTau->GetBinError(1)*selTau->GetBinError(1));
    
    
    TH1 * vMu  = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_incl__selMu__mva");
    TH1 * vE   = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMu__selE__mva");
    TH1 * vTau = Plot::getCache()->findOne("bkg_ntuple_metPre_ttbar_nMuE__selTau__mva");
    
    vMu ->Rebin(10);
    vE  ->Rebin(10);
    vTau->Rebin(10);
    
    double lepE = TMath::Sqrt(vMu->GetBinContent(binN)*vMu->GetBinContent(binN)*mSFM2 + vE->GetBinContent(binN)*vE->GetBinContent(binN)*mSFE2 + vTau->GetBinContent(binN)*vTau->GetBinContent(binN)*mSFT2);
    
    double totE = TMath::Sqrt(v->GetBinError(binN)*v->GetBinError(binN) + closureUnc*closureUnc + lepE*lepE);
	
	cout << TString::Format("\n%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",v->GetBinContent(binN),v->GetBinError(binN),closureUnc,lepE,totE);
	
}


{
	Plot::cache("ttbar_ntuple_1lepSide_plots.root");
    
    double bins[] = {-20,1,7,20};
    RebinToNewBins b(3,bins);
    
    TString nJets[] = {"nJets_eq4to7__","nJets_geq7__",""};
    TString bJets[] = {"nBJets_eq1__","nBJets_eq2__","nBJets_geq3__",""};
    TString mets[] = {"met_eq150to250__","met_eq250to350__","met_geq350__",""};
    
    o = new TObjArray;
    oo = new TObjArray;

    for(unsigned int iN = 0; nJets[iN][0]; ++iN)
        for(unsigned int iB = 0; bJets[iB][0]; ++iB)
            for(unsigned int iM = 0; mets[iM][0]; ++iM){
                p = new Plot("ttbar_(sel1EMu|sel2EMu)__"+nJets[iN]+bJets[iB]+ mets[iM] + "topness$");
                p->SetTitle(Plot::translated(nJets[iN]+bJets[iB]+ mets[iM]));
                p->normalize();
                o->Add(p);
                p = p->makeRatiogram(0u,"",b);
                oo->Add(p);
            }
            
            Pint::drawAll(o,"");
            Pint::drawAll(oo,"");
	
}

{
Plot::cache("ttbar_ntuple_1lepSide_plots.root");

double bins[] = {-20,1,7,20};
RebinToNewBins b(3,bins);

TString nJets[] = {"nJets_eq4to7__","nJets_geq7__",""};
TString bJets[] = {"nBJets_eq1__","nBJets_eq2__","nBJets_geq3__",""};
TString mets[] = {"met_eq150to250__","met_eq250to350__","met_geq350__",""};
// TString tn[] = {"topness_lt1__","topness_eq1to7__","topness_geq7__",""};
TString tn[] = {"topness_geq7__",""};

    for(unsigned int iB = 0; bJets[iB][0]; ++iB)
for(unsigned int iN = 0; nJets[iN][0]; ++iN)

        for(unsigned int iM = 0; mets[iM][0]; ++iM)
            for(unsigned int iT = 0; tn[iT][0]; ++iT)
    {
        
        TH1 * si = Plot::getCache()->findOne("ttbar_sel1EMu__"+nJets[iN]+bJets[iB]+ mets[iM] +tn[iT]+ "nEvts$");
        TH1 * di = Plot::getCache()->findOne("ttbar_sel2EMu__"+nJets[iN]+bJets[iB]+ mets[iM] +tn[iT]+ "nEvts$");
        double closureUnc =     TMath::Sqrt(Prescriptions::ratioError2(di->GetBinContent(1),di->GetBinContent(1),di->GetBinContent(1),di->GetBinError(1)*di->GetBinError(1))); 
        double totalE = TMath::Sqrt(si->GetBinError(1)*si->GetBinError(1)+closureUnc*closureUnc );
        cout << Plot::translated(nJets[iN]+bJets[iB]+ mets[iM]) <<"\t";
        cout << TString::Format("%.2f\t%.2f\t%.2f\t%.2f\n",si->GetBinContent(1),si->GetBinError(1),closureUnc,totalE);

        }



}

