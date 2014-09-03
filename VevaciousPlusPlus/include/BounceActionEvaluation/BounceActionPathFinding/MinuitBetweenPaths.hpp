/*
 * MinuitBetweenPaths.hpp
 *
 *  Created on: Sep 2, 2014
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#ifndef MINUITBETWEENPATHS_HPP_
#define MINUITBETWEENPATHS_HPP_

#include "CommonIncludes.hpp"
#include "Minuit2/FCNBase.h"
#include "PotentialMinimization/GradientBasedMinimization/MinuitMinimum.hpp"
#include "../BounceActionCalculator.hpp"
#include "../PathParameterization/TunnelPath.hpp"
#include "../PathParameterization/LinearSplineThroughNodes.hpp"

namespace VevaciousPlusPlus
{

  class MinuitBetweenPaths : public ROOT::Minuit2::FCNBase
  {
  public:
    MinuitBetweenPaths( std::vector< std::vector< double > > const& curvedPath,
                        double const pathTemperature,
                  BounceActionCalculator const* const bounceActionCalculator );
    virtual ~MinuitBetweenPaths();


    // This takes minuitParameters as a pair of weightings and creates a node
    // path of weighted averages of each node in curvedPath with the
    // corresponding node in straightPath, where the weighting for the
    // straightPath node is
    // minuitParameters[ 0 ] + ( minuitParameters[ 1 ] * the fraction along the
    // path of the node), and the weighting for the curvedPath node is
    // 1.0 - the other weighting, then it returns the bounce action along the
    // path from the composed nodes.
    virtual double
    operator()( std::vector< double > const& minuitParameters ) const;

    // This implements Up() for ROOT::Minuit2::FCNBase just to stick to a basic
    // value.
    virtual double Up() const { return 1.0; }

    // This calls Minuit2's MIGRAD function on this MinuitBetweenPaths object
    // (as a ROOT::Minuit2::FCNBase) and then returns the tunneling path
    // corresponding to the minimum found after movesPerImprovement function
    // calls.
    TunnelPath const* ImprovePath();

    // This sets up straightPath to go from curvedPath.front() to
    // curvedPath.back() in a straight line with as many nodes as curvedPath
    // has, as well as updating pathTemperature and currentMinuitTolerance.
    void UpdateNodes( double const pathTemperature );


  protected:
    std::vector< std::vector< double > > const& curvedPath;
    double pathTemperature;
    BounceActionCalculator const* const bounceActionCalculator;
    std::vector< std::vector< double > > straightPath;
    size_t numberOfFields;
    size_t numberOfSegments;
    unsigned int const minuitStrategy;
    size_t const movesPerImprovement;
    double currentMinuitTolerance;
    MinuitMinimum currentMinuitResult;


    // This takes minuitParameters as a pair of weightings and creates a node
    // path of weighted averages of each node in curvedPath with the
    // corresponding node in straightPath, where the weighting for the
    // straightPath node is
    // minuitParameters[ 0 ] + ( minuitParameters[ 1 ] * the fraction along the
    // path of the node), and the weighting for the curvedPath node is
    // 1.0 - the other weighting, then it returns the path of straight lines
    // between the composed nodes.
    TunnelPath const* PathForParameterization(
                         std::vector< double > const& minuitParameters ) const;
  };




  // This takes the tunneling path from minuitParameters according to
  // PathForParameterization( minuitParameters ) and returns the bounce action
  // along that path.
  inline double MinuitBetweenPaths::operator()(
                          std::vector< double > const& minuitParameters ) const
  {
    TunnelPath* composedPath( PathForParameterization( minuitParameters ) );
    double const bounceAction( (*bounceActionCalculator)( *composedPath ) );
    delete composedPath;
    return bounceAction;
  }

  // This calls Minuit2's MIGRAD function on this MinuitBetweenPaths object
  // (as a ROOT::Minuit2::FCNBase) and then returns the tunneling path
  // corresponding to the minimum found.
  inline TunnelPath const* MinuitBetweenPaths::ImprovePath()
  {
    ROOT::Minuit2::MnMigrad mnMigrad( (*this),
                                      currentMinuitResult.VariableValues(),
                                      currentMinuitResult.VariableErrors(),
                                      minuitStrategy );
    MinuitMinimum minuitMinimum( currentMinuitResult.VariableValues().size(),
                                 mnMigrad( movesPerImprovement,
                                           currentMinuitTolerance ) );
    currentMinuitResult = minuitMinimum;
    return PathForParameterization( minuitMinimum.VariableValues() );
  }

} /* namespace VevaciousPlusPlus */
#endif /* MINUITBETWEENPATHS_HPP_ */
