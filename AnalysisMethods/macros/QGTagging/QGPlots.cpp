root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/QGTagging/processQGInvestigations.C+("ZnunuQGTree.root")'
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/QGTagging/flattenQGTree.C+("ZnunuQGTree.root","TestAnalyzer/Events","ZnunuQGTree_flatGen.root",false)'
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/QGTagging/flattenQGTree.C+("ZnunuQGTree.root","TestAnalyzer/Events","ZnunuQGTree_flatReco.root",true)'

root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/QGTagging/produceQGMVA.C+("ZnunuQGTree_flatGen.root","Events","QGDisc_gen.root",false)'
root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/QGTagging/produceQGMVA.C+("ZnunuQGTree_flatReco.root","Events","QGDisc_reco.root",true)'


   //Q/G plots                              
{
   Plot::cache("13TeV.root");
   p = new Plot("13TeV_signal_T2bW_preselection__.*__jet_qgl");
   p->drop("b");
   p->normalize();
   p->rebin(4);
   
   p->draw();
}


//Plot all
{
   TFile *f8 = new TFile("8TeV.root","read");
   TFile *f13 = new TFile("13TeV.root","read");
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
       ""
   };
   
   TString presel = "T2bW_preselection";
   TString* vars = T2bWMVAVars;
   
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      TH1 * d8  = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_data");
      TH1 * tw8 = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_tw");
      TH1 * nt8 = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_nont");
      TH1 * t13 = (TH1*)f13->Get("ttbar_" + presel + "__"+vars[iV]);
      TH1 * s13 = (TH1*)f13->Get("signal_" + presel + "__"+vars[iV]);
      cout << "8TeV_mva_vars_" + presel + "__"+vars[iV]+"_data"<<endl;
      cout << d8 <<" "<<tw8<<" "<<nt8<<" "<<t13<<" "<<s13<<endl;
      p = new Plot();
      d8->Add(nt8, -1);
      
      p->add(d8,"8TeV data - non ttbar + W MC");
      p->add(tw8,"8TeV ttbar + W MC");
      p->add(t13,"13TeV ttbar MC");
      p->add(s13,"13TeV signal");
      p->toUnderOverflow();
      p->normalize();
      p->SetTitle(presel);
      o->Add(p);
      
      
   }
   Pint::drawAll(o,"");
   
}

//Plot select
{
   TFile *f8 = new TFile("8TeV.root","read");
   TFile *f13 = new TFile("13TeV.root","read");

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
       ""
   };
   
   TString selVars[] = {
       "met"               ,
       "num_j30"           ,
       "num_medium_btags"  ,
       "ht_along_over_away",
       "mTb"               ,
       "deta_b_rms"        ,
       "leading_jj_mass"   ,
       ""
   };   
   TString presel = "T2bW_preselection";
   TString* vars = selVars;
   

   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      o = new TObjArray;
      TH1 * d8  = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_data");
      TH1 * tw8 = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_tw");
      TH1 * nt8 = (TH1*)f8->Get("mva_vars_" + presel + "__"+vars[iV]+"_nont");
      TH1 * t13 = (TH1*)f13->Get("ttbar_" + presel + "__"+vars[iV]);
      TH1 * s13 = (TH1*)f13->Get("signal_" + presel + "__"+vars[iV]);
      cout << "8TeV_mva_vars_" + presel + "__"+vars[iV]+"_data"<<endl;
      cout << d8 <<" "<<tw8<<" "<<nt8<<" "<<t13<<" "<<s13<<endl;
      p = new Plot();
      d8->Add(nt8, -1);
      
      p->add(d8,"8TeV data - non ttbar + W MC");
      p->add(tw8,"8TeV ttbar + W MC");
      p->add(t13,"13TeV ttbar MC");
      p->add(s13,"13TeV signal");
      if(vars[iV] == "met") p->rebin(2);
      p->toUnderOverflow();
      p->normalize();
      p->SetTitle(presel);
      o->Add(p);

      int rebin = 3;
      if(vars[iV].BeginsWith("num")) rebin = 1;
      r = p->makeRatiogram(1u,"",rebin);
      r->setMinMax(0.,2.);
      o->Add(r);
      Pint::drawAll(o,"");
      
      
   }
   
}


