//
//  Display.cpp
//  DisplayInfo
//
//	Plugin for Unity to get information about connected displays
//
//  Created by Mike Cobb on 14/08/2020.
//  Copyright © 2020 Highly Interactive. All rights reserved.
//

#include <string>
#include <CoreGraphics/CGDisplayConfiguration.h>
using namespace std;

extern "C"
{
#define MAX_GPUS 24

CGDirectDisplayID displayIds[MAX_GPUS];
CGDisplayCount nDisplays = 0;


// Return the number of available displays
int GetDisplayCount ()
{
	if (CGGetOnlineDisplayList(MAX_GPUS, displayIds, &nDisplays) != kCGErrorSuccess)
    {
        return -1;
    }
	
	return nDisplays;
}

//This doesn't seem to work for all screens, but I'm keeping it as a backup
void GetNativeScreenResolutionSometimes (int screenId, int& w, int& h)
{
	auto modes = CGDisplayCopyAllDisplayModes(displayIds[screenId], nullptr);
    auto count = CFArrayGetCount(modes);
    CGDisplayModeRef mode;
	int mw = -1, mh = -1;
	
	for (auto c = count; c--;)
	{
		mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(modes, c);
		auto native = CGDisplayModeGetIOFlags(mode) & 0x02000000;
		
		w = (int)CGDisplayModeGetWidth(mode);
		h = (int)CGDisplayModeGetHeight(mode);
		
		if (w > mw) mw = w;
		if (h > mh) mh = h;
		
		//If native flag is set, return
		if (native != 0)
		{
			CFRelease(modes);
			return;
		}
	}
	
	//If no native flag, return largest resolution
	w = mw;
	h = mh;
	
	CFRelease(modes);
}

//Unfortunately this is the best solution I can find for macOS
void GetNativeScreenResolution (int screenId, int& w, int& h, int& d)
{
	char buffer[128];
	size_t lines = 0;
	string result = "";
	
	FILE* pipe = popen("system_profiler SPDisplaysDataType | grep Resolution | awk '{print $2 \"x\" $4 \";\"}'", "r");
	
	if (!pipe)
	{
		//Use backup method & record debug flag
		GetNativeScreenResolutionSometimes(screenId, w, h);
		d = 2;
		return;
	}
	
	try
	{
		while (fgets(buffer, sizeof buffer, pipe) != NULL)
		{
			if (screenId == lines)
			{
				//Loop through buffer and extract width & height
				for (int i = 0; i < sizeof buffer; i++)
				{
					if (buffer[i] == 'x')
					{
						//Set width
						w = stoi(result);
						result = "";
						continue;
					}
					
					if (buffer[i] == ';')
					{
						//Set height
						h = stoi(result);
						result = "";
						break;
					}
					
					result += buffer[i];
				}
			}
			lines++;
		}
	}
	catch (...)
	{
		pclose(pipe);
		
		//Use backup method & record debug flag
		GetNativeScreenResolutionSometimes(screenId, w, h);
		d = 3;
		return;
	}
	
	//Final error check: Make sure system_profiler reports correct display count
	if (lines != GetDisplayCount())
	{
		d = 1;
		return;
	}
	
	d = 0;
}

void GetDisplayInformation (int* info, int screenId = 0)
{
	//If display does not exist, return
    if (screenId >= GetDisplayCount()) return;
	
	//Get virtual screen resolution
	info[0] = (int)CGDisplayPixelsWide(displayIds[screenId]);
	info[1] = (int)CGDisplayPixelsHigh(displayIds[screenId]);
	
	//Get background store resolution
    auto mode = CGDisplayCopyDisplayMode(displayIds[screenId]);
	info[2] = (int)CGDisplayModeGetPixelWidth(mode);
	info[3] = (int)CGDisplayModeGetPixelHeight(mode);
	
	//Get native screen resolution
	int w, h, d;
	GetNativeScreenResolution(screenId, w, h, d);
	info[4] = w;
	info[5] = h;
	info[13] = d; //debug which method was used
	
	//Get physical display dimensions
	CGSize screenSize = CGDisplayScreenSize(displayIds[screenId]);
	info[6] = (int)(screenSize.width * 1000);
	info[7] = (int)(screenSize.height * 1000);
	
	//Get display bounds
	CGRect displayBounds = CGDisplayBounds(displayIds[screenId]);
	info[8] = displayBounds.origin.x;
	info[9] = displayBounds.origin.y;
	info[10] = displayBounds.size.width;
	info[11] = displayBounds.size.height;
	
//	//TODO: Get refresh rate (this function just returns 0)
//	int r = CGDisplayModeGetRefreshRate(mode);
//	info[12] = r;
}

}
