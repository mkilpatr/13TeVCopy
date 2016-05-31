/*
  M2 calculation using OptiMass library

  Models (ttbar/ttbar1l/...) and subsystems (A/B/AB) are changed in the model files, not here
  Constraint switching is changed here (UseConstraint). Use a new variable for each choice of constraints, eg ttbar1l_m2cc_ab, ttbar1l-m2cx_ab
  See refs for complete instructions
  
  Currently available models (all AB subsystem) in AnalysisTools/Optimass/{models/example_models,src}:
    TTbar_AB:   ttbar dilepton
    TTbar1l_AB: ttbar 1-lepton
    TTbarll_AB: ttbar lost-lepton (1-lepton process with invisible = l1 + v1)
      
  To add a new model or subsystem:
    Navigate to AnalysisTools/OptiMass/
    Write out your model's decay chain and constraints (eg mW1 = mW2 and/or mT1 = mT2)
    Prepare a model card in model/model_card.xml. See model/examples/ for ttbar-2l and ttbar-1l, and the presentation in the references
      Two constraints give four variables (C=constrained, X=unconstrained): m2{cc,cx,xc,xx}
      Choose which constraints are enforced in UseConstraint below. Use new variable for each set of choices.
      The subsystem (ab, a, b) must be changed in the model card, not here.
    Run build_model_dictionary.py to generate your model's classes MyModel.cpp/h into src/ :
      'python build_model_dictionary.py ./src'
    Navigate to AnalysisTools/OptiMass and 'scram b clean; scram b -j8' to build & link the new model codes.
      I tried linking MyModel.cpp/h to M2.hh, but the linker failed. Current method works just fine.
    The generator also writes a skeleton ExRootAnalysis analyzer in model/main_src/main_MyModel.cpp. Borrow its filling code to make fewer mistakes.
      If you want to test your new model with a standalone copy of OptiMass instead of with trees, you use this in place of main.cpp. See ref.
    Add your model's class header, MyModel.h, as below. The OptiMass object must be initialized with the correct model class.

    Use this format for variables:
      <process>-mt<parent constrained? c/x><daughter constrained? c/x>_<subsystem a/b/ab>, eg ttbar1l_m2cc_ab, ttbar2l-m2cx-a, ...

  References:
    Standalone playing with OptiMass library
      https://github.com/ASPatterson/OptiMass-installation/blob/master/README.md
    OptiMass reference papers and presentations
      http://hep-pulgrim.ibs.re.kr/optimass/
      https://indico.cern.ch/event/514422/contributions/1193455/attachments/1266368/1874791/StopSearch_DoojinKim_CMS2016.pdf
      http://arxiv.org/pdf/1401.1449v2
      http://arxiv.org/pdf/1411.0664v2
      http://arxiv.org/pdf/1508.00589v2

  Common errors:
    you called GetSubsystemMass on a particle not in the model
      terminate called after throwing an instance of 'std::out_of_range'
      what():  _Map_base::at
    you used the wrong model
      all W,v masses come out ~ zero, or very low top mass distro
*/

#ifndef M2_HH
#define M2_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "AnalysisTools/OptiMass/src/MassMinimizer.h"
#include "AnalysisTools/OptiMass/src/ProcessTree.h"
#include "AnalysisTools/OptiMass/src/ALMController.h"
#include "AnalysisTools/OptiMass/src/TTbar_AB.h"
#include "AnalysisTools/OptiMass/src/TTbar1l_AB.h" 
#include "AnalysisTools/OptiMass/src/TTbarll_AB.h"

using namespace ucsbsusy;

bool cmpcsv(RecoJetF* a, RecoJetF* b) {return ( a->csv() > b->csv() );}

struct M2TreeFiller {

  M2TreeFiller() {}

  size i_ttbar1l_m2cc_ab;
  size i_ttbar1l_m2cc_mt1_ab;
  size i_ttbar1l_m2cc_mt2_ab;
  size i_ttbarll_m2cc_ab;
  size i_ttbarll_m2cc_mt1_ab;
  size i_ttbarll_m2cc_mt2_ab;

