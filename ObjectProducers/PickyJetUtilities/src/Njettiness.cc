//  Njettiness Package
//  Version 0.5.1 (September 19, 2012)
//  Questions/Comments?  jthaler@jthaler.net

// Copyright (c) 2011-12, Jesse Thaler, Ken Van Tilburg, and Christopher K.
// Vermilion
//
//----------------------------------------------------------------------
// This file is part of the N-jettiness package ("N-jettiness").
//
//  N-jettiness is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SpartyJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SpartyJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------



#include "../interface/Njettiness.hh"

#include "../interface/NjettinessPlugin.hh"
#include "../interface/Nsubjettiness.hh"


///////
//
// Parameter classes
//
///////


// N-subjettiness pieces
std::vector<double> MeasureFunctor::subTaus(const std::vector <fastjet::PseudoJet> & particles, const std::vector<fastjet::PseudoJet>& axes) {// Returns the sub-tau values, i.e. a std::vector of the contributions to tau_N of each Voronoi region (or region within R_0)
      
   std::vector<double> tauNum(axes.size(), 0.0), tau(axes.size());
   
   double tauDen = 0.0;
   for (unsigned i = 0; i < particles.size(); i++) {
      // find minimum distance; start with 0'th axis for reference
      int j_min = 0;
      double minR = distance(particles[i],axes[0]);
      for (unsigned j = 1; j < axes.size(); j++) {
         double tempR = distance(particles[i],axes[j]); // delta R distance
         if (tempR < minR) {minR = tempR; j_min = j;}
      }
      tauNum[j_min] += numerator(particles[i],axes[j_min]);
      tauDen += denominator(particles[i]);
   }
   for (unsigned j = 0; j < axes.size(); j++) {
      tau[j] = tauNum[j]/tauDen;
   }
   return tau;
}

double MeasureFunctor::tau(const std::vector <fastjet::PseudoJet>& particles, const std::vector<fastjet::PseudoJet>& axes) {// Calculates tau_N
   std::vector<double> tau_vec = subTaus(particles, axes);
   double tau = 0.0;
   for (unsigned j = 0; j < tau_vec.size(); j++) {tau += tau_vec[j];}
   return tau;
}


///////
//
// Axes Finder Options
//
///////

/// Minimum Axes


///////
//
// Functions for minimization.
// TODO:  Wrap these in N-subjettiness class
//
///////



// Given starting axes, update to find better axes
// (This is just a wrapper for the templated version above.)
std::vector<LightLikeAxis> UpdateAxes(const std::vector <LightLikeAxis> & old_axes, 
                                      const std::vector <fastjet::PseudoJet> & inputJets, NsubParameters paraNsub, double precision) {
   int N = old_axes.size();
   switch (N) {
      case 1: return UpdateAxesFast<1>(old_axes, inputJets, paraNsub, precision);
      case 2: return UpdateAxesFast<2>(old_axes, inputJets, paraNsub, precision);
      case 3: return UpdateAxesFast<3>(old_axes, inputJets, paraNsub, precision);
      case 4: return UpdateAxesFast<4>(old_axes, inputJets, paraNsub, precision);
      case 5: return UpdateAxesFast<5>(old_axes, inputJets, paraNsub, precision);
      case 6: return UpdateAxesFast<6>(old_axes, inputJets, paraNsub, precision);
      case 7: return UpdateAxesFast<7>(old_axes, inputJets, paraNsub, precision);
      case 8: return UpdateAxesFast<8>(old_axes, inputJets, paraNsub, precision);
      case 9: return UpdateAxesFast<9>(old_axes, inputJets, paraNsub, precision);
      case 10: return UpdateAxesFast<10>(old_axes, inputJets, paraNsub, precision);
      case 11: return UpdateAxesFast<11>(old_axes, inputJets, paraNsub, precision);
      case 12: return UpdateAxesFast<12>(old_axes, inputJets, paraNsub, precision);
      case 13: return UpdateAxesFast<13>(old_axes, inputJets, paraNsub, precision);
      case 14: return UpdateAxesFast<14>(old_axes, inputJets, paraNsub, precision);
      case 15: return UpdateAxesFast<15>(old_axes, inputJets, paraNsub, precision);
      case 16: return UpdateAxesFast<16>(old_axes, inputJets, paraNsub, precision);
      case 17: return UpdateAxesFast<17>(old_axes, inputJets, paraNsub, precision);
      case 18: return UpdateAxesFast<18>(old_axes, inputJets, paraNsub, precision);
      case 19: return UpdateAxesFast<19>(old_axes, inputJets, paraNsub, precision);
      case 20: return UpdateAxesFast<20>(old_axes, inputJets, paraNsub, precision);
      default: std::cout << "N-jettiness is hard-coded to only allow up to 20 jets!" << std::endl;
         return std::vector<LightLikeAxis>();
   }

}


