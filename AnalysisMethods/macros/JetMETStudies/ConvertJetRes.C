#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TF1Container.h"

//#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;



void getRes(){

  // Conversion regex
  //in: ^(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+\S+\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*$
  //out: eta1.push_back($1); eta2.push_back($2); rho1.push_back($3); rho2.push_back($4);ptMin.push_back($5); ptMax.push_back($6); p0.push_back($7); p1.push_back($8); p2.push_back($9); p3.push_back($10);


    std::vector<double> eta1;
    std::vector<double> eta2;
    std::vector<double> rho1;
    std::vector<double> rho2;
    std::vector<double> ptMin;
    std::vector<double> ptMax;
    std::vector<double> p0;
    std::vector<double> p1;
    std::vector<double> p2;
    std::vector<double> p3;


    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(299.865); p0.push_back(2.557); p1.push_back(0.1523); p2.push_back(0.0588); p3.push_back(-0.2175);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(292.6); p0.push_back(3.111); p1.push_back(0.1684); p2.push_back(0.000138); p3.push_back(-0.1774);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(295.1); p0.push_back(3.506); p1.push_back(0.1863); p2.push_back(2.267e-05); p3.push_back(-0.2249);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(299.6); p0.push_back(3.976); p1.push_back(0.2705); p2.push_back(0.09184); p3.push_back(-0.6338);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(298.1); p0.push_back(-21.36); p1.push_back(21.45); p2.push_back(0.104); p3.push_back(-1.986);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(300.4); p0.push_back(1.402); p1.push_back(3.892); p2.push_back(0.1354); p3.push_back(-1.908);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(308.9); p0.push_back(-0.02076); p1.push_back(4.758); p2.push_back(0.136); p3.push_back(-1.898);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(302.9); p0.push_back(6.215); p1.push_back(0.154); p2.push_back(0.0007419); p3.push_back(-0.04238);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(303.3); p0.push_back(6.949); p1.push_back(-1.793e-05); p2.push_back(0.1397); p3.push_back(-1.466);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(305.4); p0.push_back(7.753); p1.push_back(-5.195e-07); p2.push_back(0.1407); p3.push_back(-1.47);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(443.6); p0.push_back(4.573); p1.push_back(0.1926); p2.push_back(3.189e-06); p3.push_back(-0.2268);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(446.6); p0.push_back(5.63); p1.push_back(0.2078); p2.push_back(4.508e-06); p3.push_back(-0.2741);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(456.1); p0.push_back(6.06); p1.push_back(0.3396); p2.push_back(0.05938); p3.push_back(-0.5531);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(449.8); p0.push_back(7.013); p1.push_back(0.2743); p2.push_back(2.302e-06); p3.push_back(-0.3832);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(457.1); p0.push_back(-102.9); p1.push_back(102.4); p2.push_back(0.08302); p3.push_back(-1.995);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(641.6); p0.push_back(4.585); p1.push_back(0.2118); p2.push_back(6.684e-07); p3.push_back(-0.3778);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(636.2); p0.push_back(5.318); p1.push_back(0.2424); p2.push_back(2.644e-06); p3.push_back(-0.4489);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(638); p0.push_back(6.201); p1.push_back(0.2732); p2.push_back(2.313e-07); p3.push_back(-0.4922);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(644.6); p0.push_back(7.217); p1.push_back(0.2285); p2.push_back(2.705e-06); p3.push_back(-0.4242);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(644.8); p0.push_back(7.962); p1.push_back(0.3192); p2.push_back(5.103e-07); p3.push_back(-0.546);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(655.4); p0.push_back(4.205); p1.push_back(0.3201); p2.push_back(3.283e-06); p3.push_back(-0.586);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(847.5); p0.push_back(4.862); p1.push_back(0.3633); p2.push_back(2.844e-07); p3.push_back(-0.6215);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(849.3); p0.push_back(3.554); p1.push_back(1.426); p2.push_back(0.04178); p3.push_back(-1.191);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(847.9); p0.push_back(4.061); p1.push_back(1.696); p2.push_back(0.04539); p3.push_back(-1.251);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(816.9); p0.push_back(7.087); p1.push_back(0.4703); p2.push_back(0.02352); p3.push_back(-0.7083);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(891.8); p0.push_back(3.484); p1.push_back(0.4017); p2.push_back(0.02593); p3.push_back(-0.6917);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(875.2); p0.push_back(3.938); p1.push_back(0.5564); p2.push_back(0.03333); p3.push_back(-0.823);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(873.2); p0.push_back(4.801); p1.push_back(0.4242); p2.push_back(0.01964); p3.push_back(-0.6759);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(867.3); p0.push_back(5.449); p1.push_back(0.4428); p2.push_back(0.01493); p3.push_back(-0.6716);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(863.1); p0.push_back(6.281); p1.push_back(0.4152); p2.push_back(-2.964e-06); p3.push_back(-0.6261);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1189); p0.push_back(3.405); p1.push_back(0.4581); p2.push_back(0.03125); p3.push_back(-0.746);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1174); p0.push_back(3.857); p1.push_back(0.491); p2.push_back(0.03292); p3.push_back(-0.7656);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1161); p0.push_back(4.363); p1.push_back(0.5315); p2.push_back(0.03569); p3.push_back(-0.7876);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1163); p0.push_back(4.46); p1.push_back(0.6932); p2.push_back(0.03589); p3.push_back(-0.872);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1151); p0.push_back(5.394); p1.push_back(0.5988); p2.push_back(0.03651); p3.push_back(-0.8103);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1206); p0.push_back(2.663); p1.push_back(0.7106); p2.push_back(0.03652); p3.push_back(-0.9051);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1201); p0.push_back(3.108); p1.push_back(0.8232); p2.push_back(0.04012); p3.push_back(-0.9596);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1191); p0.push_back(3.444); p1.push_back(0.8742); p2.push_back(0.0407); p3.push_back(-0.9711);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1199); p0.push_back(2.351); p1.push_back(1.452); p2.push_back(0.04581); p3.push_back(-1.161);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1175); p0.push_back(4.126); p1.push_back(1.004); p2.push_back(0.04237); p3.push_back(-0.9977);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1631); p0.push_back(-1.259); p1.push_back(1.39); p2.push_back(0.05809); p3.push_back(-1.155);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1635); p0.push_back(-0.4585); p1.push_back(1.512); p2.push_back(0.05827); p3.push_back(-1.177);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1637); p0.push_back(2.314); p1.push_back(1.441); p2.push_back(0.05851); p3.push_back(-1.153);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1629); p0.push_back(3.078); p1.push_back(1.414); p2.push_back(0.05949); p3.push_back(-1.144);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1632); p0.push_back(3.526); p1.push_back(1.522); p2.push_back(0.06019); p3.push_back(-1.163);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2086); p0.push_back(1.783); p1.push_back(0.7418); p2.push_back(0.05023); p3.push_back(-0.8928);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2057); p0.push_back(2.566); p1.push_back(0.7314); p2.push_back(0.04992); p3.push_back(-0.8785);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2083); p0.push_back(2.988); p1.push_back(0.7753); p2.push_back(0.04999); p3.push_back(-0.8917);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2099); p0.push_back(3.449); p1.push_back(0.7868); p2.push_back(0.04921); p3.push_back(-0.8871);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2088); p0.push_back(3.802); p1.push_back(0.8917); p2.push_back(0.05064); p3.push_back(-0.9323);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2154); p0.push_back(1.85); p1.push_back(0.4623); p2.push_back(0.0341); p3.push_back(-0.705);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2627); p0.push_back(2.383); p1.push_back(0.4783); p2.push_back(0.03276); p3.push_back(-0.7081);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2619); p0.push_back(2.937); p1.push_back(0.5203); p2.push_back(0.03549); p3.push_back(-0.7447);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2593); p0.push_back(3.259); p1.push_back(0.5784); p2.push_back(0.03626); p3.push_back(-0.7783);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2630); p0.push_back(3.965); p1.push_back(0.5726); p2.push_back(0.03623); p3.push_back(-0.7726);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2168); p0.push_back(1.598); p1.push_back(0.368); p2.push_back(0.01819); p3.push_back(-0.614);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2169); p0.push_back(2.209); p1.push_back(0.4084); p2.push_back(0.02308); p3.push_back(-0.6567);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2173); p0.push_back(2.857); p1.push_back(0.41); p2.push_back(0.02228); p3.push_back(-0.6559);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2652); p0.push_back(3.384); p1.push_back(0.431); p2.push_back(0.02329); p3.push_back(-0.671);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2664); p0.push_back(3.661); p1.push_back(0.5082); p2.push_back(0.02756); p3.push_back(-0.7339);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(3132); p0.push_back(1.308); p1.push_back(0.3597); p2.push_back(0.01513); p3.push_back(-0.6041);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2668); p0.push_back(1.962); p1.push_back(0.4033); p2.push_back(0.01816); p3.push_back(-0.6447);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2671); p0.push_back(2.584); p1.push_back(0.4265); p2.push_back(0.02032); p3.push_back(-0.6648);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2666); p0.push_back(3.215); p1.push_back(0.4443); p2.push_back(0.02137); p3.push_back(-0.6788);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2661); p0.push_back(3.728); p1.push_back(0.4898); p2.push_back(0.02347); p3.push_back(-0.7146);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(3132); p0.push_back(1.308); p1.push_back(0.3597); p2.push_back(0.01513); p3.push_back(-0.6041);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2668); p0.push_back(1.962); p1.push_back(0.4033); p2.push_back(0.01816); p3.push_back(-0.6447);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2671); p0.push_back(2.584); p1.push_back(0.4265); p2.push_back(0.02032); p3.push_back(-0.6648);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2666); p0.push_back(3.215); p1.push_back(0.4443); p2.push_back(0.02137); p3.push_back(-0.6788);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2661); p0.push_back(3.728); p1.push_back(0.4898); p2.push_back(0.02347); p3.push_back(-0.7146);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2168); p0.push_back(1.598); p1.push_back(0.368); p2.push_back(0.01819); p3.push_back(-0.614);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2169); p0.push_back(2.209); p1.push_back(0.4084); p2.push_back(0.02308); p3.push_back(-0.6567);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2173); p0.push_back(2.857); p1.push_back(0.41); p2.push_back(0.02228); p3.push_back(-0.6559);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2652); p0.push_back(3.384); p1.push_back(0.431); p2.push_back(0.02329); p3.push_back(-0.671);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2664); p0.push_back(3.661); p1.push_back(0.5082); p2.push_back(0.02756); p3.push_back(-0.7339);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2154); p0.push_back(1.85); p1.push_back(0.4623); p2.push_back(0.0341); p3.push_back(-0.705);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2627); p0.push_back(2.383); p1.push_back(0.4783); p2.push_back(0.03276); p3.push_back(-0.7081);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2619); p0.push_back(2.937); p1.push_back(0.5203); p2.push_back(0.03549); p3.push_back(-0.7447);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2593); p0.push_back(3.259); p1.push_back(0.5784); p2.push_back(0.03626); p3.push_back(-0.7783);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2630); p0.push_back(3.965); p1.push_back(0.5726); p2.push_back(0.03623); p3.push_back(-0.7726);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(2086); p0.push_back(1.783); p1.push_back(0.7418); p2.push_back(0.05023); p3.push_back(-0.8928);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(2057); p0.push_back(2.566); p1.push_back(0.7314); p2.push_back(0.04992); p3.push_back(-0.8785);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(2083); p0.push_back(2.988); p1.push_back(0.7753); p2.push_back(0.04999); p3.push_back(-0.8917);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(2099); p0.push_back(3.449); p1.push_back(0.7868); p2.push_back(0.04921); p3.push_back(-0.8871);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(2088); p0.push_back(3.802); p1.push_back(0.8917); p2.push_back(0.05064); p3.push_back(-0.9323);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1631); p0.push_back(-1.259); p1.push_back(1.39); p2.push_back(0.05809); p3.push_back(-1.155);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1635); p0.push_back(-0.4585); p1.push_back(1.512); p2.push_back(0.05827); p3.push_back(-1.177);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1637); p0.push_back(2.314); p1.push_back(1.441); p2.push_back(0.05851); p3.push_back(-1.153);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1629); p0.push_back(3.078); p1.push_back(1.414); p2.push_back(0.05949); p3.push_back(-1.144);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1632); p0.push_back(3.526); p1.push_back(1.522); p2.push_back(0.06019); p3.push_back(-1.163);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1206); p0.push_back(2.663); p1.push_back(0.7106); p2.push_back(0.03652); p3.push_back(-0.9051);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1201); p0.push_back(3.108); p1.push_back(0.8232); p2.push_back(0.04012); p3.push_back(-0.9596);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1191); p0.push_back(3.444); p1.push_back(0.8742); p2.push_back(0.0407); p3.push_back(-0.9711);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1199); p0.push_back(2.351); p1.push_back(1.452); p2.push_back(0.04581); p3.push_back(-1.161);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1175); p0.push_back(4.126); p1.push_back(1.004); p2.push_back(0.04237); p3.push_back(-0.9977);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(1189); p0.push_back(3.405); p1.push_back(0.4581); p2.push_back(0.03125); p3.push_back(-0.746);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(1174); p0.push_back(3.857); p1.push_back(0.491); p2.push_back(0.03292); p3.push_back(-0.7656);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(1161); p0.push_back(4.363); p1.push_back(0.5315); p2.push_back(0.03569); p3.push_back(-0.7876);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(1163); p0.push_back(4.46); p1.push_back(0.6932); p2.push_back(0.03589); p3.push_back(-0.872);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(1151); p0.push_back(5.394); p1.push_back(0.5988); p2.push_back(0.03651); p3.push_back(-0.8103);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(891.8); p0.push_back(3.484); p1.push_back(0.4017); p2.push_back(0.02593); p3.push_back(-0.6917);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(875.2); p0.push_back(3.938); p1.push_back(0.5564); p2.push_back(0.03333); p3.push_back(-0.823);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(873.2); p0.push_back(4.801); p1.push_back(0.4242); p2.push_back(0.01964); p3.push_back(-0.6759);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(867.3); p0.push_back(5.449); p1.push_back(0.4428); p2.push_back(0.01493); p3.push_back(-0.6716);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(863.1); p0.push_back(6.281); p1.push_back(0.4152); p2.push_back(-2.964e-06); p3.push_back(-0.6261);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(655.4); p0.push_back(4.205); p1.push_back(0.3201); p2.push_back(3.283e-06); p3.push_back(-0.586);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(847.5); p0.push_back(4.862); p1.push_back(0.3633); p2.push_back(2.844e-07); p3.push_back(-0.6215);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(849.3); p0.push_back(3.554); p1.push_back(1.426); p2.push_back(0.04178); p3.push_back(-1.191);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(847.9); p0.push_back(4.061); p1.push_back(1.696); p2.push_back(0.04539); p3.push_back(-1.251);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(816.9); p0.push_back(7.087); p1.push_back(0.4703); p2.push_back(0.02352); p3.push_back(-0.7083);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(641.6); p0.push_back(4.585); p1.push_back(0.2118); p2.push_back(6.684e-07); p3.push_back(-0.3778);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(636.2); p0.push_back(5.318); p1.push_back(0.2424); p2.push_back(2.644e-06); p3.push_back(-0.4489);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(638); p0.push_back(6.201); p1.push_back(0.2732); p2.push_back(2.313e-07); p3.push_back(-0.4922);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(644.6); p0.push_back(7.217); p1.push_back(0.2285); p2.push_back(2.705e-06); p3.push_back(-0.4242);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(644.8); p0.push_back(7.962); p1.push_back(0.3192); p2.push_back(5.103e-07); p3.push_back(-0.546);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(443.6); p0.push_back(4.573); p1.push_back(0.1926); p2.push_back(3.189e-06); p3.push_back(-0.2268);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(446.6); p0.push_back(5.63); p1.push_back(0.2078); p2.push_back(4.508e-06); p3.push_back(-0.2741);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(456.1); p0.push_back(6.06); p1.push_back(0.3396); p2.push_back(0.05938); p3.push_back(-0.5531);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(449.8); p0.push_back(7.013); p1.push_back(0.2743); p2.push_back(2.302e-06); p3.push_back(-0.3832);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(457.1); p0.push_back(-102.9); p1.push_back(102.4); p2.push_back(0.08302); p3.push_back(-1.995);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(300.4); p0.push_back(1.402); p1.push_back(3.892); p2.push_back(0.1354); p3.push_back(-1.908);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(308.9); p0.push_back(-0.02076); p1.push_back(4.758); p2.push_back(0.136); p3.push_back(-1.898);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(302.9); p0.push_back(6.215); p1.push_back(0.154); p2.push_back(0.0007419); p3.push_back(-0.04238);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(303.3); p0.push_back(6.949); p1.push_back(-1.793e-05); p2.push_back(0.1397); p3.push_back(-1.466);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(305.4); p0.push_back(7.753); p1.push_back(-5.195e-07); p2.push_back(0.1407); p3.push_back(-1.47);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(0); rho2.push_back(6.73);ptMin.push_back(15); ptMax.push_back(299.9); p0.push_back(2.557); p1.push_back(0.1523); p2.push_back(0.0588); p3.push_back(-0.2175);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(6.73); rho2.push_back(12.51);ptMin.push_back(15); ptMax.push_back(292.6); p0.push_back(3.111); p1.push_back(0.1684); p2.push_back(0.000138); p3.push_back(-0.1774);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(12.51); rho2.push_back(18.29);ptMin.push_back(15); ptMax.push_back(295.1); p0.push_back(3.506); p1.push_back(0.1863); p2.push_back(2.267e-05); p3.push_back(-0.2249);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(18.29); rho2.push_back(24.08);ptMin.push_back(15); ptMax.push_back(299.6); p0.push_back(3.976); p1.push_back(0.2705); p2.push_back(0.09184); p3.push_back(-0.6338);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(24.08); rho2.push_back(44.31);ptMin.push_back(15); ptMax.push_back(298.1); p0.push_back(-21.36); p1.push_back(21.45); p2.push_back(0.104); p3.push_back(-1.986);




