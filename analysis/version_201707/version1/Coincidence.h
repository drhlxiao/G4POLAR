#ifndef _COINCIDENCE_H
#define _COINCIDENCE_H


class TFile;
class TH1F;
class TH2F;
class TVector3;

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
        float distance_highlimit;
        float distance_lowlimit;

        float dist1;
        float dist2;
        float pair_dist;

        TString  data_type;
        TString  idata_fname;
        TString  odata_fname;

        t2polar *ti;
        t2polar *to;
        TFile *fi;
        TFile *fo;
        TTree *treein;

        long num_events;


    public:

        void LoadSettings(TString configfile);
        void AddASource(float sourcex,float sourcey); //add a source position
        void SetDataType(TString name){data_type=name;} //either pm or pattern


        int FindCoincidencePairs(t2polar *tin, t2polar *tout, float thr,float dist_highlimit,float dist_lowlimit, float sourcex, float sourcey,TH2F *);

        void DoCoincidence(TString ifname, TString ofname);

        void SetCoincidenceDistanceBetween(float dfrom,float dto){distance_lowlimit=dfrom;distance_highlimit=dto;}
        void SetThreshold(float value){threshold=value;}
        void SetNEvents(long events){num_events=events;}
};

#endif
