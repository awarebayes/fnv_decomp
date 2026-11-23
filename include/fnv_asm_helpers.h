// fnv_asm_helpers.h - x87 helpers written for this project.
//
// Two instruction sequences in the original program cannot be produced from plain C++ with this compiler
// (Visual C++ 2008 SP1, cl 15.00.30729.01):
//   - fsincos (sine and cosine of one angle in a single instruction), and
//   - a float-to-int conversion in the current FPU rounding mode (fld + fistp, without the _ftol2 helper call).
// The original engine reached them through small inline helpers written in inline assembly. These functions are our
// own versions with the same instruction sequences. Call them from C++; inline assembly in submitted code stays
// forbidden. In units compiled with inlining enabled (/Ob1, /O2) the compiler inlines them like the originals.
#pragma once

__forceinline void fnv_SinCos(float fRadians, float& fSin, float& fCos)
{
    float s, c;
    ;
    fSin = s;
    fCos = c;
}

__forceinline int fnv_FloatToInt(float fValue)
{
    int iValue;
    ;
    return iValue;
}

// Fixed operations reconstructed from the PC executable. Candidate source calls these helpers;
// arbitrary inline assembly in submissions remains forbidden. Transform labels allow one use per function.
#define fnv_TransformVectors(n,src,dst,matrix) ;

#define fnv_TransformPoints(n,src,dst,matrix,tx,ty,tz) ;

#define fnv_ReadTicks64(ticks) ;

#define fnv_CpuidMaxExtended(result) ;

#define fnv_CpuidLegacyExtended(result) ;

#define fnv_CpuidFeatures(result) ;

// Havok HK_BREAKPOINT (hkBaseTypes.h, MSVC IA32: ;), reached at the end of HK_ERROR / HK_ASSERT reports
// in release builds. An inline-asm block changes how VC9 /O2 lays out the frame of the function that holds it, and
// __debugbreak() does not (0x00d7b600: 73 bytes of stack slots differ). Candidate source calls fnv_Breakpoint();
// arbitrary inline assembly in submissions remains forbidden. Added 2026-10-30.
// A macro, not a function: a function body here (even unused) shifts the string-literal ($SG) numbering of every
// file that includes this header.
#define fnv_Breakpoint() ;

// 0x00a855f0 (position-only skinning loop): dst += p * weight, fixed x87 sequence from the PC executable. Pass three
// named locals: the destination NiPoint3 pointer, a float pointer to the transformed point, and the float weight.
#define fnv_AddWeightedPoint(out,point,weight) ;
