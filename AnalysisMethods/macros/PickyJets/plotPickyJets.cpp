{
   TString eta[] = {"eta_lt2p4__","","eta_eq2p4to4p6__",""};
   TString den[] = {"all","good_partons","","assoc_jets",""};
   TString jets[] = {"ak4","picky","puppi","trimmed",""};
   for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   o = new TObjArray;
   for(unsigned int iE = 0; eta[iE][0]; ++iE){
      p = new Plot(jets[iJ] + "_" + eta[iE] +".*__top_pt");
      p->SetTitle(Plot::translated(eta[iE]));
      p->toUnderOverflow();
    p->rebin(4);
      o->Add(p);
         for(unsigned int iD = 0; den[iD][0]; ++iD){
            r = p->makeRatiogram(den[iD]);
            r->setMinMax(0.,1.);
            o->Add(r);
         }
   }
   Pint::drawAll(o,jets[iJ]);
}
}

{
   TString vars[] = {"lead_top_pt","subLead_top_pt","avg_top_pt",""};
   TString eta[] = {"eta_lt2p4__",""};
   TString den[] = {"all","good_partons",""};
   TString jets[] = {"ak4","picky","puppi","trimmed",""};
   for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV)
   for(unsigned int iE = 0; eta[iE][0] || !iE; ++iE){
      p = new Plot(jets[iJ] + "_" + eta[iE] +".*__" + vars[iV]);
    // cout << jets[iJ] + "_" + eta[iE] +".*__" + vars[iV] << endl;
    // p->sitrep();
      p->SetTitle(Plot::translated(eta[iE]));
      p->toUnderOverflow();
    p->rebin(4);
      o->Add(p);
         for(unsigned int iD = 0; den[iD][0]; ++iD){
            r = p->makeRatiogram(den[iD]);
            r->setMinMax(0.,1.);
            o->Add(r);
         }
   }
   Pint::drawAll(o,jets[iJ]);
}
}
//Grandcomp
{
   TString vars[] = {"avg_top_pt","","top_pt","","lead_top_pt","subLead_top_pt","avg_top_pt",""};
   TString den[] = {"good_partons",""};
   // TString jets[] = {"ak4","picky","trimmed",""};
   TString jets[] = {"picky",""};
   o = new TObjArray;
    oo = new TObjArray;
   for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
     TString eta = "eta_lt2p4__";
      p = new Plot(jets[iJ] + "_" + eta +"(all|good_partons|assoc_jets|no_splits|good_top)__" + vars[iV] + "$");
    // p->sitrep();
    // cout << jets[iJ] + "_" + eta +".*__" + vars[iV] <<endl;
      p->toUnderOverflow();
    p->rebin(4);
    p->sort("i");
    p->SetTitle(jets[iJ]);
      o->Add(p);
         for(unsigned int iD = 0; den[iD][0]; ++iD){
            r = p->makeRatiogram(den[iD]);
            r->drop("all");
            r->setMinMax(0.,1.);
      r->SetTitle(jets[iJ]);
            oo->Add(r);
         }
   }
}
Pint::drawAll(o,"");
Pint::drawAll(oo,"");
}

//Grandcomp 2
{
   TString vars[] = {"top_pt","subLead_top_pt",""};
   TString den[] = {"good_partons",""};
   TString jets[] = {"ak4","picky","trimmed",""};
  // TString samps[] = {"ttbar","t2tt_lowMass","t2tt_highMass","",""};
    TString samps[] = {"ttbar","t2tt_highMass","t2tt_highMass_p5FR","t2tt_highMass_p15FR",""};
   Plot::cache("*_evttree_plots.root");
  for(unsigned int iV = 0; vars[iV][0]; ++iV){
   o = new TObjArray;
    oo = new TObjArray;
   for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
        for(unsigned int iS = 0; samps[iS][0]; ++iS){
     TString eta = "eta_lt2p4__";
      p = new Plot(samps[iS] + "_" + jets[iJ] + "_" + eta +".*__" + vars[iV]);
    // p->sitrep();
    // cout << jets[iJ] + "_" + eta +".*__" + vars[iV] <<endl;
      p->toUnderOverflow();
    p->rebin(4);
    p->SetTitle(samps[iS] +" :: " +jets[iJ]);
      o->Add(p);
         for(unsigned int iD = 0; den[iD][0]; ++iD){
            r = p->makeRatiogram(den[iD]);
            r->setMinMax(0.,1.);
      r->SetTitle(samps[iS] +" :: " +jets[iJ]);
            oo->Add(r);
         }
   }
}
Pint::drawAll(o,vars[iV]);
Pint::drawAll(oo,vars[iV]);
}

