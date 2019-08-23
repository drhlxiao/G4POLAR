/**
 * @file: DetectorConstruction.cc
 * @brief Implementation of DetectorConstruction, use the gdml which was developed by Siwei Kong (kongsw@ihep.ac.cn)
 *
 *
 * @Author  : Hualin Xiao
 * @Author  : Siwei Kong
 * @Date    : Feb., 2015
 * @Version : 1.10
 */

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "Volume.hh"
#include "Colour.hh"

#include "globals.hh"
#include "G4GDMLParser.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


DetectorConstruction::DetectorConstruction()
	: fIsModule(false), fIsDetector(false), fIsSpaceLab(false), 
	fWorldFile("physWorldSphere.gdml"), fPhysWorld(0), fCheckOverlaps(false) {

		fDetectorMessenger = new DetectorMessenger(this);
	}

DetectorConstruction::~DetectorConstruction() {

	//delete fVolModule;
	//delete fVolDetector;
	//delete fVolSpaceLab;
	delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() 
{

	G4GDMLParser parser;
	parser.Read("gdml_polar/"+fWorldFile);
	fPhysWorld = parser.GetWorldVolume();

	fPhysWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);

	if(fIsModule) {
		// Define distance between neighbour bars
		G4double distanceBars = 6.08*mm;  // Check it when modify geometry

		G4cout<<"Setting single module"<<G4endl;
		// Set Single Module
		//
		fVolModule = BuildVolume("Module", "Module");
		fLogicModule = fVolModule->GetLogicalVolume();

		SetVisAttribute(fVolModule, fLogicModule);

		G4cout<<"Setting carbon fiber sokect"<<G4endl;
		// Set Carbon fiber Socket
		// 
		Volume* volCfSocket = BuildVolume("CfSocket", "Module");
		G4LogicalVolume* logicCfSocket = volCfSocket->GetLogicalVolume();

		SetVisAttribute(volCfSocket, logicCfSocket);

		G4ThreeVector vectCfSocket(0., 0., volCfSocket->GetZPosition()*mm);
		new G4PVPlacement (0, vectCfSocket, logicCfSocket, volCfSocket->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volCfSocket;

		G4cout<<"Setting back plate"<<G4endl;
		// Set Back Plate --- PK1000
		// 
		Volume* volBackPlate = BuildVolume("BackPlate", "Module");
		G4LogicalVolume* logicBackPlate = volBackPlate->GetLogicalVolume();

		SetVisAttribute(volBackPlate, logicBackPlate);

		G4ThreeVector vectBackPlate(0., 0., volBackPlate->GetZPosition()*mm);
		new G4PVPlacement (0, vectBackPlate, logicBackPlate, volBackPlate->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volBackPlate;	

		G4cout<<"Setting Back seal"<<G4endl;
		// Set Back Seal
		//
		Volume* volBackSeal = BuildVolume("BackSeal", "Module");
		G4LogicalVolume* logicBackSeal = volBackSeal->GetLogicalVolume();

		SetVisAttribute(volBackSeal, logicBackSeal);

		G4ThreeVector vectBackSeal(0., 0., volBackSeal->GetZPosition()*mm);
		new G4PVPlacement (0, vectBackSeal, logicBackSeal, volBackSeal->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volBackSeal;

		G4cout<<"Setting Top ESR"<<G4endl;
		// Set Top ESR (Enhanced Specular Reflector) --- Vikuiti
		//
		Volume* volESRTop = BuildVolume("ESRTop", "Module");
		G4LogicalVolume* logicESRTop = volESRTop->GetLogicalVolume();

		SetVisAttribute(volESRTop, logicESRTop);

		G4ThreeVector vectESRTop(0., 0., volESRTop->GetZPosition()*mm);
		new G4PVPlacement (0, vectESRTop, logicESRTop, volESRTop->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volESRTop;

		G4cout<<"Setting baffle without grids"<<G4endl;
		// Set Front Baffle Without Grids
		//
		Volume* volOuterFrontBaffle = BuildVolume("OuterFrontBaffle", "Module");
		G4LogicalVolume* logicOuterFrontBaffle = volOuterFrontBaffle->GetLogicalVolume();

		SetVisAttribute(volOuterFrontBaffle, logicOuterFrontBaffle);

		G4ThreeVector vectOuterFrontBaffle(0., 0., volOuterFrontBaffle->GetZPosition()*mm);
		new G4PVPlacement (0, vectOuterFrontBaffle, logicOuterFrontBaffle, volOuterFrontBaffle->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volOuterFrontBaffle;

		G4cout<<"Setting Grids in Front baffle bar ESR"<<G4endl;
		// Set Grids in Front Baffle
		//
		Volume* volGridFrontBaffle = BuildVolume("GridFrontBaffle", "Module");
		G4LogicalVolume* logicGridFrontBaffle = volGridFrontBaffle->GetLogicalVolume();

		SetVisAttribute(volGridFrontBaffle, logicGridFrontBaffle);

		for (G4int i=0; i<8; i++) {
			for (G4int j=0; j<8; j++) {

				G4int copyNb = 8*i+j;
				//G4double xGrid = -3.5*distanceBars + i*distanceBars;
				//G4double yGrid = -3.5*distanceBars + j*distanceBars;
				G4double xGrid = -3.5*distanceBars + j*distanceBars;
				G4double yGrid = -3.5*distanceBars + i*distanceBars;
				G4ThreeVector vectGridFrontBaffle(xGrid, yGrid, volGridFrontBaffle->GetZPosition()*mm);
				new G4PVPlacement (0, vectGridFrontBaffle, logicGridFrontBaffle, volGridFrontBaffle->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volGridFrontBaffle;

		G4cout<<"Setting Plastic Scintillator bar ESR"<<G4endl;
		// Set Plastic Scintillator Bar
		//
		Volume* volScintillator = BuildVolume("Scintillator", "Module");


		G4LogicalVolume *logScint=volScintillator->GetLogicalVolume();
		G4Material *matScin=logScint->GetMaterial();
		matScin->GetIonisation()->SetBirksConstant(0.08*mm/MeV);  
		G4cout<<"Setting Birks constant of scintillator"<<G4endl;



		G4LogicalVolume* logicScintillator = volScintillator->GetLogicalVolume();

		SetVisAttribute(volScintillator, logicScintillator);

		//G4cout<<"#Position of Scintillator Bars:"<<G4endl;
		for (G4int i=0; i<8; i++) {
			for (G4int j=0; j<8; j++) {

				G4int copyNb = 8*i+j;
				//G4double xBar = -3.5*distanceBars + i*distanceBars;
				//G4double yBar = -3.5*distanceBars + j*distanceBars;
				G4double xBar = -3.5*distanceBars + j*distanceBars;
				G4double yBar = -3.5*distanceBars + i*distanceBars;
				G4ThreeVector vectScintillator(xBar, yBar, volScintillator->GetZPosition()*mm);
				//G4cout<<i<<" "<<j<<" "<<xBar<<" "<<yBar<<" "<<volScintillator->GetZPosition()<<G4endl;
				new G4PVPlacement (0, vectScintillator, logicScintillator, volScintillator->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volScintillator;









		//the following lines can be commented 










		G4cout<<"Setting Surrounding ESR"<<G4endl;
		// Set Surrounding ESR (Enhanced Specular Reflector) --- Vikuiti
		//
		Volume* volESRSurrounding = BuildVolume("ESRSurrounding", "Module");
		G4LogicalVolume* logicESRSurrounding = volESRSurrounding->GetLogicalVolume();

		SetVisAttribute(volESRSurrounding, logicESRSurrounding);

		for (G4int i=0; i<2; i++) {
			for (G4int j=0; j<2; j++) {

				G4int copyNb = 2*i+j;
				G4double distanceESRSurrounding = 4.0*distanceBars;
				G4double xESRSurrounding = distanceESRSurrounding*(i-0.5) + distanceESRSurrounding*(j-0.5);
				G4double yESRSurrounding = -distanceESRSurrounding*(i-0.5) + distanceESRSurrounding*(j-0.5);
				G4ThreeVector vectESRSurrounding(xESRSurrounding, yESRSurrounding, volESRSurrounding->GetZPosition()*mm);
				G4RotationMatrix rotESRSurrounding(0., 0., 0.);
				if (i==j) rotESRSurrounding.rotateZ(pi/2.);
				G4Transform3D transESRSurrounding = G4Transform3D(rotESRSurrounding, vectESRSurrounding);
				new G4PVPlacement (transESRSurrounding, logicESRSurrounding, volESRSurrounding->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volESRSurrounding;

		// Set Inner ESR (Enhanced Specular Reflector) --- Vikuiti
		//
		G4cout<<"Setting Inner ESRF"<<G4endl;
		Volume* volESRInner = BuildVolume("ESRInner", "Module");
		G4LogicalVolume* logicESRInner = volESRInner->GetLogicalVolume();

		SetVisAttribute(volESRInner, logicESRInner);

		for (G4int i=0; i<8; i++) {
			for (G4int j=0; j<7; j++) {

				G4int copyNb = 7*i+j;
				G4double xESRInner = -3.5*distanceBars + j*distanceBars;
				G4double yESRInner = -3.0*distanceBars + i*distanceBars;
				G4ThreeVector vectESRInner(xESRInner, yESRInner, volESRInner->GetZPosition()*mm);
				G4RotationMatrix rotESRInner(0., 0., 0.);
				G4Transform3D transESRInner = G4Transform3D(rotESRInner, vectESRInner);
				new G4PVPlacement (transESRInner, logicESRInner, volESRInner->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}
		for (G4int i=0; i<7; i++) {
			for (G4int j=0; j<8; j++) {

				G4int copyNb = 8*i+j+56;
				G4double xESRInner = -3.0*distanceBars + j*distanceBars;
				G4double yESRInner = -3.5*distanceBars + i*distanceBars;
				G4ThreeVector vectESRInner(xESRInner, yESRInner, volESRInner->GetZPosition()*mm);
				G4RotationMatrix rotESRInner(0., 0., pi/2.);
				G4Transform3D transESRInner = G4Transform3D(rotESRInner, vectESRInner);
				new G4PVPlacement (transESRInner, logicESRInner, volESRInner->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volESRInner;

		// Set End Baffle Without Grids
		//
		Volume* volOuterEndBaffle = BuildVolume("OuterEndBaffle", "Module");
		G4LogicalVolume* logicOuterEndBaffle = volOuterEndBaffle->GetLogicalVolume();

		SetVisAttribute(volOuterEndBaffle, logicOuterEndBaffle);

		G4ThreeVector vectOuterEndBaffle(0., 0., volOuterEndBaffle->GetZPosition()*mm);
		new G4PVPlacement (0, vectOuterEndBaffle, logicOuterEndBaffle, volOuterEndBaffle->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volOuterEndBaffle;

		// Set Grids in End Baffle
		//
		Volume* volGridEndBaffle = BuildVolume("GridEndBaffle", "Module");
		G4LogicalVolume* logicGridEndBaffle = volGridEndBaffle->GetLogicalVolume();

		SetVisAttribute(volGridEndBaffle, logicGridEndBaffle);

		for (G4int i=0; i<8; i++) {
			for (G4int j=0; j<8; j++) {

				G4int copyNb = 8*i+j;
				G4double xGrid = -3.5*distanceBars + j*distanceBars;
				G4double yGrid = -3.5*distanceBars + i*distanceBars;
				G4ThreeVector vectGridEndBaffle(xGrid, yGrid, volGridEndBaffle->GetZPosition()*mm);
				new G4PVPlacement (0, vectGridEndBaffle, logicGridEndBaffle, volGridEndBaffle->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volGridEndBaffle;

		// Set Optical Coupling
		//
		Volume* volOpticalCoupling = BuildVolume("OpticalCoupling", "Module");
		G4LogicalVolume* logicOpticalCoupling = volOpticalCoupling->GetLogicalVolume();

		SetVisAttribute(volOpticalCoupling, logicOpticalCoupling);

		G4ThreeVector vectOpticalCoupling(0., 0., volOpticalCoupling->GetZPosition()*mm);
		new G4PVPlacement (0, vectOpticalCoupling, logicOpticalCoupling, volOpticalCoupling->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volOpticalCoupling;

		// Set PMT Glass
		//
		Volume* volPMTGlass = BuildVolume("PMTGlass", "Module");
		G4LogicalVolume* logicPMTGlass = volPMTGlass->GetLogicalVolume();

		SetVisAttribute(volPMTGlass, logicPMTGlass);

		G4ThreeVector vectPMTGlass(0., 0., volPMTGlass->GetZPosition()*mm);
		new G4PVPlacement (0, vectPMTGlass, logicPMTGlass, volPMTGlass->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volPMTGlass;

		// Set PMT Box
		//
		Volume* volPMTBox = BuildVolume("PMTBox", "Module");
		G4LogicalVolume* logicPMTBox = volPMTBox->GetLogicalVolume();

		SetVisAttribute(volPMTBox, logicPMTBox);

		G4ThreeVector vectPMTBox(0., 0., volPMTBox->GetZPosition()*mm);
		new G4PVPlacement (0, vectPMTBox, logicPMTBox, volPMTBox->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volPMTBox;

		// Set PMT Pin
		//
		Volume* volPMTPin = BuildVolume("PMTPin", "Module");
		G4LogicalVolume* logicPMTPin = volPMTPin->GetLogicalVolume();

		SetVisAttribute(volPMTPin, logicPMTPin);

		for (G4int i=0; i<8; i++) {
			for (G4int j=0; j<8; j++) {

				G4int copyNb = 8*i+j;
				G4double xPMTPin = -3.5*distanceBars + j*distanceBars;
				G4double yPMTPin = -3.5*distanceBars + i*distanceBars;
				G4ThreeVector vectPMTPin(xPMTPin, yPMTPin, volPMTPin->GetZPosition()*mm);
				new G4PVPlacement (0, vectPMTPin, logicPMTPin, volPMTPin->GetVolumeName(), fLogicModule, false, copyNb, fCheckOverlaps);
			}
		}

		delete volPMTPin;

		// Set FEE Board
		//
		Volume* volFEEBoard = BuildVolume("FEEBoard", "Module");
		G4LogicalVolume* logicFEEBoard = volFEEBoard->GetLogicalVolume();

		SetVisAttribute(volFEEBoard, logicFEEBoard);

		G4ThreeVector vectFEEBoard(0., 0., volFEEBoard->GetZPosition()*mm);
		new G4PVPlacement (0, vectFEEBoard, logicFEEBoard, volFEEBoard->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volFEEBoard;

		// Set FEE Parts
		//
		Volume* volFEEParts = BuildVolume("FEEParts", "Module");
		G4LogicalVolume* logicFEEParts = volFEEParts->GetLogicalVolume();

		SetVisAttribute(volFEEParts, logicFEEParts);

		G4ThreeVector vectFEEParts(0., -12.72, volFEEParts->GetZPosition()*mm);
		new G4PVPlacement (0, vectFEEParts, logicFEEParts, volFEEParts->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volFEEParts;

		// Set Side Plate
		//
		Volume* volSidePlate = BuildVolume("SidePlate", "Module");
		G4LogicalVolume* logicSidePlate = volSidePlate->GetLogicalVolume();

		SetVisAttribute(volSidePlate, logicSidePlate);

		G4ThreeVector vectSidePlate(0., 0., volSidePlate->GetZPosition()*mm);
		G4RotationMatrix rotSidePlate(0., 0., 0.);
		rotSidePlate.rotateY(-pi/2.);
		G4Transform3D transSidePlate = G4Transform3D(rotSidePlate, vectSidePlate);
		new G4PVPlacement (transSidePlate, logicSidePlate, volSidePlate->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volSidePlate;

		// Set PMT Seal
		//
		Volume* volPMTSeal = BuildVolume("PMTSeal", "Module");
		G4LogicalVolume* logicPMTSeal = volPMTSeal->GetLogicalVolume();

		SetVisAttribute(volPMTSeal, logicPMTSeal);

		G4ThreeVector vectPMTSeal(0., 0., volPMTSeal->GetZPosition()*mm);
		new G4PVPlacement (0, vectPMTSeal, logicPMTSeal, volPMTSeal->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volPMTSeal;

		// Set End Cap
		//
		Volume* volEndCap = BuildVolume("EndCap", "Module");
		G4LogicalVolume* logicEndCap = volEndCap->GetLogicalVolume();

		SetVisAttribute(volEndCap, logicEndCap);

		G4ThreeVector vectEndCap(0., 0., volEndCap->GetZPosition()*mm);
		new G4PVPlacement (0, vectEndCap, logicEndCap, volEndCap->GetVolumeName(), fLogicModule, false, 0, fCheckOverlaps);

		delete volEndCap;


		//		*/

		if(!fIsDetector) {
			// Place Single Module to World
			//
			G4ThreeVector vectModule(0., 0., fVolModule->GetZPosition()*mm);
			new G4PVPlacement (0, vectModule, fLogicModule, 
					fVolModule->GetVolumeName(), fPhysWorld->GetLogicalVolume(), false, 0, fCheckOverlaps);
		}
	}

	if(fIsDetector) {
		// Define distance between neighbour modules
		G4double distanceModules = 60.*mm;

		// Set Whole Detector
		//
		fVolDetector = BuildVolume("Detector", "Detector");
		fLogicDetector = fVolDetector->GetLogicalVolume();

		SetVisAttribute(fVolDetector, fLogicDetector);

		// Set Base Without Grids
		//
		Volume* volOuterBase = BuildVolume("OuterBase", "Detector");
		G4LogicalVolume* logicOuterBase = volOuterBase->GetLogicalVolume();

		SetVisAttribute(volOuterBase, logicOuterBase);

		G4ThreeVector vectOuterBase(0., 0., volOuterBase->GetZPosition()*mm);
		new G4PVPlacement (0, vectOuterBase, logicOuterBase, volOuterBase->GetVolumeName(), fLogicDetector, false, 0, fCheckOverlaps);

		delete volOuterBase;

		// Set Grids in Base
		//
		Volume* volGridBase = BuildVolume("GridBase", "Detector");
		G4LogicalVolume* logicGridBase = volGridBase->GetLogicalVolume();

		SetVisAttribute(volGridBase, logicGridBase);

		//G4cout<<"#Position of Modules:"<<G4endl;
		for (G4int i=0; i<5; i++) {
			for (G4int j=0; j<5; j++) {

				G4int copyNb = 5*i+j;
				//G4double xGrid = -2.*distanceModules + i*distanceModules;
				//G4double yGrid = -2.*distanceModules + j*distanceModules;
				G4double xGrid = -2.*distanceModules + j*distanceModules;
				G4double yGrid = -2.*distanceModules + i*distanceModules;
				G4ThreeVector vectGridBase(xGrid, yGrid, volGridBase->GetZPosition()*mm);
				new G4PVPlacement (0, vectGridBase, logicGridBase, volGridBase->GetVolumeName(), fLogicDetector, false, copyNb, fCheckOverlaps);
				G4cout<<"#==>"<<i<<" "<<j<<" "<<xGrid<<" "<<yGrid<<" "<<volGridBase->GetZPosition()<<G4endl;


			}
		}

		delete volGridBase;

		// Place Single Modules to Detector
		//
		for (G4int i=0; i<5; i++) {
			for (G4int j=0; j<5; j++) {

				G4int copyNb = 5*i+j;
				//G4double xModule = -2.*distanceModules + i*distanceModules;
				//G4double yModule = -2.*distanceModules + j*distanceModules;
				G4double xModule = -2.*distanceModules + j*distanceModules;
				G4double yModule = -2.*distanceModules + i*distanceModules; //xiaohl May25
				G4ThreeVector vectModule(xModule, yModule, fVolModule->GetZPosition()*mm);
				new G4PVPlacement (0, vectModule, fLogicModule, fVolModule->GetVolumeName(), fLogicDetector, false, copyNb, fCheckOverlaps);
			}
		}

		///*

		// Set Clamp
		//
		Volume* volClamp = BuildVolume("Clamp", "Detector");
		G4LogicalVolume* logicClamp = volClamp->GetLogicalVolume();

		SetVisAttribute(volClamp, logicClamp);

		G4ThreeVector vectClamp(-78., -158.5, volClamp->GetZPosition()*mm); // Check when modify the geometry
		new G4PVPlacement (0, vectClamp, logicClamp, volClamp->GetVolumeName(), fLogicDetector, false, 0, fCheckOverlaps);

		delete volClamp;

		// Set Carbon fiber Box
		//
		Volume* volCfBox = BuildVolume("CfBox", "Detector");
		G4LogicalVolume* logicCfBox = volCfBox->GetLogicalVolume();

		SetVisAttribute(volCfBox, logicCfBox);

		G4ThreeVector vectCfBox(0., 0., volCfBox->GetZPosition()*mm);
		new G4PVPlacement (0, vectCfBox, logicCfBox, volCfBox->GetVolumeName(), fLogicDetector, false, 0, fCheckOverlaps);

		delete volCfBox;

		// Set Screw
		//
		Volume* volScrew = BuildVolume("Screw", "Detector");
		G4LogicalVolume* logicScrew = volScrew->GetLogicalVolume();

		SetVisAttribute(volScrew, logicScrew);

		for (G4int i=0; i<5; i++) {
			for (G4int j=0; j<5; j++) {

				G4int copyNb = 5*i+j;
				G4double xScrew = -2.*distanceModules + i*distanceModules;
				G4double yScrew = -2.*distanceModules + j*distanceModules;
				G4ThreeVector vectScrew(xScrew, yScrew, volScrew->GetZPosition()*mm);
				new G4PVPlacement (0, vectScrew, logicScrew, volScrew->GetVolumeName(), fLogicDetector, false, copyNb, fCheckOverlaps);
			}
		}

		delete volScrew;

		// Set Thermal Coating
		//
		Volume* volThermalCoating = BuildVolume("ThermalCoating", "Detector");
		G4LogicalVolume* logicThermalCoating = volThermalCoating->GetLogicalVolume();

		SetVisAttribute(volThermalCoating, logicThermalCoating);

		G4ThreeVector vectThermalCoating(0., 0., volThermalCoating->GetZPosition()*mm);
		new G4PVPlacement (0, vectThermalCoating, logicThermalCoating, volThermalCoating->GetVolumeName(), fLogicDetector, false, 0, fCheckOverlaps);

		delete volThermalCoating;
		//	*/

		// Place Whole Detector to World
		//


		G4ThreeVector vectDetector(0., 0., fVolDetector->GetZPosition()*mm);

		if(!fIsSpaceLab) {
			new G4PVPlacement (0, vectDetector, fLogicDetector, fVolDetector->GetVolumeName(), fPhysWorld->GetLogicalVolume(), false, 0, fCheckOverlaps);
			//place into world
		}
		else if(fIsSpaceLab) 
		{
			//placed into spacelab
			// Set Space Lab
			//
			fVolSpaceLab = BuildVolume("TG2Assembly", "SpaceLab");

			fLogicSpaceLab = fVolSpaceLab->GetLogicalVolume();

			SetVisAttribute(fVolSpaceLab, fLogicSpaceLab);

			//Rotations from FreeCAD
			//Placement: -585, 3110, 2704
			//Angle: 126.51
			//Axies:  0.61, -0.61 -0.5
			//or Euler angles:
			//Yaw=-90 degree
			//pitch=0 deg
			//Roll=101 deg
			//Roll=phi
			//Pitch=theta
			//yaw=PSI

			//placed into spacelab

			new G4PVPlacement (0, vectDetector, fLogicDetector, fVolDetector->GetVolumeName(), fLogicSpaceLab, false, 0, fCheckOverlaps);

	//		G4ThreeVector vectSpaceLab(3109.*mm, 2700*mm,-585*mm);
	//		G4double angle=101*pi/180.;
	//		G4RotationMatrix rotatSpaceLab(-pi/2.,0, angle);
			//error ~ 2cm
	//		G4Transform3D transSpaceLab = G4Transform3D(rotatSpaceLab, vectSpaceLab);
	//https://www-zeuthen.desy.de/ILC/geant4/clhep-2.0.4.3/classCLHEP_1_1HepRotation.html
	//HepRotation & 	set (double phi, double theta, double psi)


			new G4PVPlacement (0, G4ThreeVector(0,0,0), fLogicSpaceLab, fVolSpaceLab->GetVolumeName(), fPhysWorld->GetLogicalVolume(), false, 0, fCheckOverlaps);			
			//spacelab placed into world volume
		}
	}
	G4cout<<"Detector Construction is finished"<<G4endl;

	return fPhysWorld;
}

void DetectorConstruction::SetWorldFile(G4String fileName) {

	fWorldFile = fileName;
}

void DetectorConstruction::SetVolumes(G4String volumeName) {

	if (volumeName == "module") fIsModule = true;

	if (volumeName == "detector") {
		if (!fIsModule) {
			G4cout<<"Error: Must set single module first !!!"<<G4endl;
			exit(0);
		}
		fIsDetector = true;
	}

	if (volumeName == "spaceLab") {
		if (!fIsDetector) {
			G4cout<<"Error: Must set whole detector first !!!"<<G4endl;
			exit(0);
		}
		fIsSpaceLab = true;
	}
}

Volume* DetectorConstruction::BuildVolume(G4String volumeName, G4String filePosition) {

	G4String fileName;	
	if (filePosition == "Module") {
		fileName = "gdml_polar/Module/logic"+volumeName+".gdml";
	} else if (filePosition == "Detector") {
		fileName = "gdml_polar/Detector/logic"+volumeName+".gdml";
	} else if (filePosition == "SpaceLab") {
		fileName = "gdml_polar/TG2/"+volumeName+".gdml";
	} else {
		G4cout<<"Error: Wrong file position !!!"<<G4endl;
		exit(0);
	}

	G4GDMLParser parser;
	parser.Read(fileName);
	G4LogicalVolume* logicalVolume = parser.GetVolume("logic"+volumeName);

	G4bool isVisible = false;
	G4bool isWireframe = false;
	G4String color;
	G4bool isSensitive = false;
	G4double zPosition;

	const G4GDMLAuxMapType* auxMap = parser.GetAuxMap();
	for (G4GDMLAuxMapType::const_iterator iter=auxMap->begin(); iter!=auxMap->end(); iter++) {

		G4cout<<"Volume "<<((*iter).first)->GetName()<<" has the following list of auxiliary information:"<<G4endl;
		for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin(); vit!=(*iter).second.end(); vit++) {

			G4cout<<"---> Type: "<<(*vit).type<<"	Value: "<<(*vit).value<<G4endl;

			if (((*vit).type == "isVisible")&&((*vit).value == "yes")) isVisible = true;
			if (((*vit).type == "isWireframe")&&((*vit).value == "yes")) isWireframe = true;
			if (((*vit).type == "isSensitive")&&((*vit).value == "yes")) isSensitive = true;
			if ((*vit).type == "color") color = (*vit).value;
			if ((*vit).type == "zPosition") sscanf((*vit).value,"%lf",&zPosition);
		}
	}

	//G4cout<<"~~~ The MASS of "<<volumeName<<" is "<<logicalVolume->GetMass()/g<<" g. ~~~"<<G4endl;

	Volume* result = new Volume(volumeName, logicalVolume, isVisible, isWireframe, color, isSensitive, zPosition);
	return result;
}

void DetectorConstruction::SetVisAttribute(Volume* pVolume, G4LogicalVolume* logicVolume) {

	Colour* pColour = new Colour();
	G4Colour colour = pColour->GetColour(pVolume->GetVolumeColor());
	G4VisAttributes* visAtt = new G4VisAttributes(colour);
	if (pVolume->IsWireframe()) {
		visAtt->SetForceWireframe(true);
	} else {
		visAtt->SetForceSolid(true);
	}
	visAtt->SetVisibility(pVolume->IsVisible());
	logicVolume->SetVisAttributes(visAtt);
}
