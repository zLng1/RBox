#include "RBActionInitialization.hh"
#include "RBRunAction.hh"
#include "RBEventAction.hh"
#include "RBSteppingAction.hh"
#include "RBPrimaryGeneratorAction.hh"
#include "RBStackingAction.hh"

RBActionInitialization::RBActionInitialization()
 : G4VUserActionInitialization()
{ }

RBActionInitialization::~RBActionInitialization() = default; //set default destructor for RBActionInitialization

void RBActionInitialization::BuildForMaster() const
{
  SetUserAction(new RBRunAction);
}

void RBActionInitialization::Build() const
{
  RBRunAction* runAction = new RBRunAction();               //For RunAction
  SetUserAction(runAction);                                 
  RBEventAction* evtAction = new RBEventAction(runAction);  //For EventAction
  SetUserAction(evtAction);
  SetUserAction(new RBPrimaryGeneratorAction);              //For PrimaryGeneratorAction
  SetUserAction(new RBStackingAction);                      //For StackingAction
  SetUserAction(new RBSteppingAction(evtAction));           //For SteppingAction
}  