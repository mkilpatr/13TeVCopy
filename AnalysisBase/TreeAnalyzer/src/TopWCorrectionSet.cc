  // strategy feb 2017
  //   "method 1", or "reweighting via product"
  //   sfeff(X | gY) means SF(X | gY)*Eff(X | gY) where X is reco category and gY is gen category
  //
  // MERGED CASE:
  //   four categories for SFs and effs, two reco ( t or w exclusive ) and two gen ( t or w nonexclusive )
  //       P(data)  = product of sfeff( t | gt) for every fj in the ( t | gt ) category, true top tags
  //                * product of sfeff( w | gw) for -- , true w tags
  //                * product of sfeff( t | !gt) for --, top mistags
  //                * product of sfeff( w | !gw) for --, w mistags
  //                * product of ( 1 - sfeff( t | gt ) - sfeff( w | !gw ) ) for every fj in the ( !t!w | gt !gw ) category, missed opportunity tops (no overlapping w)
  //                * product of ( 1 - sfeff( t | !gt ) - sfeff( w | gw ) ) for every fj in the ( !t!w | !gt gw ) category, missed opportunity ws (no overlapping top)
  //                * product of ( 1 - sfeff( t | gt ) - sfeff( w | gw ) )  for every fj in the ( !t!w | gt gw  ) category, missed opportunity top and w overlapping
  //                * product of ( 1 - sfeff( t | !gt ) - sfeff( w | !gw) ) for every fj in the ( !t!w | !gt !gw ) category, darkness
  //       P(MC) = same, replacing sfeff by eff
  //    ---> event weight = P(data)/P(MC)
  //
  // RESOLVED CASE:
  //   two categories for SFs and effs, one reco (t) and one gen (t)
  //       P(data)  = product of sfeff( t | gt) for every candidate in the (t | gt) category, true tags
  //                * product of sfeff( t | !gt) for every candidate in the (t | !gt) category, mistags
  //                * product of ( 1 - sfeff( t | gt ) ) for every candidate in the (!t | gt) category, missed opportunity tops
  //                * product of ( 1 - sfeff( t | !gt ) ) for every candidate in the (!t | !gt) category, darkness
  //       P(MC) = same, replacing sfeff by eff
  //    ---> event weight = P(data)/P(MC)
  //
  //    stat uncertainties on effs and SFs (not systs!)
  //      generically sf = A*B*(1-C-D) etc, so combining product and sum rules,
  //        sf_unc/sf = sqrt( (A_unc/A)^2 + (b_unc/B)^2 + (C_unc^2 + D_unc^2)/(1-C-D)^2 )
  //      we can thus keep a running uncertainty on the pmc and pdata products (the arg of sqrt)
  //
  //    syst uncertainties
  //      ALL ARE DERIVED WITH DENOMINATOR THE NOMINAL CASE
  //      To apply (eg jump from usual madgraph to alternate powheg via powheg/madgraph MC effs ratio) must DIVIDE the usual SF by the systs histo:
  //        (new MC eff) = (powheg / madgraph)*(old MC eff), which means
  //        (new data/MC SF) = (data eff)/(new MC eff) = [(data eff)/(old MC eff)] / (powheg / madgraph), ie DIVIDE
  //
  //    FastSim
  //      DERIVED AS FULLSIM/FASTSIM
  //      we measure full/fastsim SFs to correct fastsim tagging efficiencies.
  //      when running on fastsim MC we want P(data) = E(data), and P(mc) = E(fast), and we have on file E(full) and SF(data/full) and E(full/fast).
  //        P(data) = E(data) = SF(data/full)*E(full)
  //                          = SF(data/full)*[E(full)/E(fast)] * E(full)/[E(full)/E(fast)]
  //        P(mc)   = E(fast) = E(full)/[E(full)/E(fast)]
  //      thus we need to take the SF from file and multiply it by the full/fast SF, and simultaneously divide the E from file by the same.
  //      this leaves P(data) unchanged and brings P(mc) to equal E(fast)
  //
  //  if you ever do unc = 1/2 the distance please use consistently TMath::Abs(1.0 - sf)*0.5;

