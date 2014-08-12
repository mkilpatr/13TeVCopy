//--------------------------------------------------------------------------------------------------
// 
// EventInfo
// 
// Class to hold some event specific information.
// 
// EventInfo.cc created on Mon Aug 11 11:22:00 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/DataFormats/interface/EventInfo.h"

ClassImp(ucsbsusy::EventInfo)

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
EventInfo::EventInfo() :
  fRunNum(0), fLumiSec(0), fEvtNum(0), fIsMC(kFALSE), fNPV(0), fPVx(0), fPVy(0), fPVz(0), fMET(0), fMETPhi(0), fMETSumET(0), fGenMET(0), fMETUp(0), fMETDown(0)
{
  // Default constructor.
}

//--------------------------------------------------------------------------------------------------
EventInfo::EventInfo(UInt_t run, UInt_t lumi, UInt_t evt) :
  fRunNum(run), fLumiSec(lumi), fEvtNum(evt), fIsMC(kFALSE), fNPV(0), fPVx(0), fPVy(0), fPVz(0), fMET(0), fMETPhi(0), fMETSumET(0), fGenMET(0), fMETUp(0), fMETDown(0)
{
  // Constructor.
}
