 /****************************************************************************
  This file is part of Space Shuttle Ultra

  SSU mathematical functions



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#pragma once

#include <OrbiterAPI.h>
#include <cmath>
#include <vector>
#include <EngConst.h>

const MATRIX3 IdentityMatrix = _M(1, 0, 0,
								  0, 1, 0,
								  0, 0, 1);
//Attitude ref. frame conversion
const double AXIS_TILT = 23.4458878*RAD;
//tilt of Earth's axis (radians)

/**
 * Converts current elements to state vector at a future time
 * Wrapper for kost functions; all input and output is in Orbiter frames/data structures
 */
//void PropagateStateVector(OBJHANDLE hPlanet, double time, const ELEMENTS& elements, VECTOR3& pos, VECTOR3& vel, bool nonsphericalGravity, double vesselMass=0.0);

//Math
/**
 * Calculates rotation axis and angle of rotation (in radians) corresponding to rotation matrix.
 */
double CalcEulerAngle(const MATRIX3 &RotMatrix, VECTOR3 &Axis);
/**
 * Returns vector v rotated by given angle about Axis
 */
VECTOR3 RotateVector(const VECTOR3 &Axis, double radAngle, const VECTOR3 &v);
/**
 * Rotates vector around specified Euler angles in XYZ order
 */
void RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
/**
 * Rotates vector around specified Euler angles in XYZ order in left-handed frame
 */
void RotateVectorLH(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
/**
 * Returns Euler angles (in radians); Pitch=X axis, Yaw=Y axis, Roll=Z axis
 */
VECTOR3 GetZYX_RYPAnglesFromMatrix(const MATRIX3 &RotMatrix); //returns angles in radians
/**
 * Returns Euler angles (in radians); Pitch=Y axis, Yaw=Z axis, Roll=X axis
 */
VECTOR3 GetYZX_PYRAnglesFromMatrix(const MATRIX3 &RotMatrix); 
/**
 * Returns Euler angles (in radians)
 * Value for each axis corresponds to angle around that axis
 */
VECTOR3 GetZYXAnglesFromMatrixLH(const MATRIX3 &RotMatrix);
/**
 * Returns rotation matrix corresponding to given Euler angles
 * Rotation is performed in YZX order - this corresponds to PYR order for shuttle body axis & LVLH frames
 * \param Angles Angle of rotation about each axis
 */
MATRIX3 GetRotationMatrixYZX(const VECTOR3& Angles);
/**
 * Calculates rotation matrix corresponding to rotation around X axis by specified angle (radians)
 */
void GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX);
/**
 * Calculates rotation matrix corresponding to rotation around Y axis by specified angle (radians)
 */
void GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY);
/**
 * Calculates rotation matrix corresponding to rotation around Z axis by specified angle (radians)
 */
void GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ);
/**
 * Returns rotation matrix with x axis in specified direction, and z axis rotated around x axis by specified angle relative to reference frame.
 * \param x_axis Normalised vector in direction of x axis
 * \param angle Angle (in radians) between z axis and z axis in reference frame
 */
MATRIX3 GetRotationMatrix(const VECTOR3& x_axis, double angle);

/**
 * Returns angle (in degress) through which body will rotate before angular velocity is killed.
 * \param radRate rotation rate (rad/s)
 * \param Mass mass of body
 * \param Moment moment of inertia about rotation axis
 * \param Torque torque applied to null rotation
 */
double NullStartAngle(double radRate, double Mass, double Moment, double Torque);
/**
 * Returns change in rotation rate (DEG/s) over given time period.
 */
double RotationRateChange(double Mass, double Moment, double Torque, double DeltaT);

/**
 * Converts vessel rotation matrix (returned by VESSEL::GetRotationMatrix) to M50 frame.
 * Also transforms body axis frame to shuttle body axis frame (i.e. +X pointing forward, +Z pointing down).
 * \returns Rotation matrix to convert between body axis and M50 frame
 */
MATRIX3 ConvertOrbitersimRotationMatrixToM50(const MATRIX3 &RotMatrix);
//MATRIX3 ConvertOrbitersimAnglesToM50Matrix(const VECTOR3 &radAngles);
//MATRIX3 ConvertLVLHAnglesToM50Matrix(const VECTOR3 &radAngles, const VECTOR3 &pos, const VECTOR3 &vel);
/**
* Converts Pitch, Yaw and Omicron angles (entered in UNIV PTG display) to angles in shuttle body frame.
*/
//VECTOR3 ConvertPYOMToBodyAngles(double radP, double radY, double radOM);
/**
 * Converts Pitch, Yaw and Omicron angles (entered in UNIV PTG display) to LVLH rotation matrix
 * \returns LVLH rotation matrix (right-handed)
 */
MATRIX3 ConvertPYOMToLVLH(double radP, double radY, double radOM);

/**
 * Returns matrix to convert direction vector in global frame to LVLH frame
 * LVLH Frame:
 *		X: from target in +ve velocity vector Z: from target towards center of Earth
 * \param pos position of vessel in global frame
 * \param vel position of vessel in inertial frame
 * \param changeHandedness true to change handedness (i.e. LH global frame to RH LVLH frame)
 * \returns rotation matrix
 */
MATRIX3 GetGlobalToLVLHMatrix(const VECTOR3& pos, const VECTOR3& vel, bool changeHandedness = false);

//VECTOR3 GetPositionVector(OBJHANDLE hPlanet, double lat, double lng, double rad);
//void ConvertEquToEcl(OBJHANDLE hPlanet, const VECTOR3& equPos, const VECTOR3& equVel, VECTOR3& eclPos, VECTOR3& eclVel);
//interpolation
/**
 * Returns index of largest element in (sorted) array which is smaller than target.
 * Maximum index returned will be size()-2 (this makes interpolation easier).
 */
