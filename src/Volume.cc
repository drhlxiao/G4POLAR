//
/// \file /src/Volume.cc
/// \brief Implementation of the Volume class
//

#include "Volume.hh" 

Volume::Volume()
: fVolumeName(""), fLogicalVolume(0), fIsVisible(false), 
fIsWireframe(false), fColor("White"), fIsSensitive(false), fZPosition(0.) {

}
	
Volume::Volume(G4String volumeName, G4LogicalVolume* logicalVolume, G4bool isVisible, 
                      G4bool isWireframe, G4String color, G4bool isSensitive, G4double zPosition)
: fVolumeName(volumeName), fLogicalVolume(logicalVolume), fIsVisible(isVisible), 
fIsWireframe(isWireframe), fColor(color), fIsSensitive(isSensitive), fZPosition(zPosition) {

}

Volume::~Volume() {

}