#include "AnalysisBase/TreeAnalyzer/interface/TopWCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <TFile.h>
namespace ucsbsusy {

// load the needed files & histos
// strategy: they're all tiny histos loaded once at runtime. so LOAD THEM ALL. user can swap the filename+histname to the dummy root filename+"dummy" if needed.
SdMVACorr::SdMVACorr() : Correction("SdMVA") {

  sdMVAInputFile         = new TFile(fileName,"read");
  sdMVAFullFastInputFile = new TFile(fileNameFullFast,"read");
  sdMVASystsFile         = new TFile(fileNameSysts,"read");

  if(!sdMVAInputFile)         throw std::invalid_argument("SdMVACorr::SdMVACorr: correction file could not be found!");
  if(!sdMVASystsFile)         throw std::invalid_argument("SdMVACorr::SdMVACorr: systs correction file could not be found!");
  if(!sdMVAFullFastInputFile) throw std::invalid_argument("SdMVACorr::SdMVACorr: full/fast correction file could not be found!");

  // these sdMVAInputFile histos were updated Jan 30 2017

  // data/mc fullsim tag SFs
  sdMVA_DataFull_toptagSF["lowEta"]  = (TH1F*)( sdMVAInputFile->Get("sf_eff_top") );
  sdMVA_DataFull_toptagSF["highEta"] = (TH1F*)( sdMVAInputFile->Get("sf_eff_top") );
  sdMVA_DataFull_wtagSF["lowEta"]    = (TH1F*)( sdMVAInputFile->Get("sf_eff_w") );
  sdMVA_DataFull_wtagSF["highEta"]   = (TH1F*)( sdMVAInputFile->Get("sf_eff_w") );

  // data/mc fullsim mistag SFs
  sdMVA_DataFull_topmistagSF["lowEta"]  = (TH1F*)( sdMVAInputFile->Get("sf_mistag_top_barrel") );
  sdMVA_DataFull_topmistagSF["highEta"] = (TH1F*)( sdMVAInputFile->Get("sf_mistag_top_endcap") );
  sdMVA_DataFull_wmistagSF["lowEta"]    = (TH1F*)( sdMVAInputFile->Get("sf_mistag_w_barrel") );
  sdMVA_DataFull_wmistagSF["highEta"]   = (TH1F*)( sdMVAInputFile->Get("sf_mistag_w_endcap") );

  // fullsim mc tagging effs
  sdMVA_Full_toptagEff["lowEta"]       = (TH1F*)( sdMVAInputFile->Get("mc_eff_top") );
  sdMVA_Full_toptagEff["highEta"]      = (TH1F*)( sdMVAInputFile->Get("mc_eff_top") );
  sdMVA_Full_wtagEff["highEta"]        = (TH1F*)( sdMVAInputFile->Get("mc_eff_w") );
  sdMVA_Full_wtagEff["lowEta"]         = (TH1F*)( sdMVAInputFile->Get("mc_eff_w") );

  // fullsim mc mistagging effs
  sdMVA_Full_topmistagEff["lowEta"]    = (TH1F*)( sdMVAInputFile->Get("mc_mistag_top_barrel") );
  sdMVA_Full_topmistagEff["highEta"]   = (TH1F*)( sdMVAInputFile->Get("mc_mistag_top_endcap") );
  sdMVA_Full_wmistagEff["highEta"]     = (TH1F*)( sdMVAInputFile->Get("mc_mistag_w_barrel") );
  sdMVA_Full_wmistagEff["lowEta"]      = (TH1F*)( sdMVAInputFile->Get("mc_mistag_w_endcap") );

  // fullsim systematics
  sdMVA_Full_systs_t_ps      = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_t_sys_ps_0") );
  sdMVA_Full_systs_w_ps      = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_w_sys_ps_0") );
  sdMVA_Full_systs_t_gen     = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_t_sys_generator_0") );
  sdMVA_Full_systs_w_gen     = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_w_sys_generator_0") );
  sdMVA_Full_systs_t_mis_u   = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_t_sys_mistagsf_0") );
  sdMVA_Full_systs_t_mis_d   = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_t_sys_mistagsf-1") );
  sdMVA_Full_systs_w_mis_u   = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_w_sys_mistagsf_0") );
  sdMVA_Full_systs_w_mis_d   = (TH1F*)( sdMVASystsFile->Get("efnl1__tt_w_sys_mistagsf-1") );
  sdMVA_Full_systs_t_mis_ps["lowEta"]  = (TH1F*) sdMVASystsFile->Get("mtnl0__qcd_t_sys_ps_0");
  sdMVA_Full_systs_t_mis_ps["highEta"] = (TH1F*) sdMVASystsFile->Get("mtnl0__qcd_t_sys_ps_1");
  sdMVA_Full_systs_w_mis_ps["lowEta"]  = (TH1F*) sdMVASystsFile->Get("mtnl0__qcd_w_sys_ps_0");
  sdMVA_Full_systs_w_mis_ps["highEta"] = (TH1F*) sdMVASystsFile->Get("mtnl0__qcd_w_sys_ps_1");

  // full/fastsim SFs
  sdMVAFullFastSF_t            = (TH1F*)( sdMVAFullFastInputFile->Get("ak8_top_fullsim_by_fastsim_sf_vs_pt") );
  sdMVAFullFastSF_w            = (TH1F*)( sdMVAFullFastInputFile->Get("ak8_w_fullsim_by_fastsim_sf_vs_pt") );

  if(sdMVA_DataFull_toptagSF.empty()) throw std::invalid_argument("SdMVACorr::SdMVACorr: data/fullsim eff SF histograms could not be found!");
  if(sdMVA_Full_toptagEff.empty())    throw std::invalid_argument("SdMVACorr::SdMVACorr: fullsim eff histograms could not be found!");
  if(!sdMVA_Full_systs_t_ps)   throw std::invalid_argument("SdMVACorr::SdMVACorr: fullsim systs histograms could not be found!");
  if(!sdMVAFullFastSF_t)       throw std::invalid_argument("SdMVACorr::SdMVACorr: fullsim/fastsim eff SF histograms could not be found!");
}
SdMVACorr::~SdMVACorr(){
  sdMVAInputFile->Close();
  delete sdMVAInputFile;
  sdMVAFullFastInputFile->Close();
  delete sdMVAFullFastInputFile;
  sdMVASystsFile->Close();
  delete sdMVASystsFile;
}
float SdMVACorr::process(int correctionOptions, const std::vector<FatJetF*> &fatjets, bool isFastSim){
  bool dbg = false;
  if(correctionOptions == TopWCorrectionSet::NULLOPT) return 1;

  if(dbg) std::cout << std::endl << "[SdMVACorr::process] looping over n fatjets " << fatjets.size() << std::endl;
  auto getbin = [](float value, const TH1F * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  auto getbincontent = [getbin](float value, const TH1F * hist) -> float {return hist->GetBinContent(getbin(value,hist)); };
  auto getbinerror   = [getbin](float value, const TH1F * hist) -> float {return hist->GetBinError(getbin(value,hist)); };

  float wgt = 1, pdata = 1, pmc = 1, wgtunc = 0, pdataunc = 0, pmcunc = 0;
  for(auto fj : fatjets){
    if(dbg) std::cout << "  this loop's fj pt/eta: " << fj->pt() << " " << fj->eta() << std::endl;

    //if(fj->genCategory() == FatJetGenCategory::GENNOTFILLED)   throw std::invalid_argument(TString::Format("[SdMVACorr::process] Fatjet of pt %.2f has gen category not filled. Cannot continue. Compare with genCategory filler in BaseTreeAnalyzer.cc", fj->pt()) );
    //if(fj->recoCategory() == FatJetRecoCategory::RECONOTFILLED) throw std::invalid_argument(TString::Format("[SdMVACorr::process] Fatjet of pt %.2f has reco category not filled. Cannot continue. Compare with recoCategory filler in BaseTreeAnalyzer.cc", fj->pt()) );
    bool recotop = fj->recoCategory() & FatJetRecoCategory::SDMVATOP;
    bool recow   = fj->recoCategory() & FatJetRecoCategory::SDMVAW;
    bool gentop  = fj->genCategory()  & FatJetGenCategory::GENTOP;
    bool genw    = fj->genCategory()  & FatJetGenCategory::GENW;
    if(dbg) std::cout << "  this fatjet recotop, recow, gentop, genw cats " << recotop << " " << recow << " " << gentop << " " << genw << std::endl;
    float fjpt  = fj->pt();
    float fjabseta = TMath::Abs(fj->eta());
    std::string fjeta_str = (fjabseta > 1.5) ? "highEta" : "lowEta"; // here decide key into map<string,TH1F*>

    if( recotop || recow) {
      if(dbg) std::cout << "  in reco tagged cat" << std::endl;
      // reco tagged categories
      std::map<std::string, TH1F*> *sfhist = 0;
      std::map<std::string, TH1F*> *effhist = 0;
      if( recotop && gentop ){
        // t | gt
        sfhist = &sdMVA_DataFull_toptagSF,    effhist = &sdMVA_Full_toptagEff;
      }else if( recotop && !gentop ){
        // t | !gt
        sfhist = &sdMVA_DataFull_topmistagSF, effhist = &sdMVA_Full_topmistagEff;
      }else if( recow && genw ){
        // w | gw
        sfhist = &sdMVA_DataFull_wtagSF,      effhist = &sdMVA_Full_wtagEff;
      }else if( recow && !genw ){
        // w | !gw
        sfhist = &sdMVA_DataFull_wmistagSF,   effhist = &sdMVA_Full_wmistagEff;
      }else{
        assert(!"possible");
      }
      float sf   = getbincontent(fjpt, (*sfhist)[fjeta_str]);
      float eff  = getbincontent(fjpt, (*effhist)[fjeta_str]);
      float sfunc  = getbinerror(fjpt, (*sfhist)[fjeta_str]);
      if(dbg) std::cout << "  sf, sfunc, eff " << sf << " " << sfunc << " " << eff << " " << std::endl;

      ////// FASTSIM /////
      if(isFastSim && (recotop && gentop)){
        float fullfastsf = getbincontent(fjpt, sdMVAFullFastSF_t);
        if(dbg) std::cout << "   full/fast top sf " << fullfastsf << std::endl;
        sf *= fullfastsf; sfunc*= fullfastsf; eff /= fullfastsf;
      }
      if(isFastSim && (recow && genw)){
        float fullfastsf = getbincontent(fjpt, sdMVAFullFastSF_w);
        if(dbg) std::cout << "   full/fast w sf " << fullfastsf << std::endl;
        sf *= fullfastsf; sfunc*= fullfastsf; eff /= fullfastsf;
      }
      ////// END FASTSIM /////

      ///// SYSTEMATICS /////
      // the SF is in 'sf'. check if SF category is top or w and apply corresponding systematic.
      if(dbg) std::cout << "  merged tagged systs: " << std::endl;
      // variations on the efficiencies
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_STATS_W) && genw && recow){
        if(dbg) std::cout << "    merged stats W systs, sf after variation is " << sf + sfunc << std::endl;
        sf  += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_STATS_T) && gentop && recotop){
        if(dbg) std::cout << "    merged stats top systs, sf after variation is " << sf + sfunc << std::endl;
        sf  += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_PS) && ((genw && recow) || (gentop && recotop))){
        float syst = getbincontent(fjpt, (recotop ? sdMVA_Full_systs_t_ps : sdMVA_Full_systs_w_ps));
        if(dbg) std::cout << "    merged ps systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_GEN) && ((genw && recow) || (gentop && recotop))){
        float syst = getbincontent(fjpt, (recotop ? sdMVA_Full_systs_t_gen : sdMVA_Full_systs_w_gen));
        if(dbg) std::cout << "    merged gen systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_UP_W) && genw && recow){
        float syst = getbincontent(fjpt, sdMVA_Full_systs_w_mis_u);
        if(dbg) std::cout << "    merged mistag w up systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_UP_T) && gentop && recotop){
        float syst = getbincontent(fjpt, sdMVA_Full_systs_t_mis_u);
        if(dbg) std::cout << "    merged mistag t up systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      // variations on the mistag rate
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_STATS_W) && (!genw) && recow){
        if(dbg) std::cout << "    merged stats W systs, sf after variation is " << sf + sfunc << std::endl;
        sf  += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_STATS_T) && (!gentop) && recotop){
        if(dbg) std::cout << "    merged stats top systs, sf after variation is " << sf + sfunc << std::endl;
        sf  += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_NB) && ((!gentop && recotop) || (!genw && recow))){
        float syst = 1.2; // flat 20%
        if(dbg) std::cout << "    merged mistag nb systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_PS) && ((!genw && recow) || (!gentop && recotop))){
        float syst = getbincontent(fjpt, (recotop) ? sdMVA_Full_systs_t_mis_ps[fjeta_str] : sdMVA_Full_systs_w_mis_ps[fjeta_str] );
        if(dbg) std::cout << "    merged mistag ps systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_FASTSIM_STATS_W) && genw && recow){
        float percent = getbinerror(fjpt, sdMVAFullFastSF_w)/getbincontent(fjpt, sdMVAFullFastSF_w);
        if(dbg) std::cout << "    merged fastsim stats W systs, eff after variation is " << eff*(1+percent) << std::endl;
        eff  *= (1+percent);
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_FASTSIM_STATS_T) && gentop && recotop){
        float percent = getbinerror(fjpt, sdMVAFullFastSF_t)/getbincontent(fjpt, sdMVAFullFastSF_t);
        if(dbg) std::cout << "    merged fastsim stats T systs, eff after variation is " << eff*(1+percent) << std::endl;
        eff  *= (1+percent);
      }
      ///// END SYSTEMATICS /////

      pdata    *= sf*eff;
      pdataunc += pow(sfunc/sf,2);
      pmc      *= eff;
      pmcunc   += 0;
      if(dbg) std::cout << "  pdata, pdataunc, pmc, pmcunc " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;

    }else{
      if(dbg) std::cout << "  in reco untagged cat" << std::endl;
      // reco untagged categories
      std::map<std::string, TH1F*> *sfhistt = 0, *sfhistw = 0;
      std::map<std::string, TH1F*> *effhistt = 0, *effhistw = 0;

      if( gentop && genw ){
        // !t!w | gt gw
        sfhistt  = &sdMVA_DataFull_toptagSF;
        sfhistw  = &sdMVA_DataFull_wtagSF;
        effhistt = &sdMVA_Full_toptagEff;
        effhistw = &sdMVA_Full_wtagEff;
      }else if( gentop && !genw ){
        // !t!w | gt !gw
        sfhistt  = &sdMVA_DataFull_toptagSF;
        sfhistw  = &sdMVA_DataFull_wmistagSF;
        effhistt = &sdMVA_Full_toptagEff;
        effhistw = &sdMVA_Full_wmistagEff;
      }else if( !gentop && genw ){
        // !t!w | !gt gw
        sfhistt  = &sdMVA_DataFull_topmistagSF;
        sfhistw  = &sdMVA_DataFull_wtagSF;
        effhistt = &sdMVA_Full_topmistagEff;
        effhistw = &sdMVA_Full_wtagEff;
      }else if( !gentop && !genw ){
        // !t!w | !gt !gw
        sfhistt  = &sdMVA_DataFull_topmistagSF;
        sfhistw  = &sdMVA_DataFull_wmistagSF;
        effhistt = &sdMVA_Full_topmistagEff;
        effhistw = &sdMVA_Full_wmistagEff;
      }else{
        assert(!"possible");
      }
      float sft  = getbincontent(fjpt, (*sfhistt)[fjeta_str]);
      float sfw  = getbincontent(fjpt, (*sfhistw)[fjeta_str]);
      float efft = getbincontent(fjpt, (*effhistt)[fjeta_str]);
      float effw = getbincontent(fjpt, (*effhistw)[fjeta_str]);
      float sftunc  = getbinerror(fjpt, (*sfhistt)[fjeta_str]);
      float sfwunc  = getbinerror(fjpt, (*sfhistw)[fjeta_str]);
      if(dbg) std::cout << "  sft, sfunct, efft, " << sft << " " << sftunc << " " << efft << " " << std::endl;
      if(dbg) std::cout << "  sfw, sfuncw, effw, " << sfw << " " << sfwunc << " " << effw << " " << std::endl;

      ////// FASTSIM /////
      if(isFastSim && gentop){
        float fullfastsf = getbincontent(fjpt, sdMVAFullFastSF_t);
        if(dbg) std::cout << "   fullfast sf top: " << fullfastsf << std::endl;
        sft *= fullfastsf; sftunc *= fullfastsf; efft /= fullfastsf;
      }
      if(isFastSim && genw){
        float fullfastsf = getbincontent(fjpt, sdMVAFullFastSF_w);
        if(dbg) std::cout << "   fullfast sf w: " << fullfastsf << std::endl;
        sfw *= fullfastsf; sfwunc *= fullfastsf; effw /= fullfastsf;
      }
      ////// END FASTSIM /////


      ///// SYSTEMATICS /////
      // in untagged categories the top/w categories of the SFs are more clear.
      if(dbg) std::cout << "  merged untagged systs: " << std::endl;
      // variations on the efficiencies
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_STATS_W) && genw){
        sfw  += sfwunc;
        if(dbg) std::cout << "    merged stats W systs, sfw after variation is " << sfw+sfwunc << std::endl;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_STATS_T) && gentop){
        sft  += sftunc;
        if(dbg) std::cout << "    merged stats top systs, sft after variation is " << sft+sftunc << std::endl;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_PS)){
        float systt = gentop ? getbincontent(fjpt, sdMVA_Full_systs_t_ps) : 1;
        float systw = genw ? getbincontent(fjpt, sdMVA_Full_systs_w_ps) : 1;
        if(dbg) std::cout << "    merged ps systs, factor on sft, sfw of " << systt << " " << systw << std::endl;
        sft /= systt;
        sfw /= systw;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_GEN)){
        float systt = gentop ? getbincontent(fjpt, sdMVA_Full_systs_t_gen) : 1;
        float systw = genw ? getbincontent(fjpt, sdMVA_Full_systs_w_gen) : 1;
        if(dbg) std::cout << "    merged gen systs, factor on sft, sfw of " << systt << " " << systw << std::endl;
        sft /= systt;
        sfw /= systw;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_UP_W) && genw){
        float systw = getbincontent(fjpt, sdMVA_Full_systs_w_mis_u);
        if(dbg) std::cout << "    merged mistag w up systs, factor on sfw of " << systw << std::endl;
        sfw /= systw;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_UP_T) && gentop){
        float systt = getbincontent(fjpt, sdMVA_Full_systs_t_mis_u);
        if(dbg) std::cout << "    merged mistag t up systs, factor on sft of " << systt << std::endl;
        sft /= systt;
      }
      // variations on the mistag rate
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_STATS_W) && (!genw)){
        sfw  += sfwunc;
        if(dbg) std::cout << "    merged mistag stats W systs, sfw after variation is " << sfw+sfwunc << std::endl;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_STATS_T) && (!gentop)){
        sft  += sftunc;
        if(dbg) std::cout << "    merged mistag stats top systs, sft after variation is " << sft+sftunc << std::endl;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_NB)){
        float systt = (!gentop) ? 1.2 : 1;
        float systw = (!genw) ? 1.2 : 1;
        if(dbg) std::cout << "    merged mistag nb systs, factor on sft, sfw of " << systt << " " << systw << std::endl;
        sft /= systt;
        sfw /= systw;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_MISTAG_PS)){
        float systt = (!gentop) ? getbincontent(fjpt, sdMVA_Full_systs_t_mis_ps[fjeta_str] ) : 1.;
        float systw = (!genw) ? getbincontent(fjpt, sdMVA_Full_systs_w_mis_ps[fjeta_str] ) : 1.;
        if(dbg) std::cout << "    merged mistag ps systs, factor on sft, sfw of " << systt << " " << systw << std::endl;
        sft /= systt;
        sfw /= systw;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_FASTSIM_STATS_W) && genw){
        float percent = getbinerror(fjpt, sdMVAFullFastSF_w)/ getbincontent(fjpt, sdMVAFullFastSF_w);
        if(dbg) std::cout << "    merged fastsim stats W systs, eff after variation is " << effw*(1+percent) << std::endl;
        effw  *= (1+percent);
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_MERGED_FASTSIM_STATS_T) && gentop){
        float percent = getbinerror(fjpt, sdMVAFullFastSF_t)/getbincontent(fjpt, sdMVAFullFastSF_t);
        if(dbg) std::cout << "    merged fastsim stats T systs, eff after variation is " << efft*(1+percent) << std::endl;
        efft  *= (1+percent);
      }
      ///// END SYSTEMATICS /////

      pdata    *= (1 - sft*efft - sfw*effw);
      pdataunc += ( pow(sftunc/sft,2) + pow(sfwunc/sfw,2) )/pow(1 - sft*efft - sfw*effw, 2); // (C_unc^2 + D_unc^2)/(1-C-D)^2
      pmc      *= (1 - efft - effw);
      pmcunc   += 0;
      if(dbg) std::cout << "  pdata, pdataunc, pmc, pmcunc " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;
    }
  }//fatjets
  wgt      = pdata/pmc;
  pdataunc = pdata*sqrt(pdataunc); // convert to uncertainty
  pmcunc   = pmc*sqrt(pmcunc);
  wgtunc   = wgt*sqrt( pow(pdataunc/pdata,2) + pow(pmcunc/pmc,2) );
  if(dbg) std::cout << "finally, wgt, wgtunc, pdata, pdataunc, pmc, pmcunc " << wgt << " " << wgtunc << " " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;
  //wgt *= ( pmc == 0 ? pdata/pmc : 0); // some precautions
  //if(wgt > 0) wgt = max(  4.0, wgt );
  //if(wgt < 0) wgt = min( -4.0, wgt );

  return wgt;
}



