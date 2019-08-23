#ifndef t2polar_h
#define t2polar_h 1
#include <TH2.h>
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
        Int_t           pattern[1600];   ///Trigger pattern
        Float_t	  pm[1600];        ///energy deposition
        Float_t     pm1[1600];       ///energy deposition after ionization quenching
        Float_t	  pm2[1600];       ///collected energy deposition
        Float_t	  pm4[1600];       ///collected energy deposition
        Float_t	  pm3[1600];       ///collected energy deposition
        Float_t	  adc[1600];       ///normalized adc channel
        Long64_t        evnt;		   ///event number
        Int_t           n;		    //number of hits
        Int_t       accepted[25]; ///if accepted by CT
        Int_t       tout1[25];  ///tout1
        Int_t       tout2[25]; ///tout2
        Int_t       tmth[25]; ///too many or too high
        Int_t       recorded; ///if it is recorded by CT
        

        //for coincidence 

        Int_t       coincidence[1600]; ///
      
        //for coincidence 
        Int_t       npairs; ///number of coincidence pairs
        Int_t     bar1[NPAIRMAX];
        Int_t     bar2[NPAIRMAX];
        Float_t	  open_angle[NPAIRMAX];       ///open angle
        Float_t	  tot_angle[NPAIRMAX];       ///open angle
        Float_t	  dist_source[NPAIRMAX];       ///open angle
        Float_t   pair_dist1[NPAIRMAX]; // the distance from source to one of the coincidence hit
        Float_t   pair_dist2[NPAIRMAX]; // the distance from source to one of the coincidence hit
        Float_t   pair_dist[NPAIRMAX]; //distance between two hit pairs

        

 


        //for simulation

        Int_t       bar_maxedep; //the bar with the maximum energy deposition
        Int_t       bar_secmaxedep; //the bar with the second maximum energy deposition
        Int_t       first_bar; //the first bar interacting with the gamma-ray
        Float_t	  distance_source_maxbar;       ///the distance between the source to the bar with maximum energy deposition
        

        ////keeps the first 5 track of the primary gamma rays
        Int_t track_process[5]; 
        ///physics process name
        ///1: compt
        // 2: photoelectric
        // 3: msc
        // 4: 
        Int_t   track_bar[5]; 
        ///the bar number where the interaction takes place
        Float_t  track_edep[5];
        /// energy deposition of the first 5 tracks



        //primary particle information
        double particle_energy; 
        double particle_position[3];
        double particle_direction[3];
        double particle_pol[3];

        double time;           //times from modules (*25ns)
        double xideg;          //azimuthal angle respect to x-axis
        double xideg2;          //azimuthal angle between  the arrow (source--> bar with maxedep) and the arrow (bar with maxedep-->bar with the
        //second edep

        // List of branches
        TBranch        *b_pattern;   //!
        TBranch        *b_pm;   //!
        TBranch        *b_pm1;   //!
        TBranch        *b_pm2;   //!
        TBranch        *b_pm4;   //!
        TBranch        *b_pm3;   //!
        TBranch        *b_adc;   //!
        TBranch        *b_evnt;   //!
        TBranch        *b_n;   //!
        TBranch        *b_time;   //!
        TBranch        *b_xideg;   //!
        TBranch        *b_xideg2;   //!
        TBranch        *b_particle_position;   //!
        TBranch        *b_particle_direction;   //!
        TBranch        *b_particle_pol;   //!
        TBranch        *b_particle_energy;   //!
        TBranch        *b_accepted;   //!
        TBranch        *b_tout1;   //!
        TBranch        *b_tout2;   //!
        TBranch        *b_tmth;   //!
        TBranch        *b_recorded;   //!
        TBranch        *b_bar_maxedep;   //!
        TBranch        *b_bar_secmaxedep;   //!
        TBranch        *b_first_bar;   //!
        TBranch        *b_distance_source_maxbar;   //!
        TBranch        *b_track_bar;   //!
        TBranch        *b_track_process;   //!
        TBranch        *b_track_edep;   //!

        TBranch        *b_open_angle;   //!
        TBranch        *b_pair_dist;   //!
        TBranch        *b_pair_dist1;   //!
        TBranch        *b_pair_dist2;   //!

        t2polar(TTree *t=0); 

        virtual ~t2polar();

        void Init(TTree *t);

        virtual void     DefaultValues();

        virtual float GetAzimuthalAngle(const float *data, double eth);

        void GetMaximum(const float *data, float &firstmax, int &idx1, float &secmax, int &idx2);

        int GetPhysicsProcessID(TString processName);
        void SetPhysicsProcess(Int_t ntrack, TString processName);
        TString GetPhysicsProcessName(int id);

        float GetDistance(float x0,float y0,int bar);
        float GetScaterAzimuthalAngle(float x0,float y0, int bar_max, int bar_sec);

        void SetLeavesFrom(t2polar *t);
//        void FindCoincidencePairs(float threshold,float distance_highlimit,float distance_lowlimit, float source_x, float source_y);

    int FindCoincidencePairs(float thr,      float sourcex, float sourcey);
    bool AreInSameGroup(int b1,int b2);
};
#endif
