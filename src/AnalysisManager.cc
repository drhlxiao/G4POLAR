/**
 * @file: AnalysisManager.cc
 * @brief AnalaysisManager
 *
 * @todo  
 *      add a class AnalaysisManagerMesseger to control the data analysis with geant4 macros
 *      simulate CT
 *
 * @Author  : Hualin Xiao (hualin.xiao@psi.ch)
 * @Date    : Nov., 2015
 * @Version : 1.0
 */
#include <fstream>
#include <string>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TString.h"
#include "TH2F.h"
#include "TRandom.h"
#include "string.h"
#include "TNamed.h"
#include "TVectorD.h"

#include "Randomize.hh"
#include <CLHEP/Random/RandFlat.h>

#include "G4ThreeVector.hh"
#include "G4EmSaturation.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "AnalysisMessenger.hh"

#include "t2polar.h"
#include "lib_det.h"
#include "Coincidence.hh"
#include "PrimaryGeneratorAction.hh"

#include "DetResponse.hh"
#include "AnalysisManager.hh"
#include "GRBGenerator.hh"
#include "globallib.hh"

const G4int cosmic_num_trigger_limit=10;

const float radTodeg=180/3.1415926;


AnalysisManager* AnalysisManager::fManager = 0;
AnalysisManager* AnalysisManager::GetInstance()
{
	if(!fManager) 
	{
		fManager = new AnalysisManager();
	}
	return fManager;
}	
AnalysisManager::AnalysisManager() 
{
	/// Birks effect, The kb constant was measured by Xiaofeng
	fkB=0.143*mm/MeV;
	fRealSim=0;
	fThreshold=5*keV;
	fModulationCurveThreshold=5*keV;
	fCalibrationFile="";
	fPositronEvents=0;
	doCoincidence=0;
	fPolNbins=1;
	fPhiNbins=1;

	fStoreSingleBarEvent=1;
	fAnalysisMessenger=new AnalysisMessenger(this);
	detRes=new DetResponse();
	h2hit=new TH2F("h2hit","h2hit;column;row",40,0,40,40,0,40);
	SetPolarMapBinLabels(h2hit);

	h2trig=new TH2F("h2trig","hardware trigger pattern;column;row",40,0,40,40,0,40);
	SetPolarMapBinLabels(h2trig);
	h2trig_prescaled=new TH2F("h2trig_prescaled",
			"hardware trigger pattern after prescaling;column;row",40,0,40,40,0,40);

	SetPolarMapBinLabels(h2trig_prescaled);
	//h2na22=new TH2F("h2na22","h2na22 coincidence hit map;column;row",40,0,40,40,0,40);

	h1xi=new TH1F("h1xi","Azimuthal Angle; Azimuthal Angle (degree);Counts",40,0,360);
	h1xi2=new TH1F("h1xi2",
			"Azimuthal Angle using smeared energy and non-uniform threshold; Azimuthal Angle (degree);Counts",40,0,360);

	h1stat=new TH1F("h1stat","simulated, positrons, detected, coincidence; statistic; Number",10,0,10);

	h2hit_firstcompt=new TH2F("h2hit_firstcompt","first Compton scattering bars;column;row",40,0,40,40,0,40);
	SetPolarMapBinLabels(h2hit_firstcompt);
	h1ncompt=new TH1F("h1ncompt","Compton scatterings in PS bars; ncompt;Counts",20,0,20); // 

	hene=new TH1F("hene","Initial Energy spectrum; Energy (keV);Counts",300,0,1e4); //
	//hene->SetBit(TH1::kCanRebin);
	CanRebin(hene);

	hmrate=new TH1F("hmrate","module event rate; Module ID; Events;", 25,0,25);

	hx=new TH1F("hx","x of incident event; x (mm);Counts",300,-300,300); //
	//hx->SetBit(TH1::kCanRebin);
	hy=new TH1F("hy","y of incident event; y (mm);Counts",300,-300,300); //
	//hy->SetBit(TH1::kCanRebin);
	hz=new TH1F("hz","z of incident event; z (mm);Counts",300,-300,300); //
	h1Esum=new TH1F("h1Esum","Energy sum;Esum, Counts",500,0,2000); //
	//hz->SetBit(TH1::kCanRebin);

	CanRebin(hx);
	CanRebin(hy);
	CanRebin(hz);

	hdx=new TH1F("hdx","dx of incident event; dx (mm);Counts",300,-1,1); //
	//hdx->SetBit(TH1::kCanRebin);
	hdy=new TH1F("hdy","dy of incident event; dy (mm);Counts",300,-1,1); //
	//hdy->SetBit(TH1::kCanRebin);
	hdz=new TH1F("hdz","dz of incident event; dz (mm);Counts",300,-1,1); //
	//hdz->SetBit(TH1::kCanRebin);


	hpolz=new TH1F("hpolz","polarization vector ; Z ;Counts",300,-1,1); //
	hpoly=new TH1F("hpoly","polarization vector ; Y ;Counts",300,-1,1); //
	hpolx=new TH1F("hpolx","polarization vector;  X;Counts",300,-1,1); //
	hpol_ang=new TH1F("hpol_ang","polarization angle; Polarization Angle (w.r.t X x M);Counts",300,0,360); //

	hmaxedep=new TH1F("hmaxedep","Energy deposition of the bar with the maximum energy depsoition using pm0 without cuts ;Energy; Counts",
			300,0,500); //
	CanRebin(hmaxedep);
	hsecedep=new TH1F("hsecedep",
			"Energy deposition of the bar with the second maximum energy deposition using pm0 without cuts ;Energy; Counts",
			300,0,500); //
	CanRebin(hsecedep);
	hbardist=new TH1F("hbardist","bar distance of between the two bars pm0 without cuts ;distance (mm); Counts",300,0,300); //
	CanRebin(hbardist);

	htrackdist=new TH1F("htrackdist","bar distance of between the bars of first two interactions; distance (mm); Counts",300,0,300); //
	CanRebin(htrackdist);




	fPresCosmic=1;
	fPresSingle=1;
	grbGun=NULL;
	//single and cosmic event counters

}
AnalysisManager::~AnalysisManager() 
{
	delete emSat;
	delete tp;
	delete fAnalysisMessenger;
	delete detRes;
	delete h2hit;
	delete h2trig;
	delete h2trig_prescaled;
	delete h1xi;
	delete h1xi2;
	delete h2hit_firstcompt;
	delete h1ncompt;
	delete hx;
	delete hy;
	delete hz;
	delete hdx;
	delete hdy;
	delete hdz;
	delete hene;
	delete hpolx;
	delete hpoly;
	delete hpolz;
	delete hpol_ang;

	delete hmaxedep;
	delete hsecedep;
	delete hbardist;
	delete htrackdist;

}