ResMVATopCorr::ResMVATopCorr() : Correction("ResMVATop") {

  resMVATopInputFile      = new TFile(fileName,"read");
  resMVASystsFile         = new TFile(fileNameSysts,"read");
  resMVAFullFastInputFile = new TFile(fileNameFullFast,"read");
  if(!resMVATopInputFile)      throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: main file could not be found!");
  if(!resMVASystsFile)         throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: systs file could not be found!");
  if(!resMVAFullFastInputFile) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: full/fast file could not be found!");

  resTop_DataFull_toptagSF["lowEta"]    = (TH1F*)( resMVATopInputFile->Get("sf_eff_restop") );
  resTop_DataFull_toptagSF["highEta"]    = (TH1F*)( resMVATopInputFile->Get("sf_eff_restop") );

  resTop_DataFull_topmistagSF["lowEta"] = (TH1F*)( resMVATopInputFile->Get("sf_mistag_restop") );
  resTop_DataFull_topmistagSF["highEta"] = (TH1F*)( resMVATopInputFile->Get("sf_mistag_restop") );

  resTop_Full_toptagEff["lowEta"]        = (TH1F*)( resMVATopInputFile->Get("mc_eff_restop") );
  resTop_Full_toptagEff["highEta"]       = (TH1F*)( resMVATopInputFile->Get("mc_eff_restop") );
  resTop_Full_topmistagEff["lowEta"]     = (TH1F*)( resMVATopInputFile->Get("mc_mistag_restop") );
  resTop_Full_topmistagEff["highEta"]    = (TH1F*)( resMVATopInputFile->Get("mc_mistag_restop") );

  resTop_Full_systs_ps           = (TH1F*)( resMVASystsFile->Get("efnl1_res__tt_rest_sys_ps_0") );
  resTop_Full_systs_gen          = (TH1F*)( resMVASystsFile->Get("efnl1_res__tt_rest_sys_generator_0") );
  resTop_Full_systs_mis_u        = (TH1F*)( resMVASystsFile->Get("efnl1_res__tt_rest_sys_mistagsf_0") );
  resTop_Full_systs_mis_d        = (TH1F*)( resMVASystsFile->Get("efnl1_res__tt_rest_sys_mistagsf-1") );
  resTop_Full_systs_nmatch       = (TH1F*)( resMVASystsFile->Get("efnl1_res__tt_rest_sys_nmatch_0") );
  resTop_Full_systs_mis_ps       = (TH1F*)( resMVASystsFile->Get("mtnl0_res__qcd_rest_sys_ps_0") );

  resMVATopFullFastSF         = (TH1F*)( resMVAFullFastInputFile->Get("res_top_fullsim_by_fastsim_sf_vs_pt") );

  if(resTop_DataFull_toptagSF.empty()) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: data/fullsim eff SF histograms could not be found!");
  if(resTop_Full_toptagEff.empty()) throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: fullsim eff histograms could not be found!");
  if(!resTop_Full_systs_ps)     throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: systs histograms could not be found!");
  if(!resMVATopFullFastSF)      throw std::invalid_argument("ResMVATopCorr::ResMVATopCorr: fullsim/fastsim eff SF histograms could not be found!");
}
ResMVATopCorr::~ResMVATopCorr(){
  resMVATopInputFile->Close();
  delete resMVATopInputFile;
  resMVASystsFile->Close();
  delete resMVASystsFile;
  resMVAFullFastInputFile->Close();
  delete resMVAFullFastInputFile;
}
float ResMVATopCorr::process(int correctionOptions, const std::vector<TopCand> &resMVATops, const std::vector<PartonMatching::TopDecay*>& hadronicGenTops, bool isFastSim){
  bool dbg = false;
  if(dbg) std::cout << "working with n resTops " << resMVATops.size() << std::endl;

  if(correctionOptions == TopWCorrectionSet::NULLOPT) return 1;
  if(resMVATops.size() < 1) return 1;

  auto getbin = [](float value, const TH1F * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  auto getbincontent = [getbin](float value, const TH1F * hist) -> float {return hist->GetBinContent(getbin(value,hist)); };
  auto getbinerror   = [getbin](float value, const TH1F * hist) -> float {return hist->GetBinError(getbin(value,hist)); };

  float wgt = 1, pdata = 1, pmc = 1, wgtunc = 0, pdataunc = 0, pmcunc = 0;
  for(auto &cand : resMVATops){
    float candpt = cand.topcand.pt();
    float candabseta = TMath::Abs(cand.topcand.eta());
    std::string candeta_str = (candabseta > 1.5) ? "highEta" : "lowEta"; // here decide key into map<string,TH1F*>

    if(dbg) std::cout << "  this cand resTop pt, abseta " << candpt << " " << candabseta << std::endl;
    bool recotop = cand.disc > ResolvedTopMVA::WP_MEDIUM;
    bool gentop  = cand.nMatchedSubjets(hadronicGenTops) >= 2;
    if(dbg) std::cout << "  this cand recotop, gentop cats " << recotop << " " << gentop << std::endl;
    if( recotop ) {
      if(dbg) std::cout << "  in reco tagged cat" << std::endl;
      // reco tagged categories
      std::map<std::string, TH1F*> *sfhist;
      std::map<std::string, TH1F*> *effhist;
      if( recotop && gentop ){
        // t | gt
        sfhist = &resTop_DataFull_toptagSF,    effhist = &resTop_Full_toptagEff;
      }else if( recotop && !gentop ){
        // t | !gt
        sfhist = &resTop_DataFull_topmistagSF, effhist = &resTop_Full_topmistagEff;
      }else{
        assert(!"possible");
      }
      float sf   = getbincontent(candpt, (*sfhist)[candeta_str]);
      float eff  = getbincontent(candpt, (*effhist)[candeta_str]);
      float sfunc  = getbinerror(candpt, (*sfhist)[candeta_str]);
      if(dbg) std::cout << "  sf, sfunc, eff,  " << sf << " " << sfunc << " " << eff << " " << std::endl;

      ///// FASTSIM /////
      if(isFastSim && gentop){
        float fullfastsf = getbincontent(candpt, resMVATopFullFastSF);
        if(dbg) std::cout << "   fullfast sf res top: " << fullfastsf << std::endl;
        sf *= fullfastsf; sfunc *= fullfastsf; eff /= fullfastsf;
      }
      ///// END FASTSIM /////

      ///// SYSTEMATICS /////
      if(dbg) std::cout << "  resolved tagged systs: " << std::endl;
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_STATS) && gentop){
        if(dbg) std::cout << "    RESOLVED stats systs, sf after variation is " << sf + sfunc << std::endl;
        sf += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_PS) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_ps);
        if(dbg) std::cout << "    RESOLVED ps systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_GEN) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_gen);
        if(dbg) std::cout << "    RESOLVED gen systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_UP) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_mis_u);
        if(dbg) std::cout << "    RESOLVED mistag t up systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_NMATCH) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_nmatch);
        if(dbg) std::cout << "    RESOLVED nmatch systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      // mistag
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_STATS) && (!gentop)){
        if(dbg) std::cout << "    RESOLVED mistag stats systs, sf after variation is " << sf + sfunc << std::endl;
        sf += sfunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_NB) && (!gentop)){
        float syst = 1.2;
        if(dbg) std::cout << "    RESOLVED mistag nb systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_PS) && (!gentop)){
        float syst = getbincontent(candpt, resTop_Full_systs_mis_ps);
        if(dbg) std::cout << "    RESOLVED mistag PS systs, factor on sf of " << syst << std::endl;
        sf /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_FASTSIM_STATS) && gentop){
        float percent = getbinerror(candpt, resMVATopFullFastSF)/ getbincontent(candpt, resMVATopFullFastSF);
        if(dbg) std::cout << "    resolved fastsim stats T systs, eff after variation is " << eff*(1+percent) << std::endl;
        eff  *= (1+percent);
      }
      ///// END SYSTEMATICS /////

      pdata    *= sf*eff;
      pdataunc += pow(sfunc/sf,2);
      pmc      *= eff;
      pmcunc   += 0;
      if(dbg) std::cout << "  pdata, pdataunc, pmc, pmcunc " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;
    }else{
      if(dbg) std::cout << "  in reco untagged cat" << std::endl;
      // reco untagged categories
      std::map<std::string, TH1F*> *sfhistt;
      std::map<std::string, TH1F*> *effhistt;
      if( gentop ){
        // !t | gt
        sfhistt  = &resTop_DataFull_toptagSF;
        effhistt = &resTop_Full_toptagEff;
      }else if( !gentop ){
        // !t | !gt
        sfhistt  = &resTop_DataFull_topmistagSF;
        effhistt = &resTop_Full_topmistagEff;
      }else{
        assert(!"possible");
      }
      float sft  = getbincontent(candpt, (*sfhistt)[candeta_str]);
      float efft = getbincontent(candpt, (*effhistt)[candeta_str]);
      float sftunc  = getbinerror(candpt, (*sfhistt)[candeta_str]);
      if(dbg) std::cout << "  sft, sfunct, efft,  " << sft << " " << sftunc << " " << efft << " " << std::endl;

      ///// FASTSIM /////
      if(isFastSim && gentop){
        float fullfastsf = getbincontent(candpt, resMVATopFullFastSF);
        if(dbg) std::cout << "   fullfast sf res top: " << fullfastsf << std::endl;
        sft *= fullfastsf; sftunc *= fullfastsf; efft /= fullfastsf;
      }
      ///// END FASTSIM /////

      ///// SYSTEMATICS /////
      if(dbg) std::cout << "  resolved untagged systs: " << std::endl;
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_STATS) && gentop){
        if(dbg) std::cout << "    RESOLVED stats systs, sf after variation is " << sft + sftunc << std::endl;
        sft += sftunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_PS) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_ps);
        if(dbg) std::cout << "    RESOLVED ps systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_GEN) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_gen);
        if(dbg) std::cout << "    RESOLVED gen systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_UP) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_mis_u);
        if(dbg) std::cout << "    RESOLVED mistag t up systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_NMATCH) && gentop){
        float syst = getbincontent(candpt, resTop_Full_systs_nmatch);
        if(dbg) std::cout << "    RESOLVED nmatch systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      // mistag
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_STATS) && (!gentop)){
        if(dbg) std::cout << "    RESOLVED stats mistag systs, sf after variation is " << sft + sftunc << std::endl;
        sft += sftunc;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_NB) && (!gentop)){
        float syst = 1.2;
        if(dbg) std::cout << "    RESOLVED mistag nb systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_MISTAG_PS) && (!gentop)){
        float syst = getbincontent(candpt, resTop_Full_systs_mis_ps);
        if(dbg) std::cout << "    RESOLVED mistag PS systs, factor on sf of " << syst << std::endl;
        sft /= syst;
      }
      if((correctionOptions & TopWCorrectionSet::SYSTS_RESOLVED_FASTSIM_STATS) && gentop){
        float percent = getbinerror(candpt, resMVATopFullFastSF)/getbincontent(candpt, resMVATopFullFastSF);
        if(dbg) std::cout << "    resolved fastsim stats T systs, eff after variation is " << efft*(1+percent) << std::endl;
        efft  *= (1+percent);
      }
      ///// END SYSTEMATICS /////

      pdata    *= (1 - sft*efft);
      pdataunc += pow(sftunc/sft,2)/pow(1 - sft*efft, 2); // (C_unc^2 + D_unc^2)/(1-C-D)^2
      pmc      *= (1 - efft);
      pmcunc   += 0;
      if(dbg) std::cout << "  pdata, pdataunc, pmc, pmcunc " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;
    }
  }//cands
  wgt      = pdata/pmc;
  pdataunc = pdata*sqrt(pdataunc); // convert to uncertainty
  pmcunc   = pmc*sqrt(pmcunc);
  wgtunc   = wgt*sqrt( pow(pdataunc/pdata,2) + pow(pmcunc/pmc,2) );
  if(dbg) std::cout << "finally, wgt, wgtunc, pdata, pdataunc, pmc, pmcunc " << wgt << " " << wgtunc << " " << pdata << " " << pdataunc << " " << pmc << " " << pmcunc << std::endl;
  //wgt *= ( pmc == 0 ? pdata/pmc : 0); // some precautions
  //if(wgt > 0) wgt = max(  4.0, wgt );
  //if(wgt < 0) wgt = min( -4.0, wgt );

  return wgt;
}



