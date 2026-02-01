#ifndef Optimization_Plugin_H
#define Optimization_Plugin_H


#include <iostream>
#include <vector>
#include "math.h"

#include "../KMO_ep2d_test_OptimPlugin/EnumDef_KM.h"

/* Imported for the OptimParam
struct sOptimParam
{
	int iMode;
	long lTE;
	long lTEPI;
	long lT90;
	long lTRF;
	long lRampTime;
	long ldT;
	double dMaxAmpl;
	long lID;
	long lSpoiler;
	double MomentTarget;
}; */

#ifdef OPTIMP_EXPORTS
    #define OPTIMP_API __declspec(dllexport)
#else
    #define OPTIMP_API __declspec(dllimport)
#endif

// Signature/Entry points functions for the DLL. Please do not change the signature, but you are free to change the content of the functions.
extern "C" OPTIMP_API bool Optimize_Plugin(sOptimParam Timing, std::vector<sGrad> &vGrad, int iWaveform);
extern "C" OPTIMP_API bool Optimize_ARB_Plugin(sOptimParam Timing, std::vector<sGrad> &vGrad, int iWaveform);
extern "C" OPTIMP_API bool GetName_Plugin(int iWaveform,  char** cpName);

// This part is the actual method implementations that you can call from the Entry Points. These functions can only be called from within the DLL and cannot be called from the actual sequence. 
namespace Optimization_ToolBox_KM
{
	bool gradientFactory(std::vector<double>& G, sOptimParam Timing, std::vector<sGrad>& vG);
	bool trap2sin(sOptimParam Timing, std::vector<sGrad>& vG);
}
#endif // Optimization_ToolBox_KM_H
