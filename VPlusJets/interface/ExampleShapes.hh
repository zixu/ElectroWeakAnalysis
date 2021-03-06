// $Id: ExampleShapes.hh 3001 2013-01-29 10:41:40Z soyez $
//
// Copyright (c) 2012-, Matteo Cacciari, Jihun Kim, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet contrib.
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#ifndef __FASTJET_CONTRIB_EXAMPLE_SHAPES_HH__
#define __FASTJET_CONTRIB_EXAMPLE_SHAPES_HH__

#include "fastjet/FunctionOfPseudoJet.hh"
#include "ShapeWithPartition.hh"
#include "ShapeWithComponents.hh"
#include "JetSubstructure/SubstructureTools/interface/JetSubstructureTools.h"

//FASTJET_BEGIN_NAMESPACE

//namespace contrib{

//------------------------------------------------------------------------
// a series of simple shapes:
//  - Pt
//  - ScalarPt
//  - Mt
//  - Mass
//  - MassSquare
//------------------------------------------------------------------------
/// the jet transverse momentum
class Pt : public fastjet::FunctionOfPseudoJet<double>{
public:
  virtual std::string description() const{return "jet pt";}
  virtual double result(const fastjet::PseudoJet &jet) const{ return jet.pt();}
};

/// the jet scalar transverse momentum
class ScalarPt : public fastjet::FunctionOfPseudoJet<double>{
public:
  virtual std::string description() const{return "jet scalar pt";}
  virtual double result(const fastjet::PseudoJet &jet) const;
};

/// the transverse mass 
class Mt : public fastjet::FunctionOfPseudoJet<double>{
public:
  virtual std::string description() const{return "jet transverse mass";}
  virtual double result(const fastjet::PseudoJet &jet) const{ return jet.mt();}
};

/// the jet mass
class Mass : public fastjet::FunctionOfPseudoJet<double>{
public:
  virtual std::string description() const{return "jet mass";}
  virtual double result(const fastjet::PseudoJet &jet) const{ return jet.m();}
};

/// the jet mass squared
class MassSquare : public fastjet::FunctionOfPseudoJet<double>{
public:
  virtual std::string description() const{return "jet mass squared";}
  virtual double result(const fastjet::PseudoJet &jet) const{ return jet.m2();}
};


//------------------------------------------------------------------------
/// the of the kt distance between the 2 kt subjets
class KtDij : public ShapeWithPartition{
public:
  virtual std::string description() const{return "kt distance between the 2 kt subjets";}
  virtual fastjet::PseudoJet partition(const fastjet::PseudoJet &jet) const;
  virtual double result_from_partition(const fastjet::PseudoJet &partit) const;
};

//------------------------------------------------------------------------
/// angularities
///
/// This is defined as 
/// \f[
///   \theta^{(\alpha)} = 1/{\tilde p_t} \sum_i p_{ti} \Delta R_{i,jet}^{2-\alpha}
/// \f]
class Angularity : public fastjet::FunctionOfPseudoJet<double>{
public:
  /// default ctor
  Angularity(double alpha=1.0) : _alpha(alpha){}

  /// description
  virtual std::string description() const;

  /// compute the function
  virtual double result(const fastjet::PseudoJet &jet) const;

protected:
  double _alpha;
};


/// angularities: numerator
///
/// Angularities are defined as
/// \f[
///   \theta^{(\alpha)} = 1/pttilde \sum_i p_{ti} \Delta R_{i,jet}^{2-\alpha}
/// \f]
/// this calss computes just the numerator
class AngularityNumerator : public fastjet::FunctionOfPseudoJet<double>{
public:
  /// default ctor
  AngularityNumerator(double alpha=1.0) : _alpha(alpha){}

  /// description
  virtual std::string description() const;

  /// compute the function
  virtual double result(const fastjet::PseudoJet &jet) const;

protected:
  double _alpha;
};


