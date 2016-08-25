/*
KinectPV2.0 library for Processing
Copyright (c) 2014 Thomas Sanchez Lengeling

* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:st
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
*

KinectPV2.0 library  library for Processing is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KinectPV2.0 library for Processing is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KinectPV2.0 library for Processing.  If not, see
<http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <stdint.h>
#include <exception>
#include <comutil.h>
#include <iostream>
#include <memory>
#include <vector>

#include <thread>
#include <mutex>

#include "ole2.h"


#include "Kinect.h"
#include "Kinect.Face.h"
#include "DeviceOptions.h"
#include "DeviceActivators.h"

#define M_PI 3.14159265358979323846

#define VERSION			"0.7.6"

static const int         cColorWidth = 1920;
static const int         cColorHeight = 1080;

static const int         cDepthWidth = 512;
static const int         cDepthHeight = 424;

#define frame_size_color 2073600
#define frame_size_depth 217088

static const int BUFFER_SIZE_COLOR = frame_size_color * 4;

//SKELETON
static const int JOINTSIZE = BODY_COUNT * (JointType_Count + 1) * 9;

//HD FACE
static const int HDFACEVERTEX = BODY_COUNT * 1347 * 2 + BODY_COUNT;

//FACE DETECTION
static const int FACESIZE = BODY_COUNT * (36);

//////////////////////////////////////////////////////////////////////////////////////////////
namespace KinectPV2{
	class Device : public DeviceOptions, DeviceActivators
	{
	private:
		//color data
		uint8_t *	  pixelsData;
		uint8_t *	  pixelsDataTemp;
		uint32_t *    colorFrameData;

		float     *	  colorChannelsData;
		RGBQUAD   *	  colorChannelsDataTemp;

		//Depth data 
		uint32_t *	  depth_16_Data;  //depth from 16 bits color
		uint32_t *	  depth_256_Data; //depth 256 color

		//uint8_t -> 16 int java 
		uint32_t *	  depthRaw_16_Data;
		uint32_t *	  depthRaw_256_Data;

		uint16_t *    depthRaw_16_temp;

		uint32_t *	  depthMaskData;


		float    *	 pointCloudPosData;
		float    *   pointCloudColorData;

		uint32_t *   pointCloudDepthImage;

		float	 *   pointCloudRawImage;

		float	 *   colorCameraPos;

		uint32_t *	 infraredData;
		uint32_t *	 infraredLongExposureData;

		float    *   skeletonData3dMap;
		float    *   skeletonDataDepthMap;
		float    *   skeletonDataColorMap;

		//FACE
		float	 *   faceColorData;
		float    *   faceInfraredData;

		//Color - depth mappers
		float	 *   mapDepthToColorData;

		//HD FACE
		float	 *   hdFaceDeformations;
		float    *   hdFaceVertex;
		UINT32		 hdFaceVertexCount;

		//track data
		uint32_t *   bodyTrackData;

		uint32_t *   bodyTrackRaw;

		uint32_t *	 bodyTrackIds;

		//get independent bodytrackData
		uint32_t *	 bodyTackDataUser_1;
		uint32_t *	 bodyTackDataUser_2;
		uint32_t *	 bodyTackDataUser_3;
		uint32_t *	 bodyTackDataUser_4;
		uint32_t *	 bodyTackDataUser_5;
		uint32_t *	 bodyTackDataUser_6;

		int			appWidth;
		int         appHeight;

		bool		mirror;

	protected:
		IColorFrameReader				 *	kColorFrameReader;
		IDepthFrameReader				 *	kDepthFrameReader;
		ILongExposureInfraredFrameReader *  kLongExposureFrameReader;
		IInfraredFrameReader			 *	kInfraredFrameReader;

		IBodyFrameReader                 *	kBodyFrameReader;
		IBodyIndexFrameReader			 *	kBodyIndexFrameReader;
		ICoordinateMapper	             *	kCoordinateMapper;

		// FACE READER
		IFaceFrameSource				*	kFaceFrameSources[BODY_COUNT];
		IFaceFrameReader				*	kFaceFrameReaders[BODY_COUNT];


		//HD FACE READER
		IHighDefinitionFaceFrameSource  *	kHDFaceSource[BODY_COUNT];
		IHighDefinitionFaceFrameReader  *   kHDFaceReader[BODY_COUNT];
		IFaceAlignment					*   kFaceAlignment[BODY_COUNT];
		IFaceModel						*   kFaceModel[BODY_COUNT];
		IFaceModelBuilder				*   kFaceModelBuilder[BODY_COUNT];
		bool								produce[BODY_COUNT];

		IKinectSensor*				kSensor;

		CameraSpacePoint				*   mCamaraSpacePointDepth;
		CameraSpacePoint				*   mCamaraSpacePointColor;
		ColorSpacePoint					*	mColorSpacePoint;
		DepthSpacePoint					*   mDepthCoordinates;
		ColorSpacePoint					*	mDepthToColorPoints;

		float						depthPCLowTh;
		float						depthPCHighTh;

		std::thread					mThreadDepth;
		std::thread					mThreadColor;
		std::thread					mThreadInfrared;
		std::thread					mThreadInfraredLongExposure;
		std::thread					mThreadSkeleton;
		std::thread					mThreadBodyTrack;
		std::thread					mThreadHDFace;
		std::thread					mThreadCoodinateMapper;

	public:
		Device(void);
		~Device(void);

		bool	init();

		//STOP FUNCTIONS
		void	disable();
		bool    cleanMemory();


		bool	update();

		void	enableMirror(bool enableMirror){ mirror = enableMirror; }

		void	setWindowSize(int appWidth, int appHeight);


		//thread Process
		void			colorProcess();
		void			depthProcess();
		void			infraredProcess();
		void			infraredLongExposureProcess();
		void			skeletonProcess();
		void			bodyTrackProcess();
		void			hdFaceProcess();
		void			coordinateMapperProcess();

		//-----JNI
		uint32_t  *						JNI_GetImage();

		uint32_t *						JNI_GetDepth_16_Data();
		uint32_t *						JNI_GetDepth_256_Data();

		uint32_t *						JNI_GetDepthRaw_16_Data();
		uint32_t *						JNI_GetDepthRaw_256_Data();

		uint32_t *						JNI_GetDepthSha();
		uint32_t *						JNI_GetInfrared();
		uint32_t *						JNI_GetLongExposureInfrared();

		uint32_t *						JNI_GetDepthMask();


		uint32_t *						JNI_GetBodyTrack();
		uint32_t *						JNI_GetRawBodyTrack();

		//FACE
		float *						    JNI_getFaceColorRawData();
		float *							JNI_getFaceInfraredRawData();
		float *							JNI_getHDFaceVertexRawData();


		float *							JNI_pointCloudPosData();

		uint32_t   *					JNI_pointCloudDepthImage();

		float *							JNI_pointCloudColorData();

		float *							JNI_getSkeletonDepthMapData();
		float *							JNI_getSkeleton3DMapData();
		float *							JNI_getSkeletonColorMapData();

		//mappers
		float *							JNI_getColorChannel();
		float *							JNI_getMapDepthToColor();

		//USERS
		uint32_t *						JNI_getBodyIndexUser(int index);

		std::string						JNI_version() { return VERSION; }

		int								JNI_getNumOfUsers();
		uint32_t *						JNI_getTrackedIds();


		void							setSkeletonType(int val){ skeletonMapType = val; }

		//HELP FUNTIONS
		float *							BodyToScreenColor(const CameraSpacePoint& bodyPoint);
		float *							BodyToScreenDepth(const CameraSpacePoint& bodyPoint);

		int								colorByte2Int(int gray);
		int								colorFloat2Int(float gray);
		HRESULT							UpdateBodyData(IBody** ppBodies);
		static void						ExtractRotationInDegrees(const Vector4* pQuaternion, float & pPitch, float & pYaw, float & pRoll);
		float							lmap(float val, float inMin, float inMax, float outMin, float outMax);

		float							constrain(float val, float min, float max);

		bool							infraredFrameReady;
		bool							colorFrameReady;
		bool							depthFrameReady;
		bool							bodyIndexReady;
		bool							longExposureReady;
		bool							depthMaskReady;

		bool							depthPointCloudFrameReady;
		bool							depthPointCloudImageReady;
		bool							colorPointCloudFrameReady;

		bool							skeleton3dReady;
		bool							skeletonDepthReady;
		bool							skeletonColorReady;

		int								skeletonMapType;

		bool							faceDetectionReady;

		void							setLowThresholdDepthPC(int val){ depthPCLowTh = val; }
		int								getLowThresholdDepthPC(){ return depthPCLowTh; }


		void							setHighThresholdDepthPC(int val){ depthPCHighTh = val; }
		int								getHighThresholdDepthPC(){ return depthPCHighTh; }


		int								numberUsers;
		void							setNumberOfUsers(int num){ if (num > 6) num = 6; if (num < 1) num = 1; numberUsers = num; }


		HRESULT  							MapCameraPointToDepthSpace(CameraSpacePoint cameraPoint, DepthSpacePoint *depthPoint){
			return kCoordinateMapper->MapCameraPointToDepthSpace(cameraPoint, depthPoint);
		}


		HRESULT  							MapCameraPointToColorSpace(CameraSpacePoint cameraPoint, ColorSpacePoint *depthPoint){
			return kCoordinateMapper->MapCameraPointToColorSpace(cameraPoint, depthPoint);
		}

		//
		//uint32_t						depthJNI[frame_size_depth];
	};

}