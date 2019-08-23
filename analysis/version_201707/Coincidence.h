#ifndef _COINCIDENCE_H
#define _COINCIDENCE_H


class TFile;
class TH1F;
class TH2F;
class TVector3;
class TNamed;

class TTree;
class t2polar;
#include <vector>


class Coincidence 
{
    public:
        Coincidence();
        ~Coincidence();
    private:
        int n_sources;
        std::vector <float> source_x;
        std::vector <float> source_y;
        float threshold;
        float high_energy_cut;
        float distance_highlimit;
        float distance_lowlimit;

        float dist1;
        float dist2;
        float pair_dist;

        int nstore;
        TString  data_type;
        TString  idata_fname;
        TString  odata_fname;

        t2polar *ti;
        t2polar *to;
        TFile *fi;
        TFile *fo;
        TTree *treein;
        TH1F *hspec_co[1600];
        TH1F *hspec[1600];
        TH1F *hspc_adc[1600];

        float dist_cut;
        float angle_cut;

        long num_events;
        bool print_hits_position;
        TNamed cmd;

        bool is_hit_dist_cut_set;


    public:

        void SetCommand(TString s){cmd.SetTitle(s);}
        void PrintHitsPosition(){print_hits_position=true;}
        void LoadSettings(TString configfile);
        void AddASource(float sourcex,float sourcey); //add a source position
        void SetDataType(TString name){data_type=name;} //either pm or pattern
        void SetNStore(int n){nstore=n;};
        void SetAngleCut(float val){angle_cut=val;}
        void SetDistanceCut(float val){dist_cut=val;}
        void SetHighEnergyCut(float val){high_energy_cut=val;}


        int FindCoincidencePairs(t2polar *tin, t2polar *tout, float thr,float dist_highlimit,float dist_lowlimit, float sourcex, float sourcey,
				TH2F *,TH2F *, TH1F *hopen, TH1F *hdist,TH1F *htot, TH1F*, TH1F*,TH1F*, TH1F*, TH1F*, TH1F*);

        void DoCoincidence(TString ifname, TString ofname);

        void SetCoincidenceDistanceBetween(float dfrom,float dto){distance_lowlimit=dfrom;distance_highlimit=dto;is_hit_dist_cut_set=true;}
        void SetThreshold(float value){threshold=value;}
        void SetNEvents(long events){num_events=events;}
        bool AreInSameGroup(int bar1,int bar2);
        bool IsLocalMaximum(int ch, float data[1600] );
		float Point2LineDistance(float px0, float py0, float px1, float py1, float px2, float py2);
		bool TestGeometry(int bar1, int bar2, float sx, float sy, float &dist2source, float &distlim, float tolerance=0, float barwidth=5.8);
};

#endif
