#include "IDP.h"
#include "IDPSoftware.h"

namespace dps {

	IDP::IDP(SubsystemDirector* _direct, const std::string& _ident, 
		unsigned short _usIDPID)
		: AtlantisSubsystem(_direct, _ident), usIDPID(_usIDPID)
	{
		usSPEC=0;
		usDISP=0;
		majfunc=GNC;
		cScratchPadLine[0] = '\0';
		CreateSoftware();
	}

	IDP::~IDP()
	{
		for(int i = 0; i<software_storage.size(); i++) {
			delete software_storage[i];
		}
		software_storage.clear();
	}

	void IDP::CreateSoftware() {
		IDPSoftware* pSoftware;

		pSoftware = new IDP_OTP(this);
		software_storage.push_back(pSoftware);
		pOTP = pSoftware;
		
	}

	unsigned short IDP::GetIDPID() const {
		return usIDPID;
	}

	unsigned short IDP::GetDisp() const
	{
		return usDISP;
	}

	unsigned short IDP::GetOps() const
	{
		return usOPS;
	}

	unsigned short IDP::GetSpec() const
	{
		return usSPEC;
	}

	MAJORFUNCTION IDP::GetMajfunc() const
	{
		return majfunc;
	}

	void IDP::IPL() {

		memstate = MS_IPL;

		//clear all data tables
		//rebuild all data tables
	}

	bool IDP::IsBFS() const {
		return false;
	}

	bool IDP::IsCompleteLine() const {
		return false;
	}

	bool IDP::PutKey(unsigned short usKeyboardID, char cKey) {
		
		//TODO: Implement checking of active keyboard
		switch(cKey) {
			case SSU_KEY_CLEAR:
				OnClear();
				break;
			case SSU_KEY_EXEC:
				OnExec();
				break;
		}
		AppendScratchPadLine(cKey);
		sprintf_s(oapiDebugString(), 255, "IDP %d|PutKey(%d, %02X)| %s", 
			usIDPID, usKeyboardID, cKey, 
			this->GetScratchPadLineString());
		return true;
	}

	void IDP::SetDisp(unsigned short disp)
	{
		usDISP=disp;
	}

	void IDP::SetSpec(unsigned short spec)
	{
		usSPEC=spec;
	}

	void IDP::SetMajFunc(MAJORFUNCTION func)
	{
		majfunc=func;
	}

	void IDP::ClearScratchPadLine() {
		cScratchPadLine[0] = '\0';
	}

	void IDP::ConnectToMDU(vc::PMDU pMDU, bool bPrimary)
	{
		if(pMDU) {
			if(bPrimary) {
				pMDU->SetPrimaryIDP(this);
			}
			else pMDU->SetSecondaryIDP(this);
		}
	}

	void IDP::OnSaveState(FILEHANDLE scn) const
	{
		char cbuf[255];
		sprintf_s(cbuf, 255, "IDP%d SPEC", usIDPID);
		oapiWriteScenario_int(scn, cbuf, usSPEC);
	}

	bool IDP::OnParseLine(const char* line)
	{
		char IDP[10];
		sprintf_s(IDP, 10, "IDP%d", usIDPID);
		if(!_strnicmp(IDP, line, 4)) {
			if(!_strnicmp(line+5, "SPEC", 4)) {
				sscanf_s(line+9, "%d", &usSPEC);
				return true;
			}
		}
		return false;
	}

	void IDP::OnAck() {
	}

	void IDP::OnClear() {

		if(IsCompleteLine()) {
			ClearScratchPadLine();
		} else {
			DelFromScratchPadLine();
		}
		
	}

	void IDP::OnExec() {

	}

	void IDP::OnFaultSummary() {
	}


	void IDP::OnMMChange(unsigned short usMM) {
		ClearScratchPadLine();
	}

	bool IDP::OnPaint(vc::MDU* pMDU) {

		//Clear text buffer, if needed

		PrintTime(pMDU);
		//delegate painting to software


		return true;
	}

