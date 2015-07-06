#pragma once

namespace KinectPV2{
	class DeviceActivators
	{
	public:
		DeviceActivators(){
			mColorFrameReady = false;
			mColorChannelsFrameReady = false;

			mDepthFrameReady = false;

			mInfraredFrameReady = false;
			mInfraredlongExposureReady = false;

			mBodyIndexReady = false;
			mBodyIndexDephReady = false;

			//POINT CLOID
			mDepthPointCloudPosReady = false;
			mDepthPointCloudImageReady = false;
			mColorPointCloudFrameReady = false;

			//SKELETON
			mSkeletonReady = false;

			//FACE
			mFaceDetectionReady = false;
			mHDFaceDetectionReady = false;
			
			mCoordinateRGBXReady = false;

		}

		bool			isColorFrameReady(){ return mColorFrameReady; }
		bool			isColorChannelsFrameReady(){ return mColorChannelsFrameReady; }

		bool			isDepthFrameReady(){ return mDepthFrameReady; }

		bool			isInfraredFrameReady(){ return mInfraredFrameReady; }
		bool			isInfraredlongExposureReady(){ return mInfraredlongExposureReady; }

		bool			isBodyIndexReady(){ return mBodyIndexReady; }
		bool			isBodyIndexDephReady(){ return mBodyIndexDephReady; }

		bool			isDepthPointCloudPosReady(){ return  mDepthPointCloudPosReady; }
		bool			isDepthPointCloudImageReady(){ return mDepthPointCloudImageReady; }
		bool			isColorPointCloudFrameReady(){ return mColorPointCloudFrameReady; }

		bool			isSkeletonReady(){ return mSkeletonReady; }

		bool			isFaceDetectionReady(){ return mFaceDetectionReady; }
		bool			isHDFaceDetectionReady(){ return  mHDFaceDetectionReady; }

		bool			isCoordinateRGBXReady(){ return  mCoordinateRGBXReady; }

	protected:
		bool			mColorFrameReady;
		bool			mColorChannelsFrameReady;

		bool			mDepthFrameReady;

		bool			mInfraredFrameReady;
		bool			mInfraredlongExposureReady;

		bool			mBodyIndexReady;
		bool			mBodyIndexDephReady;

		//POINT CLOID
		bool			mDepthPointCloudPosReady;
		bool			mDepthPointCloudImageReady;
		bool			mColorPointCloudFrameReady;

		//SKELETON
		bool			mSkeletonReady;

		//FACE
		bool			mFaceDetectionReady;
		bool			mHDFaceDetectionReady;

		//MAPERS
		bool			mCoordinateRGBXReady;

	public:
		void			colorFrameReady(bool toggle = false){ mColorFrameReady = toggle; }
		void			colorChannelsFrameReady(bool toggle = false){ mColorChannelsFrameReady = toggle; }

		void			depthFrameReady(bool toggle = false){ mDepthFrameReady = toggle; }

		void			infraredFrameReady(bool toggle = false){ mInfraredFrameReady = toggle; }
		void			infraredlongExposureReady(bool toggle = false){ mInfraredlongExposureReady = toggle; }

		void			bodyIndexReady(bool toggle = false){ mBodyIndexReady = toggle; }
		void			bodyIndexDephReady(bool toggle = false){ mBodyIndexDephReady = toggle; }

		void			depthPointCloudPosReady(bool toggle = false){ mDepthPointCloudPosReady = toggle; }
		void			depthPointCloudImageReady(bool toggle = false){ mDepthPointCloudImageReady = toggle; }
		void			colorPointCloudFrameReady(bool toggle = false){ mColorPointCloudFrameReady = toggle; }

		void			skeletonReady(bool toggle = false){ mSkeletonReady = toggle; }

		void			faceDetectionReady(bool toggle = false){ mFaceDetectionReady = toggle; }
		void			HDFaceDetectionReady(bool toggle = false){ mHDFaceDetectionReady = toggle; }

		void			coordinateRGBXReady(bool toggle = false){ mCoordinateRGBXReady = toggle; }
	};
}