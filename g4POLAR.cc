/** @file  g4POLAR.cc
 *  @brief Main file of g4POLAR, simulation package for POLAR based on Geant4
 *
 *  @Features:
 *  1. Mass model implemented with  gdml. It is a simplified version of the mass model developed by Siwei Kong.
 *  2. Use General Particle Source
 *  3. Merlin's optical simulation results have been adopted.
 *  4. Energy resolution uses the model: sigma E=sqrt(a+b*E+c*E^2)
 *  5. Birks quenching effect taken into account, the kb constant measured by Xiaofeng is adopted.
 *  6. Support reading primary particle information from root file for very complex cases.
 *  7. Simulation output src/t2polar.C,  compatible with polar experimental data
 *  8. Visualization using Qt
 *  9. Spectra and modulation factor seem in good agreement with experiments
 *  10. Quick look contain modulation curves
 *  11. Run with the latest geant version 4.10
 *  12. Support parallel computing 
 *  
 *    
 *  @author Hualin Xiao (hualin.xiao@psi.ch)
 *  @version 1.0
 *  @date Nov. 24th, 2015
 */

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "AnalysisManager.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "PhysicsList.hh"
#include "time.h"
#include <TString.h>
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "GRBGenerator.hh"
#include "SolarFlareGenerator.hh"

#include "G4VUserPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"
//#include "FTFP_BERT.hh"
//FTFP_BERT.hh"
#include "G4PhysListFactory.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

void Help() 
{
    G4cout<<"POLAR GEANT4 Simulation package"<<G4endl;
    G4cout<<"Usage:"<<G4endl
        <<"./g4POLAR [OPTIONS] -m MACFILE -o OUTPUT"<<G4endl;
    G4cout<<"Options:"<<G4endl
        <<" -h                  print help information"<<G4endl;
    G4cout<<"Examples:"<<G4endl
        <<"./g4POLAR "<<G4endl
        <<"./g4POLAR -m test.mac -o test.root"<<G4endl
        <<"./g4POLAR -m vis.mac -o testvis.root"<<G4endl
        <<"./g4POLAR -ppol  <polarization angle>  -pphi polarization angle"<<G4endl
        <<G4endl;

}

int main(int argc,char **argv) 
{
    TString outputFilename;
    TString macFilename;
    if(argc==1)Help();
    int s=0;
    G4String sel;
	G4double ppol,pphi;
	ppol=-1;
	pphi=-1;
    while(s<argc-1)
    {

        sel=argv[++s];
        if(sel=="-h"||sel=="--help"||sel=="--h")
        {
            Help();
            return 0;
        }
        else if(sel=="-o")
        {
            outputFilename=argv[++s];
            if(!outputFilename.Contains(".root"))
            {
                Help();
                return 0;
            }
        }
		else if(sel=="-ppol")
		{
			ppol=atof(argv[++s]);
		}
		else if(sel=="-pphi")
		{
			pphi=atof(argv[++s]);
		}
        else if(sel=="-m")
        {
            macFilename=argv[++s];
            if(!macFilename.Contains(".mac"))
            {
                Help();
                return 0;
            }
        }

    }

    if(outputFilename==""&&macFilename=="")outputFilename="simout.root";
    if(macFilename!=""&&outputFilename=="")
    {
        outputFilename=macFilename; 
        outputFilename.ReplaceAll(".mac",".root");
    }
    

    time_t DateTime = time( NULL );
    CLHEP::HepRandom::setTheSeed( DateTime );

    AnalysisManager* analysisManager = AnalysisManager::GetInstance();
    //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);	
    analysisManager->SetOutputFileName(outputFilename.Data());

#ifdef G4MULTITHREADED  
    G4MTRunManager * runManager = new G4MTRunManager();
    G4int nThreads = G4Threading::G4GetNumberOfCores()-1;
    runManager->SetNumberOfThreads(nThreads);
#else
    G4RunManager * runManager = new G4RunManager(); 
#endif


  // G4String plname = "QGSP_BIC_EMY";  // set however you like ...
 //  G4PhysListFactory factory;
  // G4VModularPhysicsList* physlist = factory.GetReferencePhysList(plname);
   // runManager->SetUserInitialization(physlist);

    DetectorConstruction* detConstruction = new DetectorConstruction();

    runManager->SetUserInitialization(detConstruction);


    PhysicsList* physics = new PhysicsList();
    runManager->SetUserInitialization(physics);
//	  G4String plname = "QGSP_BIC_EMY";  // set however you like ...
 //  G4PhysListFactory factory;
  // G4VModularPhysicsList* physlist = factory.GetReferencePhysList(plname);
//    polarPhysicsList *physlist=new polarPhysicsList();
   // runManager->SetUserInitialization(physlist);


    PrimaryGeneratorAction *primarygen=new PrimaryGeneratorAction();
    runManager->SetUserAction(primarygen);

	if(pphi>=0||ppol>=0)
	{
		G4cout<<"# set polarization degree: "<<ppol<<G4endl;
		G4cout<<"# set polarization angle (degree): "<<pphi<<G4endl;
		primarygen->GetSolarFlareEventGun()->SetPolAngle(pphi*degree);
		primarygen->GetSolarFlareEventGun()->SetPolFrac(ppol);
	}




    runManager->SetUserAction(new RunAction());
    EventAction* evtAction = new EventAction();
    runManager->SetUserAction(evtAction);
    runManager->SetUserAction(new SteppingAction());


#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif
    if (macFilename=="") 
    {
#ifdef G4UI_USE
        G4UIExecutive * ui = new G4UIExecutive(argc,argv,"qt");
        ui->SessionStart();
        delete ui;
#endif
    } 
    else 
    {      
        G4UIExecutive * ui = new G4UIExecutive(argc,argv,"tcsh");
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute "; 
        G4String fileName = macFilename.Data(); 
        G4cout<<"Applying command: "<<command+fileName<<G4endl;
        UImanager->ApplyCommand(command+fileName);
        delete ui;

    }

    analysisManager->WriteCommand(macFilename.Data());
    //write mac file information to root file

#ifdef G4VIS_USE
    delete visManager;
#endif

    delete runManager;
    G4cout<<"Output File: "<<outputFilename<<G4endl;
    return 0;
}
