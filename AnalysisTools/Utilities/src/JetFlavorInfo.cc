#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

//_____________________________________________________________________________
JetFlavorInfo::JetFlavor JetFlavorInfo::jetFlavor(int particleID)
{
  if (particleID < 0)   return jetFlavor(-particleID);
  switch (particleID) {
    case 0:                           return unmatched_jet;
    case ParticleInfo::p_d:
    case ParticleInfo::p_u:
    case ParticleInfo::p_s:           return uds_jet;
    case ParticleInfo::p_c:           return c_jet;
    case ParticleInfo::p_b:           return b_jet;
    case ParticleInfo::p_g:           return g_jet;
    default:                          return numJetFlavors;
  }
}
//_____________________________________________________________________________
const TString* JetFlavorInfo::jetFlavorNames()
{
  static const TString  NAMES[numJetFlavors+1]  = { "x","g","uds", "c", "b", "other" };
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
  static const TString  NAMES[numJetFlavors+1]  = { "x", "g","q", "c", "b", "" };
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
