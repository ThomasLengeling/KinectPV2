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
	return  result;
}


JNIEXPORT jboolean JNICALL Java_KinectPV2_Device_jniUpdate
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	bool result = kinect->update();

	if (result == true){
		if (kinect->colorFrameReady){
			jint * pInt = (jint *)kinect->JNI_GetImage();
			
			jintArray buffer = env->NewIntArray(frame_size_color);
			env->SetIntArrayRegion(buffer, 0, frame_size_color, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyColorImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->colorFrameReady = false;
		}
		if ( kinect->depthFrameReady){
			jint * pInt = (jint *)kinect->JNI_GetDepth();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, pInt);

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyDepthImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->depthFrameReady = false;
		}

		if (kinect->bodyIndexReady){
			jint * pInt = (jint *)kinect->JNI_GetBodyTrack();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyBodyTrackImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->bodyIndexReady = false;
		}


		if (kinect->infraredFrameReady){
			jint * pInt = (jint *)kinect->JNI_GetInfrared();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyInfraredImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->infraredFrameReady = false;
		}

		if (kinect->longExposureReady){
			jint * pInt = (jint *)kinect->JNI_GetLongExposureInfrared();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyLongExposureImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->longExposureReady = false;
		}

		if (kinect->isEnableSkeleton()){
			jfloat * pfloat = (jfloat *)kinect->JNI_getSkeletonRawData();

			jfloatArray buffer = env->NewFloatArray(JOINTSIZE);
			env->SetFloatArrayRegion(buffer, 0, JOINTSIZE, (jfloat *)(pfloat));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copySkeletonRawData", "([F)V");
			env->CallVoidMethod(obj, methodID, buffer);
		}
	}

	env->DeleteLocalRef(cls);
	env->DeleteLocalRef(obj);

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
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthFrame
(JNIEnv * env, jobject obj, jboolean toggle)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthImage((bool)toggle);
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
	kinect->enableBodyTrack((bool)toggle);
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

JNIEXPORT void JNICALL Java_KinectPV2_Device_jniGetSkeleton
(JNIEnv * env, jobject obj)
{

}