#pragma once

#include "BasicLight.h"
#include <DiscInPort.h>

namespace vc {
	using discsignals::DiscInPort;

	class StandardLight2 : public BasicLight 
	{
		float u1, v1;
		float u2, v2;
		//Activated Texture coordinates
		float u1a, v1a;
		float u2a, v2a;

		bool currentState;

		WORD vtx[4];
		UINT mshgrp;
		UINT midx;
		void setTexture();
		void modifyTexture(float u1, float v1, float u2, float v2);
	public:
		StandardLight2(Atlantis* _sts, const string& _ident, UINT midx, UINT mshgrp);
		virtual ~StandardLight2();

		void SetVertexIndexes(UINT topleft, UINT topright, UINT bottomright, UINT bottomleft);
		void SetInactiveTextureCoordinates(float u1, float v1, float u2, float v2);
		void SetActiveTextureCoordinates(float u1, float v1, float u2, float v2);
		void SetActive(bool b);

		virtual bool Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);

		DiscInPort input;
	};
};