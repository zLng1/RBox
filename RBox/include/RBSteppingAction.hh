#ifndef RBSteppingAction_h
#define RBSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class RBDetectorConstruction;
class RBEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in B4aEventAction.

class RBSteppingAction : public G4UserSteppingAction
{
public:
  RBSteppingAction(RBEventAction* eventAction);
  virtual ~RBSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  RBEventAction*  fEventAction;  
};

#endif
