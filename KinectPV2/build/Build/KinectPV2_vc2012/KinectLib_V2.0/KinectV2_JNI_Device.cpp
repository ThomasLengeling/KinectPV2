#include "stdafx.h"
#include "KinectV2_JNI_Device.h"


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDevice
(JNIEnv * env, jobject obj)
{
	KinectPV2::Device* kinect = new KinectPV2::Device();

	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	env->SetLongField(obj, fid, (jlong)kinect);
	env->DeleteLocalRef(cls);
}

JNIEXPORT jboolean JNICALL Java_KinectPV2_Device_jniInit
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	jboolean result = (jboolean)kinect->init();
	env->DeleteLocalRef(cls);
	return result;
}


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetWindowSizeSkeleton
(JNIEnv * env, jobject obj, jint width, jint height)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setWindowSize((int)width, (int)height);
	env->DeleteLocalRef(cls);
	env->DeleteLocalRef(obj);
}

JNIEXPORT jstring JNICALL Java_KinectPV2_Device_jniVersion
(JNIEnv * env, jobject obj)
{
	jstring result;

	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	std::string version = kinect->JNI_version();
	result = env->NewStringUTF(version.c_str());
	env->DeleteLocalRef(cls);
	env->DeleteLocalRef(obj);
	return  result;
}


