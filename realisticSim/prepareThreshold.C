//hualin Xiao
//prepare thresholds for simulation
//data are extracted from ESRF 2015 test
// polar4/ESRF2015/L2/dothresholdFit.C
// data in: polar4/ESRF2015/threshold_MC/

const int Pos2ModID[25] = { 413, 524, 410, 525, 424,
			      520, 414, 509, 441, 522,
			      406, 602, 503, 506, 415,
			      601, 402, 505, 408, 516,
			      423, 507, 411, 513, 405};

void prepareThreshold()
{
    TFile fo("thresholdFM_MC.root","recreate");
    fo.mkdir("threshold");
    for(int i=0;i<25;i++)
    {
        TString filename=Form("/media/xiaohl/polar4/ESRF2015/threshold_MC/threshold_%03d.root",Pos2ModID[i]);
        TFile f(filename);
        TH1F *h=(TH1F*)f.Get("threshold");

        for(int j=0;j<64;j++)
        {
            TH1F *h2=(TH1F*)f.Get(Form("h%d",j));
            fo.cd("threshold");
            //if(h2)h2->Write(Form("th_%d_ch%d",i,j));
        }
         
        fo.cd();
        fo.cd("threshold");
        h->Write(Form("threshold%d",i));
        delete h;
    }
}
