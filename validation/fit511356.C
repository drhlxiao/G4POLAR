{
    gROOT->ProcessLine(".L stepfun.C");
    TFile *_file0 = TFile::Open("511keVCenterbar.root");
    TFile *_file1 = TFile::Open("356keV.root");
    TTree *p511=(TTree*)_file0->Get("polar");
    TTree *p356=(TTree*)_file1->Get("polar");
    TCanvas c1; 
    load(200,420);
    p511->Draw("pm3[820]>>h511");
    h511->Draw();
    stepfun->SetParameters(0,67.2,348,0.0178);

    h511->Fit(stepfun,"R");

  //  TH1F *h356=(TH1F*)h511->Clone();
    p356->Project("h356","pm3[820]");
    h356->Draw("same");
    load(130,300); 
    stepfun->SetParameters(0,67.2,200,0.0178);
    h356->Fit(stepfun,"R");
}
