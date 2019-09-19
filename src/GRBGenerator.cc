
#include <TF1.h>
#include <TDatime.h>
#include <TRandom.h>
#include <TMath.h>
#include <sstream>
#include <cmath>

#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "GRBGenerator.hh"

const double PI=3.1415926;


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
		DnDE=A*TMath::Exp((alpha-beta)*   TMath::Log((alpha-beta)*E0/E100))*
			TMath::Exp((beta-alpha))* TMath::Exp(beta*TMath::Log(E/E100));
	}
	return DnDE;
	//checked by hualin Xiao, 20180127
}


GRBGenerator::GRBGenerator()
{
	pangle=0;
	theta=0*degree;
	phi=0*degree;
	nevents=0;
	alpha=-1.0,
	beta=-2.5,
	Epeak=200.*keV,
	e_min=5*keV;
	e_max=2000*keV; 
	SphRad=100*cm; //unit in cm
	CircRad=40*cm; //cm
	pol_angle=0; //degree
	pol_frac=0; //
	f1=NULL;
	px=py=pz=0;
	pos_x=pos_y= pos_z=0;
	pol_x=0;
	pol_y=0;
	pol_z=0;
	flux=0;
	total_events=0;
	fluxEmin=fluxEmax=0;

}
GRBGenerator::~GRBGenerator()
{
	delete f1;
}
void GRBGenerator::Init()
{
	G4cout<<"Initializing GRB event gun..."<<G4endl;
	f1=new TF1("band",band,e_min/keV,e_max/keV,4);

	f1->SetParameters(1.0,alpha,beta,Epeak/keV);//A alpha beta Epeak
	f1->SetNpx((int)e_max*10/keV);
	ss<<"## GRB settings:"<<G4endl
		<<"Alpha, beta, Epeak:"<<alpha<<" "<<beta<<" "<<Epeak/keV<<G4endl
		<<"Theta,phi (degree):"<<theta/degree<<" "<<phi/degree<<G4endl
		<<"Emin, Emax (keV):"<<e_min/keV<<" "<<e_max/keV<<G4endl
		<<"Polarization frac and angle (degree):"<<pol_frac<<" "<<pol_angle/degree<<G4endl
		<<"Sphere,Circle (cm):"<<SphRad/cm<<" "<<CircRad/cm<<G4endl;


	
	double int0=f1->Integral(e_min/keV, e_max/keV);
	double int1=f1->Integral(fluxEmin/keV, fluxEmax/keV);

	ss<<"Flux set (ph/cm2*s):"<<flux<<G4endl;
	ss<<"Flux in the given the energy range(keV): "<<fluxEmin/keV<<" - "<<fluxEmax/keV<<G4endl;
	if(flux>0)
	{
		total_events=(G4int)(int0*flux *PI*(CircRad/cm)*(CircRad/cm) /int1);
		ss<<"Total events to be simulated for the given flux:"<<total_events<<G4endl;
	}
	G4cout<<ss.str()<<G4endl;


}
G4int GRBGenerator::GetTotalNbEvents()
{
	
	if(!f1)Init();
	return total_events;
}


G4double GRBGenerator::GetCurrentPolarPhi()
{
	return pangle;
}

void GRBGenerator::GeneratePrimaryVertex(G4Event *anEvent, G4ParticleGun* gun)
{
	if(!f1)Init();
	nevents++;
	G4double energy;
	theta = theta/radian;
	phi = phi/radian; 
//	G4cout<<"theta,phi"<<theta/degree<<" "<<phi/degree<<" "<<G4endl;
	pos_x=SphRad*sin(theta)*cos(phi);
	pos_y=SphRad*sin(theta)*sin(phi);
	pos_z=SphRad*cos(theta);
//	G4cout<<"Center: "<<pos_x/cm<<" "<<pos_y/cm<<" "<<pos_z/cm<<G4endl;

	//center of the circle
	double* d=new double[3];

	d[0]=-1*sin(theta)*cos(phi);
	d[1]=-1*sin(theta)*sin(phi);
	d[2]=-1*cos(theta);


	energy =  f1->GetRandom()*keV;
	double philoc=gRandom->Uniform()*2*PI;
	G4double r=CircRad*sqrt(gRandom->Uniform());
	G4double xx=r*cos(philoc);
	G4double yy=r*sin(philoc);
	pos_x	= (G4double) (pos_x+cos(theta)*cos(phi)*xx-sin(phi)*yy);
	pos_y	= (G4double) (pos_y+cos(theta)*sin(phi)*xx+cos(phi)*yy);
	pos_z   = (G4double) (pos_z-sin(theta)*xx);


	double modul2= d[1]*d[1]+d[2]*d[2];
	double e_perpend[3];
	if (modul2 > 0.){
		e_perpend[0] = 0;
		e_perpend[1]=-(1./sqrt(modul2))*d[2]; 
		e_perpend[2]=(1./sqrt(modul2))*d[1];
	}
	else{
		e_perpend[0]=0;
		e_perpend[1]=1;
		e_perpend[2]=0;
	}
	double e_paralle[3];
	e_paralle[0]=e_perpend[1]*d[2]-e_perpend[2]*d[1];
	e_paralle[1]=-e_perpend[0]*d[2]+e_perpend[2]*d[0];
	e_paralle[2]=e_perpend[0]*d[1]-e_perpend[1]*d[0];


	pangle=pol_angle/radian;

	if (gRandom->Uniform()>pol_frac){
		pangle=gRandom->Uniform()*2*PI;
	}
	pol_x = (G4double) (cos(pangle)*e_paralle[0] + sin(pangle)*e_perpend[0]);
	pol_y = (G4double) (cos(pangle)*e_paralle[1] + sin(pangle)*e_perpend[1]);
	pol_z = (G4double) (cos(pangle)*e_paralle[2] + sin(pangle)*e_perpend[2]);

	px=d[0];
	py=d[1];
	pz=d[2];

	position=G4ThreeVector(pos_x,pos_y,pos_z);
	direction=G4ThreeVector(px,py,pz);
	polarization=G4ThreeVector(pol_x,pol_y,pol_z);
	
//	G4cout<<"Position:"<<pos_x/cm<<" "<<pos_y/cm<<" "<<pos_z/cm<<G4endl;
//	G4cout<<"Direction:"<<px<<" "<<py<<" "<<pz<<G4endl;
//	G4cout<<"Polarization:"<<pol_x<<" "<<pol_y<<" "<<pol_z<<G4endl;
//	G4cout<<"Energy:"<<energy/keV<<G4endl;


	gun->SetParticlePosition(position);
	gun->SetParticleEnergy(energy);
	gun->SetParticleMomentumDirection(direction);
	gun->SetParticlePolarization(polarization);
	gun->GeneratePrimaryVertex(anEvent); 

}
