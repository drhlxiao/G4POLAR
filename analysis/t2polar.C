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
using namespace std;
// ----------------------------
t2polar::t2polar(TTree *t)
{

    if(t)
    {
        Init(t);
        return;
    }
    tree = new TTree("polar","polar simulation data");	
    tree->Branch("pattern",pattern,Form("pattern[%d]/I",1600));      ///over threshold from VA
    tree->Branch("pm",pm,Form("pm[%d]/F",1600));                     ///primary energy deposition
    /*
       tree->Branch("pm1",pm1,Form("pm1[%d]/F",1600));                  ///energy deposition after quenching
       tree->Branch("pm2",pm2,Form("pm2[%d]/F",1600));                  ///collected energy deposition, Merlin's simulation results will be used
       tree->Branch("pm4",pm4,Form("pm4[%d]/F",1600));                  ///resevered
       */
    tree->Branch("pm3",pm3,Form("pm3[%d]/F",1600));                  ///
    tree->Branch("adc",adc,Form("adc[%d]/F",1600));                  ///energy deposition after considering energy resolution smearing
    //  tree->Branch("evnt",&evnt,"evnt/L");                             ///event number
    //   tree->Branch("n",&n,"n/I");                                      ///number of hit bars per photon (bars above software threshold)
    //   tree->Branch("npairs",&npairs,"npairs/I");                                      ///number of hit bars per photon (bars above software threshold)
    tree->Branch("time",&time,"time/D");                             /// event time
    tree->Branch("xideg",&xideg,"xideg/D");                          /// azimuthal angle
    tree->Branch("xideg2",&xideg2,"xideg2/D");                          /// azimuthal angle

    tree->Branch("particle_position",particle_position,"particle_position[3]/D");    ///primary particle position 
    tree->Branch("particle_direction",particle_direction,"particle_direction[3]/D");  ///primary particle direction 
    tree->Branch("particle_pol",particle_pol,"particle_pol[3]/D");    ///primary particle polarization
    tree->Branch("particle_energy",&particle_energy,"particle_energy/D"); ///primary particle energy 
    tree->Branch("accepted",accepted,"accepted[25]/I");                                      ///accepted
    tree->Branch("tout1",tout1,"tout1[25]/I");                      ///tout1                
    //  tree->Branch("pair_mark",pair_mark,"pair_mark[1600]/I");                      ///tout1                
    tree->Branch("tout2",tout1,"tout2[25]/I");                      ///tout2                        
    tree->Branch("tmth",tmth,"tmth[25]/I");                         ///too many too high           
    tree->Branch("recorded",&recorded,"recorded/I");                ///is this event recorded by POLAR                    
    //   tree->Branch("bar_maxedep",&bar_maxedep,"bar_maxedep/I");            ///the bar with the maximum energy deposition
    //   tree->Branch("bar_secmaxedep",&bar_secmaxedep,"bar_secmaxedep/I");   ///the bar with the second maximum energy deposition
    //   tree->Branch("first_bar",&first_bar,"first_bar/I");   ///the fist bar interact with the particle
    //   tree->Branch("distance_source_maxbar",&distance_source_maxbar,"distance_source_maxbar/F");   
    ///distance between the source and the bar with maximum energy deposition

    //track
    //   tree->Branch("track_bar",track_bar,"track_bar[5]/I");          
    ///corresponding bar number of the first 5  interactions                      
    //  tree->Branch("track_process",track_process,"track_process[5]/I");                                     
    ///corresponding physics process of the first 5  interactions                      
    //    tree->Branch("track_edep",track_edep,"track_edep[5]/F");                                     
    ///corresponding energy deposition of the first 5  interactions                      


    //   tree->Branch("open_angle",&open_angle,"open_angle/F");               
    //  tree->Branch("pair_dist",&pair_dist,"pair_dist/F");          ///     distance between two hit pairs
    // tree->Branch("pair_dist1",&pair_dist1,"pair_dist1/F");          ///     distance between two hit pairs
    //tree->Branch("pair_dist2",&pair_dist2,"pair_dist2/F");          ///     distance between two hit pairs

    tree->Branch("npairs",&npairs,"npairs/I");                                      ///number of hit bars per photon (bars above software threshold)
    tree->Branch("bar1",bar1,"bar1[npairs]/I");          ///    
    tree->Branch("bar2",bar2,"bar2[npairs]/I");          ///     

    tree->Branch("open_angle",open_angle,"open_angle[npairs]/F");               
    tree->Branch("pair_dist",pair_dist,"pair_dist[npairs]/F");          ///     distance between two hit pairs
    tree->Branch("pair_dist1",pair_dist1,"pair_dist1[npairs]/F");          /// 
    tree->Branch("pair_dist2",pair_dist2,"pair_dist2[npairs]/F");          ///  
    tree->Branch("tot_angle",tot_angle,"tot_angle[npairs]/F");          ///     
    tree->Branch("dist_source",dist_source,"dist_source[npairs]/F");          ///    

    tree->Branch("bar1",bar1,"bar1[npairs]/I");          ///    
    tree->Branch("bar2",bar2,"bar2[npairs]/I");          ///     


    evnt = 0;
    time=0;

}
t2polar::~t2polar()
{
    delete tree;
}
void t2polar::Init(TTree *t)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!t) return;
    tree = t;

    /*
       tree->SetBranchAddress("pattern", pattern, &b_pattern);
       tree->SetBranchAddress("pm", pm, &b_pm);
       tree->SetBranchAddress("pm1", pm1, &b_pm1);
       tree->SetBranchAddress("pm2", pm2, &b_pm2);
       tree->SetBranchAddress("pm4", pm4, &b_pm4);
       tree->SetBranchAddress("pm3", pm3, &b_pm3);
       tree->SetBranchAddress("adc", adc, &b_adc);
       tree->SetBranchAddress("evnt", &evnt, &b_evnt);
       tree->SetBranchAddress("n", &n, &b_n);
       tree->SetBranchAddress("time", &time, &b_time);
       tree->SetBranchAddress("xideg", &xideg, &b_xideg);
       tree->SetBranchAddress("xideg2", &xideg2, &b_xideg2);
       tree->SetBranchAddress("particle_position", particle_position, &b_particle_position);
       tree->SetBranchAddress("particle_direction", particle_direction, &b_particle_direction);
       tree->SetBranchAddress("particle_pol", particle_pol, &b_particle_pol);
       tree->SetBranchAddress("particle_energy", &particle_energy, &b_particle_energy);
       tree->SetBranchAddress("accepted", accepted, &b_accepted);
       tree->SetBranchAddress("tout1", tout1, &b_tout1);
       tree->SetBranchAddress("tout2", tout2, &b_tout2);
       tree->SetBranchAddress("tmth", tmth, &b_tmth);
       tree->SetBranchAddress("recorded", &recorded, &b_recorded);
       tree->SetBranchAddress("bar_maxedep", &bar_maxedep, &b_bar_maxedep);
       tree->SetBranchAddress("bar_secmaxedep", &bar_secmaxedep, &b_bar_secmaxedep);
       tree->SetBranchAddress("first_bar", &first_bar, &b_first_bar);
       tree->SetBranchAddress("distance_source_maxbar", &distance_source_maxbar, &b_distance_source_maxbar);
       tree->SetBranchAddress("track_bar", track_bar, &b_track_bar);
       tree->SetBranchAddress("track_process", track_process, &b_track_process);
       tree->SetBranchAddress("track_edep", track_edep, &b_track_edep);
       tree->SetBranchAddress("open_angle", &open_angle, &b_open_angle);
       tree->SetBranchAddress("pair_dist", &pair_dist, &b_pair_dist);
       tree->SetBranchAddress("pair_dist1", &pair_dist1, &b_pair_dist1);
       tree->SetBranchAddress("pair_dist2", &pair_dist2, &b_pair_dist2);
       */
}
void t2polar::SetLeavesFrom(t2polar *ti)
{


    evnt=ti->evnt;
    time=ti->time;
    npairs=ti->npairs;
    xideg=ti->xideg;
    xideg2=ti->xideg2;
    n = ti->n;
    bar_maxedep=ti->bar_maxedep;
    bar_secmaxedep=ti->bar_secmaxedep;
    distance_source_maxbar=ti->distance_source_maxbar;
    first_bar=ti->first_bar;


    for( Int_t i = 0; i<1600; ++i )
    {
        pattern[i] = ti->pattern[i];  
        pm[i] = ti->pm[i];       
        pm1[i] =ti->pm1[i]  ;       
        pm2[i] =ti->pm2[i]  ;       
        pm4[i] =ti->pm4[i]  ;       
        pm3[i] =ti->pm3[i]  ;       
        adc[i] =ti->adc[i]  ;       

    }
    particle_energy=ti->particle_energy;
    particle_position[0]=ti->particle_position[0];
    particle_position[1]=ti->particle_position[1];
    particle_position[2]=ti->particle_position[2];

    particle_direction[0]=ti->particle_direction[0];
    particle_direction[1]=ti->particle_direction[1];
    particle_direction[2]=ti->particle_direction[2];
    particle_pol[0]=particle_pol[0];
    particle_pol[1]=particle_pol[1];
    particle_pol[2]=particle_pol[2];

    recorded=ti->recorded;
    for( Int_t i = 0; i<25; ++i )
    {
        accepted[i]=ti->accepted[i]; ///if the trigger of this module is accepted by CT
        tout1[i]=ti->tout1[i];  ///tout1
        tout2[i]=ti->tout2[i]; ///tout2
        tmth[i]=ti->tmth[i]; ///too many or too high
    }
    for(int i=0;i<5;i++)
    {
        track_edep[i]=ti->track_edep[i];
        track_process[i]=ti->track_process[i];
        track_bar[i]=ti->track_bar[i];
    }
    for(int i=0;i<NPAIRMAX;i++)
    {

        bar1[i]=ti->bar1[i];
        bar2[i]=ti->bar2[i];
        open_angle[i]=ti->open_angle[i];
        tot_angle[i]=ti->tot_angle[i];
        dist_source[i]=ti->dist_source[i];
        pair_dist2[i]=ti->pair_dist2[i];
        pair_dist1[i]=ti->pair_dist1[i];
        pair_dist[i]=ti->pair_dist[i];

    }

}

