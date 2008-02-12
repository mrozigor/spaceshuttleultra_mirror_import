#include "Keyboard.h"
#include "CRT.h"
#include <cstdio>

// ==============================================================

Keyboard::Keyboard(Atlantis *_sts, int id): sts(_sts) {
	keyboard=id;
}

void Keyboard::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;

	if(keyboard==0) {
		oapiVCRegisterArea (AID_KYBD_CDR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
		oapiVCSetAreaClickmode_Quadrilateral (AID_KYBD_CDR, _V(-0.26, 1.868, 14.877)+ofs, _V(-0.158, 1.868, 14.877)+ofs, 
			_V(-0.26, 1.696, 14.711)+ofs, _V(-0.158, 1.696, 14.711)+ofs);
	}

	else if(keyboard==1) {
		oapiVCRegisterArea (AID_KYBD_PLT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
		oapiVCSetAreaClickmode_Quadrilateral (AID_KYBD_PLT, _V(0.151, 1.868, 14.877)+ofs, _V(0.252, 1.868, 14.877)+ofs, 
			_V(0.151, 1.696, 14.711)+ofs, _V(0.252, 1.696, 14.711)+ofs);
	}
}

bool Keyboard::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if(keyboard==0 && id!=AID_KYBD_CDR) return false;
	else if(keyboard==1 && id!=AID_KYBD_PLT) return false;
	else if(sts->Display[sts->CRT_SEL[keyboard]]==NULL) return false;
	sprintf(oapiDebugString(), "Keyboard: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);
	
	//first column of buttons
	if(p.x>=0.0 && p.x<0.25) {
		if(p.y>=0.375 && p.y<0.5) {
			ClearInput();
			sts->DataInput[sts->CRT_SEL[keyboard]].ITEM=true;
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].EXEC=true;
			if(sts->DataInput[sts->CRT_SEL[keyboard]].ITEM) {
				//parse entry
				int i, j;
				bool delim=false;
				char Data[255];
				char Name[255];
				for(i=0;i<sts->DataInput[sts->CRT_SEL[keyboard]].InputSize;i++) {
					if(sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='+' || sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='-') {
						if(delim) break;
						delim=true;
						j=0;
					}
					if(!delim) Name[i]=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
					else {
						Data[j]=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
						j++;
					}
				}
				sts->Input(sts->CRT_SEL[keyboard], 1, Name, Data);
				j=0;
				while(i<sts->DataInput[sts->CRT_SEL[keyboard]].InputSize) {
					if(sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='+' || sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='-') {
						if(j!=0) {
							sts->item++;
							sts->Input(sts->CRT_SEL[keyboard], 3, Name, Data);
						}
						j=0;
					}
					Data[j]=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
					j++;
					i++;
				}
				if(j!=0) {
					sts->item++;
					sts->Input(sts->CRT_SEL[keyboard], 3, Name, Data);
				}
			}
		}
		else if(p.y>=0.625 && p.y<0.75) {
			ClearInput();
			sts->DataInput[sts->CRT_SEL[keyboard]].OPS=true;
		}
		else if(p.y>=0.75 && p.y<0.875) {
			ClearInput();
			sts->DataInput[sts->CRT_SEL[keyboard]].SPEC=true;
		}
	}

	//second column of buttons
	if(p.x>=0.25 && p.x<0.5) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='A';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='D';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='1';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='4';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='7';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='-';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.875 && p.y<=1.0) {
			if(sts->DataInput[sts->CRT_SEL[keyboard]].PRO || sts->DataInput[sts->CRT_SEL[keyboard]].EXEC)
				ClearInput();
			else {
				if(sts->DataInput[sts->CRT_SEL[keyboard]].InputSize>0) {
					sts->DataInput[sts->CRT_SEL[keyboard]].InputSize--;
					sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='\0';
					sprintf(oapiDebugString(), "%s %d", sts->DataInput[sts->CRT_SEL[keyboard]].input, sts->DataInput[sts->CRT_SEL[keyboard]].InputSize);
				}
				else ClearInput();
			}
		}
	}

	//third column of buttons
	if(p.x>=0.5 && p.x<0.75) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='B';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='E';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='2';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			sprintf(oapiDebugString(), "%s %d", sts->DataInput[sts->CRT_SEL[keyboard]].input, sts->DataInput[sts->CRT_SEL[keyboard]].InputSize);
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='5';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='8';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='0';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.875 && p.y<=1.0) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='.';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
	}

	//fourth column of buttons
	if(p.x>=0.75 && p.x<=1.0) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='C';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='F';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='3';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='6';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='9';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='+';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
		}
		else if (p.y>=0.875 && p.y<=1.0) {
			sts->DataInput[sts->CRT_SEL[keyboard]].PRO=true;
			if(sts->DataInput[sts->CRT_SEL[keyboard]].OPS)
				sts->Input(sts->CRT_SEL[keyboard], 0, sts->DataInput[sts->CRT_SEL[keyboard]].input);
			else if(sts->DataInput[sts->CRT_SEL[keyboard]].SPEC)
				sts->Input(sts->CRT_SEL[keyboard], 2, sts->DataInput[sts->CRT_SEL[keyboard]].input);
		}
	}
	return false; //check return value
}

void Keyboard::ClearInput()
{
	int i;
	//sprintf(sts->DataInput[sts->CRT_SEL[keyboard]].input, "");
	for(i=0;i<sts->DataInput[sts->CRT_SEL[keyboard]].InputSize;i++) {
		sts->DataInput[sts->CRT_SEL[keyboard]].input[i]='\0';
	}
	//sprintf(oapiDebugString(), "%s %d", sts->DataInput[sts->CRT_SEL[keyboard]].input, sts->DataInput[sts->CRT_SEL[keyboard]].InputSize);
	sts->DataInput[sts->CRT_SEL[keyboard]].InputSize=0;
	sts->DataInput[sts->CRT_SEL[keyboard]].SPEC=false;
	sts->DataInput[sts->CRT_SEL[keyboard]].ITEM=false;
	sts->DataInput[sts->CRT_SEL[keyboard]].EXEC=false;
	sts->DataInput[sts->CRT_SEL[keyboard]].PRO=false;
	sts->DataInput[sts->CRT_SEL[keyboard]].OPS=false;
}