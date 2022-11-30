// Пользовательские заголовочные файлы
#include "linacG4DetectorConstruction.hh"

/////////////////////////////////////////

#include "G4RunManager.hh"       // Обязательный
#include "G4NistManager.hh"      // Материалы
#include "G4Box.hh"              // Куб
#include "G4Cons.hh"             // Конус
#include "G4Sphere.hh"           // Сфера
#include "G4Orb.hh"              // Полная сфера
#include "G4Tubs.hh"             // Цилиндр
#include "G4Polycone.hh"         // Фигура вращения (r_r0_z)
#include "G4SubtractionSolid.hh" // Вычет объемов
#include "G4UnionSolid.hh"       // Объединение объемов
#include "G4GenericPolycone.hh"  // Фигура вращения (r_z)
#include "G4LogicalVolume.hh"    // Логический объем
#include "G4VPhysicalVolume.hh"  // Физический объем
#include "G4RotationMatrix.hh"   // Изменение положения
#include "G4Transform3D.hh"      // Изменение положения
#include "G4VisAttributes.hh"    // Визуализация (дополнтиельные функции)
#include "G4VSolid.hh"           // Общий объем
#include "G4PVPlacement.hh"      // Объем с помещением
#include "G4SystemOfUnits.hh"    // Сантиметры и проч.

