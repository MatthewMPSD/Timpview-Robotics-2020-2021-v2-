#include "drive_functions.h"
#include "vex.h"
#include "vision_functions.h"

int centerX = 150;
int centerY = 160;
int collectedW = 150;
int slack = 80;

void centerOn (vision::signature type)
{
  bool aligned = false;
  int loopCounter = 0;
  FrontVision.takeSnapshot(type);
  while (!aligned)
  {
    if (FrontVision.largestObject.exists)
    {
      if (centerX - slack > FrontVision.largestObject.centerX)
      {
        Drivetrain.turn(left);
      } 
      else if (centerX + slack < FrontVision.largestObject.centerX)
      {
        Drivetrain.turn(right);
      } 
      else 
      {
        Drivetrain.stop();
        aligned = true;
      }
      FrontVision.takeSnapshot(type);
    } else
    {
      if (++loopCounter > 5)
      {
        break;
      }
    }
  }
}

void collectSignature (vision::signature inner_type, vision::signature front_type, int threshold = 0)
{
  double maxCollectionTime = 5000;
  timer collectionTimer;
  intake.inward();
  wait(5, msec);
  Drivetrain.drive(forward);
  while (collectionTimer.time() < maxCollectionTime && detectSignatureInner(inner_type).width < threshold)
  {
    wait(100, msec);
    centerOn(front_type);
    Drivetrain.drive(forward);
  }
  Controller1.Screen.print("COLLECTED");
  Drivetrain.stop();
  intake.off();
}

vision::object detectSignature (vision::signature sig)
{
  FrontVision.takeSnapshot(sig);
  return FrontVision.largestObject;
}

vision::object detectSignatureInner (vision::signature sig)
{
  InnerVision.takeSnapshot(sig);
  return InnerVision.largestObject;
}