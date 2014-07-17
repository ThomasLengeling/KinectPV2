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
	return result;
}


JNIEXPORT void JNICALL Java_KinectPV2_Device_jniSetWindowSize
(JNIEnv * env, jobject obj, jint width, jint height)
{

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

		if (kinect->infraredFrameReady){
			jint * pInt = (jint *)kinect->JNI_GetInfrared();

			jintArray buffer = env->NewIntArray(frame_size_depth);
			env->SetIntArrayRegion(buffer, 0, frame_size_depth, (jint *)(pInt));

			jclass clazz = env->GetObjectClass(obj);
			jmethodID methodID = env->GetMethodID(clazz, "copyInfraredImg", "([I)V");
			env->CallVoidMethod(obj, methodID, buffer);

			kinect->infraredFrameReady = false;
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
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableColorImage();
	env->DeleteLocalRef(cls);
	std::cout << "color enable" << std::endl;
}
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableDepthFrame
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableDepthImage();
	env->DeleteLocalRef(cls);
}
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniEnableInfraredFrame
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->enableInFraredImage();
	env->DeleteLocalRef(cls);
}
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDisableColorFrame
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->disableColorImage();
	env->DeleteLocalRef(cls);
}
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDisableDepthFrame
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->disableDepthImage();
	env->DeleteLocalRef(cls);
}
JNIEXPORT void JNICALL Java_KinectPV2_Device_jniDisableInfraredFrame
(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "ptr", "J");
	KinectPV2::Device * kinect = (KinectPV2::Device *) env->GetLongField(obj, fid);
	kinect->disabeInfraredImage();
	env->DeleteLocalRef(cls);
}