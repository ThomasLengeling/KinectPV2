// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>

// TODO: reference additional headers your program requires here
#include <ShlObj.h>
#include <Kinect.h>


// Safe release for interfaces
template<class T>
inline void SafeRelease(T *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

template<typename T>
inline void SafeArrayDelete(T*& pArray)
{
	if (pArray != NULL){
		delete[] pArray;
		pArray = nullptr;
	}
}

template<typename T>
inline void SafeDeletePointer(T*& pointer)
{
	if (pointer != NULL){
		free(pointer);
		(pointer) = NULL;
	}
}