	void IDP::OnSysSummary() {
	}


	void IDP::OnMsgReset() {
	}

	
	void IDP::PrintTime(vc::MDU* mdu) {
		char pszBuffer[15];

		sprintf_s(pszBuffer, 15, "%03d/%02d:%02d:%02d",
			usGPCDay, usGPCHour, usGPCMinute, usGPCSecond);
		mdu->PrintToBuffer(pszBuffer, 12, 39, 1, 0);

		if(bGPCTimerActive) {
			sprintf_s(pszBuffer, 15, "%03d/%02d:%02d:%02d",
				usTimerDay, usTimerHour, usTimerMinute, usTimerSecond);
			mdu->PrintToBuffer(pszBuffer, 12, 39, 2, 0);
		}
	}


	const char* IDP::GetScratchPadLineScan() const {
		return cScratchPadLine;
	}

	void IDP::AppendScratchPadLine(char cKey) {
		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i < 119) 
			i++;
		cScratchPadLine[i] = cKey;
		cScratchPadLine[i+1] = '\0';
		
	}

	void IDP::DelFromScratchPadLine() {
		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i < 119) 
			i++;
		if(i>0) {
			cScratchPadLine[i-1] = '\0';
		}
	}

	const char* IDP::GetScratchPadLineString() const {
		static char pszBuffer[250];

		pszBuffer[0] = '\0';

		unsigned short i = 0;
		while(cScratchPadLine[i] != '\0' && i<120) {
			switch(cScratchPadLine[i]) {
				case SSU_KEY_ITEM:
					strcat_s(pszBuffer, "ITEM");
					break;
				case SSU_KEY_GPCIDP:
					strcat_s(pszBuffer, "GPC/IDP");
					break;
				case SSU_KEY_OPS:
					strcat_s(pszBuffer, "OPS");
					break;
				case SSU_KEY_A:
					strcat_s(pszBuffer, "A");
					break;
				case SSU_KEY_B:
					strcat_s(pszBuffer, "B");
					break;
				case SSU_KEY_C:
					strcat_s(pszBuffer, "C");
					break;
				case SSU_KEY_D:
					strcat_s(pszBuffer, "D");
					break;
				case SSU_KEY_E:
					strcat_s(pszBuffer, "E");
					break;
				case SSU_KEY_F:
					strcat_s(pszBuffer, "F");
					break;
				case SSU_KEY_1:
					strcat_s(pszBuffer, "1");
					break;
				case SSU_KEY_2:
					strcat_s(pszBuffer, "2");
					break;
				case SSU_KEY_3:
					strcat_s(pszBuffer, "3");
					break;
				case SSU_KEY_4:
					strcat_s(pszBuffer, "4");
					break;
				case SSU_KEY_5:
					strcat_s(pszBuffer, "5");
					break;
				case SSU_KEY_6:
					strcat_s(pszBuffer, "6");
					break;
				case SSU_KEY_7:
					strcat_s(pszBuffer, "7");
					break;
				case SSU_KEY_8:
					strcat_s(pszBuffer, "8");
					break;
				case SSU_KEY_9:
					strcat_s(pszBuffer, "9");
					break;
				case SSU_KEY_0:
					strcat_s(pszBuffer, "0");
					break;
				case SSU_KEY_DOT:
					strcat_s(pszBuffer, ".");
					break;
				case SSU_KEY_PLUS:
					strcat_s(pszBuffer, "+");
					break;
				case SSU_KEY_MINUS:
					strcat_s(pszBuffer, "-");
					break;
				case SSU_KEY_EXEC:
					strcat_s(pszBuffer, "EXEC");
					break;
				case SSU_KEY_ACK:
					strcat_s(pszBuffer, "ACK");
					break;
				case SSU_KEY_IORESET:
					strcat_s(pszBuffer, "I/O RESET");
					break;
				default:
					break;
			}
			i++;
		}

		return pszBuffer;
	}
};
