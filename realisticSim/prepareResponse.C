
//hualin Xiao
//prepare response matrix for simulation
//data are extracted from ESRF 2015 test
// polar4/ESRF2015/L3/
// data in: polar4/ESRF2015/responseMatrix_MC/

const int Pos2ModID[25] = { 413, 524, 410, 525, 424,
			      520, 414, 509, 441, 522,
			      406, 602, 503, 506, 415,
			      601, 402, 505, 408, 516,
			      423, 507, 411, 513, 405};

void prepareResponse()
{
    TFile fo("responseFM_MC.root","recreate");
    fo.mkdir("response");
    for(int i=0;i<25;i++)
    {
        TString filename=Form("/media/xiaohl/polar4/ESRF2015/L3/daq_%03d_0000.root",Pos2ModID[i]);
        TFile f(filename);
        TH2F *h=(TH2F*)f.Get("hresp");
        fo.cd("response");
        

        h->Write(Form("hresp%d",i));
        delete h;
    }
         
}
