//
/// \file /src/Colour.cc
/// \brief Implementation of the Colour class
//

#include "Colour.hh"

Colour::Colour() {
	
	white = G4Colour(1.0, 1.0, 1.0);
	pink = G4Colour(0.94, 0.5, 0.5);
	grey = G4Colour(0.46, 0.53, 0.6);
	yellow = G4Colour(1.0, 1.0, 0.);
	blue = G4Colour(0.25,0.41, 0.88 );
	lightBlue = G4Colour(0.28, 0.82, 0.8);
	green = G4Colour(0., 1., 0.);
	brown = G4Colour(0.5, 0.5, 0.);
	purple = G4Colour(0.85,0.44,0.84);
	red = G4Colour(1.0, 0.0, 0.0);
	orange = G4Colour(1.,0.5,0.); 
	black =  G4Colour(0.,0.,0.); 
}

Colour::~Colour() { 

}

G4Colour Colour::GetColour(const G4String& colourName) {
	
	if (colourName == "pink") return pink;
	else if(colourName == "white") return white;
	else if (colourName == "grey") return grey;
	else if (colourName == "yellow") return yellow;
	else if (colourName == "blue") return blue;
	else if (colourName == "lightBlue") return lightBlue;
	else if (colourName == "green") return green;
	else if (colourName == "brown") return brown;
	else if (colourName == "purple") return purple;
	else if (colourName == "red") return red;
	else if (colourName == "orange") return orange;
	else if  (colourName == "black") return black; 
	else {
		G4cout<<"########## Notice: "<<colourName<<" does not exist !!! ##########"<< G4endl; 
		return white;
	}
}
