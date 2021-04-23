#include "RBDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SubtractionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "RBSiPMSD.hh"


RBDetectorConstruction::RBDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true) //check for overlaps in geometry
{
  BoxXYZ = 20.0*cm; //Define the dimensions of the Box. 
}

//Destructor for RBDetector Construction.
RBDetectorConstruction::~RBDetectorConstruction() = default;


G4VPhysicalVolume* RBDetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();
  default_mat = nist->FindOrBuildMaterial("G4_AIR"); //get our default Air properties from Geant4
  Box_mat = nist->FindOrBuildMaterial("G4_Al");      //get our default Aluminum properties from Geant4

  // Necessary Values
  G4double world_sizeXYZ = 50*cm;  // For our world volume size.

  G4double padding_1 = 0.1*mm;   // For our Al foil wrapping
  G4double padding_2 = 0.2*mm;   // For our Al foil wrapping

  G4double SiPM_x = (padding_2 - padding_1)+0.5*cm; // define the 'depth' of the SiPM
  G4double SiPM_y = 1.09*cm; // define the 'length' of the SiPM
  G4double SiPM_z = 1.09*cm; // define the 'width' of the SiPM

  G4double Hole_x = (padding_2 - padding_1)+0.5*cm; // define the 'depth' of the hole for the SiPM
  G4double Hole_y = 1.1*cm; // define the 'length' of the hole for the SiPM
  G4double Hole_z = 1.1*cm; // define the 'width' of the hole for the SiPM

  G4RotationMatrix* yRot = new G4RotationMatrix; // need to rotate our hole and SiPM so they align properly relative to the box
  yRot->rotateY(90*deg);  //rotate about the y-axis 90 degrees. 
    
  //  
  // Building the world volume where the simulation takes place.
  //  

  //Build the world geometry as a big box with dimension of world_sizeXYZ^3.
  G4Box* solidWorld =    
    new G4Box("World",                                              //declare its name
	      0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);   //declare its dimensions
      
  G4LogicalVolume* WorldLV =                 //Making our Logical volume for the world        
    new G4LogicalVolume(solidWorld,          //declare its geometric solid
                        default_mat,         //declare its material (Air here)
                        "WorldLV");          //declare its name
  
  G4VPhysicalVolume* WorldPV =               //Making our Physical volume for the world
    new G4PVPlacement(0,                     //declare no rotation
                      G4ThreeVector(),       //place at (0,0,0) (This denotes the origin for the whole simulation geometry)
                      WorldLV,               //declare its logical volume
                      "WorldPV",             //declare its name
                      0,                     //declare its mother volume (It has none as it is the mother to all other volumes in the simulation).
                      false,                 //no boolean operations (such as subtraction)
                      0,                     //copy number
                      fCheckOverlaps);       //checking overlaps 

  //
  // Making the box geometry:
  //

  // Build a slightly smaller block which we subtract from the larger block which will create the box.
  G4Box* smallerBox =    
    new G4Box("SmallerBox",                                                   //declare its name
	      0.5*BoxXYZ+padding_1, 0.5*BoxXYZ+padding_1, 0.5*BoxXYZ+padding_1);    //declare its size

  // Build the slightly bigger block which we turn into a box.
  G4Box* biggerBox =    
    new G4Box("BiggerBox",                                                    //declare its name
	0.5*BoxXYZ+padding_2, 0.5*BoxXYZ+padding_2, 0.5*BoxXYZ+padding_2);          //declare its dimensions

  // Subtract the smaller block from the bigger block to make our reflective Aluminum box.
  G4SubtractionSolid* RBox =
    new G4SubtractionSolid("RBox", biggerBox, smallerBox);
    
  //
  // Building the SiPM:
  //

  G4Box* solidSensor = new G4Box("SiPM", SiPM_x/2, SiPM_y/2, SiPM_z/2); //Build the SiPM geometry as a small wall-like rectangular prism

  sipmLV =                                          //Making our Logical volume for the SiPM
    new G4LogicalVolume(solidSensor,                //declare its geometric solid
		           	       default_mat,                 //declare its material (we also used air here to ensure photons that hit the SiPM exit the box for visualization sake)
			                 "sipmLV");                   //declare its name
  
  G4PVPlacement* sipmPV =                           //Making our Physical volume for the SiPM
    new G4PVPlacement(yRot,                         //rotate about y-axis 90 degrees
					     G4ThreeVector( 0, 0, 0.5*BoxXYZ ),   //at (0,0,0)
					     sipmLV,                              //its logical volume
					     "sipmPV",                            //its name
					     WorldLV,                             //its mother  volume
					     false,                               //no boolean operation
					     0,                                   //copy number
					     fCheckOverlaps);                     //checking overlaps

  // SiPM sensor visualization attribute
  G4VisAttributes photonDetectorVisAtt(G4Colour::Red()); //makes the SiPM red so we can see it easier
  photonDetectorVisAtt.SetForceWireframe(true);          //makes the SiPM visualization wire frame so we can see photons through it
  photonDetectorVisAtt.SetLineWidth(3.);                 //setting the line width
  sipmLV->SetVisAttributes(photonDetectorVisAtt);        //give the SiPM these settings

  //
  // Making a hole for the SiPM.
  //

  G4Box* solidSensorHole = new G4Box("Hole", Hole_x/2, Hole_y/2, Hole_z/2); //Create a opening hole in one wall of the box for installing the SiPM
  G4ThreeVector holeTrans( 0, 0, 0.5*BoxXYZ);                               //this is where we are going to place our hole.

  //Create the hole by subtracting our hole 'box' from the geometry of the box.
  G4SubtractionSolid* BoxHole =
    new G4SubtractionSolid("BoxHole", RBox, solidSensorHole, yRot, holeTrans);

  //
  // Building the reflective Aluminum box:
  //

  //Define our logical volume for the Box
  G4LogicalVolume* BoxLV =                          //Making our Logical volume for the SiPM
    new G4LogicalVolume(BoxHole,                    //declare its geometric solid
			Box_mat,                                      //declare its material (Aluminum here)
			"BoxLV");                                     //declare its name

  //Define our Physical volume for the Box
  G4PVPlacement* BoxPV = new G4PVPlacement(0,        //declare no rotation
					     G4ThreeVector(),                      //place at (0,0,0)
					     BoxLV,                                //delare its logical volume
					     "BoxPV",                              //define its name
					     WorldLV,                              //delare its mother  volume
					     false,                                //declare that we are using no boolean operation
					     0,                                    //copy number
					     fCheckOverlaps);                      //checking overlaps

  //Make our world volume invisible.
  WorldLV->SetVisAttributes (G4VisAttributes::GetInvisible()); 

  //
  //Optical and Surface Properties
  //

  //The energy range of our optical photons (roughly aligns with the visible spectrum)
  G4double photonEnergy[] =
  {1.79*eV,1.82*eV,1.85*eV,1.88*eV,1.91*eV,
   1.94*eV,1.97*eV,2.00*eV,2.03*eV,2.06*eV,
   2.09*eV,2.12*eV,2.15*eV,2.18*eV,2.21*eV,
   2.24*eV,2.27*eV,2.30*eV,2.33*eV,2.36*eV,
   2.39*eV,2.42*eV,2.45*eV,2.48*eV,2.51*eV,
   2.54*eV,2.57*eV,2.60*eV,2.63*eV,2.66*eV,
   2.69*eV,2.72*eV,2.75*eV,2.78*eV,2.81*eV,
   2.84*eV,2.87*eV,2.90*eV,2.93*eV,2.96*eV,
   2.99*eV,3.02*eV,3.05*eV,3.08*eV,3.11*eV,
   3.14*eV,3.17*eV,3.20*eV,3.23*eV,3.26*eV};

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double); //define the size of all our arrays based on their interaction with photons of these energies

  //
  // Define optical material properties for air
  //

  //Refractive index of air is 1.0
  G4double RefractiveIndex_Air[] =
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  
  assert(sizeof(RefractiveIndex_Air) == sizeof(photonEnergy)); //assert the size of this array to be the same as the photon energy array
  
  //Gives the attenuation length of photons to be 10m at all energies. Note this is guessed value.
  G4double Absorption_Air[] =
    {10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m,
     10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m,
     10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m,
     10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m,
     10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m
    }; 

  assert(sizeof(Absorption_Air) == sizeof(photonEnergy)); //assert the size of this array to be the same as the photon energy array

  //Allows for 1% chance of optical photons being absorbed by air, otherwise they will reflect/refract through the air (continue their track)
  G4double AirReflect[]      =
    {0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99,
     0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99,
     0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99,
     0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99,
     0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99
    }; 

  assert(sizeof(AirReflect) == sizeof(photonEnergy)); //assert the size of this array to be the same as the photon energy array
  
  //
  // Optical surfaces and boundaries
  //

  /*
  GROUND:The incidence of a photon upon a rough surface requires choosing the angle, a, 
  between a ‘micro-facet’ normal and that of the average surface. 
  
  The UNIFIED model assumes that the probability of micro-facet normals populates 
  the annulus of solid angle sin(a)dawill be  proportional to a gaussian of 
  SigmaAlpha:theOpSurface->SetSigmaAlpha(0.1);where sigma_alpha is in [rad]
  
  In the GLISUR model this is indicated by the value of polish; when it is <1, 
  then a random point is generated in a sphere of radius (1-polish), and 
  the corresponding vector is added to the normal. The value 0 means maximum 
  roughness with effective plane of reflection distributedas cos(a).theOpSurface -> SetPolish (0.0)
  */

  // For Air
  // Material Properties:
  G4MaterialPropertiesTable *Air = new G4MaterialPropertiesTable();
  Air->AddProperty("RINDEX", photonEnergy, RefractiveIndex_Air, nEntries);         //set the refractive index based on the above array
  Air->AddProperty("REFLECTIVITY", photonEnergy, AirReflect, nEntries);            //set the reflectivity based on the above array
  Air->AddProperty("ABSLENGTH", photonEnergy, Absorption_Air, nEntries);           //set the attenuation length based on the above array
  default_mat->SetMaterialPropertiesTable(Air);                                    //add these properties to the Air material
  // Optical Surface and Boundary Properties
  G4OpticalSurface *OpticalAir = new G4OpticalSurface("AirSurface");               //declare our air surface
  OpticalAir->SetModel(unified);                                                   //using the unified model
  OpticalAir->SetType(dielectric_dielectric);                                      //air surface interactions are dielectric-dielectric
  OpticalAir->SetFinish(polished);                                                 //saying air is a polished surface
 // note:
 // polishedfrontpainted: only reflection, absorption and no refraction
 // polished: follows Snell's law
 // ground: follows Snell's law

  //
  // Define a simple mirror-like property for the wrapping material
  //
  G4double fBoxPolish = 1;
  G4OpticalSurface* reflectiveSurface = new G4OpticalSurface("ReflectiveSurface",  //declare its name
							   glisur,                                                           //using the glisur model
							   ground,                                                           //using a ground surface as opposed to a polished one
							   dielectric_metal,
							   fBoxPolish);
  
  G4MaterialPropertiesTable* reflectiveSurfaceProperty =  new G4MaterialPropertiesTable();
  
  //Since these properties do not depend on photon energies, simply take a two-element array
  G4double p_box[] = {1.79*eV, 3.26*eV};                              //energy range of the optical photons the box interacts with
  const G4int nbins = sizeof(p_box)/sizeof(G4double);  
  G4double fBoxReflectivity = 1.0;                                    //set reflectivity to 1.0
  G4double refl_box[] = {fBoxReflectivity, fBoxReflectivity};         //set array for reflectivity interactions
  assert(sizeof(refl_box) == sizeof(p_box));
  G4double effi_box[] = {0, 0};                                       //set the array for the reflective efficiency (100% efficiency -> not 100% realistic)
  assert(sizeof(effi_box) == sizeof(effi_box));

  reflectiveSurfaceProperty->AddProperty("REFLECTIVITY",p_box, refl_box, nbins); //set the reflectivity based on the above array
  reflectiveSurfaceProperty->AddProperty("EFFICIENCY",p_box, effi_box, nbins);   //set the reflection efficiency based on the above array
  
  reflectiveSurface -> SetMaterialPropertiesTable(reflectiveSurfaceProperty);    //set the surface properties for the Box.

  // Use G4LogicalSkinSurface for one-directional photon propagation
  new G4LogicalSkinSurface("WrappingSurface", BoxLV, reflectiveSurface);         //assign the surface properties to the box logical volume
  
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl; 

  return WorldPV; //always return the physical World
}

// Use sensitive detector scheme for SiPM.
void RBDetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // sensitive detectors
  auto sdManager = G4SDManager::GetSDMpointer(); //access the Sensitive Detector Manager
  G4String SDname;
  auto sipmSD = new RBSiPMSD(SDname="/sipmSD");  //declare our SiPM Sensitive detector
  sdManager->AddNewDetector(sipmSD);             //Give detector attributes to the SiPM
  sipmLV->SetSensitiveDetector(sipmSD);          //link the SiPM logical volume to the Sensitive detector object
  
}