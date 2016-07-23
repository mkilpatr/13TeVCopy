//--------------------------------------------------------------------------------------------------
//
// EventInfoReader
//
// Class for reading event level information from TTree.
//
// EventInfoReader.h created on Mon Nov 3 12:56:14 CET 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_EVENTINFOREADER_H
#define ANALYSISTOOLS_TREEREADER_EVENTINFOREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

  class EventInfoReader : public BaseReader {

    public :
      EventInfoReader();
      ~EventInfoReader() {}

      void	load(TreeReader *treeReader, int options=0, std::string branchName="");
      void	refresh();
      void pushToTree();

      unsigned int  run;
      unsigned int  lumi;
      unsigned int  event;
      unsigned int  nPV;
      float         nPUTrue;
      float         rho;
      float         pvx;
      float         pvy;
      float         pvz;
      float         met_pt;
      float         met_phi;
      float         metsumEt;
      float         metNoHF_pt;
      float         metNoHF_phi;
      float         metNoHF_sumEt;
      float         puppimet_pt;
      float         puppimet_phi;
      float         puppimetsumEt;
      float         genmet_pt;
      float         genmet_phi;
      float         metunclusterup;
      float         metunclusterdn;
      bool          goodvertex;
      float         genweight;
      float         genqscale;
      float         xsec;
      float         xsecweight;
      float         genevtweight;
      float         evtweight;
      int           nStandardGenJets;
      float         lhecentralweight;
      std::vector<float> * systweights;
      size8             proc;
      defaults::Process    process;
      defaults::SignalType signalType;
      size8             datrec;
      defaults::DataReco datareco;

      std::vector<bool>          * metfilterbitpass_old;
      size                         metfilterbitpass;
      std::vector<size16>        * massparams;

      bool HBHENoiseFilter;
      bool HBHENoiseIsoFilter;
      bool globalTightHalo2016Filter;
      bool EcalDeadCellTriggerPrimitiveFilter;
      bool goodVertices;
      bool eeBadScFilter;
      bool badChCand;
      bool badPFMuon;


      int  massPar1;
      int  massPar2;
      int  massPar3;


      MomentumF    met;
      MomentumF    metNoHF;
      MomentumF    puppimet;
      MomentumF    genmet;

  };

}



#endif
