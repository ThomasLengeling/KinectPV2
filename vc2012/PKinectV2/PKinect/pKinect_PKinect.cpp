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
	result = env->NewIntArray(WIDTH*HEIGHT);
	env->SetIntArrayRegion(result, 0, WIDTH*HEIGHT, (jint *) (kinect->Nui_GetImage()));

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
	result = env->NewIntArray(WIDTH*HEIGHT);
	env->SetIntArrayRegion(result, 0, WIDTH*HEIGHT, (jint *) (kinect->Nui_GetDepth()));
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
	result = env->NewIntArray(WIDTH*HEIGHT);
	env->SetIntArrayRegion(result, 0, WIDTH*HEIGHT, (jint *) (kinect->Nui_GetMask()));
	env->DeleteLocalRef(cls);
	return result;
}

JNIEXPORT void JNICALL Java_pKinect_PKinect_getFigure
	(JNIEnv* env, jobject obj)
{
	PKinect* kinect;
	jclass cls1;
	jfieldID fid1;
	jfieldID fid2;
	jobject obj2;

	cls1 = env->GetObjectClass(obj);
	fid1 = env->GetFieldID(cls1, "ptr", "J");
	kinect = (PKinect *) env->GetLongField(obj, fid1);

	fid2 = env->GetFieldID(cls1, "joints", "[[[F");
	obj2 = env->GetObjectField(obj, fid2);

	jobjectArray result = reinterpret_cast<jobjectArray> (obj2);

	for (int i=0; i<NUI_SKELETON_COUNT; i++) 
	{
		jobjectArray v1 = env->NewObjectArray(NUI_SKELETON_POSITION_COUNT+1, env->FindClass("[F"), 0);

		for (int j=0; j<NUI_SKELETON_POSITION_COUNT+1; j++) 
		{
			jfloat temp[5];
			jfloatArray v2 = env->NewFloatArray(5);
			for (int k=0; k<5; k++) 
			{
				temp[k] = kinect->joints[i][j][k];
			}

			env->SetFloatArrayRegion(v2, 0, 5, temp);
			env->SetObjectArrayElement(v1, j, v2);
			env->DeleteLocalRef(v2);
		}
		env->SetObjectArrayElement(result, i, v1);
		env->DeleteLocalRef(v1);
	}
	env->DeleteLocalRef(result);
	env->DeleteLocalRef(obj2);
	env->DeleteLocalRef(cls1);
}