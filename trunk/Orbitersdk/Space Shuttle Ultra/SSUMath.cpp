#include "SSUMath.h"

VECTOR3 GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };

	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(crossp(Pitch, YawRoll));

	Res.data[YAW] = -asin(YawRoll.z);

	Res.data[ROLL] = atan2(YawRoll.y, YawRoll.x);

	Res.data[PITCH] = atan2(H.z, Pitch.z);

	return Res;

}

VECTOR3 GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };
	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(crossp(Pitch, YawRoll));
	Res.data[YAW] = -asin(Pitch.x);
	Res.data[ROLL] = atan2(H.x, YawRoll.x);
	Res.data[PITCH] = atan2(Pitch.y, Pitch.z);
	return Res;
}

double CalcEulerAngle(const MATRIX3 &RefAngles, const MATRIX3 &TargetAngles, VECTOR3 &Axis)
{
	double Angle;
	MATRIX3 RotMatrix;
	RotMatrix=mul(RefAngles, TargetAngles);
	double Trace=RotMatrix.m11+RotMatrix.m22+RotMatrix.m33;
	Angle=acos(0.5*(Trace-1));
	Axis.x=(RotMatrix.m23-RotMatrix.m32)/(2*sin(Angle));
	Axis.y=(RotMatrix.m31-RotMatrix.m13)/(2*sin(Angle));
	Axis.z=(RotMatrix.m12-RotMatrix.m21)/(2*sin(Angle));
	return Angle;
}

double CalcEulerAngle(const VECTOR3 &RefAngles, const VECTOR3 &TargetAngles, VECTOR3 &Axis)
{
	double Angle;
	MATRIX3 RotMatrix, Ref, Target;
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ;
	GetRotMatrixX(RefAngles.x, RotMatrixX);
	GetRotMatrixY(RefAngles.y, RotMatrixY);
	GetRotMatrixZ(RefAngles.z, RotMatrixZ);
	Ref=mul(mul(RotMatrixX, RotMatrixY), RotMatrixZ);
	GetRotMatrixX(TargetAngles.x, RotMatrixX);
	GetRotMatrixY(TargetAngles.y, RotMatrixY);
	GetRotMatrixZ(TargetAngles.z, RotMatrixZ);
	Target=mul(mul(RotMatrixX, RotMatrixY), RotMatrixZ);
	RotMatrix=mul(Ref, Target);
	double Trace=RotMatrix.m11+RotMatrix.m22+RotMatrix.m33;
	Angle=acos(0.5*(Trace-1));
	Axis.x=(RotMatrix.m23-RotMatrix.m32)/(2*sin(Angle));
	Axis.y=(RotMatrix.m31-RotMatrix.m13)/(2*sin(Angle));
	Axis.z=(RotMatrix.m12-RotMatrix.m21)/(2*sin(Angle));
	return Angle;
}

void RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result)
{
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ;
	VECTOR3 AfterZ, AfterZY;					// Temporary variables


	GetRotMatrixX(Angles.x, RotMatrixX);
	GetRotMatrixY(Angles.y, RotMatrixY);
	GetRotMatrixZ(Angles.z, RotMatrixZ);
	
	/*MultiplyByMatrix(Initial, RotMatrixZ, AfterZ);
	MultiplyByMatrix(AfterZ, RotMatrixY, AfterZY);
	MultiplyByMatrix(AfterZY, RotMatrixX, Result);*/
	AfterZ=mul(RotMatrixZ, Initial);
	AfterZY=mul(RotMatrixY, AfterZ);
	Result=mul(RotMatrixX, AfterZY);
}

void RotateVectorPYR(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result)
{
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ;
	VECTOR3 AfterP, AfterPY;					// Temporary variables


	GetRotMatrixX(Angles.x, RotMatrixX);
	GetRotMatrixY(Angles.y, RotMatrixY);
	GetRotMatrixZ(Angles.z, RotMatrixZ);
	
	/*MultiplyByMatrix(Initial, RotMatrixZ, AfterZ);
	MultiplyByMatrix(AfterZ, RotMatrixY, AfterZY);
	MultiplyByMatrix(AfterZY, RotMatrixX, Result);*/
	AfterP=mul(RotMatrixX, Initial);
	AfterPY=mul(RotMatrixY, AfterP);
	Result=mul(RotMatrixZ, AfterPY);
	/*AfterP=mul(RotMatrixZ, Initial);
	AfterPY=mul(RotMatrixY, AfterP);
	Result=mul(RotMatrixX, AfterPY);*/
	/*RotMatrix=mul(RotMatrixX, RotMatrixY);
	RotMatrix=mul(RotMatrix, RotMatrixZ);
	Result=mul(RotMatrix, Initial);*/
	//Result=AfterPY;
}

VECTOR3 GetAnglesFromMatrix(MATRIX3 RotMatrix)
{
	VECTOR3 Angles;
	Angles.data[PITCH]=atan2(RotMatrix.m23, RotMatrix.m33);
	Angles.data[YAW]=-asin(RotMatrix.m13);
	Angles.data[ROLL]=atan2(RotMatrix.m12, RotMatrix.m11);
	return Angles;
}

// Returns the rotation matrix for a rotation of a given angle around the X axis (Pitch)
void GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX)
{
	RotMatrixX.m11 = 1;
	RotMatrixX.m12 = 0;
	RotMatrixX.m13 = 0;
	RotMatrixX.m21 = 0;
	RotMatrixX.m22 = cos(Angle);
	RotMatrixX.m23 = sin(Angle);
	RotMatrixX.m31 = 0;
	RotMatrixX.m32 = -sin(Angle);
	RotMatrixX.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Y axis (Yaw)
void GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY)
{
	RotMatrixY.m11 = cos(Angle);
	RotMatrixY.m12 = 0;
	RotMatrixY.m13 = -sin(Angle);
	RotMatrixY.m21 = 0;
	RotMatrixY.m22 = 1;
	RotMatrixY.m23 = 0;
	RotMatrixY.m31 = sin(Angle);
	RotMatrixY.m32 = 0;
	RotMatrixY.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Z axis (Roll)
void GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ)
{
	RotMatrixZ.m11 = cos(Angle);
	RotMatrixZ.m12 = sin(Angle);
	RotMatrixZ.m13 = 0;
	RotMatrixZ.m21 = -sin(Angle);
	RotMatrixZ.m22 = cos(Angle);
	RotMatrixZ.m23 = 0;
	RotMatrixZ.m31 = 0;
	RotMatrixZ.m32 = 0;
	RotMatrixZ.m33 = 1;
}

void MultiplyByMatrix(const VECTOR3 &Initial, const MATRIX3 &RotMatrix, VECTOR3 &Result)
{

	Result.x =	(Initial.x * RotMatrix.m11) 
				+ (Initial.y * RotMatrix.m12) 
				+ (Initial.z * RotMatrix.m13);	
	Result.y =	(Initial.x * RotMatrix.m21) 
				+ (Initial.y * RotMatrix.m22) 
				+ (Initial.z * RotMatrix.m23);	
	Result.z =	(Initial.x * RotMatrix.m31) 
				+ (Initial.y * RotMatrix.m32) 
				+ (Initial.z * RotMatrix.m33);
}