//compare finals
{
   TString vars[] = {"avg_top_pt","","top_pt","lead_top_pt","subLead_top_pt","avg_top_pt",""};
   TString den[] = {"good_partons",""};
   // TString jets[] = {"ak4","picky","trimmed",""};
   TString jets[] = {"trimmed",""};
   o = new TObjArray;
    oo = new TObjArray;
   // for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
     TString eta = "eta_lt2p4__";
     pP = new Plot("(trimmed_|picky_|ak4_)" + eta +"good_partons__" + vars[iV] + "$");
     pT = new Plot("(trimmed_|picky_|ak4_)" + eta +"good_top__" + vars[iV] + "$");
     
     pP->toUnderOverflow();
     pP->rebin(4);
     
     pT->toUnderOverflow();
     pT->rebin(4);
     
     oo->Add(pP);
     pP = (Plot*)pP->Clone();
     
     pT = pT->divide(*pP,1,false,"b");
     pT->setMinMax(0.,1.);
     o->Add(pT);
         // p->sitrep();
    // cout << jets[iJ] + "_" + eta +".*__" + vars[iV] <<endl;
      // p->toUnderOverflow();
    // p->rebin(4);
    // p->sort("i");
    // p->SetTitle(jets[iJ]);
      // o->Add(p);
         // for(unsigned int iD = 0; den[iD][0]; ++iD){
            // r = p->makeRatiogram(den[iD]);
            // r->drop("all");
            // r->setMinMax(0.,1.);
      // r->SetTitle(jets[iJ]);
            // oo->Add(r);
         // }
   }
// }
Pint::drawAll(o,"");
Pint::drawAll(oo,"");
}



///Test parton causes for failure
{
   TString vars[] = {"top_pt","lead_top_pt","subLead_top_pt","avg_top_pt",""};
   TString pts[] = {"all","top_pt_lt250"     ,"top_pt_eq250to550","top_pt_geq550"    ,""};
   TString types[] = {"noJet","splitJet","dirtyJet","resolvedParton" ,""};
   TString jets[] = {"ak4","picky","trimmed",""};
   o = new TObjArray;
    oo = new TObjArray;
   for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   for(unsigned int iP = 0; pts[iP][0]; ++iP){

     TString eta = "eta_lt2p4__";
      p = new Plot(jets[iJ] + "_" + eta +"all__"+ pts[iP]+ "__.*__parton_pt" );
    // p->sitrep();
    // cout << jets[iJ] + "_" + eta +"all__"+ pts[iP]+ "__.*__parton_pt" <<endl;
      p->toUnderOverflow();
    // p->rebin(4);
    p->SetTitle(jets[iJ]+ pts[iP]);
      o->Add(p);
      p = (Plot*)p->Clone();
      p->normalize();
      oo->Add(p);
      //    for(unsigned int iD = 0; den[iD][0]; ++iD){
      //       r = p->makeRatiogram(den[iD]);
      //       r->setMinMax(0.,1.);
      // r->SetTitle(jets[iJ]);
      //       oo->Add(r);
      //    }
   }
}
Pint::drawAll(o,"");
// Pint::drawAll(oo,"");
}





{
   TestAnalyzer->cd();
   double ptLow[] ={20, 40, 60, 80, 100, 120, 160, 200, 250, 300, 400, 550,-1};
   double ptHigh[] ={40, 60, 80, 100, 120, 160, 200, 250, 300, 400, 550, 1000};
   for(int iG = 0; iG < 2; ++iG){
      o = new TObjArray;
      oo = new TObjArray;
      TString genString = " && isGen";
      if(iG) genString = " && !isGen";

         for(unsigned int iP = 0; ptLow[iP] >= 0; ++iP){
            Events->Draw("superJet_eta>>goodSplit(50,-5,5)",TString::Format("splitResult == 0 && (superJet_pt >= %.0f && superJet_pt < %.0f)  %s",ptLow[iP],ptHigh[iP],genString.Data()));
            Events->Draw("superJet_eta>>badSplit(50,-5,5)",TString::Format("splitResult > 0 && (superJet_pt >= %.0f && superJet_pt < %.0f)  %s",ptLow[iP],ptHigh[iP],genString.Data()));
            TH1F * goodSplit;
            TH1F * badSplit;
            gDirectory->GetObject("goodSplit",goodSplit);
            gDirectory->GetObject("badSplit",badSplit);
            p = new Plot();
            p->add(goodSplit,"goodSplit");
            p->add(badSplit,"badSplit");
            p->SetTitle(TString::Format("%.0f - %.0f",ptLow[iP],ptHigh[iP]));
            p = (Plot*)p->Clone();
            o->Add(p);
         }

         Pint::drawAll(o,genString,"",":i","");
   }
}

{
   // TestAnalyzer->cd();
   double ptLow[] ={20, 40, 60, 80, 100, 120, 160, 200, 250, 300, 400, 550,-1};
   double ptHigh[] ={40, 60, 80, 100, 120, 160, 200, 250, 300, 400, 550, 10000};
   double etaLow[]  = {0,1.9,2.9,-1};
   double etaHigh[] = {1.9,2.9,6};
   
   for(unsigned int iP = 0; ptLow[iP] >= 0; ++iP){
      cout << endl << TString::Format("%.0f - %.0f, < %.1f \t",ptLow[iP],ptHigh[iP]);
      for(unsigned int iE = 0; etaLow[iE] >= 0; ++iE){
         int good = Events->GetEntries(TString::Format("splitResult == 2 && !isGen && (superJet_pt >= %.0f && superJet_pt < %.0f) && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f)",ptLow[iP],ptHigh[iP],etaLow[iE],etaHigh[iE]));
         int bad = Events->GetEntries(TString::Format("splitResult == 1 && !isGen && (superJet_pt >= %.0f && superJet_pt < %.0f) && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f)",ptLow[iP],ptHigh[iP],etaLow[iE],etaHigh[iE]));
         cout << good <<"\t"<< bad <<"\t";
      }
   }
   cout << endl;
}


