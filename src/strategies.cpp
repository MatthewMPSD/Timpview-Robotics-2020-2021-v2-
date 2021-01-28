#include "strategies.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>

void autonomousConfig()
{
  // Set Autonomous Specific Speeds
  EscalatorRamp.setVelocity(escalatorPowerAuto, velocityUnits::pct);
  IntakeLeft.setVelocity(intakePowerAuto, velocityUnits::pct);
  IntakeRight.setVelocity(intakePowerAuto, velocityUnits::pct);
  Drivetrain.setDriveVelocity(drivetrainPowerAuto, velocityUnits::pct);
  Drivetrain.setTurnVelocity(drivetrainTurnPowerAuto, velocityUnits::pct);
}

void cornerStrategy ()
{
  Controller1.Screen.print("CORNER");
  int angleModifier = modeSelector.mode() == 0 ? 1 : -1;
  Drivetrain.driveFor(reverse, 18, inches);
  Drivetrain.turnFor(right, 45*angleModifier, degrees);
  centerOn(*enemySignature);
  intake.inward();
  driveForTime(forward, 800, msec);
  escalator.up();
  centerOn(*enemySignature);
  driveForTime(forward, 300, msec);
  wait(1500, msec);
  shimmy(300, 500, msec, 3);
  wait(2, sec);
  escalator.off();
  intake.off();
  // intake.outward();
  driveForTime(reverse, 500, msec);
  


  // int angleModifier = modeSelector.mode() == 0 ? 1 : -1;
  // Drivetrain.driveFor(reverse, 36, inches);
  // Drivetrain.turnFor(left, 135*angleModifier, degrees);
  // centerOn(*allySignature);
  // Drivetrain.setDriveVelocity(drivetrainPowerAuto/3, percent);
  // collectSignature(*allySignature);
  // Drivetrain.setDriveVelocity(drivetrainPowerAuto, percent);
  // if (modeSelector.mode() == 2)
  // {
  //   Drivetrain.driveFor(forward, 6, inches);
  // }
  // centerOn(FrontVision__GOAL);
  // intake.outward();
  // Drivetrain.drive(forward);
  // wait(500, msec);
  // Drivetrain.stop();
  // escalator.up();
  // wait(3, sec);
  // Drivetrain.driveFor(reverse, 6, inches);
  // Drivetrain.driveFor(forward, 6, inches);
  // wait(1, sec);
  // Drivetrain.driveFor(reverse, 6, inches);
  // wait(2, sec);
  // intake.outward();
  // escalator.off();
}

void middleStrategy ()
{
  Controller1.Screen.print("MIDDLE");
  Drivetrain.turnFor(right, 35, degrees);
  driveForTime(forward, 750, msec);
  escalator.up();
  wait(1500, msec);
  escalator.off();
  intake.outward();
  driveForTime(reverse, 1750, msec);
  intake.inward();
  Drivetrain.turnFor(right, 105, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 500, msec);
  centerOn(*enemySignature);
  driveForTime(forward, 500, msec);
  escalator.up();
  driveForTime(forward, 300, msec);
  wait(1000, msec);
  shimmy(300, 500, msec, 2);
  wait(1500, msec);
  intake.outward();
  driveForTime(reverse, 250, msec);
  escalator.off();
  intake.off();
}

void driverControl ()
{

  //Intake system
  Controller1.ButtonL2.pressed([](){intake.inward();});
  Controller1.ButtonL2.released([](){intake.off();});
  Controller1.ButtonL1.pressed([](){intake.outward();});
  Controller1.ButtonL1.released([](){intake.off();});

  //Swap Teams
  Controller1.ButtonUp.pressed([](){
    if (hasPressedUp && timePressedUp.time() < 500)
    {
      hasPressedUp = false;
      signature* tempEnemy = enemySignature;
      enemySignature = allySignature;
      allySignature = tempEnemy;
    } else
    {
      hasPressedUp = true;
    }
    timePressedUp.reset();
  });

  //Turn off sorting
  Controller1.ButtonY.pressed([](){doSort = !doSort;});

  //Escalator system
  Controller1.ButtonA.pressed([](){escalator.toggle();});
}

