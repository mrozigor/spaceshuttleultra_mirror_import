#include "MPMSystems.h"

MPMSystem::MPMSystem(SubsystemDirector *_director, const std::string &_ident, const char* _meshname)
	: AtlantisSubsystem(_director, _ident)
{
	mesh_index=MESH_UNDEFINED;
	hMesh=oapiLoadMeshGlobal(_meshname);

	MPMRollout.Set(AnimState::OPEN, 1.0);
	MRLLatches.Set(AnimState::OPEN, 1.0);
}

MPMSystem::~MPMSystem()
{
}

void MPMSystem::Realize()
{
	AddMesh();

	DiscreteBundle* pBundle=BundleManager()->CreateBundle(GetIdentifier()+"_MRL", 16);
	Release.Connect(pBundle, 0);
	Latch.Connect(pBundle, 1);
	for(int i=0;i<3;i++) {
		MRL_Rel_Microswitches[i].Connect(pBundle, i+2);
		if(MRLLatches.Open()) MRL_Rel_Microswitches[i].SetLine();
		else MRL_Rel_Microswitches[i].ResetLine();
	}
	for(int i=0;i<3;i++) MRL_RTL_Microswitches[i].Connect(pBundle, i+5);
	for(int i=0;i<3;i++) {
		MRL_Lat_Microswitches[i].Connect(pBundle, i+8);
		if(MRLLatches.Closed()) MRL_Lat_Microswitches[i].SetLine();
		else MRL_Lat_Microswitches[i].ResetLine();
	}
	MRL_Released.Connect(pBundle, 11);
	MRL_Latched.Connect(pBundle, 12);

	if(MRLLatches.Closed()) {
		MRL_Released.ResetLine();
		MRL_Latched.SetLine();
	}
	else if(MRLLatches.Open()) {
		MRL_Released.SetLine();
		MRL_Latched.ResetLine();
	}

	pBundle=BundleManager()->CreateBundle(GetIdentifier(), 16);
	Deploy.Connect(pBundle, 0);
	Stow.Connect(pBundle, 1);
	MPM_Deployed.Connect(pBundle, 2);
	MPM_Stowed.Connect(pBundle, 3);

	if(MPMRollout.Open()) {
		MPM_Deployed.SetLine();
		MPM_Stowed.ResetLine();
	}
	else if(MPMRollout.Closed()) {
		MPM_Deployed.ResetLine();
		MPM_Stowed.SetLine();
	}
}

void MPMSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(Deploy && !MPMRollout.Open()) {
		if(!MPMRollout.Opening()) {
			MPMRollout.action=AnimState::OPENING;
			MPM_Stowed.ResetLine();
		}
		MPMRollout.Move(DeltaT*MPM_DEPLOY_SPEED);
		STS()->SetAnimation(anim_mpm, MPMRollout.pos);
		if(MPMRollout.Open()) {
			MPM_Deployed.SetLine();
		}
	}
	else if(Stow && !MPMRollout.Closed()) {
		if(!MPMRollout.Closing()) {
			MPMRollout.action=AnimState::CLOSING;
			MPM_Deployed.ResetLine();
		}
		MPMRollout.Move(DeltaT*MPM_DEPLOY_SPEED);
		STS()->SetAnimation(anim_mpm, MPMRollout.pos);
		if(MPMRollout.Closed()) MPM_Stowed.SetLine();
	}

	if(Release && !MRLLatches.Open()) {
		if(!MRLLatches.Opening()) {
			MRLLatches.action=AnimState::OPENING;
			for(int i=0;i<3;i++) MRL_Lat_Microswitches[i].ResetLine();
			MRL_Latched.ResetLine();
			MRL_Released.ResetLine();
		}
		MRLLatches.Move(DeltaT*MRL_LATCH_SPEED);
		if(MRLLatches.Open()) {
			for(int i=0;i<3;i++) MRL_Rel_Microswitches[i].SetLine();
			MRL_Released.SetLine();

			OnMRLReleased();
		}
	}
	else if(Latch && !MRLLatches.Closed()) {
		if(!MRLLatches.Closing()) {
			MRLLatches.action=AnimState::CLOSING;
			for(int i=0;i<3;i++) MRL_Rel_Microswitches[i].ResetLine();
			MRL_Latched.ResetLine();
			MRL_Released.ResetLine();
		}
		MRLLatches.Move(DeltaT*MRL_LATCH_SPEED);
		if(MRLLatches.Closed()) {
			for(int i=0;i<3;i++) MRL_Lat_Microswitches[i].SetLine();
			MRL_Latched.SetLine();

			OnMRLLatched();
		}
	}
}

bool MPMSystem::OnParseLine(const char* line)
{
	std::string rollout=GetIdentifier()+"_ROLLOUT";
	std::string latches=GetIdentifier()+"_LATCHES";

	if(!_strnicmp(line, rollout.c_str(), rollout.length())) {
		sscan_state((char*)(line+rollout.length()), MPMRollout);
		oapiWriteLog((char*)rollout.c_str());
		return true;
	}
	else if(!_strnicmp(line, latches.c_str(), latches.length())) {
		sscan_state((char*)(line+latches.length()), MRLLatches);
		oapiWriteLog((char*)latches.c_str());
		return true;
	}
	char cbuf[255];
	sprintf_s(cbuf, 255, "MPMSystem: no matches for %s", line);
	oapiWriteLog(cbuf);
	return false;
}

void MPMSystem::OnSaveState(FILEHANDLE scn) const
{
	WriteScenario_state(scn, (char*)(GetIdentifier()+"_ROLLOUT").c_str(), MPMRollout);
	WriteScenario_state(scn, (char*)(GetIdentifier()+"_LATCHES").c_str(), MRLLatches);
}

void MPMSystem::AddMesh()
{
	VECTOR3 ofs=STS()->GetOrbiterCoGOffset();
	mesh_index=STS()->AddMesh(hMesh, &ofs);
	STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_EXTERNAL|MESHVIS_VC|MESHVIS_EXTPASS);
}

void MPMSystem::OnMRLLatched()
{
}

void MPMSystem::OnMRLReleased()
{
}
