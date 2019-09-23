/***************************************************************
 * Class to define data for full Polar.
 * Author  : N. Gauvin
 * Date    : October 2011
 * Version : 1.10
 * modified by Hualin Xiao, May, 2014
 ***************************************************************/
#include "t2polar.h"
#include "iostream"
#include <cmath>
#include "TMath.h"
#include "TVector3.h"
#include "TRandom.h"
#include "lib_det.h"
#include "assert.h"
#include <algorithm>
using namespace std;
// ----------------------------



t2polar::~t2polar()
{
	delete tree;
}

t2polar::t2polar(bool rs)
{

	realsim=rs;
	nrecorded=0;

	tree = new TTree("polar","polar simulation data");	

	tree->Branch("pattern",pattern,"pattern[1600]/O");      ///software trigger
	tree->Branch("trigger",trigger,"trigger[1600]/O");      ///hardware trigger
	tree->Branch("pm",pm,"pm[1600]/F");                  ///energy deposition after quenching


	if(realsim)
	{
		tree->Branch("pm3",pm3,"pm3[1600]/F");
		tree->Branch("thr",thr,"thr[1600]/F");
		//   tree->Branch("adc",adc,Form("adc[%d]/F",1600));                  ///
	}

	tree->Branch("nhits",&nhits,"nhits/I");                                      ///number of hit bars per photon (bars above software threshold)
	tree->Branch("ntrig",&ntrig,"ntrig/I");                             /// event time
	tree->Branch("time",&time,"time/D");                             /// event time
	tree->Branch("source_pos",source_pos,"source_pos[3]/F");    ///primary particle position 
	tree->Branch("source_dir",source_dir,"source_dir[3]/F");  ///primary particle direction 
	tree->Branch("source_pol",source_pol,"source_pol[3]/F");    ///primary particle polarization
	tree->Branch("source_ekin",&ekin,"source_ekin/F"); ///primary particle energy 




	tree->Branch("accepted",accepted,"accepted[25]/I");             ///accepted
	tree->Branch("tout1",tout1,"tout1[25]/I");                      ///tout1                 trigger simulation
	tree->Branch("tout2",tout1,"tout2[25]/I");                      ///tout2                       trigger simulation output using real experiment data
	tree->Branch("tmth",tmth,"tmth[25]/I");                         ///too many too high           


	tree->Branch("mhits",mhits,"mhits[25]/I");                         ///number of hits
	//number of hits above the software threshold

	tree->Branch("recorded",&recorded,"recorded/I");                ///is this event recorded by POLAR                    
	//detector response simulation output
	//trigger descision after prescalling



	tree->Branch("theta",&theta,"theta/F");                             /// theta  with respect to -z
	tree->Branch("phi",&phi,"phi/F");                             /// incident angle
	tree->Branch("polang",&polang,"polang/F");                             //polarization angle




	tree->Branch("xideg",xideg,"xideg[2]/F");                          /// azimuthal angle
	tree->Branch("xiok",xiok,"xiok[2]/O");                          /// azimuthal angle is ok
	tree->Branch("max_bar",max_bar, "max_bar[2]/I");     //first bar number 
	tree->Branch("sec_bar",sec_bar, "sec_bar[2]/I");  //second bar number
	tree->Branch("max_dep",max_dep,"max_dep[2]/F");  //the first bar energy deposition
	tree->Branch("sec_dep",sec_dep,"sec_dep[2]/F");  // second bar energy deposition
	tree->Branch("bar_dist",bar_dist, "bar_dist[2]/F");  //bar distance 
	//tree->Branch("bar_gap",bar_gap, "bar_gap[2]/F");
	tree->Branch("same_module",same_module, "same_module[2]/I"); //if the first two bars in the same module


	//tracking 
	tree->Branch("track_bar",track_bar, "track_bar[5]/I");   //bar number of tracks
	tree->Branch("track_dist",track_dist, "track_dist[5]/F"); //hits distance
	tree->Branch("track_phys",track_phys, "track_phys[5]/I");  //physics proce
	tree->Branch("track_deltaE",track_deltaE, "track_deltaE[5]/F"); //deleta energy deposition
	evnt = 0;
	time=0;


}

void t2polar::FillTriggerMap(TH2F *h2, TH2F *h2record)
{
	int row,col;
	for(int i=0;i<1600;i++)
	{
		if(trigger[i])
		{
                row=get_row(i);
                col=get_column(i);
                h2->Fill(col+0.5,row+0.5);
				if(recorded)h2record->Fill(col+0.5,row+0.5);
		}
	}
	if(recorded)nrecorded++;

}
void t2polar::FillPolarization(float mcthr)
{
//bool t2polar::GetAzimuthalAngle(const float *data, Float_t &edep1, Float_t &edep2, 
//		Int_t &bar1, Int_t &bar2, Float_t &dist, Float_t &xi, Int_t &module)
	GetAzimuthalAngle(pm, max_dep[0], sec_dep[0], max_bar[0], sec_bar[0], bar_dist[0], xideg[0], same_module[0]);
	//No threshold was applied to this step

	if(xideg[0]>=0&&sec_dep[0]>=mcthr &&(bar_dist[0]>=10||same_module[0]==-1 ))
	{
		xiok[0]=true;
	}
	if(realsim){
		GetAzimuthalAngle(pm3, max_dep[1], 
				sec_dep[1], max_bar[1],
				sec_bar[1], bar_dist[1], xideg[1], same_module[1]);
		int mbar1=max_bar[1];
		int mbar2=sec_bar[1];
		if(mbar1>=0&&mbar2>=0)
		{
			if(trigger[mbar1]&&trigger[mbar2])
			{
				if(xideg[1]>=0&&(bar_dist[1]>=10 ||same_module[1]==-1))
				{
					//accept all triggered bars
					xiok[1]=true;
				}
			}
		}
	}
}

