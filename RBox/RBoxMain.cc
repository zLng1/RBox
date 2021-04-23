#include "G4Types.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "RBDetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4PhysListFactory.hh"
#include "G4OpticalPhysics.hh"
#include "RBActionInitialization.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

  namespace {
    void PrintUsage() {
      G4cerr << " Usage: " << G4endl;
      G4cerr << " LoopPanel [-m macro ] [-u UIsession] " << G4endl;
    }
  }

int main(int argc,char** argv)
{
  // Evaluate arguments
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String session;
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  // Detect interactive mode (if no macro provided) and define UI session
  G4UIExecutive* ui = nullptr;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }
  
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Set mandatory initialization classes
  runManager->SetUserInitialization(new RBDetectorConstruction);

  // Need to add code for activating optical physics processes from our physics process lists
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = NULL;
  G4String physName = "";
  char* path = getenv("PHYSLIST");
  if (path) {
      physName = G4String(path);
  } else {
      physName = "FTFP_BERT"; // default
  }
  // reference PhysicsList via its name
  if (factory.IsReferencePhysList(physName)) {
      phys = factory.GetReferencePhysList(physName);
  }
  //
  // Now add and configure optical physics
  //
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics(); 
  opticalPhysics->Configure(kAbsorption, true); 
  opticalPhysics->Configure(kBoundary, true);      
  
  phys->RegisterPhysics(opticalPhysics);
  phys->DumpList();
  
  //auto physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(phys);

  // Set user action initialization
  //
  runManager->SetUserInitialization(new RBActionInitialization());

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}