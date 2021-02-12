#include "strategies.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>

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
  int angleModifier = modeSelector.mode() == 0 ? 1 : -1;
  Drivetrain.driveFor(reverse, 18, inches);
  Drivetrain.setDriveVelocity(drivetrainTurnPowerAuto/2, percent);
  Drivetrain.turnToHeading(45*angleModifier, degrees);
  Drivetrain.setDriveVelocity(drivetrainTurnPowerAuto, percent);
  centerOn(*enemySignature);
  intake.inward();
  driveForTime(forward, 800, msec);
  escalator.up();
  centerOn(*enemySignature);
  driveForTime(forward, 300, msec);
  wait(1500, msec);
  shimmy(300, 500, msec, 2);
  wait(2, sec);
  escalator.off();
  intake.off();
  driveForTime(reverse, 500, msec);
}

void middleStrategy ()
{
  doSort = false;
  int angleModifier = modeSelector.mode() == 1 ? 1 : -1;
  Drivetrain.turnToHeading(35*angleModifier, degrees);
  driveForTime(forward, 850, msec);
  escalator.up();
  wait(1500, msec);
  escalator.off();
  intake.outward();
  driveForTime(reverse, 1750, msec);
  intake.inward();
  Drivetrain.turnToHeading(135*angleModifier, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 500, msec);
  centerOn(*enemySignature);
  driveForTime(forward, 800, msec);
  escalator.up();
  wait(1500, msec);
  shimmy(300, 500, msec, 1);
  wait(1500, msec);
  intake.outward();
  driveForTime(reverse, 250, msec);
  escalator.off();
  intake.off();
  doSort = true;
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
      std::swap(enemySignature, allySignature);
      std::swap(enemySignatureInner, allySignatureInner);
      Controller1.rumble(rumblePulse);
    } else
    {
      hasPressedUp = true;
    }
    timePressedUp.reset();
  });

  //Turn off sorting
  Controller1.ButtonY.pressed([](){
    Controller1.rumble(rumblePulse);
    doSort = !doSort;
    });

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


  //Intake system
  Controller1.ButtonL2.pressed([](){intake.inward();});
  Controller1.ButtonL2.released([](){intake.off();});
  Controller1.ButtonL1.pressed([](){intake.outward();});
  Controller1.ButtonL1.released([](){intake.off();});

  //Escalator system
  Controller1.ButtonA.pressed([](){escalator.toggle();});

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
  escalator.up();
  intake.inward();

  //Corner 2
  driveForTime(forward, 1500, msec);
  shimmy(300, 500, msec, 1);
  Drivetrain.driveFor(reverse, 12, inches);

  //Middle 1
  Drivetrain.turnToHeading(-90, degrees);
  Drivetrain.driveFor(forward, 54, inches);
  Drivetrain.turnToHeading(0, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 1000, msec);
  shimmy(300, 500, msec, 1);
  Drivetrain.driveFor(reverse, 24, inches);

  //Corner 2
  Drivetrain.turnToHeading(-90, degrees);
  Drivetrain.driveFor(forward, 42, inches);
  Drivetrain.turnToHeading(-45, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 3, sec);
  shimmy(300, 500, msec, 2);
  Drivetrain.driveFor(reverse, 18, inches);

  //Middle 2
  Drivetrain.turnToHeading(-180, degrees);
  Drivetrain.driveFor(forward, 42, inches);
  Drivetrain.turnToHeading(-90, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 1, sec);
  shimmy(300, 500, msec, 1);
  Drivetrain.driveFor(reverse, 12, inches);

  //Corner 3
  Drivetrain.turnToHeading(-180, degrees);
  Drivetrain.driveFor(forward, 42, inches);
  Drivetrain.turnToHeading(-135, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 2, sec);
  shimmy(300, 500, msec, 3);
  Drivetrain.driveFor(reverse, 18, inches);

  //Middle 3
  Drivetrain.turnToHeading(90, degrees);
  Drivetrain.driveFor(forward, 44, inches);
  Drivetrain.turnToHeading(180, degrees);
  centerOn(*enemySignature);
  driveForTime(forward, 2, sec);
  shimmy(300, 500, msec, 3);
  Drivetrain.driveFor(reverse, 18, inches);
}

void doRerun ()
{
}