
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "t2polar.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TVector3.h"
#include "lib_det.h"

#include "Coincidence.h"
using namespace std;
Coincidence::Coincidence()
{
    num_events=0;
    n_sources=0;
    threshold=5;
}
Coincidence::~Coincidence()
{
   // delete ti;
   // delete to;
    //delete treein;
}

void Coincidence::DoCoincidence(TString ifname, TString ofname)
{
    fi=new TFile(ifname.Data());
    if(!fi->IsOpen())
    {
        cout<<"can not open the root file"<<endl;
        return;
    }

    treein=(TTree*)fi->Get("polar");

    ti=new t2polar(treein);

    if(!ti)
    {
        cout<<"can not load the root tree t2polar"<<endl;
        return;
    }

    fo=new TFile(ofname.Data(),"recreate");
    to=new t2polar();
    n_sources=source_x.size();
    cout<<"Number of sources:"<<n_sources<<endl;
    if(n_sources<=0)
    {
        cout<<"source information is not loaded"<<endl;
        return;
    }
    cout<<"Positions:"<<endl;
    
    for(int j=0;j<n_sources;j++)
    {
        cout<<source_x[j]<<" "<<source_y[j]<<endl;
    }
    cout<<"Threshold:"<<threshold<<endl;
    cout<<"Distances: "<<distance_lowlimit<<" "<<distance_highlimit<<endl;
    int nevt=0;
     
    int npairs;

    TH2F h2("h2","coincidence hits distribution",40,0,40,40,0,40);
    if(num_events==0)num_events=treein->GetEntries();
    cout<<"Number of events: "<<num_events<<endl;

    for(long i=0;i<num_events;i++)
    {
         
        if(i%10000==0)cout<<i<<endl;
        ti->tree->GetEntry(i);
        int pairsum=0;
         

        to->SetLeavesFrom(ti);
        
        for(int j=0;j<n_sources;j++)
        {
            npairs=FindCoincidencePairs(ti,to, threshold,distance_highlimit,distance_lowlimit, source_x[j], source_y[j], &h2);;
            pairsum+=npairs;
        }
        if(pairsum>0)
        {
            nevt++;
        }
               
        to->tree->Fill();
        
    }
    h2.Write();
    cout<<"number of coincidence events: "<<nevt<<endl;
    fo->cd();
    to->tree->Write();
}

void  Coincidence::AddASource(float x, float y)
{
    cout<<"adding a source with position: "<<x<<" "<<y<<endl;
    source_x.push_back(x);
    source_y.push_back(y);
}

void Coincidence::LoadSettings(TString filename)
{
    ifstream infile(filename.Data()); 

     if(!infile.good())
     { 
          cout<<"can not open file which contains source information..."<<endl; 
           return ;
     } 
     float x, y;
     string line;
     while(std::getline(infile, line))
     {
         std::istringstream iss(line);
         iss>>x>>y;
         AddASource(x,y);
         cout<<"add source position: "<<x<<" "<<y<<endl;
     }
     infile.close(); 
}

int Coincidence::FindCoincidencePairs(t2polar *tin, t2polar *tout,float thr,float dist_highlimit,float dist_lowlimit, float sourcex, float sourcey,TH2F *h)
{


      float open_angle; 
      float dist1;
      float dist2;
    
    
    vector <int> vbars;
    int nhits=0;
    bool sel=false;
    for(int i=0;i<1600;i++)
    {
        sel=false;

        if(data_type=="pattern")
        {
            if(tin->pattern[i])
            {
                vbars.push_back(i);
                nhits++;
            }
        }
        else 
        {
            if(data_type=="pm")sel=tin->pm[i]>thr;
            else if(data_type=="pm1")sel=tin->pm1[i]>thr;
            else if(data_type=="pm2")sel=tin->pm2[i]>thr;
            else if(data_type=="pm3")sel=tin->pm3[i]>thr;
            else if(data_type=="adc")sel=tin->adc[i]>thr;
            
            if(sel)
            {
                    vbars.push_back(i);
                    nhits++;
            }
        }


        tout->pair_mark[i]=0;
    }

    int mark=0;
    Float_t  dx1,dx2,dy1,dy2;
    Float_t  px1,px2,py1,py2;
    int bar1,bar2;
    Float_t tolerant1, tolerant2;

    int num_pairs=0;
    

	for(int i=0;i<nhits;i++)
	for(int j=i+1;j<nhits;j++)
	{
        bar1=vbars[i];
        bar2=vbars[j];
        get_bar_center(px1,py1,bar1);
        get_bar_center(px2,py2,bar2);
        dx1=px1-sourcex;
        dx2=px2-sourcex;
        dy1=py1-sourcey;
        dy2=py2-sourcey;

        dist1=TMath::Sqrt(dx1*dx1+dy1*dy1);
        dist2=TMath::Sqrt(dx2*dx2+dy2*dy2);
        if(dist1>dist_highlimit||dist2>dist_highlimit)continue;
        if(dist1<dist_lowlimit||dist2<dist_lowlimit)continue;

        tolerant1=TMath::ATan((1.414*6.08*0.5/dist1))*TMath::RadToDeg();
        tolerant2=TMath::ATan((1.414*6.08*0.5/dist2))*TMath::RadToDeg();
        TVector3 v1(dx1,dy1,0);
        TVector3 v2(dx2,dy2,0);

        Float_t tol=tolerant1+tolerant2;
        open_angle=v1.Angle(v2)*TMath::RadToDeg(); //between 0-180

        if(open_angle>180-tol)
        {

            mark++;
            num_pairs++;
            tout->pair_mark[bar1]++;
            tout->pair_mark[bar2]++;

            h->Fill(get_column(bar1), get_row(bar1));
            h->Fill(get_column(bar2), get_row(bar2));
            tout->pair_dist1=dist1;
            tout->pair_dist2=dist2;

        }
        else
        {
            open_angle=0;
        }
        
    }
    tout->open_angle=open_angle;
    tout->npairs=num_pairs;

    return num_pairs;
}
