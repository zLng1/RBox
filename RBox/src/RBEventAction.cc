#include "RBEventAction.hh"
#include "RBRunAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RBEventAction::RBEventAction(RBRunAction* runAction)
 : G4UserEventAction(),
   fRunAction(runAction)
{ }

RBEventAction::~RBEventAction() = default;

void RBEventAction::BeginOfEventAction(const G4Event* /*evt*/)
{
  // Initialize the total energy loss and the total number of steps
  totalEloss = 0.0;
  totalSteps = 0;
}

void RBEventAction::AddELoss(G4double eLoss)
{
  //Add the energy loss and the number of steps at each step
  totalEloss += eLoss;
  totalSteps += 1;
}

void RBEventAction::EndOfEventAction(const G4Event* evt )
{
  
  //Hits collections
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE) return;

  // Get hits collections IDs
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();  
  G4int HCID   = SDMan->GetCollectionID("SiPMHitCollection");

  auto hc = HCE->GetHC(HCID);
  if (hc->GetSize() > 0) {
    G4cout << "The size of the Hit Collection of This Event: " << hc->GetSize() << G4endl; //should never be greater than 1 for a photon event.
    fRunAction->CountPhoton();
  }
  G4cout << "Number of tracking steps: " << totalSteps << "     Total ELoss: " << G4BestUnit(totalEloss, "Energy")<< G4endl;
}  