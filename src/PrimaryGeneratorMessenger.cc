/**
 * @file: PrimaryGeneratorMessenger.cc
 * @class PrimaryGeneratorMessenger
 * @brief Implementation of PrimaryGeneratorMessenger, to control PrimaryGeneratorConstruction with macros
 *
 *
 * @Author  : Hualin Xiao
 * @Date    : Jan., 2015
 * @Version : 1.10
 */


#include "globals.hh"

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4ThreeVector.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "GRBGenerator.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* thepg)
	: fPrimAction(thepg) { 


		fDir=new G4UIdirectory("/polar/");

		fSetRootDataFromCmd= new G4UIcmdWithAString("/polar/readParticleFrom", this);
		fSetGunTypeCmd= new G4UIcmdWithAString("/polar/setGunType", this);
		fSetSourcePositionCmd= new G4UIcmdWith3VectorAndUnit("/polar/setSourcePosition", this);
		fSetBkgDurationCmd= new G4UIcmdWithADouble("/polar/setBkgDuration", this);

		fSetAlphaCmd=new G4UIcmdWithADouble("/polar/GRB/setAlpha",this);
		
		fSetBetaCmd=new G4UIcmdWithADouble("/polar/GRB/setBeta",this);




		fSetEpeakCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEpeak",this);
		fSetEminCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEmin",this);
		fSetEmaxCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEmax",this);
		fSetSphRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setSphRad",this);
		fSetCircRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setCircRad",this);
		fSetPolFracCmd=new G4UIcmdWithADouble("/polar/GRB/setPolFrac",this);
		fSetPolAngleCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setPolAng",this);
		fSetThetaCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setTheta",this);
		fSetPhiCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setPhi",this);
		fSetFluxCmd=new G4UIcmdWithADouble("/polar/GRB/setFlux",this);
		fSetFluxEminCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setFluxEmin",this);
		fSetFluxEmaxCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setFluxEmax",this);
		//Energy range of the flux

	}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
	delete fSetRootDataFromCmd;
	delete fSetSourcePositionCmd;
	delete fSetGunTypeCmd;
	delete fDir;
	delete fSetBkgDurationCmd;


	delete  fSetFluxCmd;

	delete  fSetFluxEminCmd;
	delete  fSetFluxEmaxCmd;

	delete  fSetAlphaCmd;
	delete  fSetBetaCmd;
	delete  fSetEpeakCmd;
	delete  fSetEminCmd;
	delete  fSetEmaxCmd;
	delete  fSetSphRadCmd;
	delete  fSetCircRadCmd;
	delete  fSetPolFracCmd;
	delete  fSetPolAngleCmd;
	delete  fSetThetaCmd;
	delete  fSetPhiCmd;




}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if (command == fSetRootDataFromCmd) { 
		fPrimAction->SetROOTDataFileName(newValue);
	}
	if (command == fSetGunTypeCmd) { 
		fPrimAction->SetParticleGunType(newValue);
	}

	if ( command == fSetSourcePositionCmd) {

		G4ThreeVector pos=fSetSourcePositionCmd->GetNew3VectorValue(newValue);
		fPrimAction->SetSourcePosition(pos);
	}
	if(command == fSetBkgDurationCmd)
	{
		fPrimAction->SetBkgDuration(fSetBkgDurationCmd->GetNewDoubleValue(newValue));
	}

	if(command==fSetAlphaCmd)fPrimAction->GetGRBEventGun()->SetAlpha(fSetAlphaCmd->GetNewDoubleValue(newValue));
	if(command==fSetBetaCmd)fPrimAction->GetGRBEventGun()->SetBeta(fSetBetaCmd->GetNewDoubleValue(newValue));

	if(command==fSetEpeakCmd)fPrimAction->GetGRBEventGun()->SetEpeak(fSetEpeakCmd->GetNewDoubleValue(newValue));
	if(command==fSetEminCmd)fPrimAction->GetGRBEventGun()->SetEmin(fSetEminCmd->GetNewDoubleValue(newValue));
	if(command==fSetEmaxCmd)fPrimAction->GetGRBEventGun()->SetEmax(fSetEmaxCmd->GetNewDoubleValue(newValue));
	if(command==fSetSphRadCmd)fPrimAction->GetGRBEventGun()->SetSphRad(fSetSphRadCmd->GetNewDoubleValue(newValue));
	if(command==fSetCircRadCmd)fPrimAction->GetGRBEventGun()->SetCircRad(fSetCircRadCmd->GetNewDoubleValue(newValue));
	if(command==fSetPolFracCmd)fPrimAction->GetGRBEventGun()->SetPolFrac(fSetPolFracCmd->GetNewDoubleValue(newValue));
	if(command==fSetPolAngleCmd)fPrimAction->GetGRBEventGun()->SetPolAngle(fSetPolAngleCmd->GetNewDoubleValue(newValue));
	if(command==fSetThetaCmd)fPrimAction->GetGRBEventGun()->SetTheta(fSetThetaCmd->GetNewDoubleValue(newValue));
	if(command==fSetPhiCmd)fPrimAction->GetGRBEventGun()->SetPhi(fSetPhiCmd->GetNewDoubleValue(newValue));
	if(command==fSetFluxCmd)fPrimAction->GetGRBEventGun()->SetFlux(fSetFluxCmd->GetNewDoubleValue(newValue));
	if(command==fSetFluxEminCmd)fPrimAction->GetGRBEventGun()->SetFluxEmin(fSetFluxEminCmd->GetNewDoubleValue(newValue));
	if(command==fSetFluxEmaxCmd)fPrimAction->GetGRBEventGun()->SetFluxEmax(fSetFluxEmaxCmd->GetNewDoubleValue(newValue));









}
