#include "stdafx.h"
#include "KinectPV2.h"

using namespace std;

static const DWORD c_FaceFrameFeatures =
  FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
| FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInInfraredSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInInfraredSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_Happy
| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
| FaceFrameFeatures::FaceFrameFeatures_LookingAway
| FaceFrameFeatures::FaceFrameFeatures_Glasses
| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;


static const double c_FaceRotationIncrementInDegrees = 5.0f;

static const long long kThreadSleepDuration = 5L;

//uint32_t  bodyColors[] = { 0x0000ff, 0x00ff00, 0xff0000, 0xffff00, 0xff00ff, 0x00ffff, 0x00ffff, 0xffffff };
//uint32_t  bodyColors[] = { 0xff0000ff, 0xff00ff00, 0xffff0000, 0xffffff00, 0xffff00ff, 0xff00ffff, 0xffffffff };
uint32_t  bodyColors[] = { 0x0000ff, 0x00ff00, 0xff0000, 0xffff00, 0xff00ff, 0x00ffff, 0x00ffff, 0xffffff };

namespace KinectPV2{
	//////////////////////////////////////////////////////////////////////////////////////////////
	Device::Device()
	{
		DeviceOptions();

		//COLOR
		pixelsData = (uint8_t  *)malloc(frame_size_color * 4 * sizeof(uint8_t));
		pixelsDataTemp = (uint8_t  *)malloc(frame_size_color * 4 * sizeof(uint8_t));
		colorFrameData = (uint32_t *)malloc(frame_size_color * sizeof(uint32_t));

		//COLOR
		depth_16_Data  = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		depth_256_Data = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		depthRaw_16_Data  = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		depthRaw_256_Data = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		//depth raw used to pass to mappers transformation
		depthRaw_16_temp  = (uint16_t *)malloc(frame_size_depth * sizeof(uint16_t));


		//INFRARED
		infraredData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		infraredLongExposureData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		depthMaskData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		//COLOR CHANNELS
		colorChannelsData = (float *)malloc(frame_size_color * 3 * sizeof(float));
		colorChannelsDataTemp = (RGBQUAD *)malloc(frame_size_color*sizeof(RGBQUAD));

		//SKELETON
		skeletonData3dMap = (float *)malloc(JOINTSIZE * sizeof(float));
		skeletonDataDepthMap = (float *)malloc(JOINTSIZE * sizeof(float));
		skeletonDataColorMap = (float *)malloc(JOINTSIZE * sizeof(float));

		//FACE
		faceColorData = (float *)malloc(FACESIZE * sizeof(float));
		faceInfraredData = (float *)malloc(FACESIZE * sizeof(float));

		pointCloudPosData = (float *)malloc(frame_size_depth * 3 * sizeof(float));

		pointCloudDepthImage = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

		pointCloudColorData = (float *)malloc(frame_size_color * 3 * sizeof(float));
		colorCameraPos = (float *)malloc(frame_size_color * 2 * sizeof(float));

		mapDepthToColorData = (float *)malloc(frame_size_depth * 2 * sizeof(float));

		mCamaraSpacePointDepth = new CameraSpacePoint[frame_size_depth];
		mCamaraSpacePointColor = new CameraSpacePoint[frame_size_color];
		mColorSpacePoint = new ColorSpacePoint[frame_size_depth];
		mDepthCoordinates = new DepthSpacePoint[frame_size_color];
		mDepthToColorPoints = new ColorSpacePoint[frame_size_depth];


		for (int i = 0; i < frame_size_depth * 3; i++){
			pointCloudPosData[i] = 0;
		}

		for (int i = 0; i < JOINTSIZE; i++){
			skeletonData3dMap[i] = 0.0;
			skeletonDataDepthMap[i] = 0.0;
			skeletonDataColorMap[i] = 0.0;
		}

		for (int i = 0; i < FACESIZE; i++){
			faceColorData[i] = 0.0;
			faceInfraredData[i] = 0.0;
		}

		for (int i = 0; i < BODY_COUNT; i++)
		{
			kFaceFrameSources[i] = nullptr;
			kFaceFrameReaders[i] = nullptr;
		}

		for (int i = 0; i < BODY_COUNT; i++)
		{
			kFaceFrameSources[i] = nullptr;
			kFaceFrameReaders[i] = nullptr;

			kHDFaceSource[i] = nullptr;
			kHDFaceReader[i] = nullptr;

			produce[i] = false;
		}


		appWidth = 1024;
		appHeight = 768;

		depthPCHighTh = 8000.0f;
		depthPCLowTh = 0.0f;

		skeletonMapType = 0;

		numberUsers = 0;
	}

