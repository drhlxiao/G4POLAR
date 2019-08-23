
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
    nstore=0;
    num_events=0;
    n_sources=0;
    threshold=5;
    angle_cut=120; //120 degree
    dist_cut=5.; //4.5 mm
    print_hits_position=false;
    data_type="pm";
    high_energy_cut=0;
    is_hit_dist_cut_set=false;
    distance_lowlimit=0;
    distance_highlimit=500;
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
    cout<<"Source Positions:"<<endl;

    for(int j=0;j<n_sources;j++)
    {
        cout<<j<<": "<<source_x[j]<<" "<<source_y[j]<<endl;
    }
    cout<<"Threshold:"<<threshold<<endl;
    if(is_hit_dist_cut_set)cout<<"maximum allowed distance between hit pairs: "<<distance_lowlimit<<" "<<distance_highlimit<<endl;
    else 
    {
        cout<<"only consider hit pairs in the same group"<<endl;
    }
    
    
    int nevt=0;

    int npairs;
    TH1F *hopen=new TH1F("hopen","open angle between any hits", 200, 0, 360); 
    TH1F *hdist=new TH1F("hdist","distance to source hits with open angle >120", 600, 0, 300); 
    TH1F *htol=new TH1F("htol","tolerant angle of any two hit pairs", 200, 0, 360); 
    TH1F *hnmult=new TH1F("hmmult","multiplicity of coincidence pairs", 100, 0, 100); 
    TH1F *hnmult_nozero=new TH1F("hmmult_nozero","multiplicity of coincidence pairs", 100, 0, 100); 

    TH1F *hopen_bad=new TH1F("hopen_bad","open angle between non-coincidence hits", 200, 0, 360); 
    TH1F *hdist_bad=new TH1F("hdist_bad","distance to source hits for non-coincidence hit pairs", 600, 0, 300); 


    TH1F *hopen_good=new TH1F("hopen_good","open angle between coincidence hits", 200, 0, 360); 
    TH1F *hdist_good=new TH1F("hdist_good","distance to source hits for coincidence hit pairs", 600, 0, 300); 
    TH1F *htol_good=new TH1F("htol_good","tolerant angle for coincidence hit pairs", 200, 0, 360); 
    TH1F *hdislim_good=new TH1F("hdislim_good","allow distance limit ", 200, 0, 200); 

    TH2F h2("h2","coincidence hits distribution",40,0,40,40,0,40);
    TH2F hmap("hmap","hits distribution",40,0,40,40,0,40);
    float xmax=900;



    if(data_type=="adc")xmax=50000;
    for(int i=0;i<1600;i++)
    {

        hspec_co[i]=new TH1F(Form("hspec_co%d",i),Form("spectra of coincidence hits ch: %d",i), 500, 0, xmax); 
        hspec_co[i]->SetBit(TH1::kCanRebin);

        hspec[i]=new TH1F(Form("hspec%d",i),Form("spectra of ch: %d",i), 300, 0, xmax); 
        hspec[i]->SetBit(TH1::kCanRebin);
        hspc_adc[i]=new TH1F(Form("hspec_adc%d",i),Form("spectra of coincidence hits ch: %d",i), 1000, 0, xmax); 
    }



    if(num_events==0)num_events=treein->GetEntries();
    cout<<"Number of events: "<<num_events<<endl;
    int ncosmic=0;

    for(long i=0;i<num_events;i++)
    {

        if(i%10000==0)cout<<i<<endl;
        ti->tree->GetEntry(i);
        int pairsum=0;

        to->DefaultValues();

        to->SetLeavesFrom(ti);

        //check cosmic rays;
        int ntriggers=0;
        for(int i=0;i<64;i++)
        {
            if(ti->pattern[i]==1)ntriggers++;
        }

        if(ntriggers>15){ncosmic++; continue;}

        for(int j=0;j<n_sources;j++)
        {
            npairs=FindCoincidencePairs(ti,to, threshold,distance_highlimit,distance_lowlimit, 
                    source_x[j], source_y[j],&hmap, &h2, hopen, hdist,htol, hopen_good, htol_good, hdist_good, hdislim_good, hopen_bad, hdist_bad);
            pairsum+=npairs;
        }
        hnmult->Fill(pairsum);
        if(pairsum>0)hnmult_nozero->Fill(pairsum);

        if(pairsum>0)nevt++;


        if(nstore==0&&pairsum>0)to->tree->Fill();
        if(nstore<0)to->tree->Fill();
        if(nstore>0&&nevt<nstore)to->tree->Fill();

    }

    cmd.Write("command");
    hnmult->Write();
    hnmult_nozero->Write();
    hdist->Write();
    hopen->Write();
    htol->Write();
	hdist_bad->Write();
	hopen_bad->Write();

	hdislim_good->Write();
    hdist_good->Write();
    hopen_good->Write();
    htol_good->Write();
    hmap.Write();
    h2.Write();
    cout<<"--------------------------"<<endl;
    cout<<"Number of events: "<<num_events<<endl;
    cout<<"Number of cosmic rays (ignored): "<<ncosmic<<endl;
    cout<<"number of coincidence events: "<<nevt<<endl;
    fo->cd();
    to->tree->Write();
    fo->mkdir("spectra");
    fo->cd("spectra");

    for(int i=0;i<1600;i++)
    {
        if(hspec_co[i]->GetEntries()>10)hspec_co[i]->Write();
        if(hspec[i]->GetEntries()>10)hspec[i]->Write();
        if(hspc_adc[i]->GetEntries()>10)hspc_adc[i]->Write();
    }

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

