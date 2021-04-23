#include "RBSiPMSD.hh"
#include "SiPMhit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4SDManager.hh"

RBSiPMSD::RBSiPMSD(G4String SDname)
  : G4VSensitiveDetector(SDname),  photonHitCollection(0)
{  
  G4cout << "Creating SD with name: " << SDname << G4endl;
  collectionName.insert("SiPMHitCollection"); //'collectionName' is a protected data member of base class G4VSensitiveDetector
}

RBSiPMSD::~RBSiPMSD() = default; //set default destructor for RBSiPMSD

G4bool RBSiPMSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  auto preStepPoint = step->GetPreStepPoint();                 //get the prestep point
  auto touchable = preStepPoint->GetTouchable();               //get the touchable value from the prestep point        
  auto hitTime = preStepPoint->GetGlobalTime();                //get the time of the photon hit
  auto hit = new SiPMHit();                                    //define our hit
  hit->AddPhotonCount();                                       //Add the photon hit count to our hit variable
  photonHitCollection->insert(hit);                            //insert our hit variable to our hit collection.
  
  return true;
}

void RBSiPMSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create a collection
  photonHitCollection = new SiPMHitCollection(GetName(), collectionName[0]);
  //To insert the collection, we need to get an index for it. This index is unique to the collection. It is provided by the GetCollectionID() method.
  static G4int HCID = -1;
  if (HCID<0) { 
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(photonHitCollection); 
  }
  HCE->AddHitsCollection(HCID, photonHitCollection);
}

void RBSiPMSD::EndOfEvent(G4HCofThisEvent* HE)
{ }