void rerunProgram()
{
  int lSpeed = 0;
  int RSpeed = 0;
  int fl = 0;
  int bl = 0;
  int fr = 0;
  int br = 0;
  int SorterSpeed = 0;
  int EleSpeed = 0;
  bool doLoop = true;
  bool wasPressingForEscalator = false;


  // //Intake system
  // Controller1.ButtonL2.pressed([](){intake.inward();});
  // Controller1.ButtonL2.released([](){intake.off();});
  // Controller1.ButtonL1.pressed([](){intake.outward();});
  // Controller1.ButtonL1.released([](){intake.off();});

  // //Escalator system
  // Controller1.ButtonA.pressed([](){escalator.toggle();});

  Brain.Timer.reset();

  std::ofstream ofs;
  ofs.open("rerun.txt", std::ofstream::out);

  while(doLoop)
  {
    if (Controller1.ButtonDown.pressing())
    {
      doLoop = false;
    }

    if (Controller1.ButtonL2.pressing())
    {
      intake.inward();
    }
    if (Controller1.ButtonL1.pressing())
    {
      intake.outward();
    }
    if (!(Controller1.ButtonL2.pressing() || Controller1.ButtonL1.pressing()))
    {
      intake.off();
    }
    if (Controller1.ButtonA.pressing() && !wasPressingForEscalator)
    {
      wasPressingForEscalator = false;
      escalator.toggle();
    } else
    {
      wasPressingForEscalator = true;
    }

    wait(.1, sec);

    lSpeed = IntakeLeft.velocity(percent);
    RSpeed = IntakeRight.velocity(percent);
    fl =  motor(PORT1, ratio18_1, false).velocity(percent);
    bl = motor(PORT2, ratio18_1, false).velocity(percent);
    fr = motor(PORT3, ratio18_1, true).velocity(percent);
    br = motor(PORT4, ratio18_1, true).velocity(percent);
    SorterSpeed = Sorter.velocity(percent);
    EleSpeed = EscalatorRamp.velocity(percent);

    ofs << "IntakeLeft.setVelocity(" << lSpeed << ", percent); \n";
    ofs << "IntakeRight.setVelocity(" << RSpeed << ", percent); \n";
    ofs << "motor(PORT1, ratio18_1, false).setVelocity(" << fl << ", percent); \n";
    ofs << "motor(PORT2, ratio18_1, false).setVelocity(" << bl << ", percent); \n";
    ofs << "motor(PORT3, ratio18_1, true).setVelocity(" << bl << ", percent); \n";
    ofs << "motor(PORT4, ratio18_1, true).setVelocity("<< br << ", percent); \n";
    ofs << "Sorter.setVelocity(" << SorterSpeed << ", percent); \n";
    ofs << "EscalatorRamp.setVelocity(" << EleSpeed << ", percent); \n";
    ofs << "wait(" << Brain.Timer.value() << ",sec) \n;";
    
    Brain.Timer.reset();
  }
  ofs.close();
}

