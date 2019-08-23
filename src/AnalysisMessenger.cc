/**
 * @file: AnalysisMessenger.cc
 * @class AnalysisMessenger
 * @brief Implementation of AnalysisMessenger, to control AnalysisManager with macros
 *
 *
 * @Author  : Hualin Xiao
 * @Date    : Dec., 2015
 * @Version : 1.00
 */


#include "globals.hh"

#include "AnalysisMessenger.hh"
#include "AnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithAnInteger.hh>
#include "G4SystemOfUnits.hh"

AnalysisMessenger::AnalysisMessenger(AnalysisManager* theAna)
: fAnalysis(theAna) { 
	
	fAnalysisDir = new G4UIdirectory( "/polar/Analysis/" );
	fAnalysisDir->SetGuidance("Analysis control.");

 	fSetOutputFileNameCmd= new G4UIcmdWithAString("/polar/Analysis/setOutputFileName", this);

 	fSetCalibrationFileNameCmd= new G4UIcmdWithAString("/polar/Analysis/setCalibrationFileName", this);

    fSetThresholdCmd=new G4UIcmdWithADoubleAndUnit("/polar/Analysis/setThreshold",this);
    fSetThresholdCmd->SetGuidance("Set recorded events, if there is no hit with energy over the threshold, this event will not be further processed");
    fSetThresholdCmd->SetParameterName("threshold",false);
    fSetThresholdCmd->SetRange("threshold>=0.");
    fSetThresholdCmd->SetUnitCategory("Energy");
    fSetThresholdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fSetMCThresholdCmd=new G4UIcmdWithADoubleAndUnit("/polar/Analysis/setMCThreshold",this);
    fSetMCThresholdCmd->SetGuidance("threshold to select hits for modulation curve  calculation (unit: ADC channel)");
    fSetMCThresholdCmd->SetParameterName("mcthreshold",false);

    
    fSetkBCmd=new G4UIcmdWithADouble("/polar/Analysis/setkB",this);
    fSetkBCmd->SetGuidance("Set kB constant (unit: mm/MeV) ");

    fSetRealSimCmd=new G4UIcmdWithAnInteger("/polar/Analysis/realSim",this);
    fSetRealSimCmd->SetGuidance("Set analysis level");
    fSetRealSimCmd->SetGuidance("0: Only simulation energy deposition");
    fSetRealSimCmd->SetGuidance("1: simulate energy resolution smearing");


    fSetPreSCmd=new G4UIcmdWithAnInteger("/polar/Analysis/setPreS",this);
    fSetPreSCmd->SetGuidance("Set prescaling of  single events");
    fSetPreSCmd->SetGuidance("1:  no prescaling of single events");

    fSetPreCCmd=new G4UIcmdWithAnInteger("/polar/Analysis/setPreC",this);
    fSetPreCCmd->SetGuidance("Set prescaling of cosmic rays");
    fSetPreCCmd->SetGuidance("1:  no prescaling of cosmic events");

    fSetStoreSingBarCmd=new G4UIcmdWithAnInteger("/polar/Analysis/storeSingleEvent",this);

	//fSetPolNbinsCmd= new G4UIcmdWithAnInteger("/polar/Analysis/setPolNbins", this);
	//fSetPhiNbinsCmd= new G4UIcmdWithAnInteger("/polar/Analysis/setPhiNbins", this);

   }

AnalysisMessenger::~AnalysisMessenger()
{
	delete fSetStoreSingBarCmd;
    delete fSetOutputFileNameCmd;
    delete fSetThresholdCmd;
    delete fSetMCThresholdCmd;
    delete fSetkBCmd;
    delete fSetRealSimCmd;
  //  delete fSetQuickLookCmd;
    delete fSetPreCCmd;
    delete fSetPreSCmd;
	//delete fSetDoCoincidenceCmd;

//	delete fSetPolNbinsCmd;
//	delete fSetPhiNbinsCmd;
}

void AnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if (command == fSetOutputFileNameCmd) { 
		
		fAnalysis->SetOutputFileName(newValue);
	}
    else if( command ==fSetThresholdCmd)
    {
        fAnalysis->SetThreshold(fSetThresholdCmd->GetNewDoubleValue(newValue));
    }
    else if( command ==fSetMCThresholdCmd)
    {
        fAnalysis->SetModulationCurveThreshold(fSetMCThresholdCmd->GetNewDoubleValue(newValue));
    }
    else if( command ==fSetkBCmd)
    {
        fAnalysis->SetkB(fSetkBCmd->GetNewDoubleValue(newValue));
    }
    else if( command ==fSetRealSimCmd)
    {
        fAnalysis->SetRealSim(fSetRealSimCmd->GetNewIntValue(newValue));
    }
	/*
    else if( command ==fSetQuickLookCmd)
    {
        fAnalysis->SetQuickLook(fSetQuickLookCmd->GetNewIntValue(newValue));
    }
	*/
    else if( command ==fSetCalibrationFileNameCmd)
    {
        fAnalysis->SetCalibrationFile(newValue);
    }
    else if(command==fSetPreSCmd)
    {
        fAnalysis->SetPrescalingSingle(fSetPreSCmd->GetNewIntValue(newValue));
    }
    else if(command==fSetPreCCmd)
    {
        fAnalysis->SetPrescalingCosmic(fSetPreCCmd->GetNewIntValue(newValue));
    }
    else if(command==fSetStoreSingBarCmd)
    {
        fAnalysis->SetStoreSingleBarEvent(fSetStoreSingBarCmd->GetNewIntValue(newValue));
    }
	/*
	else if(command==fSetDoCoincidenceCmd)
	{
        fAnalysis->SetDoCoincidence(fSetDoCoincidenceCmd->GetNewIntValue(newValue));
	}
	*/
	//else if(command==fSetPolNbinsCmd)fAnalysis->SetPolNbins(fSetPolNbinsCmd->GetNewIntValue(newValue));
	//else if(command==fSetPhiNbinsCmd)fAnalysis->SetPhiNbins(fSetPhiNbinsCmd->GetNewIntValue(newValue));

}
