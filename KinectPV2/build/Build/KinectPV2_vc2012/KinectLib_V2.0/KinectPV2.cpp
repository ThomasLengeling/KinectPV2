// PKinect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KinectPV2.h"

using namespace std;

static const DWORD c_FaceFrameFeatures =
FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInInfraredSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInInfraredSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
| FaceFrameFeatures::FaceFrameFeatures_Happy
| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
| FaceFrameFeatures::FaceFrameFeatures_LookingAway
| FaceFrameFeatures::FaceFrameFeatures_Glasses
| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;

static const double c_FaceRotationIncrementInDegrees = 5.0f;

uint32_t  bodyColors[] = { 0x0000ffff, 0x00ff00ff, 0xff0000ff, 0xffff00ff, 0xff00ffff, 0x00ffffff, 0x0000ffff };

namespace KinectPV2{
	//////////////////////////////////////////////////////////////////////////////////////////////
	Device::Device() :
		pixelsData(NULL), pixelsDataTemp(NULL), colorChannelsData(NULL), colorChannelsDataTemp(NULL), outCoordMapperRGBX(NULL),
		depthData(NULL), depthMaskData(NULL), depthRaw_16_Data(NULL), depthRaw_256_Data(NULL), pointCloudPosData(NULL), pointCloudColorData(NULL),
		pointCloudDepthImage(NULL), pointCloudDepthNormalized(NULL), pointCloudRawImage(NULL), colorCameraPos(NULL), infraredData(NULL),
		longExposureData(NULL), skeletonData3dMap(NULL), skeletonDataDepthMap(NULL), skeletonDataColorMap(NULL), faceDataColor(NULL),
		faceDataInInfrared(NULL), hdFaceDeformations(NULL), hdFaceVertex(NULL), hdFaceVertexCount(NULL), mapDepthCameraTableData(NULL),
		mapDepthToColorData(NULL), mapColorToDepthData(NULL), bodyIndexData(NULL), bodyTrackingIndex(NULL),
		bodyTackDataUser_1(NULL), bodyTackDataUser_2(NULL), bodyTackDataUser_3(NULL), bodyTackDataUser_4(NULL), bodyTackDataUser_5(NULL), bodyTackDataUser_6(NULL)
	{
		DeviceOptions();
		DeviceActivators();

		//COORDINATE MAPER RGB + DEPTH
		outCoordMapperRGBX = (uint8_t *)malloc(BUFFER_SIZE_COLOR);
		backgroundRGBX	   = (uint8_t *)malloc(BUFFER_SIZE_COLOR);

		for (int i = 0, index = 0; i < frame_size_color; i++){
			backgroundRGBX[index++] = 0x00;
			backgroundRGBX[index++] = 0xff;
			backgroundRGBX[index++] = 0x00;
			backgroundRGBX[index++] = 0xff;
		}

		pointCloudPosData = (float *)malloc(frame_size_depth * 3 * sizeof(float));

		pointCloudDepthImage = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
		pointCloudDepthNormalized = (float *)malloc(frame_size_depth * sizeof(float));
		pointCloudColorData = (float *)malloc(frame_size_color * 3 * sizeof(float));
		colorCameraPos = (float *)malloc(frame_size_color * 2 * sizeof(float));

		mCamaraSpacePointDepth = new CameraSpacePoint[frame_size_depth];
		mCamaraSpacePointColor = new CameraSpacePoint[frame_size_color];
		mColorSpacePoint	   = new ColorSpacePoint[frame_size_depth];
		mDepthCoordinates      = new DepthSpacePoint[frame_size_color];
		mDepthToColorPoints    = new ColorSpacePoint[frame_size_depth];

		//CREATE MAP DATA
		mapDepthCameraTableData = (float *)malloc(frame_size_depth * 2 * sizeof(float));

		mapDepthToColorData		= (float *)malloc(frame_size_depth * 2 * sizeof(float));
		//mapColorToDepthData     = (float *)malloc(frame_size_depth * 2 * sizeof(float));


		for (int i = 0; i < frame_size_depth * 3; i++){
			pointCloudPosData[i] = 0;
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

		depthPCHighTh = 8.0f;
		depthPCLowTh = 0.0f;

		activateMapDepthToCamaraTable = false;
		activateMapDepthToColor = false;

		numberUsers = 6;
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
			if (SUCCEEDED(hr))
			{
				hr = kSensor->get_CoordinateMapper(&kCoordinateMapper);
			}
			if (FAILED(hr)){
				cout << "Coordinate mapper fail" << std::endl;
			}

			hr = kSensor->Open();

			if (FAILED(hr)){
				cout << "ERROR KINECT" << std::endl;
				return false;
			}

			if (SUCCEEDED(hr))
			{
				long flags = 0L;
				if (DeviceOptions::isInitializedColorFrame())
				{
					std::cout << "ENABLE COLOR FRAME" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_Color;

					pixelsData = (uint8_t *)malloc(BUFFER_SIZE_COLOR);
					pixelsDataTemp = (uint8_t *)malloc(BUFFER_SIZE_COLOR);

					colorChannelsData = (float *)malloc(frame_size_color * 3 * sizeof(float));
					colorChannelsDataTemp = (RGBQUAD *)malloc(frame_size_color*sizeof(RGBQUAD));
				}

				if (DeviceOptions::isInitializedDepthFrame())
				{
					std::cout << "ENABLE DEPTH FRAME" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_Depth;

					depthData         = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					depthRaw_16_Data  = (uint16_t *)malloc(frame_size_depth * sizeof(uint16_t));
					depthRaw_256_Data = (uint16_t *)malloc(frame_size_depth * sizeof(uint16_t));

					depthMaskData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				}

				if (DeviceOptions::isInitializedInfraredFrame())
				{
					std::cout << "ENABLE INFRARED FRAME" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_Infrared;

					infraredData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				}

				if (DeviceOptions::isInitializedBodyIndexFrame())
				{
					std::cout << "ENABLE BODY TRACK" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_BodyIndex;


					bodyIndexData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

					bodyTackDataUser_1 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					bodyTackDataUser_2 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					bodyTackDataUser_3 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					bodyTackDataUser_4 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					bodyTackDataUser_5 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
					bodyTackDataUser_6 = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));

					//BODY TRACKING DATA
					bodyTrackingIndex = (uint32_t *)malloc(BODY_COUNT * sizeof(uint32_t));
				}
				if (DeviceOptions::isInitializedSkeleton())
				{
					std::cout << "ENABLE SKELETON" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_Body;

					skeletonData3dMap = (float *)malloc(JOINTSIZE * sizeof(float));
					skeletonDataDepthMap = (float *)malloc(JOINTSIZE * sizeof(float));
					skeletonDataColorMap = (float *)malloc(JOINTSIZE * sizeof(float));

					for (int i = 0; i < JOINTSIZE; i++){
						skeletonData3dMap[i] = 0.0;
						skeletonDataDepthMap[i] = 0.0;
						skeletonDataColorMap[i] = 0.0;
					}

				}
				if (DeviceOptions::isInitializedLongExposureInfraredFrame())
				{
					std::cout << "ENABLE LONG EXPOSURE INFRARED" << std::endl;
					flags |= FrameSourceTypes::FrameSourceTypes_LongExposureInfrared;

					longExposureData = (uint32_t *)malloc(frame_size_depth * sizeof(uint32_t));
				}
				if (DeviceOptions::isInitializedFaceDetection())
				{
					cout << "SETTING FACE TRACKING" << std::endl;

					if (!DeviceOptions::isInitializedSkeleton()){
						flags |= FrameSourceTypes::FrameSourceTypes_Body;

						skeletonData3dMap = (float *)malloc(JOINTSIZE * sizeof(float));
						skeletonDataDepthMap = (float *)malloc(JOINTSIZE * sizeof(float));
						skeletonDataColorMap = (float *)malloc(JOINTSIZE * sizeof(float));

						for (int i = 0; i < JOINTSIZE; i++){
							skeletonData3dMap[i] = 0.0;
							skeletonDataDepthMap[i] = 0.0;
							skeletonDataColorMap[i] = 0.0;
						}
					}

					//FACE DATA
					faceDataColor = (float *)malloc(FACESIZE * sizeof(float));
					faceDataInInfrared = (float *)malloc(FACESIZE * sizeof(float));

					for (int i = 0; i < FACESIZE; i++){
						faceDataColor[i] = 0.0;
						faceDataInInfrared[i] = 0.0;
					}

					for (int i = 0; i < BODY_COUNT; i++){
						hr = CreateFaceFrameSource(kSensor, 0, c_FaceFrameFeatures, &kFaceFrameSources[i]);
						if (SUCCEEDED(hr))
						{
							hr = kFaceFrameSources[i]->OpenReader(&kFaceFrameReaders[i]);
						}
						else{
							cout << "ERROR FACE READER FRAME" << i << std::endl;
						}
					}
				}
				if (DeviceOptions::isInitializedHDFaceDetection()){

					if (!DeviceOptions::isInitializedSkeleton()){
						flags |= FrameSourceTypes::FrameSourceTypes_Body;

						skeletonData3dMap = (float *)malloc(JOINTSIZE * sizeof(float));
						skeletonDataDepthMap = (float *)malloc(JOINTSIZE * sizeof(float));
						skeletonDataColorMap = (float *)malloc(JOINTSIZE * sizeof(float));

						for (int i = 0; i < JOINTSIZE; i++){
							skeletonData3dMap[i] = 0.0;
							skeletonDataDepthMap[i] = 0.0;
							skeletonDataColorMap[i] = 0.0;
						}
					}

					//HD FACE
					hdFaceDeformations = (float *)malloc(BODY_COUNT * FaceShapeDeformations::FaceShapeDeformations_Count * sizeof(float));
					hdFaceVertex = (float *)malloc(HDFACEVERTEX *  sizeof(float));

					for (int count = 0; count < BODY_COUNT; count++){
						// Source
						hr = CreateHighDefinitionFaceFrameSource(kSensor, &kHDFaceSource[count]);

						if (FAILED(hr)){
							cout << "Error : CreateHighDefinitionFaceFrameSource()" << std::endl;
							return false;
						}

						// Reader
						hr = kHDFaceSource[count]->OpenReader(&kHDFaceReader[count]);
						if (FAILED(hr)){
							cout << "Error : IHighDefinitionFaceFrameSource::OpenReader()" << std::endl;
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
							std::cout << "Error : CreateFaceAlignment()" << std::endl;
							return false;
						}

						// Create Face Model
						hr = CreateFaceModel(1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, &hdFaceDeformations[count*FaceShapeDeformations::FaceShapeDeformations_Count], &kFaceModel[count]);
						if (FAILED(hr)){
							std::cout << "Error : CreateFaceModel()" << std::endl;
							return false;
						}

						hr = GetFaceModelVertexCount(&hdFaceVertexCount); // 1347
						//cout << "HDFace Vertex :" << hdFaceVertexCount << std::endl;
						if (FAILED(hr)){
							std::cout << "Error : GetFaceModelVertexCount()" << std::endl;
							return false;
						}

				
					}
					std::cout << "ENABLE HDFACE" << std::endl;
				}


				hr = kSensor->OpenMultiSourceFrameReader(flags, &kMultiSourceFrameReader);
				if (FAILED(hr)) {
					hr = kSensor->OpenMultiSourceFrameReader(FrameSourceTypes_None, &kMultiSourceFrameReader);
					if (FAILED(hr)){
						SafeRelease(kMultiSourceFrameReader);
						cout << "ERROR READING FRAME DATA" << std::endl;
						return false;
					}
					else{
						cout << "NO FRAME DATA" << std::endl;
					}
				}
			}
		}
		else{
			cout << "Problem with the Device" << std::endl;
			return false;
		}

		if (!kSensor || FAILED(hr))
		{
			cout << "KINECT NOT FOUND" << std::endl;
			kSensor->Close();
			return false;
		}
		cout << "Done init Kinect v2" << endl;
		return true;
	}

	void Device::setWindowSize(int appWidth, int appHeight)
	{
		this->appHeight = appHeight;
		this->appWidth = appWidth;
	}

	Device::~Device(void)
	{

	}


	void Device::stop()
	{
		
		SafeRelease(kCoordinateMapper);
		SafeRelease(kMultiSourceFrameReader);

		if (kSensor) {
			kSensor->Close();
		}
		SafeRelease(kSensor);


		SafeDeletePointer(pixelsData);
		SafeDeletePointer(pixelsDataTemp);

		for (int i = 0; i < BODY_COUNT; i++)
		{
			SafeRelease(kFaceFrameSources[i]);
			SafeRelease(kFaceFrameReaders[i]);
		}
		
		SafeDeletePointer(colorChannelsData);
		SafeDeletePointer(colorChannelsDataTemp);
		SafeDeletePointer(depthData);
		SafeDeletePointer(infraredData);
		SafeDeletePointer(longExposureData);
		SafeDeletePointer(bodyIndexData);
		SafeDeletePointer(depthRaw_16_Data);
		SafeDeletePointer(depthRaw_256_Data);
		SafeDeletePointer(pointCloudColorData);
		SafeDeletePointer(pointCloudPosData);
		SafeDeletePointer(colorCameraPos);
		SafeDeletePointer(depthMaskData);
		SafeDeletePointer(faceDataColor);
		SafeDeletePointer(faceDataInInfrared);
		SafeDeletePointer(skeletonData3dMap);
		SafeDeletePointer(skeletonDataDepthMap);
		SafeDeletePointer(skeletonDataColorMap);
		SafeDeletePointer(pointCloudDepthImage);
		SafeDeletePointer(pointCloudDepthNormalized);
		SafeDeletePointer(outCoordMapperRGBX);
		SafeDeletePointer(backgroundRGBX);
		SafeDeletePointer(mapDepthCameraTableData);
		SafeDeletePointer(mapDepthToColorData);

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

		for (int count = 0; count < BODY_COUNT; count++){
			SafeRelease(kHDFaceSource[count]);
			SafeRelease(kHDFaceReader[count]);
			SafeRelease(kFaceModelBuilder[count]);
			SafeRelease(kFaceAlignment[count]);
			SafeRelease(kFaceModel[count]);
		}

		cout << "Closed KinectPV2" << std::endl;
	}

	bool Device::update()
	{
		if (!kMultiSourceFrameReader) {
			return false;
		}

		IMultiSourceFrame* frame = 0;
		HRESULT hr = kMultiSourceFrameReader->AcquireLatestFrame(&frame);

		///----COLOR FRAME
		if (SUCCEEDED(hr)){
			if (DeviceOptions::isEnableColorFrame())
			{
				IColorFrame* pColorFrame = NULL;
				IFrameDescription* colorFrameDescription = NULL;
				INT64 nTime = 0;
				int nWidth = 0;
				int nHeight = 0;
				ColorImageFormat imageFormat = ColorImageFormat_None;
				UINT nBufferSize = 0;
				uint8_t *pBuffer = NULL;

				IColorFrameReference* frameRef = 0;
				hr = frame->get_ColorFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&pColorFrame);
				}
				SafeRelease(frameRef);


				if (SUCCEEDED(hr))
				{
					hr = pColorFrame->get_FrameDescription(&colorFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = colorFrameDescription->get_Width(&nWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = colorFrameDescription->get_Height(&nHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
				}

				if (SUCCEEDED(hr))
				{
					//ColorImageFormat_Bgra
					pBuffer = pixelsData;// new uint32_t[frame_size_color * 4];
					hr = pColorFrame->CopyConvertedFrameDataToArray(BUFFER_SIZE_COLOR, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);

					if (SUCCEEDED(hr)) {
						memcpy(pixelsData, pBuffer, BUFFER_SIZE_COLOR);
						DeviceActivators::colorFrameReady(true);
					}

				}

				SafeRelease(colorFrameDescription);
				SafeRelease(pColorFrame);

			}
			else{
				DeviceActivators::colorFrameReady(false);
			}

		
				
			///---------DEPTH FRAME
			if (DeviceOptions::isEnableDepthFrame())
			{
				IDepthFrame* depthFrame = NULL;

				INT64 nTime = 0;
				IFrameDescription* depthFrameDescription = NULL;
				int nWidth = 0;
				int nHeight = 0;
				USHORT nDepthMinReliableDistance = 0;
				USHORT nDepthMaxReliableDistance = 0;
				UINT nBufferSize = 0;
				UINT16 *pBuffer = NULL;
				IDepthFrameReference* frameRef = 0;

				hr = frame->get_DepthFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&depthFrame);
				}

				SafeRelease(frameRef);

				if (SUCCEEDED(hr))
				{
					hr = depthFrame->get_FrameDescription(&depthFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = depthFrameDescription->get_Width(&nWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = depthFrameDescription->get_Height(&nHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = depthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
				}

				if (SUCCEEDED(hr))
				{
					hr = depthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);
				}

				if (SUCCEEDED(hr))
				{
					
					hr = depthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
					
					if (DeviceOptions::isEnableDepthFrame()){
						if (SUCCEEDED(hr) && pBuffer){

							const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
							int depthIndex = 0;
							while (pBuffer < pBufferEnd)
							{
								USHORT depth = *pBuffer;
								UINT16 intensity = static_cast<BYTE>((depth >= nDepthMinReliableDistance) && (depth <= nDepthMaxReliableDistance) ? (depth % 256) : 0);
								depthRaw_16_Data[depthIndex] = depth;
								depthRaw_256_Data[depthIndex] = intensity;
								depthData[depthIndex] = colorByte2Int((uint32_t)intensity);
								++pBuffer;
								++depthIndex;
							}
							DeviceActivators::depthFrameReady(true);
						}

					}
						
					if (DeviceOptions::isEnablePointCloudDepth() && kCoordinateMapper){
					
						if (SUCCEEDED(hr) && pBuffer){
							hr = kCoordinateMapper->MapDepthFrameToCameraSpace(frame_size_depth, reinterpret_cast<UINT16*>(depthRaw_16_Data), frame_size_depth, mCamaraSpacePointDepth);

							//hr = kCoordinateMapper->MapCameraPointsToColorSpace(frame_size_depth, mCamaraSpacePointDepth, frame_size_depth, mColorSpacePoint);

							if (SUCCEEDED(hr) && mCamaraSpacePointDepth != NULL){
								int pBufferEnd = (frame_size_depth);
								int depthIndex = 0;
								int cameraSpaceIndex = 0;
								int indexColor = 0;

								while (depthIndex < pBufferEnd){
	
									float val = constrain(mCamaraSpacePointDepth[depthIndex].Z, 0.0f, 8.0f);
									//[depthIndex] = val;

									if (val > depthPCLowTh && val < depthPCHighTh){
										pointCloudPosData[cameraSpaceIndex++] = mCamaraSpacePointDepth[depthIndex].X;
										pointCloudPosData[cameraSpaceIndex++] = mCamaraSpacePointDepth[depthIndex].Y;
										pointCloudPosData[cameraSpaceIndex++] = val;

										float colVal = lmap(val, depthPCLowTh, depthPCHighTh, 0.0f, 1.0);
										pointCloudDepthImage[depthIndex] = colorByte2Int(uint32_t(colVal * 255));

										pointCloudDepthNormalized[depthIndex] = colVal;
										//pointCloudDepthImage[depthIndex] = val;
									}
									else{
										pointCloudPosData[cameraSpaceIndex++] = NULL;
										pointCloudPosData[cameraSpaceIndex++] = NULL;
										pointCloudPosData[cameraSpaceIndex++] = NULL;

										pointCloudDepthImage[depthIndex] = colorByte2Int((uint32_t)0);
										//pointCloudDepthImage[depthIndex] = NULL;
									}

									++pBuffer; //unsigned int
									++depthIndex;
								}

								//cout << "min: " << min << " max" << max << std::endl;
								//std::cout << cameraSpaceIndex << std::endl;

								DeviceActivators::depthFrameReady(true);
								DeviceActivators::depthPointCloudPosReady(true);
								DeviceActivators::depthPointCloudImageReady(true);
							}
						}
					}


					//GetDepthFrameToCameraSpaceTable
					if (activateMapDepthToCamaraTable){
						if (kCoordinateMapper){
							PointF* table = nullptr;
							uint32_t count = 0;
							hr = kCoordinateMapper->GetDepthFrameToCameraSpaceTable(&count, &table);
							if (SUCCEEDED(hr)) {
								std::cout << " mapper count" << count << std::endl;
								int tableIndex = 0;
								int mapIndex = 0;
								int pBufferEnd = frame_size_depth;
								while (tableIndex < pBufferEnd){
									mapDepthCameraTableData[mapIndex * 2 + 0] = table[tableIndex].X;
									mapDepthCameraTableData[mapIndex * 2 + 1] = table[tableIndex].Y;
									mapIndex++;
								}
							}
						}
						activateMapDepthToCamaraTable = false;
					}

					//MAP Depth to Color
					if (activateMapDepthToColor){
						if (SUCCEEDED(hr) && pBuffer){
							hr = kCoordinateMapper->MapDepthFrameToColorSpace(frame_size_depth, reinterpret_cast<UINT16*>(depthRaw_16_Data), frame_size_depth, mDepthToColorPoints);

							if (SUCCEEDED(hr)){
								int  pBufferEnd =  (frame_size_depth);
								int depthIndex = 0;
								int cameraSpaceIndex = 0;

								while (cameraSpaceIndex < pBufferEnd){
									mapDepthToColorData[depthIndex++] = mDepthToColorPoints[cameraSpaceIndex].X;
									mapDepthToColorData[depthIndex++] = mDepthToColorPoints[cameraSpaceIndex].Y;

									++cameraSpaceIndex;
								}
							}
						}
						activateMapDepthToColor = false;
					}
				}

				SafeRelease(depthFrameDescription);
				SafeRelease(depthFrame);
			}else{
				DeviceActivators::depthFrameReady(false);
				DeviceActivators::depthPointCloudPosReady(false);
				DeviceActivators::depthPointCloudImageReady(false);
			}
			
			//POINT CLOUDS
			if (DeviceOptions::isEnablePointCloudColor())
			{
				IColorFrame* pColorFrame = NULL;
				IFrameDescription* colorFrameDescription = NULL;
				INT64 nTimeColor = 0;
				int nWidth = 0;
				int nHeight = 0;
				ColorImageFormat imageFormat = ColorImageFormat_None;
				UINT nBufferSizeColor = 0;
				RGBQUAD *pBufferColor = NULL;
				IColorFrameReference* frameColorRef = 0;

				IDepthFrame* depthFrame = NULL;

				INT64 nTimeDepth = 0;
				IFrameDescription* depthFrameDescription = NULL;
				int nWidthDepth = 0;
				int nHeightDepth = 0;
				USHORT nDepthMinReliableDistance = 0;
				USHORT nDepthMaxReliableDistance = 0;
				UINT nBufferSizeDepth = 0;
				UINT16 *pBufferDepth = NULL;
				IDepthFrameReference* frameDepthRef = 0;

				hr = frame->get_ColorFrameReference(&frameColorRef);
				if (SUCCEEDED(hr)) {
					hr = frameColorRef->AcquireFrame(&pColorFrame);
				}
				SafeRelease(frameColorRef);


				hr = frame->get_DepthFrameReference(&frameDepthRef);
				if (SUCCEEDED(hr)) {
					hr = frameDepthRef->AcquireFrame(&depthFrame);
				}
				SafeRelease(frameDepthRef);

				//COLOR
				if (SUCCEEDED(hr))
				{
					hr = pColorFrame->get_FrameDescription(&colorFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					//ColorImageFormat_Bgra  2073600 *4  = 8294400
					pBufferColor = colorChannelsDataTemp;// new uint32_t[frame_size_color * 4];
					hr = pColorFrame->CopyConvertedFrameDataToArray(BUFFER_SIZE_COLOR, reinterpret_cast<BYTE*>(pBufferColor), ColorImageFormat_Bgra);
				}

				//DEPTH
				if (SUCCEEDED(hr))
				{
					hr = depthFrame->get_FrameDescription(&depthFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = depthFrame->AccessUnderlyingBuffer(&nBufferSizeDepth, &pBufferDepth);
				}

				if (SUCCEEDED(hr) && pBufferDepth && pBufferColor && kCoordinateMapper)
				{
					hr = kCoordinateMapper->MapColorFrameToCameraSpace(frame_size_depth, reinterpret_cast<UINT16*>(depthRaw_16_Data), frame_size_color, mCamaraSpacePointColor);

					if (SUCCEEDED(hr) && mCamaraSpacePointColor != NULL){
						int indexColor = 0;
						int pixelColor = 0;
						int cameraSpaceIndex = 0;
						while (indexColor < frame_size_color)
						{  //pixelsData
							RGBQUAD  pSrc = pBufferColor[indexColor];

							pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].X;
							pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].Y;
							pointCloudColorData[cameraSpaceIndex++] = mCamaraSpacePointColor[indexColor].Z;

							colorChannelsData[pixelColor++] = pSrc.rgbRed   * 0.00390625;
							colorChannelsData[pixelColor++] = pSrc.rgbGreen * 0.00390625;
							colorChannelsData[pixelColor++] = pSrc.rgbBlue  * 0.00390625;
							++indexColor;
						}
						DeviceActivators::colorPointCloudFrameReady(true);
					}
				}
				SafeRelease(colorFrameDescription);
				SafeRelease(pColorFrame);

				SafeRelease(depthFrameDescription);
				SafeRelease(depthFrame);
			}
			else{
				DeviceActivators::colorPointCloudFrameReady(false);
			}

			//--------Infrared Exposure
			if (DeviceOptions::isEnableLongExposureInfraredFrame())
			{
				ILongExposureInfraredFrame* infraredLongExposureFrame = 0;
				INT64 nTime = 0;
				IFrameDescription* infraredLongExposureFrameDescription = NULL;
				int nWidth = 0;
				int nHeight = 0;
				USHORT nDepthMinReliableDistance = 0;
				USHORT nDepthMaxReliableDistance = 0;
				UINT nBufferSize = 0;
				UINT16 *pBuffer = NULL;

				ILongExposureInfraredFrameReference* frameRef = 0;

				hr = frame->get_LongExposureInfraredFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&infraredLongExposureFrame);
				}

				SafeRelease(frameRef);

				if (SUCCEEDED(hr))
				{
					hr = infraredLongExposureFrame->get_FrameDescription(&infraredLongExposureFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredLongExposureFrameDescription->get_Width(&nWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredLongExposureFrameDescription->get_Height(&nHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredLongExposureFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
					if (SUCCEEDED(hr) && nWidth == cDepthWidth && nHeight == cDepthHeight && pBuffer){
						//uint32_t * depthFrameDataTemp = depthFrameData;
						const UINT16* pBufferEnd = pBuffer + (frame_size_depth);
						int longExposureIndex = 0;

						while (pBuffer < pBufferEnd)
						{
							USHORT ir = *pBuffer;
							BYTE intensity = static_cast<BYTE>(ir >> 8);
							longExposureData[longExposureIndex] = colorByte2Int((uint32_t)intensity);
							++pBuffer; //unsigned int
							++longExposureIndex;
						}
						//memcpy(depthData, depthFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						DeviceActivators::infraredlongExposureReady(true);
					}
				}
				SafeRelease(infraredLongExposureFrameDescription);
				SafeRelease(infraredLongExposureFrame);
			}else{
				DeviceActivators::infraredlongExposureReady(false);
			}

				

			//------------InFrared
			if (DeviceOptions::isEnableInFraredFrame())
			{
				IInfraredFrame* infraredFrame = 0;
				INT64 nTime = 0;
				IFrameDescription* infraredFrameDescription = NULL;
				int nWidth = 0;
				int nHeight = 0;
				UINT nBufferSize = 0;
				UINT16 *pBuffer = NULL;

				IInfraredFrameReference* frameRef = 0;

				hr = frame->get_InfraredFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&infraredFrame);
				}

				SafeRelease(frameRef);

				if (SUCCEEDED(hr))
				{
					hr = infraredFrame->get_FrameDescription(&infraredFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredFrameDescription->get_Width(&nWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredFrameDescription->get_Height(&nHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = infraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);

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
						DeviceActivators::infraredFrameReady(true);
					}
				}
				SafeRelease(infraredFrameDescription);
				SafeRelease(infraredFrame);
			}
			else{
				DeviceActivators::infraredFrameReady(false);
			}

			//---------- BODY TRACK
			if (DeviceOptions::isEnableBodyIndex())
			{
				IBodyIndexFrame * bodyIndexFrame = 0;
				IFrameDescription* bodyIndexFrameDescription = NULL;
				int nBodyIndexWidth = 0;
				int nBodyIndexHeight = 0;
				UINT nBodyIndexBufferSize = 0;
				BYTE *pBodyIndexBuffer = NULL;

				IBodyIndexFrameReference* frameRef = 0;

				hr = frame->get_BodyIndexFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&bodyIndexFrame);
				}

				SafeRelease(frameRef);

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrame->get_FrameDescription(&bodyIndexFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
				}

				if (SUCCEEDED(hr))
				{
					//BODY MAPPING
					if (SUCCEEDED(hr) && pBodyIndexBuffer != NULL && nBodyIndexWidth == cDepthWidth && nBodyIndexHeight == cDepthHeight)
					{
						const BYTE* pBufferEnd = pBodyIndexBuffer + (frame_size_depth);
						
						int bodyIndex = 0;
						int bodyIndexColor = 0;

						int bodyIndexCount = 0;
						int bodyIndexCountPrev = 0;
						int bodyColor = 0;

						int sizeSortedArray = 0;
						
						std::set<int> tags;

						while (pBodyIndexBuffer < pBufferEnd)
						{
							BYTE ir = *pBodyIndexBuffer;

							//save index
							bodyIndexCountPrev = bodyColor;
							bodyColor = (uint32_t)ir;


							if (bodyColor == 1)
								bodyTackDataUser_1[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_1[bodyIndex] = 0x00000000;

							if (bodyColor == 2)
								bodyTackDataUser_2[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_2[bodyIndex] = 0x00000000;

							if (bodyColor == 3)
								bodyTackDataUser_3[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_3[bodyIndex] = 0x00000000;

							if (bodyColor == 4)
								bodyTackDataUser_4[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_4[bodyIndex] = 0x00000000;

							if (bodyColor == 5)
								bodyTackDataUser_5[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_5[bodyIndex] = 0x00000000;

							if (bodyColor == 6)
								bodyTackDataUser_6[bodyIndex] = bodyColors[bodyColor];
							else
								bodyTackDataUser_6[bodyIndex] = 0x00000000;

							/*
							//save bodycolor
							if (bodyIndex % 10 == 0){
								if (bodyColor > 0 && bodyColor <= 6){
									if (bodyIndexCountPrev != bodyColor){
										if (sizeSortedArray < numberUsers){
											tags.insert(bodyColor);
											std::set<int> arraySort(tags.begin(), tags.end());
											sizeSortedArray = arraySort.size();


											int i = 0;
											for (auto it = arraySort.begin(); it != arraySort.end(); ++it){
												int at = *it;
												std::cout << " index: " << i << " value: " << at;
												i++;
											}
											std::cout << "-" << tags.size() << std::endl;

										}
										else{
											bodyColor = 6;
										}
									}
								}
							}
							*/
						//	tags.clear();
							
							bodyIndexData[bodyIndex] = bodyColors[bodyColor];// [bodyCounter];// colorByte2Int((uint32_t)intensity);
							++pBodyIndexBuffer; //(unsigned int)
							++bodyIndex;
						}
						//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
						DeviceActivators::bodyIndexReady(true);
					}
				}
				SafeRelease(bodyIndexFrameDescription);
				SafeRelease(bodyIndexFrame);
			}
			else{
				DeviceActivators::bodyIndexReady(false);
			}


			if (DeviceOptions::isEnableCoordinateMappingColor())
			{

				IDepthFrame* pDepthFrame = NULL;
				IColorFrame* pColorFrame = NULL;
				IBodyIndexFrame* pBodyIndexFrame = NULL;

				if (SUCCEEDED(hr))
				{
					IDepthFrameReference* pDepthFrameReference = NULL;
					hr = frame->get_DepthFrameReference(&pDepthFrameReference);
					if (SUCCEEDED(hr))
					{
						hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
					}

					SafeRelease(pDepthFrameReference);
				}

				if (SUCCEEDED(hr))
				{
					IColorFrameReference* pColorFrameReference = NULL;
					hr = frame->get_ColorFrameReference(&pColorFrameReference);
					if (SUCCEEDED(hr))
					{
						hr = pColorFrameReference->AcquireFrame(&pColorFrame);
					}

					SafeRelease(pColorFrameReference);
				}

				if (SUCCEEDED(hr))
				{
					IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;

					hr = frame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
					if (SUCCEEDED(hr))
					{
						hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
					}

					SafeRelease(pBodyIndexFrameReference);
				}

				if (SUCCEEDED(hr))
				{
					IFrameDescription* pDepthFrameDescription = NULL;
					int nDepthWidth = 0;
					int nDepthHeight = 0;
					UINT16 *pDepthBuffer = NULL;
					UINT nDepthBufferSize = 0;

					IFrameDescription* pColorFrameDescription = NULL;
					int nColorWidth = 0;
					int nColorHeight = 0;
					ColorImageFormat imageFormat = ColorImageFormat_None;
					uint8_t *pColorBuffer = NULL;

					IFrameDescription* pBodyIndexFrameDescription = NULL;
					int nBodyIndexWidth = 0;
					int nBodyIndexHeight = 0;
					BYTE *pBodyIndexBuffer = NULL;
					UINT nBodyIndexBufferSize = 0;

					// get depth frame data
					if (SUCCEEDED(hr))
					{
						hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
					}

					if (SUCCEEDED(hr))
					{
						hr = pDepthFrameDescription->get_Width(&nDepthWidth);
					}

					if (SUCCEEDED(hr))
					{
						hr = pDepthFrameDescription->get_Height(&nDepthHeight);
					}

					if (SUCCEEDED(hr))
					{
						hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
					}

					// get color frame data
					if (SUCCEEDED(hr))
					{
						hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
					}

					if (SUCCEEDED(hr))
					{
						hr = pColorFrameDescription->get_Width(&nColorWidth);
					}

					if (SUCCEEDED(hr))
					{
						hr = pColorFrameDescription->get_Height(&nColorHeight);
					}

					if (SUCCEEDED(hr))
					{
						hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
					}

					if (SUCCEEDED(hr))
					{
						pColorBuffer = pixelsDataTemp;
						hr = pColorFrame->CopyConvertedFrameDataToArray(BUFFER_SIZE_COLOR, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra);
					}

					// get body index frame data
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

					HRESULT hr = kCoordinateMapper->MapColorFrameToDepthSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nColorWidth * nColorHeight, mDepthCoordinates);
					if (SUCCEEDED(hr))
					{
						// loop over output pixels
						int colorIndex = 0;
						int colorIndeRGB = 0;
						while (colorIndex < frame_size_color)
						{
							// default setting source to copy from the background pixel
							const uint8_t* pSrc = backgroundRGBX + colorIndeRGB;

							DepthSpacePoint p = mDepthCoordinates[colorIndex];

							// Values that are negative infinity means it is an invalid color to depth mapping so we
							// skip processing for this pixel
							if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
							{
								int depthX = static_cast<int>(p.X + 0.5f);
								int depthY = static_cast<int>(p.Y + 0.5f);

								if ((depthX >= 0 && depthX < nDepthWidth) && (depthY >= 0 && depthY < nDepthHeight))
								{
									BYTE player = pBodyIndexBuffer[depthX + (depthY * cDepthWidth)];

									// if we're tracking a player for the current pixel, draw from the color camera
									if (player != 0xff)
									{
										// set source for copy to the color pixel
										pSrc = pColorBuffer + colorIndeRGB;
									}
								}
							}

							// write output
							outCoordMapperRGBX[colorIndeRGB++] = *(pSrc++);
							outCoordMapperRGBX[colorIndeRGB++] = *(pSrc++);
							outCoordMapperRGBX[colorIndeRGB++] = *(pSrc++);
							outCoordMapperRGBX[colorIndeRGB++] = *(pSrc++);
							colorIndex++;
						}
						DeviceActivators::coordinateRGBXReady(false);
					}
					else{
						DeviceActivators::coordinateRGBXReady(false);
					}
					SafeRelease(pDepthFrameDescription);
					SafeRelease(pColorFrameDescription);
					SafeRelease(pBodyIndexFrameDescription);
				}

				SafeRelease(pDepthFrame);
				SafeRelease(pColorFrame);
				SafeRelease(pBodyIndexFrame);
			}
			//---------- DEPTH MASK WITH DEPTH
			if (DeviceOptions::isEnableDepthMaskFrame())
			{
				IBodyIndexFrame * bodyIndexFrame = 0;
				IFrameDescription* bodyIndexFrameDescription = NULL;
				int nBodyIndexWidth = 0;
				int nBodyIndexHeight = 0;
				UINT nBodyIndexBufferSize = 0;
				BYTE *pBodyIndexBuffer = NULL;

				IBodyIndexFrameReference* frameRef = 0;

				hr = frame->get_BodyIndexFrameReference(&frameRef);

				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&bodyIndexFrame);
				}

				SafeRelease(frameRef);

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrame->get_FrameDescription(&bodyIndexFrameDescription);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
				}

				if (SUCCEEDED(hr))
				{
					hr = bodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
				}

				if (SUCCEEDED(hr) && pBodyIndexBuffer != NULL && nBodyIndexWidth == cDepthWidth && nBodyIndexHeight == cDepthHeight)
				{
					const BYTE* pBufferEnd = pBodyIndexBuffer + (frame_size_depth);
					int depthMaskIndex = 0;

					while (pBodyIndexBuffer < pBufferEnd)
					{
						BYTE ir = int(*pBodyIndexBuffer);
					
						uint32_t intensity;
						if (ir > 0 && ir <= 6)
							intensity = bodyColors[ir];
						else
							intensity = depthData[depthMaskIndex];

						depthMaskData[depthMaskIndex] = intensity;
						++pBodyIndexBuffer; //(unsigned int)
						++depthMaskIndex;
					}
					//memcpy(infraredData, inFraredFrameDataTemp, frame_size_depth * sizeof(uint32_t));
					DeviceActivators::bodyIndexDephReady(true);
				}

				SafeRelease(bodyIndexFrameDescription);
				SafeRelease(bodyIndexFrame);
			}
			else{
				DeviceActivators::bodyIndexDephReady(false);
			}

				
			if (DeviceOptions::isEnableFaceDetection())
			{
				IBodyFrame * bodyFrame = 0;
				IBodyFrameReference * frameRefBody = nullptr;
				IBody* ppBodies[BODY_COUNT] = { 0 };

				hr = frame->get_BodyFrameReference(&frameRefBody);
				if (SUCCEEDED(hr)) {
					hr = frameRefBody->AcquireFrame(&bodyFrame);
				}
				SafeRelease(frameRefBody);

				if (SUCCEEDED(hr)) {
					hr = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
				}
				bool bHaveBodyData = SUCCEEDED(hr);

				// iterate through each face reader
				for (int iFace = 0; iFace < BODY_COUNT; ++iFace)
				{
					// retrieve the latest face frame from this reader
					IFaceFrame* pFaceFrame = nullptr;
					HRESULT hr = kFaceFrameReaders[iFace]->AcquireLatestFrame(&pFaceFrame);
					BOOLEAN bFaceTracked = false;

					IBodyFrameReference* frameRef = 0;
					hr = frame->get_BodyFrameReference(&frameRef);


					if (SUCCEEDED(hr) && nullptr != pFaceFrame)
					{
						// check if a valid face is tracked in this face frame
						hr = pFaceFrame->get_IsTrackingIdValid(&bFaceTracked);
					}

					if (SUCCEEDED(hr))
					{
						if (bFaceTracked)
						{
							//faceDetectionReady = false;
							IFaceFrameResult* pFaceFrameResult = nullptr;
							RectI faceBoxColor = { 0 };
							RectI faceBoxInFrared = { 0 };
							PointF facePointsColor[FacePointType::FacePointType_Count];
							PointF facePointsInFrared[FacePointType::FacePointType_Count];
							Vector4 faceRotation;
							DetectionResult faceProperties[FaceProperty::FaceProperty_Count];

							hr = pFaceFrame->get_FaceFrameResult(&pFaceFrameResult);

							// need to verify if pFaceFrameResult contains data before trying to access it
							if (SUCCEEDED(hr) && pFaceFrameResult != nullptr)
							{
								hr = pFaceFrameResult->get_FaceBoundingBoxInColorSpace(&faceBoxColor);
								hr = pFaceFrameResult->get_FaceBoundingBoxInInfraredSpace(&faceBoxInFrared);

								if (SUCCEEDED(hr))
								{
									hr = pFaceFrameResult->GetFacePointsInColorSpace(FacePointType::FacePointType_Count, facePointsColor);
									hr = pFaceFrameResult->GetFacePointsInInfraredSpace(FacePointType::FacePointType_Count, facePointsInFrared);
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
									faceDataColor[iFace * 36 + 35] = 1.0;
									faceDataInInfrared[iFace * 36 + 35] = 1.0;

									for (int j = 0; j < FacePointType::FacePointType_Count; j++){
										faceDataColor[iFace * 36 + j * 2 + 0] = facePointsColor[j].X;
										faceDataColor[iFace * 36 + j * 2 + 1] = facePointsColor[j].Y;
										//cout << facePoints[j].X << " " << facePoints[j].Y << std::endl;
									}

									for (int j = 0; j < FacePointType::FacePointType_Count; j++){
										faceDataInInfrared[iFace * 36 + j * 2  + 0] = facePointsInFrared[j].X;
										faceDataInInfrared[iFace * 36 + j * 2  + 1] = facePointsInFrared[j].Y;
										//cout << facePoints[j].X << " " << facePoints[j].Y << std::endl;
									}

									//OPTIMIZE PRODUCT INDEX
									int index = iFace * 36 + 20;

									faceDataColor[index + 0] = faceBoxColor.Left;
									faceDataColor[index + 1] = faceBoxColor.Top;
									faceDataColor[index + 2] = faceBoxColor.Right - faceBoxColor.Left;
									faceDataColor[index + 3] = faceBoxColor.Bottom - faceBoxColor.Top;

									faceDataInInfrared[index + 0] = faceBoxInFrared.Left;
									faceDataInInfrared[index + 1] = faceBoxInFrared.Top;
									faceDataInInfrared[index + 2] = faceBoxInFrared.Right - faceBoxInFrared.Left;
									faceDataInInfrared[index + 3] = faceBoxInFrared.Bottom - faceBoxInFrared.Top;

									int pitch, yaw, roll;
									ExtractFaceRotationInDegrees(&faceRotation, &pitch, &yaw, &roll);

									faceDataColor[index + 4] = pitch;
									faceDataColor[index + 5] = yaw;
									faceDataColor[index + 6] = roll;

									faceDataInInfrared[index + 4] = pitch;
									faceDataInInfrared[index + 5] = yaw;
									faceDataInInfrared[index + 6] = roll;

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
											faceDataColor[index + iPro] = -1;
											faceDataInInfrared[index + iPro] = -1;
											break;
										case DetectionResult::DetectionResult_Yes:
											faceDataColor[index + iPro] = 1;
											faceDataInInfrared[index + iPro] = 1;
											break;
										case DetectionResult::DetectionResult_No:
										case DetectionResult::DetectionResult_Maybe:
											faceDataColor[index + iPro] = 0;
											faceDataInInfrared[index + iPro] = 0;
											break;
										}
									}

								}
							}

							SafeRelease(pFaceFrameResult);
						}
						else
						{
							faceDataColor[iFace * 36 + 35] = 0.0;
							faceDataInInfrared[iFace * 36 + 35] = 0.0;
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
									}
								}
								//faceDetectionReady = false;
							}
						}
						SafeRelease(pFaceFrame);
					}
					DeviceActivators::faceDetectionReady(true);
				}

				SafeRelease(bodyFrame);
			}

			//ENABLE SKELETON
			if (DeviceOptions::isEnableSkeleton())
			{

				IBodyFrame * bodyFrame = 0;
				IBodyFrameReference * frameRef = nullptr;
	
				hr = frame->get_BodyFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&bodyFrame);
				}
				SafeRelease(frameRef);

				if (SUCCEEDED(hr)) 
				{
					IBody* ppBodies[BODY_COUNT] = { 0 };
					int64_t bodyTime = 0L;
					hr = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
				
					if (SUCCEEDED(hr))
					{
						for (int i = 0; i < BODY_COUNT; ++i){
							IBody* pBody = ppBodies[i];
							if (pBody) //[6][25][3]
							{
								BOOLEAN bTracked = false;
								hr = pBody->get_IsTracked(&bTracked);

								if (SUCCEEDED(hr) && bTracked)
								{
									int indexID = i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1;
									skeletonDataDepthMap[indexID] = 1.0;
									skeletonDataColorMap[indexID] = 1.0;
									skeletonData3dMap[indexID] = 1.0;

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
											//cout << "starting loop skeleton" << std::endl;
											for (int j = 0; j < _countof(jointsTracked); ++j)
											{
												int index = i*(JointType_Count + 1) * 9 + j * 9;

												//DEPTH
												float * pointScreenDepth = BodyToScreenDepth(&jointsTracked[j].Position);
												skeletonDataDepthMap[index + 0] = pointScreenDepth[0];
												skeletonDataDepthMap[index + 1] = pointScreenDepth[1];
												skeletonDataDepthMap[index + 2] = 0.0;

												//COLOR
												float * pointScreenColor = BodyToScreenColor(&jointsTracked[j].Position);
												skeletonDataColorMap[index + 0] = pointScreenColor[0];
												skeletonDataColorMap[index + 1] = pointScreenColor[1];
												skeletonDataColorMap[index + 2] = 0.0;

												//3D
												skeletonData3dMap[index + 0] = jointsTracked[j].Position.X;
												skeletonData3dMap[index + 1] = jointsTracked[j].Position.Y;
												skeletonData3dMap[index + 2] = jointsTracked[j].Position.Z;
												skeletonData3dMap[index + 3] = jointOrientations[j].Orientation.w;
												skeletonData3dMap[index + 4] = jointOrientations[j].Orientation.x;
												skeletonData3dMap[index + 5] = jointOrientations[j].Orientation.y;
												skeletonData3dMap[index + 6] = jointOrientations[j].Orientation.z;

												//HAND STATE
												if (jointsTracked[j].JointType == JointType_HandLeft){
													skeletonDataDepthMap[index + 7] = skeletonData3dMap[index + 7] = skeletonDataColorMap[index + 7] = leftHandState;
												}
												else if (jointsTracked[j].JointType == JointType_HandRight){
													skeletonDataDepthMap[index + 7] = skeletonData3dMap[index + 7] = skeletonDataColorMap[index + 7] = rightHandState;
												}
												else{
													skeletonDataDepthMap[index + 7] = skeletonData3dMap[index + 7] = skeletonDataColorMap[index + 7] = jointsTracked[j].TrackingState;
												}

												skeletonDataDepthMap[index + 8] = skeletonData3dMap[index + 8] = skeletonDataColorMap[index + 8] = jointsTracked[j].JointType;
												//std::cout << "copy skeleton loop" << std::endl;
											}
										}
									}
								}else{
									int index = i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1;
									skeletonDataDepthMap[index] = -1.0;
									skeletonDataColorMap[index] = -1.0;
									skeletonData3dMap[index] = -1.0;
								}
							}
							else{
								int index = i*(JointType_Count + 1) * 9 + (JointType_Count + 1) * 9 - 1;
								skeletonDataDepthMap[index] = -1.0;
								skeletonDataColorMap[index] = -1.0;
								skeletonData3dMap[index] = -1.0;
							}
						}
					}

					for (int i = 0; i < _countof(ppBodies); ++i){
						SafeRelease(ppBodies[i]);
					}

					DeviceActivators::skeletonReady(true);
				}

				SafeRelease(bodyFrame);
			}


			//HD FACE TRACKING
			if (DeviceOptions::isEnableHDFaceDetection())
			{
				// Body Frame
				IBodyFrame * pBodyFrame = 0;
				IBodyFrameReference * frameRef = nullptr;

				hr = frame->get_BodyFrameReference(&frameRef);
				if (SUCCEEDED(hr)) {
					hr = frameRef->AcquireFrame(&pBodyFrame);
				}
				SafeRelease(frameRef);

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
					for (int count = 0; count < BODY_COUNT; count++){
						SafeRelease(pBody[count]);
					}
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
					SafeRelease(pHDFaceFrame);
				}
				DeviceActivators::HDFaceDetectionReady(true);
			}
		

		//END OF SUCCED HR MULTIFRAME
		}

		SafeRelease(frame);

		return true;
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

	
	float * Device::BodyToScreenDepth(const CameraSpacePoint * bodyPoint)
	{
		float pointsScreen[2];
		// Calculate the body's position on the screen
		DepthSpacePoint depthPoint = { 0 };
		kCoordinateMapper->MapCameraPointToDepthSpace(*bodyPoint, &depthPoint);

		pointsScreen[0] = depthPoint.X;
		pointsScreen[1] = depthPoint.Y;
		return pointsScreen;
	}

	float * Device::BodyToScreenColor(const CameraSpacePoint * bodyPoint)
	{
		float pointsScreen[2];
		// Calculate the body's position on the screen
		ColorSpacePoint colorPoint = { 0 };

		kCoordinateMapper->MapCameraPointToColorSpace(*bodyPoint, &colorPoint);

		pointsScreen[0] = colorPoint.X;
		pointsScreen[1] = colorPoint.Y;
		return pointsScreen;
	}

	int Device::colorByte2Int(int gray){
		gray = gray & 0xffff;
		return 0xff000000 | (gray << 16) | (gray << 8) | gray;
	}



	HRESULT Device::UpdateBodyData(IBody** ppBodies, IMultiSourceFrame* frame)
	{
		IBodyFrame * bodyFrame = 0;
		IBodyFrameReference * frameRef = nullptr;

		HRESULT hr = frame->get_BodyFrameReference(&frameRef);
		if (SUCCEEDED(hr)) {
			hr = frameRef->AcquireFrame(&bodyFrame);
		}
		SafeRelease(frameRef);

		if (SUCCEEDED(hr)) {
			IBody* ppBodies[BODY_COUNT] = { 0 };
			int64_t bodyTime = 0L;
			hr = bodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBodies);
		}
		return hr;
	}

	void Device::ExtractFaceRotationInDegrees(const Vector4* pQuaternion, int* pPitch, int* pYaw, int* pRoll)
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

		// clamp rotation values in degrees to a specified range of values to control the refresh rate
		double increment = c_FaceRotationIncrementInDegrees;
		*pPitch = static_cast<int>((dPitch + increment / 2.0 * (dPitch > 0 ? 1.0 : -1.0)) / increment) * static_cast<int>(increment);
		*pYaw = static_cast<int>((dYaw + increment / 2.0 * (dYaw > 0 ? 1.0 : -1.0)) / increment) * static_cast<int>(increment);
		*pRoll = static_cast<int>((dRoll + increment / 2.0 * (dRoll > 0 ? 1.0 : -1.0)) / increment) * static_cast<int>(increment);
	}

	uint32_t  Device::getColorMaskUser(BYTE ir)
	{
		uint32_t intensity = 0xFFFFFF;

		if (ir == 0)
			return 0x0000ff;
		else if (ir == 1)
			return 0x00ff00;
		else if (ir == 2)
			return 0xff0000;
		else if (ir == 3)
			return 0xffff00;
		else if (ir == 4)
			return 0xff00ff;
		else if (ir == 5)
			return 0x00ffff;
		else
			return 0xFFFFFF;

		return intensity;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	uint8_t * Device::JNI_GetImage()
	{
		return pixelsData;
	}

	float * Device::JNI_GetColorChannels()
	{
		return colorChannelsData;
	}

	uint32_t * Device::JNI_GetDepth()
	{
		return depthData;
	}

	uint32_t * Device::JNI_GetDepthMask()
	{
		return depthMaskData;
	}

	uint16_t * Device::JNI_GetDepthRaw_16_Data()
	{
		return depthRaw_16_Data;
	}

	uint16_t * Device::JNI_GetDepthRaw_256_Data()
	{
		return depthRaw_256_Data;
	}

	uint32_t * Device::JNI_GetDepthSha()
	{
		uint32_t * newInt = (uint32_t *)depthData;
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
		return bodyIndexData;
	}

	uint8_t * Device::JNI_GetCoodinateRGBX()
	{
		return outCoordMapperRGBX;
	}

	float * Device::JNI_getSkeletonDataDepthMap()
	{
		return skeletonDataDepthMap;
	}

	float * Device::JNI_getSkeletonData3dMap()
	{
		return skeletonData3dMap;
	}

	float * Device::JNI_getSkeletonDataColorMap()
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

	float * Device::JNI_pointCloudDepthNormalized()
	{
		return pointCloudDepthNormalized;
	}

	float * Device::JNI_getFaceColorRawData()
	{
		return faceDataColor;
	}

	float * Device::JNI_getFaceInfraredRawData()
	{
		return faceDataInInfrared;
	}

	float * Device::JNI_getHDFaceVertexRawData()
	{
		return hdFaceVertex;
	}

	float * Device::JNI_getMapDepthToCameraTable()
	{
		return mapDepthCameraTableData;
	}


	float *	Device::JNI_getMapDepthToColor()
	{
		return mapDepthToColorData;
	}

	uint32_t *	Device::JNI_getBodyIndexUser(int index)
	{
		if (index > 6 && index < 1)
			index = 1;
		switch (index){
		case 1:
			return bodyTackDataUser_1;
			break;
		case 2:
			return bodyTackDataUser_2;
			break;
		case 3:
			return bodyTackDataUser_3;
			break;
		case 4:
			return bodyTackDataUser_4;
			break;
		case 5:
			return bodyTackDataUser_5;
			break;
		case 6:
			return bodyTackDataUser_6;
			break;
		}
		return bodyTackDataUser_1;
	}
}