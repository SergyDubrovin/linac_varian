#include "linacG4DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4double env_sizeXY = 100 * cm, env_sizeZ = 100 * cm;
    G4Material *env_mat = nist->FindOrBuildMaterial("G4_WATER");

    G4bool checkOverlaps = true;

    G4double world_sizeXY = env_sizeXY;
    G4double world_sizeZ = env_sizeZ;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box *solidWorld =
        new G4Box("World",
                  0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);

    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(solidWorld,
                            world_mat,
                            "World");

    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,
                          G4ThreeVector(),
                          logicWorld,
                          "World",
                          0,
                          false,
                          0,
                          checkOverlaps);

    G4Box *solidEnv =
        new G4Box("Envelope",
                  0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);

    G4LogicalVolume *logicEnv =
        new G4LogicalVolume(solidEnv,
                            env_mat,
                            "Envelope");

    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logicEnv,
                      "Envelope",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

    //
    // Flattering filter
    //
    G4Material *FF_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
    G4ThreeVector posFF = G4ThreeVector(0, 0, 0);
    G4double FF_phimin = 0.0 * deg, FF_phimax = 360.0 * deg;
    G4int numZP = 3;
    
    G4double zInner[] = {0.344*cm, 0.331*cm, 0.025*cm};

    G4double rOuter[] = {0.0*cm, 0.047*cm, 0.093*cm}; 

    G4Polycone *solidFF = new G4Polycone(
        "flattering filter",
        FF_phimin,
        FF_phimax,
        numZP,
        rOuter,
        zInner);

    G4LogicalVolume* logicFF =
    new G4LogicalVolume(solidFF,         //its solid
                        FF_mat,          //its material
                        "flattering filter");           //its name

    new G4PVPlacement(0,                       //no rotation
                    posFF,                    //at position
                    logicFF,             //its logical volume
                    "flattering filter",                //its name
                    logicFF,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

    return physWorld;
}