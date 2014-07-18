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

#include "ole2.h"
#include <Kinect.h>

#define VERSION			"0.3"

static const int        cColorWidth = 1920;
static const int        cColorHeight = 1080;

static const int        cDepthWidth = 512;
static const int        cDepthHeight = 424;

#define frame_size_color 2073600
#define frame_size_depth 217088

static const int BUFFER_SIZE_COLOR = frame_size_color * 4;

//SKELETON
static const int JOINTSSIZE = BODY_COUNT * (JointType_Count)* 5;

//////////////////////////////////////////////////////////////////////////////////////////////
namespace KinectPV2{
	class DeviceOptions{

	public:
		DeviceOptions(){}
		~DeviceOptions(){}

		void			enableColorImage(){ toggleColorFrame = true; }
		void			enableDepthImage(){ toggleDepthFrame = true;  }
		void            enableInFraredImage(){ toggleInFraredFrame = true; }
		void            enableBody(){ toggleBody = true; }

		void			disableColorImage(){ toggleColorFrame = true; }
		void			disableDepthImage(){ toggleDepthFrame = true; }
		void			disabeInfraredImage(){ toggleInFraredFrame = true; }
		void			disableBody(){ toggleBody = true; }

		inline bool		isAnableColorFrame(){ return toggleColorFrame; }
		inline bool     isAnableDepthFrame(){ return toggleDepthFrame; }
		inline bool		isAnableInFraredFrame(){ return toggleInFraredFrame; }
		inline bool     isAnableBody(){ return toggleBody; }

	private:
		bool		toggleColorFrame;
		bool		toggleDepthFrame;
		bool		toggleInFraredFrame;

		bool        toggleBody;
		bool		toggleBodyIndex;
		bool        toggleInFraredLongExposure;
	};

	class Device : public DeviceOptions
	{
	private:
		uint8_t *	 pixelsData;
		uint8_t *    colorFrameData;

		uint32_t *	 depthData;
		// *   depthFrameData;

		uint32_t *	 infraredData;
		//uint32_t *   inFraredFrameData;

		int			appWidth;
		int         appHeight;

	protected:

		IMultiSourceFrameReader*	kFrameReader;

		IColorFrameReader*			kColorFrameReader;
		IDepthFrameReader*			kDepthFrameReader;

		IInfraredFrameReader*		kInfraredFrameReader;
		IBodyFrameReader*			kBodyFrameReader;
		ICoordinateMapper*			kCoordinateMapper;

		IKinectSensor*				kSensor;

	public:
		Device(void);
		~Device(void);

		bool init();
		void stop();
		bool update();

		void setWindowSize(int appWidth, int appHeight);

		//-----JNI
		uint8_t  *						JNI_GetImage();
		uint32_t *						JNI_GetDepth();
		uint32_t *						JNI_GetDepthSha();
		uint32_t *						JNI_GetInfrared();
		float *							JNI_getBody();
		std::string						JNI_version() { return VERSION; }

		float							joints[BODY_COUNT][JointType_Count][5];

		float *							BodyToScreen(const CameraSpacePoint& bodyPoint);
		int								colorByte2Int(int gray);

		bool							infraredFrameReady;
		bool							colorFrameReady;
		bool							depthFrameReady;

		//
		//uint32_t						depthJNI[frame_size_depth];
	};

}