/*
 *  generate_band.C
 *
 *  Routine to produce the GRB spectrum following a Band function
 *
 *
 *  How to run:  
 *      - generate_band(run_number,theta_deg, phi_deg, flux_ergcm-2, duration_sec,
 *			alpha_band, beta_band, Epeak_band_keV, e_min_keV, e_max_keV,
 *			pol_frac,polangle_deg,SphRad_cm, CircRad_cm, "a0_output.root")
 *
 *
 *  The variable epr in the output "a0" file is only useful for the DPNC laboratory simulations:
 *  Meaning:
 *	-epr[0] = theta1 (angle in the scattering on the first step of DPNC lab simulation)
 *	-epr[1] = phi1
 *	-epr[2] = f1   (see Christian C. Berclaz, 2001 report)
 *	-epr[3] = m1	(see Berclaz report)
 *	-epr[4] = theta2 (angle in the scattering on the second step of DPNC lab simulation)
 *	-epr[5] = phi2
 *	-epr[6] = f2   (see Christian C. Berclaz, 2001 report)
 *	-epr[7] = m2	(see Berclaz report)
 *	-epr[8] = P12 = f1 f2 (1 + m1 m2 cos(delta Phi)	 --> not used
 *	
 *
 *  Created by Estela Suarez
 *  Copyright 2009 Universite de Geneve. All rights reserved.
 *
 *
 */

#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TRandom.h>
#include <TMath.h>
#include <TNamed.h>
#include <TString.h>
#include "t2sim.h"


using namespace std;

Double_t band(Double_t *x,Double_t *par){
	Double_t E=x[0];
	Double_t A=par[0];
	Double_t alpha=par[1];
	Double_t beta=par[2];
	Double_t Epeak=par[3];
	Double_t DnDE;
	Double_t E100=100;  //KeV
	Double_t E0=Epeak/(2+alpha);
	Double_t Elim=(alpha-beta)*E0;
	if (E<Elim){
		DnDE=A*TMath::Exp(alpha*TMath::Log(E/E100))*
			TMath::Exp(-E/E0);
	}
	else{
		DnDE=A*TMath::Exp((alpha-beta)*TMath::Log((alpha-beta)*E0/E100))*
			TMath::Exp((beta-alpha))*TMath::Exp(beta*TMath::Log(E/E100));
	}
	return DnDE;
}

Double_t eband(Double_t *x,Double_t *par){
	return x[0]*band(x,par);
}


//TTree* generate_band(int run_number=0,
void generate_band(
		Float_t theta=0., //incident angle
		Float_t phi=0,
		Float_t flux=1.e-5,
		Double_t duration=1.,
		Float_t alpha=-1.0,
		Float_t beta=-2.5,
		Float_t Epeak=200.,
		Float_t e_min=5.,
		Float_t e_max=1000.,
		Float_t pol_frac=1,
		Float_t pol_angle=0.,
		Float_t SphRad=50.,//cm
		Float_t CircRad=70.,//cm
		//const char *file = "grb1s.root"
		)
{
	gRandom->SetSeed(0);

	//TFile *rootFile = new TFile(file, "recreate");


	double position[3];
	double polarization[3];
	double direction[3];
	char pname[10];
	double energy;
	double time=0;

	double DegToRad=3.1415/180.;
	cout << "theta(deg) = " << theta << endl;
	cout << "phi(deg) = " << phi << endl;
	theta = theta*DegToRad;
	phi = phi*DegToRad; 
	cout << "theta(rad) = " << theta << endl;
	cout << "phi(rad) = " << phi << endl;


	char name[10] = "gamma";
	const double ergtokev=6.2414504e+08;
	TF1 *f1=new TF1("band",band,e_min,e_max,4);

	f1->SetParameters(1.0,alpha,beta,Epeak);//A alpha beta Epeak
	f1->SetNpx((int)e_max*10);

	double norm=f1->Integral(e_min,e_max);//integral of N(E)d(E) in keV
	TF1 *f2=new TF1("E * band",eband,e_min,e_max,4);
	// integral of E*N(E)d(E) in keV2
	f2->SetParameters(1.0,alpha,beta,Epeak);//A alpha beta Epeak
	double total_energy=f2->Integral(e_min,e_max)/norm;
	delete f2;
	double grb_energy=flux*ergtokev*duration*TMath::Pi()*CircRad*CircRad;
	Long64_t n_phot = (Long64_t) (gRandom->Poisson(grb_energy/total_energy));
	Double_t dt = Double_t (duration/n_phot);
	double sume=0;


	double* x=new double[3];
	x[0]=SphRad*sin(theta)*cos(phi);
	x[1]=SphRad*sin(theta)*sin(phi);
	x[2]=SphRad*cos(theta);
	double* d=new double[3];
	d[0]=-1*sin(theta)*cos(phi);
	d[1]=-1*sin(theta)*sin(phi);
	d[2]=-1*cos(theta);
	cout<<"direction: "<<d[0]<<" "<<d[1]<<" "<<d[2]<<endl;


    for(int i=0;i<2000;i++)
    {

    }
     

	cout << "Entering loop... " << endl;
	cout << "Spectrum Integral: " << norm << " [photons.cm-2.s-1] " << endl;		
	cout << "Number of photons: " << n_phot << endl; 
	//cout << "(Conversion constant: " << 2*TMath::Pi()*TMath::Pi()*CircRad*CircRad*duration << ")" << endl;


}



