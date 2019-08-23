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

TFile fsim("am241_sim.root");
TFile fexp("am241_merlin.root"); //Am241 spectra provided by merlin
TH1F *hsim=(TH1F*)fsim.Get("h");
TH1F *hexp=(TH1F*)fexp.Get("h");

TCanvas cc;
TF1 *fr;
TCanvas cfr;
double GetSigma(double *x,double *par)
{	
    // par[0]: energy resolution^2, reasonable range: 0.2*0.2-0.4*0.4
    // par[1]: correction of statistical contribution: ~1
    // par[2]: noise^2: should be 5*5-15*15 keV
    
    double sig=sqrt(par[0]*x[0]*x[0] + 3.63* par[1]*x[0]  + 5.52*par[2])/2.35;
    return sig;
}

double GetR(double *x,double *par)
{
    double sig= GetSigma(x,par);
    return 2.35*sig/x[0];
}

static int caltimes=0;

double smearFun(double *x, double *par)
{
    //par[0]: resolution factor
    //par[1]: energy calibration factor

    caltimes++;
    if(caltimes%10000==0)cout<<caltimes<<endl;
    double sum=0;
    for(int i=1;i<hsim->GetNbinsX();i++)
    {
        double dep=hsim->GetBinWidth(i)*i;
        double sigma=GetSigma(&dep,&par[0]);
        double fmc=hsim->GetBinContent(i);
        double gxe=par[3]*TMath::Exp(-(par[4]*x[0]-dep)*(par[4]*x[0]-dep)/(2*sigma*sigma));
        sum+=gxe*fmc;
    }

    return sum;

}

double smearCE(double *x, double *par)
{
    //par[0]: resolution factor
    //par[1]: energy calibration factor

    caltimes++;
    if(caltimes%10000==0)cout<<caltimes<<endl;
    double sum=0;
    for(int i=1;i<hsim->GetNbinsX();i++)
    {
        double dep=hsim->GetBinWidth(i)*i;
        double sigma=GetSigma(&dep,&par[0]);
        double fmc=hsim->GetBinContent(i);
        double gxe=par[3]*TMath::Exp(-(par[4]*x[0]-dep)*(par[4]*x[0]-dep)/(2*sigma*sigma));
        sum+=gxe*fmc;
    }

    return sum;

}




TF1 *fitf;

void test()
{
     fitf=new TF1("fitf",smearFun,1,100,5);
    fitf->SetNpx(300);
    fitf->SetParameters(0.09,1,100,1, 1.);
    cfr.cd();
    fitf->Draw();

}
void drawCE() //only draw Am241 energy spectra
{
   fitf=new TF1("fitf",smearCE,1,100,5);
    fitf->SetNpx(300);
    fitf->SetParameters(0.09,1,100,1, 1.);
    cfr.cd();
    fitf->Draw();

}
void testQM4()
{
     fitf=new TF1("fitf",smearFun,1000,16000,5);
    // fitf=new TF1("fitf",smearFun,5,160,5);
    fitf->SetNpx(300);
    fitf->SetParameters(4.249,87.31,49, 0.009256,0.01851);
    //fitf->SetParameters(0.2836,11.72,-2.57, 1,1);
    cfr.cd();
    fitf->Draw();

}


void drawR()
{
    double pa[5];
    fitf->GetParameters(pa);
    fr=new TF1("fr",GetR,1,300,3);
    fr->SetParameters(pa);
    fr->Draw();
}
void energyResFit()
{

    cc.Divide(1,2);
    fitf=new TF1("fitf",smearFun,2000,14000,5);
    fitf->SetNpx(300);
    //fitf->SetParameters(0.009,1,100,1, 1/120.);
    fitf->SetParameters(0.09,1,100,3.93e-2, 9.7e-3);

  //  fitf->SetParLimits(0,0.04,0.16); //instrisinc energy resolution r0^2
   // fitf->SetParLimits(1,0.2,1000); //correction factor for the Rstat
    //fitf->SetParLimits(2,49,500); //noise^2 (keV) 7*7keV--20*20 keV
   // fitf->SetParLimits(3,0,100000); // scale factor
    //fitf->SetParLimits(4,0,0.02); // energy calibration factor

    // fitf->FixParameter(0,0.09);
    // fitf->FixParameter(1,1); 
   //  fitf->FixParameter(2,200); 
   //   fitf->FixParameter(2,2.0945e2);
    //  fitf->FixParameter(3,4.678e-2);
     // fitf->FixParameter(4,9.038e-3); // energy calibration factor

    cout<<"fitting, please wait"<<endl;
    hexp->Fit(fitf,"R");
    cc.cd();
    hexp->Draw();
    drawR();


}

int main()
{
    TFile f("a.root","recreate");
    energyResFit();
  //  drawR();
    hsim->Write();
    hexp->Write();
    fitf->Write();
    fr->Write();

}




    //fitf->SetParLimits(0,0.04,100); //instrisinc energy resolution r0^2
    //fitf->SetParLimits(1,0.2,1000); //correction factor for the Rstat
    //fitf->SetParLimits(2,49,500); //noise^2 (keV) 7keV--18 keV
    //fitf->SetParLimits(3,0,100000); // scale factor
    //fitf->SetParLimits(4,0,0.02); // energy calibration factor
