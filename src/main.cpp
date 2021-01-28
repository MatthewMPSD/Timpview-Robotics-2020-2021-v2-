/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\mm023739                                         */
/*    Created:      Tue Dec 08 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 2, 3, 4, 9   
// IntakeLeft           motor         6               
// IntakeRight          motor         5               
// EscalatorRamp        motor         8               
// Sorter               motor         7               
// FrontVision          vision        10              
// InnerVision          vision        20              
// BallDetector         bumper        G               
// Controller1          controller                    
// TeamSwitcher         limit         H               
// ModePotentiometer    pot           F               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "drive_functions.h"
#include "vision_functions.h"
#include "mode_selector.h"
#include "variables.h"
#include "strategies.h"

using namespace vex;

//Modes:
//0: Left Corner
//1: Middle and Left
//2: Right Corner
//3: Middle and Right
//4: Skills

void checkSorter ()
{
  while (Competition.isAutonomous() || Competition.isDriverControl())
  {
    if (detectSignatureInner(*enemySignatureInner).width > 50 && doSort)
    {
      escalator.setSorterDirection(reverse);
    }
    else
    {
      escalator.setSorterDirection(forward);
    }
  }
}

void autonomousMode ()
{
  autonomousConfig();

  switch (modeSelector.mode())
  {
    case (0):
    case (2):
      cornerStrategy();
      break;
    case (1):
    case (3):
      middleStrategy();
      break;
    case (4):
      skillsAutonomous();
      break;
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  allySignatureInner = TeamSwitcher.pressing() ? &InnerVision__RED_BALL : &InnerVision__BLUE_BALL;
  enemySignatureInner = TeamSwitcher.pressing() ? &InnerVision__BLUE_BALL : &InnerVision__RED_BALL;

  allySignature = TeamSwitcher.pressing() ? &FrontVision__RED_BALL : &FrontVision__BLUE_BALL;
  enemySignature = TeamSwitcher.pressing() ? &FrontVision__BLUE_BALL : &FrontVision__RED_BALL;

  intake.setPower(intakePower);
  escalator.setPower(escalatorPower);
  Sorter.setVelocity(sorterPower, percent);
  Drivetrain.setDriveVelocity(drivetrainPower, percent);
  Drivetrain.setTurnVelocity(drivetrainTurnPower, percent);
  escalator.dir = 1;

  thread(checkSorter).detach();
  
  Competition.drivercontrol(rerunProgram);
  Competition.autonomous(doRerun);
}
