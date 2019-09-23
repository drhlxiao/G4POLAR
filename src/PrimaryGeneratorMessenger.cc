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
#include "SolarFlareGenerator.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* thepg)
	: primAction(thepg) { 


		fDir=new G4UIdirectory("/polar/");

		setRootDataFromCmd= new G4UIcmdWithAString("/polar/readParticleFrom", this);
		setGunTypeCmd= new G4UIcmdWithAString("/polar/setGunType", this);
		setSourcePositionCmd= new G4UIcmdWith3VectorAndUnit("/polar/setSourcePosition", this);
		setBkgDurationCmd= new G4UIcmdWithADouble("/polar/setBkgDuration", this);

		setAlphaCmd=new G4UIcmdWithADouble("/polar/GRB/setAlpha",this);
		setBetaCmd=new G4UIcmdWithADouble("/polar/GRB/setBeta",this);

		setSolarFlareGammaCmd=new G4UIcmdWithADouble("/polar/SolarFlare/setGamma",this);
		setSolarFlareF35Cmd=new G4UIcmdWithADouble("/polar/SolarFlare/setF35",this);

		setSolarFlareSphRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setSphRad",this);
		setSolarFlareCircRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setCircRad",this);
		setSolarFlarePolFracCmd=new G4UIcmdWithADouble("/polar/SolarFlare/setPolFrac",this);
		setSolarFlarePolAngleCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setPolAng",this);
		setSolarFlareThetaCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setTheta",this);
		setSolarFlarePhiCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setPhi",this);
		setSolarFlareEminCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setEmin",this);
		setSolarFlareEmaxCmd=new G4UIcmdWithADoubleAndUnit("/polar/SolarFlare/setEmax",this);




		setEpeakCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEpeak",this);

		setEminCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEmin",this);
		setEmaxCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setEmax",this);

		setSphRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setSphRad",this);
		setCircRadCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setCircRad",this);
		setPolFracCmd=new G4UIcmdWithADouble("/polar/GRB/setPolFrac",this);
		setPolAngleCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setPolAng",this);
		setThetaCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setTheta",this);
		setPhiCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setPhi",this);

		setFluxCmd=new G4UIcmdWithADouble("/polar/GRB/setFlux",this);


		setFluxEminCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setFluxEmin",this);
		setFluxEmaxCmd=new G4UIcmdWithADoubleAndUnit("/polar/GRB/setFluxEmax",this);
		//Energy range of the flux

	}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
	delete setRootDataFromCmd;
	delete setSourcePositionCmd;
	delete setGunTypeCmd;
	delete fDir;
	delete setBkgDurationCmd;

	delete setSolarFlareGammaCmd;
	delete setSolarFlareF35Cmd;
	delete setSolarFlareSphRadCmd;
	delete setSolarFlareCircRadCmd;
	delete 	setSolarFlarePolFracCmd;
	delete 		setSolarFlarePolAngleCmd;
	delete 		setSolarFlareThetaCmd;
	delete 		setSolarFlarePhiCmd;
	delete setSolarFlareEminCmd;
	delete setSolarFlareEmaxCmd;



	delete  setFluxCmd;

	delete  setFluxEminCmd;
	delete  setFluxEmaxCmd;

	delete  setAlphaCmd;
	delete  setBetaCmd;
	delete  setEpeakCmd;
	delete  setEminCmd;
	delete  setEmaxCmd;
	delete  setSphRadCmd;
	delete  setCircRadCmd;
	delete  setPolFracCmd;
	delete  setPolAngleCmd;
	delete  setThetaCmd;
	delete  setPhiCmd;




}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if(command== setRootDataFromCmd){
		primAction->SetROOTDataFileName(newValue);
	}
	else if(command== setGunTypeCmd){
		primAction->SetParticleGunType(newValue);
	}
	else if(command==  setSolarFlareGammaCmd){ 
		primAction->GetSolarFlareEventGun()->SetGamma(setSolarFlareGammaCmd->GetNewDoubleValue(newValue));
	}
	else if(command==  setSolarFlareF35Cmd){ 
		primAction->GetSolarFlareEventGun()->SetF35(setSolarFlareF35Cmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlareSphRadCmd){ 
		primAction->GetSolarFlareEventGun()->SetSphRad(setSolarFlareSphRadCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlareCircRadCmd){ 
		G4cout<<"Setting CircRadiu: "<<newValue<<G4endl;
		primAction->GetSolarFlareEventGun()->SetCircRad(setSolarFlareCircRadCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlarePolFracCmd){
		primAction->GetSolarFlareEventGun()->SetPolFrac(setSolarFlarePolFracCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlarePolAngleCmd){
		primAction->GetSolarFlareEventGun()->SetPolAngle(setSolarFlarePolAngleCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlareThetaCmd){
		primAction->GetSolarFlareEventGun()->SetTheta(setSolarFlareThetaCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlarePhiCmd){
		primAction->GetSolarFlareEventGun()->SetPhi(setSolarFlarePhiCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlareEminCmd){ 
		primAction->GetSolarFlareEventGun()->SetEmin(setSolarFlareEminCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setSolarFlareEmaxCmd){ 
		primAction->GetSolarFlareEventGun()->SetEmax(setSolarFlareEmaxCmd->GetNewDoubleValue(newValue));
	}


	else if(command== setSourcePositionCmd){
		G4ThreeVector pos=setSourcePositionCmd->GetNew3VectorValue(newValue);
		primAction->SetSourcePosition(pos);
	}

	else if(command==  setBkgDurationCmd){ 
		primAction->SetBkgDuration(setBkgDurationCmd->GetNewDoubleValue(newValue));
	}

	else if(command== setAlphaCmd){ 
		primAction->GetGRBEventGun()->SetAlpha(setAlphaCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setBetaCmd){ 
		primAction->GetGRBEventGun()->SetBeta(setBetaCmd->GetNewDoubleValue(newValue));
	}

	else if(command== setEpeakCmd){ 
		primAction->GetGRBEventGun()->SetEpeak(setEpeakCmd->GetNewDoubleValue(newValue));
	}

	else if(command== setEminCmd){ 
		primAction->GetGRBEventGun()->SetEmin(setEminCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setEmaxCmd){ 
		primAction->GetGRBEventGun()->SetEmax(setEmaxCmd->GetNewDoubleValue(newValue));
	}


	else if(command== setSphRadCmd){ 
		primAction->GetGRBEventGun()->SetSphRad(setSphRadCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setCircRadCmd){ 
		primAction->GetGRBEventGun()->SetCircRad(setCircRadCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setPolFracCmd){
		primAction->GetGRBEventGun()->SetPolFrac(setPolFracCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setPolAngleCmd){
		primAction->GetGRBEventGun()->SetPolAngle(setPolAngleCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setThetaCmd){
		primAction->GetGRBEventGun()->SetTheta(setThetaCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setPhiCmd){
		primAction->GetGRBEventGun()->SetPhi(setPhiCmd->GetNewDoubleValue(newValue));
	}

	else if(command== setFluxCmd){
		primAction->GetGRBEventGun()->SetFlux(setFluxCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setFluxEminCmd){
		primAction->GetGRBEventGun()->SetFluxEmin(setFluxEminCmd->GetNewDoubleValue(newValue));
	}
	else if(command== setFluxEmaxCmd){
		primAction->GetGRBEventGun()->SetFluxEmax(setFluxEmaxCmd->GetNewDoubleValue(newValue));
	}







}
