#include "RBPrimaryGeneratorAction.hh"
#include "RBPrimaryGeneratorMessenger.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <random>
#include "G4UnitsTable.hh"

RBPrimaryGeneratorAction::RBPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  generatorMessenger = new RBPrimaryGeneratorMessenger(this);
  
  //default particle kinematics
  gunPosition = G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm);    //Origin is the default firing position for particles
  particleType = 0;                                       //Optical photon is the default particle type
}

RBPrimaryGeneratorAction::~RBPrimaryGeneratorAction() //destructor for RBPrimaryGeneratorAction
{
  delete fParticleGun;
}

void RBPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event
  G4ParticleDefinition* particle;
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore
  //

  G4double xPos, yPos, zPos, xVec, yVec, zVec, xRand, yRand, zRand;
  G4double sigmaAngle, theta, phi, momentum, sigmaMomentum, mass, pp, Ekin;

  if (particleType == 0) {
    //
    // Generate optical photons
    //
    particle = particleTable->FindParticle("opticalphoton");  //use the geant4 optical photon information
    fParticleGun->SetParticleDefinition(particle);            //set the particle gun to fire optical photons
    
    G4int npart = 1;
    for (G4int i=1; i<=npart; i++) {
      Ekin = (1.79+(3.26-1.79)*G4UniformRand() )*eV;   //1.79-3.26eV generated randomly with a uniform random distribution
      fParticleGun->SetParticleEnergy(Ekin);           //Set the particle gun to fire photons with our random energy
      
      fParticleGun->SetParticlePosition(gunPosition);  //Set the particle gun to fire from the set positions

      //
      //Firing trajectories based on different random distrubutions
      //

      //Uniform distribution on interval [0,1]
      theta = G4UniformRand()*180*deg;                 
      phi = G4UniformRand()*360.*deg;

      //Gaussian distribution with means = 3.14/*2 and sigmas = 1.18/*2
      //theta = G4RandGauss::shoot(3.14, 1.18)*rad;
      //phi = G4RandGauss::shoot(3.14*2, 1.18*2)*rad;

      //Exponential distribution
      //theta = G4RandExponential::shoot()*3.14*rad;
      //phi = G4RandExponential::shoot()*(3.14*2)*rad;

      yVec = std::sin(phi)*std::sin(theta); //set initial direction along y-axis
      zVec = std::cos(theta);               //set initial direction along z-axis
      xVec = std::cos(phi)*std::sin(theta); //set initial direction along x-axis
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec)); //set the particle gun initial direction based on our random vector.
      fParticleGun->GeneratePrimaryVertex(anEvent); //fire a photon.
    }
  } else {
    // You should not get to here
    G4cout << "Choose a particle type, dummy! " << G4endl;
  }
}