unsigned int GetLowerIndex(const std::vector<double> &list, double target);
unsigned int GetLowerIndex(const double* list, unsigned int size, double target);
int tpir(const double* list, int n_items, double target);
double linterp(double x0, double y0, double x1, double y1, double x);
double listerp(const std::vector<double> &listx, const std::vector<double> &listy, double x);
double listerp(const double* listx, const double* listy, int n_items, double x);
double tableterp(const double* table, const double* listrow, int n_row, const double* listcol, int n_col, double rowlookup, double collookup);
	
static inline VECTOR3 Normalize(const VECTOR3 &a)
{
	return (a/length(a));
}
	
	
static inline VECTOR3 Projection(const VECTOR3 &u, const VECTOR3 &v) //Proj u onto v
{
	return v*(dotp(u, v)/dotp(v, v));
}

/*static inline VECTOR3 ToRad(const VECTOR3 &Input)
{
		VECTOR3 Output;
		for(int i=0;i<3;i++) {
			Output.data[i]=Input.data[i]*RAD;
		}
		return Output;
	}
	
static inline VECTOR3 ToDeg(const VECTOR3 &Input)
{
	VECTOR3 Output;
	for(int i=0;i<3;i++) {
		Output.data[i]=Input.data[i]*DEG;
	}
	return Output;
}*/

/**
 * Converts between Orbiter left-handed frame and right-handed frame.
 * Swaps y and z values.
 * Used mainly when calling KOST functions
 */
static inline VECTOR3 ConvertBetweenLHAndRHFrames(const VECTOR3 &v)
{
	return _V(v.x, v.z, v.y);
}
	
/**
 * Returns vector normalized by adjusting z-component only - x & y values are not changed
 */
static inline VECTOR3 NormZ(VECTOR3 &v)
{
	// error checking
	/*if((pow(v.x,2)-pow(v.y,2))>0.1) {
		sprintf_s(oapiDebugString(), 255, "NormZ ERROR: Vector %f %f %f passed at T: %f",
			v.x, v.y, v.z, met);
		oapiWriteLog(oapiDebugString());
		return _V(0, 0, 1);
	}*/
	VECTOR3 o=v;
	o.z=sqrt(1.0-pow(v.x,2)-pow(v.y,2));
	return o;
}

static inline MATRIX3 Transpose(const MATRIX3& m)
{
	return _M(m.m11, m.m21, m.m31,
			  m.m12, m.m22, m.m32,
			  m.m13, m.m23, m.m33);
}

/**
 * Returns rotation matrix representing angle from current to target matrix
 */
static inline MATRIX3 GetRotationErrorMatrix(const MATRIX3& cur, const MATRIX3& tgt)
{
	return mul(Transpose(cur), tgt);
}

static inline bool Eq(const double d1, const double d2, double dDiff=0.00001)
{
	if(fabs(d1-d2)>dDiff) return false;
	return true;
}

static inline bool Eq(const VECTOR3 v1, const VECTOR3 v2, double dDiff=0.00001)
{
	for(int i=0;i<3;i++) {
		if(fabs(v1.data[i]-v2.data[i])>dDiff) return false;
	}
	return true;
}

static inline double range(double min, double value, double max)
{
	if(value<min) return min;
	if(value>max) return max;
	return value;
}

/**
 * Converts time in DD/HH:MM:SS format (array of 4 doubles) to time in seconds
 */
inline double ConvertDDHHMMSSToSeconds(const double ddhhmmss[])
{
	return ddhhmmss[0]*86400+ddhhmmss[1]*3600+ddhhmmss[2]*60+ddhhmmss[3];
}

/**
 * Converts time in seconds to time in DD/HH:MM:SS format (array of 4 doubles)
 */
inline void ConvertSecondsToDDHHMMSS(double seconds, double ddhhmmss[])
{
	ddhhmmss[0] = static_cast<int>(seconds/86400);
	seconds -= ddhhmmss[0]*86400;
	ddhhmmss[1] = static_cast<int>(seconds/3600);
	seconds -= ddhhmmss[1]*3600;
	ddhhmmss[2] = static_cast<int>(seconds/60);
	seconds -= ddhhmmss[2]*60;
	ddhhmmss[3] = seconds;
}

inline int round(double value)
{
	return static_cast<int>(floor(value+0.5));
}

static inline double sign(double x)
{
	if(x>=0.0) return 1.0;
	else return -1.0;
}

static inline VECTOR3 RotateVectorX(const VECTOR3 &v, double angle) //rotates about angle (in degrees) in X-axis
{
	VECTOR3 Output;
	Output.x=v.x;
	Output.z=v.z*cos(angle*RAD)-v.y*sin(angle*RAD);
	Output.y=v.y*cos(angle*RAD)+v.z*sin(angle*RAD);
	return Output;
}
static inline VECTOR3 RotateVectorY(const VECTOR3 &v, double angle) //rotates about angle (in degrees) in Y-axis
{
	VECTOR3 Output;
	Output.y=v.y;
	Output.x=v.x*cos(angle*RAD)-v.z*sin(angle*RAD);
	Output.z=v.z*cos(angle*RAD)+v.x*sin(angle*RAD);
	return Output;
}
static inline VECTOR3 RotateVectorZ(const VECTOR3 &v, double angle) //rotates about angle (in degrees) in Z-axis
{
	VECTOR3 Output;
	Output.x=v.x*cos(angle*RAD)-v.y*sin(angle*RAD);
	Output.y=v.x*sin(angle*RAD)+v.y*cos(angle*RAD);
	Output.z=v.z;
	return Output;
}