int nJetEtas= 26;
float jetEtas[]= {-4.7,-3.2,-3,-2.8,-2.5,-2.3,-2.1,-1.9,-1.7,-1.3,-1.1,-0.8,-0.5,0,0.5,0.8,1.1,1.3,1.7,1.9,2.1,2.3,2.5,2.8,3,3.2,4.7};
int nJetRhos= 5;
float jetRhos[]= {0,6.73,12.51,18.29,24.08,44.31};

QuickRefold::TF1Container * a = new QuickRefold::TF1Container("AK4ResTrend",2);
a->addAxis(0,"eta",nJetEtas,jetEtas);
a->addAxis(1,"rho",nJetRhos,jetRhos);
a->stopSetup();

for(unsigned int iB = 0; iB < eta1.size(); ++iB ){
    TString name = TString::Format("resTrend_%.2f_%.2f_%.2f_%.2f",eta1[iB],eta2[iB],rho1[iB],rho2[iB]);
    name.ReplaceAll("-","m");
    name.ReplaceAll(".","p");
//    TString formula = TString::Format("sqrt([0]*abs([0])/(min(max(double(15),x),double(%f))*min(max(double(15),x),double(%f)))+[1]*[1]*pow(min(max(double(15),x),double(%f)),[3])+[2]*[2])",ptMax[iB],ptMax[iB],ptMax[iB]);
    TString formula = TString::Format("sqrt([0]*abs([0])/(min(max(15.0,x),%f)*min(max(15.0,x),%f))+[1]*[1]*pow(min(max(15.0,x),%f),[3])+[2]*[2])",ptMax[iB],ptMax[iB],ptMax[iB]);

    TF1 * trend =new TF1(name,formula);
    trend->SetParameter(0,p0[iB]);
    trend->SetParameter(1,p1[iB]);
    trend->SetParameter(2,p2[iB]);
    trend->SetParameter(3,p3[iB]);
    double eta = (eta2[iB]+eta1[iB])/2;
    double rho = (rho2[iB]+rho1[iB])/2;
    cout << eta <<" "<< rho<<" "<< name <<" "<< formula << endl;

    a->setBin(0,eta) ;
    a->setBin(1,rho) ;
    a->setValue(*trend);

    TFile * f = new TFile("ak4JetResTrends_Spring16_25nsV6_MC_PtResolution_AK4PFchs.root","recreate");
    a->Write();
    f->Close();

}
}

