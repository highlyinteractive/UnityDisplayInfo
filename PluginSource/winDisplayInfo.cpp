//
//  winDisplayInfo.cpp
//  DisplayInfo
//
//	Plugin for Unity to get information about connected displays on Windows 10
//
//  Created by Mike Cobb on 06/10/2020.
//  Copyright © 2020 Highly Interactive. All rights reserved.
//

#include "stdafx.h"
#include "wtypes.h"
#include <ShellScalingApi.h>
#pragma comment(lib,"Shcore.lib")

#define DllExport __declspec (dllexport)

extern "C"
{
	DllExport int GetDisplayCount()
	{
		return GetSystemMetrics(SM_CMONITORS);
	}

	struct dInfo
	{
		int i = 0;
		HMONITOR hMon = NULL;
	};

	BOOL CALLBACK MonitorEnumProc (HMONITOR hMon, HDC hdcMon, LPRECT lprMon, LPARAM dwData)
	{
		dInfo* info = (dInfo*)dwData;

		if (--info->i < 0)
		{
			info->hMon = hMon;
			return FALSE;
		}

		return TRUE;
	}

	DllExport void GetDisplayInformation(int* info, int screenId = 0)
	{
		//If display does not exist, return with debug flag
		if (screenId >= GetDisplayCount())
		{
			info[13] = -1;
			return;
		}

		DISPLAY_DEVICE dd;
		dd.cb = sizeof(DISPLAY_DEVICE);

		if (!EnumDisplayDevices(NULL, screenId, &dd, 0))
		{
			info[13] = -2;
			return;
		}

		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);

		if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
		{
			info[13] = -3;
			return;
		}

		dInfo display;
		display.i = screenId;
		display.hMon = NULL;

		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&display);

		if (display.hMon == NULL)
		{
			info[13] = -4;
			return;
		}

		MONITORINFO displayInfo;
		displayInfo.cbSize = sizeof(displayInfo);

		if (!GetMonitorInfo(display.hMon, &displayInfo))
		{
			info[13] = -5;
			return;
		}

		//No backing store on PC - just pass native resolution
		info[0] = dm.dmPelsWidth;
		info[1] = dm.dmPelsHeight;

		//Get reported (scaled?) screen resolution
		info[2] = displayInfo.rcMonitor.right - displayInfo.rcMonitor.left;
		info[3] = displayInfo.rcMonitor.bottom - displayInfo.rcMonitor.top;

		//Get native screen resolution
		info[4] = dm.dmPelsWidth;
		info[5] = dm.dmPelsHeight;

		//Physical dimensions not easy to find on PC
		info[6] = -1;
		info[7] = -1;

		//Get display bounds
		info[8] = displayInfo.rcMonitor.left;
		info[9] = displayInfo.rcMonitor.top;
		info[10] = displayInfo.rcMonitor.right - displayInfo.rcMonitor.left;
		info[11] = displayInfo.rcMonitor.bottom - displayInfo.rcMonitor.top;

		//Get display refresh rate
		info[12] = dm.dmDisplayFrequency;

		//Get DPI
		UINT dpix, dpiy;
		HRESULT res1 = GetDpiForMonitor(display.hMon, MDT_RAW_DPI, &dpix, &dpiy);
		if (res1 != S_OK)
		{
			info[13] = -6;
			return;
		}
		info[14] = (int)dpix;
		info[15] = (int)dpiy;

		//Get scaling factor
		DEVICE_SCALE_FACTOR pScale;
		HRESULT res2 = GetScaleFactorForMonitor(display.hMon, &pScale);
		if (res2 != S_OK)
		{
			info[13] = -7;
			return;
		}
		if (pScale == SCALE_100_PERCENT) info[16] = 100;
		if (pScale == SCALE_120_PERCENT) info[16] = 120;
		if (pScale == SCALE_125_PERCENT) info[16] = 125;
		if (pScale == SCALE_140_PERCENT) info[16] = 140;
		if (pScale == SCALE_150_PERCENT) info[16] = 150;
		if (pScale == SCALE_160_PERCENT) info[16] = 160;
		if (pScale == SCALE_175_PERCENT) info[16] = 175;
		if (pScale == SCALE_180_PERCENT) info[16] = 180;
		if (pScale == SCALE_200_PERCENT) info[16] = 200;
		if (pScale == SCALE_225_PERCENT) info[16] = 225;
		if (pScale == SCALE_250_PERCENT) info[16] = 250;
		if (pScale == SCALE_300_PERCENT) info[16] = 300;
		if (pScale == SCALE_350_PERCENT) info[16] = 350;
		if (pScale == SCALE_400_PERCENT) info[16] = 400;
		if (pScale == SCALE_450_PERCENT) info[16] = 450;
		if (pScale == SCALE_500_PERCENT) info[16] = 500;
		if (pScale == DEVICE_SCALE_FACTOR_INVALID) info[16] = -1;

		//Flag zero errors
		info[13] = 0;
	}
}