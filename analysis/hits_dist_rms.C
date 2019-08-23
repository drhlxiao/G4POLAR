
#include <iostream>
#include <string>    
#include <TApplication.h> 
#include <TROOT.h> 
#include <TFile.h>  
#include <TTree.h>  
#include <TCanvas.h>  
#include <TEventList.h>  
#include <TF1.h>  
#include "lib_det.h"
#include <TH1F.h> 
#include <TH2F.h> 
using namespace std; 
void Help()
{
	cout<<"A program to inspect merged polar data"<<endl;
	cout<<"Usage: spead -i <input > -o <output>  -thr threshold  "<<endl;
}
float get_bar_dist(int bar1,int bar2,float &x0,float &y0, float &x1,float &y1)
{
	get_bar_center(x0,y0,bar1);
	get_bar_center(x1,y1,bar2);
	return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}
float get_dist_rms(Float_t *pm, float thr, float &mx, float &my)
{
	float x0,y0;
	float sx, sy;
	sx=sy=0;
	int nhits=0;
	for(int i=0;i<1600;i++)
	{
		if(pm[i]<thr)continue;
		get_bar_center(x0,y0,i);

		sx+=x0;
		sy+=y0;
		nhits++;
	}
	mx=sx/nhits;
	my=sy/nhits;
	if(nhits<3)return -1;

	Float_t dist=0;
	for(int i=0;i<1600;i++)
	{
		if(pm[i]<thr)continue;
		get_bar_center(x0,y0,i);
		dist+=((x0-mx)*(x0-mx)+(y0-my)*(y0-my));
	}
	return	sqrt(dist/nhits);
}
int main(int argc, char *argv[])
{

	if(argc<2)
	{
		Help();
		return 0;
	}


	TString filename;
	TString ofilename;
	int s=1;
	TString opt,tstr;
	//for test only
	float thr=15;
	while (s<argc) 
	{
		opt=argv[s];

		if( opt=="-h")
		{
			Help();
			exit(0);
		}
		else if(opt=="-thr")
		{
			thr=atof(argv[++s]);
			++s;
		}
		else if(opt=="-i")
		{
			filename=argv[++s];
			++s;
		}
		else if(opt=="-o")
		{
			ofilename=argv[++s];
			++s;
		}
		else{
			cout<<"can not understand:"<<opt<<endl;
			Help();
			return 0;
		}

	}



	if(filename==""||ofilename=="")
	{
		cout<<"no input or output!"<<endl;
		Help();
		return 0;
	}
	TFile f(filename.Data());
	cout<<"reading file:"<<filename.Data()<<endl;
	TTree *t=(TTree*)f.Get("polar");

	Bool_t pattern[1600];
	Float_t pm[1600];
	Short_t nhits;
	//int pair_mark[1600];
	//float open_angle;
	//cout<<"Cut:"<<ccut.GetTitle()<<endl;
//	if(cut!="")t->Draw(">>elist",ccut);
	
//	TEventList *elist = (TEventList*)gDirectory->Get("elist");
//	t->SetEventList(elist);

	//t->SetEventList(elist);
	int n;
	t->SetBranchStatus("*",0);
	t->SetBranchStatus("pm",1);
	t->SetBranchAddress("pm",pm);
	//t->SetBranchAddress("unix_time",&unix_time);
	//t->SetBranchAddress("pair_mark",pair_mark);
	//t->SetBranchAddress("open_angle",&open_angle);
	int nbtot=t->GetEntries();
	TFile fo(ofilename.Data(),"recreate");

	TH1F hdis_twomax("hdis_twomax","distance between the two maximum energy deposition bars",450,0,450); 
	TH2F h1max_pos("h1max_pos","max energy deposition bar position",40,0,40,40,0,40); 
	TH2F h2max_pos("h2max_pos","max energy deposition bar position",40,0,40,40,0,40); 
	TH1F hdisRMS("hdisRMS","RMS distance from barycentre",100,0,450); 
	TH2F h2center("h2center","hit center",100,-200,200,100,-200,200); 

	TCanvas c1;
	Float_t max, sec_max;
	Int_t  mi, smi;
	float x0,x1,y0,y1;
	float rms;
	for(int i=0;i<nbtot;i++)
	{
		t->GetEntry(i);
		if(i%10000==0)cout<<i<<"/"<<nbtot<<endl;


		//	GetTwoMax(pm,1600,max, mi, sec_max,smi);
		//	float dist=	get_bar_dist(mi,smi,x0,y0,x1,y1);
		//	hdis_twomax.Fill(dist);
		//	h1max_pos.Fill(x0,y0);
		//	h2max_pos.Fill(x1,y1);

			rms=get_dist_rms(pm,thr, x0,y0);
			if(rms>0){
				hdisRMS.Fill(rms);
				h2center.Fill(x0,y0);
			}
		 
	}

	fo.cd();
	hdis_twomax.Write();
	h1max_pos.Write();
	h2max_pos.Write();
	hdisRMS.Write();
	h2center.Write();

	cout<<"output:"<<ofilename.Data()<<endl;
	return 0;
} 

