{
    //////////////////////////////////////////////////////////
    //   This file has been automatically generated 
    //     (Wed Feb 10 14:13:09 2016 by ROOT version5.34/30)
    //   from TTree polar/polar simulation data
    //   found on file: 80keV_bar4_4.root
    //////////////////////////////////////////////////////////


    //Reset ROOT and connect tree file
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("511keV.root");
    if (!f) {
        f = new TFile("511keV.root");
    }
    TTree *tree;
    f->GetObject("polar",tree);

    //Declaration of leaves types
    Int_t           pattern[1600];
    Float_t         pm[1600];
    Float_t         pm1[1600];
    Float_t         pm2[1600];
    Float_t         pm4[1600];
    Float_t         pm3[1600];
    Float_t         adc[1600];
    Long64_t        evnt;
    Int_t           n;
    Int_t           npairs;
    Double_t        time;
    Double_t        xideg;
    Double_t        xideg2;
    Double_t        particle_position[3];
    Double_t        particle_direction[3];
    Double_t        particle_pol[3];
    Double_t        particle_energy;
    Int_t           accepted[25];
    Int_t           tout1[25];
    Int_t           pair_mark[1600];
    Int_t           tout2[25];
    Int_t           tmth[25];
    Int_t           recorded;
    Int_t           bar_maxedep;
    Int_t           bar_secmaxedep;
    Int_t           first_bar;
    Float_t         distance_source_maxbar;
    Int_t           track_bar[5];
    Int_t           track_process[5];
    Float_t         track_edep[5];
    Float_t         open_angle;
    Float_t         pair_dist;
    Float_t         pair_dist1;
    Float_t         pair_dist2;

    // Set branch addresses.
    polar->SetBranchAddress("pattern",pattern);
    polar->SetBranchAddress("pm",pm);
    polar->SetBranchAddress("pm1",pm1);
    polar->SetBranchAddress("pm2",pm2);
    polar->SetBranchAddress("pm4",pm4);
    polar->SetBranchAddress("pm3",pm3);
    polar->SetBranchAddress("adc",adc);
    polar->SetBranchAddress("evnt",&evnt);
    polar->SetBranchAddress("n",&n);
    polar->SetBranchAddress("npairs",&npairs);
    polar->SetBranchAddress("time",&time);
    polar->SetBranchAddress("xideg",&xideg);
    polar->SetBranchAddress("xideg2",&xideg2);
    polar->SetBranchAddress("particle_position",particle_position);
    polar->SetBranchAddress("particle_direction",particle_direction);
    polar->SetBranchAddress("particle_pol",particle_pol);
    polar->SetBranchAddress("particle_energy",&particle_energy);
    polar->SetBranchAddress("accepted",accepted);
    polar->SetBranchAddress("tout1",tout1);
    polar->SetBranchAddress("pair_mark",pair_mark);
    polar->SetBranchAddress("tout2",tout2);
    polar->SetBranchAddress("tmth",tmth);
    polar->SetBranchAddress("recorded",&recorded);
    polar->SetBranchAddress("bar_maxedep",&bar_maxedep);
    polar->SetBranchAddress("bar_secmaxedep",&bar_secmaxedep);
    polar->SetBranchAddress("first_bar",&first_bar);
    polar->SetBranchAddress("distance_source_maxbar",&distance_source_maxbar);
    polar->SetBranchAddress("track_bar",track_bar);
    polar->SetBranchAddress("track_process",track_process);
    polar->SetBranchAddress("track_edep",track_edep);
    polar->SetBranchAddress("open_angle",&open_angle);
    polar->SetBranchAddress("pair_dist",&pair_dist);
    polar->SetBranchAddress("pair_dist1",&pair_dist1);
    polar->SetBranchAddress("pair_dist2",&pair_dist2);

    //     This is the loop skeleton
    //       To read only selected branches, Insert statements like:
    // polar->SetBranchStatus("*",0);  // disable all branches
    // TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

    Long64_t nentries = polar->GetEntries();

    TH1F h("h","h",200,0,600);
    TH1F h2("h2","h",200,0,600);
    Long64_t nbytes = 0;
    cout<<polar->GetEntries()<<endl;

    int row,col;

        for (Long64_t i=0; i<nentries;i++) {
            nbytes += polar->GetEntry(i);
            //int max=0;
            //float maxbar;

            bool no=false;
            for(int j=0;j<1600;j++)
            {
                row=j/40;
                col=j%40;
                if(row>24||row<16)continue;
                if(col>24||col<16)continue;

                if(j!=820&&pm[j]>10)
                {
                    no=true;
                }
            }
            h.Fill(pm3[820]);
            if(!no)h2.Fill(pm3[820]);
        }
        TCanvas c1;
    h.Draw();
        TCanvas c2;
    h2.Draw();
}
