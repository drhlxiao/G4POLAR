/// @file PhysicsList.cc
/// @brief Implementation of the PhysicsList class
/// @Author  Hualin Xiao(hualin.xiao@psi.ch)
/// @History:
///  Nov. 26th, 2015, Hualin Xiao, use ModularPhysicsList

#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4DataQuestionaire.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"


PhysicsList::PhysicsList() :  G4VModularPhysicsList()
{
  G4int ver = 1;
  SetVerboseLevel(ver);

  // EM Physics
  RegisterPhysics( new G4EmLivermorePolarizedPhysics(ver) );

  // Synchroton Radiation & GN Physics
  //RegisterPhysics( new G4EmExtraPhysics(ver) );

  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );

  //G4RadioactiveDecayPhysics
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // Hadron Elastic scattering
//  RegisterPhysics( new G4HadronElasticPhysics(ver) );

  // Hadron Physics
 // RegisterPhysics( new G4HadronPhysicsQGSP_BERT(ver));

  // Stopping Physics
 // RegisterPhysics( new G4StoppingPhysics(ver) );

  // Ion Physics
 // RegisterPhysics( new G4IonPhysics(ver));
 // Neutron
 // RegisterPhysics( new G4NeutronTrackingCut(ver));


}

PhysicsList::~PhysicsList()
{
}


void PhysicsList::SetCuts()
{
    SetCutsWithDefault();   
 
}
