
#include "TRandom.h"
#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "math.h"
#include "lib_det.h"
#include "DetResponse.hh"
#include "t2polar.h"

#include "G4ios.hh"
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


const double noiseE=2; 
//mean pedestal: 33 ADC channel
//Bank 50 mean CE edge: 33/( 5000 /340.7) =2.2
//It will not make big difference

const double thresholdError=noiseE;


const double defaultThreshold=9;
const double thrMin=4;
const double thrMax=25;
const double resMin=6;
const double resMax=20;


//set threshold to 9 for the bad threshold values
const double conversionFactor=9*0.32*0.2;

double getNbPE(double x)
{
	return x*9*0.32*0.2;
}
double calResolution(double x)
{
	double 	res=sqrt(0.10*0.10+1.0/(conversionFactor*x));
	return res;
}

/*
Double_t Poisson(Double_t *x, Double_t *par)
{
	return TMath::Poisson(x[0],par[0]);
}
Double getRandomEnergy(Double x)
{
	return x;

}
*/

DetResponse::DetResponse()
{
    memset(response_matrix,0,sizeof(response_matrix));
    do_realistic_sim=false;
    n_tout1=256;
    n_cosmic=256;
	hthkeV=NULL;
	hres=NULL;

}
DetResponse::~DetResponse()
{
    delete fcal;
}
void DetResponse::LoadResponseMatrixFrom(TString filename)
{
    if(filename=="")return;
    G4cout<<"####Reading energy response matrix from: "<<filename<<G4endl;

 //   TString verify_fname=filename;
  //  if(verify_fname.Contains(".root"))verify_fname.ReplaceAll(".root",".txt");
  //  else verify_fname="response.txt";




    fcal=new TFile(filename);
    if(!fcal)
    {
        G4cout<<"#can not read the response file"<<G4endl;
        exit(0);
    }
   hthkeV=(TH1F*)fcal->Get(Form("hthkeV"));
   hres=(TH1F*)fcal->Get(Form("h1res"));
   if(!hthkeV)return;

   G4cout<<"#Loading threshold values"<<G4endl;
   double thr,reso;
   for(int i=1;i<=1600;i++)
   {
	   thr=hthkeV->GetBinContent(i);
	   if(thr>thrMax||thr<thrMin)hthkeV->SetBinContent(i,defaultThreshold);
	   hthkeV->SetBinError(i,thresholdError);

	   reso=hres->GetBinContent(i);
	   if(reso<resMin)hres->SetBinContent(i,resMin);
	   if(reso>resMax)hres->SetBinContent(i,resMax);


	   //verified to be correct
   }
   do_realistic_sim=true;
}
void DetResponse::WriteResponses(TFile *fo)
{
    if(!fo||!do_realistic_sim)return;
    fo->cd();
	TH1F hres_simple("hres_simple","Energy resolution model; Energy(keV); Resolution (sigma/E)",1000,0,1000);
	for(int i=0;i<1000;i++)
	{
		double resolution=calResolution(i+0.1);
		hres_simple.SetBinContent(i+1,resolution);
	}
	hres_simple.Write();
	if(hthkeV)hthkeV->Write();
	if(hres)hres->Write();
}


void DetResponse::SmearEnergy(const float coll[1600], float *out)
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

//	memcpy(coll,adc);
	//HUalin Xiao 2016;



		double res;
		double eSigma;

        for(int i=0;i<1600;i++)
        {

            double dep=coll[i]; //collected visible energy
            //if(dep<=2)continue;
			
			double pedestal=gRandom->Gaus(0,noiseE);
			out[i]=pedestal;
			if(dep>0)
			{
				res=calResolution(dep);
				eSigma= dep* res;
				out[i]+=gRandom->Gaus(dep,eSigma);
				
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
void DetResponse::SimTrigger(const float adc[1600],int pre_s, int pre_c, t2polar *tp)
{
    //simulate trigger, and fill tout1, tout2, too-many information here
    ///TODO
    int total_triggers=0;
    for(int k=0;k<25;k++)
    {
        int triggers=0;
        tp->tout1[k]=0;
        tp->tout2[k]=0;
		//this is module number instead of ct number 
		//becareful
        tp->tmth[k]=0;

        for(int j=0;j<64;j++)
        {
            int bar=get_index(k,j);
            //randomize the threshold

			double thr=hthkeV->GetBinContent(bar+1);
            float real_threshold=gRandom->Gaus(thr,thresholdError);
            //float real_threshold=thr;
			//gausian threshold
            if(adc[bar]>real_threshold)
            {
				tp->thr[bar]=real_threshold;
                tp->trigger[bar]=true;
                triggers++;

            }
            else 
            {
                tp->trigger[bar]=false;
            }
        }
        if(triggers==0)
        {
            for(int j=0;j<64;j++)
            {
                int bar=get_index(k,j);
                tp->adc[bar]=0;
				//not readout by electronics

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
    tp->ntrig=total_triggers;

	if(tp->ntrig>=1)n_tout1++;
	if(tp->ntrig>cosmic_num_trigger_limit)
	{
		n_cosmic++;
	}


	tp->recorded=0;
	if(n_tout1%pre_s==0)tp->recorded=1;

	if(n_cosmic%pre_c==0)tp->recorded=1;

    for(int k=0;k<25;k++)
    {
        if((total_triggers>=2&&total_triggers<=cosmic_num_trigger_limit)||tp->recorded)
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
/*    if(!do_realistic_sim)return;
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
	*/

	//don't simult it anymore
	
	//not to goto adc channel

}