{
   TFile *f8 = new TFile("8TeV.root","read");
   TFile *f13 = new TFile("13TeV.root","read");
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
       "T2bw_lowX"               ,
       "T2bw_lowMass"           ,
       "T2bw_medXHighMass"       ,
       "T2bw_highXHighMass"  ,
       "T2bw_veryHighMass"         ,
       ""
   };
   
   TString presel = "T2bW_preselection";
   TString* vars = T2bWMVAVars;
   
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      TH1 * t13 = (TH1*)f13->Get("ttbar_" + presel + "__"+vars[iV]);
      TH1 * s13 = (TH1*)f13->Get("signal_" + presel + "__"+vars[iV]);
      // cout << d8 <<" "<<tw8<<" "<<nt8<<" "<<t13<<" "<<s13<<endl;
      p = new Plot();
      
      p->add(t13,"13TeV ttbar MC");
      p->add(s13,"13TeV signal");
      p->toUnderOverflow();
      p->normalize();
      p->rebin(10);
      p->SetTitle(presel);
      
      o->Add(p);
      
      
   }
   Pint::drawAll(o,"");
   
}


/// Q/G

#Classification

p = new Plot("gen__none__def_.*__nearDR"); p->toUnderOverflow(); p->draw("","/i");

p = new Plot("reco__none__def_.*__nearDR"); p->toUnderOverflow(); p->draw("","/i");

p = new Plot("reco__none__def_.*__betaStar_o_cut$"); p->toUnderOverflow(); p->normalize(); p->draw("",":i(.2,-1)");

{
   npu = new Plot("reco__npv_.*__nonPU__betaStar_o_cut$");
   pu = new Plot("reco__npv_.*__def_pu_jet__betaStar_o_cut$");
   
   npu->toUnderOverflow();
   pu->toUnderOverflow();
   
   TCanvas * c = npu->makeCutEfficiencies(*pu,"PU jets","non-PU jets");
   c->Draw();
}


p = new Plot("reco__none__def_.*__csv$"); p->toUnderOverflow(); p->normalize(); p->draw("","");

{
   npu = new Plot("reco__none__def_.*__csv$");
   
   npu->toUnderOverflow();
   
   TCanvas * c = npu->makeCutEfficiencies(0);
   c->Draw();
}


{
   // TString vars[] = {"ptD$","axis1$","axis2$","mult$","qgl$",""};
   TString vars[] = {"qgl$","newQgl$","newQglwPU$",""};
   
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      o = new TObjArray();
      

      
      pq = new Plot("reco__npv_.*__def_uds_quark__" + vars[iV] );
      pg = new Plot("reco__npv_.*__def_gluon__" + vars[iV] );
      pq->toUnderOverflow();
      pg->toUnderOverflow();
      pq->normalize();
      pg->normalize();            
      TCanvas * c = pq->makeCutEfficiencies(*pg,"gluon","uds quark");
      c->Draw();
      
      pq = pq->makeRatiogram(0u,"",2);
      pq->SetTitle("uds quark");
      o->Add(pq);
      pg = pg->makeRatiogram(0u,"",2);
      pg->SetTitle("gluon");
      o->Add(pg);
      
      p = new Plot("gen__none__def_(uds_quark|gluon|b_quark)__"+vars[iV]);
      p->SetTitle("gen");
      // if(p.getNumberOfHistograms() == 0){
       p = new Plot("reco__none__def_(uds_quark|gluon|b_quark)__"+vars[iV]);
       p->SetTitle("reco");
    // }
      p->toUnderOverflow();
      p->normalize();
      o->Add(p);
     


         Pint::drawAll(o,vars[iV]);
   }
   
}


p = new Plot("npv_.*__def_(uds_quark|gluon)__axis1"); p->toUnderOverflow(); p->draw();

