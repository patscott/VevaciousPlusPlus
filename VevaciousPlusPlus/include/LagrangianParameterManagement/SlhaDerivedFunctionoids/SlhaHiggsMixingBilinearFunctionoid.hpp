/*
 * SlhaHiggsMixingBilinearFunctionoid.hpp
 *
 *  Created on: Oct 28, 2015
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#ifndef SLHAHIGGSMIXINGBILINEARFUNCTIONOID_HPP_
#define SLHAHIGGSMIXINGBILINEARFUNCTIONOID_HPP_

#include "CommonIncludes.hpp"
#include "LagrangianParameterManagement/SlhaSourcedParameterFunctionoid.hpp"

namespace VevaciousPlusPlus
{

  class SlhaHiggsMixingBilinearFunctionoid :
                                         public SlhaSourcedParameterFunctionoid
  {
  public:
    SlhaHiggsMixingBilinearFunctionoid( size_t const indexInValuesVector,
            SlhaSourcedParameterFunctionoid const& treePseudoscalarMassSquared,
                              SlhaSourcedParameterFunctionoid const& tanBeta );
    virtual ~SlhaHiggsMixingBilinearFunctionoid();


    // This returns sin(beta) * cos(beta) * mA^2 evaluated at the scale given
    // through logarithmOfScale.
    virtual double operator()( double const logarithmOfScale ) const
    { return ( SinBetaCosBeta( tanBeta( logarithmOfScale ) )
               * treePseudoscalarMassSquared( logarithmOfScale ) ); }

    // This returns sin(beta) * cos(beta) * mA^2 evaluated from the given
    // parameters.
    virtual double operator()( double const logarithmOfScale,
                        std::vector< double > const& interpolatedValues ) const
    { return ( SinBetaCosBeta( interpolatedValues[ tanBetaIndex ] )
               * interpolatedValues[ treePseudoscalarMassSquaredIndex ] ); }

    // This is for creating a Python version of the potential.
    virtual std::string
    PythonParameterEvaluation( int const indentationSpaces ) const;

    // This is mainly for debugging.
    virtual std::string AsDebuggingString() const;


  protected:
    // This returns the value of sin(beta) * cos(beta) given the value of
    // tan(beta) without using possibly expensive trigonometric operations.
    static double SinBetaCosBeta( double const tanBeta )
    { return ( tanBeta / ( 1.0 + ( tanBeta * tanBeta ) ) ); }

    SlhaSourcedParameterFunctionoid const& treePseudoscalarMassSquared;
    size_t const treePseudoscalarMassSquaredIndex;
    SlhaSourcedParameterFunctionoid const& tanBeta;
    size_t const tanBetaIndex;
  };





  // This is for creating a Python version of the potential.
  inline std::string
  SlhaHiggsMixingBilinearFunctionoid::PythonParameterEvaluation(
                                            int const indentationSpaces ) const
  {
    std::stringstream stringBuilder;
    stringBuilder << PythonIndent( indentationSpaces )
    << "parameterValues[ " << IndexInValuesVector()
    << " ] = ( ( parameterValues[ " << treePseudoscalarMassSquaredIndex
    << " ] * parameterValues[ " << tanBetaIndex
    << " ] ) / ( 1.0 + ( parameterValues[ \" << tanBetaIndex << \" ] )**2 ) )";
    return stringBuilder.str();
  }

  // This is mainly for debugging.
  inline std::string
  SlhaHiggsMixingBilinearFunctionoid::AsDebuggingString() const
  {
    std::stringstream stringBuilder;
    stringBuilder
    << "IndexInValuesVector() = " << IndexInValuesVector() << std::endl;
    stringBuilder << "treePseudoscalarMassSquaredIndex = "
    << treePseudoscalarMassSquaredIndex << std::endl;
    stringBuilder << "treePseudoscalarMassSquared = "
    << treePseudoscalarMassSquared.AsDebuggingString() << std::endl;
    stringBuilder << "tanBetaIndex = " << tanBetaIndex << std::endl;
    stringBuilder << "tanBeta = " << tanBeta.AsDebuggingString() << std::endl;
    return stringBuilder.str();
  }

} /* namespace VevaciousPlusPlus */

#endif /* SLHAHIGGSMIXINGBILINEARFUNCTIONOID_HPP_ */