////////// ICHEP16 OUTDATED //////////////
SdTopCorr::SdTopCorr(TString fileName) : Correction("SdTop"), sdTopinputFile(0), sdTopDataFullSF(0), sdTopFullFastSF(0) {
  sdTopinputFile = new TFile(fileName,"read");
  if(!sdTopinputFile) throw std::invalid_argument("SdTopCorr::SdTopCorr: file could not be found!");
  //sdTopDataFullSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_data_tagging__over__ak8toppasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdTopDataFullSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_data___over__ak8toppasspt_over_ak8candpt_mc_unsmeared_") );
  sdTopFullFastSF = (TH1F*)(sdTopinputFile->Get("ak8toppasspt_over_ak8candpt_t2tt-850-100-full_t2tt-850-100__over__ak8toppasspt_over_ak8candpt_t2tt-850-100-fast_t2tt-850-100") );
  if(!(sdTopDataFullSF || sdTopFullFastSF)) throw std::invalid_argument("SdTopCorr::SdTopCorr: eff SF histograms could not be found!");
}
SdTopCorr::~SdTopCorr(){
  sdTopinputFile->Close();
  delete sdTopinputFile;
}
float SdTopCorr::process(CORRTYPE corrType, double maxGoodTopPT){
  if(maxGoodTopPT <= 0.) return 1;
  if(corrType == NONE) return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdTopDataFullSF;
  TH1F * hff = sdTopFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodTopPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodTopPT,hff);

  // strategy: for data/full correction:
  //             for W tag, assign 3% uncertainty below 700 GeV, 10% above 700 GeV. no correction (consistent w/1).
  //             for top tag, correct per-bin and assign 1/2 of the corr as unc
  //           for full/fastsim correction:
  //             for top tag, correct per-bin the (400,450) and (800,inf) bins
  //             for w tag, correct per-bin the (200-250) bin
  //             for both, unc of 1/2 the difference from 1 if bin was corrected, 3% to all other bins which weren't corrected

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;

  float sf_ff     = 1.;
  float sf_ff_unc = 0.;
  if( (maxGoodTopPT > 400 && maxGoodTopPT < 450) || maxGoodTopPT > 800){
    sf_ff     = hff->GetBinContent(binFullFast);
    sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;
  }else{
    sf_ff = 1.;
    sf_ff_unc = 0.03;
  }

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdTopCorr::process: Invalid argument!");
  }
}



