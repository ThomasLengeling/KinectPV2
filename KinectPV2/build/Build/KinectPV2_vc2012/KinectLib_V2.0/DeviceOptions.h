#pragma once

namespace KinectPV2{
	class DeviceOptions{

	public:
		DeviceOptions() :
			toggleColorFrame(false), toggleDepthFrame(false), toggleInFraredFrame(false), toggleBodyIndexDepthFrame(false), toggleDepthSmooth(false),
			toggleLongExposureInFraredFrame(false), togglePointCloudFrame(false), togglePointCloudColorFrame(false), toggleFaceDetection(false),
			toggleRawDepthData(false), toggleBodyIndexFrame(false), toggleSkeleton(false), togglCoodinateMappingColor(false), toggleHDFaceDetection(false),
			toggleColorChannelsFrame(false),
			initToggleDepthFrame(false), initToggleColorFrame(false), initToggleInfraredFrame(false), initToggleSkeleton(false), initToggleBodyIndexFrame(false),
			initToggleFaceDetection(false), initToggleLongExposureInfraredFrame(false), initToggleHDFaceDetection(false) { }

		~DeviceOptions(){}

		//BASIC FUNCTIONS

		void			enableColorImage(bool toggle = true){
							initToggleColorFrame = toggle;
							toggleColorFrame = toggle;
		}

		void			enableColorChannelsFrame(bool toggle = true){
							initToggleColorFrame = toggle;
							toggleColorChannelsFrame = toggle;
		}

		void			enableDepthImage(bool toggle = true){
							initToggleDepthFrame = toggle;
							toggleDepthFrame = toggle;
		}

		void			enableDepthMaskImage(bool toggle = true){
							initToggleBodyIndexFrame = toggle;
							initToggleDepthFrame = toggle;
							toggleDepthFrame = toggle;
							toggleBodyIndexDepthFrame = toggle;
		}

		void			enableDepthSmooth(bool toggle){ 
							toggleDepthSmooth = toggle;
		}

		//FACE
		void			enableFaceDetection(bool toggle = false){
							initToggleInfraredFrame = toggle;
							initToggleColorFrame = toggle;
							initToggleSkeleton = toggle;
							initToggleFaceDetection = toggle;
							toggleFaceDetection = toggle;
							toggleInFraredFrame = toggle;
							toggleColorFrame = toggle;
		}

		//HD FACE
		void			enableHDFaceDetection(bool toggle = false){
							initToggleInfraredFrame = toggle;
							initToggleColorFrame = toggle;
							initToggleSkeleton = toggle;
							initToggleHDFaceDetection = toggle;
							toggleHDFaceDetection = toggle;
							toggleSkeleton = toggle;
			//toggleBodyTrack = true;
		}

		void			enableCoordinateMapperColor(bool toggle = false){
							togglCoodinateMappingColor = toggle;
							initToggleDepthFrame = toggle;
							initToggleBodyIndexFrame = toggle;
		}

		void            enableInFraredImage(bool toggle = true){
							initToggleInfraredFrame = toggle;
							toggleInFraredFrame = toggle;
		}

		void			enableInFraredExposureImage(bool toggle = true){
							initToggleLongExposureInfraredFrame = toggle;
							toggleLongExposureInFraredFrame = toggle;
		}


		void            enableBodyIndex(bool toggle = true){
							toggleBodyIndexFrame = toggle;
							initToggleBodyIndexFrame = toggle;
		}

		void			enablePointCloudDepth(bool toggle = true){ 
							togglePointCloudFrame = toggle; 
						}
						
		void			enablePointCloudColor(bool toggle = true){
							togglePointCloudColorFrame = toggle;
							toggleDepthFrame = toggle;
							initToggleDepthFrame = toggle;
							initToggleColorFrame = toggle;
		}

		void			enableRawDepthData(bool toggle = true){ 
							toggleRawDepthData = toggle; 
		}


		void			enableSkeleton(bool toggle = true){
							initToggleSkeleton = toggle;
							initToggleDepthFrame = toggle;
							initToggleColorFrame = toggle;
							toggleSkeleton = toggle;
							toggleDepthFrame = toggle;
							toggleColorFrame = toggle;
		}

		inline bool		isEnableColorFrame(){ return toggleColorFrame; }
		inline bool		isEnableColorChannelsFrame(){ return toggleColorChannelsFrame; }

		inline bool     isEnableDepthFrame(){ return toggleDepthFrame; }
		inline bool		isEnableDepthMaskFrame(){ return toggleBodyIndexDepthFrame; }
		inline bool		isEnableDepthSmoothFrame(){ return toggleDepthSmooth; }

		inline bool		isEnableInFraredFrame(){ return toggleInFraredFrame; }
		inline bool		isEnableLongExposureInfraredFrame(){ return toggleLongExposureInFraredFrame; }

		inline bool     isEnableFaceDetection(){ return toggleFaceDetection; }
		inline bool		isEnableHDFaceDetection(){ return toggleHDFaceDetection; }

		inline bool		isEnableBodyIndex(){ return toggleBodyIndexFrame; }

		inline bool		isEnablePointCloudDepth(){ return togglePointCloudFrame; }
		inline bool		isEnablePointCloudColor(){ return togglePointCloudColorFrame; }

		inline bool		isEnableRawDepthData(){ return toggleRawDepthData; }

		inline bool     isEnableSkeleton(){ return toggleSkeleton; }

		inline bool		isEnableCoordinateMappingColor(){ return togglCoodinateMappingColor; }

		//starters
	protected:
		inline bool		isInitializedColorFrame(){ return initToggleColorFrame; }
		inline bool		isInitializedDepthFrame(){ return initToggleDepthFrame; }
		inline bool		isInitializedInfraredFrame(){ return initToggleInfraredFrame; }
		inline bool		isInitializedSkeleton(){ return initToggleSkeleton; }
		inline bool		isInitializedBodyIndexFrame(){ return initToggleBodyIndexFrame; }
		inline bool		isInitializedFaceDetection(){ return initToggleFaceDetection; }
		inline bool		isInitializedHDFaceDetection(){ return initToggleHDFaceDetection; }
		inline bool		isInitializedLongExposureInfraredFrame(){ return initToggleLongExposureInfraredFrame; }

	private:
		bool		toggleColorFrame;
		bool		toggleColorChannelsFrame;

		bool		toggleDepthFrame;
		bool		toggleDepthSmooth;

		bool		toggleFaceDetection;
		bool		toggleHDFaceDetection;

		bool		toggleInFraredFrame;
		bool        toggleLongExposureInFraredFrame;

		bool		togglePointCloudFrame;
		bool		togglePointCloudColorFrame;

		bool		toggleRawDepthData;

		bool        toggleBodyIndexFrame;
		bool		toggleBodyIndexDepthFrame;

		bool		toggleSkeleton;

		bool		togglCoodinateMappingColor;

		//STARTERS FUNCTIONS
		bool		initToggleColorFrame;
		bool		initToggleInfraredFrame;
		bool		initToggleDepthFrame;
		bool		initToggleSkeleton;
		bool		initToggleBodyIndexFrame;
		bool		initToggleFaceDetection;
		bool		initToggleLongExposureInfraredFrame;
		bool		initToggleHDFaceDetection;

	};
}