void testRes(){
  TFile * f = new TFile("ak4JetResTrends_Spring16_25nsV6_MC_PtResolution_AK4PFchs.root","read");
  QuickRefold::TF1Container * a;
  f->GetObject("AK4ResTrend",a);

  for(float eta = -5.01; eta <= 5.0; eta+=.1){
    for(float rho = 0; rho <= 30; rho+=2){
//      cout << eta<<" "<< rho<<endl;
      a->setBin(0,eta);
      a->setBin(1,rho);
      const TF1& f = a->getValue();
      for(float pt = 10; pt <= 500; pt += 10){
        cout << TString::Format("%.3f ",f.Eval(pt));
      }
      cout << endl;
    }
  }


  f->Close();
}


void makeCorr() {
  TFile * f = new TFile("ak4JetResCorr_Spring16_25nsV6_MC_SF_AK4PFchs.root","recreate");
//  float bins[] = {0,.8,1.3,1.9,2.5,3.0,3.2,5.0};
//  float counts[] = {1.061,1.088,1.106,1.126,1.343,1.303,1.320};
//  float errors[] = {.023,.029,.030,.094,0.123,0.111,0.286};

  float bins[] = {0,0.5,0.8,1.1,1.3,1.7,1.9,2.1,2.3,2.5,2.8,3.0,3.2,5.0};
  float counts[] = {1.122,1.167,1.168,1.029,1.115, 1.041,1.167,1.094,1.168,1.266, 1.595,0.998,1.226};
  float errors[] = {0.026,0.048,0.046,0.066,0.03,  0.062,0.086,0.093,0.120,0.132, 0.175,0.066,0.145};

  int nBins = 13;
  TH1 * h = new TH1F("AK4ResCorr",";eta",nBins,bins);
  for(unsigned int iB = 0; iB < nBins; ++iB){
    h->SetBinContent(iB+1,counts[iB]);
    h->SetBinError(iB+1,errors[iB]);
  }
  h->Write();
  f->Close();
}

#endif



void ConvertJetRes(const TString inFile="jetResSkim_orig.root", const TString treeName = "Events", const TString outFile = "resCoreOut.root")
{
//  getRes();
//  testRes();
  makeCorr();
}
