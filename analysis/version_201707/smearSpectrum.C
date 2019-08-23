// a program to smear energy spectrum 
// Hualin Xiao. Feb. 12, 2016

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

void Smear(float res_low, float res_high, TH1F *hin, TH1F *hout)
{
    float a,b;
    b=(res_low*res_low-res_high*res_high)/1.043e-2;
    a=res_low*res_low-b/80.;

    if(!hin)return;
    if(!hout)return;

    int entries=hin->GetEntries();
    if(entries==0)return;
    float norfactor=1;
    int nevents=entries;
    if(entries<1000)
    {
        norfactor=1000./entries;
        nevents=1000;
    }

    hout->Reset();
    double sigma;
    float adc;
    for(int i=0;i<nevents;i++)
    {
        double dep=hin->GetRandom();
        sigma= dep* sqrt(a+b/dep);
         adc=gRandom->Gaus(dep,sigma);
         hout->Fill(adc);
    }
    hout->Scale(1./norfactor);
    hout->SetEntries(entries);
}

int main(int argc, char *argv[])
{
    if(argc!=4)
    {
        cout<<"smearSpectrum  response.root input.root, output.root"<<endl;
        return 0;
    }
    TString fnres=argv[1];
    TString finame=argv[2];
    TString foname=argv[2];

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

    for(int i=0;i<1600;i++)
    {
        hspec_co[i]=(TH1F*)fin.Get(Form("spectra/hspec_co%d",i));
        hspec[i]=(TH1F*)fin.Get(Form("spectra/hspec%d",i));

        if(hspec_co[i])hspec_co_smear[i]=(TH1F*)hspec_co[i]->Clone();
        if(hspec[i])hspec_smear[i]=(TH1F*)hspec[i]->Clone();

        res_low=hres_low->GetBinContent(i+1);
        res_high=hres_high->GetBinContent(i+1);
        Smear(res_low,res_high,hspec[i],hspec_smear[i]);
        Smear(res_low,res_high,hspec_co[i],hspec_co_smear[i]);
        
        fout.cd();
        hspec_co_smear[i]->Write(Form("hspec_co_smear%d",i));
        hspec_co[i]->Write(Form("hspec_co%d",i));
        hspec_smear[i]->Write(Form("hspec_smear%d",i));
        hspec[i]->Write(Form("hspec%d",i));

    }
     
     
    return 0;
} 

