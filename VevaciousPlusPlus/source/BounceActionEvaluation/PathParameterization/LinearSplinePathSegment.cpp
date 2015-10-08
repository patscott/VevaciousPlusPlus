/*
 * LinearSplinePathSegment.cpp
 *
 *  Created on: Jul 10, 2014
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#include "BounceActionEvaluation/PathParameterization/LinearSplinePathSegment.hpp"

namespace VevaciousPlusPlus
{

  LinearSplinePathSegment::LinearSplinePathSegment(
                                        std::vector< double > const& startNode,
                                          std::vector< double > const& endNode,
                                        double const segmentAuxiliaryLength ) :
    numberOfFields( startNode.size() ),
    fieldConstants( startNode ),
    fieldLinears( numberOfFields ),
    segmentAuxiliaryLength( segmentAuxiliaryLength )
  {
    for( size_t fieldIndex( 0 );
         fieldIndex < numberOfFields;
         ++fieldIndex )
    {
      fieldLinears[ fieldIndex ]
      = ( ( endNode[ fieldIndex ] - startNode[ fieldIndex ] )
          / segmentAuxiliaryLength );
    }
  }

  LinearSplinePathSegment::LinearSplinePathSegment(
                                  LinearSplinePathSegment const& copySource ) :
    numberOfFields( copySource.numberOfFields ),
    fieldConstants( copySource.fieldConstants ),
    fieldLinears( copySource.fieldLinears ),
    segmentAuxiliaryLength( copySource.segmentAuxiliaryLength )
  {
    // This constructor is just an initialization list.
  }

  LinearSplinePathSegment::LinearSplinePathSegment() :
    numberOfFields( 0 ),
    fieldConstants(),
    fieldLinears(),
    segmentAuxiliaryLength( -1.0 )
  {
    // This constructor is just an initialization list.
  }

  LinearSplinePathSegment::~LinearSplinePathSegment()
  {
    // This does nothing.
  }

} /* namespace VevaciousPlusPlus */