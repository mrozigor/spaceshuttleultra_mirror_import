#include "StandardLight2.h"
#include "../Atlantis.h"

namespace vc
{
	StandardLight2::StandardLight2(Atlantis* _sts, const string& _ident, UINT _midx, UINT _mshgrp)
		: BasicLight(_sts, _ident), midx(_midx), mshgrp(_mshgrp)
	{
		
	}

	StandardLight2::~StandardLight2()
	{

	}

	bool StandardLight2::Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine)
	{
		return false;
	}

	void StandardLight2::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		if (input.IsSet() != currentState) {
			currentState = input.IsSet();
			
			setTexture();
		}
	}

	void StandardLight2::setTexture() 
	{
		if (currentState)
		{
			modifyTexture(u1a, v1a, u2a, v2a);
		}
		else
		{
			modifyTexture(u1, v1, u2, v2);
		}
	}

	void StandardLight2::modifyTexture(float u1, float v1, float u2, float v2)
	{
		VISHANDLE vis = STS()->GetVisual();
		if (vis != NULL)
		{
			DEVMESHHANDLE dmsh = STS()->GetDevMesh(vis, midx);

			GROUPEDITSPEC grpEditSpec;
			NTVERTEX changes[4];

			grpEditSpec.flags = GRPEDIT_VTXTEX;
			grpEditSpec.nVtx = 4;
			grpEditSpec.vIdx = vtx;
			grpEditSpec.Vtx = changes;
			changes[0].tu = u1;
			changes[0].tv = v1;
			changes[1].tu = u2;
			changes[1].tv = v1;
			changes[2].tu = u2;
			changes[2].tv = v2;
			changes[3].tu = u1;
			changes[3].tv = v2;
			oapiEditMeshGroup(dmsh, mshgrp, &grpEditSpec);
		}
	}
}