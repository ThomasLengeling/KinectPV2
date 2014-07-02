// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PKinect.h"
#include <iostream>

using namespace std;

PKinect::PKinect(void)
{
	frame_size = WIDTH*HEIGHT;
	pixels = (int *) malloc(frame_size*4);
	depth = (int *) malloc(frame_size*4);
	mask = (int *) malloc(frame_size*4);

	col_info = new LONG[frame_size*2];
	m_depth = new USHORT[frame_size];

	for (int i=0; i<frame_size; i++) 
	{
		pixels[i] = 0;
		depth[i] = 0;
		mask[i] = 0;
	}

	for (int i=0; i<NUI_SKELETON_COUNT; i++) 
	{
		for (int j=0; j<NUI_SKELETON_POSITION_COUNT+1; j++)
		{
			for (int k=0; k<5; k++) 
			{
				joints[i][j][k] = 0.0f;
			}
		}
	}
	Nui_Init();
	cout << "Creating Kinect object ..." << endl;
}

PKinect::~PKinect(void)
{
	free(pixels);
	free(depth);
	free(mask);
	delete [] pixels;
	delete [] depth;
	delete [] mask;
	delete [] col_info;
	delete [] m_depth;

	if (m_pNuiSensor)
	{
		m_pNuiSensor->NuiShutdown();
	}
	if (m_hNextImageFrameEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hNextImageFrameEvent);
	}
	if (m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hNextDepthFrameEvent);
	}
	if (m_hNextSkeletonEvent && (m_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hNextSkeletonEvent);
	}
}

DWORD WINAPI PKinect::Nui_ProcessThread(LPVOID pParam)
{
	PKinect* pThis = (PKinect *) pParam;
	return pThis->Nui_ProcessThread();
}

DWORD WINAPI PKinect::Nui_ProcessThread()
{
	const int NUM = 4;
	HANDLE hEvents[NUM] = {
		m_hEvNuiProcessStop,
		m_hNextDepthFrameEvent,
		m_hNextImageFrameEvent,
		m_hNextSkeletonEvent
	};
	int idx;
	bool running = true;

	while (running) 
	{
		idx = WaitForMultipleObjects(NUM, hEvents, FALSE, 100);
		switch (idx)
		{
		case WAIT_TIMEOUT:
			continue;
		case WAIT_OBJECT_0:
			running = false;
			break;
		case WAIT_OBJECT_0 + 1:
			Nui_GotDepthAlert();
			break;
		case WAIT_OBJECT_0 + 2:
			Nui_GotImageAlert();
			break;
		case WAIT_OBJECT_0 + 3:
			Nui_GotSkeletonAlert();
			break;
		default:
			break;
		}
	}
	return 0;
}

HRESULT PKinect::Nui_Init()
{
	INuiSensor* pNuiSensor;
	HRESULT hr;

	int cnt = 0;
	hr = NuiGetSensorCount(&cnt);
	if (FAILED(hr))
	{
		return hr;
	}
	for (int i=0; i<cnt; i++)
	{
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}
		hr = pNuiSensor->NuiStatus();
		if (hr==S_OK)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}
		pNuiSensor->Release();
	}
	if (m_pNuiSensor != NULL)
	{
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | 
			NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | 
			NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			m_hNextImageFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			m_pImageStreamHandle = NULL;
			m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			m_pDepthStreamHandle = NULL;
			m_hNextSkeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			m_pSkeletonStreamHandle = NULL;

			hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, 
				m_hNextImageFrameEvent, &m_pImageStreamHandle);
			hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_640x480, 0, 2,
				m_hNextDepthFrameEvent, &m_pDepthStreamHandle);
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
		}
	}
	if (m_pNuiSensor == NULL || FAILED(hr))
		return hr;

	m_hEvNuiProcessStop = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThNuiProcess = CreateThread(NULL, 0, Nui_ProcessThread, this, 0, NULL);
	cout << "Init ok ... " << endl;
	return hr;
}

