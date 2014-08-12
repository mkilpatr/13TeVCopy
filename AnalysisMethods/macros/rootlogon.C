{
  // make sure libraries you need are loaded
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libAnalysisToolsDataFormats.so");
}
