//--------------------------------------------------------------------------------------------------
// 
// LeptonMVA
// 
// Class to compute discriminator value for hadronic tau candidates.
// 
// LeptonMVA.cc created on Wed Jan 28 03:33:04 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/ObjectSelection/interface/LeptonMVA.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
LeptonMVA::LeptonMVA()
{
  // Constructor.  
}

//--------------------------------------------------------------------------------------------------
LeptonMVA::~LeptonMVA()
{
  delete reader;
}


void LeptonMVA::initialize(std::string weightsfile ){

  reader = new TMVA::Reader( "!Color:!Silent:Error" ); 
  reader->AddVariable("pt",    &fMVAVar_pt);
  reader->AddVariable("sef",    &fMVAVar_sef);
  reader->AddVariable("sip3d",    &fMVAVar_sip3d);
  reader->AddVariable("rhocorrectediso",    &fMVAVar_rhocorrectediso);
  reader->BookMVA("BDTG", weightsfile.c_str());

}

double LeptonMVA::evaluateMVA(float pt, float slf, float sip3d, float rhocorrectediso)
{
 
  fMVAVar_pt=pt;
  fMVAVar_sef=slf;
  if(slf<0) fMVAVar_sef=0;
  if(slf>2) fMVAVar_sef=2;
  fMVAVar_sip3d=sip3d;
  fMVAVar_rhocorrectediso=rhocorrectediso;

  return reader->EvaluateMVA("BDTG");

}