void PKinect::Nui_GotDepthAlert()
{
	NUI_IMAGE_FRAME pDepthFrame;
	HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &pDepthFrame);
	if (FAILED(hr))
		return;
	INuiFrameTexture *pTexture = pDepthFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
	{
		memcpy(m_depth, LockedRect.pBits, LockedRect.size);

		for (int i=0; i<frame_size; i++) 
		{
			BYTE temp1 = (m_depth[i] >> 3) / 32;
			BYTE temp2 = 255 - temp1;
			depth[i] = (temp1==0) ? 0xff000000 : 0xff000000 + (temp2<<16) + (temp2<<8) + temp2; 
		}
		m_pNuiSensor->NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
			NUI_IMAGE_RESOLUTION_640x480,
			NUI_IMAGE_RESOLUTION_640x480,
			frame_size,
			m_depth,
			frame_size*2,
			col_info);
	}
	pTexture->UnlockRect(0);
	hr = m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &pDepthFrame);
}

void PKinect::Nui_GotImageAlert()
{
	NUI_IMAGE_FRAME pImageFrame;
	HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pImageStreamHandle, 0, &pImageFrame);

	if (FAILED(hr))
		return;
	INuiFrameTexture *pTexture = pImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) 
	{
		BYTE *pBuffer = (BYTE *) LockedRect.pBits;
		UINT *imgBuf = (UINT *) pBuffer;

//		memcpy(pixels, LockedRect.pBits, LockedRect.size);
		UINT* pSrc;
		UINT* pDest;

		pSrc = NULL;
		for (int i=0; i<frame_size; i++) 
		{
			pixels[i] = imgBuf[i] | 0xff000000;
//			pixels[i] = pixels[i] | 0xff000000;
			USHORT pDepth = m_depth[i];
			USHORT player = NuiDepthPixelToPlayerIndex(pDepth);
			UINT colorInDepthX = col_info[i*2];
			UINT colorInDepthY = col_info[i*2+1];
			if (colorInDepthX >= 0 && colorInDepthX < WIDTH &&
				colorInDepthY >= 0 && colorInDepthY < HEIGHT)
			{
				UINT colorIndex = colorInDepthX + colorInDepthY*WIDTH;
				pSrc = (UINT *) pixels + colorIndex;
			}
			pDest = (UINT *) mask + i;
			if (player>0) 
			{
				*pDest = *pSrc | 0xff000000;
			} else {
				*pDest = *pSrc & 0x00ffffff;
			}
		}
//		memcpy(m_colour, LockedRect.pBits, LockedRect.size);
	}
	pTexture->UnlockRect(0);
	hr = m_pNuiSensor->NuiImageStreamReleaseFrame(m_pImageStreamHandle, &pImageFrame);
}

void PKinect::Nui_GotSkeletonAlert()
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if (FAILED(hr))
	{
		return;
	}
	m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);

	for (int i=0; i<NUI_SKELETON_COUNT; i++)
	{
		LONG px, py;
		USHORT dep;

		NuiTransformSkeletonToDepthImage(skeletonFrame.SkeletonData[i].Position, &px, &py, &dep);
		joints[i][NUI_SKELETON_POSITION_COUNT][0] = static_cast<float> (px/320.0f);
		joints[i][NUI_SKELETON_POSITION_COUNT][1] = static_cast<float> (py/240.0f);
		joints[i][NUI_SKELETON_POSITION_COUNT][2] = static_cast<float> (dep);
		joints[i][NUI_SKELETON_POSITION_COUNT][3] = (float) skeletonFrame.SkeletonData[i].eTrackingState;
		joints[i][NUI_SKELETON_POSITION_COUNT][4] = (float) skeletonFrame.SkeletonData[i].dwTrackingID;

		for (int j=0; j<NUI_SKELETON_POSITION_COUNT; j++) 
		{
			LONG x, y;
			USHORT depth;
			NuiTransformSkeletonToDepthImage(skeletonFrame.SkeletonData[i].SkeletonPositions[j], &x, &y, &depth);

			joints[i][j][0] = static_cast<float> (x/320.0f);
			joints[i][j][1] = static_cast<float> (y/240.0f);
			joints[i][j][2] = static_cast<float> (depth);
			joints[i][j][3] = skeletonFrame.SkeletonData[i].eSkeletonPositionTrackingState[j];
			joints[i][j][4] = 0.0f;
		}
	}
}

int * PKinect::Nui_GetImage()
{
	return pixels;
}

int * PKinect::Nui_GetDepth()
{
	return depth;
}

int * PKinect::Nui_GetMask()
{
	return mask;
}

