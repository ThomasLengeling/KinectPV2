// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PKinect.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
DeviceOptions::DeviceOptions()
: mDeviceIndex( 0 ), mDeviceId( "" ), mEnabledAudio( false ), mEnabledBody( false ), 
mEnabledBodyIndex( false ), mEnabledColor( true ), mEnabledDepth( true ), 
mEnabledInfrared( false ), mEnabledInfraredLongExposure( false )
{
}

DeviceOptions& DeviceOptions::enableAudio( bool enable )
{
	mEnabledAudio = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableBody( bool enable )
{
	mEnabledBody = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableBodyIndex( bool enable )
{
	mEnabledBodyIndex = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableColor( bool enable )
{
	mEnabledColor = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableDepth( bool enable )
{
	mEnabledDepth = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableInfrared( bool enable )
{
	mEnabledInfrared = enable;
	return *this;
}

DeviceOptions& DeviceOptions::enableInfraredLongExposure( bool enable )
{
	mEnabledInfraredLongExposure = enable;
	return *this;
}

DeviceOptions& DeviceOptions::setDeviceId( const string& id )
{
	mDeviceId = id;
	return *this;
}

DeviceOptions& DeviceOptions::setDeviceIndex( int32_t index )
{
	mDeviceIndex = index;
	return *this;
}

const string& DeviceOptions::getDeviceId() const
{
	return mDeviceId;
}

int32_t	 DeviceOptions::getDeviceIndex() const
{
	return mDeviceIndex;
}

bool DeviceOptions::isAudioEnabled() const
{
	return mEnabledAudio;
}

bool DeviceOptions::isBodyEnabled() const
{
	return mEnabledBody;
}

bool DeviceOptions::isBodyIndexEnabled() const
{
	return mEnabledBodyIndex;
}

bool DeviceOptions::isColorEnabled() const
{
	return mEnabledColor;
}

bool DeviceOptions::isDepthEnabled() const
{
	return mEnabledDepth;
}

bool DeviceOptions::isInfraredEnabled() const
{
	return mEnabledInfrared;
}

bool DeviceOptions::isInfraredLongExposureEnabled() const
{
	return mEnabledInfraredLongExposure;
}

PKinect::PKinect(const DeviceOptions & deviceOptions )
{
	frame_size = cDepthWidth * cDepthHeight;
	pixels =  (uint8_t * ) malloc(frame_size * sizeof( uint8_t ) * 4);
	depth  =  (uint16_t * ) malloc(frame_size * sizeof( uint16_t ) * 4);
	mask   =  (uint16_t * ) malloc(frame_size * sizeof( uint16_t ) * 4);

	col_info = new LONG[frame_size*2];
	m_depth = new USHORT[frame_size];

	for (int i=0; i<frame_size; i++) 
	{
		pixels[i] = 0;
		depth[i] = 0;
		mask[i] = 0;
	}

	cout << "Creating Kinect object ..." << endl;

	long hr = S_OK;
	kDeviceOptions = deviceOptions;
	
	IKinectSensorCollection* sensorCollection = 0;
	hr = GetKinectSensorCollection( &sensorCollection );
	if ( FAILED( hr ) || sensorCollection == 0 ) {
		throw ExcDeviceNotAvailable( hr );
	}

	//sensorCollection->SubscribeCollectionChanged( &Device::onSensorCollectionChanged );
	IEnumKinectSensor* sensorEnum = 0;
	hr = sensorCollection->get_Enumerator( &sensorEnum );
	if ( FAILED( hr ) || sensorEnum == 0 ) {
		throw ExcDeviceEnumerationFailed( hr );
	}

	hr			= S_OK;
	int32_t i	= 0;

	while ( SUCCEEDED( hr ) && i < 8 ) { // TODO find actual max device count
		hr = sensorEnum->GetNext( &kSensor );
		if ( kSensor != 0 ) {
			string id = "";
			wchar_t wid[ 48 ];
			if ( SUCCEEDED( kSensor->get_UniqueKinectId( 48, wid ) ) ) {
				id = wcharToString( wid );
			}
			if ( kDeviceOptions.getDeviceId().empty() ) {
				if ( kDeviceOptions.getDeviceIndex() == i ) {
					kDeviceOptions.setDeviceId( id );
					break;
				}
			} else {
				if ( kDeviceOptions.getDeviceId() == id ) {
					kDeviceOptions.setDeviceIndex( i );
					break;
				}
			}
		}
		++i;
	}

	if ( kSensor == 0 ) {
		throw ExcDeviceInitFailed( hr, kDeviceOptions.getDeviceId() );
	} else {
		hr = kSensor->Open();
		if ( SUCCEEDED( hr ) ) {
			hr = kSensor->get_CoordinateMapper( &kCoordinateMapper );
			if ( SUCCEEDED( hr ) ) {
				long flags = 0L;
				if ( kDeviceOptions.isAudioEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_Audio;
				}
				if ( kDeviceOptions.isBodyEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_Body;
				}
				if ( kDeviceOptions.isBodyIndexEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_BodyIndex;
				}
				if ( kDeviceOptions.isColorEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_Color;
				}
				if ( kDeviceOptions.isDepthEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_Depth;
				}
				if ( kDeviceOptions.isInfraredEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_Infrared;
				}
				if ( kDeviceOptions.isInfraredLongExposureEnabled() ) {
					flags |= FrameSourceTypes::FrameSourceTypes_LongExposureInfrared;
				}
				hr = kSensor->OpenMultiSourceFrameReader( flags, &kFrameReader );
				if ( FAILED( hr ) ) {
					if ( kFrameReader != 0 ) {
						kFrameReader->Release();
						kFrameReader = 0;
					}
					throw ExcOpenFrameReaderFailed( hr, kDeviceOptions.getDeviceId() );
				}
			} else {
				throw ExcDeviceOpenFailed( hr, kDeviceOptions.getDeviceId() );
			}
		} else {
			throw ExcGetCoordinateMapperFailed( hr, kDeviceOptions.getDeviceId() );
		}
	}
}

PKinect::~PKinect(void)
{
	free(pixels);
	free(depth);
	free(mask);
	delete [] pixels;
	delete [] depth;
	delete [] mask;
	//delete [] col_info;
	//delete [] m_depth;

	stop();
}

void PKinect::stop()
{
	if ( kCoordinateMapper != 0 ) {
		kCoordinateMapper->Release();
		kCoordinateMapper = 0;
	}
	if ( kFrameReader != 0 ) {
		kFrameReader->Release();
		kFrameReader = 0;
	}
	if ( kSensor != 0 ) {
		long hr = kSensor->Close();
		if ( SUCCEEDED( hr ) && kSensor != 0 ) {
			kSensor->Release();
			kSensor = 0;
		}
	}
}

void PKinect::update()
{

	if ( kFrameReader == 0 ) {
		return;
	}

	IAudioBeamFrame* audioFrame								= 0;
	IBodyFrame* bodyFrame									= 0;
	IBodyIndexFrame* bodyIndexFrame							= 0;
	IColorFrame* colorFrame									= 0;
	IDepthFrame* depthFrame									= 0;
	IMultiSourceFrame* frame								= 0;
	IInfraredFrame* infraredFrame							= 0;
	ILongExposureInfraredFrame* infraredLongExposureFrame	= 0;
	
	HRESULT hr = kFrameReader->AcquireLatestFrame( &frame );
	
	if ( SUCCEEDED( hr ) && kDeviceOptions.isAudioEnabled() ) {
		// TODO audio
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isBodyEnabled() ) {
		IBodyFrameReference* frameRef = 0;
		hr = frame->get_BodyFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &bodyFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isBodyIndexEnabled() ) {
		IBodyIndexFrameReference* frameRef = 0;
		hr = frame->get_BodyIndexFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &bodyIndexFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isColorEnabled() ) {
		IColorFrameReference* frameRef = 0;
		hr = frame->get_ColorFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &colorFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isDepthEnabled() ) {
		IDepthFrameReference* frameRef = 0;
		hr = frame->get_DepthFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &depthFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isInfraredEnabled() ) {
		IInfraredFrameReference* frameRef = 0;
		hr = frame->get_InfraredFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &infraredFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) && kDeviceOptions.isInfraredLongExposureEnabled() ) {
		ILongExposureInfraredFrameReference* frameRef = 0;
		hr = frame->get_LongExposureInfraredFrameReference( &frameRef );
		if ( SUCCEEDED( hr ) ) {
			hr = frameRef->AcquireFrame( &infraredLongExposureFrame );
		}
		if ( frameRef != 0 ) {
			frameRef->Release();
			frameRef = 0;
		}
	}

	if ( SUCCEEDED( hr ) ) {
		long long timeStamp										= 0L;

		// TODO audio
		
		int* colorSurface;
		IFrameDescription* colorFrameDescription				= 0;
		int32_t colorWidth										= 0;
		int32_t colorHeight										= 0;
		ColorImageFormat colorImageFormat						= ColorImageFormat_None;
		uint32_t colorBufferSize								= 0;
		uint8_t* colorBuffer									= 0;

		int* depthChannel;
		IFrameDescription* depthFrameDescription				= 0;
		int32_t depthWidth										= 0;
		int32_t depthHeight										= 0;
		uint16_t depthMinReliableDistance						= 0;
		uint16_t depthMaxReliableDistance						= 0;
		uint32_t depthBufferSize								= 0;
		uint16_t* depthBuffer									= 0;

		int* infraredChannel;
		IFrameDescription* infraredFrameDescription				= 0;
		int32_t infraredWidth									= 0;
		int32_t infraredHeight									= 0;
		uint32_t infraredBufferSize								= 0;
		uint16_t* infraredBuffer								= 0;



		hr = depthFrame->get_RelativeTime( &timeStamp );

		// TODO audio
		if ( kDeviceOptions.isAudioEnabled() ) {

		}

		if ( kDeviceOptions.isColorEnabled() ) {
			if ( SUCCEEDED( hr ) ) {
				hr = colorFrame->get_FrameDescription( &colorFrameDescription );
				if ( SUCCEEDED( hr ) ) {
					float vFov = 0.0f;
					float hFov = 0.0f;
					float dFov = 0.0f;
					colorFrameDescription->get_VerticalFieldOfView( &vFov );
					colorFrameDescription->get_HorizontalFieldOfView( &hFov );
					colorFrameDescription->get_DiagonalFieldOfView( &dFov );
				}
			}
			if ( SUCCEEDED( hr ) ) {
				hr = colorFrameDescription->get_Width( &colorWidth );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = colorFrameDescription->get_Height( &colorHeight );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = colorFrame->get_RawColorImageFormat( &colorImageFormat );
			}
			if ( SUCCEEDED( hr ) ) {
				colorBufferSize = colorWidth * colorHeight * sizeof( uint8_t ) * 4;
				colorBuffer		= new uint8_t[ colorBufferSize ];
				hr = colorFrame->CopyConvertedFrameDataToArray( colorBufferSize, reinterpret_cast<uint8_t*>( colorBuffer ), ColorImageFormat_Rgba );
			
				if ( SUCCEEDED( hr ) ) {
					memcpy( pixels, colorBuffer, colorWidth * colorHeight * sizeof( uint8_t ) );
				}

				delete [] colorBuffer;
				colorBuffer = 0;
			}
		}

		if ( kDeviceOptions.isDepthEnabled() ) {
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrame->get_FrameDescription( &depthFrameDescription );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrameDescription->get_Width( &depthWidth );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrameDescription->get_Height( &depthHeight );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrame->get_DepthMinReliableDistance( &depthMinReliableDistance );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrame->get_DepthMaxReliableDistance( &depthMaxReliableDistance );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = depthFrame->AccessUnderlyingBuffer( &depthBufferSize, &depthBuffer );
			}
			if ( SUCCEEDED( hr ) ) {
				memcpy( depth, depthBuffer, depthWidth * depthHeight * sizeof( uint16_t ) );
			}
		}

		if ( kDeviceOptions.isInfraredEnabled() ) {
			if ( SUCCEEDED( hr ) ) {
				hr = infraredFrame->get_FrameDescription( &infraredFrameDescription );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = infraredFrameDescription->get_Width( &infraredWidth );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = infraredFrameDescription->get_Height( &infraredHeight );
			}
			if ( SUCCEEDED( hr ) ) {
				hr = infraredFrame->AccessUnderlyingBuffer( &infraredBufferSize, &infraredBuffer );
			}
			if ( SUCCEEDED( hr ) ) {
				memcpy( mask, infraredBuffer,  infraredWidth * infraredHeight * sizeof( uint16_t ) );
			}
		}


		if ( SUCCEEDED( hr ) ) {
			std::string mDeviceId = kDeviceOptions.getDeviceId();
			std::cout<<mDeviceId<<"Device ID"<<std::endl;
		}

		if ( colorFrameDescription != 0 ) {
			colorFrameDescription->Release();
			colorFrameDescription = 0;
		}
		if ( depthFrameDescription != 0 ) {
			depthFrameDescription->Release();
			depthFrameDescription = 0;
		}
		if ( infraredFrameDescription != 0 ) {
			infraredFrameDescription->Release();
			infraredFrameDescription = 0;
		}

	}

	if ( audioFrame != 0 ) {
		audioFrame->Release();
		audioFrame = 0;
	}
	if ( bodyFrame != 0 ) {
		bodyFrame->Release();
		bodyFrame = 0;
	}
	if ( bodyIndexFrame != 0 ) {
		bodyIndexFrame->Release();
		bodyIndexFrame = 0;
	}
	if ( colorFrame != 0 ) {
		colorFrame->Release();
		colorFrame = 0;
	}
	if ( depthFrame != 0 ) {
		depthFrame->Release();
		depthFrame = 0;
	}
	if ( frame != 0 ) {
		frame->Release();
		frame = 0;
	}
	if ( infraredFrame != 0 ) {
		infraredFrame->Release();
		infraredFrame = 0;
	}
	if ( infraredLongExposureFrame != 0 ) {
		infraredLongExposureFrame->Release();
		infraredLongExposureFrame = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

const char* PKinect::Exception::what() const throw()
{
	return mMessage;
}

PKinect::ExcDeviceEnumerationFailed::ExcDeviceEnumerationFailed( long hr ) throw()
{
	sprintf_s( mMessage, "Unable to enumerate devices. Error: %i", hr );
}

PKinect::ExcDeviceInitFailed::ExcDeviceInitFailed( long hr, const string& id ) throw()
{
	sprintf_s( mMessage, "Device initialization failed. Device ID: %s. Error: %i", id, hr );
}

PKinect::ExcDeviceNotAvailable::ExcDeviceNotAvailable( long hr ) throw()
{
	sprintf_s( mMessage, "No devices are available. Error: %i", hr );
}

PKinect::ExcDeviceOpenFailed::ExcDeviceOpenFailed( long hr, const string& id ) throw()
{
	sprintf_s( mMessage, "Unable to open device. Device ID: %s. Error: %i", id, hr );
}

PKinect::ExcGetCoordinateMapperFailed::ExcGetCoordinateMapperFailed( long hr, const string& id ) throw()
{
	sprintf_s( mMessage, "Unable to get device coordinate mapper. Device ID: %s. Error: %i", id, hr );
}

PKinect::ExcOpenFrameReaderFailed::ExcOpenFrameReaderFailed( long hr, const string& id ) throw()
{
	sprintf_s( mMessage, "Unable to open frame reader. Device ID: %s. Error: %i", id, hr );
}


uint8_t * PKinect::Nui_GetImage()
{
	return pixels;
}

uint16_t * PKinect::Nui_GetDepth()
{
	return depth;
}

uint16_t * PKinect::Nui_GetMask()
{
	return mask;
}