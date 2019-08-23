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

void Smear(TH1F *hin, TH1F *hout)
{
    float a,b;
    if(!hin)return;

    int entries=hin->GetEntries();
	cout<<"processing entries:"<<entries<<endl;
    if(entries==0)return;
    float norfactor=1;
    int nevents=entries;
    if(entries<1000)
    {
        norfactor=1000./entries;
        nevents=1000;
    }
	//hout=(TH1F*)hin->Clone();
    //hout->Reset();
    double sigma;
    float adc;
    for(int i=0;i<nevents;i++)
    {
        double dep=hin->GetRandom();
		if(dep==0)continue;
		sigma= dep* sqrt(-0.019915+15.485/dep);
		 adc=gRandom->Gaus(dep,sigma);
         hout->Fill(adc);
    }
}

int main(int argc, char *argv[])
{
    TString fnres;
    TString finame;
    TString foname;

	bool realdata=true;
	if(argc==3)
	{
		finame=argv[1];
		foname=argv[2];
	}
	else
	{
        cout<<"smearSpectrum  input.root, output.root"<<endl;
		return 0;
	}


    TH1F *hspec_co[1600];
    TH1F *hspec[1600];
    
    TH1F *hspec_co_smear[1600];
    TH1F *hspec_smear[1600];
    
    TFile fin(finame.Data());
    TFile fout(foname.Data(),"recreate");


    for(int i=0;i<1600;i++)
    {
        hspec_co[i]=(TH1F*)fin.Get(Form("spectra/hspec_co%d",i));
        hspec[i]=(TH1F*)fin.Get(Form("spectra/hspec%d",i));


		if(hspec_co[i])
		{
			hspec_co_smear[i]=NULL;
			fout.cd();
			TH1F *hco=(TH1F*)hspec_co[i]->Clone();
			TH1F *hco2=(TH1F*)hspec_co[i]->Clone();
			
			hco2->Reset();
			Smear(hco,hco2);
			hco->Write(Form("hspec_co%d",i));
			if(hco2)hco2->Write(Form("hspec_co_smear%d",i));
		}

    }
     
     
    return 0;
} 

