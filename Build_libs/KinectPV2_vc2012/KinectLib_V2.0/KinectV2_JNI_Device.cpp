#include "stdafx.h"
#include "KinectV2_JNI_Device.h"

/*
* Class:     KinectPV2_Device
* Method:    jniDevice
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDevice
(JNIEnv * env, jobject obj)
{
	KinectPV2::Device* kinect = new KinectPV2::Device();

	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	env->SetLongField(obj, fid, (jlong)kinect);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniInit
* Signature: ()Z
*/
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


/*
* Class:     KinectPV2_Device
* Method:    jniVersion
* Signature: ()Ljava/lang/String;
*/
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


/*
* Class:     KinectPV2_Device
* Method:    jniUpdate
* Signature: ()Z
*/
JNIEXPORT jboolean JNICALL Java_KinectPV2_Device_jniUpdate
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	bool result = true;// kinect->update();

	if (result){
		//if (kinect->isColorFrameReady()){
		//}
	}
	return result;
}

/*
* Class:     KinectPV2_Device
* Method:    jniStopDevice
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniStopDevice
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->disable();
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniStopSignal
* Signature: ()Z
*/
JNIEXPORT jboolean JNICALL Java_KinectPV2_Device_jniStopSignal
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	jboolean  val  = (jboolean)kinect->cleanMemory();
	env->DeleteLocalRef(cls);
	return val;
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableColorFrame
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableColorFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableColorImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableDepthFrame
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableDepthMaskFrame
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthMaskFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthMaskImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableInfraredFrame
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableInfraredFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableInFraredImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableBodyTrackFrame
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableBodyTrackFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableBodyTrack((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableLongExposureInfrared
* Signature: (Z)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableInfraredLongExposure
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableInFraredLongExposureImage((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableSkeletonDepth
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableSkeletonDepth
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	if (toggle == JNI_TRUE)
		kinect->enableSkeleton((bool)toggle);
	kinect->enableSkeletonDepthMap((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableSkeletonColor
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableSkeletonColor
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	if (toggle == JNI_TRUE)
		kinect->enableSkeleton((bool)toggle);
	kinect->enableSkeletonColorMap((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableSkeleton3D
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableSkeleton3D
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	if (toggle == JNI_TRUE)
		kinect->enableSkeleton((bool)toggle);
	kinect->enableSkeleton3DMap((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableFaceDetection
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableFaceDetection
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	if (toggle == JNI_TRUE)
		kinect->enableSkeleton((bool)toggle);
	kinect->enableFaceDetection((bool)toggle);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniEnableHDFaceDetection
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableHDFaceDetection
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableSkeleton((bool)toggle);
	kinect->enableHDFaceDetection((bool)toggle);
	env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableColorChannel
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableColorChannelsFrame((bool)toggle);
	env->DeleteLocalRef(cls);
}


JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetColorChannel
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)frame_size_color * 3);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getColorChannel();
	env->SetFloatArrayRegion(buffer, 0, (jsize)frame_size_color * 3, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetDepthData
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetColorData
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_color);
	const jint * pInt = (const jint *)kinect->JNI_GetImage();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_color, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetDepth_16_Data
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetDepth16Data
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetDepth_16_Data();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method :   jniGetDepth_256_Data
* Signature : ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetDepth256Data
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetDepth_256_Data();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

//Raw depth
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetRawDepth16Data
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetDepthRaw_16_Data();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}


JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetRawDepth256Data
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetDepthRaw_256_Data();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetDepthData
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetInfraredData
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetInfrared();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetInfraredLongExposure
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetInfraredLongExposure
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetLongExposureInfrared();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetBodyTrack
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetBodyTrack
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetBodyTrack();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetDepthMask
* Signature: ()[I
*/
JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetDepthMask
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_GetDepthMask();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}



/*
* Class:     KinectPV2_Device
* Method:    jniGetSkeleton3D
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetSkeleton3D
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)JOINTSIZE);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getSkeleton3DMapData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)JOINTSIZE, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetSkeletonDepth
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetSkeletonDepth
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)JOINTSIZE);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getSkeletonDepthMapData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)JOINTSIZE, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetSkeletonColor
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetSkeletonColor
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)JOINTSIZE);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getSkeletonColorMapData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)JOINTSIZE, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetFaceDetection
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetFaceColorData
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)FACESIZE);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getFaceColorRawData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)FACESIZE, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetFaceDetection
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetFaceInfraredData
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)FACESIZE);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getFaceInfraredRawData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)FACESIZE, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetHDFaceDetection
* Signature: ()[I
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetHDFaceDetection
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)HDFACEVERTEX);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_getHDFaceVertexRawData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)HDFACEVERTEX, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}


//POINT CLOUD

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnablePointCloud
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enablePointCloud(bool(toggle));
	env->DeleteLocalRef(cls);
}


JNIEXPORT jintArray JNICALL Java_KinectPV2_Device_jniGetPointCloudDepthImage
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	const jint * pInt = (const jint *)kinect->JNI_pointCloudDepthImage();
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));
	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetPointCloudDeptMap
* Signature: ()[F
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetPointCloudDeptMap
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloat * pFloat = (jfloat *)kinect->JNI_pointCloudPosData();

	jfloatArray buffer = env->NewFloatArray(frame_size_depth * 3);
	env->SetFloatArrayRegion(buffer, 0, frame_size_depth * 3, pFloat);


	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jniGetPointCloudColorMap
* Signature: ()[F
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetPointCloudColorMap
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	jfloatArray buffer = env->NewFloatArray((jsize)frame_size_color * 3);
	const jfloat * pFloat = (const jfloat *)kinect->JNI_pointCloudColorData();
	env->SetFloatArrayRegion(buffer, 0, (jsize)frame_size_color * 3, (const jfloat *)(pFloat));
	env->DeleteLocalRef(cls);

	return buffer;
}
/*
* Class:     KinectPV2_Device
* Method:    jniSetLowThresholdDepthPC
* Signature: (F)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetLowThresholdDepthPC
(JNIEnv * env, jobject obj, jint val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setLowThresholdDepthPC((int)val);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetLowThresholdDepthPC
* Signature: ()F
*/
JNIEXPORT jint JNICALL Java_KinectPV2_Device_jniGetLowThresholdDepthPC
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	float val = kinect->getLowThresholdDepthPC();
	env->DeleteLocalRef(cls);

	return (jint)val;
}

/*
* Class:     KinectPV2_Device
* Method:    jniSetHighThresholdDepthPC
* Signature: (F)V
*/
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetHighThresholdDepthPC
(JNIEnv * env, jobject obj, jint val)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->setHighThresholdDepthPC((jint)val);
	env->DeleteLocalRef(cls);
}

/*
* Class:     KinectPV2_Device
* Method:    jniGetHighThresholdDepthPC
* Signature: ()F
*/
JNIEXPORT jint JNICALL Java_KinectPV2_Device_jniGetHighThresholdDepthPC
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	int val = kinect->getHighThresholdDepthPC();
	env->DeleteLocalRef(cls);

	return (jint)val;
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

//BODY TRACK INFORMATION
JNIEXPORT jintArray JNICALL  Java_KinectPV2_Device_jniGetBodyIndexUser
(JNIEnv * env, jobject obj, jint index)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	const jint * pInt = (const jint *)kinect->JNI_getBodyIndexUser((jint)index);
	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));

	env->DeleteLocalRef(cls);

	return buffer;
}

JNIEXPORT jint JNICALL  Java_KinectPV2_Device_jniGetNumberOfUsers
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	return kinect->JNI_getNumOfUsers();
}

