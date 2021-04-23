#include "RBRunAction.hh"
#include "RBPrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RBRunAction::RBRunAction()
 : G4UserRunAction(),
   fPhotons(0)
{  
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance(); // Register accumulable to the accumulable manager
  accumulableManager->RegisterAccumulable(fPhotons);                           //register the total number of photon hits in a run 
}

RBRunAction::~RBRunAction() = default; //set default destructor for RBRunAction

void RBRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  
  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void RBRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();    //the number of events in a single run
  if (nofEvents == 0) return;
  
  // Merge our accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Run conditions
  const RBPrimaryGeneratorAction* generatorAction = static_cast<const RBPrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String partName;
  if (generatorAction) 
  {
    G4ParticleDefinition* particle = generatorAction->GetParticleGun()->GetParticleDefinition(); //set up our particle gun for the run
    partName = particle->GetParticleName();                                                      //get the particle name we're firing
  }  
          
  // Print the total number of events results
  //
  if (IsMaster())
  {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------"
     << G4endl
     << "  The run was " << nofEvents << " events ";
  

  //Print the total number of photons that have hit the SiPM
  //
    G4cout
     << "; Number of photons " << fPhotons.GetValue()  << G4endl
     << "------------------------------------------------------------" << G4endl 
     << G4endl;
  }
}