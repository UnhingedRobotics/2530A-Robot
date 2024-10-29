#pragma once
#include "JAR-Template/drive.h"
#include "drive-functions.h"

class ArmControl;
class Drive;

extern ArmControl armmotor;
extern Drive chassis;

void default_constants();

void skills();
void winpoint();