void t2polar::DefaultValues()
{
    npairs=0;
    xideg=-100;
    xideg2=-100;
    n = 0;
    bar_maxedep=-1;
    bar_secmaxedep=-1;
    distance_source_maxbar=0;
    first_bar=-1;
    particle_energy=0;
    particle_position[0]=particle_position[1]=particle_position[2]=0;
    particle_direction[0]=particle_direction[1]=particle_direction[2]=0;

    for( Int_t i = 0; i<1600; ++i )
    {
        pattern[i] = 0;  
        pm[i] = 0;       
        pm1[i] = 0;       
        pm2[i] = 0;       
        pm4[i] = 0;       
        pm3[i] = 0;       
        adc[i] = 0;       
    }
    recorded=0;
    for( Int_t i = 0; i<25; ++i )
    {
        accepted[i]=0; ///if the trigger of this module is accepted by CT
        tout1[i]=0;  ///tout1
        tout2[i]=0; ///tout2
        tmth[i]=0; ///too many or too high
    }
    for(int i=0;i<5;i++)
    {
        track_bar[i]=-1;
        track_edep[i]=0;
    }

}

//for the quick look of modulation curve in simulation
void t2polar::GetMaximum(const float *data, float &firstmax, int &bar_sec, float &secmax, int &idx2)
{
    double max=0;
    double second_max=0;

    for(int i=0;i<1600;i++)
    {  
        if(data[i]>max){  
            idx2=bar_sec;
            second_max=max;  
            max=data[i];            
            bar_sec=i;
        }
        else if (data[i] > second_max && data[i] != max)
        {
            second_max = data[i];
            idx2=i;
        }
    }
    firstmax=max;
    secmax=second_max;
}

