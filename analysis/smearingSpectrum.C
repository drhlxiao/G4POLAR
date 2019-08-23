#include <iostream>
#include <string>  
#include <TMath.h>  
#include <TApplication.h> 
#include <TRandom.h> 
#include <TGraph.h>
#include <TROOT.h> 
#include <TFile.h>  
#include <TTree.h>  
#include <TCanvas.h>  
#include <TF1.h>  
#include <TH1F.h> 
using namespace std; 



TF1 f("f","gaus(0)",0,1000);
double Gaus(double x, double x0,double sigma)
{
    return TMath::Exp(-(x-x0)*(x-x0)/(2*sigma*sigma));
}
/*
   bool Smear(float res_low, float res_high, TH1F *hin, TH1F *hout, float &a, float &b)
   {
   b=(res_low*res_low-res_high*res_high)/1.043e-2;
   a=res_low*res_low-b/80.;

   if(!hin)return false;
   if(!hout)return false;

   int entries=hin->GetEntries();
   if(entries==0)return false;
   float norfactor=1;
   int nevents=entries;
   if(entries<1000)
   {
   norfactor=1000./entries;
   nevents=1000;
   }

   int nbinx=hin->GetNbinsX();

   hout->Reset();
   double sigma;
   double r;
   if(a<0.0)a=0;
   double y;
   for(int i=0;i<nbinx;i++)
   {
   y=0;

   double x0=hin->GetBinCenter(i+1);
   for(int j=0;j<nbinx;j++)
   {
   double x=hin->GetBinCenter(j+1);
   r=sqrt(a+b/x);
   sigma=r*x;
   double val=Gaus(x,x0,sigma);
   y+=val;
   cout<<x0<<" "<<x<<" "<<sigma<<" "<<val<<endl;
   if(x<x0-3*sigma)break;
   if(x>x0+3*sigma)break;
   }
   hout->SetBinContent(i+1,y);
   cout<<"setting: "<<i+1<<" "<<x0<<" "<<y<<endl;
   }

   hout->SetEntries(entries);
   return true;
   }
   */
bool Smear(float res_low, float res_high, TH1F *hin, TH1F *hout, float &a, float &b)
{
    b=(res_low*res_low-res_high*res_high)/1.043e-2;
    a=res_low*res_low-b/80.;

    if(!hin)return false;
    if(!hout)return false;

    int entries=hin->GetEntries();
    if(entries==0)return false;
    float norfactor=1;
    int nevents=entries;
    if(entries<1000)
    {
        norfactor=1000./entries;
        nevents=1000;
    }

    int nbinx=hin->GetNbinsX();

    hout->Reset();
    double sigma;
    double r;
    if(a<0.0)a=0;
    double y;
    for(int i=0;i<nbinx;i++)
    {
        double x=hin->GetBinCenter(i+1);
        //    r=sqrt(a+b/x);
        r=sqrt(res_high*res_high+1./(x*9*0.3*0.32));
        sigma=r*x;

        if(x>500)break;
        for(int k=0;k<hin->GetBinContent(i+1);k++)
        {
            for(int j=0;j<10;j++)
            {
                double rnd=gRandom->Gaus(x,sigma);
                hout->Fill(rnd);
            }
        }
    }
    return true;
}


void Help()
{
    cout<<"smearingSpectrum  -res response -i input.root -o output.root"<<endl;
}

int main(int argc, char *argv[])
{
    if(argc!=7)
    {
        Help();
        return 0;
    }
    TString sel;
    int s=0;
    TString fnres;
    TString finame;
    TString foname;
    while(s<argc-1)
    {

        sel=argv[++s];
        if(sel=="-h"||sel=="--help"||sel=="--h")
        {
            Help();
            return 0;

        }
        else if(sel=="-res")
        {
            fnres=argv[++s];
            if(!fnres.Contains(".root"))
            {
                Help();
                return 0;
            }

        }
        else if(sel=="-o")
        {
            foname=argv[++s];
            if(!foname.Contains(".root"))
            {
                Help();
                return 0;
            }
        }
        else if(sel=="-i")
        {
            finame=argv[++s];
            if(!finame.Contains(".root"))
            {
                Help();
                return 0;
            }
        }

    }


    TH1F *hspec_co[1600];
    TH1F *hspec[1600];

    TH1F *hspec_co_smear[1600];
    TH1F *hspec_smear[1600];

    TFile fin(finame.Data());
    TFile fout(foname.Data(),"recreate");

    TFile fres(fnres.Data());

    TH1F *hres_low=(TH1F*)fres.Get("hres_low");
    TH1F *hres_high=(TH1F*)fres.Get("hres_high");
    if(!hres_low||!hres_high)
    {
        cout<<"can not load histogram having resolution"<<endl;
        return 0;
    }

    double res_low,res_high;
    f.SetNpx(500);

    for(int i=0;i<1600;i++)
    {
        cout<<"channel:"<<i<<endl;
        hspec_co[i]=(TH1F*)fin.Get(Form("spectra/hspec_co%d",i));
        hspec[i]=(TH1F*)fin.Get(Form("spectra/hspec%d",i));
        res_low=hres_low->GetBinContent(i+1);
        res_high=hres_high->GetBinContent(i+1);

        if(hspec_co[i])
        {
            hspec_co_smear[i]=(TH1F*)hspec_co[i]->Clone();
            hspec_co_smear[i]->Clear();
            float a, b;
            if(!Smear(res_low,res_high,hspec_co[i],hspec_co_smear[i],a,b))cout<<"spect1 smear wrongly for channel: "<<i<<endl;;
            fout.cd();
            hspec_co_smear[i]->Write(Form("hspec_co_smear%d",i));
            hspec_co[i]->Write(Form("hspec_co%d",i));
        }
        if(hspec[i])
        {
            hspec_smear[i]=(TH1F*)hspec[i]->Clone();
            hspec_smear[i]->Clear();
            float a, b;
            if(!Smear(res_low,res_high,hspec[i],hspec_smear[i],a,b))cout<<"smear wrongly for channel: "<<i<<endl;;
            fout.cd();
            hspec_smear[i]->Write(Form("hspec_smear%d",i));
            hspec[i]->Write(Form("hspec%d",i));
        }



    }
    cout<<"output:"<<foname<<endl;


    return 0;
} 

