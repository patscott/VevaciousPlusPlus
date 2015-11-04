/*
 * PotentialFunction.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: Ben O'Leary (benjamin.oleary@gmail.com)
 */

#include "PotentialEvaluation/PotentialFunction.hpp"

namespace VevaciousPlusPlus
{

  PotentialFunction::PotentialFunction() :
    fieldNames(),
    numberOfFields( 0 ),
    dsbFieldInputStrings(),
    dsbFieldValueInputs()
  {
    // This constructor is just an initialization list.
  }

  PotentialFunction::PotentialFunction( PotentialFunction const& copySource ) :
    fieldNames( copySource.fieldNames ),
    numberOfFields( copySource.numberOfFields ),
    dsbFieldInputStrings( copySource.dsbFieldInputStrings ),
    dsbFieldValueInputs( copySource.dsbFieldValueInputs )
  {
    // This constructor is just an initialization list.
  }

  PotentialFunction::~PotentialFunction()
  {
    // This does nothing.
  }

} /* namespace VevaciousPlusPlus */
