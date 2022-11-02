#include "linacG4DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4GenericPolycone.hh"
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
    // Get nist material manager
    G4NistManager *nist = G4NistManager::Instance();

    // Envelope parameters
    //
    G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
    G4Material *env_mat = nist->FindOrBuildMaterial("G4_WATER");

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //
    // World
    //
    G4double world_sizeXY = 1.2 * env_sizeXY;
    G4double world_sizeZ = 1.2 * env_sizeZ;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box *solidWorld =
        new G4Box("World",                                                    // its name
                  0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ); // its size

    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(solidWorld, // its solid
                            world_mat,  // its material
                            "World");   // its name

    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,               // no rotation
                          G4ThreeVector(), // at (0,0,0)
                          logicWorld,      // its logical volume
                          "World",         // its name
                          0,               // its mother  volume
                          false,           // no boolean operation
                          0,               // copy number
                          checkOverlaps);  // overlaps checking

    //
    // Envelope
    //
    G4Box *solidEnv =
        new G4Box("Envelope",                                           // its name
                  0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ); // its size

    G4LogicalVolume *logicEnv =
        new G4LogicalVolume(solidEnv,    // its solid
                            env_mat,     // its material
                            "Envelope"); // its name

    new G4PVPlacement(0,               // no rotation
                      G4ThreeVector(), // at (0,0,0)
                      logicEnv,        // its logical volume
                      "Envelope",      // its name
                      logicWorld,      // its mother  volume
                      false,           // no boolean operation
                      0,               // copy number
                      checkOverlaps);  // overlaps checking

    //
    // Flattering filter
    //
    G4Material *FF_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); // !!!
    G4ThreeVector posFF = G4ThreeVector(0, 0, 0);

    // Polycon section shape
    // G4int nmbRZ = 41;
    G4int nmbRZ = 42;

    G4double r[] = {1.3650 * cm, 1.3630 * cm, 1.3600 * cm, 1.3550 * cm, 1.3490 * cm,
                    1.3400 * cm, 1.3320 * cm, 1.3200 * cm, 1.3100 * cm, 1.2720 * cm,
                    1.2460 * cm, 1.1940 * cm, 1.1550 * cm, 1.1220 * cm, 1.0940 * cm, 
                    1.0400 * cm, 0.9940 * cm, 0.9570 * cm, 0.9500 * cm, 0.9460 * cm,
                    0.9420 * cm, 0.9380 * cm, 0.9270 * cm, 0.9160 * cm, 0.9050 * cm,
                    0.8860 * cm, 0.8490 * cm, 0.8090 * cm, 0.7690 * cm, 0.6910 * cm,
                    0.6290 * cm, 0.5740 * cm, 0.5240 * cm, 0.4770 * cm, 0.4250 * cm,
                    0.3700 * cm, 0.3130 * cm, 0.2540 * cm, 0.1950 * cm, 0.1360 * cm,
                    0.0930 * cm};

    G4double z[] = {0. * cm, 0.012 * cm, 0.033 * cm, 0.040 * cm, 0.050 * cm,
                    0.068 * cm, 0.075 * cm, 0.068 * cm, 0.075 * cm, 0.090 * cm,
                    0.100 * cm, 0.125 * cm, 0.150 * cm, 0.175 * cm, 0.200 * cm,
                    0.250 * cm, 0.300 * cm, 0.350 * cm, 0.360 * cm, 0.370 * cm,
                    0.380 * cm, 0.390 * cm, 0.400 * cm, 0.410 * cm, 0.420 * cm,
                    0.430 * cm, 0.450 * cm, 0.475 * cm, 0.500 * cm, 0.550 * cm,
                    0.600 * cm, 0.650 * cm, 0.700 * cm, 0.750 * cm, 0.800 * cm,
                    0.850 * cm, 0.900 * cm, 0.950 * cm, 1.000 * cm, 1.050 * cm,
                    1.100 * cm};

    // G4Polycone *solidFF = new G4Polycone("FF", 0.0 * deg, 360.0 * deg, nmbRZ, r, z);
    G4VSolid *solidFF = new G4GenericPolycone("FF", 0.0 * deg, 360.0 * deg, nmbRZ, r, z);

    G4LogicalVolume *logicFF = new G4LogicalVolume(solidFF, FF_mat, "FF");

    new G4PVPlacement(0,
                      posFF,
                      logicFF,
                      "FF",
                      logicEnv,
                      false,
                      0,
                      checkOverlaps);

    return physWorld;
}