{
   TString REG[] = {"sel_CR__","sel_CR_DPHI__",""};
   TString VARS[] = {"top1_pt","top2_pt","num_j30","num_j70",""};
   o  = new TObjArray;
      oo  = new TObjArray;
         ooo  = new TObjArray;
   for(unsigned int  iV = 0; VARS[iV][0]; ++iV){
      TString sel = "ttbarincl__";
      if(iV != 0) sel = "ttbarsel_.*__";
      pi = new Plot("sel_CR__"+sel+VARS[iV]);
      pd = new Plot("sel_CR_DPHI__"+sel+VARS[iV]);
      
      pi->toUnderOverflow();
      pd->toUnderOverflow();
      
      pi->SetTitle("Inclusive #Delta#Phi");
      pd->SetTitle("#Delta#Phi < 1");
      o->Add(pi);
      oo->Add(pd);
      pd = (Plot*)pd->Clone();    
      pi = (Plot*)pi->Clone();      
      
      p = pd->divide(*pi);
      p->SetTitle("#Delta#Phi < 1 / Inclusive");
      ooo->Add(p);
      
   }
   Pint::drawAll(o,"");
   Pint::drawAll(oo,"");
   Pint::drawAll(ooo,"","hist");
   
}


{
   TString SR[] = {      
      "T2bw_lowX"         ,
      "T2bw_lowMass"      ,
      "T2bw_medXHighMass" ,
      "T2bw_highXHighMass",
      "T2bw_veryHighMass" ,
      "T2tt_lowMass"      ,
      "T2tt_medMass"      ,
      "T2tt_highMass"     ,
      "T2tt_veryHighMass" ,
      ""      
   }
   
   
   TString REG[] = {"sel_incl__","sel_DPHI__",""};
   o  = new TObjArray;
   oo  = new TObjArray;
   ooo  = new TObjArray;
   for(unsigned int  iV = 0; SR[iV][0]; ++iV){
      pi = new Plot("proc_.*__sel_incl__"+SR[iV]+"$");
      pd = new Plot("proc_.*__sel_DPHI__"+SR[iV]+"$");
      pi->toUnderOverflow();
      pd->toUnderOverflow();
      pi->SetTitle("Inclusive #Delta#Phi");
      pd->SetTitle("#Delta#Phi < 1");
      
      pi = pi->makeRatiogram("incl","b");
      pd = pd->makeRatiogram("incl","b");
      
      
      o->Add(pi);
      oo->Add(pd);
      pd = (Plot*)pd->Clone();    
      pi = (Plot*)pi->Clone();      
      
      p = pd->divide(*pi);
      p->SetTitle("#Delta#Phi < 1 / Inclusive");      p->setMinMax(0.5,1.5);
      ooo->Add(p);
      
   }
   Pint::drawAll(o,"");
   Pint::drawAll(oo,"");
   Pint::drawAll(ooo,"","hist");
   
}



{
   double bins[] = {180,200,260,320,400,500};
   int nBins = 5;
   RebinToNewBins b(nBins,bins);
   
   TString SR[] = {      
      "T2bw_lowX"         ,
      "T2bw_lowMass"      ,
      "T2bw_medXHighMass" ,
      "T2bw_highXHighMass",
      "T2bw_veryHighMass" ,
      "T2tt_lowMass"      ,
      "T2tt_medMass"      ,
      "T2tt_highMass"     ,
      "T2tt_veryHighMass" ,
      ""      
   }
   
   
   TString REG[] = {"ltM0p5_","eqM0p5to0_","eq0to0p5_","geq0p5_","SR_"};
   o  = new TObjArray;
   oo  = new TObjArray;
   ooo  = new TObjArray;
   for(unsigned int  iV = 0; SR[iV][0]; ++iV){      
   
      
      pi = new Plot("proc_incl__sel_incl__"+SR[iV]+"_.*metNoLep$");
      pd = new Plot("proc_incl__sel_DPHI__"+SR[iV]+"_.*metNoLep$");
      
      pi->rebin(b);
      pd->rebin(b);
      
      pi->toUnderOverflow();
      pd->toUnderOverflow();
      
      
      
      pi->SetTitle("Inclusive #Delta#Phi");
      pd->SetTitle("#Delta#Phi < 1");
      
      // pi = pi->makeRatiogram("incl","b");
      // pd = pd->makeRatiogram("incl","b");
      
      
      o->Add(pi);
      oo->Add(pd);
      pd = (Plot*)pd->Clone();    
      pi = (Plot*)pi->Clone();      
      
      p = pd->divide(*pi);
      p->SetTitle("#Delta#Phi < 1 / Inclusive");      p->setMinMax(0.8,1.2);
      ooo->Add(p);
      
   }
   Pint::drawAll(o,"");
   Pint::drawAll(oo,"");
   Pint::drawAll(ooo,"","hist");
   
}