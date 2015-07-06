/*
KinectV2.0 library for Processing
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

KinectfV2.0 library  library for Processing is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KinectfV2.0 library for Processing is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KinectfV2.0 library for Processing.  If not, see
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
#include <algorithm>
#include <set>

#include "ole2.h"

#include "Kinect.h"
#include "Kinect.Face.h"
#include "DeviceActivators.h"
#include "DeviceOptions.h"

#define M_PI 3.14159265358979323846

#define VERSION			"0.7.3"

static const int         cColorWidth = 1920;
static const int         cColorHeight = 1080;

static const int         cDepthWidth = 512;
static const int         cDepthHeight = 424;

#define frame_size_color  2073600

#define frame_size_depth  217088

static const int BUFFER_SIZE_COLOR = frame_size_color * 4;

//SKELETON
static const int JOINTSIZE = BODY_COUNT * (JointType_Count + 1) * 9;

static const int HDFACEVERTEX = BODY_COUNT * 1347 * 2 + BODY_COUNT;

//FACE DETECTION
static const int FACESIZE = BODY_COUNT * (36);

//////////////////////////////////////////////////////////////////////////////////////////////
namespace KinectPV2{

	class Device : public DeviceOptions, public DeviceActivators
	{
	private:
		uint8_t *	 pixelsData;
		uint8_t *    pixelsDataTemp;

		float     *	 colorChannelsData;
		RGBQUAD   *	 colorChannelsDataTemp;

		uint8_t	*	 outCoordMapperRGBX;

		uint32_t *	 depthData;
		uint32_t *   depthMaskData;

		uint16_t  *   depthRaw_16_Data; //rawData
		uint16_t *   depthRaw_256_Data;

		float    *	 pointCloudPosData;
		float    *   pointCloudColorData;

		uint32_t *   pointCloudDepthImage;
		float    *   pointCloudDepthNormalized;

		float	 *   pointCloudRawImage;

		float	 *   colorCameraPos;

		uint32_t *	 infraredData;
		uint32_t *	 longExposureData;

		float    *   skeletonData3dMap;
		float    *   skeletonDataDepthMap;
		float    *   skeletonDataColorMap;

		//FACE
		float	 *   faceDataColor;
		float    *   faceDataInInfrared;

		//HD FACE
		float	 *   hdFaceDeformations;
		float    *   hdFaceVertex;
		UINT32		 hdFaceVertexCount;

		//MAPS
		float    *   mapDepthCameraTableData;
		float	 *   mapDepthToColorData;
		float	 *   mapColorToDepthData;

		uint32_t *   bodyIndexData;
		//get independent bodytrackData

		uint32_t *	 bodyTackDataUser_1;
		uint32_t *	 bodyTackDataUser_2;
		uint32_t *	 bodyTackDataUser_3;
		uint32_t *	 bodyTackDataUser_4;
		uint32_t *	 bodyTackDataUser_5;
		uint32_t *	 bodyTackDataUser_6;

		uint32_t *   bodyTrackingIndex;

		int			appWidth;
		int         appHeight;

		bool		mirror;


	//	uint32_t	frame_size_depth;
	protected:

		IKinectSensor					*	kSensor;

		IMultiSourceFrameReader			*	kMultiSourceFrameReader;

		ICoordinateMapper	            *	kCoordinateMapper;
		//IBodyFrameReader				*   kBodyFrameReader;

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

		CameraSpacePoint				*   mCamaraSpacePointDepth;
		CameraSpacePoint				*   mCamaraSpacePointColor;
		ColorSpacePoint					*	mColorSpacePoint;
		DepthSpacePoint					*   mDepthCoordinates;
		ColorSpacePoint					*	mDepthToColorPoints;

		float								depthPCLowTh;
		float								depthPCHighTh;

		bool								activateMapDepthToCamaraTable;
		bool								activateMapDepthToColor;

	public:
		Device(void);
		~Device(void);

		bool	init();
		void	stop();
		bool	update();

		void	enableMirror(bool enableMirror){ mirror = enableMirror;}

		void	setWindowSize(int appWidth, int appHeight);



		uint8_t *	 backgroundRGBX;

		//-----JNI
		uint8_t  *						JNI_GetImage();
		float    *						JNI_GetColorChannels();
		

		uint32_t *						JNI_GetDepth();

		uint16_t *						JNI_GetDepthRaw_16_Data();
		uint16_t *						JNI_GetDepthRaw_256_Data();


		uint32_t *						JNI_GetDepthSha();
		uint32_t *						JNI_GetInfrared();
		uint32_t *						JNI_GetLongExposureInfrared();

		uint8_t  *						JNI_GetCoodinateRGBX();

		uint32_t *						JNI_GetDepthMask();

		uint32_t *						JNI_GetBodyTrack();

		//FACE
		float *						    JNI_getFaceColorRawData();
		float *							JNI_getFaceInfraredRawData();

		float *							JNI_getHDFaceVertexRawData();

		float *							JNI_pointCloudPosData();

		uint32_t   *					JNI_pointCloudDepthImage();
		float      *					JNI_pointCloudDepthNormalized();

		float *							JNI_pointCloudColorData();

		float *							JNI_getSkeletonDataDepthMap();
		float *							JNI_getSkeletonData3dMap();
		float *							JNI_getSkeletonDataColorMap();

		//MAPS
		float *							JNI_getMapDepthToCameraTable();
		float *							JNI_getMapDepthToColor();

		//USERS
		uint32_t *						JNI_getBodyIndexUser(int index);

		std::string						JNI_version() { return VERSION; }

		//HELP FUNTIONS
		float *							BodyToScreenColor(const CameraSpacePoint * bodyPoint);
		float *							BodyToScreenDepth(const CameraSpacePoint * bodyPoint);

		int								colorByte2Int(int gray);
		int								colorFloat2Int(float gray);
		HRESULT							UpdateBodyData(IBody** ppBodies, IMultiSourceFrame* frame);
		static void						ExtractFaceRotationInDegrees(const Vector4* pQuaternion, int* pPitch, int* pYaw, int* pRoll);
		float							lmap(float val, float inMin, float inMax, float outMin, float outMax);

		int								numberUsers;
		uint32_t						getColorMaskUser(BYTE ir);

		float							constrain(float val, float min, float max);



		void							setLowThresholdDepthPC(float val){ depthPCLowTh = val; }
		float							getLowThresholdDepthPC(){ return depthPCLowTh; }


		void							setHighThresholdDepthPC(float val){ depthPCHighTh = val; }
		float							getHighThresholdDepthPC(){ return depthPCHighTh; }

		void							setNumberOfUsers(int num){ if (num > 6) num = 6; if (num < 1) num = 1; numberUsers = num; }


		//MAPERS
		void							enableMapDepthToCamaraTable(){ activateMapDepthToCamaraTable = true; }
		void							enableMapDepthToColorData(){ activateMapDepthToColor = true; }

		//SIZES
///		uint32_t						getFrameSizeDepth(){ return frame_size_depth; }


		//
		//uint32_t						depthJNI[frame_size_depth];
	};

}