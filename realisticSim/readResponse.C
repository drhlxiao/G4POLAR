void readResponse1() {

    TFile *f1 = TFile::Open("response.root");
    TIter next(f1->GetListOfKeys());
    TKey *key;
    TCanvas c1;
    c1.Print("hresponse.pdf[");
    while ((key = (TKey*)next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (cl->InheritsFrom("TH1"))
        {
            TH1 *h = (TH1*)key->ReadObj();
            h->Draw();
            c1.Print("hresponse.pdf");
        }
    }


    c1.Print("hresponse.pdf]");
}

void WriteEnergyResponse()
{
    TFile *fcal=new TFile("response.root");
    TH1F*    hres_low=(TH1F*)fcal->Get("hres_low");
    TH1F*    hres_high=(TH1F*)fcal->Get("hres_high");

    TFile f("energyRes.root","recreate");


    double energy_resolution_high[1600];
    double energy_resolution_low[1600];
  for(int i=0;i<1600;i++)
        {
            double res80=hres_low->GetBinContent(i+1);
            if(res80>2)res80=2;
            //wrong fit channels
double            res_high=hres_high->GetBinContent(i+1)/2.35;
            double res_low=res80/2.35;

            if(res_high>res_low)cout<<"wront:"<<res_high<<" "<<res_low<<endl;

            double b=(res_low*res_low-res_high*res_high)/(1/80.-1/477.);
            double a=res_high*res_high-b/477.;
            double a2=res_low*res_low-b/80.;
            if(a!=a2)cout<<"wrong:"<<i<<" "<<a<<" "<<a2<<endl;
            if(a<0)cout<<i<<" "<<a<<endl;
            
            TF1 tf(Form("f%d",i),Form("sqrt(%f+%f/x)",a,b),10,500);
            tf.SetNpx(500);
            f.cd();
            tf.Write();
        }
}

