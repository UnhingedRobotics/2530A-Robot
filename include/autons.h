#pragma once
#include "JAR-Template/drive.h"
#include "drive-functions.h"

class ArmControl;
class IntakeControl;
class Drive;

extern ArmControl armControl;
extern IntakeControl intakeControl;
extern Drive chassis;

void default_constants();

void skills();
void winpoint();
