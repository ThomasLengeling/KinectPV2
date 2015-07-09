#pragma once

namespace KinectPV2{
	class DeviceActivators
	{
	public:
		DeviceActivators(){
			disableAll();
		}


		void disableAll()
		{
			//COLOR
			mColorFrameReady = false;
			mColorChannelsFrameReady = false;

			//DEPTH
			mDepthFrameReady = false;
			mDepthFrameDone = false;

			//INFRARED
			mInfraredFrameReady = false;
			mInfraredFrameDone = false;
			mInfraredlongExposureReady = false;

			//BODY INDEX
			mBodyIndexReady = false;
			mBodyIndexDephReady = false;

			//POINT CLOID
			mDepthPointCloudPosReady = false;
			mDepthPointCloudImageReady = false;
			mColorPointCloudFrameReady = false;

			//SKELETON
			mSkeletonDepthReady = false;
			mSkeletonColorReady = false;
			mSkeleton3DReady = false;

			mSkeletonDepthDone = false;
			mSkeletonColorDone = false;
			mSkeleton3DDone = false;

			//FACE
			mFaceDetectionReady = false;
			mHDFaceDetectionReady = false;

			//MAPERS
			mCoordinateRGBXReady = false;

			//THREADS
			colorProcess = false;
			depthProcess = false;
			infraredProcess = false;
			infraredLongExposureProcess = false;
			skeletonProcess = false;
			bodyTrackProcess = false;
			hdFaceProcess = false;
			coordinateMapperProcess = false;
		}

		bool			isColorFrameReady(){ return mColorFrameReady; }
		bool			isColorChannelsFrameReady(){ return mColorChannelsFrameReady; }

		bool			isDepthFrameReady(){ return mDepthFrameReady; }
		bool			isDepthFrameDone(){ return mDepthFrameDone; }

		bool			isInfraredFrameReady(){ return mInfraredFrameReady; }
		bool			isInfraredFrameDone(){ return mInfraredFrameDone; }

		bool			isInfraredlongExposureReady(){ return mInfraredlongExposureReady; }

		bool			isBodyIndexReady(){ return mBodyIndexReady; }
		bool			isBodyIndexDephReady(){ return mBodyIndexDephReady; }

		bool			isDepthPointCloudPosReady(){ return  mDepthPointCloudPosReady; }
		bool			isDepthPointCloudImageReady(){ return mDepthPointCloudImageReady; }
		bool			isColorPointCloudFrameReady(){ return mColorPointCloudFrameReady; }

		bool			isSkeletonDepthReady(){ return mSkeletonDepthReady; }
		bool			isSkeletonDepthDone(){ return mSkeletonDepthDone; }

		bool			isSkeletonColorReady(){ return mSkeletonColorReady; }
		bool			isSkeleton3DReady(){ return mSkeleton3DReady; }

		bool			isFaceDetectionReady(){ return mFaceDetectionReady; }
		bool			isHDFaceDetectionReady(){ return  mHDFaceDetectionReady; }

		bool			isCoordinateRGBXReady(){ return  mCoordinateRGBXReady; }

	protected:
		bool			mColorFrameReady;
		bool			mColorChannelsFrameReady;

		bool			mDepthFrameReady;
		bool			mDepthFrameDone;

		bool			mInfraredFrameReady;
		bool			mInfraredFrameDone;

		bool			mInfraredlongExposureReady;

		bool			mBodyIndexReady;
		bool			mBodyIndexDephReady;

		//POINT CLOID
		bool			mDepthPointCloudPosReady;
		bool			mDepthPointCloudImageReady;
		bool			mColorPointCloudFrameReady;

		//SKELETON
		bool			mSkeletonDepthReady;
		bool			mSkeletonColorReady;
		bool			mSkeleton3DReady;

		bool			mSkeletonDepthDone;
		bool			mSkeletonColorDone;
		bool			mSkeleton3DDone;

