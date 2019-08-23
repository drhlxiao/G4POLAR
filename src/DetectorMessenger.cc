/**
 * @file: DetectorMessenger.cc
 * @class DetectorMessenger
 * @brief Implementation of DetectorMessenger, to control DetectorConstruction with macros
 *
 *
 * @Author  : Hualin Xiao
 * @Author  : Siwei Kong
 * @Date    : Jan., 2015
 * @Version : 1.10
 */


#include "globals.hh"

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* theDet)
: fDetector(theDet) { 
	
	fDetectorDir = new G4UIdirectory( "/polar/det/" );
	fDetectorDir->SetGuidance("Detector control.");

	fSetWorldFileCommand = new G4UIcmdWithAString("/polar/det/setWorldFile", this);
	fSetWorldFileCommand ->SetGuidance("Read world file with given name");
	fSetWorldFileCommand ->SetParameterName("WorldFileName", false);
	fSetWorldFileCommand ->SetDefaultValue("physWorldBox.gdml");
	fSetWorldFileCommand ->AvailableForStates(G4State_PreInit);

    	fSetVolumesCommand = new G4UIcmdWithAString("/polar/det/setVolumes", this);
	fSetVolumesCommand ->SetGuidance("Choose the mother volume: module, detector or spaceLab");
	fSetVolumesCommand ->SetParameterName("MotherVolName", false);
	fSetVolumesCommand ->SetDefaultValue("detector");
	fSetVolumesCommand ->AvailableForStates(G4State_PreInit);

  
}

DetectorMessenger::~DetectorMessenger()
{
	delete fSetWorldFileCommand;
    delete fSetVolumesCommand ;
	delete fDetectorDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if (command == fSetWorldFileCommand) { 
		
		fDetector->SetWorldFile(newValue);
	}
    	if ( command == fSetVolumesCommand ) {
		
		fDetector->SetVolumes(newValue);
	}

}
