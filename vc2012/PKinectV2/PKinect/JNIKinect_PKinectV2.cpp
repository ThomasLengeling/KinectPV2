#include "stdafx.h"
#include "JNIKinect_PKinectV2.h"
#include "PKinect.h"

JNIEXPORT void JNICALL Java_JNIKinect_PKinectV2_init
  (JNIEnv * env, jobject obj)
{
	PKinect* kinect = new PKinect();
	jclass cls;
	jfieldID fid;

	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	env->SetLongField(obj, fid, (jlong) kinect);
	env->DeleteLocalRef(cls);
}

JNIEXPORT jintArray JNICALL Java_JNIKinect_PKinectV2_Nui_1GetImage
  (JNIEnv * env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	jintArray result;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	result = env->NewIntArray(cDepthWidth*cDepthHeight);
	env->SetIntArrayRegion(result, 0, cDepthWidth*cDepthHeight, (jint *) (kinect->JNI_GetImage()));

	env->DeleteLocalRef(cls);
	return result;
}

JNIEXPORT jintArray JNICALL Java_JNIKinect_PKinectV2_Nui_1GetMask
  (JNIEnv * env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	jintArray result;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	result = env->NewIntArray(cDepthWidth*cDepthHeight);
	env->SetIntArrayRegion(result, 0, cDepthWidth*cDepthHeight, (jint *) (kinect->JNI_GetDepth()));
	env->DeleteLocalRef(cls);
	return result;
}

JNIEXPORT jstring JNICALL Java_JNIKinect_PKinectV2_Nui_1load
  (JNIEnv * env, jobject obj)
{
	jstring result;
	
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	std::string version = kinect->JNI_version();
	result = env->NewStringUTF(version.c_str());
	env->DeleteLocalRef(cls);	
	return  result;
}

JNIEXPORT void JNICALL Java_JNIKinect_PKinectV2_update
  (JNIEnv * env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	kinect->update();
	env->DeleteLocalRef(cls);
}
