/**
 * @file: EventAction.cc
 * @class EventAction 
 * @brief PrimaryGeneratorAction support two kinds of particle guns: ParticleGun and GeneralParticleSource. 
 *        The first one allows to read particle information from a tree t2sim in a root file and 
 *        the second one allows to set particle information with geant4 macros
 *
 *
 * @Author  : Hualin Xiao
 * @Date    : Oct., 2015
 * @Version : 1.0
 * @history:
 *
 * Dec. 16, add na22 generator
 */

#include "TFile.h"
#include "TTree.h"
#include "t2sim.h"
#include "backgrounds.hh"

#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include <CLHEP/Random/RandFlat.h>
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "GRBGenerator.hh"
#include "SolarFlareGenerator.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction():
	hasPositron(false),    gunType("gps"),fTree(NULL),fFile(NULL),nEntries(0),iEntry(0),nPositrons(0)
{

	gunType="gps"; //default guntype
	bkgDuration=0;

	gunGRB=NULL;
	//gunGRB=new GRBGenerator();

	gunSolarFlare=new SolarFlareGenerator();
	primMessenger=new PrimaryGeneratorMessenger(this);
	//particle source
	G4int n_particle = 1;
	fParticleGun  = new G4ParticleGun(n_particle);     
	particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle
		= particleTable->FindParticle(particleName="gamma");
	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
	//fParticleGun->SetParticleEnergy(511.*keV);
	fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,15*cm));
	//particle source
	//
	fParticleSource  = new G4GeneralParticleSource();
	fParticleSource->SetParticleDefinition(particleTable->FindParticle("gamma"));
	//default particle

}
PrimaryGeneratorAction::~PrimaryGeneratorAction() 
{
	delete fParticleSource;
	delete fParticleGun;
	delete gunGRB;
	delete gunSolarFlare;
	if(bkg)delete bkg;
	if(ts)delete ts;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) 
{

	if(gunType=="root")
	{
		if(!InitFile())
		{
			G4UImanager* UImanager = G4UImanager::GetUIpointer();
			UImanager->ApplyCommand("/run/abort"); 
			return;
			//terminate the program in case of any problems
		}
		//generate multiple particles
		ts->GetEntry(iEntry);
		SetGunWithCurrentEntry(ts,fParticleGun);
		fParticleGun->GeneratePrimaryVertex(anEvent); 
		iEntry++;

	}
	else if(gunType=="na22")
	{
		GenerateANa22Event(anEvent);
	}
	else if(gunType=="GRB")
	{
		GenerateGRB(anEvent);
	}
	else if(gunType=="SolarFlare")
	{
		GenerateSolarFlareEvent(anEvent);
	}

	else if(gunType=="two511")
	{
		GenerateATwo511Event(anEvent);
	}
	else if(gunType=="bkg")
	{
		if(!bkg)InitBackgroundGun();
		bkg->GetEvent(fParticleGun);
		fParticleGun->GeneratePrimaryVertex(anEvent); 
		iEntry++;
		//generate a background event
	}
	else
	{
		fParticleSource->GeneratePrimaryVertex(anEvent); 
	}



}


G4bool PrimaryGeneratorAction::InitFile()
{
	if(ts!=NULL&&(iEntry<nEntries))return true;
	//do not initialize it again

	fFile =TFile::Open(rootDataFileName);
	if(!fFile)return false;
	fTree = (TTree*) fFile->Get("t2sim");
	if(!fTree)return false;
	ts=new t2sim(fTree);
	nEntries= fTree->GetEntries();
	G4cout<<"Number of events in the ROOT file:"<<nEntries<<G4endl;
	if(nEntries==0)return false;
	if(ts)return true;
	else
	{
		G4cout<<"can not load events"<<G4endl;
		return false;
	}
}
void PrimaryGeneratorAction::GetParticleInformation(G4ThreeVector &position, G4ThreeVector &direction, G4double &energy,G4ThreeVector &pol)
{
	if(gunType!="gps")
	{
		position=fParticleGun->GetParticlePosition(); 
		direction=fParticleGun->GetParticleMomentumDirection();
		energy=fParticleGun->GetParticleEnergy()/keV;
		if(gunType=="na22")energy=511+511.+1274.;
		if(gunType=="two511")energy=511+511.;
		pol=fParticleGun->GetParticlePolarization();
	}
	else
	{
		position=fParticleSource->GetParticlePosition(); 
		direction=fParticleSource->GetParticleMomentumDirection();
		energy=fParticleSource->GetParticleEnergy()/keV;
		pol=fParticleSource->GetParticlePolarization();
	}


}

void PrimaryGeneratorAction::GenerateANa22Event(G4Event *anEvent)
{
}
void PrimaryGeneratorAction::GenerateGRB(G4Event *anEvent)
{

	if(!gunGRB)
	{
		G4cout<<" GRB particle gun is not initialized"<<G4endl;
	}
	gunGRB->GeneratePrimaryVertex(anEvent, fParticleGun);
}
void PrimaryGeneratorAction::GenerateSolarFlareEvent(G4Event *anEvent)
{

	if(!gunSolarFlare)
	{
		G4cout<<" GRB particle gun is not initialized"<<G4endl;
	}
	gunSolarFlare->GeneratePrimaryVertex(anEvent, fParticleGun);
}



void PrimaryGeneratorAction::GenerateATwo511Event(G4Event *anEvent)
{

}
void PrimaryGeneratorAction::SetROOTDataFileName(G4String val)
{
	rootDataFileName=val; 
	gunType="root";
	InitFile();
}
void PrimaryGeneratorAction::SetGunWithCurrentEntry(t2sim *tsim, G4ParticleGun* partgun )
{
	G4String partiName(tsim->particle_name);
	partgun->SetParticleDefinition(particleTable->FindParticle(partiName));
	partgun->SetParticleEnergy(tsim->energy*keV);
	partgun->SetParticlePosition(G4ThreeVector(tsim->position[0]*cm, tsim->position[1]*cm, tsim->position[2]*cm));
	partgun->SetParticleMomentumDirection(G4ThreeVector(tsim->direction[0], tsim->direction[1], tsim->direction[2]));
	partgun->SetParticlePolarization(G4ThreeVector(tsim->polarization[0], tsim->polarization[1], tsim->polarization[2]));
}

void PrimaryGeneratorAction::InitBackgroundGun()
{
	bkg=new backgrounds();
	bkg->SetDuration(bkgDuration);
	bkg->InitParticleGun();
	G4int	fnBkgEvents=bkg->GetNumberOfParticles();
	G4cout<<"backgrounds duration: "<<bkgDuration<<G4endl;
	G4cout<<"Number of Backgrounds :"<<fnBkgEvents<<G4endl;

	G4RunManager::GetRunManager()->BeamOn(fnBkgEvents);
	//start a run
}
TF1 *PrimaryGeneratorAction::GetSpectrumModel()
{
	if(gunSolarFlare)
	{
		return gunSolarFlare->GetSpectrumModel();
	}
	else
		return NULL;
}
