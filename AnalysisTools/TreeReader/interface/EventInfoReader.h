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

      unsigned int  run;
      unsigned int  lumi;
      unsigned int  event;
      unsigned int  nPV;
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
      float         genmet_pt;
      float         genmet_phi;
      bool          goodvertex;
      float         genweight;
      float         genqscale;
      float         xsecweight;
      float         genevtweight;
      float         evtweight;
      size8             proc;
      defaults::Process process;
      size8             datrec;
      defaults::DataReco datareco;
      std::vector<unsigned long> * trigbitflags;
      std::vector<bool>          * trigbitpass;
      std::vector<unsigned int>  * trigbitprescale;

      std::vector<unsigned long> * metfilterbitflags;
      std::vector<bool>          * metfilterbitpass;
      std::vector<bool> * hbheHNFlt;

      unsigned long triggerflag;

      bool hbheFixFlt;
      bool hbheFlt;
      bool cscFlt;
      bool eeBadSCFlt;

      MomentumF    met;
      MomentumF    metNoHF;
      MomentumF    genmet;

  };

}



#endif