int Coincidence::FindCoincidencePairs(t2polar *tin, t2polar *tout,float thr,
        float dist_highlimit,float dist_lowlimit, 
        float sourcex, float sourcey,
        TH2F *hmap, TH2F *h, TH1F *hopen, 
        TH1F *hdist,TH1F *htol, 
        TH1F *hopen_good, TH1F* htol_good,TH1F* hdist_good, TH1F *hdistance_lim, TH1F *hopen_bad, TH1F *hdist_bad)
{


    float dist1;
    float dist2;


    vector <int> vbars;
    int nhits=0;
    for(int i=0;i<1600;i++)
    {
        if(tin->pattern[i]>0)hspec[i]->Fill(tin->pm[i]);

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
            float data;
            if(data_type=="pm")data=tin->pm[i];
            else if(data_type=="pm1")data=tin->pm1[i];
            else if(data_type=="pm2")data=tin->pm2[i];
            else if(data_type=="pm3")data=tin->pm3[i];
            else if(data_type=="adc")data=tin->adc[i];

            bool select=false;
            if(data>thr)select=true;
            if(high_energy_cut>0)
            {
                if(data>high_energy_cut)select=false;
            }

            if(select)
            {
                hmap->Fill(i/40,i%40);
                vbars.push_back(i);
                nhits++;
            }
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



           // tolerant1=TMath::ATan((1.414*6.08*0.5/dist1))*TMath::RadToDeg();
           // tolerant2=TMath::ATan((1.414*6.08*0.5/dist2))*TMath::RadToDeg();
            TVector3 v1(dx1,dy1,0);
            TVector3 v2(dx2,dy2,0);

            //Float_t tol=tolerant1+tolerant2;
           // Float_t px0=sourcex;
           // Float_t py0=sourcey;

            //Float_t distsource=TMath::Abs((py2-py1)*px0-(px2-px1)*py0-px1*py2+px2*py1)/TMath::Sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));

			float distlim;
			float distsource;
            Float_t open_angle=v1.Angle(v2)*TMath::RadToDeg(); //between 0-180
			bool ok=TestGeometry(bar1, bar2, sourcex, sourcey, distsource, distlim,0,5.8);
            //cout<<"to:"<<tol<<" "<<open_angle<<" "<<distsource<<" "<<endl;
			//

            //if(ok) 
            hopen->Fill(open_angle);
			if(open_angle>140)hdist->Fill(distsource);
            //htol->Fill(tol);
			hdistance_lim->Fill(distlim);

			/*
            bool are_same_region=true;
            bool is_dist_good=false;


            if(is_hit_dist_cut_set)
            {
                if(dist1>dist_highlimit||dist2>dist_highlimit)are_same_region=false;
                if(dist1<dist_lowlimit||dist2<dist_lowlimit)are_same_region=false;
            }

            if(!is_hit_dist_cut_set)
            {
                if( AreInSameGroup(bar1,bar2))are_same_region=true;
                else are_same_region=false;
            }

			*/
			/*
            if(are_same_region&&print_hits_position)
            {
                cout<<px1<<" "<<py1<<" "<<px2<<" "<<py2<<" "<<endl;
                continue;
            }
			*/


           // if(open_angle>angle_cut&&distsource<=dist_cut)is_dist_good=true;
            //angle_cut=120

            if(!(ok&&open_angle>90)){hopen_bad->Fill(open_angle); 
				hdist_bad->Fill(distsource);
			}
            if(ok&&open_angle>90)
            {
                //   cout<<"good to:"<<tol<<" "<<open_angle<<" "<<distsource<<" "<<endl;
                hdist_good->Fill(distsource);
                hopen_good->Fill(open_angle);
                //htol_good->Fill(tol);

                Float_t dep1;
                Float_t dep2;

                bool bar1_is_local_max=false;
                bool bar2_is_local_max=false;

                if(data_type=="pm"||data_type=="pattern")
                {
                    dep1=tin->pm[bar1];
                    dep2=tin->pm[bar2];
                    bar1_is_local_max=IsLocalMaximum(bar1, tin->pm );
                    bar2_is_local_max=IsLocalMaximum(bar2, tin->pm );
                }
                else if(data_type=="pm1")
                {
                    dep1=tin->pm1[bar1];
                    dep2=tin->pm1[bar2];
                    bar1_is_local_max=IsLocalMaximum(bar1, tin->pm1 );
                    bar2_is_local_max=IsLocalMaximum(bar2, tin->pm1 );
                }
                else if(data_type=="pm2")
                {
                    dep1=tin->pm2[bar1];
                    dep2=tin->pm2[bar2];
                    bar1_is_local_max=IsLocalMaximum(bar1, tin->pm2 );
                    bar2_is_local_max=IsLocalMaximum(bar2, tin->pm2 );
                }
                else if(data_type=="pm3")
                {
                    dep1=tin->pm3[bar1];
                    dep2=tin->pm3[bar2];
                    bar1_is_local_max=IsLocalMaximum(bar1, tin->pm3 );
                    bar2_is_local_max=IsLocalMaximum(bar2, tin->pm3 );
                }
                else if(data_type=="adc")
                {
                    dep1=tin->adc[bar1];
                    dep2=tin->adc[bar2];
                    bar1_is_local_max=IsLocalMaximum(bar1, tin->adc);
                    bar2_is_local_max=IsLocalMaximum(bar2, tin->adc);
                }

                
                
              if(bar1_is_local_max)hspec_co[bar1]->Fill(dep1);
              if(bar2_is_local_max)hspec_co[bar2]->Fill(dep2);

                hspc_adc[bar1]->Fill(tin->adc[bar1]);
                hspc_adc[bar2]->Fill(tin->adc[bar2]);

                if(num_pairs<NPAIRMAX)
                {
                    tout->xi[num_pairs]=GetXi(px1,py1,px2,py2); 

                    tout->bar1[num_pairs]=bar1;
                    tout->bar2[num_pairs]=bar2;
                    tout->open_angle[num_pairs]=open_angle;
                    //tout->tot_angle[num_pairs]=tol;

                    if(bar1_is_local_max)tout->is_local_max_bar1[num_pairs]=1;
                    else tout->is_local_max_bar1[num_pairs]=0;
                    if(bar2_is_local_max)tout->is_local_max_bar2[num_pairs]=1;
                    else tout->is_local_max_bar2[num_pairs]=0;

                    tout->pair_dist1[num_pairs]=dist1;
                    tout->pair_dist2[num_pairs]=dist2;
                    tout->pair_dist[num_pairs]=TMath::Sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));
                    tout->dist_source[num_pairs]=distsource;
                }

                tout->coincidence[bar1]=bar2+10000;
                tout->coincidence[bar2]=bar1+10000;
                if(bar1_is_local_max)tout->coincidence[bar1]+=10000;
                if(bar2_is_local_max)tout->coincidence[bar2]+=10000;

                h->Fill(get_column(bar1), get_row(bar1));
                h->Fill(get_column(bar2), get_row(bar2));

                mark++;
                num_pairs++;

            }

        }


    //calculate rms of xi;

    if(num_pairs>0)
    {
        double sumxi=0;
        double rmsxi_sum=0;
        for(int i=0;i<num_pairs;i++)sumxi+=tout->xi[i];
        for(int i=0;i<num_pairs;i++)
        {
            rmsxi_sum+=(tout->xi[i]-sumxi/num_pairs)*(tout->xi[i]-sumxi/num_pairs);
        }
       double rmsxi=TMath::Sqrt(rmsxi_sum/num_pairs);
        tout->xideg2=rmsxi;
    }

     

    tout->npairs=num_pairs;

    return num_pairs;
}
bool Coincidence::AreInSameGroup(int bar1,int bar2)
{
    int row1,row2,col1,col2;
    row1=get_row(bar1);
    row2=get_row(bar2);
    col1=get_column(bar1);
    col2=get_column(bar2);

    bool rowsame=(row1-20)*(row2-20)>=0?true:false;
    bool colsame=(col1-20)*(col2-20)>=0?true:false;
    return rowsame&&colsame;

}
bool Coincidence::IsLocalMaximum(int ch, float data[1600] )
{
    int row_bar=get_row(ch);
    int  col_bar=get_column(ch);

    int cur_row,cur_col;
    for(int i=-3;i<3;i++)
    for(int j=-3;j<3;j++)
    {
        cur_row=row_bar+i;
        cur_col=col_bar+j;
        if(cur_col<0||cur_col>=40)continue;
        if(cur_row<0||cur_row>=40)continue;
        if(i==0&&j==0)continue;
        int index=get_index_row_col(cur_row,cur_col);
        if(data[index]>data[ch])return false;
    }
     
    return true;
}
float Coincidence::Point2LineDistance(float px0, float py0, float px1, float py1, float px2, float py2)
{
	float dist_source=TMath::Abs((py2-py1)*px0-(px2-px1)*py0-px1*py2+px2*py1)/TMath::Sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));
	return dist_source;
}
bool Coincidence::TestGeometry(int bar1, int bar2, float sx, float sy, float &dist2source, float &distlim, float tolerance, float barwidth)
{
	float px1, px2, py1, py2;
	get_bar_center(px1,py1,bar1);
	get_bar_center(px2,py2,bar2);
	float pad=tolerance+barwidth/2.;

	bool test;
	if(px1==px2)
	{
		return ((sy-py1)*(sy-py2)<0 )&& (  (sx<px1+pad) &&(sx>px1-pad  ) );
	}
	if(py1==py2)
	{
		return  ((sx-px1)*(sx-px2)<0) && (  (sy<py1+pad) &&(sy>py1-pad ) ) ;
	}


	dist2source=Point2LineDistance(sx, sy, px1, py1, px2, py2);
	float maxdist=0;
	//int maxi=0;
	float vertex_x[4]={px1+pad, px1+pad, px1-pad,px1-pad };
	float vertex_y[4]={py1-pad, py1+pad, py1+pad, py1-pad};
	float dist;
	for(int i=0;i<4;i++)
	{
		dist=Point2LineDistance(vertex_x[i], vertex_y[i], px1, py1, px2, py2);
		if(dist>=maxdist)
		{
		//	maxi=i;
			maxdist=dist;
		}
	}
	distlim=maxdist;
	bool ok=dist2source<=distlim;
	return ok;
}