void skillsAutonomous ()
{
  autonomousConfig();

  Drivetrain.setDriveVelocity(drivetrainPowerAutoSkills, percent);

  depositBalls(1);

  Drivetrain.driveFor(reverse, 12, inches);
  Drivetrain.turnFor(left, 135, degrees);
  Drivetrain.driveFor(forward, 48, inches);
  Drivetrain.turnFor(right, 90, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 750, msec);
  wait(500, msec);
  descoreCompletely();
  Drivetrain.driveFor(forward, 18, inches);
  Drivetrain.turnFor(right, 130, degrees);
  driveForTime(forward, 1, sec);
  centerOn(*enemySignature);
  driveForTime(forward, 1, sec);
  centerOn(*enemySignature);
  driveForTime(forward, 750, msec);
  centerOn(*enemySignature);
  driveForTime(forward, 750, msec);
  centerOn(*enemySignature);
  descoreCompletely();
  Drivetrain.driveFor(forward, 6, inches);
  Drivetrain.turnFor(right, 45, degrees);
  Drivetrain.driveFor(forward, 40, inches);
  Drivetrain.turnFor(right, 90, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 1, sec);
  descoreCompletely();
  Drivetrain.driveFor(forward, 6, inches);
  Drivetrain.turnFor(left, 90, degrees);
  Drivetrain.driveFor(forward, 36, inches);
  Drivetrain.turnFor(right, 45, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 1, sec);
  descoreCompletely();
}

