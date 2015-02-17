#ifndef JETFLAVORINFO_H
#define JETFLAVORINFO_H

#include <TString.h>

namespace JetFlavorInfo
{
// ps_*_jet means that the jet was matched with post showering partons
// otherwise matching was done by either matrix element partons (g/uds) or hadronization(c/b)
enum JetFlavor      { unmatched_jet, g_jet, uds_jet, c_jet, b_jet, ps_g_jet, ps_uds_jet, ps_c_jet, ps_b_jet, numJetFlavors };

//convert from pdgID to jet flavor...for comparing
JetFlavor      jetFlavor(int particleID, bool isPartonShower = false);

//Naming info
const TString* jetFlavorNames();
const TString& jetFlavorName(JetFlavor flavor);
const TString& jetFlavorTag(JetFlavor flavor);
const TString* jetFlavorTags();
const char*    specialJetFlavor(JetFlavor flavor, JetFlavor special);




};  // end class ParticleInfo

#endif //PARTICLEINFO_H
