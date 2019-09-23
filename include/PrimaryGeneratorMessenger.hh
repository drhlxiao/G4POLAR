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
     	     PrimaryGeneratorAction* primAction;
     	     G4UIcmdWithAString*        setRootDataFromCmd;
     	     G4UIcmdWithAString*        setGunTypeCmd;
     	     G4UIcmdWithADouble*        setBkgDurationCmd;
             G4UIcmdWith3VectorAndUnit* setSourcePositionCmd;
             G4UIdirectory *fDir;
			 
			 G4UIcmdWithADouble  *setAlphaCmd;
			 G4UIcmdWithADouble  *setBetaCmd;

			 G4UIcmdWithADouble  *setFluxCmd;
			 G4UIcmdWithADoubleAndUnit  *setFluxEminCmd;
			 G4UIcmdWithADoubleAndUnit  *setFluxEmaxCmd;
			 G4UIcmdWithADoubleAndUnit  *setEpeakCmd;
			 G4UIcmdWithADoubleAndUnit  *setEminCmd;
			 G4UIcmdWithADoubleAndUnit  *setEmaxCmd;
			 G4UIcmdWithADoubleAndUnit  *setSphRadCmd;
			 G4UIcmdWithADoubleAndUnit  *setCircRadCmd;
			 G4UIcmdWithADouble *setPolFracCmd;
			 G4UIcmdWithADoubleAndUnit  *setPolAngleCmd;
			 G4UIcmdWithADoubleAndUnit  *setThetaCmd;
			 G4UIcmdWithADoubleAndUnit  *setPhiCmd;

		G4UIcmdWithADouble* setSolarFlareGammaCmd;
		G4UIcmdWithADouble* setSolarFlareF35Cmd;
		G4UIcmdWithADoubleAndUnit* setSolarFlareSphRadCmd;
		G4UIcmdWithADoubleAndUnit* setSolarFlareCircRadCmd;
		G4UIcmdWithADouble* 	setSolarFlarePolFracCmd;
		G4UIcmdWithADoubleAndUnit* 		setSolarFlarePolAngleCmd;
		G4UIcmdWithADoubleAndUnit* 		setSolarFlareThetaCmd;
		G4UIcmdWithADoubleAndUnit* 		setSolarFlarePhiCmd;
				 
		G4UIcmdWithADoubleAndUnit *setSolarFlareEminCmd;
		G4UIcmdWithADoubleAndUnit *setSolarFlareEmaxCmd;

				 
 

	


};

#endif
