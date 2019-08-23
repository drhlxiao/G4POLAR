/**
 * @file: SteppingAction.cc
 * @class SteppingAction
 *
 *
 * @Author  : Hualin Xiao (hualin.xiao@psi.ch)
 * @Date    : Oct., 2015
 * @Version : 1.0
 */



#include "G4SystemOfUnits.hh"
#include "TString.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "EventAction.hh"
//#include "Birks.hh"
#include "AnalysisManager.hh"
#include "SteppingAction.hh"

    SteppingAction::SteppingAction()
:	G4UserSteppingAction()
{

}

SteppingAction::~SteppingAction()
{ 
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

	AnalysisManager* analysisManager = AnalysisManager::GetInstance();

    analysisManager->SteppingAction(aStep);

}
