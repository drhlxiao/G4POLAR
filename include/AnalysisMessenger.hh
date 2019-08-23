//
/// \file /include/AnalysisMessenger.hh
/// \brief Definition of the AnalysisMessenger class
//

#ifndef AnalysisMessenger_h
#define AnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class AnalysisManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3Vector;

class AnalysisMessenger: public G4UImessenger 
{
     public:
     	     AnalysisMessenger(AnalysisManager* );
     	     ~AnalysisMessenger();
    
     	     void SetNewValue( G4UIcommand*, G4String );
     	     
     private:
     	     AnalysisManager* fAnalysis;
     	     G4UIdirectory*             fAnalysisDir;
            G4UIcmdWithAString *fSetOutputFileNameCmd;
            G4UIcmdWithAString *fSetCalibrationFileNameCmd;
            G4UIcmdWithAString *fExportResponseDataCmd;

            G4UIcmdWithADoubleAndUnit *fSetThresholdCmd;
            G4UIcmdWithADoubleAndUnit *fSetMCThresholdCmd;
            G4UIcmdWithAnInteger* fSetRealSimCmd;
            //G4UIcmdWithAnInteger* fSetQuickLookCmd;
            G4UIcmdWithAnInteger* fSetPreSCmd;
            G4UIcmdWithAnInteger* fSetPreCCmd;
            //G4UIcmdWithAnInteger* fSetDoCoincidenceCmd;

            G4UIcmdWithADouble* fSetkBCmd;

			 G4UIcmdWithAnInteger* fSetPolNbinsCmd;
             G4UIcmdWithAnInteger* fSetPhiNbinsCmd;

			 G4UIcmdWithAnInteger* fSetStoreSingBarCmd;
};

#endif
