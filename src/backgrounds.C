//#include <EventFormat.hh>

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "backgrounds.hh"
#include <TF1.h>
//#include <t2sim.h>
#include <TDatime.h>
#include <TRandom.h>
#include <math.h>
#include "G4SystemOfUnits.hh"

const double Pi=3.1415926;
double elespc(double *x, double *par)
{
	return par[0]*(pow(x[0],(par[1])));
}
double posspc(double *x, double *par)
{
	return par[0]*pow(x[0],(par[1]));
}


double cxbspec99(double *x, double *par)
{
	//Function from Gruber et al. ApJ 520 (1999) 124, 
	// Divided by E to get it in normal spectral units [cm-2 s-1 keV-1 sr-1]
	//x[0]: energy 
	//p[0]: normalization factor (1 for Gruber, 1.1 for Churazov)
	double E = x[0]; //energy in keV
	double result(0);


	if (E<60.) 
	{
		result = par[0]*(7.877*pow(E,-0.29)*exp(-E/41.13))/E;
	} else if (E>60.)
	{
		result = par[0]*(0.0259*pow((E/60.),-5.5) + 0.504*pow((E/60.),-1.58) + 0.0288*pow((E/60.),-1.05))/E;
	}
	return result;
}

double spectrum(double *x, double *par)
{
	//Function from Gruber et al. ApJ 520 (1999) 124, 
	// Divided by E to get it in normal spectral units [cm-2 s-1 keV-1 sr-1]
	//x[0]: energy 
	//p[0]: normalization factor (1 for Gruber, 1.1 for Churazov)
	double E = x[0]; //energy in keV
	double result(0);

	if (E<60.) 
	{
		result = par[0]*(7.877*pow(E,-0.29)*exp(-E/41.13))/E;
	} else if (E>60.)
	{
		result = par[0]*(0.0259*pow((E/60.),-5.5) + 0.504*pow((E/60.),-1.58) + 0.0288*pow((E/60.),-1.05))/E;
	}
	return result;
}



backgrounds::~backgrounds()
{
	delete cxbb99;
	delete eleb;
}
backgrounds::backgrounds()
{
	duration=5;
	emin=50;
	emax=10e3;
	SphrRad=30.;
	CircRad=30.;

}
void backgrounds::InitParticleGun()
{
	//	double duration=5;
	//string fileName="bkg_5s.root"

	// diffuse cosmic x rays
	//	TF1 *cxbb=new TF1("cxbb",cxbspc,emin,emax,3);
	//	cxbb->SetNpx(500);
	//	cxbb->SetParameters(0.940059, -1.28089, -0.262414);

	//diffuse cosmic x ray, Gruber
    emin=50;
    emax=10e3;
	cxbb99=new TF1("cxbb99",cxbspec99, emin,emax,1);
	cxbb99->SetNpx(500);
	cxbb99->SetParameter(0,1.1);

	double cxbb99i=cxbb99->Integral(emin,emax);


	double eminele=100;
	emax=100e3;
	// electron
	eleb=new TF1 ("eleb",elespc,eminele,emax,2);
	eleb->SetNpx(500);
	eleb->SetParameters(9113., -2.30);
	double elebi=eleb->Integral(eminele,emax);

    /*
	//positron 
	posb=new TF1 ("posb",posspc,eminele,emax,2);
	posb->SetNpx(500);
	posb->SetParameters(11.14, -1.70);
	double posbi=posb->Integral(eminele,emax);

    */
	//caculate number of event
	double allIntegral=cxbb99i+elebi;
	//	double allIntegral=elebi;


	cxb_frac=(cxbb99i)/allIntegral;
	ele_frac=1;
	G4cout<<"background duration:"<<duration<<G4endl;
	G4cout<<"background(cxb, positron, electron): "<< cxbb99i<<" "<<" "<<elebi<<G4endl;

	G4cout<<"Sum of the backgrounds (positron, electron and CXB ) :"<<allIntegral<<G4endl;

	double number=2*Pi*Pi*CircRad*CircRad*duration*allIntegral;
	//	double number=2*Pi*Pi*SphrRad*SphrRad*duration*allIntegral;
	G4cout<<"conversion factor: "<<2*Pi*Pi*CircRad*CircRad*duration<<G4endl;
	G4cout<<"Number of Photons: " << number<<G4endl; 
	//n_phot=(double)gRandom->Poisson(number);
	n_phot=number;
	G4cout<<"Number of Photons to be generated : "<<n_phot<<G4endl;
	dt=duration/n_phot;
	G4cout<<"delta time is :"<<dt<<G4endl;
	//	ts=new t2sim();
	fPartiTable = G4ParticleTable::GetParticleTable();

}
void backgrounds::GetEvent(G4ParticleGun *fParticleGun)
{

	TDatime now;
	double position[3];
	double polarization[3];
	double direction[3];
	char pname[10];
	double energy;
	double time=now.GetTime();

	double theta,phi;
	time+=gRandom->Exp(dt);
	double rnd=gRandom->Uniform();
    /*
	if(rnd<pos_frac)
	{//positron
		strcpy(pname,"e+");
		energy=posb->GetRandom();

	}
	else*/
    if(rnd<cxb_frac)
	{
		//cxb
		strcpy(pname,"gamma");
		energy=cxbb99->GetRandom();

	}
	else 
	{
		strcpy(pname,"e-");
		//electron
		energy=eleb->GetRandom();
	}

	theta=acos(gRandom->Uniform());
	phi=2*Pi*gRandom->Uniform();
	position[0]=SphrRad*cos(phi)*sin(theta);
	position[1]=SphrRad*sin(phi)*sin(theta);
	position[2]=SphrRad*cos(theta);

	//direction

	direction[0]=-cos(phi)*sin(theta);
	direction[1]=-sin(phi)*sin(theta);
	direction[2]=-cos(theta);

	double philoc=gRandom->Uniform()*2*Pi;
	double r=CircRad*sqrt(gRandom->Uniform());
	double xx=r*cos(philoc);
	double yy=r*sin(philoc);
	position[0]= (Float_t) position[0]+cos(theta)*cos(phi)*xx-sin(phi)*yy;
	position[1]= (Float_t) position[1]+cos(theta)*sin(phi)*xx+cos(phi)*yy;
	position[2]= (Float_t) position[2]-sin(theta)*xx;
	polarization[0]=0.;
	polarization[1]=0.;
	polarization[2]=0.;


	//	strcpy(particle_name,pname);
	//

	//G4cout<<"particle Energy gen:"<<energy<<G4endl;
	fParticleGun->SetParticleDefinition(fPartiTable->FindParticle(pname));
	fParticleGun->SetParticleEnergy(energy*keV);
	fParticleGun->SetParticlePosition(G4ThreeVector(position[0]*cm, position[1]*cm, position[2]*cm));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(direction[0], direction[1], direction[2]));

}
