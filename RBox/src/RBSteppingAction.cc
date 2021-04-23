#include "RBSteppingAction.hh"
#include "RBEventAction.hh"
#include "RBDetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

RBSteppingAction::RBSteppingAction(
                      RBEventAction* eventAction)
  : G4UserSteppingAction(),
    fEventAction(eventAction)
{ }

RBSteppingAction::~RBSteppingAction() = default; //set default destructor for RBSteppingAction

void RBSteppingAction::UserSteppingAction(const G4Step* step)
{
    // Collect energy and track length step by step
    
    // get volume of the current step
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit();
          
    if (edep > 0.) {
      fEventAction->AddELoss(edep); //if there was any energy loss, add it to the count.
    }
}