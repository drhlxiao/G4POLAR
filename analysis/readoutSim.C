//A program to simulation detector response and readout
//Hualin Xiao. Feb.15th, 2016

#include <TApplication.h> 
#include <TSystem.h> 
#include <TGraph.h>
#include <TROOT.h> 
#include <TFile.h>  
#include <TTree.h>  
#include <TCanvas.h>  
#include <TF1.h>  
#include <TH1F.h> 
#include <TH2F.h> 
#include "DetResponse.hh"
#include "t2polar.h"
#include <iostream>
#include <string>    
using namespace std; 

void Help()
{
    cout<<"Simulate detector response and readout "<<endl;
    cout<<"usage:"<<endl
        <<"readoutSim OPTIONS  -i  <input> -o <output> "<<endl
        <<"Options:"<<endl
        <<"-fcal            <file>"<<endl
        <<"                 set calibration file"<<endl
        <<"-n               number of events"<<endl
        <<"-sim_trigger      simulate trigger (false)"<<endl
        <<"-sim_xtalk        simulate crosstalk (false)"<<endl
        <<"-sim_smearing     simulation energy resolution smearing (true)"<<endl
        <<"-pre_s           <value> "<<endl
        <<"                 set prescaling single. The default value is 1."<<endl
        <<"-pre_c           <value> "<<endl
        <<"                 set prescaling cosmic. The default value is 1. "<<endl
        <<"-do_modc          <data_type> <threshold> calculate modulation curve"<<endl
        <<"                   data_type can be pm, pm1, pm2, pm3, adc"<<endl;
}
int main(int argc, char *argv[])
{

    TString fcal="";
    TString finame;
    TString foname;
    bool sim_trigger=false;
    bool sim_xtalk=false;
    bool sim_smearing=true;
    bool do_modc=false;
    int pre_s=1;
    int pre_c=1;
    float modc_thr=30;
    int s=0;
    TString sel;
    TString data_type="pm";
    long nevents=0;
    while(s<argc-1)
    {

        sel=argv[++s];
        if(sel=="-h"||sel=="--help"||sel=="--h")
        {
            Help();
            return 0;

        }
        else if(sel=="-i")
        {
            finame=argv[++s];
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
        else if(sel=="-fcal")
        {
            fcal=argv[++s];
            if(!fcal.Contains(".root"))
            {
                Help();
                return 0;
            }
        }
        else if(sel=="-sim_xtalk")
        {
            sim_xtalk=true;
        }
        else if(sel=="-sim_smearing")
        {
            sim_smearing=true;
        }
        else if(sel=="-sim_trigger")
        {
            sim_trigger=true;
        }
        else if(sel=="-pre_s")
        {
            pre_s=atoi(argv[++s]);
        }
        else if(sel=="-pre_c")
        {
            pre_c=atoi(argv[++s]);
        }
        else if(sel=="-n")
        {
            nevents=atoi(argv[++s]);
        }
        else if(sel=="-do_modc")
        {
            data_type=argv[++s];
            modc_thr=atof(argv[++s]);
        }
        else 
        {
            cout<<"can not understand: "<<sel<<endl;
            Help();
            return 0;
        }

    }

    if(fcal==""||finame==""||foname=="")
    {
        Help();
        return 0;
    }
    TFile fFile(finame.Data());

    if(!fFile.IsOpen())
    {
        Help();
        return 0;
    }





    DetResponse *detRes=new DetResponse();
    detRes->LoadResponseMatrixFrom(fcal);

    TTree *tin=(TTree*)fFile.Get("polar");
    t2polar *tp=new t2polar(tin);
    TFile *fout=new TFile(foname.Data(),"recreate");
    t2polar *to=new t2polar(NULL);

    if(sim_smearing)
    {
        cout<<"it will simulate energy smearing"<<endl;
    }
    if(sim_xtalk)
    {
        cout<<"it will simulate crosstalk"<<endl;
    }
    if(sim_trigger)
    {
        cout<<"it will simulate trigger"<<endl;
    }



    TH1F *h1xi=new TH1F("h1xi","Azimuthal Angle; Azimuthal Angle (degree);Counts",40,0,360);

    long nevt=tin->GetEntries();
    cout<<"events in the tree:" <<nevt<<endl;
    if(nevents==0)nevents=nevt;

    cout<<"events to be processed:"<<nevents<<endl;

    fout->cd();
    int one_per=nevents/100;

    for(long i=0;i<nevents;i++)
    {
        tp->tree->GetEntry(i);

        if(i%one_per==0)cout<<"Processing: "<<i<<"/"<<nevents<<endl;

        to->SetLeavesFrom(tp);
        if(sim_smearing)
        {
            detRes->SmearEnergy(tp->pm2, tp->pm3); 
        }
        if(sim_xtalk)
        {
            detRes->SimCrosstalkAndNonuniformity(tp->pm3, tp->adc); 
        }
        if(sim_trigger)
        {
            detRes->SimTrigger(tp->adc,tp);
        }

        if(do_modc)
        {
            float *data;
            if(data_type=="pm")data=tp->pm;
            else if(data_type=="pm1")data=tp->pm1;
            else if(data_type=="pm2")data=tp->pm2;
            else if(data_type=="pm3")data=tp->pm3;
            else if(data_type=="adc")data=tp->adc;
            float xideg=tp->GetAzimuthalAngle(data,modc_thr);
            if(xideg>=0)h1xi->Fill(xideg);
        }

        bool pass=true;
        pass=detRes->PassPrescaling(tp,pre_s,pre_c);
        fout->cd();
        if(pass)to->tree->Fill();
    }

    fout->cd();
    to->tree->Write();
    detRes->WriteResponses(fout);
    h1xi->Write();

    return 0;
} 