//////////////////////////////////////////

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Видимость, цвета
    G4Colour brown(0.7, 0.4, 0.1);
    G4Colour yellow(1., 1., 0.);
    G4Colour red(1., 0., 0.);
    G4Colour silver(192., 192., 192.);
    G4Colour green(0., 1., 0);
    G4Colour blue(0., 0., 1.);
    G4VisAttributes *invsible_world = new G4VisAttributes(0);
    G4VisAttributes *Pk_silver = new G4VisAttributes(silver);
    G4VisAttributes *Pc_green = new G4VisAttributes(green);
    G4VisAttributes *brown_uk = new G4VisAttributes(brown);
    G4VisAttributes *yellow_uk = new G4VisAttributes(yellow);
    G4VisAttributes *blue_uk = new G4VisAttributes(blue);

    // Подплючение материалов
    G4NistManager *nist = G4NistManager::Instance();

    // Материалы
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *primarycollimator_mt = nist->FindOrBuildMaterial("G4_W");
    G4Material *sphere_mt = nist->FindOrBuildMaterial("G4_W");
    G4Material *VWCyl = nist->FindOrBuildMaterial("G4_Be");
    G4Material *FF_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL ");
    G4Material *mlc_mat_1 = nist->FindOrBuildMaterial("G4_W");

    // Включение/выключения проверки перекрытия объемов
    constexpr auto checkOverlaps = true;

    ////////////////////////////////////////////////////////////////////////
    // Параметры World /////////////////////////////////////////////////////
    constexpr auto world_sizeXY = 90 * cm;
    constexpr auto world_sizeZ = 150 * cm;

    // Геометрический объем
    G4Box *solidWorld =
        new G4Box("World", // Название
                  world_sizeXY,
                  world_sizeXY,
                  world_sizeZ);
    // Логический объем
    G4LogicalVolume *logicWorld =
        new G4LogicalVolume(solidWorld,
                            world_mat,
                            "World");

    logicWorld->SetVisAttributes(invsible_world);
    // Физический объем
    G4VPhysicalVolume *physWorld =
        new G4PVPlacement(0,
                          G4ThreeVector(), // Расположение
                          logicWorld,      // Логический объем
                          "World",         // Название
                          0,               // Материнский объем
                          false,           // без булевых операций
                          0,               // Номер копии
                          checkOverlaps);  // Проверка
                                           ////////
    ////////////////////////////////////////////////////////////////////////
    // Общие параметры
    constexpr auto Phi_0 = 0. * deg;
    constexpr auto Phi_max = 180 * deg;

    // Параметры Primary Collimator ///////////////////////////////////////
    // Часть  1
    // Геометрия цилиндра
    constexpr auto ch1_cyl_pRMax = 5. * cm;  // !!!!
    constexpr auto ch1_cyl_pRMin = 0.5 * cm; // !!!!
    constexpr auto ch1_cyl_pDz = 10. * cm;   // !!!!

    constexpr auto ch1_cyl2_pRMax = 5.2 * cm; // !!!!
    constexpr auto ch1_cyl2_pRMin = 4.4 * cm; // !!!!
    constexpr auto ch1_cyl2_pDz = 4. * cm;    // !!!!

    G4Tubs *ch1_cyl_geom = new G4Tubs("ch_1_cyl",
                                      ch1_cyl_pRMin,
                                      ch1_cyl_pRMax,
                                      ch1_cyl_pDz,
                                      Phi_0,
                                      Phi_max);

    G4Tubs *ch1_cyl2_geom = new G4Tubs("ch_1_cyl2",
                                       ch1_cyl2_pRMin,
                                       ch1_cyl2_pRMax,
                                       ch1_cyl2_pDz,
                                       Phi_0,
                                       Phi_max);
    // Сфера, которую вставляем
    constexpr auto ch1_sphere_pRmax = 2. * cm; // !!!

    G4Orb *ch1_sphere_geom = new G4Orb("ch_1_sph",
                                       ch1_sphere_pRmax);

    // Конус, который вставляем в сh_1
    constexpr auto ch1_con_pRmin1 = 0. * cm;
    constexpr auto ch1_con_pRmax1 = 0. * cm;
    constexpr auto ch1_con_pRmin2 = 0. * cm;
    constexpr auto ch1_con_pRmax2 = 4.18 * cm;
    constexpr auto ch1_con_pDz = 8. * cm;

    G4Cons *ch1_cons_geom = new G4Cons("ch_1 cons",
                                       ch1_con_pRmin1,
                                       ch1_con_pRmax1,
                                       ch1_con_pRmin2,
                                       ch1_con_pRmax2,
                                       ch1_con_pDz,
                                       Phi_0,
                                       Phi_max);

    // Вспомогательная конструкция
    G4ThreeVector zTrans(0, 0, -2.5 * cm);
    G4ThreeVector z2Trans(0, 0, 3.0 * cm);
    G4ThreeVector z3Trans(0, 0, 8.0 * cm);
    G4RotationMatrix *yRot = new G4RotationMatrix;
    G4RotationMatrix *yRot2 = new G4RotationMatrix;
    G4RotationMatrix *yRot3 = new G4RotationMatrix;
    G4RotationMatrix invRot = yRot->invert();
    G4RotationMatrix invRot2 = yRot2->invert();
    G4RotationMatrix invRot3 = yRot3->invert();
    G4Transform3D transform(invRot, zTrans);    // !!!
    G4Transform3D transform2(invRot2, z2Trans); // !!!
    G4Transform3D transform3(invRot3, z3Trans); // !!!

    G4SubtractionSolid *substration_sphere_in_cyl =
        new G4SubtractionSolid("Ch1 Cyl-Sphere",
                               ch1_cyl_geom,
                               ch1_sphere_geom,
                               transform); // !!!

    G4SubtractionSolid *substration_con_in_cyl =
        new G4SubtractionSolid("Ch1 Cyl-cons",
                               substration_sphere_in_cyl,
                               ch1_cons_geom,
                               transform2); // !!!

    G4SubtractionSolid *primarycollimator =
        new G4SubtractionSolid("primarycollimator",
                               substration_con_in_cyl,
                               ch1_cyl2_geom,
                               transform3); // !!!

    // Логический объем
    G4LogicalVolume *logicPrimarycollimator =
        new G4LogicalVolume(primarycollimator,
                            primarycollimator_mt,
                            "Primary collimator");
    logicPrimarycollimator->SetVisAttributes(Pk_silver);

    // Физический объем
    new G4PVPlacement(0,                      // без поворота
                      G4ThreeVector(0, 0, 0), // Расположение
                      logicPrimarycollimator, // Логический объем
                      "Primary collimator",   // Название
                      logicWorld,             // Помещаем в материнский объем
                      false,                  // без булевых операций
                      0,                      // Номер копии
                      checkOverlaps);         // Проверка
    // Sphere

    G4double spherePM_pRmax = 0.5 * cm; // !!!

    G4Orb *PMsphere_geom = new G4Orb("Sph_PM",
                                     spherePM_pRmax);

    G4LogicalVolume *LogicPmSphere =
        new G4LogicalVolume(PMsphere_geom,
                            sphere_mt,
                            "Sphere in primary collimator");

    LogicPmSphere->SetVisAttributes(Pc_green);

    new G4PVPlacement(0,                              // без поворота
                      G4ThreeVector(0, 0, 7 * cm),    // Расположение
                      LogicPmSphere,                  // Логический объем
                      "Sphere in primary collimator", // Название
                      logicWorld,                     // Помещаем в материнский объем
                      false,                          // без булевых операций
                      0,                              // Номер копии
                      checkOverlaps);                 // Проверка

    // Параметры Primary Collimator ///////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////
    // Параметры Vacuum Window//////////////////////////////////////////////
    G4double RVW_max = 4 * cm; // !!!!!
    G4double RVW_min = 0 * cm; // !!!!!
    G4double zVW = 0.1 * cm;   // !!!!!

    G4Tubs *TubVm_ch1 = new G4Tubs("Cul_VW",
                                   RVW_min,
                                   RVW_max,
                                   zVW,
                                   Phi_0,
                                   Phi_max);

    G4LogicalVolume *LogicCylVM_ch1 =
        new G4LogicalVolume(TubVm_ch1,
                            VWCyl,
                            "Tube is vacuum window");

     LogicCylVM_ch1->SetVisAttributes(brown_uk);

    new G4PVPlacement(0,                            // без поворота
                      G4ThreeVector(0, 0, 12 * cm), // Расположение
                      LogicCylVM_ch1,               // Логический объем
                      "Vacuum window cyl",          // Название
                      logicWorld,                   // Помещаем в материнский объем
                      false,                        // без булевых операций
                      0,                            // Номер копии
                      checkOverlaps);               // Проверка

    G4double RVW2_max = 5. * cm;     // !!!!!
    G4double RVW2_r_max = 4.99 * cm; // !!!!!
    G4double RVW2_r2_max = 4.5 * cm; // !!!!!

    G4double RVW2_min = 0. * cm; // !!!!!
    G4double zVW2 = 2 * cm;      // !!!!!
    G4double zVW2_r = 1.99 * cm; // !!!!!
    G4double zVW2_r2 = 0.3 * cm; // !!!!!

    G4Tubs *TubVm_ch2 = new G4Tubs("Cul_ob_VW",
                                   RVW2_min,
                                   RVW2_max,
                                   zVW2,
                                   Phi_0,
                                   Phi_max);

    G4Tubs *TubVm_ch2_r = new G4Tubs("Cul_VW_r",
                                     RVW2_min,
                                     RVW2_r_max,
                                     zVW2_r,
                                     Phi_0,
                                     Phi_max);

    G4Tubs *TubVm_ch2_r2 = new G4Tubs("Cul_VW_r2",
                                      RVW2_min,
                                      RVW2_r2_max,
                                      zVW2_r2,
                                      Phi_0,
                                      Phi_max);

    G4SubtractionSolid *substration_Tub_Tubr =
        new G4SubtractionSolid("Substration_Tub_Tub",
                               TubVm_ch2,
                               TubVm_ch2_r);

    // Вспомогательная конструкция//////////////
    G4ThreeVector zTrans_VM(0, 0, -2 * cm);
    G4RotationMatrix *yRot_Vm = new G4RotationMatrix;
    G4RotationMatrix invRot_Vm = yRot_Vm->invert();
    G4Transform3D transformVM(invRot_Vm, zTrans_VM); // !!!
    ///////////////////////////////////////////

    G4SubtractionSolid *substration_Tub_Tubr2 =
        new G4SubtractionSolid("Substration_Tub_Tub2",
                               substration_Tub_Tubr,
                               TubVm_ch2_r2,
                               transformVM);


    G4LogicalVolume *LogicCylVM_ch2 =
        new G4LogicalVolume(substration_Tub_Tubr2,
                            VWCyl,
                            "Tube is vacuum window");

    LogicCylVM_ch2->SetVisAttributes(yellow_uk);

    new G4PVPlacement(0,                            // без поворота
                      G4ThreeVector(0, 0, 11 * cm), // Расположение
                      LogicCylVM_ch2,               // Логический объем
                      "Vacuum window ch2",          // Название
                      logicWorld,                   // Помещаем в материнский объем
                      false,                        // без булевых операций
                      0,                            // Номер копии
                      checkOverlaps);               // Проверка

    // Vacuum window ////////////////////////////////////////
    /////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    // Flattering filter ///////////////////////////////////

    // Part 1
    // constexpr auto nmbRZ = 8;
    // constexpr auto nmbRZ_e = 5;

    // G4double FF_r1[] =
    //     {1.3650 * cm, 1.3630 * cm, 1.3600 * cm, 1.3550 * cm,
    //      1.3490 * cm, 1.3400 * cm, 1.3320 * cm, 1.3200 * cm};
    // G4double FF_z1[] =
    //     {0. * cm, 0.012 * cm, 0.033 * cm, 0.040 * cm,
    //      0.050 * cm, 0.068 * cm, 0.075 * cm, 0.068 * cm};
    // G4GenericPolycone *FF_ch1 =
    //     new G4GenericPolycone("Ch_1 FF",
    //                           Phi_0,
    //                           Phi_max,
    //                           nmbRZ,
    //                           FF_r1,
    //                           FF_z1);

    // G4double FF_r2[] =
    //     {1.3100 * cm, 1.2720 * cm, 1.2460 * cm, 1.1940 * cm,
    //      1.1550 * cm, 1.1220 * cm, 1.0940 * cm, 1.0400 * cm};
    // G4double FF_z2[] =
    //     {0.075 * cm, 0.090 * cm, 0.100 * cm, 0.125 * cm,
    //      0.150 * cm, 0.175 * cm, 0.200 * cm, 0.250 * cm};
    // G4GenericPolycone *FF_ch2 =
    //     new G4GenericPolycone("Ch_2 FF",
    //                           Phi_0,
    //                           Phi_max,
    //                           nmbRZ,
    //                           FF_r2,
    //                           FF_z2);

    // G4double FF_r3[] =
    //     {0.9940 * cm, 0.9570 * cm, 0.9500 * cm, 0.9460 * cm,
    //      0.9420 * cm, 0.9380 * cm, 0.9270 * cm, 0.9160 * cm};
    // G4double FF_z3[] =
    //     {0.300 * cm, 0.350 * cm, 0.360 * cm, 0.370 * cm,
    //      0.380 * cm, 0.390 * cm, 0.400 * cm, 0.410 * cm};
    // G4GenericPolycone *FF_ch3 =
    //     new G4GenericPolycone("Ch_3 FF",
    //                           Phi_0,
    //                           Phi_max,
    //                           nmbRZ,
    //                           FF_r3,
    //                           FF_z3);

    // G4double FF_r4[] =
    //     {0.9050 * cm, 0.8860 * cm, 0.8490 * cm, 0.8090 * cm,
    //      0.7690 * cm, 0.6910 * cm, 0.6290 * cm, 0.5740 * cm};
    // G4double FF_z4[] =
    //     {0.420 * cm, 0.430 * cm, 0.450 * cm, 0.475 * cm,
    //      0.500 * cm, 0.550 * cm, 0.600 * cm, 0.650 * cm};
    // G4GenericPolycone *FF_ch4 =
    //     new G4GenericPolycone("Ch_4 FF",
    //                           Phi_0,
    //                           Phi_max,
    //                           nmbRZ,
    //                           FF_r4,
    //                           FF_z4);

    // G4double FF_r5[] =
    //     {0.5240 * cm, 0.4770 * cm, 0.4250 * cm, 0.3700 * cm,
    //      0.3130 * cm, 0.2540 * cm, 0.1950 * cm, 0.1360 * cm,
    //      0.0930 * cm};
    // G4double FF_z5[] =
    //     {0.700 * cm, 0.750 * cm, 0.800 * cm, 0.850 * cm,
    //      0.900 * cm, 0.950 * cm, 1.000 * cm, 1.050 * cm,
    //      1.100 * cm};
    // G4GenericPolycone *FF_ch5 =
    //     new G4GenericPolycone("Ch_5 FF",
    //                           Phi_0,
    //                           Phi_max,
    //                           9,
    //                           FF_r5,
    //                           FF_z5);

    // // G4double FF_r6_1[] = {0.6290 * cm, 0.5740 * cm, 0.5240 * cm};
    // // G4double FF_r6_2[] = {0.4770 * cm, 0.4250 * cm, 0.3700 * cm};
    // // G4double FF_z6_1[] = {0.600 * cm, 0.650 * cm, 0.700 * cm};
    // // G4double FF_z6_2[] = {0.750 * cm, 0.800 * cm, 0.850 * cm};

    // // G4GenericPolycone *FF_ch6 =
    // //     new G4GenericPolycone("Ch_6 FF",
    // //                           Phi_0,
    // //                           Phi_max,
    // //                           6,
    // //                           FF_r6,
    // //                           FF_z6);

    // // G4GenericPolycone *FF_ch6_1 =
    // //     new G4GenericPolycone("Ch_6_1 FF",
    // //                           Phi_0,
    // //                           Phi_max,
    // //                           3,
    // //                           FF_r6_1,
    // //                           FF_z6_1);

    // // G4GenericPolycone *FF_ch6_2 =
    // //     new G4GenericPolycone("Ch_6_2 FF",
    // //                           Phi_0,
    // //                           Phi_max,
    // //                           3,
    // //                           FF_r6_2,
    // //                           FF_z6_2);

    // // G4UnionSolid *FF_ch6 =
    // //     new G4UnionSolid("unionFF_Ch1", FF_ch6_1, FF_ch6_2);

    // // G4double FF_r7[] =
    // //     {0.3130 * cm, 0.2540 * cm, 0.1950 * cm, 0.1360 * cm, 0.0930 * cm};
    // // G4double FF_z7[] =
    // //     {0.900 * cm, 0.950 * cm, 1.000 * cm, 1.050 * cm, 1.100 * cm};
    // // G4GenericPolycone *FF_ch7 =
    // //     new G4GenericPolycone("Ch_7 FF",
    // //                           Phi_0,
    // //                           Phi_max,
    // //                           nmbRZ_e,
    // //                           FF_r7,
    // //                           FF_z7);

    // G4UnionSolid *unionCh1_Ch2 =
    //     new G4UnionSolid("Ch1+Ch2", FF_ch1, FF_ch2);
    // G4UnionSolid *unionCh2_Ch3 =
    //     new G4UnionSolid("Ch2+Ch3", unionCh1_Ch2, FF_ch3);
    // G4UnionSolid *unionCh3_Ch4 =
    //     new G4UnionSolid("Ch3+Ch4", unionCh2_Ch3, FF_ch4);
    // G4UnionSolid *unionCh4_Ch5 =
    //     new G4UnionSolid("Ch4+Ch5", unionCh3_Ch4, FF_ch5);
    // // G4UnionSolid *unionCh5_Ch6 =
    // //     new G4UnionSolid("Ch5+Ch6", unionCh4_Ch5, FF_ch6);
    // // G4UnionSolid *unionCh6_Ch7 =
    // //     new G4UnionSolid("Ch6+Ch7", unionCh5_Ch6, FF_ch7);

    // G4LogicalVolume *LogicFF =
    //     new G4LogicalVolume(unionCh4_Ch5,
    //                         FF_mat,
    //                         "Flattering Filter up");

    // new G4PVPlacement(0,                            // без поворота
    //                   G4ThreeVector(0, 0, 15 * cm), // Расположение
    //                   LogicFF,                      // Логический объем
    //                   "Flattering Filter up",       // Название
    //                   logicWorld,                   // Помещаем в материнский объем
    //                   false,                        // без булевых операций
    //                   0,                            // Номер копии
    //                   checkOverlaps);               // Проверка

    /////////////////////////////////////////////////////////////
    // Multi-leaf collimator

    // Box 1
    constexpr auto MLCBox_sizeXY = 3.88 * cm;
    constexpr auto MLCBox_sizeZ = 3.02 * cm;

    constexpr auto MLCBox_sizeXY_r = 2. * cm;
    constexpr auto MLCBox_sizeZ_r = 1.65 * cm;

    G4Box *MlcRightBox =
        new G4Box("MlcRightBox",
                  MLCBox_sizeXY,
                  MLCBox_sizeXY,
                  MLCBox_sizeZ);

    G4Box *MlcRightBox_r =
        new G4Box("MlcRightBox_r",
                  MLCBox_sizeXY_r,
                  MLCBox_sizeXY_r,
                  MLCBox_sizeZ_r);

    G4RotationMatrix *yTrans_u = new G4RotationMatrix;
    G4ThreeVector yTrans(0, -3.0 * cm, 0);
    G4RotationMatrix invRot_box = yTrans_u->invert();
    G4Transform3D transform_mlc(invRot_box, yTrans);

    G4SubtractionSolid *MlcRightBox_subs_Box =
        new G4SubtractionSolid("MlcBox - Box", MlcRightBox,
                               MlcRightBox_r, transform_mlc);

    G4LogicalVolume *LogicMlcRightBox =
        new G4LogicalVolume(MlcRightBox_subs_Box,
                            mlc_mat_1,
                            "MlcRightBox");

    LogicMlcRightBox->SetVisAttributes(blue_uk);

    new G4PVPlacement(0,                                    // без поворота
                      G4ThreeVector(0, 6.50 * cm, 50 * cm), // Расположение
                      LogicMlcRightBox,                     // Логический объем
                      "MlcRightBox",                        // Название
                      logicWorld,                           // Помещаем в материнский объем
                      false,                                // без булевых операций
                      0,                                    // Номер копии
                      checkOverlaps);                       // Проверка

    G4Box *MlcLeftBox =
        new G4Box("MlcLeftBox",
                  MLCBox_sizeXY,
                  MLCBox_sizeXY,
                  MLCBox_sizeZ);

    G4RotationMatrix *yTrans_uu = new G4RotationMatrix;
    G4ThreeVector yTrans1(0, 3.0 * cm, 0);
    G4RotationMatrix invRot_box1 = yTrans_uu->invert();
    G4Transform3D transform_mlc1(invRot_box1, yTrans1);

    G4SubtractionSolid *MlcLeftBox_subs_Box1 =
        new G4SubtractionSolid("MlcBox - Box", MlcLeftBox,
                               MlcRightBox_r, transform_mlc1);



    G4LogicalVolume *LogicMlcLeftBox =
        new G4LogicalVolume(MlcLeftBox_subs_Box1,
                            mlc_mat_1,
                            "MlcLeftBox");

    LogicMlcLeftBox->SetVisAttributes(blue_uk);

    new G4PVPlacement(0,                                     // без поворота
                      G4ThreeVector(0, -6.50 * cm, 50 * cm), // Расположение
                      LogicMlcLeftBox,                       // Логический объем
                      "MlcLeftBox",                          // Название
                      logicWorld,                            // Помещаем в материнский объем
                      false,                                 // без булевых операций
                      0,                                     // Номер копии
                      checkOverlaps);                        // Проверка
    // Part 2
    // constexpr auto mlc_weight_1 =
    // constexpr auto mlc_weight_1 =

    // G4Box* Mlcplans1 =
    //     new G4Box()

    return physWorld;
}