void doRerun ()
{
IntakeLeft.spin(forward);
IntakeRight.spin(forward);
Sorter.spin(forward);
EscalatorRamp.spin(forward);
Drivetrain.drive(forward);


IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.12,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.109,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-4, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-31, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-52, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-52, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-31, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-4, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-59, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-45, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-45, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-56, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-83, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-78, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-78, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-84, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-95, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-94, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-94, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-96, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-62, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-45, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-45, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-86, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-82, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-47, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-47, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-93, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(-49, percent); 
IntakeRight.setVelocity(50, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-68, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-69, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-69, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-79, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(-110, percent); 
IntakeRight.setVelocity(115, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-45, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-55, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-55, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-49, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(-98, percent); 
IntakeRight.setVelocity(92, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-41, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-37, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-37, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-43, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(-100, percent); 
IntakeRight.setVelocity(100, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-36, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-35, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-35, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-43, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(-69, percent); 
IntakeRight.setVelocity(81, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-22, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-26, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-26, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-24, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(-45, percent); 
IntakeRight.setVelocity(63, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-17, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-17, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-17, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-13, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(52, percent); 
IntakeRight.setVelocity(-55, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-9, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-6, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-6, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-11, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(101, percent); 
IntakeRight.setVelocity(-103, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-3, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-4, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-4, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-2, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(88, percent); 
IntakeRight.setVelocity(-96, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-2, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-2, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-2, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-2, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(110, percent); 
IntakeRight.setVelocity(-102, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(66, percent); 
IntakeRight.setVelocity(-81, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(21, percent); 
IntakeRight.setVelocity(-61, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(1, percent); 
IntakeRight.setVelocity(-26, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-13, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-4, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-3, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(-55, percent); 
IntakeRight.setVelocity(60, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(-40, percent); 
IntakeRight.setVelocity(42, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(-15, percent); 
IntakeRight.setVelocity(28, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(-4, percent); 
IntakeRight.setVelocity(13, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(31, percent); 
motor(PORT2, ratio18_1, false).setVelocity(35, percent); 
motor(PORT3, ratio18_1, true).setVelocity(35, percent); 
motor(PORT4, ratio18_1, true).setVelocity(19, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(22, percent); 
motor(PORT1, ratio18_1, false).setVelocity(32, percent); 
motor(PORT2, ratio18_1, false).setVelocity(106, percent); 
motor(PORT3, ratio18_1, true).setVelocity(106, percent); 
motor(PORT4, ratio18_1, true).setVelocity(42, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(61, percent); 
motor(PORT2, ratio18_1, false).setVelocity(96, percent); 
motor(PORT3, ratio18_1, true).setVelocity(96, percent); 
motor(PORT4, ratio18_1, true).setVelocity(70, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(77, percent); 
motor(PORT2, ratio18_1, false).setVelocity(97, percent); 
motor(PORT3, ratio18_1, true).setVelocity(97, percent); 
motor(PORT4, ratio18_1, true).setVelocity(79, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(95, percent); 
motor(PORT2, ratio18_1, false).setVelocity(100, percent); 
motor(PORT3, ratio18_1, true).setVelocity(100, percent); 
motor(PORT4, ratio18_1, true).setVelocity(90, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(71, percent); 
motor(PORT2, ratio18_1, false).setVelocity(61, percent); 
motor(PORT3, ratio18_1, true).setVelocity(61, percent); 
motor(PORT4, ratio18_1, true).setVelocity(77, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(65, percent); 
motor(PORT2, ratio18_1, false).setVelocity(68, percent); 
motor(PORT3, ratio18_1, true).setVelocity(68, percent); 
motor(PORT4, ratio18_1, true).setVelocity(47, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(54, percent); 
motor(PORT2, ratio18_1, false).setVelocity(58, percent); 
motor(PORT3, ratio18_1, true).setVelocity(58, percent); 
motor(PORT4, ratio18_1, true).setVelocity(61, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(46, percent); 
motor(PORT2, ratio18_1, false).setVelocity(53, percent); 
motor(PORT3, ratio18_1, true).setVelocity(53, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-16, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(80, percent); 
motor(PORT2, ratio18_1, false).setVelocity(49, percent); 
motor(PORT3, ratio18_1, true).setVelocity(49, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-61, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(52, percent); 
motor(PORT2, ratio18_1, false).setVelocity(82, percent); 
motor(PORT3, ratio18_1, true).setVelocity(82, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-76, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(24, percent); 
motor(PORT2, ratio18_1, false).setVelocity(3, percent); 
motor(PORT3, ratio18_1, true).setVelocity(3, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-37, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(28, percent); 
motor(PORT2, ratio18_1, false).setVelocity(37, percent); 
motor(PORT3, ratio18_1, true).setVelocity(37, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-42, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(5, percent); 
motor(PORT2, ratio18_1, false).setVelocity(8, percent); 
motor(PORT3, ratio18_1, true).setVelocity(8, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-9, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(2, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(12, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-2, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-4, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-4, percent); 
motor(PORT4, ratio18_1, true).setVelocity(53, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-4, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-5, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-5, percent); 
motor(PORT4, ratio18_1, true).setVelocity(82, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(8, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(56, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(9, percent); 
motor(PORT2, ratio18_1, false).setVelocity(2, percent); 
motor(PORT3, ratio18_1, true).setVelocity(2, percent); 
motor(PORT4, ratio18_1, true).setVelocity(55, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-17, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-15, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-15, percent); 
motor(PORT4, ratio18_1, true).setVelocity(17, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-59, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-42, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-42, percent); 
motor(PORT4, ratio18_1, true).setVelocity(5, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-11, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-1, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-1, percent); 
motor(PORT4, ratio18_1, true).setVelocity(3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-35, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-25, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-25, percent); 
motor(PORT4, ratio18_1, true).setVelocity(2, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-4, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-12, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-12, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-10, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-3, percent); 
motor(PORT2, ratio18_1, false).setVelocity(-4, percent); 
motor(PORT3, ratio18_1, true).setVelocity(-4, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-1, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(23, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(18, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-4, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-48, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-46, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-52, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-5, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-19, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(13, percent); 
motor(PORT1, ratio18_1, false).setVelocity(34, percent); 
motor(PORT2, ratio18_1, false).setVelocity(93, percent); 
motor(PORT3, ratio18_1, true).setVelocity(93, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-4, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(13, percent); 
motor(PORT1, ratio18_1, false).setVelocity(54, percent); 
motor(PORT2, ratio18_1, false).setVelocity(77, percent); 
motor(PORT3, ratio18_1, true).setVelocity(77, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-7, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(27, percent); 
motor(PORT2, ratio18_1, false).setVelocity(25, percent); 
motor(PORT3, ratio18_1, true).setVelocity(25, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(43, percent); 
motor(PORT2, ratio18_1, false).setVelocity(13, percent); 
motor(PORT3, ratio18_1, true).setVelocity(13, percent); 
motor(PORT4, ratio18_1, true).setVelocity(16, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(17, percent); 
motor(PORT2, ratio18_1, false).setVelocity(27, percent); 
motor(PORT3, ratio18_1, true).setVelocity(27, percent); 
motor(PORT4, ratio18_1, true).setVelocity(5, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(3, percent); 
motor(PORT2, ratio18_1, false).setVelocity(7, percent); 
motor(PORT3, ratio18_1, true).setVelocity(7, percent); 
motor(PORT4, ratio18_1, true).setVelocity(3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(7, percent); 
motor(PORT3, ratio18_1, true).setVelocity(7, percent); 
motor(PORT4, ratio18_1, true).setVelocity(3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(35, percent); 
motor(PORT2, ratio18_1, false).setVelocity(72, percent); 
motor(PORT3, ratio18_1, true).setVelocity(72, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(19, percent); 
motor(PORT2, ratio18_1, false).setVelocity(92, percent); 
motor(PORT3, ratio18_1, true).setVelocity(92, percent); 
motor(PORT4, ratio18_1, true).setVelocity(21, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(3, percent); 
motor(PORT2, ratio18_1, false).setVelocity(10, percent); 
motor(PORT3, ratio18_1, true).setVelocity(10, percent); 
motor(PORT4, ratio18_1, true).setVelocity(11, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(27, percent); 
motor(PORT2, ratio18_1, false).setVelocity(11, percent); 
motor(PORT3, ratio18_1, true).setVelocity(11, percent); 
motor(PORT4, ratio18_1, true).setVelocity(26, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(8, percent); 
motor(PORT2, ratio18_1, false).setVelocity(9, percent); 
motor(PORT3, ratio18_1, true).setVelocity(9, percent); 
motor(PORT4, ratio18_1, true).setVelocity(11, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(4, percent); 
motor(PORT2, ratio18_1, false).setVelocity(7, percent); 
motor(PORT3, ratio18_1, true).setVelocity(7, percent); 
motor(PORT4, ratio18_1, true).setVelocity(5, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(4, percent); 
motor(PORT3, ratio18_1, true).setVelocity(4, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-11, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(17, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(6, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(18, percent); 
motor(PORT2, ratio18_1, false).setVelocity(74, percent); 
motor(PORT3, ratio18_1, true).setVelocity(74, percent); 
motor(PORT4, ratio18_1, true).setVelocity(22, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(37, percent); 
motor(PORT2, ratio18_1, false).setVelocity(102, percent); 
motor(PORT3, ratio18_1, true).setVelocity(102, percent); 
motor(PORT4, ratio18_1, true).setVelocity(47, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(66, percent); 
motor(PORT2, ratio18_1, false).setVelocity(79, percent); 
motor(PORT3, ratio18_1, true).setVelocity(79, percent); 
motor(PORT4, ratio18_1, true).setVelocity(71, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(-1, percent); 
motor(PORT1, ratio18_1, false).setVelocity(38, percent); 
motor(PORT2, ratio18_1, false).setVelocity(16, percent); 
motor(PORT3, ratio18_1, true).setVelocity(16, percent); 
motor(PORT4, ratio18_1, true).setVelocity(21, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(30, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(8, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(3, percent); 
motor(PORT3, ratio18_1, true).setVelocity(3, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-16, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-2, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-5, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-3, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(-3, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(-2, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.115,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.108,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.106,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.107,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.1,sec) 
;IntakeLeft.setVelocity(0, percent); 
IntakeRight.setVelocity(0, percent); 
motor(PORT1, ratio18_1, false).setVelocity(0, percent); 
motor(PORT2, ratio18_1, false).setVelocity(0, percent); 
motor(PORT3, ratio18_1, true).setVelocity(0, percent); 
motor(PORT4, ratio18_1, true).setVelocity(0, percent); 
Sorter.setVelocity(0, percent); 
EscalatorRamp.setVelocity(0, percent); 
wait(0.101,sec) 
;
}