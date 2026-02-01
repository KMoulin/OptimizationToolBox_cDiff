#include "Optimization_Plugin.h"
#include <ostream>
#include <fstream>
#include "stdio.h"
#include "stdlib.h"
#include <string>
#include "string.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <limits>
#include <math.h>    
#include <cmath>                                   // ln
#include <assert.h>                                     // assert
#include <algorithm> // For std::max
#include "MrMeasSrv/SeqFW/libGSL/libGSL.h"

#ifndef M_PI
    const double M_PI = 3.14159265358979323846;
#endif

extern "C" OPTIMP_API bool Optimize_Plugin(sOptimParam Timing, std::vector<sGrad> &vGrad, int iWaveform)
{
	// lt is the duration available for diffusion encoding
	long lt = fSDSRoundDownGRT((Timing.lTE / 2 - Timing.lTRF / 2 - std::max(Timing.lT90, Timing.lTEPI)));
	// if we use spoiler the duration is smaller
	long lt_spoil = fSDSRoundDownGRT((lt- Timing.lSpoiler)/2);
	long fillTimePre=0;
	long fillTimePost=0;
	long st=0;
	vGrad.clear();
	switch(iWaveform)
	{
		case 3: // Bipolar (M0M1)
		
			fillTimePre=Timing.lTE / 2 - Timing.lTRF / 2 - Timing.lT90-lt_spoil*2 - Timing.lSpoiler;
			fillTimePost=Timing.lTE / 2 - Timing.lTRF / 2  - Timing.lTEPI -lt_spoil*2 - Timing.lSpoiler; 
			//  Total Duration , Amplitude Factor, RampUp Time, Is Refocussing?, Has Spoiler?
			vGrad.push_back({st,lt_spoil, 1, Timing.lRampTime,0,0,{}});  // First Gradient D1
			vGrad.push_back({st+=lt_spoil,lt_spoil, -1, Timing.lRampTime,0,0,{}}); // Second Gradient D2
			vGrad.push_back({st+=lt_spoil,fillTimePre, 0, Timing.lRampTime,0,0,{}});  // Fill Time Empty Gradients
			vGrad.push_back({st+=fillTimePre,Timing.lTRF+2*Timing.lSpoiler, 0, Timing.lRampTime,1,1,{}});  // RF Pulse + 2 spoilers
			vGrad.push_back({st+=Timing.lTRF+2*Timing.lSpoiler,lt_spoil, 1, Timing.lRampTime,0,0,{}});  // First Gradient D1
			vGrad.push_back({st+=lt_spoil,lt_spoil, -1, Timing.lRampTime,0,0,{}}); // Second Gradient D1

			break;
			
		case 2: // Bipolar (M0)
		
			fillTimePre=Timing.lTE / 2 - Timing.lTRF / 2 - Timing.lT90-lt_spoil*2 - Timing.lSpoiler;
			fillTimePost=Timing.lTE / 2 - Timing.lTRF / 2  - Timing.lTEPI -lt_spoil*2 -Timing.lSpoiler; 
			//  Total Duration , Amplitude Factor, RampUp Time, Is Refocussing?, Has Spoiler?
			vGrad.push_back({st,lt_spoil, 1, Timing.lRampTime,0,0,{}});  // First Gradient D1
			vGrad.push_back({st+=lt_spoil,lt_spoil, -1, Timing.lRampTime,0,0,{}}); // Second Gradient D2
			vGrad.push_back({st+=lt_spoil,fillTimePre, 0, Timing.lRampTime,0,0,{}});  // Fill Time Empty Gradients
			vGrad.push_back({st+=fillTimePre,Timing.lTRF+2*Timing.lSpoiler, 0, Timing.lRampTime,1,1,{}});  // RF Pulse + 2 spoilers
			vGrad.push_back({st+=Timing.lTRF+2*Timing.lSpoiler,lt_spoil, -1, Timing.lRampTime,0,0,{}});  // First Gradient D1
			vGrad.push_back({st+=lt_spoil,lt_spoil, 1, Timing.lRampTime,0,0,{}}); // Second Gradient D1
			break;
			
		case 1: // Monopolar (M0)
		
			fillTimePre=Timing.lTE / 2 - Timing.lTRF / 2 - Timing.lT90-lt;
			fillTimePost=Timing.lTE / 2 - Timing.lTRF / 2  - Timing.lTEPI -lt; 
			//  Total Duration , Amplitude Factor, RampUp Time, Is Refocussing?, Has Spoiler?
			vGrad.push_back({st,lt, 1, Timing.lRampTime,0,0,{}});  // First Gradient D1
			vGrad.push_back({st+=lt,fillTimePre, 0, Timing.lRampTime,0,0,{}});  // Fill Time Empty Gradients
			vGrad.push_back({st+=fillTimePre,Timing.lTRF, 0, Timing.lRampTime,1,0});  // RF Pulse , no spoilers
			vGrad.push_back({st+=Timing.lTRF,lt, 1, Timing.lRampTime,0,0,{}}); // Second Gradient D1
			break;	
		
		default:
			return false;
	}
	
	// Check up
	for (int i=0; i<vGrad.size(); i++)
	{
		if (vGrad[i].D<2*vGrad[i].Rt)vGrad[i].Rt=vGrad[i].D/2;  // Waveform too short for 2 full ramp time
		//if (vGrad[i].D<0 || vGrad[i].Rt<0) return false; // No negative time
	}
	
	return true;

}	
extern "C" OPTIMP_API bool Optimize_ARB_Plugin(sOptimParam Timing,  std::vector<sGrad> &vGrad, int iWaveform)
{ 
	if(!Optimize_Plugin(Timing,vGrad,  iWaveform)) // Run the trazoid base optimization 
	{
		return false;
	}
	if(!Optimization_ToolBox_KM::trap2sin(Timing, vGrad)) // Convert the trapezoid to sinusoid 
	{
		return false;
	}
	return true;
}

extern "C" OPTIMP_API bool GetName_Plugin(int iWaveform,  char** cpName)
{
	  static const char Option0[] = "Empty";
	  static const char Option1[] = "Monopolar (M0)";
	  static const char Option2[] = "Bipolar (M0)";
	  static const char Option3[] = "Bipolar (M0M1)";
	  
	  if (iWaveform>=10)
	  {
		  cpName[0] = (char*)Option0;
		  return false;
	  }
	  switch(iWaveform)
      {
        case 1:
		    cpName[0] = (char*)Option1;
            return true;
		case 2:
		    cpName[0] = (char*)Option2;
            return true;
		case 3:
		    cpName[0] = (char*)Option3;
            return true;	
		default:
			cpName[0] = (char*)Option0;
            return false;	
	  }			
	  return false;	  
}
bool Optimization_ToolBox_KM::trap2sin(sOptimParam Timing, std::vector<sGrad>& vG)
{
	// Generate sinusoid from the trapezoid duration
	for( int i=0;i<vG.size();i++)
	{		
		int total_points=static_cast<int>(std::floor(static_cast<double>(vG[i].D) / static_cast<double>(Timing.ldT))); // Total number of points of the waveform
		
		vG[i].points.clear();
		vG[i].points.resize(total_points);
		
		for (int k = 0;k < total_points; k++) 
		{
			vG[i].points[k]=static_cast<float>(sin(M_PI *k/total_points)); // [between -1 and 1] later on it's multiplied by the waveform sign
		}
	}
	
	return true;
}