{
   TestAnalyzer->cd();

   double ptLow[] = {20,50,100,200,-1};
   double ptHigh[] = {50,100,200,1000};

   double etaLow[]  = {0,2.4,-1};
   double etaHigh[] = {2.4,5};
   
   for(int iG = 0; iG < 2; ++iG){
   o = new TObjArray;
   oo = new TObjArray;
   TString genString = " && isGen";
   if(iG) genString = " && !isGen";
   for(unsigned int iE = 0; etaLow[iE] >= 0; ++iE){
      Events->Draw("superJet_pt>>goodSplit_all(50,0,500)",TString::Format("splitResult == 0  && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      Events->Draw("superJet_pt>>goodSplit_pass(50,0,500)",TString::Format("splitResult == 0 && oldDisc_shouldSplit > 0 && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      // Events->Draw("superJet_pt>>badSplit_all(50,0,500)",TString::Format("splitResult > 0  && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      // Events->Draw("superJet_pt>>badSplit_pass(50,0,500)",TString::Format("splitResult > 0 && oldDisc_shouldSplit > 0 && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));

      Events->Draw("superJet_pt>>badSplit_alll(50,0,500)",TString::Format("splitResult ==1   && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      Events->Draw("superJet_pt>>badSplit_passl(50,0,500)",TString::Format("splitResult ==1 && oldDisc_shouldSplit > 0 && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      
      Events->Draw("superJet_pt>>badSplit_allh(50,0,500)",TString::Format("splitResult ==2  && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));
      Events->Draw("superJet_pt>>badSplit_passh(50,0,500)",TString::Format("splitResult ==2 && oldDisc_shouldSplit > 0 && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",etaLow[iE],etaHigh[iE],genString.Data()));

      TH1F * goodSplit_all;
      TH1F * goodSplit_pass;
      gDirectory->GetObject("goodSplit_all",goodSplit_all);
      gDirectory->GetObject("goodSplit_pass",goodSplit_pass);
      TH1F * badSplit_alll;
      TH1F * badSplit_passl;
      gDirectory->GetObject("badSplit_alll",badSplit_alll);
      gDirectory->GetObject("badSplit_passl",badSplit_passl);
      TH1F * badSplit_allh;
      TH1F * badSplit_passh;
      gDirectory->GetObject("badSplit_allh",badSplit_allh);
      gDirectory->GetObject("badSplit_passh",badSplit_passh);
      pa = new Plot();
      pa->add(goodSplit_all,"goodSplit");
      pa->add(badSplit_alll,"badSplit1");
      pa->add(badSplit_allh,"badSplit2");
      pp = new Plot();
      pp->add(goodSplit_pass,"goodSplit");
      pp->add(badSplit_passl,"badSplit1");
      pp->add(badSplit_passh,"badSplit2");
      pa = (Plot*)pa->Clone();
      pp = (Plot*)pp->Clone();
      pa->toUnderOverflow();
      pp->toUnderOverflow();
      pa->SetTitle(TString::Format("all : < %.1f",etaLow[iE]));
      pp->SetTitle(TString::Format("pass : < %.1f",etaLow[iE]));

      pr = (Plot*)pp->Clone();
      pr->divide(*pa,1,false,"b");
      pr->SetTitle(TString::Format("ratio : < %.1f",etaLow[iE]));
      
      oo->Add(pa);
      oo->Add(pp);
      oo->Add(pr);
      
      
   for(unsigned int iP = 0; ptLow[iP] >= 0; ++iP){
         Events->Draw("oldDisc>>goodSplit(50,-1,1)",TString::Format("splitResult == 0 && (superJet_pt >= %.0f && superJet_pt < %.0f) && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",ptLow[iP],ptHigh[iP],etaLow[iE],etaHigh[iE],genString.Data()));
         Events->Draw("oldDisc>>badSplit(50,-1,1)",TString::Format("splitResult > 0 && (superJet_pt >= %.0f && superJet_pt < %.0f) && (abs(superJet_eta) >= %.1f && abs(superJet_eta) < %.1f) %s",ptLow[iP],ptHigh[iP],etaLow[iE],etaHigh[iE],genString.Data()));
         TH1F * goodSplit;
         TH1F * badSplit;
         gDirectory->GetObject("goodSplit",goodSplit);
         gDirectory->GetObject("badSplit",badSplit);
         p = new Plot();
         p->add(goodSplit,"goodSplit");
         p->add(badSplit,"badSplit");
         p->SetTitle(TString::Format("%.0f - %.0f, < %.1f",ptLow[iP],ptHigh[iP],etaLow[iE]));
         p = (Plot*)p->Clone();
         o->Add(p);

   }
}
   Pint::drawAll(o,genString,"","","2x4");
   Pint::drawAll(oo,genString,"",":i","2x3");
   
}
}


///Test MVAs
{
   TString pts[] = {"pt_eq20to50",  "pt_eq50to100", "pt_eq100to200","pt_eq200to400","pt_geq400",""}  ;
   TString etas[] = {"eta_lt1p9","","eta_eq1p9to2p9","eta_geq2p9",""};
   TString vars[] = {"oldDisc","newDisc",""};
   for(unsigned int iV = 0;vars[iV][0]; ++iV){
   o = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         TString title = Plot::translated(etas[iE] +" " +pts[iP]);
         // pb =new Plot("gen__PURE_TO_.*__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         pb =new Plot("reco__.*_TO.*__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         // pb =new Plot("gen__(PURE_TO|MIXED_TO)_.*__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         // pb =new Plot("reco__(goodSplit|badSplit)__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         // pb =new Plot("reco__(MIXED_TO_CLEANER|MIXED_TO_BETTER)__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         pb->toUnderOverflow();
         // pb->normalize();
         pb->rebin(4);

         pb->SetTitle(title);

         o->Add(pb);
}
}
   Pint::drawAll(o,vars[iV],"",":i");
}

}

//test types
{
   TString pts[] = {"pt_eq20to50",  "pt_eq50to100", "pt_eq100to200","pt_eq200to400","pt_geq400",""}  ;
   TString etas[] = {"eta_lt1p9","","eta_eq1p9to2p9","eta_geq2p9",""};
   TString vars[] = {"jet_mass","1_subjettiness","2_subjettiness","highest_peak","lowest_peak","minimum_value","lowest_peak_location","highest_peak_location","minimum_location","sub12_dr",""};
   for(unsigned int iV = 0;vars[iV][0]; ++iV){
   o = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         TString title = Plot::translated(etas[iE] +" " +pts[iP]);
         pb =new Plot("gen__(dirtySplit|splitParton|goodSplit)__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         pb->toUnderOverflow();
         pb->normalize();

         pb->SetTitle(title);

         o->Add(pb);
}
}
   Pint::drawAll(o,"","");
}
   
}


//check input variables
{
   // TString pts[] = {"pt_eq20to50",  "pt_eq50to100", "pt_eq100to200","pt_eq200to400","pt_geq400",""}  ;
   TString pts[] = {"pt_eq100to200",""}  ;
   TString etas[] = {"eta_lt2p0","","eta_eq2p0to2p9","eta_geq3p0",""};
   TString vars[] = {"jet_mass","1_subjettiness","2_subjettiness","highest_peak","lowest_peak","minimum_value","lowest_peak_location","highest_peak_location","minimum_location","sub12_dr",""};
   for(unsigned int iV = 0;vars[iV][0]; ++iV){
   o = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         TString title = Plot::translated(etas[iE] +" " +pts[iP]);
         pb =new Plot("(reco__|gen__)(goodSplit|badSplit)__ee_"+etas[iE]+"__ep_"+pts[iP]+"__" + vars[iV] + "$");
         pb->toUnderOverflow();
         pb->normalize();

         pb->SetTitle(title);

         o->Add(pb);
}
}
   Pint::drawAll(o,"","");
}

}
//compare discriminators
{
   // TString pts[] = {"pt_eq20to50",  "pt_eq50to100", "pt_eq100to200","pt_eq200to400","pt_geq400",""}  ;
TString pts[] = {   "emp_20_40","emp_40_60","emp_60_80","emp_80_100","emp_100_120","emp_120_160","emp_160_200","emp_200_250","emp_250_300","emp_300_400","emp_400_550","emp_550_1000",""};
TString etas[] = {"eta_lt2p0","eta_eq2p0to3p0","eta_geq3p0",""};
   
   o = new TObjArray;
   oo = new TObjArray;
   ooo = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         TString title = Plot::translated(etas[iE] +" " +pts[iP]);
         pb =new Plot("_badSplit__ee_"+etas[iE]+"__"+pts[iP]+"__.*Disc");
         pg =new Plot("_goodSplit__ee_"+etas[iE]+"__"+pts[iP]+"__.*Disc");
         pb->toUnderOverflow();
         pg->toUnderOverflow();
         pb->normalize();
         pg->normalize();
         pb->SetTitle(title);
         pg->SetTitle(title);
         if(pb->getNumberOfHistograms() < 4 || pg->getNumberOfHistograms() < 4) continue;

         TCanvas * c = pb->makeCutEfficiencies(*pg,"goodSplits","badSplits");
         ooo->Add(c);

         pb->rebin(40);
         pg->rebin(40);
         o->Add(pg);
         oo->Add(pb);



   }
}
   Pint::drawAll(o,"goodSplits","",":i(-.21,-1)");
   Pint::drawAll(oo,"badSplits","",":i(-.21,-1)");
   Pint::drawAll(ooo,"ROC","");

   
}
// compute points
{
   TString type[] = {"gen","reco",""};
   TString pts[] = {"emp_20_40","emp_40_60","emp_60_80","emp_80_100","emp_100_120","emp_120_160","emp_160_200","emp_200_250","emp_250_300","emp_300_400","emp_400_550","emp_550_1000",""};
   TString etas[] = {"eta_lt2p0","eta_eq2p0to3p0","eta_geq3p0",""};
   for(unsigned int iT = 0; type[iT][0]; ++iT){
      cout <<"-------- "<< type[iT] <<" --------" << endl;
   o = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      cout <<"-------- "<< etas[iE] << endl;
      vector<double> cuts         (12,-1);
      vector<double> signaleff    (12,-1);
      vector<double> backgroundeff(12,-1);
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         pb = new Plot(type[iT] + "__badSplit__ee_"+ etas[iE]+"__"+pts[iP]+"__newDisc" );
         pg = new Plot(type[iT] + "__goodSplit__ee_"+ etas[iE]+"__"+pts[iP]+"__newDisc" );
         if(pb->getNumberOfHistograms() != 1) continue;
         if(pg->getNumberOfHistograms() != 1) continue;

         pb->toUnderOverflow();
         pg->toUnderOverflow();
         double totalNum = pb->at(0)->Integral() + pg->at(0)->Integral();
         pb->normalize();
         pg->normalize();
         pb = pb->makeIntegral(true);
         pg = pg->makeIntegral(true);
         pb->SetTitle(Plot::translated(etas[iE] +" " +pts[iP]));


         o->Add(pb);

         TH1 * h = pb->at(0);
         for(unsigned int iB = h->GetNbinsX(); iB >= 1; --iB){
            if(h->GetBinContent(iB) < .1 ) continue;
            cuts[iP] = h->GetBinLowEdge(iB) +h->GetBinWidth(iB);
            signaleff[iP] = pg->at(0)->GetBinContent(iB+1);
            backgroundeff[iP] = h->GetBinContent(iB+1);
            break;
         }
      }
      for(unsigned int iP = 0;pts[iP][0]; ++iP ){
         cout << TString::Format("%.2f,",cuts[iP]);
      }
      cout << endl;
      for(unsigned int iP = 0;pts[iP][0]; ++iP ){
         cout << TString::Format("%.3f,",signaleff[iP]);
      }
      cout << endl;
      for(unsigned int iP = 0;pts[iP][0]; ++iP ){
         cout << TString::Format("%.3f,",backgroundeff[iP]);
      }
      cout << endl;
   }
   // Pint::drawAll(o,type[iT],"");
   
 }
}
////Prety comp
{
   // TString pts[] = {"pt_eq20to50",  "pt_eq50to100", "pt_eq100to200","pt_eq200to400","pt_geq400",""}  ;
// TString pts[] = {   "emp_20_40","emp_40_60","emp_60_80","emp_80_100","emp_100_120","emp_120_160","emp_160_200","emp_200_250","emp_250_300","emp_300_400","emp_400_550","emp_550_1000",""};
// TString etas[] = {"eta_lt2p0","eta_eq2p0to3p0","eta_geq3p0",""};
    
    TString pts[] = {   "20_40","100_120","550_1000",""};
    TString etas[] = {"eta_lt2p0","eta_geq3p0",""};
   
   o = new TObjArray;
   oo = new TObjArray;
   ooo = new TObjArray;
   for(unsigned int iE = 0;etas[iE][0]; ++iE){
      for(unsigned int iP = 0;pts[iP][0]; ++iP){
         TString title = Plot::translated(etas[iE] +", p_{T} = " +pts[iP]);
         pb =new Plot("_badSplit__ee_"+etas[iE]+"__emp_"+pts[iP]+"__newDisc");
         pg =new Plot("_goodSplit__ee_"+etas[iE]+"__emp_"+pts[iP]+"__newDisc");
         pb->toUnderOverflow();
         pg->toUnderOverflow();
         pb->normalize();
         pg->normalize();
         pb->SetTitle(title);
         pg->SetTitle(title);
         if(pb->getNumberOfHistograms() < 2 || pg->getNumberOfHistograms() < 2) continue;

         TCanvas * c = pb->makeCutEfficiencies(*pg,"goodSplits","badSplits");
         ooo->Add(c);

         pb->rebin(40);
         pg->rebin(40);
         
         pc =new Plot("(goodSplit|badSplit)__ee_"+etas[iE]+"__emp_"+pts[iP]+"__newDisc");
         pc->SetTitle(title);
         o->Add(pc);
         // oo->Add(pb);



   }
}
   Pint::drawAll(o,"goodSplits","","");
   // Pint::drawAll(oo,"badSplits","",":i(-.21,-1)");
   Pint::drawAll(ooo,"ROC","");

   
}

