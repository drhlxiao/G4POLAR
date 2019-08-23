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
    cout<<"usage: hitsViewer data.root  start_event"<<endl;
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
    int pair_mark[1600];
    float open_angle;
    t->SetBranchAddress("pm",pm);
    t->SetBranchAddress("pair_mark",pair_mark);
    t->SetBranchAddress("open_angle",&open_angle);
    TCanvas c1;
    TH2F h("h","h",40,0,40,40,0,40);
    for(int i=start_event;i<entries;i++)
    {
        t->GetEntry(i);

        cout<<"-----"<<endl;
        for(int j=0;j<1600;j++)
        {
            int row,col;
            row=j%40;
            col=j/40;
            h.SetBinContent(row+1,col+1,pm[j]);
            if(pair_mark[j]>0)
            {
                cout<<row<<" "<<col<<endl;
                h.SetBinContent(row+1,col+1,10000);
                if(open_angle!=0)cout<<open_angle<<endl;
            }
        }


        h.Draw("colz");
        c1.WaitPrimitive();
    }
    app.Run();
     
     
    return 0;
} 
