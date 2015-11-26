#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

//_____________________________________________________________________________
JetFlavorInfo::JetFlavor JetFlavorInfo::jetFlavor(int particleID, bool isPartonShower)
{
  if (particleID < 0)   return jetFlavor(-particleID);
  switch (particleID) {
    case 0:                           return unmatched_jet;
    case ParticleInfo::p_d:
    case ParticleInfo::p_u:
    case ParticleInfo::p_s:           return isPartonShower ? ps_uds_jet : uds_jet ;
    case ParticleInfo::p_c:           return isPartonShower ? ps_c_jet   : c_jet   ;
    case ParticleInfo::p_b:           return isPartonShower ? ps_b_jet   : b_jet   ;
    case ParticleInfo::p_g:           return isPartonShower ? ps_g_jet   : g_jet   ;
    default:                          return numJetFlavors;
  }
}
//_____________________________________________________________________________
const TString* JetFlavorInfo::jetFlavorNames()
{
  static const TString  NAMES[numJetFlavors+1]  = { "x","g","uds", "c", "b", "ps_g","ps_uds","ps_c","ps_b","other" };
  return NAMES;
}
//_____________________________________________________________________________
const TString& JetFlavorInfo::jetFlavorName(JetFlavor flavor)
{
  return jetFlavorNames()[flavor];
}
//_____________________________________________________________________________
const TString& JetFlavorInfo::jetFlavorTag(JetFlavor flavor)
{
  return jetFlavorTags()[flavor];
}
//_____________________________________________________________________________
const TString* JetFlavorInfo::jetFlavorTags()
{
  static const TString  NAMES[numJetFlavors+1]  = { "x", "g","q", "c", "b", "ps_g","ps_uds","ps_c","ps_b", "" };
  return NAMES;
}
//_____________________________________________________________________________
const char* JetFlavorInfo::specialJetFlavor(JetFlavor flavor, JetFlavor special)
{
  switch (flavor) {
    case uds_jet:       return "q";
    case c_jet:         return (flavor == special ? "c" : "q");
    case b_jet:         return (flavor == special ? "b" : "q");
    case g_jet:         return "g";
    default:            return "x";
  }
}