  void book(TreeWriterData* data) {
    i_ttbar1l_m2cc_ab                          = data->add<float>("","ttbar1l_m2cc_ab" ,"F",0);
    i_ttbar1l_m2cc_mt1_ab                      = data->add<float>("","ttbar1l_m2cc_mt1_ab","F",0);
    i_ttbar1l_m2cc_mt2_ab                      = data->add<float>("","ttbar1l_m2cc_mt2_ab","F",0);
    i_ttbarll_m2cc_ab                          = data->add<float>("","ttbarll_m2cc_ab" ,"F",0);
    i_ttbarll_m2cc_mt1_ab                      = data->add<float>("","ttbarll_m2cc_mt1_ab","F",0);
    i_ttbarll_m2cc_mt2_ab                      = data->add<float>("","ttbarll_m2cc_mt2_ab","F",0);
  }
 
  enum Model {TTBAR_1L, TTBAR_LL};
  
  void fillM2Info(TreeWriterData* data, BaseTreeAnalyzer* ana, M2TreeFiller::Model model) {

    bool coutOptiMass = false;
    gErrorIgnoreLevel = 1001; // Minuit is noisy

    float ttbar1l_m2cc_ab_     = -9.;
    float ttbar1l_m2cc_mt1_ab_ = -9.;
    float ttbar1l_m2cc_mt2_ab_ = -9.;
    float ttbarll_m2cc_ab_     = -9.;
    float ttbarll_m2cc_mt1_ab_ = -9.;
    float ttbarll_m2cc_mt2_ab_ = -9.;

    switch ( model ) {
      case TTBAR_1L: {
        // make model-specific OptiMass objects (same performance if made/torn down for every event, or once outside event loop)
        // optm = ttbar1l_m2cc_ab
        // require >0 selected leptons and >=4 jets to reconstruct ttbar-1l (2 bs and 2 for hadr. W)
        assert( (ana->nSelLeptons > 0) && (ana->nJets >= 4) );

        OptiMass::MassMinimizer *optm = 0;
        optm = static_cast<OptiMass::MassMinimizer *>(new OptiMass::TTbar1l_AB);
        optm->InitContainers();
        OptiMass::ProcessTree& process_tree = optm->GetProcessTree();
        OptiMass::ALMController& alm_controller = optm->GetALMController();

        // set met (never change unless artificially adding lep back to met)
        optm->SetInvisibleSubsystemMomenta(0,ana->met->px(),ana->met->py());

        // set visibles
        // b1,b2    = highest two csv jets same hemi as met+lep, b2 closest to w2
        // ja2, jb2 = hardest two jets opposite hemi as met+lep (w2 - ja2 jb2)
        // l1       = lep

        // lep
        MomentumF lep = ana->selectedLeptons.at(0)->p4();
        MomentumF w1 = ana->met->p4() + lep.p4();
        
        vector<RecoJetF*> sortedcsvjets = ana->jets;
        std::sort(sortedcsvjets.begin(), sortedcsvjets.end(), cmpcsv);
        RecoJetF* b1 = sortedcsvjets.at(0);
        RecoJetF* b2 = sortedcsvjets.at(1);
        float drb1w1 = abs(PhysicsUtilities::deltaPhi(*b1,w1));
        float drb2w1 = abs(PhysicsUtilities::deltaPhi(*b2,w1));
        if(drb2w1 < drb1w1) { // swap b1, b2
          RecoJetF* tmp = b1;
          b1 = b2; b2 = tmp;
        }

        // jets ja2 jb2 from w2
        MomentumF ja2, jb2;
        for(auto* j : ana->jets) {
          if( (j->pt() == b1->pt()) || (j->pt() == b2->pt()) ) continue; // no overlaps
          float drjw1 = abs(PhysicsUtilities::deltaPhi(*j,w1));
          if(drjw1 > (3.14/2.)) {
            if( ja2.pt() == 0. ) { 
              ja2 = j->p4();  
            } else if ( jb2.pt() == 0 ) {
              if(ja2.pt() < j->pt()) { // order ja2, jb2 by pt
                jb2 = ja2;
                ja2 = j->p4();
              } else{
                jb2 = j->p4();
              }
            }
          }
        }
        MomentumF w2 = ja2.p4() + jb2.p4();

        /* 
        // copy+paste the following from your generated AnalysisTools/OptiMass/src/main_<MyModel>.cpp to make fewer mistakes:
        //   - the template fillers, SetMomentumValue
        //   - the invisible filler, SetInitInvisibleMomentum
        //   - constraint function switching (but you can change this to be CC/CX/XC/XX) now)
        //   - ... and then fill them
        */
        optm->SetMomentumValue("b1_x",   b1->px());
        optm->SetMomentumValue("b1_y",   b1->py());
        optm->SetMomentumValue("b1_z",   b1->pz());
        optm->SetMomentumValue("b1_m",   b1->mass());
        optm->SetMomentumValue("l1_x",   lep.px());
        optm->SetMomentumValue("l1_y",   lep.py());
        optm->SetMomentumValue("l1_z",   lep.pz());
        optm->SetMomentumValue("l1_m",   lep.mass());
        optm->SetMomentumValue("b2_x",   b2->px());
        optm->SetMomentumValue("b2_y",   b2->py());
        optm->SetMomentumValue("b2_z",   b2->pz());
        optm->SetMomentumValue("b2_m",   b2->mass());
        optm->SetMomentumValue("w2_x",   w2.px());
        optm->SetMomentumValue("w2_y",   w2.py());
        optm->SetMomentumValue("w2_z",   w2.pz());
        optm->SetMomentumValue("w2_m",   w2.mass());

        // Invisible Particle (Trial) Mass Input : ( Initial values of invisible momenta can also be assigned here with simple key change of <particle_label>_m -> <particle_la$
        optm->SetInitInvisibleMomentum("v1_m", 0. ); 

       /*-------------------------------------------------------------+
        | Constraint Function Switching :                             |
        | >> Check the definition of the functions in the list above  |
        +-------------------------------------------------------------*/
        //
        optm->UseConstraint(0, true);// (true or false) for the constraint : t1.M() - t2.M() = 0.
        optm->UseConstraint(1, true);// (true or false) for the constraint : w1.M() - w2.M() = 0.

       /*-----------------------+
        | OptiMass Calculation  |
        +-----------------------*/
        optm->Calc();

        if(coutOptiMass){
        //---------------------------------------+
        //  Print output of the current result   |  
        //---------------------------------------+  
        cout << "==================================================" << endl;
        cout << "OptiMass    = "<< sqrt(optm->GetOptiMass()) << endl;
        cout << "==================================================" << endl;
        cout << "<Reconstructed Particle Masses in full decay system> " << endl;
        cout << "ab-mass = " << process_tree.GetSubsystemMass("ab") << endl;
        cout << "t1-mass = " << process_tree.GetSubsystemMass("t1") << endl;
        cout << "w1-mass = " << process_tree.GetSubsystemMass("w1") << endl;
        cout << "v1-mass = " << process_tree.GetSubsystemMass("v1") << endl;
        cout << "t2-mass = " << process_tree.GetSubsystemMass("t2") << endl;
        cout << "w2-mass = " << process_tree.GetSubsystemMass("w2") << endl;
        cout << "==================================================" << endl;
        cout << "modConst    = "<< alm_controller.GetSumSquaredConstraints() << endl; // norm of the constraint functions (turned-on)
        cout << "nIter       = "<< alm_controller.GetNumberIteration() << endl;
        cout << "nALM-Phase1 = "<< alm_controller.GetNumberPhase1() << endl;
        cout << "nALM-Phase2 = "<< alm_controller.GetNumberPhase2() << endl;
        cout << "==================================================" << endl;
        }

        ttbar1l_m2cc_ab_     = process_tree.GetSubsystemMass("ab");
        ttbar1l_m2cc_mt1_ab_ = process_tree.GetSubsystemMass("t1");
        ttbar1l_m2cc_mt2_ab_ = process_tree.GetSubsystemMass("t2");

        data->fill<float>(i_ttbar1l_m2cc_ab,     ttbar1l_m2cc_ab_ );
        data->fill<float>(i_ttbar1l_m2cc_mt1_ab, ttbar1l_m2cc_mt1_ab_);
        data->fill<float>(i_ttbar1l_m2cc_mt2_ab, ttbar1l_m2cc_mt2_ab_);
        delete optm;
        break;
      }//ttbar_1l

      case TTBAR_LL: {
        // code lost-lepton ... same as 1L, but with nSelLeptons==0.
        data->fill<float>(i_ttbarll_m2cc_ab,     ttbarll_m2cc_ab_ );
        data->fill<float>(i_ttbarll_m2cc_mt1_ab, ttbarll_m2cc_mt1_ab_);
        data->fill<float>(i_ttbarll_m2cc_mt2_ab, ttbarll_m2cc_mt2_ab_);
        break;
      }//ttbar_ll

    }
    return;

  }//fillM2Info

};//M2TreeFiller

#endif //M2_HH
