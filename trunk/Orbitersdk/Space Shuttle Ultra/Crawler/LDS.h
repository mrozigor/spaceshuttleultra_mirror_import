#include "orbitersdk.h"

#define OFF 0
#define ON 1

class LDS   //ONLY CALCULATIONS AND METHODS HERE
{
public:
	LDS();
	~LDS();

	void ShowAlignmentError(double lateral);
	void CalculateDistanceBetweenAttachments(VECTOR3 MLP_ATTACH_GLOBAL, MATRIX3 RotationMatrix);
};