from ROOT import TH1F, TH2F, gStyle, gROOT, gSystem

files={0.05:'bestfit_phi90_pol_0_0_5.root',
        0.1:'bestfit_phi90_pol_0_1.root'
        0.2:'bestfit_phi90_pol_0_2.root'
        0.5:'bestfit_phi90_pol_0_5.root'}


def plot(pol, fname):
    f=TFile(fname)
    c=f.Get('hchi2_CL')
    gStyle.SetOptStat(0)
    c.Update()
    h2=c.FindObject("hchi2")
    h2.GetXaxis().SetRangeUser

