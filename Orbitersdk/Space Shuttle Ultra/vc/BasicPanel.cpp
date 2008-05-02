#include "../Atlantis.h"
#include "BasicPanel.h"


namespace vc {

	BasicPanel::BasicPanel(Atlantis* sts, const string& _name)
	{
	}

	BasicPanel::~BasicPanel()
	{
	}

	void BasicPanel::DefineSwitches()
	{
	}

	void BasicPanel::DefineVCAnimations(UINT vcidx)
	{
	}

	void BasicPanel::RegisterVC()
	{
	}

	void BasicPanel::Realize()
	{
	}

	bool BasicPanel::OnParseLine(char *line)
	{
		return false;
	}

	void BasicPanel::OnSaveState(FILEHANDLE scn)
	{
	}

	bool BasicPanel::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		return false;
	}

	bool BasicPanel::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		return false;
	}

	void BasicPanel::DefinePanelMouseArea()
	{
	}

	void BasicPanel::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void BasicPanel::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void BasicPanel::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}

	bool BasicPanel::HasSwitch(const string& id)
	{
		return false;
	}

	const string& BasicPanel::GetIdentifier() const
	{
		static string null = "";
		return null;
	}

	const string& BasicPanel::GetQualifiedIdentifier() const
	{
		static string null = "";
		return null;
	}

};