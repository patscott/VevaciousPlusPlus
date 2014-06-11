/*
 * SplinePotential.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#include "../../include/TunnelingCalculation.hpp"

namespace VevaciousPlusPlus
{

  SplinePotential::SplinePotential(
                                   double const minimumFalseVacuumConcavity ) :
    auxiliaryValues(),
    potentialValues( 1,
                     0.0 ),
    firstDerivatives(),
    halfSecondDerivatives(),
    finalPotential( NAN ),
    halfFinalSecondDerivative( NAN ),
    finalQuarticCoefficient( NAN ),
    minimumFalseVacuumConcavity( minimumFalseVacuumConcavity ),
    definiteUndershootAuxiliary( NAN ),
    definiteOvershootAuxiliary( NAN ),
    auxiliaryUpToCurrentSegment( 0.0 )
  {
    // This constructor is just an initialization list.
  }

  SplinePotential::SplinePotential( SplinePotential const& copySource ) :
    auxiliaryValues( copySource.auxiliaryValues ),
    potentialValues( copySource.potentialValues ),
    firstDerivatives( copySource.firstDerivatives ),
    halfSecondDerivatives( copySource.halfSecondDerivatives ),
    finalPotential( copySource.finalPotential ),
    halfFinalSecondDerivative( copySource.halfFinalSecondDerivative ),
    finalQuarticCoefficient( copySource.finalQuarticCoefficient ),
    minimumFalseVacuumConcavity( copySource.minimumFalseVacuumConcavity ),
    definiteUndershootAuxiliary( copySource.definiteUndershootAuxiliary ),
    definiteOvershootAuxiliary( copySource.definiteOvershootAuxiliary ),
    auxiliaryUpToCurrentSegment( copySource.auxiliaryUpToCurrentSegment )
  {
    // This constructor is just an initialization list.
  }

  SplinePotential::~SplinePotential()
  {
    // This does nothing.
  }


  // This returns the value of the potential at auxiliaryValue, by finding the
  // correct segment and then returning its value at that point.
  double SplinePotential::operator()( double const auxiliaryValue ) const
  {
    if( auxiliaryValue <= 0.0 )
    {
      return potentialValues.front();
    }
    if( auxiliaryValue >= 1.0 )
    {
      return finalPotential;
    }
    size_t segmentIndex( 0 );
    double auxiliaryDifference( auxiliaryValue );
    while( segmentIndex < auxiliaryValues.size() )
    {
      if( auxiliaryDifference < auxiliaryValues[ segmentIndex ] )
      {
        return ( potentialValues[ segmentIndex ]
                 + ( ( firstDerivatives[ segmentIndex ]
                       + ( halfSecondDerivatives[ segmentIndex ]
                           * auxiliaryDifference ) )
                     * auxiliaryDifference ) );
      }
      auxiliaryDifference -= auxiliaryValues[ segmentIndex ];
      ++segmentIndex;
    }
    // If we get to here, we're beyond the last normal segment:
    // auxiliaryValues.back() < auxiliaryValue < 1.0.
    auxiliaryDifference = ( auxiliaryValue - 1.0 );
    return ( finalPotential
             + ( ( halfFinalSecondDerivative
                   + ( finalQuarticCoefficient
                       * auxiliaryDifference * auxiliaryDifference ) )
                 * ( auxiliaryDifference * auxiliaryDifference ) ) );
  }

  // This returns the value of the first derivative of the potential at
  // auxiliaryValue, by finding the correct segment and then returning its
  // slope at that point.
  double SplinePotential::FirstDerivative( double const auxiliaryValue ) const
  {
    if( ( auxiliaryValue <= 0.0 )
        ||
        ( auxiliaryValue >= 1.0 ) )
    {
      return 0.0;
    }
    size_t segmentIndex( 0 );
    double auxiliaryDifference( auxiliaryValue );
    while( segmentIndex < auxiliaryValues.size() )
    {
      if( auxiliaryDifference < auxiliaryValues[ segmentIndex ] )
      {
        return ( firstDerivatives[ segmentIndex ]
                 + ( 2.0 * halfSecondDerivatives[ segmentIndex ]
                         * auxiliaryDifference ) );
      }
      auxiliaryDifference -= auxiliaryValues[ segmentIndex ];
      ++segmentIndex;
    }
    // If we get to here, we're beyond the last normal segment:
    // auxiliaryValues[ currentGivenSize - 1 ] < auxiliaryValue < 1.0.
    auxiliaryDifference = ( auxiliaryValue - 1.0 );
    return ( ( ( 2.0 * halfFinalSecondDerivative )
               + ( 4.0 * finalQuarticCoefficient
                       * auxiliaryDifference * auxiliaryDifference ) )
             * auxiliaryDifference );
  }

  // This sets up the spline based on auxiliaryValues and potentialValues,
  // ensuring that the potential reaches the correct values for the vacua,
  // and that the potential derivative vanishes at the vacua. It also notes
  // the first point where the potential drops below that of the false vacuum
  // in definiteUndershootAuxiliary and the first maximum after that in
  // definiteOvershootAuxiliary.
  void
  SplinePotential::SetSpline( double const trueVacuumPotentialDifference )
  {
    firstDerivatives.resize( auxiliaryValues.size() );
    halfSecondDerivatives.resize( auxiliaryValues.size() );
    finalPotential = trueVacuumPotentialDifference;
    firstDerivatives[ 0 ] = 0.0;
    // There is a chance that numerical jitter means that the false vacuum may
    // be slightly ahead or behind the auxiliary variable p = 0. Either way we
    // flatten out the spline a bit (either forced concave by setting both
    // the slope to 0 and the second derivative to minimumFalseVacuumConcavity
    // if the proper false vacuum is at positive p, or just flattening it at
    // p = 0 if it should be at negative p, by setting the slope to 0 at
    // p = 0). It has the side-effect that potentials with energy barriers too
    // thin to be resolved by the path resolution are then forced to have a
    // small barrier at least.
    if( potentialValues[ 1 ] < 0.0 )
    {
      halfSecondDerivatives[ 0 ] = minimumFalseVacuumConcavity;
      potentialValues[ 0 ]
      = ( potentialValues[ 1 ]
          - ( minimumFalseVacuumConcavity
              * auxiliaryValues[ 0 ] * auxiliaryValues[ 0 ] ) );
      finalPotential += potentialValues[ 0 ];
      // The true vacuum is also lowered to ensure that tunneling is still
      // possible.
    }
    else
    {
      halfSecondDerivatives[ 0 ] = ( potentialValues[ 1 ]
                           / ( auxiliaryValues[ 0 ] * auxiliaryValues[ 0 ] ) );
    }
    bool definiteUndershootFound( false );
    bool definiteOvershootFound( false );
    auxiliaryUpToCurrentSegment = auxiliaryValues[ 0 ];
    for( size_t segmentIndex( 1 );
         segmentIndex < auxiliaryValues.size();
         ++segmentIndex )
    {
      firstDerivatives[ segmentIndex ]
      = ( firstDerivatives[ segmentIndex - 1 ]
          + ( 2.0 * auxiliaryValues[ segmentIndex - 1 ]
                  * halfSecondDerivatives[ segmentIndex - 1 ] ) );
      halfSecondDerivatives[ segmentIndex ]
      = ( ( potentialValues[ segmentIndex + 1 ]
            - potentialValues[ segmentIndex ]
            - ( firstDerivatives[ segmentIndex ]
                * auxiliaryValues[ segmentIndex ] ) )
        / ( auxiliaryValues[ segmentIndex ]
            * auxiliaryValues[ segmentIndex ] ) );

      // Now we check for the potential dropping below potentialValues[ 0 ] in
      // this segment.
      if( !definiteUndershootFound )
      {
        if( ( halfSecondDerivatives[ segmentIndex ] == 0.0 )
            &&
            ( potentialValues[ segmentIndex + 1 ] < potentialValues[ 0 ] ) )
        {
          definiteUndershootAuxiliary
          = ( auxiliaryUpToCurrentSegment
              + ( ( potentialValues[ 0 ] - potentialValues[ segmentIndex ] )
                  / firstDerivatives[ segmentIndex ] ) );
          definiteUndershootFound = true;
        }
        else
        {
          double const
          discriminantValue( ( firstDerivatives[ segmentIndex ]
                               * firstDerivatives[ segmentIndex ] )
                             - ( 4.0 * potentialValues[ segmentIndex ]
                                   * halfSecondDerivatives[ segmentIndex ] ) );
          if( discriminantValue >= 0.0 )
          {
            double const discriminantRoot( sqrt( discriminantValue ) );
            double
            crossingAuxiliary( ( -0.5 * ( firstDerivatives[ segmentIndex ]
                                          + discriminantRoot ) )
                               / ( halfSecondDerivatives[ segmentIndex ] ) );
            // We take the lower value for crossing to lower potential first,
            // but switch to the upper value if the lower value is negative
            // (and thus outside the segment).
            if( crossingAuxiliary < 0.0 )
            {
              crossingAuxiliary += ( discriminantRoot
                                 / ( halfSecondDerivatives[ segmentIndex ] ) );
            }
            // Then we check that crossingAuxiliary is within the segment.
            if( ( crossingAuxiliary >= 0.0 )
                &&
                ( crossingAuxiliary < auxiliaryValues[ segmentIndex ] ) )
            {
              definiteUndershootAuxiliary
              = ( auxiliaryUpToCurrentSegment + crossingAuxiliary );
              definiteUndershootFound = true;
            }
          }
        }
      }
      // End of checking for crossing the line where the potential equals its
      // value at the false vacuum.

      // Now we check to see if we are looking for the 1st minimum after the
      // definite undershoot point.
      if( definiteUndershootFound
          &&
          !definiteOvershootFound )
      {
        double const
        extremumAuxiliary( ( 0.5 * firstDerivatives[ segmentIndex ] )
                           / ( halfSecondDerivatives[ segmentIndex ] ) );
        if( ( extremumAuxiliary > std::max( 0.0,
                                            ( definiteUndershootAuxiliary
                                            - auxiliaryUpToCurrentSegment ) ) )
            &&
            ( extremumAuxiliary < auxiliaryValues[ segmentIndex ] ) )
        {
          definiteOvershootAuxiliary
          = ( auxiliaryUpToCurrentSegment + extremumAuxiliary );
          definiteOvershootFound = true;
        }
      }

      // Now we note the auxiliary value that starts the next segment.
      auxiliaryUpToCurrentSegment += auxiliaryValues[ segmentIndex ];
    }
    // The last element of potentialValues is already the value of the
    // potential at the end of the last normal segment, but
    // firstDerivatives.back() is only the slope at the start of the last
    // normal segment, so the second derivative must be added, multiplied by
    // the last element of auxiliaryValues.
    double finalDifference( auxiliaryUpToCurrentSegment - 1.0 );
    halfFinalSecondDerivative
    = ( 0.5 * ( ( 4.0 * ( potentialValues.back() - finalPotential ) )
                - ( ( firstDerivatives.back()
                      + ( 2.0 * halfSecondDerivatives.back()
                              * auxiliaryValues.back() ) )
                    * finalDifference ) ) );
    finalQuarticCoefficient
    = ( ( potentialValues.back() - finalPotential
          - halfFinalSecondDerivative )
        / ( finalDifference * finalDifference
            * finalDifference * finalDifference ) );
    halfFinalSecondDerivative = ( halfFinalSecondDerivative
                                   / ( finalDifference * finalDifference ) );

    // If the definite undershoot has not been found yet, both
    // definiteUndershootAuxiliary and definiteOvershootAuxiliary must be in
    // the (implicit) last segment.
    if( !definiteUndershootFound )
    {
      // If the potential hasn't dropped below the value of the false potential
      // in the other segments, we'll just leave the definite undershoot
      // auxiliary value as the start of the final segment.
      definiteUndershootAuxiliary = auxiliaryUpToCurrentSegment;
    }
    if( !definiteOvershootFound )
    {
      // The definite overshoot auxiliary depends on whether the minimization
      // condition can be satisfied for p != 1.
      double squareOfPossibleSolution( -halfFinalSecondDerivative
                                       / ( 2.0 * finalQuarticCoefficient ) );
      if( squareOfPossibleSolution > 0.0 )
      {
        definiteOvershootAuxiliary
        = ( 1.0 - sqrt( squareOfPossibleSolution ) );
      }
      else
      {
        definiteOvershootAuxiliary = 1.0;
      }
    }
  }

  // This is for debugging.
  std::string SplinePotential::AsDebuggingString() const
  {
    std::stringstream returnStream;
    double cumulativeAuxiliary( 0.0 );
    for( size_t segmentIndex( 0 );
         segmentIndex < auxiliaryValues.size();
         ++segmentIndex )
    {
      if( segmentIndex > 0 )
      {
        returnStream << " + ";
      }
      returnStream
      << "UnitStep[x - " << cumulativeAuxiliary << "] * ( ("
      << potentialValues[ segmentIndex ] << ") + (x-(" << cumulativeAuxiliary
      << ")) * (" << firstDerivatives[ segmentIndex ] << ") + (x-("
      << cumulativeAuxiliary << "))^2 * ("
      << halfSecondDerivatives[ segmentIndex ] << ") ) * UnitStep[";
      cumulativeAuxiliary += auxiliaryValues[ segmentIndex ];
      returnStream << cumulativeAuxiliary << " - x]" << std::endl;
    }
    returnStream
    << " + UnitStep[x - " << cumulativeAuxiliary << "] * ( (" << finalPotential
    << ") + (x-1)^2 * (" << halfFinalSecondDerivative << ") + (x-1)^4 * ("
    << finalQuarticCoefficient << ") ) * UnitStep[1 - x]";
    return returnStream.str();
  }

} /* namespace VevaciousPlusPlus */
