#include <iostream>
#include <string>    
#include <TApplication.h> 
#include <TSystem.h> 
#include <TGraph.h>
#include <TROOT.h> 
#include <TFile.h>  
#include <TTree.h>  
#include <TCanvas.h>  
#include <TF1.h>  
#include <TLine.h>  
#include <TText.h>  
#include <TH1F.h> 
#include <TH2F.h> 
#include <unistd.h> 
using namespace std; 
void Help()
{
	cout<<"usage: hitsViewer data.root  <start_event> lego or colz"<<endl;
}
int main(int argc, char *argv[])
{

	if(argc==1)
	{
		Help();
		return 0;
	}
	TString data_file=argv[1];
	TString opt="colz+text";

	if(argc==4)opt=argv[3];


	int start_event=0;
	if(argc==3)	start_event=atoi(argv[2]);
	TApplication app("app",&argc,argv);

	TFile f(data_file.Data());
	TTree *t=(TTree*)f.Get("polar");
	int entries=t->GetEntries();
	float pm[1600];
	float xideg[2];
	int max_bar[2];
	int sec_bar[2];
	bool xiok[2];
	int pair_mark[1600];
	float open_angle;
	t->SetBranchAddress("pm",pm);
	t->SetBranchAddress("xideg",xideg);
	t->SetBranchAddress("max_bar",max_bar);
	t->SetBranchAddress("sec_bar",sec_bar);
	t->SetBranchAddress("xiok",xiok);
	// t->SetBranchAddress("pair_mark",pair_mark);
	// t->SetBranchAddress("open_angle",&open_angle);
	TCanvas c1;
	TH2F h("h","h",40,0,40,40,0,40);
	int nhits=0;
	int ns=0;
	c1.Divide(1,2);
	for(int i=start_event;ns<500;i++)
	{
		t->GetEntry(i);

		if(nhits==0)continue;
		h.Reset();
		for(int j=0;j<1600;j++)
		{
			int row,col;
			row=j%40;
			col=j/40;
			if(pm[j]>10)
			{
				h.SetBinContent(row+1,col+1,pm[j]);
				nhits++;
			}

		}
		if(nhits<2)continue;
		cout<<"-----"<<i<<" "<<nhits<<endl;

		ns++;
		c1.cd(1);
		TText *tt=NULL;
		TLine *tl=NULL;
		TLine *tl2=NULL;
		int row0,col0,row1,col1;
		h.Draw(opt.Data());
		if(xiok[0])
		{
			row0=max_bar[0]%40;
			col0=max_bar[0]/40;
			row1=sec_bar[0]%40;
			col1=sec_bar[0]/40;

			tl=new TLine(row0,col0,row1,col1);
			tl->SetLineColor(kBlack);
			tl->Draw();
		}

		if(xiok[1])
		{
			row0=max_bar[1]%40;
			col0=max_bar[1]/40;
			row1=sec_bar[1]%40;
			col1=sec_bar[1]/40;

			tl2=new TLine(row0,col0,row1,col1);
			tl2->SetLineColor(kBlack);
			tl2->Draw();
		}


		c1.cd(2);
		h.Draw("lego2");

		c1.Print(Form("view%d.png",i));


	}

return 0;
} 
