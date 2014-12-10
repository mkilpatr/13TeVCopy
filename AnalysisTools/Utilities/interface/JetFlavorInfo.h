#ifndef JETFLAVORINFO_H
#define JETFLAVORINFO_H

#include <TString.h>

namespace JetFlavorInfo
{
enum JetFlavor      { unmatched_jet, g_jet, uds_jet, c_jet, b_jet, numJetFlavors };

//convert from pdgID to jet flavor...for comparing
JetFlavor      jetFlavor(int particleID);

//Naming info
const TString* jetFlavorNames();
const TString& jetFlavorName(JetFlavor flavor);
const TString& jetFlavorTag(JetFlavor flavor);
const TString* jetFlavorTags();
const char*    specialJetFlavor(JetFlavor flavor, JetFlavor special);




};  // end class ParticleInfo

#endif //PARTICLEINFO_H
