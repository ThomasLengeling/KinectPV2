// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PKinect.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////

PKinect::PKinect()
{
	std::cout<<"starting kinect"<<std::endl;

	init();
}

void PKinect::init()
{
	HRESULT hr;

	cout << "Starting init ..." << endl;

	frame_size = cDepthWidth * cDepthHeight * 4;

	pixels =  (uint8_t * )  malloc(frame_size  * sizeof( uint8_t ));
	depth  =  (uint16_t * ) malloc(frame_size * sizeof( uint16_t ));
	mask   =  (uint16_t * ) malloc(frame_size * sizeof( uint16_t ));

	for (int i=0; i<frame_size; i++) 
	{
		pixels[i] = 0;
		depth[i]  = 0;
		mask[i]   = 0;
	}

	std::cout << "Creating Kinect object ..." << endl;

    hr = GetDefaultKinectSensor(&kSensor);
    if (FAILED(hr))
    {
		std::cout<<"ERROR LOADING KINECT"<<std::endl;
        return;
    }

    if (kSensor)
    {
		std::cout<<"PASSING COLOR FRAME"<<std::endl;
        // Initialize the Kinect and get the color reader
        IColorFrameSource* pColorFrameSource = NULL;

        hr = kSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = kSensor->get_ColorFrameSource(&pColorFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pColorFrameSource->OpenReader(&kColorFrameReader);
        }

        SafeRelease(pColorFrameSource);
    }

    if (!kSensor || FAILED(hr))
    {
        cout<<"KINECT NOT FOUND ERROR"<<std::endl;
		return;
    }

	cout << "Done inicialization" << endl;
}

PKinect::~PKinect(void)
{
	free(pixels);
	free(depth);
	free(mask);

	delete [] pixels;
	delete [] depth;
	delete [] mask;

	stop();
}

void PKinect::stop()
{
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
if (!kColorFrameReader)
    {
		std::cout<<"ERROR READING COLOR FRAME"<<std::endl;
        return;
    }

    IColorFrame* pColorFrame = NULL;

    HRESULT hr = kColorFrameReader->AcquireLatestFrame(&pColorFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;
        IFrameDescription* pFrameDescription = NULL;
        int nWidth = 0;
        int nHeight = 0;
        ColorImageFormat imageFormat = ColorImageFormat_None;
        UINT nBufferSize = 0;
		uint8_t *pBuffer = NULL;

        hr = pColorFrame->get_RelativeTime(&nTime);

        if (SUCCEEDED(hr))
        {
            hr = pColorFrame->get_FrameDescription(&pFrameDescription);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Width(&nWidth);
        }

        if (SUCCEEDED(hr))
        {
            hr = pFrameDescription->get_Height(&nHeight);
        }

        if (SUCCEEDED(hr))
        {
            hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
        }

        if (SUCCEEDED(hr))
        {
            
			nBufferSize = cDepthWidth * cDepthHeight * sizeof( uint8_t ) * 4;
			pBuffer		= new uint8_t[ nBufferSize ];
			hr = pColorFrame->CopyConvertedFrameDataToArray( nBufferSize, reinterpret_cast<uint8_t*>( pBuffer ), ColorImageFormat_Rgba );
			
			if ( SUCCEEDED( hr ) ) {
				memcpy( pixels, pBuffer, cDepthWidth * cDepthHeight * sizeof( uint8_t ) * 4 );
			}           

        }

        SafeRelease(pFrameDescription);
    }

    SafeRelease(pColorFrame);
}
//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t * PKinect::JNI_GetImage()
{
	return pixels;
}

uint16_t * PKinect::JNI_GetDepth()
{
	return depth;
}

uint16_t * PKinect::JNI_GetMask()
{
	return mask;
}