///

void AnalysisManager::BeginOfRunAction(const G4Run *run)
{
	emSat=new G4EmSaturation(fkB*mm/MeV);	
	CreateFile();
	CreateTree();
	detRes->LoadResponseMatrixFrom(fCalibrationFile);
	fTFile->cd();
	fnEvents=0;
	fnAccepted=0;



	runManager = G4RunManager::GetRunManager();
	primaryAction = (PrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();
	grbGun=primaryAction->GetGRBEventGun();
	//init grbGun

}

void AnalysisManager::EndOfRunAction(const G4Run *run)
{

	WriteConfigurations();
	std::stringstream ss;
	ss<<"========================================================================"<<G4endl;
	ss<<"Simulated Events: "<<fnEvents<<G4endl;
	ss<<"Quick look modulatoin curve threshold (keV): "<<fModulationCurveThreshold/keV<<G4endl;
	ss<<"Software pattern threshold: (keV): "<<fThreshold/keV<<G4endl;
	ss<<"Prescaling single: "<<fPresSingle<<G4endl;
	ss<<"Prescaling cosmic: "<<fPresCosmic<<G4endl;
	ss<<"Total number of Single trigger Events: "<<detRes->GetNumSingleEvents()<<G4endl;
	ss<<"Total number of cosmic trigger Events: "<<detRes->GetNumCosmicEvents()<<G4endl;
	ss<<"cosmic Events: "<<detRes->GetNumCosmicEvents()<<G4endl;
	ss<<"Events in output tree: "<<tp->tree->GetEntries()<<G4endl;
	ss<<"Events with n>=2 && n<cosmic: "<<fnAccepted<<G4endl;
	ss<<"Events after prescaling using energy after smearing real and threshold:"<<tp->nrecorded<<G4endl;
	G4int na22positrons=0;
	if(primaryAction)
	{
		na22positrons=primaryAction->GetSimulatedPositrons();
		ss<<"Simulated  Positrons:"<<na22positrons<<G4endl;
		ss<<"Coincidence  Positrons:"<<fPositronEvents<<G4endl;
		grbGun=primaryAction->GetGRBEventGun();
		if(grbGun){
			G4String grbMessage=grbGun->GetMessage();
			ss<<grbMessage.data()<<G4endl;
			G4int total=grbGun->GetTotalNbEvents();
			G4double norfactor=(G4double)total/(G4double)fnEvents;
			ss<<"Modulation curve normalization factor (Nflux/Nsim):"<<norfactor<<G4endl;
			//		h1xi->Scale(norfactor);
			h1xi->SetTitle(Form("Modulation curve normalization factor (Nflux/Nsim): %f", norfactor));
			h1xi2->SetTitle(Form("Modulation curve normalization factor (Nflux/Nsim): %f", norfactor));
			//		h1xi2->Scale(norfactor);




		}
	}
	ss<<"Entries in the quicklook modulation curve:"<<h1xi->GetEntries()<<G4endl;
	ss<<"Entries in the realistic modulation curve:"<<h1xi2->GetEntries()<<G4endl;




	summary=ss.str();
	G4cout<<ss.str();
	h1stat=new TH1F("h1stat","simulated, positrons, single, cosmic, tree_entries, t_hd_accepted, coincidence; statistic; Number",10,0,10);
	h1stat->SetBinContent(1, fnEvents);
	h1stat->SetBinContent(2, na22positrons);
	h1stat->SetBinContent(3, detRes->GetNumSingleEvents());
	h1stat->SetBinContent(4, detRes->GetNumCosmicEvents());
	h1stat->SetBinContent(5, tp->tree->GetEntries());
	h1stat->SetBinContent(6, fnAccepted);
	h1stat->SetBinContent(7, fPositronEvents);
	G4cout<<"Writing file..."<<G4endl;
	WriteFile();
	G4cout<<"Closing file..."<<G4endl;
	fTFile->Close();
}


void AnalysisManager::WriteCommand(G4String fname)
{
	fTFile->cd();
	std::ifstream ifs(fname.data());
	std::string cs;
	getline (ifs, cs, (char) ifs.eof());
	TNamed macros;
	macros.SetTitle(cs.c_str());
	macros.Write("macros");
	TNamed tfilen;
	tfilen.SetTitle(fname.data());
	tfilen.Write("macFileName");
}

void AnalysisManager::WriteConfigurations()
{
	fTFile->cd();
	fTFile->mkdir("settings");
	fTFile->cd("settings");	
	TVectorD v(1);
	v[0]=fkB;
	v.Write("kB");
	v[0]=fThreshold/keV;
	v.Write("common_threshold");
	v[0]=fModulationCurveThreshold/keV;
	v.Write("modulationcurve_threshold");

	detRes->WriteResponses(fTFile);

}
void AnalysisManager::BeginOfEventAction(const G4Event *event)
{
	fnEvents++;
	esum=0;
	for (G4int i=0; i<25; i++) 
	{
		mrate[i]=0;
		for (G4int j=0; j<64; j++) 
		{

			fTotalEdepSum[i][j] = 0.0;
			fVisibleEdepSum[i][j] = 0.0;
			fCollectedEdepSum[i][j] = 0.0;
		}
	}
	tp->DefaultValues();
	nGammaTracks=0;
	ncompt=0;

}
void AnalysisManager::EndOfEventAction(const G4Event *event)
{
	h1ncompt->Fill(ncompt);

	FillData(fTotalEdepSum, fVisibleEdepSum, fCollectedEdepSum); 
	if(tp->nhits<1)return;
	h1Esum->Fill(esum);

	if(fRealSim>=1)
	{
		detRes->SmearEnergy(tp->pm, tp->pm3); 
		//detRes->SimCrosstalkAndNonuniformity(tp->pm3, tp->adc); 
		detRes->SimTrigger(tp->pm3,fPresSingle, fPresCosmic, tp);
	}
	tp->FillPolarization(fModulationCurveThreshold/keV);
	if(tp->xiok[0])h1xi->Fill(tp->xideg[0]);
	if(tp->xiok[1])h1xi2->Fill(tp->xideg[1]);
	if(fRealSim>=1)tp->FillTriggerMap(h2trig, h2trig_prescaled);
	fTFile->cd();

	hmaxedep->Fill(tp->max_dep[0]);
	hsecedep->Fill(tp->sec_dep[0]);
	hbardist->Fill(tp->bar_dist[0]);
	htrackdist->Fill(tp->track_dist[1]);




	//if(nentries	%1000==0)G4cout<<"Entries in the tree:"<<nentries<<G4endl;
	//int nentries=tp->tree->GetEntries();
	
	if(fStoreSingleBarEvent==1||(tp->nhits>1||tp->ntrig>1))
	{
		tp->tree->Fill();
	}
}

void AnalysisManager::SteppingAction(const G4Step *aStep)
{
	///Stepping Action
	G4TouchableHandle preTouchable = aStep->GetPreStepPoint()->GetTouchableHandle();
	G4String preVolName = preTouchable->GetVolume(0)->GetName();
	const G4Track* track = aStep->GetTrack();
	G4int barPosition;
	G4int modulePosition;
	G4int copyNbBar;
	G4int copyNbModule;


	G4double totalEdep, visEdep, collectedEdep;
	totalEdep=visEdep=collectedEdep=0;

	if (preVolName == "Scintillator") 
	{

		copyNbBar= preTouchable->GetVolume(0)->GetCopyNo();
		copyNbModule= preTouchable->GetVolume(1)->GetCopyNo();

		modulePosition=get_mc_module_position(copyNbModule);
		barPosition=get_mc_bar_position(copyNbBar);
		//xiaohl 20180119
		//convert x,y to previous convention 
		//use get_bar_center to convert back to the space coordinates 




		G4double zz=track->GetPosition().z()/mm;
		G4double collectionEff=detRes->GetLightCollectionEfficiency(zz);
		//use Merlin's simulation result, see the document "New optical simulations" in svn
		totalEdep=aStep->GetTotalEnergyDeposit()/keV; 
		visEdep=emSat->VisibleEnergyDepositionAtAStep(aStep)/keV;
		//not supported for  geant versions < 4.10.03
		//xiaohl 20180119
		visEdep=totalEdep;
		collectedEdep=visEdep*collectionEff;
		if (totalEdep> 0.0) 
		{
			AddTotalEnergy(modulePosition, barPosition, totalEdep);
			AddVisibleEnergy(modulePosition, barPosition, visEdep);
			AddCollectedEnergy(modulePosition, barPosition, collectedEdep);


			int barnb=get_index(modulePosition,barPosition);
			G4String processName=aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
			//G4int IDp =  aStep->GetTrack()->GetParentID();
			//if(IDp==0)
			if(processName=="compt"||processName=="phot"||processName=="pol-compt")
			{
				G4double deltaE=-aStep->GetDeltaEnergy()/keV;
				//G4cout<<"deltaE: "<<deltaE<<" "<<totalEdep<<G4endl;
				tp->FillTrack(ncompt, barnb, deltaE, processName);
				//store track information
				if(ncompt==0)
				{
					int row,col;
					row=get_row(barnb);
					col=get_column(barnb);
					h2hit_firstcompt->Fill(col+0.5,row+0.5);
				}
				ncompt++;
			}




		}	
	}



}

void AnalysisManager::CreateFile()
{
	G4cout<<"Output filename: "<<fOutputFile<<G4endl;
	fTFile = new TFile(fOutputFile.Data(),"recreate");

}
void AnalysisManager::CreateTree() 
{
	G4cout<<"Creating root tree"<<G4endl;
	fTFile->cd();
	tp=new t2polar(fRealSim);
	tp->tree->AutoSave();
}
void AnalysisManager::FillData(G4double total[25][64], G4double  vis[25][64], G4double coll[25][64])
{
	int nhits=0;
	for(G4int i=0; i<25; i++) 
	{
		int nn=0;
		bool is_triggered=false;
		for(G4int j=0; j<64; j++) 
		{
			int index=get_index(i,j);
			//tp->pm[index]=total[i][j];
			tp->pm[index]=vis[i][j];
			tp->pm2[index]=coll[i][j];

			if(total[i][j]>fThreshold/keV) 
			{
				nn++;
				nhits++;
				int row,col;
				row=get_row(index);
				col=get_column(index);
				h2hit->Fill(col+0.5,row+0.5);
				tp->pattern[index]=true;
				esum+=total[i][j];
				is_triggered=true;
			} 
		}
		tp->mhits[i]=nn;
		hmrate->Fill(i+0.5);

	}

	tp->nhits=nhits;
	tp->evnt++;


	if(nhits>=2&&nhits<=15)
	{
		// tp->recorded=1;
		fnAccepted++;
	}

	runManager = G4RunManager::GetRunManager();
	primaryAction = (PrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();


	//primary particle information
	G4ThreeVector position,direction,polarization;
	G4double energy;
	primaryAction->GetParticleInformation(position,direction,energy, polarization);
	hene->Fill(energy);

	G4double pos_x,pos_y,pos_z;
	G4double dir_x,dir_y,dir_z;

	pos_x=position.getX();
	pos_y=position.getY();
	pos_z=position.getZ();


	dir_x=direction.getX();
	dir_y=direction.getY();
	dir_z=direction.getZ();

	hx->Fill(pos_x);
	hy->Fill(pos_y);
	hz->Fill(pos_z);

	hdx->Fill(dir_x);
	hdy->Fill(dir_y);
	hdz->Fill(dir_z);

	hpolx->Fill(polarization.getX());
	hpoly->Fill(polarization.getY());
	hpolz->Fill(polarization.getZ());

	CopyThreeVector(position, tp->source_pos);
	CopyThreeVector(direction, tp->source_dir);
	CopyThreeVector(polarization, tp->source_pol);
	tp->theta=180-direction.theta()*radTodeg;
	tp->phi=direction.phi()*radTodeg;
	if(tp->phi<0)tp->phi=tp->phi+180;

	//G4ThreeVector e_perpend;
	//double mod2=direction.y()*direction.y()+direction.z()*direction.z();
	//if(mod2>0){
	//	e_perpend=G4ThreeVector(0, -direction.z(), direction.y());
	//}
	//else
	//{
	//	e_perpend=G4ThreeVector(0, 1, 0);
	//}

	tp->polang=-1;
	if(grbGun)
	{
		tp->polang=grbGun->GetCurrentPolarPhi()/degree;
	}

	//tp->polang=e_perpend.angle(polarization)*radTodeg;
	hpol_ang->Fill(tp->polang);
	//G4cout<<polarization<<" "<<tp->pang<<G4endl;
	//G4cout<<tp->theta<<" "<<tp->phi<<G4endl;
	//Note polarization degree must be randomized by users other wise it is zero
	tp->ekin=energy;
	//G4cout<<tp->ekin<<G4endl;
	//tested in Jan.
}

void AnalysisManager::WriteFile()
{

	G4cout<<"Writing Nb. of events: "<<fnEvents<<G4endl;
	fTFile->cd();
	if(	summary.size()>0)
	{
		TNamed summ;
		summ.SetTitle(summary.c_str());
		summ.Write("summary");
	}
	hmrate->Write();
	tp->tree->Write();
	fTFile->mkdir("quicklook");
	fTFile->cd("quicklook");
	h2hit->Write();
	h2trig->Write();
	h2trig_prescaled->Write();
	h1xi->Write();
	h1Esum->Write();
	h1xi2->Write();
	h2hit_firstcompt->Write();
	h1ncompt->Write();
	hene->Write();
	hx->Write();
	hy->Write();
	hz->Write();
	//h2na22->Write();
	hdx->Write();
	hdy->Write();
	hdz->Write();
	hpolx->Write();
	hpolz->Write();
	hpoly->Write();
	hpol_ang->Write();
	h1stat->Write();

	hmaxedep->Write();
	hsecedep->Write();
	hbardist->Write();
	htrackdist->Write();
	G4cout<<"#Events written to root:"<<tp->tree->GetEntries()<<G4endl;
}