./makejobs.py  -n 2 -p ../../../ObjectProducers/JetProducers/test/ -c runPickyJetTreeProducer.py -o  /uscms_data/d3/nmccoll/2011-04-15-susyra2/Work7/jobs_noPuppi -t condor



>>>>>>Test Gen Match


p = new Plot("DE_.*__partonDR"); new TCanvas(); p->draw("",":ii(.3,-1)i(1,-1)"); new TCanvas(); p = p->makeRatiogram("inc"); p->draw();
p = new Plot("DE_.*__partonHadronDR"); new TCanvas(); p->draw("",":ii(.3,-1)"); new TCanvas(); p = p->makeRatiogram("inc"); p->draw();

p = new Plot("DPT_.*__partonDR"); new TCanvas(); p->draw("",":ii(.3,-1)i(1,-1)"); new TCanvas(); p = p->makeRatiogram("inc"); p->draw();
p = new Plot("DPT_.*__partonHadronDR"); new TCanvas(); p->draw("",":ii(.3,-1)"); new TCanvas(); p = p->makeRatiogram("inc"); p->draw();

p = new Plot("__partonPTFrac"); new TCanvas(); p->draw(); new TCanvas(); p = p->makeRatiogram("all_match"); p->draw();






{

   p = new Plot("DE_.*__partonDR"); new TCanvas(); p->draw("",":ii(.3,-1)i(1,-1)");
   p->toUnderOverflow();

   new TCanvas();
   p1 = (Plot*)p->Clone();
   p1->scale(1.0/p1->at("inc")->Integral());
   p1 = p1->makeIntegral(true,true);
   p1->draw();

   new TCanvas();
   p2 = (Plot*)p->Clone();
   p2 = p2->makeIntegral(true,true);
   p2 = p2->makeRatiogram("inc");
   p2->draw();



   // p = p->makeRatiogram("inc"); p->draw();

}