/// Energy-energy correlators
///
/// Defined as 
/// \f[
///   \tau_EEC = \sum_{i,j} p_ti p_tj \theta_{ij}^\beta / p_t^2
/// \f]
class TauEEC : public fastjet::FunctionOfPseudoJet<double>{
public:
  /// default ctor
  TauEEC(double i_beta=1.0) : _beta(i_beta){}

  /// description
  virtual std::string description() const;

  double beta() const {return _beta;}

  /// compute the function
  virtual double result(const fastjet::PseudoJet &jet) const;

protected:
  double _beta;
};


//------------------------------------------------------------------------
/// Energy-energy correlators: numerator
///
/// Defined as 
/// \f[
///   \tau_EEC = \sum_{i,j} p_ti p_tj \theta_{ij}^\beta / p_t^2
/// \f]
class TauEECNumerator : public ShapeWithPartition{
public:
  /// default ctor
  TauEECNumerator(double beta=1.0) : _beta(beta){}

  /// description
  virtual std::string description() const;

  /// compute the function
  virtual double result(const fastjet::PseudoJet &jet) const;

protected:
  double _beta;
};

// a quick coding of the "numerator" of N-subjettiness
class NSubjettinessNumerator : public ShapeWithPartition{
public:
  NSubjettinessNumerator(unsigned int N) : _N(N){}
  virtual std::string description() const{ return "N-subjettiness numerator";}

  /// compute the partition associated with a given jet
  virtual fastjet::PseudoJet partition(const fastjet::PseudoJet &jet) const;

  /// compute the shape from the partition
  virtual double result_from_partition(const fastjet::PseudoJet &partit) const;

protected:
  const unsigned int _N;
};

/*
class NSubjettinessRatio : public ShapeWithComponents{
	public:
		NSubjettinessRatio(int N) : _N(N){ assert(_N>1); }
		virtual std::string description() const{ return "N-subjettiness ratio from components";}

		virtual unsigned int n_components() const { return 2;}

		virtual std::vector<double> components(const fastjet::PseudoJet &jet) const{
			std::vector<double> comp(n_components());
			comp[0] = NSubjettinessNumerator(_N)(jet);
			comp[1] = NSubjettinessNumerator(_N-1)(jet);
			return comp;
		}

		virtual double result_from_components(const std::vector <double> &components) const{
			return components[0]/components[1];
		}

		virtual FunctionOfPseudoJet<double> * component_shape(unsigned int index) const{
			return new NSubjettinessNumerator(_N-index);
		}

	protected:
		const int _N;
};*/
class NSubjettinessRatio : public ShapeWithComponents{
	public:
		NSubjettinessRatio(int N, Njettiness::AxesMode in_mode, double in_beta, double in_R0, double in_Rcut) : _N(N), _mode(in_mode), _beta(in_beta), _R0(in_R0), _Rcut(in_Rcut) { assert(_N>1); }
		virtual std::string description() const{ return "N-subjettiness ratio from components";}

		virtual unsigned int n_components() const { return 2;}

		virtual std::vector<double> components(const fastjet::PseudoJet &jet) const{
			std::vector<double> comp(n_components());
			comp[0] = fastjet::Nsubjettiness(_N  , _mode, _beta, _R0, _Rcut)(jet);
			comp[1] = fastjet::Nsubjettiness(_N-1, _mode, _beta, _R0, _Rcut)(jet);

			return comp;
		}

		virtual double result_from_components(const std::vector <double> &components) const{
			return components[0]/components[1];
		}

		virtual FunctionOfPseudoJet<double> * component_shape(unsigned int index) const{
			return new fastjet::Nsubjettiness(_N-index, _mode, _beta, _R0, _Rcut);
		}

	protected:
		const int _N;
		const Njettiness::AxesMode _mode;
		const double _beta;
		const double _R0;
		const double _Rcut;

};

//} // namespace contrib

//FASTJET_END_NAMESPACE

#endif // __FASTJET_CONTRIB_EXAMPLE_SHAPES_HH__
