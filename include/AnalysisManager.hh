//
/// \file AnalysisManager.hh
/// \brief Definition of the AnalysisManager class

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

#include "globals.hh"

//#include "G4Step.hh"
//#include "G4Event.hh"
//#include "G4Run.hh"
#include "TString.h"
#include <string>
class G4Run;
class G4Event;
class G4Step;

class TCanvas;
class TH1F;
class TFile;
class TTree;
class t2polar;
class TH2F;

class G4EmSaturation;
class PrimaryGeneratorAction;
class G4RunManager;
class AnalysisMessenger;
class DetResponse;
class GRBGenerator;
class SolarFlareGenerator;

class AnalysisManager
{
    public:
        static AnalysisManager* GetInstance();
        static void Dispose();

        void CreateFile();
        void CreateTree();
        void WriteFile();

        void SetEventID(G4int eventid){eventID=eventid;}
        void SetOutputFileName(TString filen){fOutputFile=filen;}

        void FillData(G4double total[25][64], G4double  vis[25][64], G4double coll[25][64]);

		void SetStoreSingleBarEvent(int opt){fStoreSingleBarEvent=opt;}




        void BeginOfEventAction(const G4Event *event);
        void EndOfEventAction(const G4Event *event);
        void SteppingAction(const G4Step *aStep);
        void BeginOfRunAction(const G4Run *);
        void EndOfRunAction(const G4Run *);


        void AddTotalEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep);
        void AddVisibleEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep);
        void AddCollectedEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep);
        void SetThreshold(G4double val){fThreshold=val;}
        void SetModulationCurveThreshold(G4double val){fModulationCurveThreshold=val;}
        void SetCalibrationFile(G4String fs){fCalibrationFile=fs;}


        void SetRealSim(G4int val){fRealSim=val;}

        void SetkB(G4double val){fkB=val;}

        void WriteCommand(G4String fname);
        //write macro file to 
        void WriteConfigurations();
        //write settings to the file

        AnalysisManager();
        ~AnalysisManager();


        void SetPrescalingSingle(int pres){fPresSingle=pres;}
        void SetPrescalingCosmic(int prec){fPresCosmic=prec;}
	//	void Coincidence(PrimaryGeneratorAction *pri, t2polar *tp);

	//	void SetDoCoincidence(G4int co){doCoincidence=co;}
	
//		void SetPolNbins(G4int v){fPolNbins=v;}
//		void SetPhiNbins(G4int v){fPhiNbins=v;}

//		void LoopPolPhi();
//		void CreateHMC();
		

    private:
        G4int fPresSingle;
        G4int fPresCosmic;
		std::string summary;


		G4int fPolNbins;
		G4int fPhiNbins;

        G4int fNumSingle;
        G4int fNumCosmic;

		G4int fRealSim; 
        static AnalysisManager* fManager;

        TString fOutputFile; //output file name

        
		TH1F *hmaxedep, *hsecedep, *hbardist, *htrackdist;
		TH1F *hmrate;

        G4int nGammaTracks;


        TFile* fTFile;
        G4double fTotalEdepSum[25][64];
        G4double fVisibleEdepSum[25][64];
        G4double fCollectedEdepSum[25][64];
        G4double fThreshold;
        G4double fModulationCurveThreshold;
        G4double fkB; //birks constant


        G4double particle_position[3];
        G4double particle_direction[3];
        G4double particle_energy;
        G4int eventID;


        G4int fEventID;

        //	TTree* fTTree;
        t2polar *tp;
        long long fEventsFill;
        TH2F *h2hit;

        TH2F *h2trig;
        TH2F *h2trig_prescaled;


        TH2F *h2na22;
        TH2F *h2hit_firstcompt;
        TH1F *h1ncompt; //
        TH1F *h1xi; //azimuthal angle
        TH1F *h1xi2; //azimuthal angle
        TH1F *h1stat; //azimuthal angle

        TH1F *h1Esum; //azimuthal angle
        G4double fEdep[1600];
        G4int ncompt;

        TH1F *hene;
        TH1F *hx;
        TH1F *hy;
        TH1F *hz;
        TH1F *hdx;
        TH1F *hdy;
        TH1F *hdz;
        TH1F *hpolx;
        TH1F *hpoly;
        TH1F *hpolz;
        TH1F *hpol_ang;


        G4EmSaturation *emSat;
        PrimaryGeneratorAction* primaryAction;
        G4RunManager* runManager;
        AnalysisMessenger *fAnalysisMessenger;
		G4int doCoincidence;
		G4double esum;


		G4int mrate[25]; 
		G4int fStoreSingleBarEvent;
        G4int fnEvents;
        G4int fnAccepted;
		G4int numEventStored;

		GRBGenerator *grbGun;
		SolarFlareGenerator *solarFlareGun;

		G4int fPositronEvents;

        G4String fCalibrationFile;
        TFile  *fCalibration;
        //calibration file
        DetResponse *detRes;
};
inline void AnalysisManager::AddTotalEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep) 
{
    fTotalEdepSum[copyNbModule][copyNbBar] += edep;
}
inline void AnalysisManager::AddVisibleEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep) 
{
    fVisibleEdepSum[copyNbModule][copyNbBar] += edep;
}
inline void AnalysisManager::AddCollectedEnergy(G4int copyNbModule, G4int copyNbBar, G4double edep) 
{
    fCollectedEdepSum[copyNbModule][copyNbBar] += edep;
}


#endif



