//--------------------------------------------------------------------------------------------------
//
// JetReader
//
// Class for reading electron object information from TTree.
//
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_HTTREADER_H
#define ANALYSISTOOLS_TREEREADER_HTTREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/HTTFatJet.h"
#include "AnalysisTools/ObjectSelection/interface/SoftdropMVA.h"
#include "AnalysisTools/ObjectSelection/interface/HTTMVA.h"

namespace ucsbsusy {

  class HTTReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , FILLOBJ     = (1 << 1)   ///< Fill objects (as opposed to just pointers
                    , LOADMVA     = (1 << 2)
    };
    static const int defaultOptions;

    HTTReader();
    ~HTTReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

  public:


    std::vector<float>* pt        ;
    std::vector<float>* eta       ;
    std::vector<float>* phi       ;
    std::vector<float>* mass      ;
    std::vector<float>* csv       ;
    std::vector<float>* ropt      ;
    std::vector<float>* frec      ;
    std::vector<float>* roptcalc  ;
    std::vector<float>* ptforropt ;
    std::vector<float>* ropt_pt   ;
    std::vector<float>* ropt_eta  ;
    std::vector<float>* ropt_phi  ;
    std::vector<float>* ropt_mass ;
    std::vector<float>* ropt_tau1 ;
    std::vector<float>* ropt_tau2 ;
    std::vector<float>* ropt_tau3 ;
    std::vector<float>* sd_mass   ;
    std::vector<float>* sd_tau1   ;
    std::vector<float>* sd_tau2   ;
    std::vector<float>* sd_tau3   ;
    std::vector<int  >* nsubjets  ;
    std::vector<float>* w1_pt     ;
    std::vector<float>* w1_eta    ;
    std::vector<float>* w1_phi    ;
    std::vector<float>* w1_mass   ;
    std::vector<float>* w1_csv    ;
    std::vector<float>* w1_ptD    ;
    std::vector<float>* w1_axis1  ;
    std::vector<float>* w1_axis2  ;
    std::vector<int  >* w1_mult   ;
    std::vector<float>* w2_pt     ;
    std::vector<float>* w2_eta    ;
    std::vector<float>* w2_phi    ;
    std::vector<float>* w2_mass   ;
    std::vector<float>* w2_csv    ;
    std::vector<float>* w2_ptD    ;
    std::vector<float>* w2_axis1  ;
    std::vector<float>* w2_axis2  ;
    std::vector<int  >* w2_mult   ;
    std::vector<float>* b_pt      ;
    std::vector<float>* b_eta     ;
    std::vector<float>* b_phi     ;
    std::vector<float>* b_mass    ;
    std::vector<float>* b_csv     ;
    std::vector<float>* b_ptD     ;
    std::vector<float>* b_axis1   ;
    std::vector<float>* b_axis2   ;
    std::vector<int  >* b_mult    ;


    //the actual jet collection
    HTTFatJetFCollection fatJets;

  private:
    HTTMVA * httMVA = nullptr;

};

}

#endif
