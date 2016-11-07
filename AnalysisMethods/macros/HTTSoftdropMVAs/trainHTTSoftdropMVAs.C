// derived from the TMVA tutorial tutorials/tmva/TMVAClassification.C by hqu
// modified by patterson
//
// see CHANGEME locations
//
// 'new_tmva' uses the new dataloader if you're using newer tmva version

#include <iostream>
#include <map>
#include <string>

//#define NEW_TMVA

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#ifdef NEW_TMVA
#include "TMVA/DataLoader.h"
#endif

void trainHTTv2MVA()
{
   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;

   // --- Boosted Decision Trees
   Use["BDTG"]            = 1; // uses Gradient Boost //CHANGEME default is BDTG. could try neural ("MLPBNN"), BDT adaptive ("BDT"), SVM ("SVM")
   Use["BDTG3"]           = 0; // uses Gradient Boost
   //
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Read training and test data
   TString treedir = "/data/apatters/trees/29102016-all-traintrees/"; //CHANGEME trees. see TMVA manual for all sorts of combinations of opening up sig / bkg trees / files.
   TFile *f_sig = TFile::Open( treedir+"T2tt_850_100_tree.root" );
   TFile *f_bkg = TFile::Open( treedir+"SM_tree.root" );

//   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

   // --- Register the training and test trees

   TTree *signal     = (TTree*)f_sig->Get("Events");
   TTree *background = (TTree*)f_bkg->Get("Events");

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
       "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification" );
//                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" ); //CHANGEME try initially transforming the variables

   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

   std::vector<TString> varsF, varsI;

   TString varset = "hqu"; //CHANGEME variable set for training: hqu, nick, alex

// HQU MVA
   if(varset == "hqu"){
   varsF = {
     "ak8_sdmass",
     "ak8_ptDR",
     "ak8_tau21",
     "ak8_tau32",
     "ak8_rel_ptdiff",

//     "ak8_tau1",
//     "ak8_tau2",
//     "ak8_tau3",
//     "ak8_pt",
//     "ak8_deltaR",
     // pt ranked
//     "ak8_sj1_pt",
//     "ak8_sj1_mass",
//     "ak8_sj1_csv",
//     "ak8_sj1_ptD",
//     "ak8_sj1_axis1",
//     "ak8_sj1_axis2",
//
//     "ak8_sj2_pt",
//     "ak8_sj2_mass",
//     "ak8_sj2_csv",
//     "ak8_sj2_ptD",
//     "ak8_sj2_axis1",
//     "ak8_sj2_axis2",

     // csv ranked
//     "ak8_csv1_pt",
     "ak8_csv1_mass",
     "ak8_csv1_csv",
     "ak8_csv1_ptD",
     "ak8_csv1_axis1",
//     "ak8_csv1_axis2",

//     "ak8_csv2_pt",
     "ak8_csv2_mass",
//     "ak8_csv2_csv",
     "ak8_csv2_ptD",
     "ak8_csv2_axis1",
//     "ak8_csv2_axis2",

//     // mass ranked
//     "ak8_mass1_pt",
//     "ak8_mass1_mass",
//     "ak8_mass1_csv",
//     "ak8_mass1_ptD",
//     "ak8_mass1_axis1",
//     "ak8_mass1_axis2",
//
//     "ak8_mass2_pt",
//     "ak8_mass2_mass",
//     "ak8_mass2_csv",
//     "ak8_mass2_ptD",
//     "ak8_mass2_axis1",
//     "ak8_mass2_axis2",

//     "ak8_sd_0",
//     "ak8_sd_n0p5",
//     "ak8_sd_n1",
//     "ak8_sd_n1p5",
//     "ak8_sd_n2",
   };

   varsI = {
//     "ak8_sj1_mult",
//     "ak8_sj2_mult",
//
     "ak8_csv1_mult",
     "ak8_csv2_mult",
//
//     "ak8_mass1_mult",
//     "ak8_mass2_mult",
   };
   }//hqu
   

