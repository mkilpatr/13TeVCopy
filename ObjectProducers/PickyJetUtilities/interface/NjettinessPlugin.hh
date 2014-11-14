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


#ifndef __NJETTINESSPLUGIN_HH__
#define __NJETTINESSPLUGIN_HH__

#include "fastjet/ClusterSequence.hh"
#include "fastjet/JetDefinition.hh"

#include <string>
#include <climits>
#include "../interface/Njettiness.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

///Redoing in terms of ClusterSequence::Extras

class NjettinessExtras : public ClusterSequence::Extras {
   private:
   
      double _tau;
      std::vector<fastjet::PseudoJet> _jets;
      std::vector<double> _taus;
      std::vector<fastjet::PseudoJet> _axes;
      
      int labelOf(const fastjet::PseudoJet& jet) const {
         int thisJet = -1;
         for (unsigned int i = 0; i < _jets.size(); i++) {
            if (_jets[i].cluster_hist_index() == jet.cluster_hist_index()) {
               thisJet = i;
               break;
            }
         }
         return thisJet;
      }
      
   public:
      NjettinessExtras(double tau, std::vector<fastjet::PseudoJet> jets, std::vector<double> taus, std::vector<fastjet::PseudoJet> axes) : _tau(tau), _jets(jets), _taus(taus), _axes(axes) {}
      
      double totalTau() const {return _tau;}
      std::vector<double> subTaus() const {return _taus;}
      std::vector<fastjet::PseudoJet> jets() const {return _jets;}
      std::vector<fastjet::PseudoJet> axes() const {return _axes;}
      
      double totalTau(const fastjet::PseudoJet& jet) const {
         return _tau;
      }
      double subTau(const fastjet::PseudoJet& jet) const {
         if (labelOf(jet) == -1) return NAN;
         return _taus[labelOf(jet)];
      }
      
      double beamTau() const {  // need to implement
         return 0.0;
      }
      
      fastjet::PseudoJet axis(const fastjet::PseudoJet& jet) const {
         return _axes[labelOf(jet)];
      }

      bool has_njettiness_extras(const fastjet::PseudoJet& jet) const {
         return (labelOf(jet) >= 0);
      }

};



/*/// NjettinessInfo stores potentially relevant info found by the Njettiness jet finding algorithm
class NjettinessInfo : public fastjet::PseudoJet::UserInfoBase {
   private:
      double _subTau;
      double _totalTau;
      fastjet::PseudoJet _axis;
   public:
      NjettinessInfo(const double subTau, const double totalTau, const fastjet::PseudoJet & axis) : _subTau(subTau), _totalTau(totalTau), _axis(axis) {}
      double subTau() const { return _subTau;}
      double totalTau() const { return _totalTau;}
      fastjet::PseudoJet axis() const { return _axis;}
}; // set the info*/


/// The Njettiness jet algorithm
/**
 * An exclusive jet finder that identifies N jets; first N axes are found, then
 * particles are assigned to the nearest (DeltaR) axis and for each axis the
 * corresponding jet is simply the four-momentum sum of these particles.
 *
 * Axes can be found in several ways, specified by the AxesMode argument:
 *
 * kt_axes              : exclusive kT
 * ca_axes              : exclusive CA
 * min_axes             : minimize N-jettiness by iteration (100 passes default)
 * onepass_{kt,ca}_axes : one-pass minimization seeded by kt or CA (pretty good)
 *
 * N-jettiness is defined as:
 *
 * tau_N = Sum_{all particles i}
 *             p_T^i min((DR_i1/R_0)^beta, (DR_i2/R_0)^beta, ... , 1)
 *
 *   DR_ij is the distance sqrt(Delta_phi^2 + Delta_rap^2) between particle i
 *    and jet j.  R_0 and beta are parameters of the algorithm.  R_0 effectively
 *    defines an angular cutoff similar in effect to a cone-jet radius.
 *
 * You can also specify an angular cutoff Rcutoff.  Particles within R0 of an
 * axis affect tau_N, but only particles with Rcutoff are included in the final
 * jets.  In principal Rcutoff can be smaller of larger than R0.  Rcutoff=inf
 * corresponds to a partition of the event such that all particles are assigned
 * to jets.
 * 
 */

class NjettinessPlugin : public JetDefinition::Plugin {
public:

   NjettinessPlugin(int N, Njettiness::AxesMode mode, double beta, double R0, double Rcutoff=std::numeric_limits<double>::max());
   NjettinessPlugin(int N, NsubGeometricParameters);


   // The things that are required by base class.
   virtual std::string description () const;
   virtual double R() const {return -1.0;} // todo: make this not stupid
   virtual void run_clustering(ClusterSequence&) const;

   virtual ~NjettinessPlugin() {}

private:

   int _N;
   mutable Njettiness _njettinessFinder; // should muck with this so run_clustering can be const without this mutable

};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif  // __NJETTINESSPLUGIN_HH__
