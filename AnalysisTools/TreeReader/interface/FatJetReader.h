//--------------------------------------------------------------------------------------------------
//
// JetReader
//
// Class for reading electron object information from TTree.
//
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_FATJETREADER_H
#define ANALYSISTOOLS_TREEREADER_FATJETREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/ObjectSelection/interface/SoftdropTopMVA.h"
#include "AnalysisTools/ObjectSelection/interface/SoftdropWTagMVA.h"

namespace ucsbsusy {

  class FatJetReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , LOADPUPPI   = (1 << 1)
                    , FILLOBJ     = (1 << 2)   ///< Fill objects (as opposed to just pointers
                    , LOADSHAPE   = (1 << 3)
                    , LOADCTAG    = (1 << 4)
                    , UPDATETOPMVA  = (1 << 5)
                    , UPDATEWTAGMVA = (1 << 6)
    };
    static const int defaultOptions;

    FatJetReader();
    ~FatJetReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    void pushToTree(); //push changes made to the momentum back to the tree
  public:
    // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
    std::vector<float>* fjrawmass_;
    std::vector<float>* fjprunedmass_;
    std::vector<float>* fjsoftdropmass_;
    std::vector<float>* fjtau1_;
    std::vector<float>* fjtau2_;
    std::vector<float>* fjtau3_;
    std::vector<float>* fjpt_;
    std::vector<float>* fjeta_;
    std::vector<float>* fjphi_;
    std::vector<float>* fjcsv_;
    std::vector<int  >* fjnsdsubjets_;
    std::vector<float>* fjsdsj1mass_;
    std::vector<float>* fjsdsj1pt_;
    std::vector<float>* fjsdsj1eta_;
    std::vector<float>* fjsdsj1phi_;
    std::vector<float>* fjsdsj2mass_;
    std::vector<float>* fjsdsj1csv_;
    std::vector<float>* fjsdsj1cmva_;
    std::vector<float>* fjsdsj1cvsl_;
    std::vector<float>* fjsdsj1cvsb_;
    std::vector<float>* fjsdsj1ptD_;
    std::vector<float>* fjsdsj1axis1_;
    std::vector<float>* fjsdsj1axis2_;
    std::vector<int  >* fjsdsj1mult_;
    std::vector<float>* fjsdsj2pt_;
    std::vector<float>* fjsdsj2eta_;
    std::vector<float>* fjsdsj2phi_;
    std::vector<float>* fjsdsj2csv_;
    std::vector<float>* fjsdsj2cmva_;
    std::vector<float>* fjsdsj2cvsl_;
    std::vector<float>* fjsdsj2cvsb_;
    std::vector<float>* fjsdsj2ptD_;
    std::vector<float>* fjsdsj2axis1_;
    std::vector<float>* fjsdsj2axis2_;
    std::vector<int  >* fjsdsj2mult_;
    std::vector<float>* fjtopmva_;
    std::vector<float>* fjwmva_;

    //Puppi variables
    std::vector<float>* fj_puppi_pt_          ;
    std::vector<float>* fj_puppi_eta_         ;
    std::vector<float>* fj_puppi_phi_         ;
    std::vector<float>* fj_puppi_mass_        ;
    std::vector<float>* fj_puppi_tau1_        ;
    std::vector<float>* fj_puppi_tau2_        ;
    std::vector<float>* fj_puppi_tau3_        ;
    std::vector<float>* fj_puppi_softdropmass_;
    std::vector<int  >* fj_puppi_nsdsubjets_  ;
    std::vector<float>* fj_puppi_sdsj1mass_   ;
    std::vector<float>* fj_puppi_sdsj1pt_     ;
    std::vector<float>* fj_puppi_sdsj1eta_    ;
    std::vector<float>* fj_puppi_sdsj1phi_    ;
    std::vector<float>* fj_puppi_sdsj2mass_   ;
    std::vector<float>* fj_puppi_sdsj1csv_    ;
    std::vector<float>* fj_puppi_sdsj1cmva_   ;
    std::vector<float>* fj_puppi_sdsj1cvsl_   ;
    std::vector<float>* fj_puppi_sdsj1cvsb_   ;
    std::vector<float>* fj_puppi_sdsj2pt_     ;
    std::vector<float>* fj_puppi_sdsj2eta_    ;
    std::vector<float>* fj_puppi_sdsj2phi_    ;
    std::vector<float>* fj_puppi_sdsj2csv_    ;
    std::vector<float>* fj_puppi_sdsj2cmva_   ;
    std::vector<float>* fj_puppi_sdsj2cvsl_   ;
    std::vector<float>* fj_puppi_sdsj2cvsb_   ;






    //the actual jet collection
    FatJetFCollection fatJets;

    private:
      SoftdropTopMVA * sdTopMVA = nullptr;
      SoftdropWTagMVA * sdWTagMVA = nullptr;
};

}

#endif