{
   TString pts[] = {
      // "pt_incl__"      ,
      // "pt_lt20__"      ,
      "pt_eq20to50__"  ,
      "pt_eq50to100__" ,
      "pt_eq100to200__",
      "pt_geq200__"    ,
      ""
   };

   TString vars[] = {
     "quark_ptRes",
     "w_ptRes",
     "w_massRes",
     "top_ptRes",
     "top_massRes",
     ""
   };

   // TString set[] = {
   //    "iDR_0p00_",
   //    "iDR_0p20_",
   //    "iDR_0p30_",
   //    "iDR_0p40_",
   //   ""
   // };
   //
   // TString set[] = {
   //    "iDR_0p00_oDR_0p60_",
   //    "iDR_0p00_oDR_0p80_",
   //    "iDR_0p00_oDR_1p00_",
   //   ""
   // };

   TString set[] = {
      "iDR_0p00_oDR_1p20_",
     ""
   };


   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      o = new TObjArray;
      cout << vars[iV]<<endl;

      for(unsigned int iS = 0; set[iS][0]; ++iS){
      for(unsigned int iP = 0; pts[iP][0]; ++iP){
      cout << pts[iP]<<endl;

         p = new Plot(set[iS] +".*" + pts[iP] + vars[iV]);
         p->toUnderOverflow();
         p->SetTitle(Plot::translated(pts[iP]));
         p->normalize();
         o->Add(p);
         for(unsigned int iH = 0; iH < p->getNumberOfHistograms(); ++iH){
            TString name = set[iS]+p->getAlias(iH);
            cout <<             TString::Format("%s\t%.2f\t%.2f\t%.2f\t%.2f\n",name.Data(),p->at(iH)->Integral(0,15),p->at(iH)->Integral(55,-1),p->at(iH)->GetMean(),p->at(iH)->GetRMS());

                  }
      }
   }
      Pint::drawAll(o,vars[iV],"","","");
   }

}


