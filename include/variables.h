#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "vex.h"
#include "mode_selector.h"
#include "vex.h"
#include "mode_selector.h"

extern signature* allySignatureInner;
extern signature* enemySignatureInner;

extern signature* allySignature;
extern signature* enemySignature;

//Define competition object
extern competition Competition;
extern ModeSelector modeSelector;


extern bool hasPressedUp;
extern timer timePressedUp;
extern bool doSort;

#endif