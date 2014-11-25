root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_0_9_patch1/src/AnalysisMethods/macros/PickyJets/processPickyInvestigations.C+("ttbarPickyTree.root")'
   
   

./scripts/makeplots.py  -O -i jobs_presto_prediction_noCor  -p $CMSSW_BASE/src/SUSYReferenceAnalysis2/BackgroundEstimation/plugins/TopEWKTuningTreeMaker.cc -c $CMSSW_BASE/src/SUSYReferenceAnalysis2/BackgroundEstimation/test/topewktuningtreemaker_cfg.py -A 'targetDataSample=met pickyJESScale=-9' -f 2 -T '::pickyTrimKT1-met' '[tW].*'  'Znunu-HT[1-4]'
   
   
   targetDataSample=met pickyJESScale=-9


p = new Plot(".*__none__.*__inclusive__superJetPT"); p->toUnderOverflow(); p->draw("",":i");

{
   // TString type = "__inclusive__";
      TString type = "__split__";
   
   // TString vars[] = {"superJetPT","1_subjettiness","2_subjettiness","2o1_subjettiness",""};
   
   TString vars[] = {"jet_mass","sub12_dr","highest_peak","lowest_peak","minimum_value","lowest_peak_location","highest_peak_location","minimum_location", "shouldSplitDisc","shouldSplitDescision",""};
   
   
   TString pts[] = {"pt_lt50","pt_eq50to100","pt_eq100to200","pt_geq200",""};
   

   for(unsigned int iV = 0; vars[iV][0]; ++iV){
         o = new TObjArray;
         for(unsigned int iP = 0; pts[iP][0]; ++iP){
      p = new Plot(".*none__type_.*__pt_"+ pts[iP]+ type + vars[iV] + "$");
      p->SetTitle(Plot::translated(pts[iP]));
      p->toUnderOverflow();
      p->normalize();
      o->Add(p);
   }
   Pint::drawAll(o,"","","");
   
   }
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



{
   TString vars[] = {"2o1_subjettiness",""};
   TString pts[] = {"pt_pt_lt50","pt_pt_eq50to100","pt_pt_eq100to200","pt_pt_geq200",""};
   TString npvs[] = {"nPV_lt15","nPV_eq15to22","nPV_eq15to22",""};
   

   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      o = new TObjArray;
      for(unsigned int iP = 0; pts[iP][0]; ++iP){
         pg = new Plot("gen__none__type_.*__"+ pts[iP]+"__inclusive__" + vars[iV]);
         pg->sitrep();
         for(unsigned int iN = 0; npvs[iN][0]; ++iN){
            p = new Plot("reco__npv_"+ npvs[iN]+ "__type_.*__"+ pts[iP]+"__inclusive__" + vars[iV]);
            
            for(unsigned int iH = 0; iH < pg->getNumberOfHistograms();++iH){
               TString name("gen, ");
               name +=  pg->getAlias(iH);
               TH1 * h = (TH1*)pg->at(iH)->Clone();
               p->add(h,name);
            }
            
            
            
            cout << ".*__npv_"+ npvs[iN]+ "__type_.*__"+ pts[iP]+"__inclusive__" + vars[iV] <<endl;
            p->SetTitle(pts[iP]+ npvs[iN]);
            p->toUnderOverflow();
            p->normalize();
            o->Add(p);
         }
      }
      Pint::drawAll(o,"","","","4x3");
   }
}



{
   TString vars[] = {"superJetPT","1_subjettiness","2_subjettiness","2o1_subjettiness","jet_mass","sub12_dr","highest_peak","lowest_peak","minimum_value","lowest_peak_location","highest_peak_location","minimum_location", "shouldSplitDisc","shouldSplitDescision",""};
   TString pts[] = {"pt_pt_lt50","pt_pt_eq50to100","pt_pt_eq100to200","pt_pt_geq200",""};
   

   for(unsigned int iV = 0; vars[iV][0]; ++iV){
         o = new TObjArray;
         for(unsigned int iP = 0; pts[iP][0]; ++iP){
      p = new Plot(".*none__type_.*__"+ pts[iP]+"__split__" + vars[iV]+"$");
      p->SetTitle(pts[iP]);
      p->toUnderOverflow();
      p->normalize();
      o->Add(p);
   }
   Pint::drawAll(o,"","",":i");
   
   }
}


{
   TString vars[] = {"superJetPT","1_subjettiness","2_subjettiness","shouldSplitDisc","shouldSplitDescision",""};
   
   o = new TObjArray;
   for(unsigned int iV = 0; vars[iV][0]; ++iV){
      p = new Plot(".*__none__.*__none__split__" + vars[iV]);
      p->toUnderOverflow();
      // p->normalize();
      o->Add(p);
   }
   Pint::drawAll(o,"","",":i");
}