
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"

using namespace ucsbsusy;

TFile* FileUtilities::open(const char* path, const char option[], bool stopIfMissing){
  if (path == 0 || path[0] == 0)  return 0;

  if (!stopIfMissing) {
    Long_t    id, flags, modtime;
    Long64_t  fileSize;
    if (gSystem->GetPathInfo(path, &id, &fileSize, &flags, &modtime) || flags)
      return 0;
  }

  TFile*    file  = TFile::Open(path, option);
  if (stopIfMissing && file->IsZombie())
    throw cms::Exception("BaseUtilities.open()", TString::Format("Failed to open '%s' for %s.", path, (file->IsWritable() ? "output" : "input")).Data());
  return file;
}
