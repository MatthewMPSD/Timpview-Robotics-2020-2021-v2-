#include "variables.h"

signature* allySignatureInner;
signature* enemySignatureInner;

signature* allySignature;
signature* enemySignature;

//Define competition object
competition Competition;
ModeSelector modeSelector(ModePotentiometer, 5);

bool hasPressedUp = false;
timer timePressedUp;
bool doSort = true;