#ifndef _DET_RESPONSE_
#define _DET_RESPONSE_
class t2polar;
class TH2F;
class TH1F;
class TFile;
class DetResponse
{

    public:
        DetResponse();
        ~DetResponse();

    private:
        float response_matrix[25][64][64];
        float threshold[25][64];
        float threshold_error[25][64];
        float energy_resolution_low[1600];
        float energy_resolution_high[1600];
        bool  do_realistic_sim;
        TH2F *hresp[25];
        TH1F *hth[25];
        TH1F *hres_low;
        TH1F *hres;
        TH1F *hres_high;
		TH1F *hthkeV;
        TFile *fcal;
        int n_tout1;
        int n_cosmic;

    
    public:
        void WriteResponses(TFile *f);

        void SimTrigger(const float adc[1600],int pre_s, int pre_c, t2polar *t);
        void SimCrosstalkAndNonuniformity(const Float_t in[1600], float *out);
        void LoadResponseMatrixFrom(TString filename);
        void SmearEnergy(const float coll[1600], float *adc );
        double GetLightCollectionEfficiency(double);
        int GetNumSingleEvents(){return n_tout1;}
        int GetNumCosmicEvents(){return n_cosmic;}

        //void SimPrescaling(int pre_s, int pres_c,t2polar *tp);
        
    



};

#endif
