#pragma once
#include "BasicVCComponent.h"
#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscreteBundle.h"

namespace vc {

	using class discsignals::DiscInPort;

	class BasicLight:public BasicVCComponent {
		DiscInPort input;
	public:
		BasicLight(Atlantis* _sts, const string& _ident);
		virtual ~BasicLight();
		virtual bool Connect(unsigned short usPort, ::discsignals::DiscreteBundle* pBundle, unsigned short usLine);
	};
};