//
/// \file /include/Colour.hh
/// \brief Definition of the Colour class
//

#ifndef Colour_h
#define Colour_h 1

#include "globals.hh"
#include "G4Colour.hh"

class Colour
{ 
   public:
	Colour();
	~Colour();
	G4Colour GetColour(const G4String&);

   private:
	G4Colour white;
  	G4Colour pink;
   	G4Colour grey;
   	G4Colour yellow;
   	G4Colour blue;
   	G4Colour lightBlue;
  	G4Colour green;
   	G4Colour brown;
  	G4Colour purple;
   	G4Colour red;
   	G4Colour orange;
   	G4Colour black;
};
#endif