>>>>>>>>>>>>>>>>  Validation plots
    
//Start with inclusivw plots
{
    // TString pts[] = {"minTopPT_lt100__","minTopPT_eq100to200__","minTopPT_geq200__",""};
    TString vars[] = {"num_cen_j20","num_j20","ht",""};
    
    TString pts[] = {"incl__",""};
        // TString vars[] = {"j1_pt","j2_pt","j3_pt","j1_ptRes","j2_ptRes","j3_ptRes","j1_csv","j2_csv","j3_csv",""};
    
    o = new TObjArray;
    oo = new TObjArray;
    for(unsigned int iP = 0; pts[iP][0]; ++iP)
            for(unsigned int iV = 0; vars[iV][0]; ++iV){
                p = new Plot("(ak4|ak4Gen|puppi)_"+pts[iP] + vars[iV] + "$");
                p->SetTitle(pts[iP] + vars[iV]);
                p->toUnderOverflow();
                o->Add(p);
                p = p->makeRatiogram(0u,"");
                oo->Add(p);
            }
            
           Pint::drawAll(o,"");
                      Pint::drawAll(oo,"");
}



{
    // TString pts[] = {"minTopPT_lt100__","minTopPT_eq100to200__","minTopPT_geq200__",""};
    // TString vars[] = {"j1_ptRes","j2_ptRes","j3_ptRes","j4_ptRes","j5_ptRes","jgeq6_ptRes",""};
    TString vars[] = {"ptRes","num_cen_j20","num_j20","eta",""};
    
    TString pts[] = {"incl__",""};
        // TString vars[] = {"j1_pt","j2_pt","j3_pt","j1_ptRes","j2_ptRes","j3_ptRes","j1_csv","j2_csv","j3_csv",""};
    
    o = new TObjArray;
    oo = new TObjArray;
    for(unsigned int iP = 0; pts[iP][0]; ++iP)
            for(unsigned int iV = 0; vars[iV][0]; ++iV){
                p = new Plot("(ak4Raw|puppi)_"+pts[iP] + vars[iV] + "$");
                p->SetTitle("_"+pts[iP] + vars[iV]);
                p->toUnderOverflow();
                p->normalize();
                o->Add(p);
                // p = p->makeRatiogram(0u,"");
                // oo->Add(p);
            }
            
           Pint::drawAll(o,"","",":m");
                      // Pint::drawAll(oo,"");
}


