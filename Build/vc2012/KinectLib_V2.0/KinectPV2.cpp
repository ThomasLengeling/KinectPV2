// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KinectPV2.h"

using namespace std;

namespace KinectPV2{
	//////////////////////////////////////////////////////////////////////////////////////////////
	Device::Device()
	{
		pixelsData       = (uint8_t  *)malloc(BUFFER_SIZE_COLOR);
		colorFrameData   = (uint8_t *)malloc(BUFFER_SIZE_COLOR);

		depthData        = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		infraredData     = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		bodyTrackData    = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		longExposureData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		skeletonData     = (float *)malloc(JOINTSIZE * sizeof(float));
		
		for (int i = 0; i < JOINTSIZE; i++){
			skeletonData[i] = 0.0;
		}

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

			if (DeviceOptions::isEnableColorFrame())
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

			if (DeviceOptions::isEnableDepthFrame())
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
			if (DeviceOptions::isEnableInFraredFrame())
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
			if (DeviceOptions::isEnableSkeleton())
			{
				std::cout << "SETTING SKELETON"<<std::endl;
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
			if (DeviceOptions::isEnableBodyTrack())
			{
				std::cout << "SETTING BODY TRACK" << std::endl;
				IBodyIndexFrameSource * pBodyIndexFrameSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pBodyIndexFrameSource->OpenReader(&kBodyIndexFrameReader);
				}

				SafeRelease(pBodyIndexFrameSource);
			}
			if (DeviceOptions::isEnableInfraredExposureFrame())
			{
				std::cout << "SETTING LONG EXPOSURE INFRARED" << std::endl;
				ILongExposureInfraredFrameSource * pLongExposureInFraredSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_LongExposureInfraredFrameSource(&pLongExposureInFraredSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pLongExposureInFraredSource->OpenReader(&kLongExposureFrameReader);
				}

				SafeRelease(pLongExposureInFraredSource);
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
		/*
		free(pixelsData);
		free(depthData);
		free(infraredData);
		free(colorFrameData);
		free(skeletonData);
		free(longExposureData);
		free(bodyTrackData);

		
		delete[] pixelsData;
		delete[] depthData;
		delete[] infraredData;
		delete[] skeletonData;
		delete[] longExposureData;
		delete[] bodyTrackData;
		delete[] colorFrameData;
		*/
	}


	void Device::stop()
	{
		cout << "Clossing kinect V2" << std::endl;
		if (DeviceOptions::isEnableColorFrame())
			SafeRelease(kColorFrameReader);

		if (DeviceOptions::isEnableSkeleton()){
			SafeRelease(kBodyFrameReader);
			SafeRelease(kCoordinateMapper);
		}

		if (DeviceOptions::isEnableBodyTrack())
			SafeRelease(kBodyIndexFrameReader);

		if (DeviceOptions::isEnableInfraredExposureFrame())
			SafeRelease(kLongExposureFrameReader);

		if (DeviceOptions::isEnableDepthFrame())
			SafeRelease(kDepthFrameReader);

		if (DeviceOptions::isEnableInFraredFrame())
			SafeRelease(kInfraredFrameReader);

		DeviceOptions::enableInFraredImage(false);
		DeviceOptions::enableColorImage(false);
		DeviceOptions::enableDepthImage(false);
		DeviceOptions::enableInFraredExposureImage(false);
		DeviceOptions::enableBodyTrack(false);
		DeviceOptions::enableSkeleton(false);

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

		free(pixelsData);
		free(depthData);
		free(infraredData);
		free(colorFrameData);
		free(skeletonData);
		free(longExposureData);
		free(bodyTrackData);
		 
		(pixelsData)   = NULL;
		(depthData)    = NULL;
		(infraredData) = NULL;
		(colorFrameData) = NULL;
		(skeletonData) = NULL;
		(longExposureData) = NULL;
		(bodyTrackData) = NULL;
	}

	bool Device::update()
	{
		if (DeviceOptions::isEnableColorFrame())
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

		if (DeviceOptions::isEnableDepthFrame())
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

		if (DeviceOptions::isEnableInfraredExposureFrame())
		{
			if (!kLongExposureFrameReader)
			{
				std::cout << "ERROR READING DEPTH FRAME" << std::endl;
				longExposureReady = false;
				return false;
			}
			ILongExposureInfraredFrame * pLongExposureFrame = NULL;
			HRESULT hr = kLongExposureFrameReader->AcquireLatestFrame(&pLongExposureFrame);

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

				hr = pLongExposureFrame->get_RelativeTime(&nTime);

				if (SUCCEEDED(hr))
				{
					hr = pLongExposureFrame->get_FrameDescription(&pFrameDescription);
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
					hr = pLongExposureFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
					if (SUCCEEDED(hr) && nWidth == cDepthWidth && nHeight == cDepthHeight && pBuffer){
						//uint32_t * depthFrameDataTemp = depthFrameData;
						const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
						int longExposureIndex = 0;

						while (pBuffer < pBufferEnd)
						{
							USHORT ir = *pBuffer;
							BYTE intensity =  static_cast<BYTE>(ir >> 4);
							longExposureData[longExposureIndex] = colorByte2Int((uint32_t)intensity);
							++pBuffer; //unsigned int
							++longExposureIndex;
						}
						//memcpy(depthData, depthFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						longExposureReady = true;
					}
				}
				SafeRelease(pFrameDescription);
			}
			else{
				longExposureReady = false;
			}
			SafeRelease(pLongExposureFrame);
		}

		if (DeviceOptions::isEnableInFraredFrame())
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

		if (DeviceOptions::isEnableBodyTrack())
		{

			if (!kBodyIndexFrameReader)
			{
				std::cout << "ERROR LOADING BODY TRACK FRAME" << std::endl;
				return false;
			}
			IBodyIndexFrame * pBodyIndexFrame = NULL;

			HRESULT hr = kBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);

			if (SUCCEEDED(hr))
			{

				IFrameDescription* pBodyIndexFrameDescription = NULL;
				int nBodyIndexWidth = 0;
				int nBodyIndexHeight = 0;
				UINT nBodyIndexBufferSize = 0;
				BYTE *pBodyIndexBuffer = NULL;

				if (SUCCEEDED(hr))
				{
					hr = pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = pBodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = pBodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
				}

				if (SUCCEEDED(hr))
				{
					if (SUCCEEDED(hr) && pBodyIndexBuffer != NULL && nBodyIndexWidth == cDepthWidth && nBodyIndexHeight == cDepthHeight)
					{
						//uint32_t * inFraredFrameDataTemp = inFraredFrameData;
						const BYTE* pBufferEnd = pBodyIndexBuffer + (frame_size_depth);
						int depthIndex = 0;
						while (pBodyIndexBuffer < pBufferEnd)
						{
							BYTE ir = *pBodyIndexBuffer;
							int irC = uint32_t(ir);
							uint32_t intensity;
							if (ir == 0)
								intensity = 0x00000000; //->black
							else if (ir == 1)
								intensity = 0xff000000 | 0x00 | 0x00 | 0xFF;
							else if (ir == 2)
								intensity = 0xff000000 | 0x00 | 0xFF | 0x00;
							else if (ir == 3)
								intensity = 0xff000000 | 0xFF | 0x00 | 0x00;
							else
								intensity = 0xFFFFFFFF;
								//uint32_t color = ((ir / 32) << 5) + ((ir / 32) << 2) + (ir / 64);
							bodyTrackData[depthIndex] = intensity;// colorByte2Int((uint32_t)intensity);
							
							++pBodyIndexBuffer; //(unsigned int)
							++depthIndex;
						}
						//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						bodyIndexReady = true;
					}
				}
				SafeRelease(pBodyIndexFrameDescription);
			}
			else{
				bodyIndexReady = false;
			}

			SafeRelease(pBodyIndexFrame);
		}


