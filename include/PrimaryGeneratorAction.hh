//
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "TString.h"
#include "backgrounds.hh"

class G4Event;

class G4GeneralParticleSource;
class G4ParticleGun;
class G4ParticleTable;
class PrimaryGeneratorMessenger;

class GRBGenerator;

class TTree;
class TFile;
class t2sim;


//class PrimaryGeneratorMessenger;
class PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        virtual ~PrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event* event);


        void SetROOTDataFileName(G4String val);
        void SetSourcePosition(G4ThreeVector pos){sourcePosition=pos;}
        void SetParticleGunType(G4String type){gunType=type; }
		G4ThreeVector GetSourcePosition(){return sourcePosition;}

		
        //can be 
        //gps
        //root
        //na22
        //two 511 keV gamma

		G4int GetSimulatedPositrons(){return nPositrons;}


        void SetBkgDuration(G4double dur){bkgDuration=dur; InitBackgroundGun();}

        void GenerateANa22Event(G4Event *);
        void GenerateATwo511Event(G4Event *);
		void GenerateGRB(G4Event *anEvent);
        G4bool InitFile();
        void GetParticleInformation(G4ThreeVector &position, G4ThreeVector &direction, G4double &energy,G4ThreeVector &pol);
        void NextBkgEvent();
        void SetGunWithCurrentEntry(t2sim *tsim, G4ParticleGun* gun );

        void InitBackgroundGun();
		G4bool IsWithPositron(){return hasPositron;}


		GRBGenerator *GetGRBEventGun(){return gunGRB;}


    private:
        G4GeneralParticleSource*  fParticleSource;
        G4ParticleGun*  fParticleGun;
		GRBGenerator *gunGRB;
        //	PrimaryGeneratorMessenger* fParticleGunMessenger;

        G4String particleSourceFilename;
        G4int iEntry; //entry read
        G4int nEntries; //total entries

        G4String gunType;

		G4int nPositrons;

        TTree *fTree;
        TFile *fFile;
        t2sim *ts;
        G4ParticleTable *particleTable;
        G4ThreeVector sourcePosition;
        G4String rootDataFileName;
		G4bool hasPositron;

        G4double bkgDuration;
        backgrounds *bkg;

        PrimaryGeneratorMessenger *primMessenger;

};

#endif
