{
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  // Uh, apparently needed for loading vector dlls into CINT? Loading of std::vector branches crashes without this.
  gROOT->ProcessLine("#include<vector>");
  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libAnalysisMethodsPlotUtils.so");
  gSystem->Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libAnalysisBaseTreeAnalyzer.so");
}
