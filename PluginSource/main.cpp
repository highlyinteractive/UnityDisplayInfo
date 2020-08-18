//
//  Display.cpp
//  DisplayInfo
//
//	Plugin for Unity to get information about connected displays
//
//  Created by Mike Cobb on 14/08/2020.
//  Copyright © 2020 Highly Interactive. All rights reserved.
//

#include <CoreGraphics/CGDisplayConfiguration.h>

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

void GetNativeScreenResolution (int screenId, int& w, int& h)
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
	int w, h;
	GetNativeScreenResolution(screenId, w, h);
	info[4] = w;
	info[5] = h;
	
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
	
	//TODO: Work out how to get other display info
	//Refresh rate
	//DPI
	//Display Serial/Model/Etc...
	//EDID String
}

}
