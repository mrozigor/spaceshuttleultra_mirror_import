/****************************************************************************
  This file is part of Space Shuttle Ultra

  Input Electronics definition



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
#ifndef _mps_IE_H_
#define _mps_IE_H_


#include <orbitersdk.h>


namespace mps
{
	class SSME;

	class InputElectronics
	{
		friend class SSMEController;

		protected:
			int ch;
			SSME* eng;
			SSMEController* Controller;

			// sensor input
			double* press;
			double* temp;
			double* speed;
			double* flow;
			double* actpos;
			double* samplehold;

			// sensor count
			int numP;
			int numT;
			int numS;
			int numF;

			// flight data sensor count
			int FD_P;
			int FD_T;
			int FD_F;
			int FD_S;

		public:
			InputElectronics( int ch, SSME* eng, SSMEController* Controller, int numP, int numT, int numS, int numF, int FD_P, int FD_T, int FD_S, int FD_F );
			virtual ~InputElectronics( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			virtual void tmestp( double time ) = 0;

			void CopySensorData( int count, int startsource, int startdest, double* source, double* dest );

			void GetData( unsigned short device, unsigned short* data );
	};
}


#endif// _mps_IE_H_
