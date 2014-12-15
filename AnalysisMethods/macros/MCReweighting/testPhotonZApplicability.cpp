{
   TString CUTS[] = {"ht","nBs","nGenJets","nRecoJets",""};
   o = new TObjArray;
   oo = new TObjArray;
   ooo = new TObjArray;
   for(unsigned int iC = 0; CUTS[iC][0]; ++ iC){
      pz = new Plot("z_" + CUTS[iC] + "_.*__boson_pt");
      // pg = new Plot("g_timesSF_" + CUTS[iC] + "_.*__boson_pt");
      pg = new Plot("g_" + CUTS[iC] + "_.*__boson_pt");
      
      pz->rebin(5);
      pg->rebin(5);
      
      pz->SetTitle("Z : " + CUTS[iC]);
      pg->SetTitle("G : " + CUTS[iC]);
      pz->toUnderOverflow();
      pg->toUnderOverflow();
      o->Add(pz);
      oo->Add(pg);

      pz = (Plot*)pz->Clone();
      pz->divide(*pg);
      pz->SetTitle("Z/G "+CUTS[iC] );
      pz->setMinMax(0.,.5);
      ooo->Add(pz);

      
   }
   Pint::drawAll(o,"Z","",":i"    );
   Pint::drawAll(oo,"G","",":i"   );
   Pint::drawAll(ooo,"Z/G","","");
}


{
   double bins[] = {100,120,140,160,180,200,250,300,350,400,500,600,1000};
   RebinToNewBins b(12,bins);
   
   TString CUTS[] = {
      "nGenJets_eq2to3__" ,
      "nGenJets_eq4to5__" ,
      "nGenJets_eq6to7__" ,
      "nGenJets_geq8__"   ,
      ""
      };
   o = new TObjArray;
   oo = new TObjArray;
   ooo = new TObjArray;
   for(unsigned int iC = 0; CUTS[iC][0]; ++ iC){
      pz = new Plot("z_nCross_" + CUTS[iC] + ".*__boson_pt");
      pg = new Plot("g_nCross_" + CUTS[iC] + ".*__boson_pt");
      
      pz->rebin(b);
      pg->rebin(b);
      
      pz->SetTitle("Z : " + CUTS[iC]);
      pg->SetTitle("G : " + CUTS[iC]);
      pz->toUnderOverflow();
      pg->toUnderOverflow();
      o->Add(pz);
      oo->Add(pg);

      pz = (Plot*)pz->Clone();
      pz->divide(*pg);
      pz->SetTitle(Plot::translated("Z/G "+CUTS[iC]) );

      pz->setMinMax(0.,.5);
      ooo->Add(pz);
      
      
   }
   // Pint::drawAll(o,"Z","",":i"    );
   // Pint::drawAll(oo,"G","",":i"   );
   Pint::drawAll(ooo,"Z/G","","");
}




{
   TString CUTS[] = {"nBs_eq0__","nBs_eq1__","nBs_geq2__","",""};
   o = new TObjArray;
   oo = new TObjArray;
   ooo = new TObjArray;
   for(unsigned int iC = 0; CUTS[iC][0]; ++ iC){
      pz = new Plot("z_nCross_.*" + CUTS[iC] + "boson_pt");
      pg = new Plot("g_timesSF_nCross_.*" + CUTS[iC] + "boson_pt");
      
      pz->rebin(90);
      pg->rebin(90);
      
      pz->SetTitle("Z : " + CUTS[iC]);
      pg->SetTitle("G : " + CUTS[iC]);
      pz->toUnderOverflow();
      pg->toUnderOverflow();
      o->Add(pz);
      oo->Add(pg);

      pz = (Plot*)pz->Clone();
      pz->divide(*pg);
      pz->SetTitle("Z/G "+CUTS[iC] );
      // pz->setMinMax(0.,.5);
      ooo->Add(pz);

      
   }
   // Pint::drawAll(o,"Z","",":i"    );
   // Pint::drawAll(oo,"G","",":i"   );
   Pint::drawAll(ooo,"Z/G","",":i");
}





{

   // TString VARS[] = {
   //     "met"               ,
   //     "num_j30"           ,
   //     "dphi_j3_met"       ,
   //     "num_medium_btags"  ,
   //     // "q2_likeli"         ,
   //     // "q1_likeli"         ,
   //     // "quark_likeli"      ,
   //     "ht_along_over_away",
   //     // "rms_pt"            ,
   //     // "rms_dphi"          ,
   //     // "bb_mass"           ,
   //     "mTb"               ,
   //     // "deta_b_rms"        ,
   //     // "leading_jj_mass"   ,
   //     ""
   // };

   TString VARS[] = {
       "T2bw_lowX"               ,
       "T2bw_lowMass"           ,
       "T2bw_medXHighMass"       ,
       "T2bw_highXHighMass"  ,
       "T2bw_veryHighMass"         ,
       ""
   };


   o = new TObjArray;
   oo = new TObjArray;

   for(unsigned int iV = 0; VARS[iV][0]; ++iV){
      p = new Plot("(z|g_timesSF)_T2bW_preselection__"+VARS[iV]);
      p->toUnderOverflow();
      // if(iV == 0)      p->rebin(3);
   p->rebin(5);
      // p->at("g")->Scale(p->at("z")->Integral()/p->at("g")->Integral());
      o->Add(p);      
      
      p=p->makeRatiogram("z","");
      oo->Add(p);
      
   }
   
   Pint::drawAll(o,"" );
   Pint::drawAll(oo,"" );
}