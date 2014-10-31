// ------------------------------------------------------------
//  
//    QGLikelihoodCalculator - Class
//    for the computation of the QG likelihood.
//
// ------------------------------------------------------------

#ifndef QGLikelihoodCalculator_h
#define QGLikelihoodCalculator_h

#include <string>

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrector.h"




class QGLikelihoodCalculator {

 public:

  QGLikelihoodCalculator( const std::string& fileName_nCharged="AnalysisTools/JetShapeVariables/data/QGTaggerConfig_nCharged_AK5PFCHS.txt", const std::string& fileName_nNeutral="AnalysisTools/JetShapeVariables/data/QGTaggerConfig_nNeutral_AK5PFCHS.txt", const std::string& fileName_ptD="AnalysisTools/JetShapeVariables/data/QGTaggerConfig_ptD_AK5PFCHS.txt");
   ~QGLikelihoodCalculator();


  float computeQGLikelihood( float pt, float rhoPF, int nCharged, int nNeutral, float ptD );
  

 private:


  JetCorrectorParameters *jcp_nCharged_quark_;
  JetCorrectorParameters *jcp_nCharged_gluon_;
  JetCorrectorParameters *jcp_nNeutral_quark_;
  JetCorrectorParameters *jcp_nNeutral_gluon_;
  JetCorrectorParameters *jcp_ptD_quark_;
  JetCorrectorParameters *jcp_ptD_gluon_;

  SimpleJetCorrector *sjc_nCharged_quark_;
  SimpleJetCorrector *sjc_nCharged_gluon_;
  SimpleJetCorrector *sjc_nNeutral_quark_;
  SimpleJetCorrector *sjc_nNeutral_gluon_;
  SimpleJetCorrector *sjc_ptD_quark_;
  SimpleJetCorrector *sjc_ptD_gluon_;

};


#endif
