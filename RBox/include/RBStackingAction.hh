#ifndef RBStackingAction_h
#define RBStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

/// Stacking action class : manage the newly generated particles
///
/// One wishes do not track secondary neutrino.Therefore one kills it 
/// immediately, before created particles will  put in a stack.

class RBStackingAction : public G4UserStackingAction
{
  public:
    RBStackingAction();
    virtual ~RBStackingAction();
     
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        
};

#endif

