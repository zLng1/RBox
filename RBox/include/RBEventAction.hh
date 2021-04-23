#ifndef RBEventAction_h
#define RBEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RBRunAction;

class RBEventAction : public G4UserEventAction
{
public:
  RBEventAction(RBRunAction* runAction);
  virtual ~RBEventAction();
  
  virtual void  BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);
  void AddELoss(G4double eLoss);
  
private:
  RBRunAction*  fRunAction;
  G4double totalEloss;
  G4int totalSteps;
};

#endif

    
