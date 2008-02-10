#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "Atlantis.h"

// ==============================================================
// class Keyboard
// data entry Keyboards
// ==============================================================

class Keyboard
{
	friend class Atlantis;
	friend class CRT;

public:
	Keyboard(Atlantis *_sts, int id); //0=CDR, 1=PLT
	void RegisterVC();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	void ClearInput();

private:
	Atlantis *sts; // vessel instance pointer

	int keyboard; //0=CDR, 1=PLT
};

#endif //__KEYBOARD_H