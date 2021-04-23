#include "globals.hh"
#include "RBPrimaryGeneratorMessenger.hh"
#include "RBPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

RBPrimaryGeneratorMessenger::RBPrimaryGeneratorMessenger(
                                          RBPrimaryGeneratorAction* RBGun)
:RBAction(RBGun)
{
  gunDir = new G4UIdirectory("/RB/gun/");                                      //create new directory in qt5 ui
  gunDir->SetGuidance("PrimaryGenerator event control:");

  SetGunPositionCmd = new G4UIcmdWith3VectorAndUnit("/RB/gun/position", this); //create a new command to change the firing position
  SetGunPositionCmd->SetGuidance("Set particle gun position");                 //set description for tooltip
  SetGunPositionCmd->SetParameterName("X","Y","Z", true);                      //set the names of the parameters command accepts
  SetGunPositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);        
  SetGunPositionCmd->SetUnitCategory("Length");                                //this parameter is operates on position which is in units of length
}

RBPrimaryGeneratorMessenger::~RBPrimaryGeneratorMessenger() //destructor for RBPrimaryGeneratorMessenger
{
  delete SetGunPositionCmd;
  delete SetGunParticleType;
  delete gunDir;
}

void RBPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValues)
{ 
    if (command == SetGunPositionCmd ) {
      G4ThreeVector vec = SetGunPositionCmd->GetNew3VectorValue(newValues); //this vector is our new position
      RBAction->SetGunPosition(vec);                                        //updates the gun position based on the input from the command
    }
}