////////// ICHEP16 OUTDATED //////////////
SdWCorr::SdWCorr(TString fileName) : Correction("SdW"), sdWinputFile(0), sdWDataFullSF(0), sdWFullFastSF(0) {
  sdWinputFile = new TFile(fileName,"read");
  if(!sdWinputFile) throw std::invalid_argument("SdWCorr::SdWCorr: file could not be found!");
  //sdWDataFullSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_data_tagging__over__ak8wpasspt_over_ak8candpt_mc_unsmeared_tagging") );
  sdWDataFullSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_data___over__ak8wpasspt_over_ak8candpt_mc_unsmeared_") );
  sdWFullFastSF = (TH1F*)(sdWinputFile->Get("ak8wpasspt_over_ak8candpt_t2tt-850-100-full_t2tt-850-100__over__ak8wpasspt_over_ak8candpt_t2tt-850-100-fast_t2tt-850-100") );
  if(!(sdWDataFullSF || sdWFullFastSF)) throw std::invalid_argument("SdWCorr::SdWCorr: eff SF histograms could not be found!");
}
SdWCorr::~SdWCorr(){
  sdWinputFile->Close();
  delete sdWinputFile;
}
float SdWCorr::process(CORRTYPE corrType, double maxGoodWPT){
  if(maxGoodWPT <= 0.) return 1;
  if(corrType == NONE) return 1;
  auto getNoUnderOver = [](float value, const TH1 * hist) -> int {return std::min(std::max(hist->FindFixBin(value),1),hist->GetNbinsX());  };
  TH1F * hdf = sdWDataFullSF;
  TH1F * hff = sdWFullFastSF;
  int binDataFull = getNoUnderOver(maxGoodWPT,hdf);
  int binFullFast = getNoUnderOver(maxGoodWPT,hff);

  // strategy: for data/full correction:
  //             for W tag, assign 3% uncertainty below 700 GeV, 10% above 700 GeV. no correction (consistent w/1).
  //             for top tag, correct per-bin and assign 1/2 of the corr as unc
  //           for full/fastsim correction:
  //             for top tag, correct per-bin the (400,450) and (800,inf) bins
  //             for w tag, correct per-bin the (200-250) bin
  //             for both, unc of 1/2 the difference from 1 if bin was corrected, 3% to all other bins which weren't corrected

  float sf_df     = hdf->GetBinContent(binDataFull);
  float sf_df_unc = TMath::Abs(1 - sf_df)*.5;
  sf_df     = 1.; // overrride
  sf_df_unc = (maxGoodWPT < 700) ? 0.03 : 0.10;

  float sf_ff     = 1.;
  float sf_ff_unc = 0.;
  if( (maxGoodWPT > 200 && maxGoodWPT < 250) ){
    sf_ff     = hff->GetBinContent(binFullFast);
    sf_ff_unc = TMath::Abs(1 - sf_ff)*.5;
  }else{
    sf_ff = 1.;
    sf_ff_unc = 0.03;
  }

  float sf    = sf_ff*sf_df;
  float sfunc = sf*sqrt( pow(sf_df_unc/sf_df,2) + pow(sf_ff_unc/sf_ff,2) );

  switch(corrType){
    case UP:      return  sf + sfunc;
    case DOWN:    return  sf - sfunc;
    case NOMINAL: return  sf;
    default: throw std::invalid_argument("SdWCorr::process: Invalid argument!");
  }
}

