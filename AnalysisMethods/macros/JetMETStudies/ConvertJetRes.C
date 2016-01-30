#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/QuickRefold/interface/TF1Container.h"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include<iostream>

using namespace std;

void getRes(){

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

    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(319.576); p0.push_back(2.47); p1.push_back(0.2656); p2.push_back(0.08192); p3.push_back(-0.5689);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(328.4); p0.push_back(-14.09); p1.push_back(13.97); p2.push_back(0.1034); p3.push_back(-1.971);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(324.2); p0.push_back(3.839); p1.push_back(0.3175); p2.push_back(0.08782); p3.push_back(-0.6835);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(319.6); p0.push_back(4.521); p1.push_back(0.1928); p2.push_back(1.5e-07); p3.push_back(-0.2449);
    eta1.push_back(-4.7); eta2.push_back(-3.2); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(331.8); p0.push_back(4.979); p1.push_back(0.2058); p2.push_back(0.08534); p3.push_back(-0.468);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(340.1); p0.push_back(4.807); p1.push_back(4.39e-05); p2.push_back(0.1402); p3.push_back(-1.152);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(339.9); p0.push_back(5.599); p1.push_back(4.503e-06); p2.push_back(0.1433); p3.push_back(-1.414);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(336); p0.push_back(6.565); p1.push_back(1.32e-05); p2.push_back(0.1372); p3.push_back(-1.432);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(335.2); p0.push_back(7.386); p1.push_back(0.0001028); p2.push_back(0.1412); p3.push_back(-1.424);
    eta1.push_back(-3.2); eta2.push_back(-3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(337.8); p0.push_back(8.127); p1.push_back(2.033e-05); p2.push_back(0.1388); p3.push_back(-1.44);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(432.6); p0.push_back(4.531); p1.push_back(0.1378); p2.push_back(0.0002641); p3.push_back(-0.09081);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(436.5); p0.push_back(5.04); p1.push_back(0.4434); p2.push_back(0.09554); p3.push_back(-0.9167);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(436.4); p0.push_back(6.182); p1.push_back(0.1834); p2.push_back(0.0803); p3.push_back(-0.4117);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(441.2); p0.push_back(7.223); p1.push_back(0.617); p2.push_back(0.09261); p3.push_back(-1.166);
    eta1.push_back(-3); eta2.push_back(-2.8); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(442.3); p0.push_back(8.447); p1.push_back(0.0002492); p2.push_back(0.1003); p3.push_back(-1.47);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(598.4); p0.push_back(-0.02204); p1.push_back(2.758); p2.push_back(0.0733); p3.push_back(-1.692);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(604.2); p0.push_back(-3.6); p1.push_back(4.377); p2.push_back(0.0685); p3.push_back(-1.775);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(603.7); p0.push_back(-8.693); p1.push_back(9.393); p2.push_back(0.06802); p3.push_back(-1.928);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(609.2); p0.push_back(-33.58); p1.push_back(33.9); p2.push_back(0.06833); p3.push_back(-1.993);
    eta1.push_back(-2.8); eta2.push_back(-2.5); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(606.6); p0.push_back(7.819); p1.push_back(1.13e+08); p2.push_back(0.07231); p3.push_back(-14.34);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(822.5); p0.push_back(3.926); p1.push_back(0.2447); p2.push_back(0.0002373); p3.push_back(-0.4796);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(805.7); p0.push_back(1.798); p1.push_back(1.866); p2.push_back(0.05604); p3.push_back(-1.41);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(806.4); p0.push_back(4.341); p1.push_back(0.8304); p2.push_back(0.04588); p3.push_back(-1.043);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(791); p0.push_back(2.372); p1.push_back(2.707); p2.push_back(0.05653); p3.push_back(-1.517);
    eta1.push_back(-2.5); eta2.push_back(-2.3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(607.4); p0.push_back(5.757); p1.push_back(1.015); p2.push_back(0.05033); p3.push_back(-1.101);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(833.4); p0.push_back(2.666); p1.push_back(0.6073); p2.push_back(0.03841); p3.push_back(-0.9028);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(628.1); p0.push_back(3.244); p1.push_back(0.7134); p2.push_back(0.04336); p3.push_back(-0.9777);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(831.5); p0.push_back(4.05); p1.push_back(0.7653); p2.push_back(0.04598); p3.push_back(-1.016);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(816); p0.push_back(4.947); p1.push_back(0.6384); p2.push_back(0.03978); p3.push_back(-0.8972);
    eta1.push_back(-2.3); eta2.push_back(-2.1); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(820.7); p0.push_back(5.814); p1.push_back(0.5735); p2.push_back(0.03396); p3.push_back(-0.8122);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1087); p0.push_back(2.474); p1.push_back(0.7125); p2.push_back(0.03931); p3.push_back(-0.9523);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1091); p0.push_back(2.709); p1.push_back(0.8556); p2.push_back(0.03845); p3.push_back(-1.011);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1090); p0.push_back(3.402); p1.push_back(0.9552); p2.push_back(0.04044); p3.push_back(-1.062);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(836.9); p0.push_back(4.355); p1.push_back(0.8412); p2.push_back(0.03762); p3.push_back(-0.9916);
    eta1.push_back(-2.1); eta2.push_back(-1.9); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1070); p0.push_back(5.5); p1.push_back(0.5617); p2.push_back(0.02768); p3.push_back(-0.7904);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1143); p0.push_back(-0.9196); p1.push_back(1.261); p2.push_back(0.04051); p3.push_back(-1.145);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1132); p0.push_back(1.603); p1.push_back(1.266); p2.push_back(0.0416); p3.push_back(-1.146);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1126); p0.push_back(2.093); p1.push_back(1.419); p2.push_back(0.04109); p3.push_back(-1.178);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(1122); p0.push_back(1.719); p1.push_back(1.786); p2.push_back(-0.04281); p3.push_back(-1.257);
    eta1.push_back(-1.9); eta2.push_back(-1.7); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1122); p0.push_back(2.451); p1.push_back(1.818); p2.push_back(-0.04165); p3.push_back(-1.241);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1651); p0.push_back(1.212); p1.push_back(0.987); p2.push_back(0.04369); p3.push_back(-0.9724);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1640); p0.push_back(2.553); p1.push_back(0.9421); p2.push_back(0.04276); p3.push_back(-0.9513);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1648); p0.push_back(2.831); p1.push_back(1.089); p2.push_back(-0.04586); p3.push_back(-1.01);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(1630); p0.push_back(3.942); p1.push_back(0.9216); p2.push_back(0.0406); p3.push_back(-0.9265);
    eta1.push_back(-1.7); eta2.push_back(-1.3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1635); p0.push_back(4.31); p1.push_back(1.072); p2.push_back(0.04398); p3.push_back(-0.981);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1686); p0.push_back(1.873); p1.push_back(0.6535); p2.push_back(0.04663); p3.push_back(-0.8215);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(2159); p0.push_back(2.809); p1.push_back(0.594); p2.push_back(0.04259); p3.push_back(-0.7709);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(2175); p0.push_back(3.065); p1.push_back(0.6603); p2.push_back(0.04368); p3.push_back(-0.8076);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(2154); p0.push_back(4.138); p1.push_back(0.521); p2.push_back(0.03308); p3.push_back(-0.6907);
    eta1.push_back(-1.3); eta2.push_back(-1.1); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(2168); p0.push_back(4.559); p1.push_back(0.6244); p2.push_back(0.04449); p3.push_back(-0.7768);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(2666); p0.push_back(1.747); p1.push_back(0.4232); p2.push_back(0.03264); p3.push_back(-0.661);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(2670); p0.push_back(2.719); p1.push_back(0.394); p2.push_back(0.0259); p3.push_back(-0.6186);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(2651); p0.push_back(3.298); p1.push_back(0.415); p2.push_back(0.02981); p3.push_back(-0.6451);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(2678); p0.push_back(3.904); p1.push_back(0.3885); p2.push_back(0.02473); p3.push_back(-0.6047);
    eta1.push_back(-1.1); eta2.push_back(-0.8); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(2631); p0.push_back(4.348); p1.push_back(0.4395); p2.push_back(0.02954); p3.push_back(-0.6533);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(3125); p0.push_back(1.83); p1.push_back(0.3226); p2.push_back(0.01823); p3.push_back(-0.5606);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(3099); p0.push_back(2.422); p1.push_back(0.3583); p2.push_back(0.02385); p3.push_back(-0.6043);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(3155); p0.push_back(3.101); p1.push_back(0.3634); p2.push_back(0.02263); p3.push_back(-0.6054);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(3183); p0.push_back(3.669); p1.push_back(0.3911); p2.push_back(0.0253); p3.push_back(-0.6357);
    eta1.push_back(-0.8); eta2.push_back(-0.5); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(3157); p0.push_back(4.185); p1.push_back(0.4274); p2.push_back(0.02707); p3.push_back(-0.665);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(3662); p0.push_back(1.65); p1.push_back(0.3122); p2.push_back(0.01014); p3.push_back(-0.5396);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(3582); p0.push_back(2.349); p1.push_back(0.3483); p2.push_back(0.01886); p3.push_back(-0.587);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(3197); p0.push_back(3.096); p1.push_back(0.3558); p2.push_back(0.01943); p3.push_back(-0.5949);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(3606); p0.push_back(3.594); p1.push_back(0.4129); p2.push_back(0.02507); p3.push_back(-0.6554);
    eta1.push_back(-0.5); eta2.push_back(0); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(3723); p0.push_back(4.101); p1.push_back(0.472); p2.push_back(0.02734); p3.push_back(-0.704);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(3662); p0.push_back(1.65); p1.push_back(0.3122); p2.push_back(0.01014); p3.push_back(-0.5396);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(3582); p0.push_back(2.349); p1.push_back(0.3483); p2.push_back(0.01886); p3.push_back(-0.587);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(3197); p0.push_back(3.096); p1.push_back(0.3558); p2.push_back(0.01943); p3.push_back(-0.5949);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(3606); p0.push_back(3.594); p1.push_back(0.4129); p2.push_back(0.02507); p3.push_back(-0.6554);
    eta1.push_back(0); eta2.push_back(0.5); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(3723); p0.push_back(4.101); p1.push_back(0.472); p2.push_back(0.02734); p3.push_back(-0.704);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(3125); p0.push_back(1.83); p1.push_back(0.3226); p2.push_back(0.01823); p3.push_back(-0.5606);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(3099); p0.push_back(2.422); p1.push_back(0.3583); p2.push_back(0.02385); p3.push_back(-0.6043);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(3155); p0.push_back(3.101); p1.push_back(0.3634); p2.push_back(0.02263); p3.push_back(-0.6054);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(3183); p0.push_back(3.669); p1.push_back(0.3911); p2.push_back(0.0253); p3.push_back(-0.6357);
    eta1.push_back(0.5); eta2.push_back(0.8); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(3157); p0.push_back(4.185); p1.push_back(0.4274); p2.push_back(0.02707); p3.push_back(-0.665);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(2666); p0.push_back(1.747); p1.push_back(0.4232); p2.push_back(0.03264); p3.push_back(-0.661);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(2670); p0.push_back(2.719); p1.push_back(0.394); p2.push_back(0.0259); p3.push_back(-0.6186);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(2651); p0.push_back(3.298); p1.push_back(0.415); p2.push_back(0.02981); p3.push_back(-0.6451);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(2678); p0.push_back(3.904); p1.push_back(0.3885); p2.push_back(0.02473); p3.push_back(-0.6047);
    eta1.push_back(0.8); eta2.push_back(1.1); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(2631); p0.push_back(4.348); p1.push_back(0.4395); p2.push_back(0.02954); p3.push_back(-0.6533);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1686); p0.push_back(1.873); p1.push_back(0.6535); p2.push_back(0.04663); p3.push_back(-0.8215);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(2159); p0.push_back(2.809); p1.push_back(0.594); p2.push_back(0.04259); p3.push_back(-0.7709);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(2175); p0.push_back(3.065); p1.push_back(0.6603); p2.push_back(0.04368); p3.push_back(-0.8076);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(2154); p0.push_back(4.138); p1.push_back(0.521); p2.push_back(0.03308); p3.push_back(-0.6907);
    eta1.push_back(1.1); eta2.push_back(1.3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(2168); p0.push_back(4.559); p1.push_back(0.6244); p2.push_back(0.04449); p3.push_back(-0.7768);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1651); p0.push_back(1.212); p1.push_back(0.987); p2.push_back(0.04369); p3.push_back(-0.9724);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1640); p0.push_back(2.553); p1.push_back(0.9421); p2.push_back(0.04276); p3.push_back(-0.9513);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1648); p0.push_back(2.831); p1.push_back(1.089); p2.push_back(-0.04586); p3.push_back(-1.01);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(1630); p0.push_back(3.942); p1.push_back(0.9216); p2.push_back(0.0406); p3.push_back(-0.9265);
    eta1.push_back(1.3); eta2.push_back(1.7); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1635); p0.push_back(4.31); p1.push_back(1.072); p2.push_back(0.04398); p3.push_back(-0.981);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1143); p0.push_back(-0.9196); p1.push_back(1.261); p2.push_back(0.04051); p3.push_back(-1.145);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1132); p0.push_back(1.603); p1.push_back(1.266); p2.push_back(0.0416); p3.push_back(-1.146);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1126); p0.push_back(2.093); p1.push_back(1.419); p2.push_back(0.04109); p3.push_back(-1.178);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(1122); p0.push_back(1.719); p1.push_back(1.786); p2.push_back(-0.04281); p3.push_back(-1.257);
    eta1.push_back(1.7); eta2.push_back(1.9); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1122); p0.push_back(2.451); p1.push_back(1.818); p2.push_back(-0.04165); p3.push_back(-1.241);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(1087); p0.push_back(2.474); p1.push_back(0.7125); p2.push_back(0.03931); p3.push_back(-0.9523);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(1091); p0.push_back(2.709); p1.push_back(0.8556); p2.push_back(0.03845); p3.push_back(-1.011);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(1090); p0.push_back(3.402); p1.push_back(0.9552); p2.push_back(0.04044); p3.push_back(-1.062);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(836.9); p0.push_back(4.355); p1.push_back(0.8412); p2.push_back(0.03762); p3.push_back(-0.9916);
    eta1.push_back(1.9); eta2.push_back(2.1); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(1070); p0.push_back(5.5); p1.push_back(0.5617); p2.push_back(0.02768); p3.push_back(-0.7904);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(833.4); p0.push_back(2.666); p1.push_back(0.6073); p2.push_back(0.03841); p3.push_back(-0.9028);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(628.1); p0.push_back(3.244); p1.push_back(0.7134); p2.push_back(0.04336); p3.push_back(-0.9777);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(831.5); p0.push_back(4.05); p1.push_back(0.7653); p2.push_back(0.04598); p3.push_back(-1.016);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(816); p0.push_back(4.947); p1.push_back(0.6384); p2.push_back(0.03978); p3.push_back(-0.8972);
    eta1.push_back(2.1); eta2.push_back(2.3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(820.7); p0.push_back(5.814); p1.push_back(0.5735); p2.push_back(0.03396); p3.push_back(-0.8122);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(822.5); p0.push_back(3.926); p1.push_back(0.2447); p2.push_back(0.0002373); p3.push_back(-0.4796);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(805.7); p0.push_back(1.798); p1.push_back(1.866); p2.push_back(0.05604); p3.push_back(-1.41);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(806.4); p0.push_back(4.341); p1.push_back(0.8304); p2.push_back(0.04588); p3.push_back(-1.043);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(791); p0.push_back(2.372); p1.push_back(2.707); p2.push_back(0.05653); p3.push_back(-1.517);
    eta1.push_back(2.3); eta2.push_back(2.5); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(607.4); p0.push_back(5.757); p1.push_back(1.015); p2.push_back(0.05033); p3.push_back(-1.101);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(598.4); p0.push_back(-0.02204); p1.push_back(2.758); p2.push_back(0.0733); p3.push_back(-1.692);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(604.2); p0.push_back(-3.6); p1.push_back(4.377); p2.push_back(0.0685); p3.push_back(-1.775);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(603.7); p0.push_back(-8.693); p1.push_back(9.393); p2.push_back(0.06802); p3.push_back(-1.928);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(609.2); p0.push_back(-33.58); p1.push_back(33.9); p2.push_back(0.06833); p3.push_back(-1.993);
    eta1.push_back(2.5); eta2.push_back(2.8); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(606.6); p0.push_back(7.819); p1.push_back(1.13e+08); p2.push_back(0.07231); p3.push_back(-14.34);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(432.6); p0.push_back(4.531); p1.push_back(0.1378); p2.push_back(0.0002641); p3.push_back(-0.09081);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(436.5); p0.push_back(5.04); p1.push_back(0.4434); p2.push_back(0.09554); p3.push_back(-0.9167);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(436.4); p0.push_back(6.182); p1.push_back(0.1834); p2.push_back(0.0803); p3.push_back(-0.4117);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(441.2); p0.push_back(7.223); p1.push_back(0.617); p2.push_back(0.09261); p3.push_back(-1.166);
    eta1.push_back(2.8); eta2.push_back(3); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(442.3); p0.push_back(8.447); p1.push_back(0.0002492); p2.push_back(0.1003); p3.push_back(-1.47);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(340.1); p0.push_back(4.807); p1.push_back(4.39e-05); p2.push_back(0.1402); p3.push_back(-1.152);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(339.9); p0.push_back(5.599); p1.push_back(4.503e-06); p2.push_back(0.1433); p3.push_back(-1.414);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(336); p0.push_back(6.565); p1.push_back(1.32e-05); p2.push_back(0.1372); p3.push_back(-1.432);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(335.2); p0.push_back(7.386); p1.push_back(0.0001028); p2.push_back(0.1412); p3.push_back(-1.424);
    eta1.push_back(3); eta2.push_back(3.2); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(337.8); p0.push_back(8.127); p1.push_back(2.033e-05); p2.push_back(0.1388); p3.push_back(-1.44);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(0); rho2.push_back(7.8); ptMin.push_back(15); ptMax.push_back(319.6); p0.push_back(2.47); p1.push_back(0.2656); p2.push_back(0.08192); p3.push_back(-0.5689);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(7.8); rho2.push_back(14.38); ptMin.push_back(15); ptMax.push_back(328.4); p0.push_back(-14.09); p1.push_back(13.97); p2.push_back(0.1034); p3.push_back(-1.971);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(14.38); rho2.push_back(20.97); ptMin.push_back(15); ptMax.push_back(324.2); p0.push_back(3.839); p1.push_back(0.3175); p2.push_back(0.08782); p3.push_back(-0.6835);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(20.97); rho2.push_back(27.55); ptMin.push_back(15); ptMax.push_back(319.6); p0.push_back(4.521); p1.push_back(0.1928); p2.push_back(1.5e-07); p3.push_back(-0.2449);
    eta1.push_back(3.2); eta2.push_back(4.7); rho1.push_back(27.55); rho2.push_back(34.14); ptMin.push_back(15); ptMax.push_back(331.8); p0.push_back(4.979); p1.push_back(0.2058); p2.push_back(0.08534); p3.push_back(-0.468);




int nJetEtas= 26;
float jetEtas[]= {-4.7,-3.2,-3,-2.8,-2.5,-2.3,-2.1,-1.9,-1.7,-1.3,-1.1,-0.8,-0.5,0,0.5,0.8,1.1,1.3,1.7,1.9,2.1,2.3,2.5,2.8,3,3.2,4.7};
int nJetRhos= 5;
float jetRhos[]= {0,7.8,14.38,20.97,27.55,34.14};

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

    TFile * f = new TFile("ak4JetResTrends_Summer15_25nsV6_MC_PtResolution_AK4PFchs.root","recreate");
    a->Write();
    f->Close();

}
}

void testRes(){
  TFile * f = new TFile("ak4JetResTrends_Summer15_25nsV6_MC_PtResolution_AK4PFchs.root","read");
  QuickRefold::TF1Container * a;
  f->GetObject("AK4ResTrend",a);

  for(float eta = -5.01; eta <= 5.0; eta+=.1){
    for(float rho = 0; rho <= 30; rho+=2){
//      cout << eta<<" "<< rho<<endl;
      a->setBin(0,eta);
      a->setBin(1,rho);
      const TF1& f = a->getValue();
      for(float pt = 10; pt <= 500; pt += 10){
        cout << TString::Format("%.1f ",f.Eval(pt));
      }
      cout << endl;
    }
  }


  f->Close();
}


void makeCorr() {
  TFile * f = new TFile("ak4JetResCorr_Summer15_25nsV6_MC_PtResolution_AK4PFchs.root","recreate");
  float bins[] = {0,.8,1.3,1.9,2.5,3.0,3.2,5.0};
  float counts[] = {1.061,1.088,1.106,1.126,1.343,1.303,1.320};
  float errors[] = {.023,.029,.030,.094,0.123,0.111,0.286};
  int nBins = 7;
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
