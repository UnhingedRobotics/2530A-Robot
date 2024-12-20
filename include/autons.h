#pragma once
#include "JAR-Template/drive.h"
#include "drive-functions.h"

class IntakeControl;
class Drive;

extern IntakeControl intakeControl;
extern Drive chassis;

void default_constants();

void red_left_elims();
void red_left_winpoint();
void blue_left_elims();
void blue_left_winpoint();
void skills();
