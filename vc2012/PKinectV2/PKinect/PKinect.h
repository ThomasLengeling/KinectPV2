#pragma once

#define WIDTH  640
#define HEIGHT 480

class PKinect
{
private:
	HANDLE	m_hNextImageFrameEvent;
	HANDLE	m_hNextDepthFrameEvent;
	HANDLE	m_hNextSkeletonEvent;

	HANDLE	m_pImageStreamHandle;
	HANDLE	m_pDepthStreamHandle;
	HANDLE	m_pSkeletonStreamHandle;

	HANDLE	m_hEvNuiProcessStop;
	HANDLE	m_hThNuiProcess;

	int		frame_size;
	int*	pixels;
	int*	depth;
	int*	mask;

	LONG*	col_info;
	USHORT*	m_depth;
	
	INuiSensor*	m_pNuiSensor;

public:
	PKinect(void);
	~PKinect(void);

	float	joints[NUI_SKELETON_COUNT][NUI_SKELETON_POSITION_COUNT+1][5];
	HRESULT Nui_Init();
	static DWORD WINAPI Nui_ProcessThread(LPVOID);
	DWORD WINAPI Nui_ProcessThread();
	void Nui_GotImageAlert();
	void Nui_GotDepthAlert();
	void Nui_GotSkeletonAlert();

	int * Nui_GetImage();
	int * Nui_GetDepth();
	int * Nui_GetMask();
};
