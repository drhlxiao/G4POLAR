//
/// \file /include/Volume.hh
/// \brief Definition of the Volume class
//

#ifndef Volume_h
#define Volume_h 1

#include "globals.hh"
#include "G4LogicalVolume.hh"

class Volume {

    public:
  	  Volume();
	  Volume(G4String, G4LogicalVolume*, G4bool, G4bool, G4String, G4bool, G4double);
  	  ~Volume();

  	  G4String GetVolumeName() {return fVolumeName;}
	  G4LogicalVolume* GetLogicalVolume() {return fLogicalVolume;}
	  G4bool IsVisible() {return fIsVisible;}
	  G4bool IsWireframe() {return fIsWireframe;}
	  G4String GetVolumeColor() {return fColor;}
	  G4bool IsSensitive() {return fIsSensitive;}
	  G4double GetZPosition() {return fZPosition;}

	private:
	  G4String fVolumeName;
	  G4LogicalVolume* fLogicalVolume;
	  G4bool fIsVisible;
	  G4bool fIsWireframe;
	  G4String fColor;
	  G4bool fIsSensitive;
	  G4double fZPosition;
};

#endif