// make corrector objects for each requested correction. keep forwarding options bits so they know which files/histos to load.
void TopWCorrectionSet::load(TString sdCorrName, int correctionOptions)
{
  name = "TOPW"; // necessary for src/BaseTreeAnalyzer when re-loading some corrections in processMoreVariables
  options_ = correctionOptions; // bring options bits into CorrectionSet class for use by processCorrection()

  if(correctionOptions & SDMVA) {
      sdMVACorr = new SdMVACorr();
      corrections.push_back(sdMVACorr);
  }
  if(correctionOptions & RESMVATOP) {
      resMVATopCorr = new ResMVATopCorr();
      corrections.push_back(resMVATopCorr);
  }
  ///// ICHEP16 OUTDATED /////
  if(correctionOptions & ICHEP16SDTOP) {
      sdTopCorr = new SdTopCorr(sdCorrName);
      corrections.push_back(sdTopCorr);
  }
  ///// ICHEP16 OUTDATED /////
  if(correctionOptions & ICHEP16SDW) {
      sdWCorr = new SdWCorr(sdCorrName);
      corrections.push_back(sdWCorr);
  }
}

// per-event, parse the options bits for correction type and build up the weights using each process(). the TopWCorrectionSet getters() give access to the TreeFiller.
// the systematics are handled by each corrector by passing the whole options bits to them.
void TopWCorrectionSet::processCorrection(const BaseTreeAnalyzer * ana) {

  bool dbg = false;
  if(dbg) std::cout << "[TopWCorrectionSet::processCorrection] event: " << ana->event << std::endl;

  sdMVAWeight     = 1;
  resMVATopWeight = 1;
  sdTopWeight     = 1; ///// ICHEP16 OUTDATED /////
  sdWWeight       = 1; ///// ICHEP16 OUTDATED /////
  if(!ana->isMC()) return;

  defaults::SignalType type = ana->evtInfoReader.signalType;
  bool isFastSim = ana->evtInfoReader.isfastsim;

  if(options_ & SDMVA) {
    if(dbg) std::cout << "Processing systs for SDMVA for n fatjets: " << ana->fatJets.size() << std::endl;
    sdMVAWeight = sdMVACorr->process(options_, ana->fatJets, isFastSim);
  }

  if(options_ & RESMVATOP) {
    if(dbg) std::cout << "Processing systs for RESMVATOP for n res cands: " << ana->resMVATopCands.size() << std::endl;
    resMVATopWeight = resMVATopCorr->process(options_, ana->resMVATopCands, ana->hadronicGenTops, isFastSim);
  }

  ///// ICHEP16 OUTDATED /////
  if(options_ & ICHEP16SDTOP) {
    if(ana->process == defaults::SIGNAL && type == defaults::T2tt){
      const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
      sdTopWeight = sdTopCorr->process(cfg.corrections.sdTopCorrType,ana->nSelSdTops == 0 ? -1 : ana->selectedSdTops.front()->pt());
    }else{
      sdTopWeight = 1.0;
    }
  }
  ///// ICHEP16 OUTDATED /////
  if(options_ & ICHEP16SDW) {
    if(ana->process == defaults::SIGNAL && (type == defaults::T2tt || type == defaults::T2bW)){
      const cfgSet::ConfigSet& cfg = ana->getAnaCfg();
      sdWWeight = sdWCorr->process(cfg.corrections.sdWCorrType,ana->nSelSdWs == 0 ? -1 : ana->selectedSdWs.front()->pt());
    }else{
      sdWWeight = 1.0;
    }
  }

}///TopWCorrectionSet

} /* namespace ucsbsusy */
