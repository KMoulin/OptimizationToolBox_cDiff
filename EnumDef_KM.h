
// Regroup all the structures and define that are shared accross classes to avoid cross referring 
#ifndef EnumDef_KM_h
#define EnumDef_KM_h 1

#define MAX_TE_SHORTS					10  // Modif KM #1100: Define the max number of short TEs for T2/ADC calculation.

// Modif KM #404 : Struct for UITooltip
struct sUIParam
{
	double dGmax;
	double Moment0;
	double Moment1;
	double Moment2;
	double Moment3;
	long lRamptime;
	long lTimeTillEncode;
	long lTimeTillEcho;
	long lRfTime;
	long lG1;
	double dAmplG1;
	long lG2;
	double dAmplG2;
	long lG3;
	double dAmplG3;
	long lG4;
	double dAmplG4;
	long lG5;
	double dAmplG5;
	long lG6;
	double dAmplG6;
	long lTurn;
	long lPrefill;
	long lPostfill;
	long lSpoil;
	long TEs[MAX_TE_SHORTS]; // CHECK KM : MAX_TE_SHORTS
};


struct s0Grad // Modif KM #1200: Struct for Optimization
{
    double dStart;
    double dStop;
    double dDuration;
    long lDuration_us;
    double dAmpl;
    long lSign;
	long lRampTime;
	long lAera;
};
struct sWaveform // Modif KM #1200: Struct for Optimization
{
	long D1;
	long D2;
	long D3;
	long D4;
	long D5;
	long D6;
	double A1;
	double A2;
	double A3;
	double A4;
	double A5;
	double A6;
	long Rt1;
	long Rt2;
	long Rt3;
	long Rt4;
	long Rt5;
	long Rt6;
	bool Spoiler;
	long Maxwell;
};
struct sGrad // Modif KM #1200: Struct for Optimization
{
    long   D;
    double A;
    long   Rt;
    bool   Rf;
	bool   Spoiler;
};
struct sOptimParam // Modif KM #1200: Struct for Optimization
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
};

// Modif KM #400 : Add specific definition for UI 
// TTR; Wip lPos should starts at 0 and not 1 ?
	enum EnumDiffSpecialParameters {
		WIP_Shift_Increment = 1,
		WIP_Diff_Scheme,
		WIP_Mixing_Time,
		WIP_Moment_Mode,
		WIP_Nb_Free_Waveform,
		WIP_Moment_Var,
		WIP_Delay,
		WIP_Inner_Volume,
		WIP_Inner_Thck,
		// TTR; WPT new entries ->
		//WIP_Diff_Ice, /* // Modif TTR; #1300: Add Ice/Eva check box*/
		//WIP_Diff_Eva,
		WIP_EchoShift,
		WIP_Diff_Arbitrary,
		WIP_Diff_Moco,
		WIP_MS_Nav,
		WIP_Diff_Eddy,
		WIP_Diff_Optim,
		WIP_Diff_RiseTimeMax,
		// TTR; WPT new entries <-
		WIP_T2_map,
		WIP_T2_map_Array,
		
		WIP_Grad_delta,
		WIP_RF_Sweep,
		WIP_RF_Grad,
		WIP_TRSE_Alpha

	};

	// Modif KM #401 : Enum the options for motion compensations
	enum EnumWaveformMode {
		WM_M0 = 1,
		WM_M0M1,
		WM_M0M1M2,
		WM_M0M1M2M3,
		WM_Free,
		WM_Var
	};

	// Modif KM #402 : Create our own enumerator of waveforms because for us TRSE ~= Bipolar and so on...
	enum EnumDiffusionScheme_KM {

		
		SequenceSchemeTRSE = 1,
        SequenceSchemeBip,
        SequenceSchemeMono,
        SequenceSchemeAsym,
        SequenceSchemeSteam,
        SequenceSchemeaMC,
        SequenceSchemeCODE,
        SequenceSchemeGropt,
        SequenceSchemeENCODE,
        SequenceSchemepSym,
        SequenceSchemeSym,
        SequenceSchemeAMC_3RF6,
		SequenceSchemeAMC_3RF4,
        SequenceSchemeAMC_2RF,
        SequenceSchemeFree,
        SequenceSchemeSpen,
        SequenceSchemeSpenTRSE,
        SequenceSchemeCont,
        SequenceSchemeMonoFixed,
        SequenceSchemeSteamFixed,
        SequenceSchemeOGSE,
        SequenceSchemeTrace,
        SequenceScheme2DE,
        SequenceSchemeMulti,
        SequenceSchemeFactory,
        SequenceSchemeNone,



	};

	// Modif KM #403 : Enumerator for advance refocusing options
	enum EnumInnerVolume_KM {

		SequenceNone = 1,
		SequenceIV,
		SequenceSliceSelect,
		SequenceBir4Refoc,
		SequenceBir4Tip,
		SequenceBirAll,
		SequenceIVRest,
		SequenceIVRest_nograd,
		SequenceRest,
		Sequence2D,
		SequenceAdiabatic,
		SequenceIVAdiabatic
	};


	// Modif KM #408 : Enum for advance parameters (Spiral,Spen,...)
	enum EnumConvexArray {

		WIP_Gradient_Scale = 0,
		WIP_SW_Scale,
		WIP_NInterleaves,
		WIP_FOV_FACT,
		WIP_A1,
		WIP_A2,
		WIP_A3,
		WIP_A4,
		WIP_A5,
		WIP_A6,
		WIP_G1,
		WIP_G2,
		WIP_G3,
		WIP_G4,
		WIP_G5,
		WIP_G6

	};
	
	#endif