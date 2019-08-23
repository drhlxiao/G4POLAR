#ifndef t2polar_h
#define t2polar_h 1
#include <TH2.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

const int NPAIRMAX=10;

class t2polar {
	public :
		TTree          *tree;   //!pointer to the analyzed TTree or TChain
		Bool_t pattern[1600];   ///Trigger pattern
		Float_t	  pm[1600];        ///energy deposition
		Float_t     pm1[1600];       ///energy deposition after ionization quenching
		Float_t	  pm2[1600];       ///collected energy deposition

		Float_t	  pm3[1600];       ///energy resolution smearing
		Float_t	  adc[1600];       ///normalized adc channel
		Float_t	  thr[1600];       ///normalized adc channel

		Float_t   theta;
		Float_t   phi;
		Float_t   polang; //electric vector angle polarization angle

		Int_t           nhits;		    //number of hits

		Int_t        nrecorded; 

		Long64_t        evnt;		   ///event number
		Int_t ntrig;
		Int_t           nmod;		    //number of modules 
		Int_t       accepted[25]; ///if accepted by CT
		Int_t       tout1[25];  ///tout1
		Int_t       tout2[25]; ///tout2
		Int_t       tmth[25]; ///too many or too high
		Int_t       mhits[25]; ///too many or too high
		Int_t       recorded; ///if it is recorded by CT

		Int_t   track_bar[5]; //bar of the first 5 interactions 
		Float_t track_dist[5]; // distance between bars of the  first 5 interactions, the first value =0 always
		Int_t   track_phys[5];  //physics process of the interactions
		Float_t track_deltaE[5];  //physics process of the interactions


		Bool_t trigger[1600];   
		//hardware trigger


		Float_t  phi_scan, pol_scan;
		Float_t  phi_scan_step, pol_scan_step;


		Float_t xideg[2];
		Bool_t  xiok[2];
		Float_t max_dep[2];
		Float_t sec_dep[2];
		Float_t bar_dist[2];

		Int_t  max_bar[2];
		Int_t  sec_bar[2];
		Int_t  same_module[2];



		Float_t ekin; 
		Float_t source_pos[3];
		Float_t source_pol[3];
		Float_t source_dir[3];

		Double_t time;           //times from modules (*25ns)

		bool realsim;

		t2polar(bool rs=false); 
		virtual ~t2polar();

		virtual void     DefaultValues();

		float GetDistance(int bar1, int bar2);

		void GetMaximum(const float *data, 
				int &firstbar, float &depmax, int &secbar, float &depsec);

		void FillPolarization(float thr);
		void FillTriggerMap(TH2F *h2,TH2F *h2prescal);

		bool GetAzimuthalAngle(const float *data, Float_t &edep1, Float_t &edep2, 
			Int_t &bar1, Int_t &bar2, Float_t &dist, Float_t &xi, Int_t &module);
		int GetPhysicsProcessID(TString &processName);
		void FillTrack(int ncompt, int bar,  float totalEdep, TString  processName);
};
#endif