	bool Device::init()
	{

		std::cout << "Creating Kinect object ..." << endl;

		if (FAILED(GetDefaultKinectSensor(&kSensor))) {
			std::cerr << "Kinect Not Found" << std::endl;
			kSensor->Close();
			return false;
		}


		BOOLEAN isOpen = false;
		kSensor->get_IsOpen(&isOpen);
		if (isOpen){
			std::cerr << "Is open" << std::endl;
			kSensor->Close();
		}


		// Initialize the Kinect and get the color reader
		HRESULT hr = kSensor->Open();

		if (FAILED(hr))
		{
			std::cerr << "Error Opening" << std::endl;
			return false;
		}

		
		//
		isOpen = false;
		kSensor->get_IsOpen(&isOpen);
		if (!isOpen){
			std::cerr << "Kinect Not Found" << std::endl;
			return false;
		}


		hr = kSensor->get_CoordinateMapper(&kCoordinateMapper);

		if (FAILED(hr))
		{
			std::cerr << "Error Loading Kinect Mapper" << std::endl;
			return false;
		}

		if (SUCCEEDED(hr))
		{

			if (DeviceOptions::isInitializedColorFrame())
			{

				IColorFrameSource* pColorFrameSource = NULL;

				if (SUCCEEDED(hr))
				{
					std::cout << "Setting Color Frame" << std::endl;
					hr = kSensor->get_ColorFrameSource(&pColorFrameSource);
				}

				if (FAILED(hr))
				{
					std::cerr << "Error Opening Color Frame" << std::endl;
					SafeRelease(pColorFrameSource);
					return false;
				}


				if (SUCCEEDED(hr))
				{
					hr = pColorFrameSource->OpenReader(&kColorFrameReader);
				}

				if (FAILED(hr))
				{
					std::cerr << "Error Opening Color Frame" << std::endl;
					SafeRelease(pColorFrameSource);
					return false;
				}


				SafeRelease(pColorFrameSource);
			}

			if (DeviceOptions::isInitializedDepthFrame())
			{
				std::cout << "Setting Depth Frame" << std::endl;
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
			if (DeviceOptions::isInitializedInfraredFrame())
			{
				std::cout << "Setting Infrared Frame" << std::endl;
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
			if (DeviceOptions::isInitializedSkeleton())
			{
				std::cout << "Setting Skeleton" << std::endl;
				IBodyFrameSource* pBodyFrameSource = NULL;

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
			if (DeviceOptions::isInitializedBodyIndexFrame())
			{
				std::cout << "Setting Body Index" << std::endl;
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

				//BODY TRACK AND MASK
				bodyTrackData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

				bodyTrackRaw = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

				//boyd Ids
				bodyTrackIds = (uint32_t *)malloc(BODY_COUNT * sizeof(uint32_t));

				for (int i = 0; i < BODY_COUNT; i++){
					bodyTrackIds[i] = 0;
				}

				bodyTackDataUser_1 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				bodyTackDataUser_2 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				bodyTackDataUser_3 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				bodyTackDataUser_4 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				bodyTackDataUser_5 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				bodyTackDataUser_6 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
			}

			if (DeviceOptions::isInitializedLongExposureInfraredFrame())
			{
				std::cout << "Setting Long Exposure Infrared" << std::endl;
				ILongExposureInfraredFrameSource * pLongExposureInFraredSource = NULL;

				if (SUCCEEDED(hr))
				{
					hr = kSensor->get_LongExposureInfraredFrameSource(&pLongExposureInFraredSource);
				}

				if (SUCCEEDED(hr))
				{
					hr = pLongExposureInFraredSource->OpenReader(&kLongExposureFrameReader);
				}
			}
			if (DeviceOptions::isInitializedFaceDetection())
			{
				cout << "Setting Face Tracking" << std::endl;

				if (!DeviceOptions::isEnableSkeleton()){
					IBodyFrameSource* pBodyFrameSource = NULL;

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

				// create a face frame source + reader to track each body in the fov
				for (int i = 0; i < BODY_COUNT; i++){
					//cout << i << endl;
					// create the face frame source by specifying the required face frame features
					hr = CreateFaceFrameSource(kSensor, 0, c_FaceFrameFeatures, &kFaceFrameSources[i]);
					//cout << "frace source" << endl;
					if (SUCCEEDED(hr))
					{
						// open the corresponding reader
						//cout << "open face reader" << std::endl;
						hr = kFaceFrameSources[i]->OpenReader(&kFaceFrameReaders[i]);
					}
					else{
						std::cerr << "Error Face Reader " << i << std::endl;
					}
				}
			}
			if (DeviceOptions::isInitializedHDFaceDetection())
			{

				//HD FACE
				hdFaceDeformations = (float *)malloc(BODY_COUNT * FaceShapeDeformations::FaceShapeDeformations_Count * sizeof(float));
				hdFaceVertex = (float *)malloc(HDFACEVERTEX *  sizeof(float));

				for (int count = 0; count < BODY_COUNT; count++){
					// Source
					hr = CreateHighDefinitionFaceFrameSource(kSensor, &kHDFaceSource[count]);

					if (FAILED(hr)){
						std::cerr << "Error : CreateHighDefinitionFaceFrameSource()" << std::endl;
						return false;
					}

					// Reader
					hr = kHDFaceSource[count]->OpenReader(&kHDFaceReader[count]);
					if (FAILED(hr)){
						std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenReader()" << std::endl;
						return false;
					}

					// Open Face Model Builder
					hr = kHDFaceSource[count]->OpenModelBuilder(FaceModelBuilderAttributes::FaceModelBuilderAttributes_None, &kFaceModelBuilder[count]);
					if (FAILED(hr)){
						std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenModelBuilder()" << std::endl;
						return -1;
					}

					// Start Collection Face Data
					hr = kFaceModelBuilder[count]->BeginFaceDataCollection();
					if (FAILED(hr)){
						std::cerr << "Error : IFaceModelBuilder::BeginFaceDataCollection()" << std::endl;
						return -1;
					}

					hr = CreateFaceAlignment(&kFaceAlignment[count]);
					if (FAILED(hr)){
						std::cerr << "Error : CreateFaceAlignment()" << std::endl;
						return false;
					}

					// Create Face Model
					hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, &hdFaceDeformations[count*FaceShapeDeformations::FaceShapeDeformations_Count], &kFaceModel[count]);
					if (FAILED(hr)){
						std::cerr << "Error : CreateFaceModel()" << std::endl;
						return false;
					}

					hr = GetFaceModelVertexCount(&hdFaceVertexCount); // 1347
					//cout << "HDFace Vertex :" << hdFaceVertexCount << std::endl;
					if (FAILED(hr)){
						std::cerr << "Error : GetFaceModelVertexCount()" << std::endl;
						return false;
					}


				}
				std::cout << "Enable HDFace" << std::endl;
			}
		}
		else{
			std::cerr << "Problem with the Device" << std::endl;
			return false;
		}

		if (!kSensor || FAILED(hr))
		{
			std::cerr << "Kinect Not Found" << std::endl;
			kSensor->Close();
			return false;
		}

		if (DeviceOptions::isInitializedColorFrame())
		{
			DeviceActivators::enableColorProcess(true);
			mThreadColor = std::thread(&Device::colorProcess, this);
		}

		if (DeviceOptions::isInitializedDepthFrame())
		{
			DeviceActivators::enableDepthProcess(true);
			mThreadDepth = std::thread(&Device::depthProcess, this);
		}
		if (DeviceOptions::isInitializedInfraredFrame())
		{
			DeviceActivators::enableInfraredProcess();
			mThreadInfrared = std::thread(&Device::infraredProcess, this);
			//std::cout << DeviceActivators::isInfraredProcessActivated() << std::endl;
			///std::cout << "initialized infraed" << std::endl;
		}
		if (DeviceOptions::isInitializedSkeleton())
		{
			DeviceActivators::enableSkeletonProcess(true);
			mThreadSkeleton = std::thread(&Device::skeletonProcess, this);
		}
		if (DeviceOptions::isInitializedBodyIndexFrame())
		{
			DeviceActivators::enableBodyTrackProcess(true);
			mThreadBodyTrack = std::thread(&Device::bodyTrackProcess, this);
		}

		if (DeviceOptions::isInitializedLongExposureInfraredFrame())
		{
			DeviceActivators::enableInfraredLongExposureProcess(true);
			mThreadInfraredLongExposure = std::thread(&Device::infraredLongExposureProcess, this);


		}
		if (DeviceOptions::isInitializedFaceDetection())
		{
			if (!DeviceOptions::isEnableSkeleton()){
				if (!DeviceActivators::isSkeletonProcessActivated()){
					DeviceActivators::enableSkeletonProcess(true);
					mThreadSkeleton = std::thread(&Device::skeletonProcess, this);
					//std::cout << "activated skeleton face" << std::endl;
				}
			}

		}

		if (DeviceOptions::isInitializedHDFaceDetection())
		{
			DeviceActivators::enableHDFaceProcess();
			mThreadHDFace = std::thread(&Device::hdFaceProcess, this);
		}


		if (DeviceOptions::isEnableCoordinateMapper()){
			DeviceActivators::enableCoordinateMapperProcess(true);
			mThreadCoodinateMapper = std::thread(&Device::coordinateMapperProcess, this);
		}

		
		std::cout << "Done Loading Kinect V2 " << endl;

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


	void Device::disable()
	{
		//deactivate options
		DeviceOptions::disableAll();
		DeviceActivators::disableAll();

		//threads
		/*mThreadDepth.join();
		mThreadColor.join();
		mThreadInfrared.join();
		mThreadInfraredLongExposure.join();
		mThreadSkeleton.join();
		mThreadBodyTrack.join();
		*/

	}

	bool Device::cleanMemory()
	{
		SafeRelease(kColorFrameReader);
		SafeRelease(kBodyFrameReader);
		SafeRelease(kBodyIndexFrameReader);
		SafeRelease(kLongExposureFrameReader);
		SafeRelease(kDepthFrameReader);
		SafeRelease(kInfraredFrameReader);
		SafeRelease(kCoordinateMapper);

		SafeDeletePointer(pixelsData);
		SafeDeletePointer(pixelsDataTemp);
		SafeDeletePointer(depth_16_Data);
		SafeDeletePointer(depth_256_Data);
		SafeDeletePointer(infraredData);
		SafeDeletePointer(colorFrameData);
		SafeDeletePointer(infraredLongExposureData);
		SafeDeletePointer(bodyTrackData);
		SafeDeletePointer(depthRaw_16_Data);
		SafeDeletePointer(depthRaw_256_Data);
		SafeDeletePointer(depthRaw_16_temp);
		SafeDeletePointer(pointCloudColorData);
		SafeDeletePointer(pointCloudPosData);
		SafeDeletePointer(colorCameraPos);
		SafeDeletePointer(depthMaskData);
		SafeDeletePointer(faceColorData);
		SafeDeletePointer(faceInfraredData);
		SafeDeletePointer(skeletonData3dMap);
		SafeDeletePointer(skeletonDataDepthMap);
		SafeDeletePointer(skeletonDataColorMap);
		SafeDeletePointer(pointCloudDepthImage);
		SafeDeletePointer(hdFaceVertex);
		SafeDeletePointer(hdFaceDeformations);


		SafeDeletePointer(colorChannelsDataTemp);

		SafeDeletePointer(bodyTrackIds);
		SafeDeletePointer(bodyTackDataUser_1);
		SafeDeletePointer(bodyTackDataUser_2);
		SafeDeletePointer(bodyTackDataUser_3);
		SafeDeletePointer(bodyTackDataUser_4);
		SafeDeletePointer(bodyTackDataUser_5);
		SafeDeletePointer(bodyTackDataUser_6);

		SafeArrayDelete(mCamaraSpacePointDepth);
		SafeArrayDelete(mCamaraSpacePointColor);
		SafeArrayDelete(mColorSpacePoint);
		SafeArrayDelete(mDepthCoordinates);
		SafeArrayDelete(mDepthToColorPoints);

		SafeDeletePointer(mapDepthToColorData);


		//source readers clean up
		for (int count = 0; count < BODY_COUNT; count++){
			SafeRelease(kHDFaceSource[count]);
			SafeRelease(kHDFaceReader[count]);
			SafeRelease(kFaceModelBuilder[count]);
			SafeRelease(kFaceAlignment[count]);
			SafeRelease(kFaceModel[count]);
			SafeRelease(kFaceFrameSources[count]);
			SafeRelease(kFaceFrameReaders[count]);
		}

		//close the kinect
		if (kSensor != NULL){
			kSensor->Close();
			SafeRelease(kSensor);
			cout << "Closed Kinect V2" << std::endl;
		}
		else{
			std::cerr << "error closing kinect" << std::endl;
			return false;
		}

		return true;
	}

	bool Device::update()
	{

	}

	void Device::colorProcess()
	{
		while (DeviceActivators::isColorProcessActivated()){
			if (DeviceOptions::isEnableColorFrame())
			{
				if (!kColorFrameReader)
				{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
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
					uint8_t * pBufferColor = NULL;

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
						pBufferColor = pixelsDataTemp;// new uint32_t[frame_size_color * 4];
						hr = pColorFrame->CopyConvertedFrameDataToArray(frame_size_color * 4 * sizeof(uint8_t), reinterpret_cast<BYTE*>(pBufferColor), ColorImageFormat_Bgra);


						if (SUCCEEDED(hr)) {
							memcpy(pixelsData, pBufferColor, frame_size_color * 4 * sizeof(uint8_t));
							colorFrameReady = true;
						}

						if (DeviceOptions::isEnableColorChannelsFrame()){

							if (SUCCEEDED(hr) && pBufferColor && kCoordinateMapper)
							{

								hr = kCoordinateMapper->MapColorFrameToCameraSpace(frame_size_depth, (depthRaw_16_temp), frame_size_color, mCamaraSpacePointColor);

								if (SUCCEEDED(hr) && mCamaraSpacePointColor != NULL){
									int indexColor = 0;
									int pixelColor = 0;
									int pixelBuffer = 0;
									int cameraSpaceIndex = 0;


									//cout << "passing color" << std::endl;
									while (indexColor < frame_size_color)
									{  //pixelsData
										pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].X;
										pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].Y;
										pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].Z;

									
										colorChannelsData[pixelColor++] = pBufferColor[pixelBuffer++]; //  *0.00390625;
										colorChannelsData[pixelColor++] = pBufferColor[pixelBuffer++]; // *0.00390625;
										colorChannelsData[pixelColor++] = pBufferColor[pixelBuffer++];  // *0.00390625;
										pixelBuffer++;
										++indexColor;
									}
								}
							}
						}

					}
					SafeRelease(pFrameDescription);
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					colorFrameReady = false;
					continue;
				}
				SafeRelease(pColorFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}
		}
	}

	void Device::infraredProcess()
	{
		while (DeviceActivators::isInfraredProcessActivated()){
			if (DeviceOptions::isEnableInFraredFrame()){
				if (!kInfraredFrameReader)
				{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
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
						hr = pInfraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

						if (SUCCEEDED(hr) && pBuffer != NULL && nWidth == cDepthWidth && nHeight == cDepthHeight)
						{
							const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
							int depthIndex = 0;


							while (pBuffer < pBufferEnd)
							{
								USHORT ir = *pBuffer;
								BYTE intensity = static_cast<BYTE>(ir >> 8);
								//inFraredFrameDataTemp[depthIndex] = colorByte2Int((uint32_t)intensity);
								infraredData[depthIndex] = colorByte2Int((uint32_t)intensity);
								++pBuffer;
								++depthIndex;
							}

							infraredFrameReady = true;
						}
					}
					SafeRelease(pFrameDescription);
				}
				else{
					infraredFrameReady = false;
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
				}

				SafeRelease(pInfraredFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}
		}
	}

	void Device::depthProcess()
	{
		std::cout << "thread started" << std::endl;

		while (DeviceActivators::isDepthProcessActivated()){
			if (DeviceOptions::isEnableDepthFrame())
			{
				if (!kDepthFrameReader)
				{
					std::cout << "ERROR READING DEPTH FRAME" << std::endl;
					depthFrameReady = false;
					depthPointCloudFrameReady = false;
					colorPointCloudFrameReady = false;
					cout << "eror" << std::endl;

					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
				}

				IDepthFrame* pDepthFrame = NULL;

				HRESULT hr = kDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

				if (SUCCEEDED(hr))
				{
					INT64 nTime = 0;
					IFrameDescription* pFrameDescription = NULL;
					USHORT nDepthMinReliableDistance = 0;
					USHORT nDepthMaxReliableDistance = 0;
					UINT nBufferSize = 0;
					UINT16 * pBufferDepth = NULL;

					hr = pDepthFrame->get_RelativeTime(&nTime);

					if (SUCCEEDED(hr))
					{
						hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
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
						//cout << "get frame" << std::endl;
						hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBufferDepth);

						if (SUCCEEDED(hr) && pBufferDepth){
							//uint32_t * depthFrameDataTemp = depthFrameData;
							const UINT16* pBufferEnd = pBufferDepth + (frame_size_depth);
							int depthIndex = 0;

							//std::mutex mtx;
							//mtx.lock();
							while (pBufferDepth < pBufferEnd)
							{
								USHORT depth = *pBufferDepth;
								BYTE intensity = static_cast<BYTE>((depth >= nDepthMinReliableDistance) && (depth <= nDepthMaxReliableDistance) ? (depth % 256) : 0);

								depthRaw_16_Data[depthIndex] = static_cast<uint32_t>(depth);
								depthRaw_256_Data[depthIndex] = static_cast<uint32_t>(intensity);

								depthRaw_16_temp[depthIndex] = static_cast<uint16_t>((depth >= nDepthMinReliableDistance) && (depth <= nDepthMaxReliableDistance) ? depth : 0);// (uint16_t(((float)intensity * 0.056666f)));

								//(value/4500)*255 ->  0.056666f
								depth_16_Data[depthIndex]  = colorByte2Int(uint32_t( ((float)depth * 0.056666f) ));
								depth_256_Data[depthIndex] = colorByte2Int(uint32_t(intensity));

								++pBufferDepth;
								++depthIndex;

							}
							//std::cout << "depth" << std::endl;
							//mtx.unlock();
							//memcpy(depthData, depthFrameDataTemp, frame_size_depth * sizeof(uint32_t));
							depthFrameReady = true;
						}
						else{
							this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
							continue;
							//std::cout << "error AccessUnderlyingBuffer frame depth" << std::endl;
						}

						if (DeviceOptions::isEnablePointCloud() && kCoordinateMapper){

							UINT16 *pBufferPC = NULL;
							hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBufferPC);
							//std::cout << "PC" << std::endl;
							if (SUCCEEDED(hr) && pBufferPC){

								hr = kCoordinateMapper->MapDepthFrameToCameraSpace(frame_size_depth, pBufferPC, frame_size_depth, mCamaraSpacePointDepth);

								hr = kCoordinateMapper->MapDepthFrameToColorSpace(frame_size_depth, pBufferPC, frame_size_depth, mDepthToColorPoints);


								if (SUCCEEDED(hr) && mCamaraSpacePointDepth != NULL){
									const UINT16 * pBufferEnd = pBufferPC + (frame_size_depth);
									int depthIndex = 0;
									int cameraSpaceIndex = 0;
									int deptToColorIndex = 0;

									while (pBufferPC < pBufferEnd){
										USHORT depth = *pBufferPC; //0 -4500 depth

										if (depth > depthPCLowTh && depth < depthPCHighTh){
											pointCloudPosData[cameraSpaceIndex++] = mCamaraSpacePointDepth[depthIndex].X;
											pointCloudPosData[cameraSpaceIndex++] = mCamaraSpacePointDepth[depthIndex].Y;
											pointCloudPosData[cameraSpaceIndex++] = mCamaraSpacePointDepth[depthIndex].Z;

											mapDepthToColorData[deptToColorIndex++] = mDepthToColorPoints[depthIndex].X;
											mapDepthToColorData[deptToColorIndex++] = mDepthToColorPoints[depthIndex].Y;
											
											pointCloudDepthImage[depthIndex] = colorByte2Int(uint32_t(((float)depth*0.056666f)));
										}
										else{
											pointCloudPosData[cameraSpaceIndex++] = NULL;
											pointCloudPosData[cameraSpaceIndex++] = NULL;
											pointCloudPosData[cameraSpaceIndex++] = NULL;

											mapDepthToColorData[deptToColorIndex++] = NULL;
											mapDepthToColorData[deptToColorIndex++] = NULL;

											pointCloudDepthImage[depthIndex] = colorByte2Int((uint32_t)0);
											//pointCloudDepthImage[depthIndex] = NULL;
										}

										//std::cout << colorByte2Int(uint32_t(val * 255)) << std::endl;
										++pBufferPC; //unsigned int
										++depthIndex;
									}

									//cout << "min: " << min << " max" << max << std::endl;
									//std::cout << cameraSpaceIndex << std::endl;

									//DeviceActivators::depthFrameReady(true);
									//DeviceActivators::depthPointCloudPosReady(true);
									//DeviceActivators::depthPointCloudImageReady(true);
								}
							}
							else{
								this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
								continue;
							}
						}

					}
					//SafeDeletePointer(pBuffer);
					SafeRelease(pFrameDescription);
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					depthFrameReady = false;
					continue;
				}
				SafeRelease(pDepthFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}
		}

	}


