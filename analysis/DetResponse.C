
#include "TRandom.h"
#include "TString.h"
#include "TFile.h"
#include "math.h"
#include "lib_det.h"
#include "DetResponse.hh"
#include "t2polar.h"
#include <fstream>
#include <iostream>
using namespace std; 
/**
 * @file: DetResponse.cc
 * @brief Simulation of detector response
 *
 *
 * @Author  : Hualin Xiao(hualin.xiao@psi.ch)
 * @Date    : Dec., 2015
 * @Version : 1.10
 */

const int cosmic_num_trigger_limit=10;
//it number of triggers is above this limit, this event is considered as a cosmic event
DetResponse::DetResponse()
{
    memset(response_matrix,0,sizeof(response_matrix));
    do_realistic_sim=false;
    n_single=0;
    n_cosmic=0;

}
DetResponse::~DetResponse()
{
    delete fcal;
}
void DetResponse::LoadResponseMatrixFrom(TString filename)
{
    if(filename=="")return;
    cout<<"####Reading energy response matrix from: "<<filename<<endl;

    TString verify_fname=filename;
    if(verify_fname.Contains(".root"))verify_fname.ReplaceAll(".root",".txt");
    else verify_fname="response.txt";

    ofstream fverify(verify_fname.Data()); 



    fcal=new TFile(filename);
    if(!fcal)
    {
        cout<<"#can not read the response file"<<endl;
        exit(0);
    }
    for(int k=0;k<25;k++)
    {
        ///load response matrices
        hresp[k]=(TH2F*)fcal->Get(Form("response/hresp%d",k));
        if(!hresp[k])
        {
            cout<<"##Failed to load response matrix of module : "<<k<<endl;
            cout<<"##Check if hresp"<<k<<" existing in the calibration file : "<<filename.Data()<<endl;
            do_realistic_sim=false;
            exit(0);
            return;
        }
        fverify<<"#Response matrix of module: "<<k<<endl;
        for(int i=0;i<64;i++)
        {
            for(int j=0;j<64;j++)
            {
                response_matrix[k][i][j]=hresp[k]->GetBinContent(i+1,j+1);
                fverify<<response_matrix[k][i][j]<<" ";
            }
            fverify<<endl;
        }

        ///to load thresholds
        hth[k]=(TH1F*)fcal->Get(Form("threshold/threshold%d",k));
        if(!hth[k]){
            cout<<"##Failed to load threshold of module : "<<k<<endl;
            cout<<"##Check if threshold"<<k<<" existing in the calibration file : "<<filename.Data()<<endl;
            do_realistic_sim=false;
            exit(0);
            return;
        }
        fverify<<"#PM, channel, index, threshold, threshold error "<<k<<endl;
        for(int i=0;i<64;i++)
        {
            threshold[k][i]=hth[k]->GetBinContent(i+1);

            threshold_error[k][i]=hth[k]->GetBinError(i+1);
            fverify<<k<<" "<<i<<" "<<threshold[k][i]<<" "<<threshold_error[k][i]<<endl;
        }

        //resolution @ 80keV

        fverify<<"#PM, channel, index, Energy resolution"<<k<<endl;
        hres_low=(TH1F*)fcal->Get("hres_low");
        hres_high=(TH1F*)fcal->Get("hres_high");
        if(!hres_low||!hres_high)
        {
            exit(0);
            return ;
        }
        for(int i=0;i<1600;i++)
        {
            double res80=hres_low->GetBinContent(i+1);
            if(res80>2)res80=2;
            //wrong fit channels
            energy_resolution_low[i]=res80/2.35;
            energy_resolution_high[i]=hres_high->GetBinContent(i+1)/2.35;
            //FWHF to sigma/E
            fverify<<k<<" "<<get_bar(i)<<" "<<i<<" "<<energy_resolution_low[i]<<" "<<energy_resolution_high[i]<<endl;

        }

    }
    do_realistic_sim=true;
    fverify.close();
}
void DetResponse::WriteResponses(TFile *fo)
{
    if(!fo||!do_realistic_sim)return;
    fo->cd();
    fo->mkdir("responses");
    fo->cd("responses");
    cout<<"writing responses to the output file"<<endl;
    //write response matrix
    for(int k=0;k<25;k++)
    {
        hresp[k]->Write(Form("hresp%d",k));
        hth[k]->Write(Form("threshold%d",k));;
    }
    hres_low->Write("hres_low");
    hres_high->Write("hres_low");
    fo->cd();
}


