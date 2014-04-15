#include "stdafx.h"
#include "pKinect_PKinect.h"
#include "PKinect.h"

JNIEXPORT void JNICALL Java_pKinect_PKinect_init
	(JNIEnv* env, jobject obj)
{
	PKinect* kinect = new PKinect();
	jclass cls;
	jfieldID fid;

	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	env->SetLongField(obj, fid, (jlong) kinect);
	env->DeleteLocalRef(cls);
}

JNIEXPORT jintArray JNICALL Java_pKinect_PKinect_Nui_1GetImage
	(JNIEnv* env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	jintArray result;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	result = env->NewIntArray(cDepthWidth*cDepthHeight);
	env->SetIntArrayRegion(result, 0, cDepthWidth*cDepthHeight, (jint *) (kinect->Nui_GetImage()));

	env->DeleteLocalRef(cls);
	return result;
}

JNIEXPORT jintArray JNICALL Java_pKinect_PKinect_Nui_1GetDepth
	(JNIEnv* env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	jintArray result;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	result = env->NewIntArray(cDepthWidth*cDepthHeight);
	env->SetIntArrayRegion(result, 0, cDepthWidth*cDepthHeight, (jint *) (kinect->Nui_GetDepth()));
	env->DeleteLocalRef(cls);
	return result;
}

JNIEXPORT jintArray JNICALL Java_pKinect_PKinect_Nui_1GetMask
  (JNIEnv* env, jobject obj)
{
	PKinect* kinect;
	jclass cls;
	jfieldID fid;
	jintArray result;
	
	cls = env->GetObjectClass(obj);
	fid = env->GetFieldID(cls, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid);
	result = env->NewIntArray(cDepthWidth*cDepthHeight);
	env->SetIntArrayRegion(result, 0, cDepthWidth*cDepthHeight, (jint *) (kinect->Nui_GetMask()));
	env->DeleteLocalRef(cls);
	return result;
}