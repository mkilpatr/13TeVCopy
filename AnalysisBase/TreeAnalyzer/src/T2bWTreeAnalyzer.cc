#include "AnalysisTools/Parang/interface/Panvariate.h"
#include "AnalysisBase/TreeAnalyzer/interface/T2bWTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

using namespace ucsbsusy;

const double T2bWTreeAnalyzer::T2BWDiscCuts[] = {.94,.73,.92,.82,.93};

T2bWTreeAnalyzer::T2bWTreeAnalyzer(TString fileName, TString treeName, bool isMCTree)
: BaseTreeAnalyzer(fileName,treeName,isMCTree,0)
, minPT (30)
, maxETA(2.4)
, T2BWParams(NUM_T2BW_SEARCHREGIONS,0)
, T2BWParNames(NUM_T2BW_SEARCHREGIONS,"")
, loadedT2BW(false)
, met             (0)
, met_pt          (0)
, nJ70            (0)
, nJ30            (0)
, nTightBTags     (0)
, dPhiMET12       (10.7)
, dPhiMET3        (10.7)
, passPreselction (0)
, nMedBTags       (0)
, secLeadQL       (0)
, leadLeadQL      (0)
, prodQL          (1)
, htAlongAway     (8000)
, rmsJetPT        (0)
, rmsJetDphiMET   (0)
, bInvMass        (0)
, bTransverseMass (0)
, rmsBEta         (0)
, wInvMass        (0)
, T2BWDiscValues(NUM_T2BW_SEARCHREGIONS,-10)
{
  TString T2BWMVAInputFileName = "T2BWSearchRegionMVAs.root";

  if (T2BWMVAInputFileName != ""){
    clog << T2BWMVAInputFileName<<endl;

    clog << " ++  T2BWDiscCuts       = ";
    for(unsigned int iS = 0; iS < NUM_T2BW_SEARCHREGIONS; ++iS )clog << T2BWDiscCuts[iS]<<" ";
    clog << endl;

    const_cast<vector<TString>&>(T2BWParNames)[T2BW_LX]   = "MVAvs_vSet__rebin_chi_0p25_mStop_550_MLSP_175";
    const_cast<vector<TString>&>(T2BWParNames)[T2BW_LM]   = "MVAvs_vSet__rebin_chi_0p75_mStop_350_MLSP_75" ;
    const_cast<vector<TString>&>(T2BWParNames)[T2BW_MXHM] = "MVAvs_vSet__rebin_chi_0p50_mStop_550_MLSP_125";
    const_cast<vector<TString>&>(T2BWParNames)[T2BW_HXHM] = "MVAvs_vSet__rebin_chi_0p75_mStop_400_MLSP_25" ;
    const_cast<vector<TString>&>(T2BWParNames)[T2BW_VHM]  = "MVAvs_vSet__rebin_chi_0p75_mStop_550_MLSP_25" ;

    TFile*              T2BWMVAFile    = TFile::Open(T2BWMVAInputFileName, "READ");
    for(unsigned int iS = 0; iS < NUM_T2BW_SEARCHREGIONS; ++iS)
      const_cast<vector<const ParamatrixMVA*>& >(T2BWParams)[iS] =  dynamic_cast<ParamatrixMVA*>(T2BWMVAFile->Get(T2BWParNames[iS]));
    T2BWMVAFile->Close();              delete T2BWMVAFile;
    const_cast<bool&>(loadedT2BW) = true;
  }
};