		if (DeviceOptions::isEnableSkeleton())
		{

			if (!kBodyFrameReader)
			{
				return false;
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
						if (pBody) //[6][25][3]
						{
							BOOLEAN bTracked = false;
							hr = pBody->get_IsTracked(&bTracked);
							
							if (SUCCEEDED(hr) && bTracked)
							{
								skeletonData[i*(JointType_Count)* 5 + JointType_Count * 5 + 0] = 1.0;
								Joint jointsTracked[JointType_Count];
								HandState leftHandState = HandState_Unknown;
								HandState rightHandState = HandState_Unknown;

								pBody->get_HandLeftState(&leftHandState);
								pBody->get_HandRightState(&rightHandState);

								hr = pBody->GetJoints(_countof(jointsTracked), jointsTracked);
								if (SUCCEEDED(hr))
								{
									int stateHand;
									for (int j = 0; j < _countof(jointsTracked); ++j)
									{
										float * pointScreen = BodyToScreen(jointsTracked[j].Position);
										skeletonData[i*(JointType_Count)*5 + j*5 + 0] = pointScreen[0];
										skeletonData[i*(JointType_Count)*5 + j*5 + 1] = pointScreen[1];
										skeletonData[i*(JointType_Count)*5 + j*5 + 2] = 0;

										if (j == JointType_HandLeft)
											skeletonData[i*(JointType_Count)* 5 + j * 5 + 3] = leftHandState;
										else if (j == JointType_HandRight)
											skeletonData[i*(JointType_Count)* 5 + j * 5 + 3] = rightHandState;
										else
											skeletonData[i*(JointType_Count)* 5 + j * 5 + 3] = jointsTracked[j].TrackingState;
										skeletonData[i*(JointType_Count)* 5 + j * 5 + 4] = jointsTracked[j].JointType;
									}
								}
							}
							else{
								skeletonData[i*(JointType_Count)* 5 + JointType_Count * 5 + 0] = 0.0;
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

	uint32_t *	Device::JNI_GetLongExposureInfrared()
	{
		return longExposureData;
	}

	uint32_t * Device::JNI_GetBodyTrack()
	{
		return bodyTrackData;
	}

	float * Device::JNI_getSkeletonRawData()
	{
		return skeletonData;
	}
}