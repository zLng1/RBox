#include "RBStackingAction.hh"
#include "G4Track.hh"
#include "G4NeutrinoE.hh"


RBStackingAction::RBStackingAction()
{ }

RBStackingAction::~RBStackingAction() = default; //set default destructor for RBStackingAction

G4ClassificationOfNewTrack
RBStackingAction::ClassifyNewTrack(const G4Track* track)
{
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //kill secondary neutrino to reduce computation cost
  if (track->GetDefinition() == G4NeutrinoE::NeutrinoE()) return fKill;
  else return fUrgent;
}