float t2polar::GetAzimuthalAngle(const float *data, double e_thr)
{
    float pmax0,pmax1;

    GetMaximum(data, pmax0,bar_maxedep,pmax1,bar_secmaxedep);

    distance_source_maxbar=GetDistance(particle_position[0],particle_position[1],bar_maxedep);
    xideg2=GetScaterAzimuthalAngle(particle_position[0],particle_position[1],bar_maxedep,bar_secmaxedep);

    int row0,row1,col0,col1;
    row0=get_row(bar_maxedep);
    col0=get_column(bar_maxedep);
    if(pmax1<e_thr)return -100;
    row1=get_row(bar_secmaxedep);
    col1=get_column(bar_secmaxedep);
    float xrnd0,yrnd0;
    float xrnd1,yrnd1;
    get_rand_pos(xrnd0,yrnd0,row0,col0);
    get_rand_pos(xrnd1,yrnd1,row1,col1);
    xideg=-100;
    if(! AreClosestNeighbours(row0,row1,col0,col1))
    {
        xideg = (float)GetXi(xrnd0,yrnd0,xrnd1,yrnd1); 
    }
    return xideg;
}

TString t2polar::GetPhysicsProcessName(int id)
{
    TString name; 
    switch(id)
    {
        case 1:
            name="compt";break;
        case 2:
            name="pol-comp";break;
        case 3:
            name="phot";break;
        case 4:
            name="conv";break;
        case 5:
            name="Rayl";break;
        default:
            name="unknown";
    }
    return name;

}