//void T2bWTreeAnalyzer::load(VarType type, int options, string branchName)
//{
//  switch (type) {
//    case EVTINFO : {
//      reader.load(&evtInfoReader, 0, "");
//      break;
//    }
//    case AK4JETS : {
//      int defaultOptions = (JetReader::defaultOptions | JetReader::LOADJETSHAPE) | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
//      reader.load(&ak4Reader, options < 0 ? defaultOptions : options, branchName == "" ? "ak4pfchs" : branchName);
//      break;
//    }
//    default : {
//      cout << endl << "No settings for type: " << type << " found!"<<endl;
//      break;
//    }
//  }
//}
void T2bWTreeAnalyzer::processVariables(){
  filterJets(jets,minPT,maxETA);
  met = &evtInfoReader.met;
  fillSearchVars();
  if(loadedT2BW)
    computeT2BWDiscriminators();
}
void T2bWTreeAnalyzer::filterJets(vector<RecoJetF*>& newJets, const double minPT, const double maxETA){
  newJets.resize(0);
  newJets.reserve(ak4Reader.recoJets.size());
  for(auto& jet : ak4Reader.recoJets){
    if(JetKinematics::passCuts(jet,minPT,maxETA))
      newJets.push_back(&jet);
  }
}
void T2bWTreeAnalyzer::fillSearchVars(){

  nElectrons = 0;
  nMuons = 0;
  for(const auto& l : electronReader.electrons)  if(l.pt() >= 5 && TMath::Abs(l.eta()) < 2.4 && l.isgoodpogelectron()) nElectrons++;
  for(const auto& l : muonReader.muons)  if(l.pt() >= 5 && TMath::Abs(l.eta()) < 2.4 && l.isgoodpogmuon()) nMuons++;

  met_pt = met->pt();

  nJ70 = PhysicsUtilities::countObjectsDeref(jets,70,999);
  nJ30 = jets.size();
  nTightBTags = PhysicsUtilities::countObjectsDeref(jets,minPT,maxETA,&T2bWTreeAnalyzer::isTightBTaggedJet);
  dPhiMET12 = JetKinematics::absDPhiMETJ12(*met,jets);
  dPhiMET3 = JetKinematics::absDPhiMETJ3(*met,jets);
  passPreselction = met_pt >= 175 && nJ70 >= 2 && nJ30 >= 5 && nTightBTags >= 1  && dPhiMET12 >= .5 && dPhiMET3 >= .3;

  nMedBTags = PhysicsUtilities::countObjectsDeref(jets,minPT,maxETA,&T2bWTreeAnalyzer::isMediumBTaggedJet);

  leadLeadQL = 0;
  secLeadQL  = 0;
  prodQL     = 1;

  for(const auto * jet : jets){
    double qgl = ak4Reader.jetqgl_->at(jet->index());
    prodQL *= qgl;
    if(qgl > leadLeadQL){
      secLeadQL = leadLeadQL;
      leadLeadQL = qgl;
    } else if(qgl > secLeadQL)
      secLeadQL = qgl;
  }
  htAlongAway = JetKinematics::htAlongHtAway(*met,jets);
  rmsJetPT = JetKinematics::ptRMS(jets);
  rmsJetDphiMET = JetKinematics::deltaPhiMETRMS(*met,jets);
  bInvMass = JetKinematics::bJetInvMass(jets,&T2bWTreeAnalyzer::isMediumBTaggedJet);
  bTransverseMass = JetKinematics::bJetTranverseMass(*met,jets,&T2bWTreeAnalyzer::isMediumBTaggedJet);
  rmsBEta = JetKinematics::deltaEtaBJetRMS(jets,&T2bWTreeAnalyzer::isMediumBTaggedJet);
  wInvMass = JetKinematics::highestPTJetPair(jets);
}


void T2bWTreeAnalyzer::computeT2BWDiscriminators(){
assert(loadedT2BW);
for(unsigned int iD = 0; iD < NUM_T2BW_SEARCHREGIONS; ++iD){
  const Panvariate*           mvaReader     = T2BWParams[iD]->getOne();
  assert(mvaReader);

  mvaReader->setVariable(  mvaReader ->findVariable("met"                     ),met_pt                        );
  mvaReader->setVariable(  mvaReader ->findVariable("nj30"                    ),nJ30                       );
  mvaReader->setVariable(  mvaReader ->findVariable("absDphi_met_J3"          ),dPhiMET3                    );
  mvaReader->setVariable(  mvaReader ->findVariable("ht_along_over_away"      ),htAlongAway                );
  mvaReader->setVariable(  mvaReader ->findVariable("ptRMS"                   ),rmsJetPT                       );
  mvaReader->setVariable(  mvaReader ->findVariable("rms_jet_phi_awayMET"     ),rmsJetDphiMET         );
  mvaReader->setVariable(  mvaReader ->findVariable("rms_jet_eta_b"           ),rmsBEta               );
  mvaReader->setVariable(  mvaReader ->findVariable("mT_nearest_MedB_to_met"  ),bTransverseMass               );
  mvaReader->setVariable(  mvaReader ->findVariable("bInvMass"                ),bInvMass                    );
  mvaReader->setVariable(  mvaReader ->findVariable("numMedBtags"             ),nMedBTags                 );
  mvaReader->setVariable(  mvaReader ->findVariable("maxQuarkLikelihood"      ),leadLeadQL      );
  mvaReader->setVariable(  mvaReader ->findVariable("secondMaxQuarkLikelihood"),secLeadQL);
  mvaReader->setVariable(  mvaReader ->findVariable("eventQuarkLikelihood"    ),prodQL        );
  mvaReader->setVariable(  mvaReader ->findVariable("getLeadingWPairMass"     ),wInvMass                );

  T2BWDiscValues[iD] = mvaReader->evaluateMethod(0);


//    if(iD == 0){
//      cout << endl << "T2BW!";
//      for(unsigned int iV = 0; iV < mvaReader->getNumVariables(); ++iV)
//        cout << endl << iV <<  " " << mvaReader->getVariable(iV).GetName() << "  ->  " << mvaReader->getVariable(iV).getValue();
//    }
}

}