JNIEXPORT jintArray JNICALL  Java_KinectPV2_Device_jniGetBodyTrackIds
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	const jint * pInt = (const jint *)kinect->JNI_getTrackedIds();
	jintArray buffer = env->NewIntArray((jsize)BODY_COUNT);
	env->SetIntArrayRegion(buffer, 0, (jsize)BODY_COUNT, (const jint *)(pInt));

	env->DeleteLocalRef(cls);

	return buffer;
}

JNIEXPORT jintArray JNICALL  Java_KinectPV2_Device_jniGetRawBodyTrack
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);

	const jint * pInt = (const jint *)kinect->JNI_GetRawBodyTrack();
	jintArray buffer = env->NewIntArray((jsize)frame_size_depth);
	env->SetIntArrayRegion(buffer, 0, (jsize)frame_size_depth, (const jint *)(pInt));

	env->DeleteLocalRef(cls);

	return buffer;
}

/*
* Class:     KinectPV2_Device
* Method:    jni_MapCameraPointToDepthSpace
* Signature: (I)V
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniMapCameraPointToDepthSpace
(JNIEnv * env, jobject obj, jfloat valX, jfloat valY, jfloat valZ)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	DepthSpacePoint depthSpacePoint;
	CameraSpacePoint cameraPoint;

	cameraPoint.X = (float)valX;
	cameraPoint.Y = (float)valY;
	cameraPoint.Z = (float)valZ;

	HRESULT hr = kinect->MapCameraPointToDepthSpace(cameraPoint, &depthSpacePoint);

	float depthPoint[2] = { depthSpacePoint.X, depthSpacePoint.Y };

	jfloatArray buffer = env->NewFloatArray((jsize)2);
	env->SetFloatArrayRegion(buffer, 0, (jsize)2, (const jfloat *)(depthPoint));

	env->DeleteLocalRef(cls);

	return buffer;
}


/*
* Class:     KinectPV2_Device
* Method:    jni_MapCameraPointToDepthSpace
* Signature: (I)V
*/
JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniMapCameraPointToColorSpace
(JNIEnv * env, jobject obj, jfloat valX, jfloat valY, jfloat valZ)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	ColorSpacePoint depthSpacePoint;
	CameraSpacePoint cameraPoint;

	cameraPoint.X = (float)valX;
	cameraPoint.Y = (float)valY;
	cameraPoint.Z = (float)valZ;

	HRESULT hr = kinect->MapCameraPointToColorSpace(cameraPoint, &depthSpacePoint);

	float depthPoint[2] = { depthSpacePoint.X, depthSpacePoint.Y };

	jfloatArray buffer = env->NewFloatArray((jsize)2);
	env->SetFloatArrayRegion(buffer, 0, (jsize)2, (const jfloat *)(depthPoint));

	env->DeleteLocalRef(cls);

	return buffer;
}

JNIEXPORT jfloatArray JNICALL Java_KinectPV2_Device_jniGetMapDethToColorSpace(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	//kinect->enableMapDepthToColorData();

	jfloat * pfloat = (jfloat *)kinect->JNI_getMapDepthToColor();

	jfloatArray buffer = env->NewFloatArray((jsize)frame_size_depth * 2);
	env->SetFloatArrayRegion(buffer, 0, (jsize)frame_size_depth * 2, (jfloat *)(pfloat));

	env->DeleteLocalRef(cls);

	return buffer;
}

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableCoordinateMapperRGBDepth
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	
	kinect->enableCoordinateMapper(true);

	env->DeleteLocalRef(cls);
}