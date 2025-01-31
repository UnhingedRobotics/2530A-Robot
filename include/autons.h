#pragma once
#include "JAR-Template/drive.h"
#include "drive-functions.h"

class IntakeControl;
class Drive;

extern IntakeControl intakeControl;
extern Drive chassis;

void default_constants();

void red_left_4_ring();
void red_left_winpoint();
void red_right_2_ring();
void blue_right_4_ring();
void blue_right_winpoint();
void blue_left_2_ring();
void skills();