// Go from internal LightLikeAxis to PseudoJet
// TODO:  Make part of LightLikeAxis class.
std::vector<fastjet::PseudoJet> ConvertToPseudoJet(const std::vector <LightLikeAxis>& axes) {
   
   int n_jets = axes.size();
   
   double px, py, pz, E;
   std::vector<fastjet::PseudoJet> FourVecJets;
   for (int k = 0; k < n_jets; k++) {
      E = axes[k].mom();
      pz = (std::exp(2.0*axes[k].rap()) - 1.0) / (std::exp(2.0*axes[k].rap()) + 1.0) * E;
      px = std::cos(axes[k].phi()) * std::sqrt( std::pow(E,2) - std::pow(pz,2) );
      py = std::sin(axes[k].phi()) * std::sqrt( std::pow(E,2) - std::pow(pz,2) );
      fastjet::PseudoJet temp = fastjet::PseudoJet(px,py,pz,E);
      FourVecJets.push_back(temp);
   }
   return FourVecJets;
}


// Get minimization axes
std::vector<fastjet::PseudoJet> GetMinimumAxes(const std::vector <fastjet::PseudoJet> & seedAxes, const std::vector <fastjet::PseudoJet> & inputJets, KmeansParameters para, 
                                          NsubParameters paraNsub,MeasureFunctor* functor) {
   int n_jets = seedAxes.size();
   double noise = 0, tau = 10000.0, tau_tmp, cmp;
   std::vector< LightLikeAxis > new_axes(n_jets, LightLikeAxis(0,0,0,0)), old_axes(n_jets, LightLikeAxis(0,0,0,0));
   std::vector<fastjet::PseudoJet> tmp_min_axes, min_axes;
   for (int l = 0; l < para.n_iterations(); l++) { // Do minimization procedure multiple times
      // Add noise to guess for the axes
      for (int k = 0; k < n_jets; k++) {
         if ( 0 == l ) { // Don't add noise on first pass
            old_axes[k].set_rap( seedAxes[k].rap() );
            old_axes[k].set_phi( seedAxes[k].phi() );
         } else {
            noise = ((double)rand()/(double)RAND_MAX) * para.noise_range() * 2 - para.noise_range();
            old_axes[k].set_rap( seedAxes[k].rap() + noise );
            noise = ((double)rand()/(double)RAND_MAX) * para.noise_range() * 2 - para.noise_range();
            old_axes[k].set_phi( seedAxes[k].phi() + noise );
         }
      }
      cmp = 100.0; int h = 0;
      while (cmp > para.precision() && h < para.halt()) { // Keep updating axes until near-convergence or too many update steps
         cmp = 0.0; h++;
         new_axes = UpdateAxes(old_axes, inputJets, paraNsub, para.precision()); // Update axes
         for (int k = 0; k < n_jets; k++) {
            cmp += old_axes[k].Distance(new_axes[k]);
         }
         cmp = cmp / ((double) n_jets);
         old_axes = new_axes;
      }
      tmp_min_axes = ConvertToPseudoJet(old_axes); // Convert axes directions into four-std::vectors

      tau_tmp = functor->tau(inputJets, tmp_min_axes); 
      if (tau_tmp < tau) {tau = tau_tmp; min_axes = tmp_min_axes;} // Keep axes and tau only if they are best so far
   }	
   return min_axes;
}

///////
//
// Main Njettiness Class
//
///////

void Njettiness::establishTaus(const std::vector <fastjet::PseudoJet> & inputs) {
   //subTau values
   _currentTaus = _functor->subTaus(inputs, _currentAxes);
   
   //totalTau value
   _currentTau = 0.0;
   for (unsigned j = 0; j < _currentTaus.size(); j++) {_currentTau += _currentTaus[j];}
}



//Use NsubAxesMode to pick which type of axes to use
void Njettiness::establishAxes(unsigned int n_jets, const std::vector <fastjet::PseudoJet> & inputs) {
   _currentAxes = _axesFinder->getAxes(n_jets,inputs,_currentAxes);   
}


