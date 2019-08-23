/**
 * @file: RunAction.cc
 * @class RunAction
 *
 *
 * @Author  : Hualin Xiao (hualin.xiao@psi.ch)
 * @Date    : Oct., 2015
 * @Version : 1.0
 */

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "AnalysisManager.hh"
#include "RunAction.hh"

RunAction::RunAction()
 :	G4UserRunAction()
{ 
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
	G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
	AnalysisManager* analysisManager = AnalysisManager::GetInstance();
    analysisManager->BeginOfRunAction(run);
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    
    AnalysisManager* analysisManager = AnalysisManager::GetInstance();
    analysisManager->EndOfRunAction(aRun); 
	
	G4cout << "### This run is over." << G4endl;
	
}
