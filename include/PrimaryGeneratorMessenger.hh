//
/// \file /include/PrimaryGeneratorMessenger.hh
/// \brief Definition of the PrimaryGeneratorMessenger class
//

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;

class PrimaryGeneratorMessenger: public G4UImessenger 
{
     public:
     	     PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
     	     ~PrimaryGeneratorMessenger();
    
     	     void SetNewValue( G4UIcommand*, G4String );
     	     
     private:
     	     PrimaryGeneratorAction* fPrimAction;
     	     G4UIcmdWithAString*        fSetRootDataFromCmd;
     	     G4UIcmdWithAString*        fSetGunTypeCmd;
     	     G4UIcmdWithADouble*        fSetBkgDurationCmd;
             G4UIcmdWith3VectorAndUnit* fSetSourcePositionCmd;
             G4UIdirectory *fDir;
			 
			 G4UIcmdWithADouble  *fSetAlphaCmd;
			 G4UIcmdWithADouble  *fSetBetaCmd;

			 G4UIcmdWithADouble  *fSetFluxCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetFluxEminCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetFluxEmaxCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetEpeakCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetEminCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetEmaxCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetSphRadCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetCircRadCmd;
			 G4UIcmdWithADouble *fSetPolFracCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetPolAngleCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetThetaCmd;
			 G4UIcmdWithADoubleAndUnit  *fSetPhiCmd;

			 

			 
 

	


};

#endif