	void	Device::infraredLongExposureProcess()
	{

		while (DeviceActivators::isInfraredLongExposureProcessActivated()){
			if (DeviceOptions::isEnableInfraredLongExposureFrame())
			{
				if (!kLongExposureFrameReader)
				{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					longExposureReady = false;
					continue;
					//return false;
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
								BYTE intensity = static_cast<BYTE>(ir >> 8);
								infraredLongExposureData[longExposureIndex] = colorByte2Int((uint32_t)intensity);
								++pBuffer; //unsigned int
								++longExposureIndex;
							}
							//memcpy(depthData, depthFrameDataTemp, frame_size_depth * sizeof(uint32_t));
							//longExposureReady = true;
						}
					}
					SafeRelease(pFrameDescription);
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
					//longExposureReady = false;
				}
				SafeRelease(pLongExposureFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}
		}
	}

	void	Device::bodyTrackProcess()
	{
		while (DeviceActivators::isBodyTrackProcessActivated()){
			if (DeviceOptions::isEnableBodyIndex())
			{

				if (!kBodyIndexFrameReader)
				{
					std::cout << "ERROR LOADING BODY TRACK FRAME" << std::endl;
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
					//return false;
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
						std::vector<bool> tmpUserCount(6, false);
					
						if ( pBodyIndexBuffer != NULL && nBodyIndexWidth == cDepthWidth && nBodyIndexHeight == cDepthHeight)
						{
							//uint32_t * inFraredFrameDataTemp = inFraredFrameData;
							const BYTE* pBufferEnd = pBodyIndexBuffer + (frame_size_depth);

							int bodyIndex = 0;
							int bodyIndexCountPrev = 0;
							int bodyColor = 0;

							int min = 3;
							int max = 4;

							while (pBodyIndexBuffer < pBufferEnd)
							{
								BYTE ir = *pBodyIndexBuffer;
								//save index
								bodyIndexCountPrev = bodyColor;
								bodyColor = (uint32_t)ir;


								//if (bodyIndex %200)
								//std::cout <<" - "<< uint8_t(ir) << ", " << bodyColor << " - ";

								if (bodyColor == 0){
									bodyTackDataUser_1[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(0) = true;
								}
								else{
									bodyTackDataUser_1[bodyIndex] = 0x00000000;
								}

								if (bodyColor == 1){
									bodyTackDataUser_2[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(1) = true;
								}
								else{
									bodyTackDataUser_2[bodyIndex] = 0x00000000;
								}

								if (bodyColor == 2){
									bodyTackDataUser_3[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(2) = true;
								}
								else{
									bodyTackDataUser_3[bodyIndex] = 0x00000000;
								}

								if (bodyColor == 3){
									bodyTackDataUser_4[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(3) = true;
								}
								else{
									bodyTackDataUser_4[bodyIndex] = 0x00000000;
								}

								if (bodyColor == 4){
									bodyTackDataUser_5[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(4) = true;
								}
								else{
									bodyTackDataUser_5[bodyIndex] = 0x00000000;
								}

								if (bodyColor == 5){
									bodyTackDataUser_6[bodyIndex] = bodyColors[bodyColor];
									tmpUserCount.at(5) = true;
								}
								else{
									bodyTackDataUser_6[bodyIndex] = 0x00000000;
								}

								bodyTrackRaw[bodyIndex] = bodyColor;
								//uint32_t color = ((ir / 32) << 5) + ((ir / 32) << 2) + (ir / 64);
								bodyTrackData[bodyIndex]  = colorByte2Int((uint32_t)bodyColor);
								++pBodyIndexBuffer; //(unsigned int)
								++bodyIndex;

							}

							//obtain the current number of Users based on the tracking data
							int mUserCounter = 0;
							int ids = 0;
							for (auto & numUser : tmpUserCount){
								if (numUser){
									bodyTrackIds[ids] = 1;
									mUserCounter++;
								}
								else{
									bodyTrackIds[ids] = 0;
								}
								ids++;
							}
							numberUsers  = mUserCounter;

							//std::cout << min <<" m, max "<< max << std::endl;
							//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
							//	bodyIndexReady = true;
						}

						if (DeviceOptions::isEnableDepthMaskFrame()){
							BYTE *pBodyIndexBufferDeptMask = NULL;

							hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBufferDeptMask);

							if (SUCCEEDED(hr) && pBodyIndexBufferDeptMask != NULL && nBodyIndexWidth == cDepthWidth && nBodyIndexHeight == cDepthHeight)
							{

								//uint32_t * inFraredFrameDataTemp = inFraredFrameData;
								const BYTE* pBufferEnd = pBodyIndexBufferDeptMask + (frame_size_depth);
								int depthMaskIndex = 0;
								while (pBodyIndexBufferDeptMask < pBufferEnd)
								{
									BYTE ir = int(*pBodyIndexBufferDeptMask);
									uint32_t intensity;
									if (ir == 0)
										intensity = 0x0000ff;
									else if (ir == 1)
										intensity = 0x00ff00;
									else if (ir == 2)
										intensity = 0xff0000;
									else if (ir == 3)
										intensity = 0xffff00;
									else if (ir == 4)
										intensity = 0xff00ff;
									else if (ir == 5)
										intensity = 0x00ffff;
									else if (ir == 6)
										intensity = 0xffffff;
									else
										intensity = depth_16_Data[depthMaskIndex];
									depthMaskData[depthMaskIndex] = intensity;
									++pBodyIndexBufferDeptMask; //(unsigned int)
									++depthMaskIndex;
								}
								//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
								//depthMaskReady = true;

							}
							//	else{
							//this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
							//	continue;
							//	}
						}
					}
					else{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}
					SafeRelease(pBodyIndexFrameDescription);
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));


					//bodyIndexReady = false;
					//depthMaskReady = false;

					continue;
				}

				SafeRelease(pBodyIndexFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}
		}
	}

	void	Device::skeletonProcess()
	{

		while (DeviceActivators::isSkeletonProcessActivated()){
			//std::cout << "face -2" << std::endl;
			if (DeviceOptions::isEnableFaceDetection() || DeviceOptions::isEnableSkeleton())
			{
				if (!kBodyFrameReader)
				{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
				}

				IBody* ppBodies[BODY_COUNT] = { 0 };
				IBodyFrame* pBodyFrame = nullptr;

				HRESULT hr = E_FAIL;

				if (kBodyFrameReader != nullptr)
				{

					hr = kBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
					if (SUCCEEDED(hr))
					{
						hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
					}
					else{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}
				}


				//	std::cout << "face -1" << std::endl;

				if (DeviceOptions::isEnableFaceDetection()){

					//		std::cout << "face" << std::endl;
					bool bHaveBodyData = SUCCEEDED(hr);

					// iterate through each face reader
					for (int iFace = 0; iFace < BODY_COUNT; ++iFace)
					{
						// retrieve the latest face frame from this reader
						IFaceFrame* pFaceFrame = nullptr;
						HRESULT hr = kFaceFrameReaders[iFace]->AcquireLatestFrame(&pFaceFrame);
						BOOLEAN bFaceTracked = false;

						//cout << "tracking "<<iFace << std::endl;
						if (SUCCEEDED(hr) && nullptr != pFaceFrame)
						{
							// check if a valid face is tracked in this face frame
							hr = pFaceFrame->get_IsTrackingIdValid(&bFaceTracked);
						}

						if (SUCCEEDED(hr))
						{
							if (bFaceTracked)
							{
								//	cout << "tracking" << std::endl;
								//faceDetectionReady = false;
								IFaceFrameResult* pFaceFrameResult = nullptr;
								RectI faceBoxColor = { 0 };
								RectI faceBoxInfrared = { 0 };
								PointF facePointsColor[FacePointType::FacePointType_Count];
								PointF facePointsInfrared[FacePointType::FacePointType_Count];
								Vector4 faceRotation;
								DetectionResult faceProperties[FaceProperty::FaceProperty_Count];

								hr = pFaceFrame->get_FaceFrameResult(&pFaceFrameResult);

								// need to verify if pFaceFrameResult contains data before trying to access it
								if (SUCCEEDED(hr) && pFaceFrameResult != nullptr)
								{
									hr = pFaceFrameResult->get_FaceBoundingBoxInColorSpace(&faceBoxColor);
									hr = pFaceFrameResult->get_FaceBoundingBoxInInfraredSpace(&faceBoxInfrared);

									if (SUCCEEDED(hr))
									{
										hr = pFaceFrameResult->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, facePointsColor);
										hr = pFaceFrameResult->GetFacePointsInInfraredSpace(FacePointType::FacePointType_Count, facePointsInfrared);
									}

									if (SUCCEEDED(hr))
									{
										hr = pFaceFrameResult->get_FaceRotationQuaternion(&faceRotation);
									}

									if (SUCCEEDED(hr))
									{
										hr = pFaceFrameResult->GetFaceProperties(FaceProperty::FaceProperty_Count, faceProperties);
									}

									if (SUCCEEDED(hr))
									{

										faceColorData[iFace * 36 + 35] = 1.0;
										faceInfraredData[iFace * 36 + 35] = 1.0;

										for (int j = 0; j < FacePointType::FacePointType_Count; j++){
											int index2 = iFace * 36 + j * 2;
											faceColorData[index2 + 0] = facePointsColor[j].X;
											faceColorData[index2 + 1] = facePointsColor[j].Y;

											faceInfraredData[index2 + 0] = facePointsInfrared[j].X;
											faceInfraredData[index2 + 1] = facePointsInfrared[j].Y;
											//cout << facePoints[j].X << " " << facePoints[j].Y << std::endl;
										}

										int index = iFace * 36 + 20;

										faceColorData[index + 0] = faceBoxColor.Left;
										faceColorData[index + 1] = faceBoxColor.Top;
										faceColorData[index + 2] = faceBoxColor.Right - faceBoxColor.Left;
										faceColorData[index + 3] = faceBoxColor.Bottom - faceBoxColor.Top;

										faceInfraredData[index + 0] = faceBoxInfrared.Left;
										faceInfraredData[index + 1] = faceBoxInfrared.Top;
										faceInfraredData[index + 2] = faceBoxInfrared.Right - faceBoxInfrared.Left;
										faceInfraredData[index + 3] = faceBoxInfrared.Bottom - faceBoxInfrared.Top;

										float pitch, yaw, roll;
										ExtractRotationInDegrees(&faceRotation, pitch, yaw, roll);

										faceColorData[index + 4] = pitch;
										faceColorData[index + 5] = yaw;
										faceColorData[index + 6] = roll;

										faceInfraredData[index + 4] = pitch;
										faceInfraredData[index + 5] = yaw;
										faceInfraredData[index + 6] = roll;

										index = index + 7;
										for (int iProperty = 0; iProperty < FaceProperty::FaceProperty_Count; iProperty++)
										{
											int iPro = 0;
											switch (iProperty)
											{
											case FaceProperty::FaceProperty_Happy:
												iPro = 0;
												break;
											case FaceProperty::FaceProperty_Engaged:
												iPro = 1;
												break;
											case FaceProperty::FaceProperty_LeftEyeClosed:
												iPro = 2;
												break;
											case FaceProperty::FaceProperty_RightEyeClosed:
												iPro = 3;
												break;
											case FaceProperty::FaceProperty_LookingAway:
												iPro = 4;
												break;
											case FaceProperty::FaceProperty_MouthMoved:
												iPro = 5;
												break;
											case FaceProperty::FaceProperty_MouthOpen:
												iPro = 6;
												break;
											case FaceProperty::FaceProperty_WearingGlasses:
												iPro = 7;
												break;
											}

											switch (faceProperties[iProperty])
											{
											case DetectionResult::DetectionResult_Unknown:
												faceColorData[index + iPro] = -1;
												faceInfraredData[index + iPro] = -1;
												break;
											case DetectionResult::DetectionResult_Yes:
												faceColorData[index + iPro] = 1;
												faceInfraredData[index + iPro] = 1;
												break;
											case DetectionResult::DetectionResult_No:
											case DetectionResult::DetectionResult_Maybe:
												faceColorData[index + iPro] = 0;
												faceInfraredData[index + iPro] = 0;
												break;
											}
										}

									}
								}
								else{
									this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
									continue;
								}

								SafeRelease(pFaceFrameResult);
							}
							else
							{
								faceColorData[iFace * 36 + 35] = 0.0;
								faceInfraredData[iFace * 36 + 35] = 0.0;;
								if (bHaveBodyData)
								{
									// check if the corresponding body is tracked 
									// if this is true then update the face frame source to track this body
									IBody* pBody = ppBodies[iFace];
									if (pBody != nullptr)
									{
										BOOLEAN bTracked = false;
										hr = pBody->get_IsTracked(&bTracked);

										UINT64 bodyTId;
										if (SUCCEEDED(hr) && bTracked)
										{
											// get the tracking ID of this body
											hr = pBody->get_TrackingId(&bodyTId);
											if (SUCCEEDED(hr))
											{
												// update the face frame source with the tracking ID
												kFaceFrameSources[iFace]->put_TrackingId(bodyTId);
											}
											else{
												this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
												continue;
											}
										}
									}
									//faceDetectionReady = false;
								}
							}
						}
						else{
							this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
							continue;
						}

						SafeRelease(pFaceFrame);

					}
					faceDetectionReady = true;
				}


				if (DeviceOptions::isEnableSkeleton())
				{

					if (!kBodyFrameReader)
					{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}

					if (SUCCEEDED(hr))
					{
						INT64 nTime = 0;
						hr = pBodyFrame->get_RelativeTime(&nTime);

						if (SUCCEEDED(hr) && kCoordinateMapper)
						{
							if (DeviceOptions::isEnableSkeletonDepthMap()){
								for (int i = 0; i < BODY_COUNT; ++i){
									IBody* pBody = ppBodies[i];
									if (pBody) //[6][25][3]
									{
										BOOLEAN bTracked = false;
										hr = pBody->get_IsTracked(&bTracked);

										if (SUCCEEDED(hr) && bTracked)
										{
											skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = 1.0;
											Joint jointsTracked[JointType_Count];
											HandState leftHandState = HandState_Unknown;
											HandState rightHandState = HandState_Unknown;

											pBody->get_HandLeftState(&leftHandState);
											pBody->get_HandRightState(&rightHandState);

											hr = pBody->GetJoints(_countof(jointsTracked), jointsTracked);

											if (SUCCEEDED(hr))
											{
												JointOrientation jointOrientations[_countof(jointsTracked)];
												hr = pBody->GetJointOrientations(_countof(jointsTracked), jointOrientations);
												if (SUCCEEDED(hr))
												{
													for (int j = 0; j < _countof(jointsTracked); ++j)
													{
														float * pointScreen = BodyToScreenDepth(jointsTracked[j].Position);
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 0] = pointScreen[0];
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 1] = pointScreen[1];
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 2] = pointScreen[2];

														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 3] = jointOrientations[j].Orientation.w;
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 4] = jointOrientations[j].Orientation.x;
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 5] = jointOrientations[j].Orientation.y;
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 6] = jointOrientations[j].Orientation.z;

														if (jointsTracked[j].JointType == JointType_HandLeft)
															skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = leftHandState;
														else if (jointsTracked[j].JointType == JointType_HandRight)
															skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = rightHandState;
														else
															skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = jointsTracked[j].TrackingState;
														skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + j * 9 + 8] = jointsTracked[j].JointType;
													}
												}
											}
											else{
												skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
												continue;
											}
										}
										else{
											//this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
											skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
											//continue;
										}
									}
									else{
										//this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
										skeletonDataDepthMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
										//continue;
									}
								}
								skeletonDepthReady = true;
							}

							if (DeviceOptions::isEnableSkeletonColorMap()){
								for (int i = 0; i < BODY_COUNT; ++i){
									IBody* pBody = ppBodies[i];
									if (pBody) //[6][25][3]
									{
										BOOLEAN bTracked = false;
										hr = pBody->get_IsTracked(&bTracked);

										if (SUCCEEDED(hr) && bTracked)
										{
											skeletonDataColorMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = 1.0;
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
													float * pointScreen = BodyToScreenColor(jointsTracked[j].Position);
													skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 0] = pointScreen[0];
													skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 1] = pointScreen[1];
													skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 2] = pointScreen[2];

													if (jointsTracked[j].JointType == JointType_HandLeft)
														skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = leftHandState;
													else if (jointsTracked[j].JointType == JointType_HandRight)
														skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = rightHandState;
													else
														skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = jointsTracked[j].TrackingState;
													skeletonDataColorMap[i*(JointType_Count + 1) * 9 + j * 9 + 8] = jointsTracked[j].JointType;
												}
											}
										}
										else{
											skeletonDataColorMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
										}
									}
									else{
										skeletonDataColorMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
									}
								}
								skeletonColorReady = true;
							}

							if (DeviceOptions::isEnableSkeleton3DMap()){
								for (int i = 0; i < BODY_COUNT; ++i){
									IBody* pBody = ppBodies[i];
									if (pBody) //[6][25][3]
									{
										BOOLEAN bTracked = false;
										hr = pBody->get_IsTracked(&bTracked);

										if (SUCCEEDED(hr) && bTracked)
										{
											skeletonData3dMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = 1.0;
											Joint jointsTracked[JointType_Count];
											HandState leftHandState = HandState_Unknown;
											HandState rightHandState = HandState_Unknown;

											pBody->get_HandLeftState(&leftHandState);
											pBody->get_HandRightState(&rightHandState);

											hr = pBody->GetJoints(_countof(jointsTracked), jointsTracked);
											if (SUCCEEDED(hr))
											{
												JointOrientation jointOrientations[_countof(jointsTracked)];
												hr = pBody->GetJointOrientations(_countof(jointsTracked), jointOrientations);

												//Joint parentJoint = jointsTracked[JointType_Head];
												//JointOrientation parentJointOrientation = jointOrientations[JointType_Head];

												if (SUCCEEDED(hr)){
													for (int j = 0; j < _countof(jointsTracked); ++j)
													{

														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 0] = jointsTracked[j].Position.X;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 1] = jointsTracked[j].Position.Y;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 2] = jointsTracked[j].Position.Z;

														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 3] = jointOrientations[j].Orientation.w;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 4] = jointOrientations[j].Orientation.x;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 5] = jointOrientations[j].Orientation.y;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 6] = jointOrientations[j].Orientation.z;

														if (jointsTracked[j].JointType == JointType_HandLeft){
															skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = leftHandState;
														}
														else if (jointsTracked[j].JointType == JointType_HandRight)
															skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = rightHandState;
														else
															skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 7] = jointsTracked[j].TrackingState;
														skeletonData3dMap[i*(JointType_Count + 1) * 9 + j * 9 + 8] = jointsTracked[j].JointType;
													}
												}
											}
										}
										else{
											skeletonData3dMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
										}
									}
									else{
										skeletonData3dMap[i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1] = -1.0;
									}
								}
								skeleton3dReady = true;
							}
						}
					}
					else{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}
				}

				for (int i = 0; i < _countof(ppBodies); ++i)
				{
					SafeRelease(ppBodies[i]);
				}

				SafeRelease(pBodyFrame);
			}
			else{
				this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
				continue;
			}

		}
	}

	void Device::hdFaceProcess()
	{
		while (DeviceActivators::isHDFaceProcessActivated()){
			if (DeviceOptions::isEnableHDFaceDetection())
			{
				// Body Frame
				IBodyFrame * pBodyFrame = 0;
				IBodyFrameReference * frameRef = nullptr;
				HRESULT hr = E_FAIL;

				if (kBodyFrameReader != nullptr)
				{
					hr = kBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
				}

				if (SUCCEEDED(hr))
				{
					IBody* pBody[BODY_COUNT] = { 0 };
					hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody);
					if (SUCCEEDED(hr))
					{
						for (int count = 0; count < BODY_COUNT; count++)
						{
							BOOLEAN bTrackingIdValid = false;
							hr = kHDFaceSource[count]->get_IsTrackingIdValid(&bTrackingIdValid);
							if (!bTrackingIdValid)
							{
								BOOLEAN bTracked = false;
								hr = pBody[count]->get_IsTracked(&bTracked);
								if (SUCCEEDED(hr) && bTracked)
								{
									// Set TrackingID to Detect Face
									UINT64 trackingId = _UI64_MAX;
									hr = pBody[count]->get_TrackingId(&trackingId);
									if (SUCCEEDED(hr))
									{
										kHDFaceSource[count]->put_TrackingId(trackingId);
									}
								}
							}
						}
					}
					else{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}
					for (int count = 0; count < BODY_COUNT; count++){
						SafeRelease(pBody[count]);
					}
				}
				else{
					this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
					continue;
				}
				SafeRelease(pBodyFrame);

				for (int count = 0; count < BODY_COUNT; count++)
				{
					IHighDefinitionFaceFrame* pHDFaceFrame = nullptr;
					hr = kHDFaceReader[count]->AcquireLatestFrame(&pHDFaceFrame); ///not getting last frame

					if (SUCCEEDED(hr) && pHDFaceFrame != nullptr)
					{
						BOOLEAN bFaceTracked = false;
						hr = pHDFaceFrame->get_IsFaceTracked(&bFaceTracked);

						if (SUCCEEDED(hr) && bFaceTracked)
						{
							//cout << "face tracking" << std::endl;
							hdFaceVertex[BODY_COUNT*hdFaceVertexCount * 2 + count] = 1;
							hr = pHDFaceFrame->GetAndRefreshFaceAlignmentResult(kFaceAlignment[count]);
							if (SUCCEEDED(hr) && kFaceAlignment[count] != nullptr)
							{
								if (!produce[count])
								{
									FaceModelBuilderCollectionStatus status;
									hr = kFaceModelBuilder[count]->get_CollectionStatus(&status);
									if (status == FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_Complete)
									{
										//std::cout << "Status : Complete" << std::endl;

										IFaceModelData* pFaceModelData = nullptr;
										hr = kFaceModelBuilder[count]->GetFaceData(&pFaceModelData);
										if (SUCCEEDED(hr) && pFaceModelData != nullptr)
										{
											hr = pFaceModelData->ProduceFaceModel(&kFaceModel[count]);
											if (SUCCEEDED(hr) && kFaceModel[count] != nullptr){
												produce[count] = true;
											}
										}
										SafeRelease(pFaceModelData);
									}
									//else{
									//std::system("cls");
									//std::cout << "Status : " << status << std::endl;
									//}
								}

								std::vector<CameraSpacePoint> facePoints(hdFaceVertexCount);
								hr = kFaceModel[count]->CalculateVerticesForAlignment(kFaceAlignment[count], hdFaceVertexCount, &facePoints[0]);
								if (SUCCEEDED(hr))
								{
									for (int point = 0; point < hdFaceVertexCount; point++)
									{
										ColorSpacePoint colorSpacePoint;
										hr = kCoordinateMapper->MapCameraPointToColorSpace(facePoints[point], &colorSpacePoint);
										if (SUCCEEDED(hr))
										{

											hdFaceVertex[count*hdFaceVertexCount * 2 + point * 2 + 0] = colorSpacePoint.X;
											hdFaceVertex[count*hdFaceVertexCount * 2 + point * 2 + 1] = colorSpacePoint.Y;
										}
									}
								}
							}
						}
						else{
							hdFaceVertex[BODY_COUNT*hdFaceVertexCount * 2 + count] = 0;
						}
					}
					else{
						this_thread::sleep_for(chrono::milliseconds(kThreadSleepDuration));
						continue;
					}
					SafeRelease(pHDFaceFrame);
				}
				DeviceActivators::HDFaceDetectionReady(true);
			}
		}
	}

	void Device::coordinateMapperProcess()
	{
		while (DeviceActivators::isCoordinateMapperActivated()){


		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	float Device::lmap(float val, float inMin, float inMax, float outMin, float outMax)
	{
		return outMin + (outMax - outMin) * ((val - inMin) / (inMax - inMin));
	}

	float Device::constrain(float val, float min, float max)
	{
		return (val >= max) ? max : ((min >= val) ? min : val);
	}


	float * Device::BodyToScreenDepth(const CameraSpacePoint& bodyPoint)
	{
		float pointsScreen[2];
		// Calculate the body's position on the screen
		DepthSpacePoint depthPoint = { 0 };
		kCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

		pointsScreen[0] = depthPoint.X;
		pointsScreen[1] = depthPoint.Y;
		return pointsScreen;
	}

	float * Device::BodyToScreenColor(const CameraSpacePoint& bodyPoint)
	{
		float pointsScreen[2];
		// Calculate the body's position on the screen
		ColorSpacePoint colorPoint = { 0 };

		kCoordinateMapper->MapCameraPointToColorSpace(bodyPoint, &colorPoint);

		pointsScreen[0] = colorPoint.X;
		pointsScreen[1] = colorPoint.Y;
		return pointsScreen;
	}

	int Device::colorByte2Int(int gray){
		gray = gray & 0xffff;
		return 0xff000000 | (gray << 16) | (gray << 8) | gray;
	}

	HRESULT Device::UpdateBodyData(IBody** ppBodies)
	{
		HRESULT hr = E_FAIL;
		if (kBodyFrameReader != nullptr)
		{
			IBodyFrame* pBodyFrame = nullptr;
			hr = kBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
			if (SUCCEEDED(hr))
			{
				hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
			}
			SafeRelease(pBodyFrame);
		}
		return hr;
	}

	void Device::ExtractRotationInDegrees(const Vector4* pQuaternion, float & pPitch, float & pYaw, float & pRoll)
	{
		double x = pQuaternion->x;
		double y = pQuaternion->y;
		double z = pQuaternion->z;
		double w = pQuaternion->w;

		// convert face rotation quaternion to Euler angles in degrees		
		double dPitch, dYaw, dRoll;
		dPitch = atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z) / M_PI * 180.0;
		dYaw = asin(2 * (w * y - x * z)) / M_PI * 180.0;
		dRoll = atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z) / M_PI * 180.0;

		pPitch = dPitch;
		pYaw = dYaw;
		pRoll = dRoll;

		//cout << pPitch << " " << pYaw << " " << pRoll << std::endl;

		// clamp rotation values in degrees to a specified range of values to control the JNI_GetDepth_16_Data -1.0)) / increment) * static_cast<int>(increment);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t * Device::JNI_GetImage()
	{
		return  reinterpret_cast<uint32_t *>(pixelsData);
	}



	uint32_t * Device::JNI_GetDepth_16_Data()
	{
		return depth_16_Data;
	}

	uint32_t * Device::JNI_GetDepth_256_Data()
	{
		return depth_256_Data;
	}


	uint32_t * Device::JNI_GetDepthMask()
	{
		return depthMaskData;
	}

	uint32_t * Device::JNI_GetDepthRaw_16_Data()
	{
		return depthRaw_16_Data;
	}

	uint32_t * Device::JNI_GetDepthRaw_256_Data()
	{
		return depthRaw_256_Data;
	}

	uint32_t * Device::JNI_GetInfrared()
	{
		return infraredData;
	}

	uint32_t *	Device::JNI_GetLongExposureInfrared()
	{
		return infraredLongExposureData;
	}

	uint32_t * Device::JNI_GetBodyTrack()
	{
		return bodyTrackData;
	}

	uint32_t * Device::JNI_GetRawBodyTrack()
	{
		return bodyTrackRaw;
	}

	float *	Device::JNI_getMapDepthToColor()
	{
		return mapDepthToColorData;
	}


	uint32_t *	Device::JNI_getBodyIndexUser(int index)
	{
		switch (index){
		case 0:
			return bodyTackDataUser_1;
			break;
		case 1:
			return bodyTackDataUser_2;
			break;
		case 2:
			return bodyTackDataUser_3;
			break;
		case 3:
			return bodyTackDataUser_4;
			break;
		case 4:
			return bodyTackDataUser_5;
			break;
		case 5:
			return bodyTackDataUser_6;
			break;
		}
		return bodyTackDataUser_1;
	}
	int Device::JNI_getNumOfUsers()
	{
		return numberUsers;
	}

	uint32_t * Device::JNI_getTrackedIds()
	{
		return bodyTrackIds;
	}


	float * Device::JNI_getSkeletonDepthMapData()
	{
		return skeletonDataDepthMap;
	}

	float * Device::JNI_getSkeleton3DMapData()
	{
		return skeletonData3dMap;
	}

	float * Device::JNI_getSkeletonColorMapData()
	{
		return skeletonDataColorMap;
	}

	float * Device::JNI_pointCloudPosData()
	{
		return pointCloudPosData;
	}

	float * Device::JNI_pointCloudColorData()
	{
		return pointCloudColorData;
	}

	uint32_t * Device::JNI_pointCloudDepthImage()
	{
		return pointCloudDepthImage;
	}


	float * Device::JNI_getFaceColorRawData()
	{
		return faceColorData;
	}

	float * Device::JNI_getFaceInfraredRawData()
	{
		return faceInfraredData;
	}

	float * Device::JNI_getHDFaceVertexRawData()
	{
		return hdFaceVertex;
	}

	float * Device::JNI_getColorChannel()
	{
		return colorChannelsData;
	}


}