{
   TString vars[] = {"num_cen_j20","ht","j1_pt","j1_ptoht","j2_ptopt1","",""};
   // TString jets[] = {"ak4","picky","trimmed",""};
   o = new TObjArray;
    oo = new TObjArray;
   // for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
     p = new Plot("(trimmed_|picky_|ak4_)incl__" + vars[iV] + "$");
     p->toUnderOverflow();
     o->Add(p);
   }
// }
Pint::drawAll(o,"");

}


{
   TString vars[] = {"nDirtyPartons",
"nNoJetPartons",
"nResolovedPartons",
"nSplitPartons",""};
   // TString jets[] = {"ak4","picky","trimmed",""};
   o = new TObjArray;
    oo = new TObjArray;
   // for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
     p = new Plot("(trimmed_|picky_|ak4_)eta_lt2p4__" + vars[iV] + "$");
     p2 = new Plot("(trimmed_|picky_|ak4_)eta_lt2p4__nGoodPartons$");
     p2 = (Plot*)p2->Clone();
     // p->add(p2->at(0),"goodPartons");
     p->toUnderOverflow();
     p->SetTitle(vars[iV]);
     o->Add(p);
     p = p->makeRatiogram("ak4");
     oo->Add(p);     
     
   }
// }
Pint::drawAll(o,"");
Pint::drawAll(oo,"");

}



>>>>>>>>>>> Picky jet PU correections
    
{
    TString pu[] = {"nV_lt15__","nV_eq15to22__","nV_geq22__",""};
    TString vars[] = {"num_20","num_30","num_50","num_100",""};
    // TString vars[] = {"pt",""};
    o = new TObjArray();
    for(unsigned int iP = 0; pu[iP][0]; ++iP){
        for(unsigned int iV = 0; vars[iV][0]; ++iV){
            p = new Plot("incl__"+pu[iP]+"reco_"+vars[iV]);
            pg = new Plot("corr_0p00__incl__"+pu[iP]+"gen_"+vars[iV]);
            if(pg->getNumberOfHistograms() == 0)continue;
            p->add(pg->at(0),"gen");
            p->SetTitle(Plot::translated(pu[iP]+vars[iV]));
            p->toUnderOverflow();
            p = p->makeRatiogram("gen","");
            p->setMinMax(0.,2.);
            o->Add(p);

        
    }
}

Pint::drawAll(o,"");
}


{
    TString pu[] = {"nV_lt15__","nV_eq15to22__","nV_geq22__",""};
    TString pt[] = {"pt_eq20to50__","pt_eq50to100__","pt_eq100to300__","pt_geq300__",""};
    // TString vars[] = {"pt",""};
    o = new TObjArray();
    for(unsigned int iP = 0; pu[iP][0]; ++iP){
        for(unsigned int iPT = 0; pt[iPT][0]; ++iPT){
            p = new Plot("incl__"+pu[iP]+pt[iPT] +"reco_eta");
            pg = new Plot("corr_0p00__incl__"+pu[iP]+pt[iPT] +"gen_eta");
            if(pg->getNumberOfHistograms() == 0)continue;
            p->add(pg->at(0),"gen");
            p->SetTitle(Plot::translated(pu[iP]+pt[iPT]));
            p->toUnderOverflow();
            p = p->makeRatiogram("gen","",4);
            p->setMinMax(0.,2.);
            o->Add(p);

        
    }
}

Pint::drawAll(o,"");
}



{
    TString pu[] = {"nV_lt15__","nV_eq15to22__","nV_geq22__",""};
        TString pt[] = {"pt_eq20to50__","pt_eq50to100__","pt_eq100to300__","pt_geq300__",""};
    o = new TObjArray();
    for(unsigned int iP = 0; pu[iP][0]; ++iP){
        for(unsigned int iPT = 0; pt[iPT][0]; ++iPT){
            p = new Plot("incl__"+pu[iP]+pt[iPT] + "ptRes");
            p->toUnderOverflow();

            p->SetTitle(Plot::translated(pu[iP]+pt[iPT]));
            // p = p->makeRatiogram("gen","");
            // p->setMinMax(0.,2.);
            o->Add(p);

        
    }
}

Pint::drawAll(o,"","",":mr");
}