JNIEXPORT jboolean JNICALL Java_KinectPV2_Device_jniUpdate
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	bool result = kinect->update();

	if (result){
		if (kinect->isColorFrameReady()){
			
			jint * pInt = (jint *)kinect->JNI_GetImage();
			
			jintArray buffer = env->NewIntArray(frame_size_color);
			env->SetIntArrayRegion(buffer, 0, frame_size_color, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyColorImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}
		if (kinect->isDepthFrameReady()){
			jint * pInt = (jint *)kinect->JNI_GetDepth();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, pInt);

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyDepthImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}

		if (kinect->isBodyIndexReady()){
			jint * pInt = (jint *)kinect->JNI_GetBodyTrack();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyBodyIndexImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}
		if (kinect->isBodyIndexDephReady()){
			jint * pInt = (jint *)kinect->JNI_GetDepthMask();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyBodyIndexDepthImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}

		if (kinect->isInfraredFrameReady()){
			jint * pInt = (jint *)kinect->JNI_GetInfrared();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyInfraredImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}
		
		if (kinect->isDepthPointCloudPosReady()){
			jfloat * pFloat = (jfloat *)kinect->JNI_pointCloudPosData();

			jfloatArray buffer = env->NewFloatArray(frame_size_depth * 3);
			env->SetFloatArrayRegion(buffer, 0, frame_size_depth * 3, pFloat);

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyPointCloudPos", "([F)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pFloat));

		}
		if (kinect->isDepthPointCloudImageReady()){
			jint * pFloatCloud = (jint *)kinect->JNI_pointCloudDepthImage();

			jintArray bufferCloud = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(bufferCloud, 0, frame_size_depth, pFloatCloud);

			jclass clazzCloud       = env->GetObjectClass(obj);
			jmethodID methodIDCloud = env->GetMethodID(clazzCloud, "copyPointCloudImage", "([I)V");
			env->CallVoidMethod(obj, methodIDCloud, bufferCloud);

			env->DeleteLocalRef(clazzCloud);
			env->DeleteLocalRef((jobjectArray)bufferCloud);
			env->DeleteLocalRef((jobjectArray)(pFloatCloud));
		}

		if (kinect->isColorPointCloudFrameReady()){
			jfloat * pFloat = (jfloat *)kinect->JNI_pointCloudColorData();

			jfloatArray buffer = env->NewFloatArray(frame_size_color * 3);
			env->SetFloatArrayRegion(buffer, 0, frame_size_color * 3, pFloat);

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyPointCloudColor", "([F)V");
			env->CallVoidMethod(obj, methodID, buffer);

			jfloat * pInt = (jfloat *)kinect->JNI_GetColorChannels();

			jfloatArray bufferColor = env->NewFloatArray(frame_size_color * 3);
			env->SetFloatArrayRegion(bufferColor, 0, frame_size_color * 3, (jfloat *)(pInt));

			jclass clazzColor = env->GetObjectClass(obj);
			jmethodID methodIDColor = env->GetMethodID(clazzColor, "copyColorChannelImg", "([F)V");
			env->CallVoidMethod(obj, methodIDColor, bufferColor);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef(clazzColor);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(bufferColor));
		}


		if (kinect->isInfraredlongExposureReady()){
			jint * pInt = (jint *)kinect->JNI_GetLongExposureInfrared();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyInfraredLongExposureImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}

		if (kinect->isSkeletonReady()){
			//DEPTH
			jfloat * pfloatDepth = (jfloat *)kinect->JNI_getSkeletonDataDepthMap();

			jfloatArray bufferDepth = env->NewFloatArray(JOINTSIZE);
			env->SetFloatArrayRegion(bufferDepth, 0, JOINTSIZE, (jfloat *)(pfloatDepth));

			jclass clazzDepth = env->GetObjectClass(obj);
			jmethodID methodIDDepth = env->GetMethodID(clazzDepth, "copySkeletonDepthData", "([F)V");
			env->CallVoidMethod(obj, methodIDDepth, bufferDepth);

			env->DeleteLocalRef(clazzDepth);
			env->DeleteLocalRef((jobjectArray)bufferDepth);
			env->DeleteLocalRef((jobjectArray)(pfloatDepth));

			//COLOR
			jfloat * pfloatColor = (jfloat *)kinect->JNI_getSkeletonDataColorMap();

			jfloatArray bufferColor = env->NewFloatArray(JOINTSIZE);
			env->SetFloatArrayRegion(bufferColor, 0, JOINTSIZE, (jfloat *)(pfloatColor));

			jclass clazzColor = env->GetObjectClass(obj);
			jmethodID methodIDColor = env->GetMethodID(clazzColor, "copySkeletonColorData", "([F)V");
			env->CallVoidMethod(obj, methodIDColor, bufferColor);

			env->DeleteLocalRef(clazzColor);
			env->DeleteLocalRef((jobjectArray)bufferColor);
			env->DeleteLocalRef((jobjectArray)(pfloatColor));

			//3D
			jfloat * pfloat3D = (jfloat *)kinect->JNI_getSkeletonData3dMap();

			jfloatArray buffer3D = env->NewFloatArray(JOINTSIZE);
			env->SetFloatArrayRegion(buffer3D, 0, JOINTSIZE, (jfloat *)(pfloat3D));

			jclass clazz3D = env->GetObjectClass(obj);
			jmethodID methodID3D = env->GetMethodID(clazz3D, "copySkeleton3DData", "([F)V");
			env->CallVoidMethod(obj, methodID3D, buffer3D);

			env->DeleteLocalRef(clazz3D);
			env->DeleteLocalRef((jobjectArray)buffer3D);
			env->DeleteLocalRef((jobjectArray)(pfloat3D));

			kinect->skeletonReady(false);
			//std::cout << "JNI done" << std::endl;
		}

		if (kinect->isCoordinateRGBXReady()){
			jint * pInt = (jint *)kinect->JNI_GetCoodinateRGBX();

			jintArray buffer = env->NewIntArray(frame_size_color);
			env->SetIntArrayRegion(buffer, 0, frame_size_color, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyCoordinateMapper", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);
			
			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pInt));
		}

		if (kinect->isFaceDetectionReady()){
			//COLOR FACE
			jfloat * pfloatColor = (jfloat *)kinect->JNI_getFaceColorRawData();

			jfloatArray bufferColor = env->NewFloatArray(FACESIZE);
			env->SetFloatArrayRegion(bufferColor, 0, FACESIZE, (jfloat *)(pfloatColor));

			jclass clazzColor = env->GetObjectClass(obj);
			jmethodID methodIDColor = env->GetMethodID(clazzColor, "copyFaceColorRawData", "([F)V");
			env->CallVoidMethod(obj, methodIDColor, bufferColor);

			//INFRARED FACE
			jfloat * pfloatInfrared = (jfloat *)kinect->JNI_getFaceInfraredRawData();

			jfloatArray bufferInfrared = env->NewFloatArray(FACESIZE);
			env->SetFloatArrayRegion(bufferInfrared, 0, FACESIZE, (jfloat *)(pfloatInfrared));

			jclass clazzInfrared = env->GetObjectClass(obj);
			jmethodID methodIDInfrared = env->GetMethodID(clazzInfrared, "copyFaceInfraredRawData", "([F)V");
			env->CallVoidMethod(obj, methodIDInfrared, bufferInfrared);

			//kinect->faceDetectionReady = false;
		}

		if (kinect->isHDFaceDetectionReady()){

			jfloat * pfloat = (jfloat *)kinect->JNI_getHDFaceVertexRawData();

			jfloatArray buffer = env->NewFloatArray(HDFACEVERTEX);
			env->SetFloatArrayRegion(buffer, 0, HDFACEVERTEX, (jfloat *)(pfloat));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyHDFaceVertexRawData", "([F)V");
			env->CallVoidMethod(obj, methodID, buffer);

			env->DeleteLocalRef(clazz);
			env->DeleteLocalRef((jobjectArray)buffer);
			env->DeleteLocalRef((jobjectArray)(pfloat));
		}
	
	}
	

	env->DeleteGlobalRef(cls);

	return (jboolean)result;
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniStopDevice
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->stop();
	env->DeleteLocalRef(cls);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableColorFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableColorImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableColorChannelsFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableColorChannelsFrame((bool)toggle);
	env->DeleteLocalRef(cls);
}



JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthMaskFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthMaskImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableInfraredFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableInFraredImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableBodyTrackFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableBodyIndex((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableLongExposureInfrared
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableInFraredExposureImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableSkeleton
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableSkeleton((bool)toggle);
	env->DeleteLocalRef(cls);
}

////FACE DETECTION
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableFaceDetection
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableFaceDetection((bool)toggle);
	env->DeleteLocalRef(cls);
}

//HDFACE DETECTION
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableHDFaceDetection
(JNIEnv *env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableHDFaceDetection(true);
	env->DeleteLocalRef(cls);

}

