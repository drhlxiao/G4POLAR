/* a program to draw coincidence hits*/
/* hualin Xiao*/

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
#include <TH1F.h> 
 #include <TH2F.h> 
using namespace std; 
void Help()
{
    cout<<"usage: coincidenceViewer data.root  start_event"<<endl;
}
int main(int argc, char *argv[])
{

    if(argc!=3)
    {
        Help();
        return 0;
    }
    TString data_file=argv[1];
    int start_event=atoi(argv[2]);
    TApplication app("app",&argc,argv);

    TFile f(data_file.Data());
    TTree *t=(TTree*)f.Get("polar");
    int entries=t->GetEntries();
    float pm[1600];
    float pattern[1600];
    int npairs;
    int bar1[10];
    int bar2[10];
    float dist_source[10];
    t->SetBranchAddress("npairs",&npairs);
    t->SetBranchAddress("bar1",bar1);
    t->SetBranchAddress("bar2",bar2);
    t->SetBranchAddress("pm",pm);
    t->SetBranchAddress("pattern",pattern);
    t->SetBranchAddress("dist_source",dist_source);
    TCanvas c1;
    TH2F h("h","h",40,0,40,40,0,40);
    for(int i=start_event;i<entries;i++)
    {
        t->GetEntry(i);

        h.Reset();

        cout<<"pairs: "<<npairs<<endl;
        int color=100;

        for(int j=0;j<npairs;j++)
        {
            int b1,b2;
            b1=bar1[j];
            b2=bar2[j];
            int row1,col1;
            row1=b1%40;
            col1=b1/40;
            int row2,col2;
            row2=b2%40;
            col2=b2/40;
            h.SetBinContent(row1+1,col1+1,color);
            h.SetBinContent(row2+1,col2+1,color);
            cout<<"bar1,bar2,pm1,pm2:"<<b1<<" "<<b2<<" "<<pm[b1]<<" "<<pm[b2]<<" "<<dist_source[j]<<endl;
            color+=100;
        }

        h.Draw("colz");
        c1.WaitPrimitive();
    }
    app.Run();
     
     
    return 0;
} 