if(varset == "nick"){
// NICK MVA
  varsF = {
  // NICK MVA
  "htt_nick_subjet_pt",
  "htt_nick_tau1",
  "htt_nick_tau2",
  "htt_nick_tau3",
  "htt_nick_ropt",
  "htt_nick_subjet_mass",
  "htt_nick_subjet_w_mass",
  "htt_nick_subjet_w_pt",
  "htt_nick_b_pt",
  "htt_nick_b_csv",
  "htt_nick_b_mass",
  "htt_nick_subjet_w_bycsv_mass",
  "htt_nick_subjet_w_bycsv_pt",
  "htt_nick_subjet_b_bycsv_pt",
  "htt_nick_subjet_b_bycsv_csv",
  "htt_nick_subjet_b_bycsv_mass",
  };
  varsI = {};
  } // nick

if(varset == "alex"){
  // ALEX MVA
varsF = {
//       "htt_pt",
       "htt_sdmass",
//       "htt_rawmass",
//       "htt_tau1",
//       "htt_tau2",
//       "htt_tau3",
//       "htt_tau31",
//       "htt_tau21",
//       "htt_tau32",
//       "htt_ropt_pt",
       "htt_ropt_mass",
//       "htt_ropt_tau1",
//       "htt_ropt_tau2",
//       "htt_ropt_tau3",
       "htt_ropt_tau21",
       "htt_ropt_tau32",
       "htt_ropt_tau31",
       "htt_ropt",
       "htt_ropt_deltaRopt",
       "htt_ropt_ptDR", // ropt drpt
//       "htt_ropt_calc",
       "htt_ropt_frec",

//       "htt_bydef_b_pt",
//       "htt_bydef_b_mass",
//       "htt_bydef_b_csv",
//       "htt_bydef_b_ptD",
//       "htt_bydef_b_axis1",
//       "htt_bydef_b_axis2",
//       "htt_bydef_b_mult",
//
//       "htt_bydef_w1_pt",
//       "htt_bydef_w1_csv",
//       "htt_bydef_w1_ptD",
//       "htt_bydef_w1_axis1",
//       "htt_bydef_w1_axis2",
//
//       "htt_bydef_w2_pt",
//       "htt_bydef_w2_csv",
//       "htt_bydef_w2_ptD",
//       "htt_bydef_w2_axis1",
//       "htt_bydef_w2_axis2",
//
//       "htt_bydef_w_pt",
//       "htt_bydef_w_mass",
//       "htt_bydef_w_ptDR",
//       "htt_bydef_w_deltaR",
//       "htt_bydef_w_ptDR",
//
//       "htt_bydef_t_deltaR",
//       "htt_bydef_t_ptDR",
//
//       "htt_bydef_b_w1_mass",
//       "htt_bydef_b_w2_mass",

 // bycsv vars
       "htt_bycsv_b_pt",
//       "htt_bycsv_b_mass",
//       "htt_bycsv_b_csv",
//       "htt_bycsv_b_ptD",
//       "htt_bycsv_b_axis1",
//       "htt_bycsv_b_axis2",
       "htt_bycsv_b_squaredaxis",

//       "htt_bycsv_w1_pt",
/////         "htt_bycsv_w1_mass",
//       "htt_bycsv_w1_csv",
//       "htt_bycsv_w1_ptD",
//       "htt_bycsv_w1_axis1",
//       "htt_bycsv_w1_axis2",
       "htt_bycsv_w1_squaredaxis",

//       "htt_bycsv_w2_pt",
//////         "htt_bycsv_w2_mass",
//       "htt_bycsv_w2_csv",
//       "htt_bycsv_w2_ptD",
//       "htt_bycsv_w2_axis1",
//       "htt_bycsv_w2_axis2",
       "htt_bycsv_w2_squaredaxis",

//       "htt_bycsv_w_pt",
       "htt_bycsv_w_mass",
//       "htt_bycsv_w_deltaR",
       "htt_bycsv_w_ptDR",
//       "htt_bycsv_t_deltaR",
//       "htt_bycsv_t_ptDR",
//       "htt_bycsv_b_w1_mass",
//       "htt_bycsv_b_w2_mass",
 };
   varsI = {
//       "htt_bydef_b_mult",
//       "htt_bydef_w1_mult",
//       "htt_bydef_w2_mult",
//       "htt_bycsv_b_mult",
//       "htt_bycsv_w1_mult",
//       "htt_bycsv_w2_mult",
   };
}//alex

#ifdef NEW_TMVA
   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");
#else
#define dataloader factory
#endif

   for (const auto &v : varsF)
     dataloader->AddVariable(v, 'F');

   for (const auto &v : varsI)
     dataloader->AddVariable(v, 'I');


   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables

//   dataloader->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
//   dataloader->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );


   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree    ( signal,     signalWeight     );
   dataloader->AddBackgroundTree( background, backgroundWeight );

   // To give different trees for training and testing, do as follows:
   //    dataloader->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
   //    dataloader->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );

   // Use the following code instead of the above two or four lines to add signal and background
   // training and test events "by hand"
   // NOTE that in this case one should not give expressions (such as "var1+var2") in the input
   //      variable definition, but simply compute the expression before adding the event
   //
   //     // --- begin ----------------------------------------------------------
   //     std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
   //     Float_t  treevars[4], weight;
   //
   //     // Signal
   //     for (UInt_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<signal->GetEntries(); i++) {
   //        signal->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < signal->GetEntries()/2.0) dataloader->AddSignalTrainingEvent( vars, signalWeight );
   //        else                              dataloader->AddSignalTestEvent    ( vars, signalWeight );
   //     }
   //
   //     // Background (has event weights)
   //     background->SetBranchAddress( "weight", &weight );
   //     for (UInt_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<background->GetEntries(); i++) {
   //        background->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < background->GetEntries()/2) dataloader->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
   //        else                                dataloader->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
   //     }
         // --- end ------------------------------------------------------------
   //
   // --- end of tree registration

   // Set individual event weights (the variables must exist in the original TTree)
   //    for signal    : dataloader->SetSignalWeightExpression    ("weight1*weight2");
   //    for background: dataloader->SetBackgroundWeightExpression("weight1*weight2");
//   dataloader->SetSignalWeightExpression    ( "1.0" );
//   dataloader->SetBackgroundWeightExpression( "1.0" );
   dataloader->SetSignalWeightExpression    ( "weight" ); //CHANGEME set training weight expression. small influence if eg bad b modeling + kinematic region defined by b cut.
   dataloader->SetBackgroundWeightExpression( "weight" );

   // Apply additional cuts on the signal and background samples (can be different)


   //CHANGEME define in mycuts and mycutb the signal and bkg cuts (usually: jet quality and is_matched for sig, jet quality and !is_matched for bkg)
   // NB that TCut objects don't add together like strings. They automatically add parentheses and "&&", eg do TCut cut = cut1+cut2 same thing as TString cut = "("+cut1+") && ("+cut2+")"

   TCut baseline = "nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0)) && (passmetfilters || process==10) && j1chEnFrac>0.1 && j1chEnFrac<0.99 && passjson && met>250 && njets>=5 && nbjets>=1 && nlbjets>=2 && dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5";
   //TCut baseline = baseline+"mtcsv12met>=175"; // training in high mtb
   TCut mycutb, mycuts;
   if(varset == "hqu"){
     mycuts = "";
     mycutb = "";
   }else{ // alex or nick
     mycuts = "";
     mycutb = "";
   }

   // Tell the dataloader how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used
   // for training, and the other half for testing:
   //    dataloader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // To also specify the number of testing events, use:
   //    dataloader->PrepareTrainingAndTestTree( mycut,
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   int nSig = signal->GetEntries(mycuts);
   int nBkg = background->GetEntries(mycutb);
   const double trainSplitRatio = 0.7;//CHANGEME 70% training, 30% testing is fine
   TString splitEvtStr = TString::Format("nTrain_Signal=%d:nTrain_Background=%d", int(trainSplitRatio*nSig), int(trainSplitRatio*nBkg));
   dataloader->PrepareTrainingAndTestTree( mycuts, mycutb, splitEvtStr+":SplitMode=Random:NormMode=NumEvents:!V" );

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

#ifdef NEW_TMVA
   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod( dataloader, TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

   if (Use["BDTG3"]) // Gradient Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG3",
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:nCuts=200" );
#else
   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Boosted Decision Trees
/*
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
*/
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2%:BoostType=Grad:Shrinkage=0.05:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=4" );

   if (Use["BDTG3"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG3",
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:nCuts=200" );

   if (Use["MLPBNN"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

#endif


   // For an example of the category classifier usage, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // ---- STILL EXPERIMENTAL and only implemented for BDT's !
   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   //factory->OptimizeAllMethods("ROCIntegral","FitGA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;

//   delete dataloader;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

}