int t2polar::GetPhysicsProcessID(TString processName)
{
    int pid=0;
    if(processName=="compt")pid=1;
    else if(processName=="pol-compt")pid=2;
    else if(processName=="phot")pid=3;
    else if(processName=="conv")pid=4;
    else if(processName=="Rayl")pid=5;

    return pid;

}
void t2polar::SetPhysicsProcess(Int_t ntrack, TString processName)
{
    track_process[ntrack]=GetPhysicsProcessID(processName);
}

float t2polar::GetScaterAzimuthalAngle(float x0,float y0, int bar_max, int bar_sec)
{
    float x1,y1,x2,y2;
    get_bar_center(x1,y1,bar_max);
    get_bar_center(x2,y2,bar_sec);

    TVector3 v1(x1-x0,y1-y0,0);
    TVector3 v2(x1-x2,y1-y2,0);
    return v1.Angle(v2)*TMath::RadToDeg(); //between 0-180

}

float t2polar::GetDistance(float x0,float y0,int bar)
{
    float x1,y1;
    get_bar_center(x1,y1,bar);
    return TMath::Sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));

}



int t2polar::FindCoincidencePairs(float thr,
        float sourcex, float sourcey)
{


    float dist1;
    float dist2;


    vector <int> vbars;
    int nhits=0;
    for(int i=0;i<1600;i++)
    {
        if(pattern[i])
        {
            vbars.push_back(i);
            nhits++;
        }
    }

    if(nhits<2)return 0;

    int mark=0;
    Float_t  dx1,dx2,dy1,dy2;
    Float_t  px1,px2,py1,py2;
    int bar1,bar2;
    Float_t tolerant1, tolerant2;

    int num_pairs=0;
    //for coincidence 

    for(int i=0;i<nhits;i++)
        for(int j=i+1;j<nhits;j++)
        {
            bar1=vbars[i];
            bar2=vbars[j];
            //            cout<<bar1<<" "<<bar2<<endl;
            get_bar_center(px1,py1,bar1);
            get_bar_center(px2,py2,bar2);
            //cout<<"bar1,bar2:"<<bar1<<" "<<bar2<<endl;
            dx1=px1-sourcex;
            dx2=px2-sourcex;
            dy1=py1-sourcey;
            dy2=py2-sourcey;

            dist1=TMath::Sqrt(dx1*dx1+dy1*dy1);
            dist2=TMath::Sqrt(dx2*dx2+dy2*dy2);

            TVector3 v1(dx1,dy1,0);
            TVector3 v2(dx2,dy2,0);
            Float_t open_angle=v1.Angle(v2)*TMath::RadToDeg(); //between 0-180
            Float_t px0=sourcex;
            Float_t py0=sourcey;

            Float_t distsource=TMath::Abs((py2-py1)*px0-(px2-px1)*py0-px1*py2+px2*py1)/TMath::Sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));

            bool are_same_region=true;
            bool is_dist_good=false;

            if( AreInSameGroup(bar1,bar2))are_same_region=true;
            else are_same_region=false;


            if(open_angle>120&&distsource<=5.0)is_dist_good=true;
            //angle_cut=120

            if(are_same_region&&is_dist_good)
            {


                pair_dist1[num_pairs]=dist1;
                pair_dist2[num_pairs]=dist2;
                pair_dist[num_pairs]=TMath::Sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));
                dist_source[num_pairs]=distsource;

                coincidence[bar1]++;
                coincidence[bar2]++;

                mark++;
                num_pairs++;

            }

        }

    npairs=num_pairs;

    return num_pairs;
}

bool t2polar::AreInSameGroup(int b1,int b2)
{
    int row1,row2,col1,col2;
    row1=get_row(b1);
    row2=get_row(b2);
    col1=get_column(b1);
    col2=get_column(b2);

    bool rowsame=(row1-20)*(row2-20)>=0?true:false;
    bool colsame=(col1-20)*(col2-20)>=0?true:false;
    return rowsame&&colsame;

}