void DetResponse::SmearEnergy(const float coll[1600], float *adc)
{
    //energy resolution smearing simulation

    /// sigma/E= sqrt[ (noise_sigma/E)^2 + (a/sqrt(E))^2 +R^2)]
    ///  where:
    ///  the first item: electronics noise
    ///  the second item: statistical contribution
    ///  the third item: impurity of scintillator, spread of collection efficiency
    ///  Experimental data:
    ///    noise~ 2 keV
    ///    sigma/80=43%, @80keV
    ///    sigma/340=26%, @340keV


    //realistic model 



    if(do_realistic_sim)
    {
        //r^2=a+b/E
        //
        //use experimental data;
        
        /*
        double a, b;

        for(int i=0;i<1600;i++)
        {

            double dep=coll[i]; //collected visible energy
            if(dep<=2)continue;

            double res_low=energy_resolution_low[i];
            double res_high=energy_resolution_high[i];
            b=(res_low*res_low-res_high*res_high)/1.043e-2;
            a=res_low*res_low-b/80.;
            double eSigma= dep* sqrt(a+b/dep);
            adc[i]=gRandom->Gaus(dep,eSigma);
        }

    }
    else
    {
    */
        //do not use experimental data

        ///The following model is based on the paper:
        ///NIM A C.W.E. van Eijk et al. / Radiation Measurements 33 (2001) 521–525
        ///parameters were tune with experimental spectra
        for(int i=0;i<1600;i++)
        {
            adc[i]=0;
            float dep=coll[i]; //collected visible energy
            float ped=0;


            double res_high=energy_resolution_high[i];

            //Rstat@470keV=4.7%
            //
            float res_sci=sqrt(res_high*res_high-0.047*0.047);
            float resLid=0;
            if(dep>3)resLid=sqrt(1./(dep*9*0.3*0.3));//see the paper
            //light yield=9/keV
            //quantum efficiency 0.2
            //oct.19th, 2015
            //sigma/80=43%, @80keV
            //sigma/340=26%, @340keV
            //collection efficiency 0.32, according to Merlin's simulation
            float resTot=sqrt(res_sci*res_sci+resLid*resLid);
            float eSigma=resTot*dep;
            //double esigma=resTot*dep;
            //FWHM to sigma
            float energy=gRandom->Gaus(dep,eSigma);
            adc[i]=energy+ped;
            //double th=gRandom->Gaus(40,5);
            //if(pm2[i]<th)pm2[i]=0;
        }
    }

}



double DetResponse::GetLightCollectionEfficiency(double z)
{

    //use Merlin's simulation result, see the document "New optical simulations" in polar svn
    double tankZ=172./2.; //mm
    double disBottom=z+tankZ;
    double disTop=tankZ-z;
    double collectionEff;
    double normalizationFactor=0.32;
    if(disBottom<=4)collectionEff=((60-disBottom*(30/4.))/100.)/normalizationFactor;
    else if(disTop<4)
    {
        collectionEff=((60-disTop*(30/4.))/100.)/normalizationFactor;
    }
    else collectionEff=0.32/normalizationFactor;

    return collectionEff;

}
void DetResponse::SimTrigger(const float adc[1600],t2polar *tp)
{
    //simulate trigger, and fill tout1, tout2, too-many information here
    ///TODO
    int total_triggers=0;
    for(int k=0;k<25;k++)
    {
        int triggers=0;
        tp->tout1[k]=0;
        tp->tout2[k]=0;
        tp->tmth[k]=0;

        for(int j=0;j<64;j++)
        {
            int bar=get_index(k,j);
            //randomize the threshold
            float real_threshold=
            gRandom->Uniform(threshold[k][j]-threshold_error[k][j], threshold_error[k][j]+threshold[k][j]);
            if(adc[bar]>real_threshold)
            {
                tp->pattern[bar]=1;
                triggers++;

            }
            else 
            {
                tp->pattern[bar]=0;
            }
        }
        if(triggers==0)
        {
            for(int j=0;j<64;j++)
            {
                int bar=get_index(k,j);
                tp->adc[bar]=0;

            }
             
        }
        total_triggers+=triggers;

        

        if(triggers>=1)tp->tout1[k]=1;
        if(triggers>=2)tp->tout2[k]=1;
        if(triggers>=cosmic_num_trigger_limit)
        {
            tp->tmth[k]=1;
        }
    }

    //reset number of triggers and accepted signal
    tp->n=total_triggers;
    for(int k=0;k<25;k++)
    {
        if(total_triggers>=2&&total_triggers<=cosmic_num_trigger_limit)
        {
            if(tp->tout1[k]==1)tp->accepted[k]=1;
            tp->recorded=1;

        }
        else
        {
            tp->accepted[k]=0;
            tp->recorded=0;

        }

    }
}
void DetResponse::SimCrosstalkAndNonuniformity(const Float_t in[1600], float *out)
{
    if(!do_realistic_sim)return;
    //do not simulation if it is not set
    int module,bar;
    float response;

    int bar_in_module;

    float pedestal;

    for(int i=0;i<1600;i++)
    {

        module=get_pm(i);
        bar_in_module=get_bar(i);

        float sum=0;
        for(int j=0;j<64;j++)
        {
            response=response_matrix[module][bar_in_module][j];
            bar=get_index(module,j);
            sum=sum+response*in[bar];
            //  cout<<i<<" "<<bar<<endl;
            // cout<<in[i]<<" "<<in[bar]<<" "<<response<<endl;
        }

        out[i]=sum;
        //simulate pedestal
        pedestal=gRandom->Gaus(0,130);
        ///sigma of pedestal ~ 130 channel;
        out[i]=sum+pedestal;
    }


}

//implementation of prescalling
//return true if it pass prescaling condition
bool DetResponse::PassPrescaling(t2polar *t, int pre_s, int pre_c)
{

    bool pass=true;
    bool is_single=false;
    bool is_cosmic=false;
    if(t->n==1)
    {
        is_single=true;
        n_single++;
    }
    if(t->n>=cosmic_num_trigger_limit)
    {
        is_cosmic=true;
        n_cosmic++;
    }

    if(is_single&&pre_s==0)pass=false;
    if(is_cosmic&& pre_c==0)pass=false;

    if(pre_s>0&&is_single)
    {
        if(n_single%pre_s==0)pass=true;
        else pass=false;
    }

    if(pre_c>0&&is_cosmic)
    {
        if(n_cosmic%pre_c==0)pass=true;
        else pass=false;
    }

    return pass;

}