void t2polar::DefaultValues()
{
	fill_n(track_bar,5,-100); //bar of the first 5 interactions 
	fill_n(track_dist,5,-100); //bar of the first 5 interactions 
	fill_n(track_phys,5,0); //bar of the first 5 interactions 
	fill_n(track_deltaE,5,0); //bar of the first 5 interactions 


	phi_scan=pol_scan=-1;
	phi_scan_step=pol_scan_step=0;
	
	
	fill_n(pattern, 1600,false);
	fill_n(trigger, 1600,false);
	fill_n(pm, 1600,0);
	fill_n(pm1, 1600,0);
	fill_n(pm2, 1600,0);
	fill_n(pm3, 1600,0);
	fill_n(adc, 1600,0);
	fill_n(thr, 1600,0);
	nhits=ntrig=time=ekin=0;
	theta=phi=polang=0;
	recorded=0;

	fill_n(source_pos,3,0);
	fill_n(source_dir,3,0);
	fill_n(source_pol,3,0);

	fill_n(accepted,25,0);
	fill_n(tout1,25,0);
	fill_n(tout2,25,0);
	fill_n(tmth,25,0);
	fill_n(mhits,25,0);

	fill_n(xideg,2,-100);
	fill_n(xiok,2,false);
	fill_n(max_bar,2,-100);
	fill_n(sec_bar,2,-100);
	fill_n(max_dep,2,-100);
	fill_n(sec_dep,2,-100);
	fill_n(bar_dist,2,-100);
	//fill_n(bar_gap,2,0);
	fill_n(same_module,2,-1);

}

//for the quick look of modulation curve in simulation
void t2polar::GetMaximum(const float *data, 
		int &firstbar, float &depmax, int &secbar, float &depsec)
{
	for(int i=0;i<1600;i++)
	{  
		if(data[i]>depmax){  
			secbar=firstbar;
			depsec=depmax;  
			depmax=data[i];            
			firstbar=i;
		}
		else if (data[i] > depsec&& data[i] != depmax)
		{
			depsec= data[i];
			secbar=i;
		}
	}
}
bool t2polar::GetAzimuthalAngle(const float *data, Float_t &edep1, Float_t &edep2, 
		Int_t &bar1, Int_t &bar2, Float_t &dist, Float_t &xi, Int_t &module)
{

	if(nhits<2)return false;

	GetMaximum(data, bar1, edep1,bar2,edep2);
	dist=GetDistance(bar1,bar2);
	//G4cout<<"distance:"<<dist<<G4endl;

	int row0,row1,col0,col1;
	row0=get_row(bar1);
	col0=get_column(bar1);
	row1=get_row(bar2);
	col1=get_column(bar2);
	float xrnd0,yrnd0;
	float xrnd1,yrnd1;
	get_rand_pos(xrnd0,yrnd0,row0,col0);
	get_rand_pos(xrnd1,yrnd1,row1,col1);
	xi= (float)GetXi(xrnd0,yrnd0,xrnd1,yrnd1); 
	int module1=get_pm(bar1);
	int module2=get_pm(bar2);

	module=(module1==module2)?-1:module2;

	return true;
}


float t2polar::GetDistance(int bar1, int bar2)
{
	float x1,y1,x2,y2;
	get_bar_center(x1,y1,bar1);
	get_bar_center(x2,y2,bar2);
	return TMath::Sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
int t2polar::GetPhysicsProcessID(TString & processName)
{
    int pid=0;
    if(processName=="compt")pid=1;
    else if(processName=="pol-compt")pid=2;
    else if(processName=="phot")pid=3;
    else if(processName=="conv")pid=4;
    else if(processName=="Rayl")pid=5;
    return pid;
}
void t2polar::FillTrack(int i, int bar,  float totalEdep, TString  processName)
{
	if(i>=5||i<0)return;

	int pid=GetPhysicsProcessID(processName);
	track_bar[i]=bar;
	track_phys[i]=pid;
	track_deltaE[i]=totalEdep;
	if(i>0)
	{
		int prebar=track_bar[i-1];
		if(prebar!=bar)
		{
			float prex,prey;
			float thisx, thisy;
			get_bar_center(thisx,thisy,bar);
			get_bar_center(prex,prey,prebar);
			track_dist[i]=sqrt((thisx-prex)*(thisx-prex)+(thisy-prey)*(thisy-prey));
		}
		else
		{
			track_dist[i]=0;
		}
	}
	
}
