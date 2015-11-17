/*
 * SlhaTrilinearDiagonalFunctionoid.hpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#ifndef SLHATRILINEARDIAGONALFUNCTIONOID_HPP_
#define SLHATRILINEARDIAGONALFUNCTIONOID_HPP_

#include "CommonIncludes.hpp"
#include "LagrangianParameterManagement/SlhaSourcedParameterFunctionoid.hpp"

namespace VevaciousPlusPlus
{

  class SlhaTrilinearDiagonalFunctionoid :
                                         public SlhaSourcedParameterFunctionoid
  {
  public:
    SlhaTrilinearDiagonalFunctionoid( size_t const indexInValuesVector,
                        SlhaSourcedParameterFunctionoid const& directTrilinear,
                    SlhaSourcedParameterFunctionoid const& trilinearOverYukawa,
                    SlhaSourcedParameterFunctionoid const& appropriateYukawa );
    virtual ~SlhaTrilinearDiagonalFunctionoid();


    // This returns trilinear coupling evaluated at the scale given
    // through logarithmOfScale either from its direct value printed in the TL,
    // TE, TQ, TD, or TU blocks, or by multiplying the A-value with the
    // appropriate Yukawa coupling.
    virtual double operator()( double const logarithmOfScale ) const;

    // This returns trilinear coupling evaluated at the scale given
    // through logarithmOfScale either from its direct value printed in the TL,
    // TE, TQ, TD, or TU blocks, or by multiplying the A-value with the
    // appropriate Yukawa coupling.
    virtual double operator()( double const logarithmOfScale,
                        std::vector< double > const& interpolatedValues ) const
    { return ( ( interpolatedValues[ directTrilinearIndex ] == 0.0 ) ?
                   ( interpolatedValues[ trilinearOverYukawaIndex ]
                     * interpolatedValues[ appropriateYukawaIndex ] ):
                   interpolatedValues[ directTrilinearIndex ] ); }

    // This is for creating a Python version of the potential.
    virtual std::string
    PythonParameterEvaluation( int const indentationSpaces ) const;

    // This is mainly for debugging.
    virtual std::string AsDebuggingString() const;


  protected:
    SlhaSourcedParameterFunctionoid const& directTrilinear;
    size_t const directTrilinearIndex;
    SlhaSourcedParameterFunctionoid const& trilinearOverYukawa;
    size_t const trilinearOverYukawaIndex;
    SlhaSourcedParameterFunctionoid const& appropriateYukawa;
    size_t const appropriateYukawaIndex;
  };





  // This returns trilinear coupling evaluated at the scale given
  // through logarithmOfScale either from its direct value printed in the TL,
  // TE, TQ, TD, or TU blocks, or by multiplying the A-value with the
  // appropriate Yukawa coupling.
  inline double SlhaTrilinearDiagonalFunctionoid::operator()(
                                          double const logarithmOfScale ) const
  {
    double directValue( directTrilinear( logarithmOfScale ) );
    if( directValue == 0.0 )
    {
      return ( trilinearOverYukawa( logarithmOfScale )
               * appropriateYukawa( logarithmOfScale ) );
    }
    else
    {
      return directValue;
    }
  }

  // This is for creating a Python version of the potential.
  inline std::string
  SlhaTrilinearDiagonalFunctionoid::PythonParameterEvaluation(
                                            int const indentationSpaces ) const
  {
    std::stringstream stringBuilder;
    stringBuilder << std::setprecision( 12 )
    << PythonIndent( indentationSpaces )
    << "parameterValues[ " << IndexInValuesVector()
    << " ] = FirstIfNonzeroOtherwiseSecond( parameterValues[ "
    << directTrilinearIndex
    << " ], ( parameterValues[ " << trilinearOverYukawaIndex
    << " ] * parameterValues[ " << appropriateYukawaIndex << " ] ) )";
    return stringBuilder.str();
  }

  // This is mainly for debugging.
  inline std::string
  SlhaTrilinearDiagonalFunctionoid::AsDebuggingString() const
  {
    std::stringstream stringBuilder;
    stringBuilder
    << "IndexInValuesVector() = " << IndexInValuesVector() << std::endl;
    stringBuilder
    << "directTrilinearIndex = " << directTrilinearIndex << std::endl;
    stringBuilder << "directTrilinear = "
    << directTrilinear.AsDebuggingString() << std::endl;
    stringBuilder << "trilinearOverYukawaIndex = "
    << trilinearOverYukawaIndex << std::endl;
    stringBuilder << "trilinearOverYukawa = "
    << trilinearOverYukawa.AsDebuggingString() << std::endl;
    stringBuilder
    << "appropriateYukawaIndex = " << appropriateYukawaIndex << std::endl;
    stringBuilder << "appropriateYukawa = "
    << appropriateYukawa.AsDebuggingString() << std::endl;
    return stringBuilder.str();
  }

} /* namespace VevaciousPlusPlus */

#endif /* SLHATRILINEARDIAGONALFUNCTIONOID_HPP_ */