Njettiness::Njettiness(NsubGeometricParameters paraGeo) {
   double Rcutoff = paraGeo.Rcutoff();
   _functor = new GeometricMeasure(Rcutoff);
   _axesFinder = new AxesFinderFromGeometricMinimization(new AxesFinderFromKT(),Rcutoff);
}

//Constructor sets KmeansParameters from NsubAxesMode input
Njettiness::Njettiness(AxesMode axes, NsubParameters paraNsub) {

   _functor = new DefaultMeasure(paraNsub);  //Is there a way to do this without pointers?

   switch (axes) {
      case kt_axes:
         _axesFinder = new AxesFinderFromKT();
         break;
      case ca_axes:
         _axesFinder = new AxesFinderFromCA();
         break;
      case antikt_0p2_axes:
         _axesFinder = new AxesFinderFromAntiKT(0.2);      
         break;
      case onepass_kt_axes:
         _axesFinder = new AxesFinderFromKmeansMinimization(new AxesFinderFromKT(),KmeansParameters(1,0.0001,1000,0.8), paraNsub);      
         break;
      case onepass_ca_axes:
         _axesFinder = new AxesFinderFromKmeansMinimization(new AxesFinderFromCA(),KmeansParameters(1,0.0001,1000,0.8), paraNsub);
         break;
      case onepass_antikt_0p2_axes:
         _axesFinder = new AxesFinderFromKmeansMinimization(new AxesFinderFromAntiKT(0.2),KmeansParameters(1,0.0001,1000,0.8), paraNsub);
         break;
      case onepass_manual_axes:
         _axesFinder = new AxesFinderFromKmeansMinimization(new AxesFinderFromUserInput(),KmeansParameters(1,0.0001,1000,0.8), paraNsub);
         break;
      case min_axes:
         _axesFinder = new AxesFinderFromKmeansMinimization(new AxesFinderFromKT(),KmeansParameters(100,0.0001,1000,0.8), paraNsub);
         break;
      case manual_axes:
         _axesFinder = new AxesFinderFromUserInput();
         break;
      default:
         assert(false);
         break;
   }

}

Njettiness::~Njettiness() {
   delete _functor;
   delete _axesFinder;
}


// Partition a list of particles according to which N-jettiness axis they are closest to.
// Return a vector of length _currentAxes.size() (which should be N).
// Each vector element is a list of ints corresponding to the indices in
// particles of the particles belonging to that jet.
std::vector<std::list<int> > Njettiness::getPartition(const std::vector<fastjet::PseudoJet> & particles) {
   std::vector<std::list<int> > partitions(_currentAxes.size());

   int j_min = -1;
   for (unsigned i = 0; i < particles.size(); i++) {
      // find minimum distance
      double minR = 10000.0; // large number
      for (unsigned j = 0; j < _currentAxes.size(); j++) {
         double tempR = _functor->distance(particles[i],_currentAxes[j]); // delta R distance
         if (tempR < minR) {
            minR = tempR;
            j_min = j;
         }
      }
      if (_functor->doCluster(particles[i],_currentAxes[j_min])) partitions[j_min].push_back(i);
   }
   return partitions;
}


// Having found axes, assign each particle in particles to an axis, and return a set of jets.
// Each jet is the sum of particles closest to an axis (Njet = Naxes).
std::vector<fastjet::PseudoJet> Njettiness::getJets(const std::vector<fastjet::PseudoJet> & particles) {

  //-- SAK --------------------------------------------------------------------
  // Reworked to have proper history of constituents
#if 0
  std::vector<fastjet::PseudoJet> jets(_currentAxes.size());

  std::vector<std::list<int> > partition = getPartition(particles);
  for (unsigned j = 0; j < partition.size(); ++j) {
    std::list<int>::const_iterator it, itE;
    for (it = partition[j].begin(), itE = partition[j].end(); it != itE; ++it) {
      jets[j] += particles[*it];
    }
  }
#else
  std::vector<fastjet::PseudoJet> jets;
  jets.reserve(_currentAxes.size());

  std::vector<std::list<int> > partition = getPartition(particles);
  for (unsigned j = 0; j < partition.size(); ++j) {
    std::list<int>::const_iterator it, itE;
    std::vector<fastjet::PseudoJet>   constituents;
    for (it = partition[j].begin(), itE = partition[j].end(); it != itE; ++it)
      constituents.push_back(particles[*it]);
    jets.push_back(fastjet::join(constituents));
  }
#endif
  //---------------------------------------------------------------------------


  return jets;
}


