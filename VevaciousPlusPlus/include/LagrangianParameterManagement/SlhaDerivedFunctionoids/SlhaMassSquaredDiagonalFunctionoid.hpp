/*
 * SlhaMassSquaredDiagonalFunctionoid.hpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#ifndef SLHAMASSSQUAREDDIAGONALFUNCTIONOID_HPP_
#define SLHAMASSSQUAREDDIAGONALFUNCTIONOID_HPP_

#include "CommonIncludes.hpp"
#include "LagrangianParameterManagement/SlhaSourcedParameterFunctionoid.hpp"

namespace VevaciousPlusPlus
{

  class SlhaMassSquaredDiagonalFunctionoid :
                                         public SlhaSourcedParameterFunctionoid
  {
  public:
    SlhaMassSquaredDiagonalFunctionoid( size_t const indexInValuesVector,
                                        size_t const squareMassIndex,
                                        size_t const linearMassIndex );
    virtual ~SlhaMassSquaredDiagonalFunctionoid();


    // This returns the mass-squared parameter evaluated at the scale given
    // through logarithmOfScale either from its direct value printed in the
    // MSL2, MSE2, MSQ2, MSD2, or MSU2 blocks, or by squaring the appropriate
    // value from the MSOFT block.
    virtual double operator()( double const logarithmOfScale,
                        std::vector< double > const& interpolatedValues ) const
    { return ( ( interpolatedValues[ squareMassIndex ] == 0.0 ) ?
               ( interpolatedValues[ linearMassIndex ]
                 * interpolatedValues[ linearMassIndex ] ) :
               interpolatedValues[ squareMassIndex ] ); }

    // This returns the mass-squared parameter evaluated at the scale given
    // through logarithmOfScale either from its direct value printed in the
    // MSL2, MSE2, MSQ2, MSD2, or MSU2 blocks, or by squaring the appropriate
    // value from the MSOFT block.
    double operator()( double const squareMass,
                       double const linearMass ) const
    { return ( ( squareMass != 0.0 ) ?
               squareMass :
               ( linearMass * linearMass ) ); }

    // This is for creating a Python version of the potential.
    virtual std::string
    PythonParameterEvaluation( int const indentationSpaces ) const;

    // This is mainly for debugging.
    virtual std::string AsDebuggingString() const;


  protected:
    size_t const squareMassIndex;
    size_t const linearMassIndex;
  };




  // This is for creating a Python version of the potential.
  inline std::string
  SlhaMassSquaredDiagonalFunctionoid::PythonParameterEvaluation(
                                            int const indentationSpaces ) const
  {
    std::stringstream stringBuilder;
    stringBuilder << std::setprecision( 12 )
    << PythonIndent( indentationSpaces )
    << "parameterValues[ " << IndexInValuesVector()
    << " ] = FirstIfNonzeroOtherwiseSecond( parameterValues[ "
    << squareMassIndex
    << " ], ( ( parameterValues[ " << linearMassIndex << " ] )**2 ) )";
    return stringBuilder.str();
  }

  // This is mainly for debugging.
  inline std::string
  SlhaMassSquaredDiagonalFunctionoid::AsDebuggingString() const
  {
    std::stringstream stringBuilder;
    stringBuilder
    << "IndexInValuesVector() = " << IndexInValuesVector() << std::endl;
    stringBuilder << "squareMassIndex = " << squareMassIndex << std::endl;
    stringBuilder << "linearMassIndex = " << linearMassIndex << std::endl;
    return stringBuilder.str();
  }

} /* namespace VevaciousPlusPlus */

#endif /* SLHAMASSSQUAREDDIAGONALFUNCTIONOID_HPP_ */
