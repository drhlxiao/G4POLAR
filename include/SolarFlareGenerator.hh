
#ifndef SolarFlareGenerator_h
#define SolarFlareGenerator_h 1

#include <sstream>
#include <TROOT.h>

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;

class TF1;


class SolarFlareGenerator
{
	public: 
		SolarFlareGenerator();
		~SolarFlareGenerator();
	public:
		void GeneratePrimaryVertex(G4Event *evt,  G4ParticleGun *gun);

		//void SetAlpha(G4double a){alpha=a;}
		//void SetBeta(G4double a){beta=a;}
		//void SetEpeak(G4double a){Epeak=a;}
		void SetGamma(G4double a){gamma=a;}
		void SetF35(G4double a){flux35=a;}
		void SetEmin(G4double a){e_min=a;}
		void SetEmax(G4double a){e_max=a;}
		void SetSphRad(G4double a){SphRad=a;}
		void SetCircRad(G4double a){CircRad=a;}

		void SetPolAngle(G4double a){pol_angle=a;}
		void SetPolFrac(G4double a){pol_frac=a;}

		void SetTheta(G4double a){theta=a;}
		void SetPhi(G4double a){phi=a;}
		//void SetFlux(G4double a){flux=a;}
		//void SetFluxEmin(G4double a){fluxEmin=a;}
		//void SetFluxEmax(G4double a){fluxEmax=a;}

		void Init();

		G4double GetTheta(){return theta;}
		G4double GetPhi(){return phi;}
		//G4double GetAlpha(){return alpha;}
		//	G4double GetBeta(){return beta;}
		//	G4double GetEpeak(){return Epeak;}
		G4double GetGamma(){return gamma;}
		G4double GetF35(){return flux35;}


		G4double GetEmin(){return e_min;}
		G4double GetEmax(){return e_max;}
		G4double GetPolFrac(){return pol_frac;}
		G4double GetPolAngle(){return pol_angle;}

		G4ThreeVector GetPositionVector(){return position;}
		G4ThreeVector GetPolarizationVector(){return polarization;}
		G4ThreeVector GetDirectionVector(){return direction;}
		G4int GetEventID(){return nevents;}
		G4int GetTotalNbEvents();
		G4String GetMessage(){return ss.str();}
		G4double GetCurrentPolarPhi();

	private:

		std::stringstream ss;
		G4double theta;
		G4double phi;
		//G4double alpha;
		//	G4double beta;
		//	G4double Epeak;
		G4double gamma;
		G4double flux35;
		G4double e_min;
		G4double e_max;
		G4double pol_frac;
		G4double pol_angle;
		G4double SphRad;
		G4double CircRad;
				G4double flux;
		//		G4double fluxEmin;
		//		G4double fluxEmax;
		G4int nevents;
		TF1 *f1;
		G4double px,py,pz;
		G4double pos_x,pos_y, pos_z;
		G4double pol_x,pol_y, pol_z;

		G4ThreeVector position;
		G4ThreeVector direction;
		G4ThreeVector polarization;
		G4int total_events;
		G4double pangle;

};

#endif
