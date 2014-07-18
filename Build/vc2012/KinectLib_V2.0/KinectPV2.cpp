// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KinectPV2.h"

using namespace std;

namespace KinectPV2{
	//////////////////////////////////////////////////////////////////////////////////////////////
	Device::Device()
	{
		pixelsData = (uint8_t  *)malloc(BUFFER_SIZE_COLOR);
		depthData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		infraredData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		//inFraredFrameData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		//depthFrameData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		colorFrameData = (uint8_t *)malloc(BUFFER_SIZE_COLOR);

		appWidth  = 1024;
		appHeight = 768;
	}

	bool Device::init()
	{
		std::cout << "Creating Kinect object ..." << endl;

		HRESULT hr = GetDefaultKinectSensor(&kSensor);
		if (FAILED(hr))
		{
			std::cout << "ERROR LOADING KINECT" << std::endl;
			return false;
		}

		if (kSensor)
		{
			// Initialize the Kinect and get the color reader
			hr = kSensor->Open();

			if (DeviceOptions::isAnableColorFrame())
			{
				std::cout << "PASSING COLOR FRAME" << std::endl;
				IColorFrameSource* pColorFrameSource = NULL;

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

			if (DeviceOptions::isAnableDepthFrame())
			{
				std::cout << "PASSING DEPTH FRAME" << std::endl;
				IDepthFrameSource* pDepthFrameSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_DepthFrameSource(&pDepthFrameSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pDepthFrameSource->OpenReader(&kDepthFrameReader);
				}

				SafeRelease(pDepthFrameSource);
			}
			if (DeviceOptions::isAnableInFraredFrame())
			{
				std::cout << "PASSING INFRARED FRAME" << std::endl;
				IInfraredFrameSource* pInfraredFrameSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_InfraredFrameSource(&pInfraredFrameSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pInfraredFrameSource->OpenReader(&kInfraredFrameReader);
				}

				SafeRelease(pInfraredFrameSource);
			}
			if (DeviceOptions::isAnableBody())
			{
				std::cout << "SETTING BODY" << std::endl;
				IBodyFrameSource* pBodyFrameSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_CoordinateMapper(&kCoordinateMapper);
				}

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_BodyFrameSource(&pBodyFrameSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pBodyFrameSource->OpenReader(&kBodyFrameReader);
				}

				SafeRelease(pBodyFrameSource);
			}
		}
		else{
			cout << "Problem with the Device" << std::endl;
			return false;
		}

		if (!kSensor || FAILED(hr))
		{
			cout << "KINECT NOT FOUND" << std::endl;
			if (kFrameReader != 0) {
				kFrameReader->Release();
				kFrameReader = 0;
			}
			return false;
		}
		cout << "Done initialization Kinect V2" << endl;
		return true;
	}

	void Device::setWindowSize(int appWidth, int appHeight)
	{
		this->appHeight = appHeight;
		this->appWidth = appWidth;
	}

	Device::~Device(void)
	{
		free(pixelsData);
		free(depthData);
		free(infraredData);

		delete[] pixelsData;
		delete[] depthData;
		delete[] infraredData;
	}


	void Device::stop()
	{
		cout << "Clossing kinect V2" << std::endl;
		if (DeviceOptions::isAnableColorFrame())
			SafeRelease(kColorFrameReader);

		if (DeviceOptions::isAnableBody()){
			SafeRelease(kBodyFrameReader);
			SafeRelease(kCoordinateMapper);
		}

		if (DeviceOptions::isAnableDepthFrame())
			SafeRelease(kDepthFrameReader);

		if (DeviceOptions::isAnableInFraredFrame())
			SafeRelease(kInfraredFrameReader);

		DeviceOptions::disabeInfraredImage();
		DeviceOptions::disableColorImage();
		DeviceOptions::disableDepthImage();
		DeviceOptions::disableBody();

		if (kFrameReader != 0) {
			kFrameReader->Release();
			kFrameReader = 0;
		}
		if (kSensor != 0) {
			long hr = kSensor->Close();
			if (SUCCEEDED(hr) && kSensor != 0) {
				kSensor->Release();
				kSensor = 0;
			}
		}
	}

	bool Device::update()
	{
		if (DeviceOptions::isAnableColorFrame())
		{
			if (!kColorFrameReader)
			{
				std::cout << "ERROR READING COLOR FRAME" << std::endl;
				return false;
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
					//ColorImageFormat_Bgra
					pBuffer = colorFrameData;// new uint32_t[frame_size_color * 4];
					hr = pColorFrame->CopyConvertedFrameDataToArray(BUFFER_SIZE_COLOR, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
					if (SUCCEEDED(hr)) {
						memcpy(pixelsData, pBuffer, BUFFER_SIZE_COLOR);
						colorFrameReady = true;
					}
				}
				SafeRelease(pFrameDescription);
			}
			else{
				colorFrameReady = false;
			}
			SafeRelease(pColorFrame);
		}

		if (DeviceOptions::isAnableDepthFrame())
		{
			if (!kDepthFrameReader)
			{
				std::cout << "ERROR READING DEPTH FRAME" << std::endl;
				depthFrameReady = false;
				return false;
			}

			IDepthFrame* pDepthFrame = NULL;

			HRESULT hr = kDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

			if (SUCCEEDED(hr))
			{
				INT64 nTime = 0;
				IFrameDescription* pFrameDescription = NULL;
				int nWidth = 0;
				int nHeight = 0;
				USHORT nDepthMinReliableDistance = 0;
				USHORT nDepthMaxReliableDistance = 0;
				UINT nBufferSize = 0;
				UINT16 *pBuffer = NULL;

				hr = pDepthFrame->get_RelativeTime(&nTime);

				if (SUCCEEDED(hr))
				{
					hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
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
					hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
				}

				if (SUCCEEDED(hr))
				{
					hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);
				}

				if (SUCCEEDED(hr))
				{
					hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
					if (SUCCEEDED(hr) && nWidth == cDepthWidth && nHeight == cDepthHeight && pBuffer){
						//uint32_t * depthFrameDataTemp = depthFrameData;
						const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
						int depthIndex = 0;

						while (pBuffer < pBufferEnd)
						{
							USHORT depth = *pBuffer;
							BYTE intensity = static_cast<BYTE>((depth >= nDepthMinReliableDistance) && (depth <= nDepthMaxReliableDistance) ? (depth % 256) : 0);
							//depthFrameDataTemp[depthIndex] = colorByte2Int((uint32_t)intensity);
							depthData[depthIndex] = colorByte2Int((uint32_t)intensity);
							++pBuffer; //unsigned int
							++depthIndex;
						}
						//memcpy(depthData, depthFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						depthFrameReady = true;
					}
				}
				SafeRelease(pFrameDescription);
			}
			else{
				depthFrameReady = false;
			}
			SafeRelease(pDepthFrame);
		}