>>> PIKCY JET VALIDATION
    

{
    Plot::cache("*_plots.root");
    TString vars[]= {"num_20","num_bjets","topness",""};
    // TString sels[]= {"0Lep__nBJets_inc__met_geq100__","0Lep__nBJets_inc__met_geq150__","0Lep__nBJets_inc__met_geq200__",""};
    // TString sels[]= {"1Lep_dPhi__nBJets_inc__met_geq100__","1Lep_dPhi__nBJets_inc__met_geq150__","1Lep_dPhi__nBJets_inc__met_geq200__",""};
        TString sels[]= {"0Lep__nBJets_inc__met_geq100__",""};
    TString jets[] ={"picky_","ak4_",""};
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        o = new TObjArray;
        oo = new TObjArray;
        for(unsigned int iS = 0; sels[iS][0]; ++iS){
            for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
                p = new Plot(jets[iJ] + sels[iS] + vars[iV]);
                p->SetTitle(Plot::translated(jets[iJ] + sels[iS]));
                p->toUnderOverflow();
                p->normalize();
                o->Add(p);
                
                // TCanvas * c = p->makeCutEfficiencies(3);
                // oo->Add(c);
                
            }
        }    
        Pint::drawAll(o,"");
        // Pint::drawAll(oo,"");
    }
}


{
    double bins[] = {-20,1,7,20};
    RebinToNewBins b(3,bins);
    Plot::cache("*_plots.root");
    TString vars[]= {"num_20","num_bjets","topness",""};
    // TString sels[]= {"0Lep__nBJets_inc__met_geq100__","0Lep__nBJets_inc__met_geq150__","0Lep__nBJets_inc__met_geq200__",""};
    // TString sels[]= {"1Lep_dPhi__nBJets_geq1__met_geq100__","1Lep_dPhi__nBJets_geq1__met_geq150__","1Lep_dPhi__nBJets_geq1__met_geq200__",""};
        TString sels[]= {"0Lep__nBJets_inc__met_geq100__","0Lep__nBJets_geq1__met_geq200__",""};
    TString jets[] ={"picky_","ak4_",""};
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        o = new TObjArray;
        oo = new TObjArray;
        for(unsigned int iS = 0; sels[iS][0]; ++iS){
            
            pa= new Plot("ak4_"+sels[iS] + vars[iV]);
            pp = new Plot("picky_"+sels[iS] + vars[iV]);
            pa->toUnderOverflow();
            pp->toUnderOverflow();
            pa->normalize();
            pp->normalize();
            
            if(iV == 2){
                pp->rebin(b);
                pa->rebin(b);
            }
            
            pp->divide(*pa);
            pp->SetTitle(Plot::translated(sels[iS]));
            o->Add(pp);            
        }    
        Pint::drawAll(o,"");
        // Pint::drawAll(oo,"");
    }
}


{
    Plot::cache("*_plots.root");
    TString vars[]= {"num_20",""};
    TString presel = "0Lep__nBJets_inc__";
    // TString presel = "1Lep_dPhi__nBJets_inc__";
    TString bkg = "ttbar";
    // TString sigs[] = {"t2tt_lowDM","t2tt_largeDM","t1tttt_largeDM",""};
        TString sigs[] = {"t1tttt_largeDM",""};
    TString sels[]= {"met_geq100__","met_geq200__",""};        
    TString jets[] ={"picky_","ak4_",""};
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        o = new TObjArray;
        oo = new TObjArray;
        
        for(unsigned int isig = 0; sigs[isig][0]; ++isig){
            pb = new Plot();
            ps = new Plot();
            for(unsigned int iS = 0; sels[iS][0]; ++iS){
                for(unsigned int iJ = 0; jets[iJ][0]; ++iJ){
                    TH1* b = Plot::getCache()->findOne(bkg + "_" +jets[iJ]+presel + sels[iS] + vars[iV] );
                    TH1* s = Plot::getCache()->findOne(sigs[isig] + "_" +jets[iJ]+presel + sels[iS] + vars[iV] );
                    if(b == 0) continue;
                    if(s == 0) continue;
                    pb->add(b,Plot::translated(jets[iJ] + sels[iS]));
                    ps->add(s,Plot::translated(jets[iJ] + sels[iS]));                    
                }
            }
            if(ps->getNumberOfHistograms() == 0 ||pb->getNumberOfHistograms() == 0) continue;
            ps->toUnderOverflow();
            ps->normalize();
            pb->toUnderOverflow();
            pb->normalize();
            ps->SetTitle(Plot::translated(presel + sigs[isig]));
            TCanvas *c1 = ps->makeCutEfficiencies(*pb,"ttbar",sigs[isig]);
            o->Add(c1);
            
        }
        Pint::drawAll(o,"");
    }
}