//-- SAK ----------------------------------------------------------------------
void Njettiness::getJets(const std::vector<fastjet::PseudoJet> & particles, std::vector<fastjet::PseudoJet*>& outputJets)
{
  outputJets.reserve(_currentAxes.size());

  std::vector<std::list<int> >        partition = getPartition(particles);
  for (unsigned j = 0; j < partition.size(); ++j) {
    if (partition[j].empty())         continue;

    std::vector<fastjet::PseudoJet>   constituents;
    for (std::list<int>::const_iterator it = partition[j].begin(), itE = partition[j].end(); it != itE; ++it)
      constituents.push_back(particles[*it]);

    outputJets.push_back(new fastjet::PseudoJet( fastjet::join(constituents) ));
  }
}
//-----------------------------------------------------------------------------



FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

Nsubjettiness::Nsubjettiness(int N, Njettiness::AxesMode mode, double beta, double R0, double Rcutoff)
  : _N(N), _njettinessFinder(mode, NsubParameters(beta, R0, Rcutoff))
{}

Double32_t Nsubjettiness::result(const PseudoJet& jet) const
{
   std::vector<fastjet::PseudoJet> particles = jet.constituents();
   return _njettinessFinder.getTau(_N, particles);
}




const NjettinessExtras * njettiness_extras(const fastjet::PseudoJet& jet) {
   const ClusterSequence * myCS = jet.associated_cluster_sequence();   
   if (myCS == NULL) return NULL;
   const NjettinessExtras* extras = dynamic_cast<const NjettinessExtras*>(myCS->extras());   
   return extras;   
}

const NjettinessExtras * njettiness_extras(const fastjet::ClusterSequence& myCS) {
   const NjettinessExtras* extras = dynamic_cast<const NjettinessExtras*>(myCS.extras());   
   return extras;   
}

NjettinessPlugin::NjettinessPlugin(int N, Njettiness::AxesMode mode, double beta, double R0, double Rcutoff)
  : _N(N), _njettinessFinder(mode, NsubParameters(beta, R0, Rcutoff))
{}

NjettinessPlugin::NjettinessPlugin(int N, NsubGeometricParameters paraGeo)
  : _N(N), _njettinessFinder(paraGeo)
{}


std::string NjettinessPlugin::description() const {return "NJettiness";}

void NjettinessPlugin::run_clustering(ClusterSequence& cs) const
{
   std::vector<fastjet::PseudoJet> particles = cs.jets();
   _njettinessFinder.getTau(_N, particles);
   std::vector<std::list<int> > partition = _njettinessFinder.getPartition(particles);

   std::vector<fastjet::PseudoJet> jet_indices_for_extras;

   // output clusterings for each jet
   for (size_t i = 0; i < partition.size(); ++i) {
      std::list<int>& indices = partition[i];
      if (indices.size() == 0) continue;
      //std::list<int>::const_iterator it = indices.begin();
      while (indices.size() > 1) {
         int merge_i = indices.back(); indices.pop_back();
         int merge_j = indices.back(); indices.pop_back();
         int newIndex;
         double fakeDij = -1.0;
         
         // Store explicit combination, adding current Axis Information
//         fastjet::PseudoJet combination = cs.jets()[merge_i] + cs.jets()[merge_j];
//         cs.plugin_record_ij_recombination(merge_i, merge_j, fakeDij, combination, newIndex);
         cs.plugin_record_ij_recombination(merge_i, merge_j, fakeDij, newIndex);

         indices.push_back(newIndex);
      }
      double fakeDib = -1.0;
      
      int finalJet = indices.back();
      cs.plugin_record_iB_recombination(finalJet, fakeDib);
      jet_indices_for_extras.push_back(cs.jets()[finalJet]);  // Get the four vector for the final jets to compare later.
   }

   NjettinessExtras * extras = new NjettinessExtras(_njettinessFinder.currentTau(),jet_indices_for_extras,_njettinessFinder.currentTaus(),_njettinessFinder.currentAxes());
   cs.plugin_associate_extras(std::auto_ptr<ClusterSequence::Extras>(extras));
   
}

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh
