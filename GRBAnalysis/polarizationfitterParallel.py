#!/usr/bin/python
import os
#from ROOT import TH1F, TH2F, TFile, TTree, TCanvas,gROOT,TList,TLatex
from ROOT import TH1F, TH2F, TFile, TTree, TCanvas,gROOT,TList,gStyle, TLatex, Double,TMemFile, gDirectory,Long
import numpy as np
import argparse
import matplotlib
import matplotlib.pyplot as plt
from subprocess import Popen
from multiprocessing import Pool
import sys
#from scipy.stats import chi2
from optparse import OptionParser



config={'NPhi': 90,
        'NPol': 40,
        'dir': '/RAID/simulations/GRB170206A/database/',
        'GRB': '170206A',
        'hmc': 'quicklook/h1xi', #h1xi or h1xi2 
        'hmc_type': 'ideal', #h1xi or h1xi2 
        'output': 'chi2grid.root',
        }


def drawContour(hchi2, fo):
    hchi2r=hchi2.Clone()
    levels=np.array([2.3, 6.17, 11.8,18.4])
    s_sigma=["1#\sigma", "2#\sigma", "3#\sigma","4#\sigma"]
    hchi2r.SetContour(100)

    gStyle.SetPalette(55)
    gStyle.SetOptStat(0)


    hchi2.SetContour(3,levels)
    hchi2.Draw("CONT Z LIST")
    print(gDirectory)
    a = TMemFile("a", "recreate")
    print(gDirectory)  # this is "a"
    curr_dir = gDirectory  # this is "a"
    b = TMemFile("b", "recreate")
    print(curr_dir)  # this now points to "b"! Yikes!

    conts =gROOT.GetListOfSpecials().FindObject("contours")
    curv=None
    gc=None
    contLevel=None
    nGraphs    = 0;
    TotalConts = 0;

    cc=TCanvas()
    hchi2r.Draw("colz")

    gcl=[]
    l=TLatex()
    if conts:
        TotalConts=conts.GetSize()
        print 'Total counts {0}'.format(TotalConts)
        for i in range(0,TotalConts):
            contLevel = conts.At(i)
	    txt=s_sigma[i]
            curv = contLevel.First()
            for j in range(0,contLevel.GetSize()):
                xval0=Double(0)
                yval0=Double(0)

                print 'Drawing CLs: %d %d'%(i,j)
                print txt

                curv.GetPoint(0, xval0, yval0)
                gc=curv.Clone()
                gc.SetLineColor(0)
                gc.SetLineWidth(2)
                gcl.append(gc)
                gc.Draw("C")
                l.SetTextColor(0)
		l.DrawLatex(xval0,yval0,txt)
                
                curv=contLevel.After(curv)

    gStyle.SetTitleW(0.)
    gStyle.SetTitleH(0.)
    fo.cd()
    cc.Write("chi2_CL")
    return cc


def getHXI(fname, conf):
    f=TFile(fname)
    hname=conf['hmc']
    hmc=f.Get(hname)
    if not hmc:
        print 'Can not load histogram in File:', fname
        sys.exit(0)

    hmc.SetDirectory(0)
    return hmc
        
def getP0HXI(fname='phi_0_pol_0.root',conf=config):
    fname=conf['dir']+'/'+fname
    return getHXI(fname, conf)

def getP1HXI(phi,conf=config):
    nphi=conf['NPhi']
    phi_div=180./config['NPhi']
    if phi%phi_div<>0:
        return None
    fname=conf['dir']+'/'+'phi_%d_pol_1.root'%phi
    return getHXI(fname, conf)



def mixModulationCurve(phi, pol, conf=config):
    
    hxi_p=getP0HXI()
    hxi_p1=getP1HXI(phi)
    
    hp=hxi_p.Clone()
    hp.Reset()
    hp.Add(hxi_p,hxi_p1,1-pol, pol )
    return hp

def getAllHXI(num_phi, num_pol, fo,conf=config):
    phi_step=180./float(num_phi)
    pol_step=1./float(num_pol)
    hcoll=[]
    fo.cd()
    dirmc=conf['hmc_type']
    fo.mkdir(dirmc)
    for iphi in range(0, num_phi):
        phi=iphi*phi_step
        for pol in np.arange(0,1, pol_step):
            hist= mixModulationCurve(phi, pol)
            #print phi,pol
            if hist:
                fo.cd(dirmc)
                hist.Write('hxi_phi_%.1f_p_%.2f'%(phi,pol))
                hcoll.append([phi,pol, hist])
    return hcoll


def chi2test(hsim, hexp,title):
    nbins_sim=hsim.GetXaxis().GetNbins()
    nbins_exp=hexp.GetXaxis().GetNbins()
    n_sim=hsim.Integral(1, nbins_sim)
    n_exp=hexp.Integral(1, nbins_exp)
    scale_factor=float(n_exp)/float(n_sim)
    hsim.Scale(scale_factor)
    chi=hexp.Chi2Test(hsim, "WW CHI2 P")

    cchi=TCanvas()

    hsim.SetLineColor(2)
    hexp.SetLineColor(4)

    maxsim=hsim.GetMaximum()
    maxexp=hexp.GetMaximum()
    if maxsim>maxexp:
        hsim.Draw()
        hexp.Draw("same")
    else:
        hexp.Draw()
        hsim.Draw("same")
    l=TLatex()
    txt='#\chi=%f'%chi
    l.DrawLatex(10,10,txt)
    #cchi.Write(title)


    return chi

def makeDeltaChi2(h2in):

    minValue=h2in.GetMinimum()
    print 'minimal chisquare value:'
    h2out=h2in.Clone()
    nbinsx=h2in.GetXaxis().GetNbins()
    nbinsy=h2in.GetYaxis().GetNbins()
    h2out.SetTitle("#\Delta \chi^2")
    min_bin_x=0
    min_bin_y=0
    for i in range(0,nbinsx):
        for j in range(0,nbinsy):
            value=h2in.GetBinContent(i+1,j+1)
            if value==minValue:
                value-=0.1
                min_bin_x=i+1
                min_bin_y=j+1
            h2out.SetBinContent(i+1,j+1,value-minValue)

    return h2out, minValue,min_bin_x,min_bin_y






def fitPolarization(expfilename,hname,conf=config):
    fout=TFile(config['output'],"recreate")
    fexp=TFile(expfilename)
    hmcexp=fexp.Get(hname)
    fout.cd()
    hmcexp.Write('hmcexp')

    phi_nbins=int(conf['NPhi'])
    pol_nbins=int(conf['NPol'])
    print 'Preparing simulated modulation curves...'
    hcoll=getAllHXI(phi_nbins, pol_nbins, fout, conf)
    htitle='chi2 distribution %s;Polarization degree; phi_p (degree)'%config['hmc_type'] 
    print htitle
    hchi2=TH2F("hchi2",htitle,pol_nbins, 0, 1, phi_nbins, 0, 180)
    print 'Number of simulated modulations %d'%len(hcoll)
    print 'Chisquare testing...'



    chi2dir='chi2test'
    fout.mkdir(chi2dir)
    fout.cd(chi2dir)
    print 'preparing modulation curves from simulation data_d...'




    paralist=[]
    for row in hcoll:
        phi=row[0]
        pol=row[1] 
        hmcsim=row[2]
        title='phi_%.2f_pol_%0.2f'%(phi,pol)
        print 'Fitting:',phi, pol
        para={'hmcsim':hmcsim,
                'hmcexp': hmcexp,
                'title':title,
                'phi': phi,
                'pol':pol
                }
        paralist.append(para)

    #print paralist
    print 'starting multithresholding ..'
    results=multiThreadChi2Test(paralist)
    print 'threading done..'

    hmcsim_min=None
    chi2_min=1e10
    for chi2, par in zip(results,paralist):
        print(par['pol'],par['phi'],chi2)
        hchi2.Fill(par['pol'],par['phi'],chi2)
        if chi2<chi2_min:
            chi2_min=chi2
            hmcsim_min=par['hmcsim']





    #    chi2=chi2test(hmcsim, hmcexp,title)
    #    hchi2.Fill(pol,phi,chi2)




    
    fout.cd()

    hchi2_delta,min_chi2,min_bin_x, min_bin_y=makeDeltaChi2(hchi2)
    fig=drawContour(hchi2_delta,fout)
    hchi2.Write()
    hchi2_delta.Write("hchi2_delta")

    best_pol=hchi2.GetXaxis().GetBinCenter(min_bin_x)
    best_phi=hchi2.GetYaxis().GetBinCenter(min_bin_y)
    bestfit_msg='Best Fit polarization %.2f, phi, %.2f, #\chi2_{min}%.2f'%(best_pol, best_phi, min_chi2)
    print bestfit_msg

    cm=TCanvas()
    
    cm.cd()
    hmcsim_min.Draw()
    hmcexp.Draw("same")
    hmcexp.SetTitle(bestfit_msg)
    la=TLatex()
    la.DrawLatex(20,20,bestfit_msg)
    cm.Write("C_MC_bestFit")


    #return fig


def chi2test_(data):
    return chi2test(data['hmcsim'],data['hmcexp'], data['title'])


def multiThreadChi2Test(paralist):
    p=Pool(14)
    results=p.map(chi2test_,paralist)
    return results


        




if __name__=='__main__':
    parser=OptionParser()
    parser.add_option("-i",  action='store', dest='input', 
            help='file contains experimental modulation curve')
    parser.add_option("-o",  action='store',dest='output',
            help='output filename')
    parser.add_option("-e",  action='store',dest='mcexp',
            help='name of the histogram default quicklook/h1xi')
    parser.add_option("-d",  action='store',dest='dbdir',help='database dir')
    parser.add_option("-a",  action='store',dest='phibins',help='Number bins for phi')
    parser.add_option("-p",  action='store',dest='polbins',
            help='Number bins for polarization degree')
    parser.add_option("-s",  action='store',
            dest='hmcsimname',help='histogram name in the simulated files')

    options, args=parser.parse_args()
    expfilename=''
    hexpmcname='quicklook/h1xi'



    if options.input:
        expfilename=options.input

    if options.mcexp:
        hname=options.mcexp
    if options.output:
        config['output']=options.output
    if options.dbdir:
        config['dir']=options.dbdir
    if options.phibins:
        config['NPhi']=options.phibins
    if options.polbins:
        config['NPol']=options.polbins
    if options.hmcsimname:
        config['hmc']=options.hmcsimname

    print 'input:', expfilename
    print 'histogram:', hexpmcname
    if expfilename and hexpmcname:
        fitPolarization(expfilename,hexpmcname,config)


