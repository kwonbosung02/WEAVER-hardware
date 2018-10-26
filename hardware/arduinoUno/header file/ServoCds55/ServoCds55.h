/*
ServoCds55.h - Library for Digital Servo Driver Shield
May 15th, 2013
Version 0.1
*/


#ifndef ServoCds55_H
#define ServoCds55_H

#include "Arduino.h"


class ServoCds55{
public:
	ServoCds55();
    void WritePos(int ID,int Pos);
    void write(int ID,int Pos);
    void setVelocity(int velocity);
    void setPoslimit(int posLimit);
    void rotate(int ID,int velocity);
    void SetServoLimit(int ID,int upperLimit);
    void SetMotormode(int ID, int velocity);
    void SetID(int ID, int newID);
	void Reset(int ID);
	byte transferAndWait (const byte what);

private:

    int velocity_temp;
    int upperLimit_temp;

};

#endif



