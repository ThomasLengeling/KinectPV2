/*
* 
* Copyright (c) 2013, Thomas Sanchez Lengeling
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

#pragma once

#include <string>
#include <stdint.h>
#include <exception>
#include <comutil.h>

#include "ole2.h"
#include "Kinect.h"

#if defined( _DEBUG )
#pragma comment( lib, "comsuppwd.lib" )
#else
#pragma comment( lib, "comsuppw.lib" )
#endif
#pragma comment( lib, "wbemuuid.lib" )

static const int        cDepthWidth  = 512;
static const int        cDepthHeight = 424;

//////////////////////////////////////////////////////////////////////////////////////////////

class DeviceOptions
{
public:
	DeviceOptions();
	
	DeviceOptions&								enableAudio( bool enable = true );
	DeviceOptions&								enableBody( bool enable = true );
	DeviceOptions&								enableBodyIndex( bool enable = true );
	DeviceOptions&								enableColor( bool enable = true );
	DeviceOptions&								enableDepth( bool enable = true );
	DeviceOptions&								enableInfrared( bool enable = true );
	DeviceOptions&								enableInfraredLongExposure( bool enable = true );
	DeviceOptions&								setDeviceId( const std::string& id = "" ); 
	DeviceOptions&								setDeviceIndex( int32_t index = 0 );

	const std::string&							getDeviceId() const;
	int32_t										getDeviceIndex() const;
	bool										isAudioEnabled() const;
	bool										isBodyEnabled() const;
	bool										isBodyIndexEnabled() const;
	bool										isColorEnabled() const;
	bool										isDepthEnabled() const;
	bool										isInfraredEnabled() const;
	bool										isInfraredLongExposureEnabled() const;
protected:
	std::string									mDeviceId;
	int32_t										mDeviceIndex;

	bool										mEnabledAudio;
	bool										mEnabledBody;
	bool										mEnabledBodyIndex;
	bool										mEnabledColor;
	bool										mEnabledDepth;
	bool										mEnabledInfrared;
	bool										mEnabledInfraredLongExposure;
};

class PKinect
{
private:
	HANDLE	m_hNextImageFrameEvent;
	HANDLE	m_hNextDepthFrameEvent;
	HANDLE	m_hNextSkeletonEvent;

	HANDLE	m_pImageStreamHandle;
	HANDLE	m_pDepthStreamHandle;
	HANDLE	m_pSkeletonStreamHandle;

	HANDLE	m_hEvNuiProcessStop;
	HANDLE	m_hThNuiProcess;

	int		frame_size;
	uint8_t *	pixels;
	uint16_t *	depth;
	uint16_t *	mask;

	LONG*	col_info;
	USHORT*	m_depth;
	
protected:

	ICoordinateMapper*			kCoordinateMapper;
	IMultiSourceFrameReader*	kFrameReader;
	IKinectSensor*				kSensor;

	DeviceOptions				kDeviceOptions;

public:
	PKinect(const DeviceOptions & deviceOptions = DeviceOptions() );
	~PKinect(void);

	void stop();

	void update();

	const DeviceOptions&			getDeviceOptions() const;
	ICoordinateMapper*				getCoordinateMapper() const;

	uint8_t *						Nui_GetImage();
	uint16_t *						Nui_GetDepth();
	uint16_t *						Nui_GetMask();


	std::string wcharToString( wchar_t* v ){
			std::string str = "";
			wchar_t* id = ::SysAllocString( v );
			_bstr_t idStr( id );
			if ( idStr.length() > 0 ) {
				str = std::string( idStr );
			}
			::SysFreeString( id );
			return str;
	}

	public:

	//////////////////////////////////////////////////////////////////////////////////////////////

	class Exception : public std::exception
	{
	public:
		const char* what() const throw();
	protected:
		char									mMessage[ 2048 ];
		friend class							Device;
	};
	
	class ExcDeviceEnumerationFailed : public Exception 
	{
	public:
		ExcDeviceEnumerationFailed( long hr ) throw();
	};

	class ExcDeviceInitFailed : public Exception 
	{
	public:
		ExcDeviceInitFailed( long hr, const std::string& id ) throw();
	};
	
	class ExcDeviceNotAvailable : public Exception 
	{
	public:
		ExcDeviceNotAvailable( long hr ) throw();
	};

	class ExcDeviceOpenFailed : public Exception 
	{
	public:
		ExcDeviceOpenFailed( long hr, const std::string& id ) throw();
	};

	class ExcGetCoordinateMapperFailed : public Exception 
	{
	public:
		ExcGetCoordinateMapperFailed( long hr, const std::string& id ) throw();
	};

	class ExcOpenFrameReaderFailed : public Exception 
	{
	public:
		ExcOpenFrameReaderFailed( long hr, const std::string& id ) throw();
	};
};