//COORDINATE MAPPER RGB DEPTH
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableCoordinateMappingRGBDepth
(JNIEnv *env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableCoordinateMapperColor(true);
	env->DeleteLocalRef(cls);
}

//SET BACKGROUND COORDINATE MAPPER
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSendCoordinateBkg
(JNIEnv *env, jobject obj, jintArray ptr)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	int i;
	jsize len = env->GetArrayLength(ptr);
	jint *body = env->GetIntArrayElements(ptr, 0);
	for (i = 0; i < len; i++){
		kinect->backgroundRGBX[i] = body[i];
	}

	env->ReleaseIntArrayElements(ptr, body, 0);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSendCoordinateDepth
(JNIEnv *env, jobject obj, jintArray ptr)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	int i;
	jsize len = env->GetArrayLength(ptr);
	jint *body = env->GetIntArrayElements(ptr, 0);
	for (i = 0; i < len; i++){
		kinect->backgroundRGBX[i] = body[i];
	}

	env->ReleaseIntArrayElements(ptr, body, 0);
	env->DeleteLocalRef(cls);
}

//POINT CLOUD
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnablePointCloud
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enablePointCloudDepth((bool)toggle);
	if (toggle == JNI_TRUE)
		kinect->enableDepthImage(true);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnablePointCloudColor
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enablePointCloudColor(bool(toggle));
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetHighThresholdDepthPC
(JNIEnv * env, jobject obj, jfloat val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setHighThresholdDepthPC((float)val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT jfloat JNICALL Java_KinectPV2_Device_jniGetHighThresholdDepthPC
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	float val = kinect->getHighThresholdDepthPC();
	env->DeleteLocalRef(cls);

	return (jfloat)val;
}


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetLowThresholdDepthPC
(JNIEnv * env, jobject obj, jfloat val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setLowThresholdDepthPC((float)val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT jfloat JNICALL Java_KinectPV2_Device_jniGetLowThresholdDepthPC
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	float val = kinect->getLowThresholdDepthPC();
	env->DeleteLocalRef(cls);

	return (jfloat)val;
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetMirror
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableMirror(bool(toggle));
	env->DeleteLocalRef(cls);
}


//MAPPER

JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniEnableMapDethCamaraTable
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	//kinect->enableMapDepthToCamaraTable();

	jfloat * pfloat = (jfloat *)kinect->JNI_getMapDepthToCameraTable();

	jfloatArray buffer = env->NewFloatArray((jsize)frame_size_depth * 2);
	env->SetFloatArrayRegion(buffer, 0, (jsize)frame_size_depth * 2, (jfloat *)(pfloat));

	env->DeleteLocalRef(cls);

	return buffer;
}


JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniEnableMapDethToColorSpace
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableMapDepthToColorData();

	jfloat * pfloat = (jfloat *)kinect->JNI_getMapDepthToColor();

	jfloatArray buffer = env->NewFloatArray((jsize)frame_size_depth * 2);
	env->SetFloatArrayRegion(buffer, 0, (jsize)frame_size_depth * 2, (jfloat *)(pfloat));

	env->DeleteLocalRef(cls);

	return buffer;
}

//ENABLE DEPTH SMOOTH
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthSmooth
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthSmooth(bool(toggle));
	env->DeleteLocalRef(cls);
}

//NUMBER OF USERS

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetNumberOfUsers
(JNIEnv * env, jobject obj, jint val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setNumberOfUsers(bool(val));
	env->DeleteLocalRef(cls);
}


//--------------------------------///readyCopy----------------------------------------------
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniColorReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->colorFrameReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDepthReadyCopy
(JNIEnv *env , jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->depthFrameReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniInfraredReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->infraredFrameReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniBodyIndexReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->bodyIndexReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDepthMaskBodyIndexReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->bodyIndexDephReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniInfraredLongExposureReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->infraredlongExposureReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSkeletonReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->skeletonReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniFaceDetectionReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->faceDetectionReady(!val);
	env->DeleteLocalRef(cls);
}


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniHDFaceVertexReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->HDFaceDetectionReady(!val);
	env->DeleteLocalRef(cls);
}


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniPointColorReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->colorPointCloudFrameReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDepthPointCloudImageReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->depthPointCloudImageReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL  Java_KinectPV2_Device_jniPointCoudPosReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->depthPointCloudPosReady(!val);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL  Java_KinectPV2_Device_jniCoordinateMapperReadyCopy
(JNIEnv *env, jobject obj, jboolean val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->coordinateRGBXReady(!val);
	env->DeleteLocalRef(cls);
}


//---------------------GET USERS-----------------------------------------------------------
JNIEXPORT jintArray JNICALL  Java_KinectPV2_Device_jniGetBodyIndexUser
(JNIEnv *env, jobject obj, jint index)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)0);
	const jint * pInt = (const jint *)kinect->JNI_getBodyIndexUser((jint)index);
	buffer = env->NewIntArray((jsize)frame_size_depth);
	env->NewIntArray((jsize)frame_size_depth);
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));

	env->DeleteLocalRef(cls);
	//env->DeleteLocalRef((jobject)pInt);

	return buffer;
}