#include "Keyboard.h"
#include "CRT.h"
#include <cstdio>
#include <string>
#include "dps/dps_defs.h"
#include "dps/IDP.h"

// ==============================================================

Keyboard::Keyboard(Atlantis *_sts, int id): sts(_sts) {
	keyboard=id;
}

void Keyboard::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;

	if(keyboard==0) {
		oapiVCRegisterArea (AID_KYBD_CDR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
		oapiVCSetAreaClickmode_Quadrilateral (AID_KYBD_CDR, _V(-0.252, 1.937, 14.525)+ofs, _V(-0.154, 1.937, 14.525)+ofs, 
			_V(-0.252, 1.769, 14.365)+ofs, _V(-0.154, 1.769, 14.365)+ofs);
		//oapiVCSetAreaClickmode_Quadrilateral (AID_KYBD_CDR, _V(-0.26, 1.868, 14.877)+ofs, _V(-0.158, 1.868, 14.877)+ofs, 
			//_V(-0.26, 1.696, 14.711)+ofs, _V(-0.158, 1.696, 14.711)+ofs);
	}

	else if(keyboard==1) {
		oapiVCRegisterArea (AID_KYBD_PLT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
		oapiVCSetAreaClickmode_Quadrilateral (AID_KYBD_PLT, _V(0.146, 1.937, 14.525)+ofs, _V(0.245, 1.937, 14.525)+ofs, 
			_V(0.146, 1.769, 14.365)+ofs, _V(0.245, 1.769, 14.365)+ofs);
	}
}

bool Keyboard::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if(keyboard==0 && id!=AID_KYBD_CDR) return false;
	else if(keyboard==1 && id!=AID_KYBD_PLT) return false;
	//else if(sts->Display[sts->CRT_SEL[keyboard]]==NULL) return false;
	
	
	//first column of buttons
	if(p.x>=0.0 && p.x<0.25) {
		if(p.y>=0.375 && p.y<0.5) {
			if(sts->DataInput[sts->CRT_SEL[keyboard]].PRO || sts->DataInput[sts->CRT_SEL[keyboard]].EXEC)
				ClearInput();

			sts->DataInput[sts->CRT_SEL[keyboard]].ITEM=true;
			PutKey(SSU_KEY_ITEM);
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].EXEC=true;

			PutKey(SSU_KEY_EXEC);

			/*if(sts->DataInput[sts->CRT_SEL[keyboard]].ITEM) {
				//parse entry
				int i;
				bool delim=false;
				string Data, Name;
				for(i=0;i<sts->DataInput[sts->CRT_SEL[keyboard]].InputSize;i++) {
					if(sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='+' || sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='-') {
						if(delim) break;
						delim=true;
					}
					if(!delim) Name+=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
					else {
						Data+=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
					}
				}
				sts->Input(sts->CRT_SEL[keyboard], 1, Name.c_str(), Data.c_str());
				Data=""; //clear string
				while(i<sts->DataInput[sts->CRT_SEL[keyboard]].InputSize) {
					if(sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='+' || sts->DataInput[sts->CRT_SEL[keyboard]].input[i]=='-') {
						if(Data.length()>0) {
							sts->item++;
							sts->Input(sts->CRT_SEL[keyboard], 3, Name.c_str(), Data.c_str());
						}
						Data=""; //clear string
					}
					Data+=sts->DataInput[sts->CRT_SEL[keyboard]].input[i];
					i++;
				}
				if(Data.length()>0) {
					sts->item++;
					sts->Input(sts->CRT_SEL[keyboard], 3, Name.c_str(), Data.c_str());
				}
			}
			else {
				const char* scratchPad=sts->GetIDP(sts->CRT_SEL[keyboard]+1)->GetScratchPadLineScan();
				sprintf_s(oapiDebugString(), 255, "Scratch Pad: %s %d", scratchPad, strlen(scratchPad));
				if(strlen(scratchPad)==1) sts->Input(sts->CRT_SEL[keyboard], 10, NULL);
			}*/
			ClearInput();
		}
		else if(p.y>=0.625 && p.y<0.75) {
			ClearInput();
			sts->DataInput[sts->CRT_SEL[keyboard]].OPS=true;
			PutKey(SSU_KEY_OPS);
		}
		else if(p.y>=0.75 && p.y<0.875) {
			ClearInput();
			sts->DataInput[sts->CRT_SEL[keyboard]].SPEC=true;
			PutKey(SSU_KEY_SPEC);
		}
		else if(p.y>=0.875 && p.y<1.0) {
			ClearInput();
			//sts->DataInput[sts->CRT_SEL[keyboard]].SPEC=true;
			//Resume key
			//sts->Input(sts->CRT_SEL[keyboard], DEU_KEY_RESUME, NULL, NULL);
			PutKey(SSU_KEY_RESUME);
		}
	}

	//second column of buttons
	else if(p.x>=0.25 && p.x<0.5) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='A';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_A);
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='D';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_D);
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='1';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_1);
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='4';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_4);
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='7';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_7);
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='-';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_MINUS);
		}
		else if(p.y>=0.875 && p.y<=1.0) {
			PutKey(SSU_KEY_CLEAR);
			if(sts->DataInput[sts->CRT_SEL[keyboard]].PRO || sts->DataInput[sts->CRT_SEL[keyboard]].EXEC)
				ClearInput();
			else {
				if(sts->DataInput[sts->CRT_SEL[keyboard]].InputSize>0) {
					sts->DataInput[sts->CRT_SEL[keyboard]].InputSize--;
					sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='\0';
					//sprintf(oapiDebugString(), "%s %d", sts->DataInput[sts->CRT_SEL[keyboard]].input, sts->DataInput[sts->CRT_SEL[keyboard]].InputSize);
				}
				else ClearInput();
			}
		}
	}

	//third column of buttons
	else if(p.x>=0.5 && p.x<0.75) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='B';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_B);
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='E';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_E);
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='2';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_2);
		}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='5';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_5);
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='8';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_8);
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='0';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_0);
		}
		else if(p.y>=0.875 && p.y<=1.0) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='.';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_DOT);
		}
	}

	//fourth column of buttons
	else if(p.x>=0.75 && p.x<=1.0) {
		if(p.y>=0.125 && p.y<0.25) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='C';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_C);
		}
		else if(p.y>=0.25 && p.y<0.375) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='F';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_F);
		}
		else if(p.y>=0.375 && p.y<0.5) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='3';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_3);
					}
		else if(p.y>=0.5 && p.y<0.625) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='6';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_6);
			
		}
		else if(p.y>=0.625 && p.y<0.75) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='9';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_9);
			
		}
		else if(p.y>=0.75 && p.y<0.875) {
			sts->DataInput[sts->CRT_SEL[keyboard]].input[sts->DataInput[sts->CRT_SEL[keyboard]].InputSize]='+';
			sts->DataInput[sts->CRT_SEL[keyboard]].InputSize++;
			PutKey(SSU_KEY_PLUS);
			
		}
		else if (p.y>=0.875 && p.y<=1.0) {
			sts->DataInput[sts->CRT_SEL[keyboard]].PRO=true;
			PutKey(SSU_KEY_PRO);
			
			/*if(sts->DataInput[sts->CRT_SEL[keyboard]].OPS)
				sts->Input(sts->CRT_SEL[keyboard], 0, sts->DataInput[sts->CRT_SEL[keyboard]].input);
			else if(sts->DataInput[sts->CRT_SEL[keyboard]].SPEC)
				sts->Input(sts->CRT_SEL[keyboard], 2, sts->DataInput[sts->CRT_SEL[keyboard]].input);*/
		}
	} else {
		sprintf_s(oapiDebugString(), 255, "KEYBOARD %c| ID %d EVENT %d p %f %f %f",
			(keyboard==0)?'C':'P', id, event,p.x,p.y,p.z);
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


void Keyboard::PutKey(char ckey) {
	switch(keyboard) {
		/*case 0:
			sts->GetIDP(1)->PutKey(keyboard, ckey);
			sts->GetIDP(3)->PutKey(keyboard, ckey);
			return;
		case 1:
			sts->GetIDP(2)->PutKey(keyboard, ckey);
			sts->GetIDP(3)->PutKey(keyboard, ckey);
			return;*/
		case 0:
		case 1:
			sts->GetIDP(sts->CRT_SEL[keyboard]+1)->PutKey(keyboard, ckey);
			return;
		case 2:
			sts->GetIDP(4)->PutKey(keyboard, ckey);
			return;
	}
}