		//FACE
		bool			mFaceDetectionReady;
		bool			mHDFaceDetectionReady;

		//MAPERS
		bool			mCoordinateRGBXReady;


		//THREADS
		volatile bool	colorProcess;
		volatile bool	depthProcess;
		volatile bool	infraredProcess;
		volatile bool	infraredLongExposureProcess;
		volatile bool	skeletonProcess;
		volatile bool	bodyTrackProcess;
		volatile bool	hdFaceProcess;
		volatile bool   coordinateMapperProcess;

	public:
		void			colorFrameReady(bool toggle = true){ mColorFrameReady = toggle; }
		void			colorChannelsFrameReady(bool toggle = true){ mColorChannelsFrameReady = toggle; }

		void			depthFrameReady(bool toggle = true){ mDepthFrameReady = toggle; }
		void			depthFrameDone(bool toggle = true){ mDepthFrameDone = toggle; }

		void			setInfraredFrameReady(bool toggle = true){ mInfraredFrameReady = toggle; }
		void			setInfraredFrameDone(bool toggle = true){ mInfraredFrameDone = toggle; }

		void			infraredlongExposureReady(bool toggle = true){ mInfraredlongExposureReady = toggle; }

		void			bodyIndexReady(bool toggle = true){ mBodyIndexReady = toggle; }
		void			bodyIndexDephReady(bool toggle = true){ mBodyIndexDephReady = toggle; }

		void			depthPointCloudPosReady(bool toggle = true){ mDepthPointCloudPosReady = toggle; }
		void			depthPointCloudImageReady(bool toggle = true){ mDepthPointCloudImageReady = toggle; }
		void			colorPointCloudFrameReady(bool toggle = true){ mColorPointCloudFrameReady = toggle; }

		void			setSkeletonDepthReady(bool toggle = true){ mSkeletonDepthReady = toggle; }
		void			setSkeletonDepthDone(bool toggle = true){ mSkeletonDepthDone = toggle; }

		void			skeletonColorReady(bool toggle = true){ mSkeletonColorReady = toggle; }
		void			skeleton3DReady(bool toggle = true){ mSkeleton3DReady = toggle; }

		void			faceDetectionReady(bool toggle = true){ mFaceDetectionReady = toggle; }
		void			HDFaceDetectionReady(bool toggle = true){ mHDFaceDetectionReady = toggle; }

		void			coordinateRGBXReady(bool toggle = true){ mCoordinateRGBXReady = toggle; }

		//THREADS
		volatile bool			isColorProcessActivated(){ return colorProcess; }
		volatile bool			isDepthProcessActivated(){ return depthProcess; }
		volatile bool			isInfraredProcessActivated(){ return infraredProcess; }
		volatile bool			isInfraredLongExposureProcessActivated(){ return infraredLongExposureProcess; }
		volatile bool			isSkeletonProcessActivated(){ return skeletonProcess; }
		volatile bool			isBodyTrackProcessActivated(){ return bodyTrackProcess; }
		volatile bool			isHDFaceProcessActivated(){ return hdFaceProcess; }
		volatile bool			isCoordinateMapperActivated(){ return coordinateMapperProcess; }


		void			enableColorProcess(volatile bool toggle = true){ colorProcess = toggle; }
		void			enableDepthProcess(volatile bool toggle = true){ depthProcess = toggle; }
		void			enableInfraredProcess(volatile bool toggle = true){ infraredProcess = toggle; }
		void			enableInfraredLongExposureProcess(volatile bool toggle = true){ infraredLongExposureProcess = toggle; }
		void			enableSkeletonProcess(volatile bool toggle = true){ skeletonProcess = toggle; }
		void			enableBodyTrackProcess(volatile bool toggle = true){ bodyTrackProcess = toggle; }
		void			enableHDFaceProcess(volatile bool toggle = true){ hdFaceProcess = toggle; }
		void			enableCoordinateMapperProcess(volatile bool toggle = true){ coordinateMapperProcess = toggle; }
	};
}