//
/// \file /include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "globals.hh"

class Volume;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
	DetectorConstruction();
  	~DetectorConstruction();

    	G4VPhysicalVolume* Construct();

    	void SetWorldFile(G4String fileName);
     	void SetVolumes(G4String volumeName);
     	     
    	Volume* BuildVolume(G4String volumeName, G4String filePosition);
	void SetVisAttribute(Volume* pVolume, G4LogicalVolume* logicVolume);
	
    private:   
     	G4bool fIsModule;
     	G4bool fIsDetector;
     	G4bool fIsSpaceLab;
     	     
    	G4String fWorldFile;
    	G4VPhysicalVolume* fPhysWorld;
     	     
   	G4LogicalVolume* fLogicModule;
     	G4LogicalVolume* fLogicDetector;
     	G4LogicalVolume* fLogicSpaceLab;
     	     
     	Volume* fVolModule;
     	Volume* fVolDetector;
     	Volume* fVolSpaceLab;
  
    	DetectorMessenger* fDetectorMessenger;
     	     
     	G4bool fCheckOverlaps;
};

#endif