		if (DeviceOptions::isAnableInFraredFrame())
		{

			if (!kInfraredFrameReader)
			{
				std::cout << "ERROR LOADIN INFRARED FRAME" << std::endl;
				return false;
			}

			IInfraredFrame* pInfraredFrame = NULL;

			HRESULT hr = kInfraredFrameReader->AcquireLatestFrame(&pInfraredFrame);

			if (SUCCEEDED(hr))
			{
				INT64 nTime = 0;
				IFrameDescription* pFrameDescription = NULL;
				int nWidth = 0;
				int nHeight = 0;
				UINT nBufferSize = 0;
				UINT16 *pBuffer = NULL;

				hr = pInfraredFrame->get_RelativeTime(&nTime);

				if (SUCCEEDED(hr))
				{
					hr = pInfraredFrame->get_FrameDescription(&pFrameDescription);
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
					//pBuffer = new uint16_t[cDepthWidth * cDepthHeight*4];
					hr = pInfraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

					if (SUCCEEDED(hr) && pBuffer != NULL && nWidth == cDepthWidth && nHeight == cDepthHeight)
					{
						//uint32_t * inFraredFrameDataTemp = inFraredFrameData;
						const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
						int depthIndex = 0;
						while (pBuffer < pBufferEnd)
						{
							USHORT ir = *pBuffer;
							BYTE intensity = static_cast<BYTE>(ir >> 8);
							//inFraredFrameDataTemp[depthIndex] = colorByte2Int((uint32_t)intensity);
							infraredData[depthIndex] = colorByte2Int((uint32_t)intensity);
							++pBuffer; //(unsigned int)
							++depthIndex;
						}
						//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						infraredFrameReady = true;
					}
				}
				SafeRelease(pFrameDescription);
			}
			else{
				infraredFrameReady = false;
			}

			SafeRelease(pInfraredFrame);
		}


		/*
		if (DeviceOptions::isAnableBody())
		{

		if (!kBodyFrameReader)
		{
		return;
		}

		IBodyFrame* pBodyFrame = NULL;

		HRESULT hr = kBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

		if (SUCCEEDED(hr))
		{
		INT64 nTime = 0;
		hr = pBodyFrame->get_RelativeTime(&nTime);
		IBody* ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
		hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr) && kCoordinateMapper)
		{
		for (int i = 0; i < BODY_COUNT; ++i)
		{
		IBody* pBody = ppBodies[i];
		if (pBody)
		{
		BOOLEAN bTracked = false;
		hr = pBody->get_IsTracked(&bTracked);
		//[6][25][3]
		if (SUCCEEDED(hr) && bTracked)
		{
		Joint jointsTracked[JointType_Count];
		HandState leftHandState = HandState_Unknown;
		HandState rightHandState = HandState_Unknown;

		pBody->get_HandLeftState(&leftHandState);
		pBody->get_HandRightState(&rightHandState);

		hr = pBody->GetJoints(_countof(jointsTracked), jointsTracked);
		if (SUCCEEDED(hr))
		{
		for (int j = 0; j < _countof(jointsTracked); ++j)
		{
		float * pointScreen = BodyToScreen(jointsTracked[j].Position);
		joints[i][j][0] = pointScreen[0];
		joints[i][j][1] = pointScreen[1];
		joints[i][j][2] = 0;
		joints[i][j][3] = 0;
		joints[i][j][4] = 0.0f;
		}
		}
		}
		}
		}
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
		SafeRelease(ppBodies[i]);
		}
		}
		SafeRelease(pBodyFrame);
		}
		}
		*/
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	float * Device::BodyToScreen(const CameraSpacePoint& bodyPoint)
	{
		float pointsScreen[2];
		// Calculate the body's position on the screen
		DepthSpacePoint depthPoint = { 0 };
		kCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

		pointsScreen[0] = static_cast<float>(depthPoint.X * appWidth) / (float)cDepthWidth;
		pointsScreen[1] = static_cast<float>(depthPoint.Y * appHeight) / (float)cDepthHeight;
		return pointsScreen;
	}

	int Device::colorByte2Int(int gray){
		gray = gray & 0xffff;
		return 0xff000000 | (gray << 16) | (gray << 8) | gray;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t * Device::JNI_GetImage()
	{
		return pixelsData;
	}

	uint32_t * Device::JNI_GetDepth()
	{
		return depthData;
	}

	uint32_t * Device::JNI_GetDepthSha()
	{
		uint32_t * newInt = depthData;
		return newInt;
	}

	uint32_t * Device::JNI_GetInfrared()
	{
		return infraredData;
	}
}