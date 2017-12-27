/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized ArcSoft employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify ArcSoft and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*---------------------------------------------------------------------------------------------*/

#ifndef _ADKTRIAL_H_
#define _ADKTRIAL_H_

#include "platform_export.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef AM_ADK_TRIAL
typedef struct 
{
	MByte*	pData;
	MLong	lWidth;
	MLong	lHeight;
	MLong	lLineBytes;
	MDWord	dwPaf;
}ADK_TRIAL_DISPDATA, *LPADK_TRIAL_DISPDATA;

PLATFORM_EXPORT MVoid ADK_DrawTrialLogo(LPADK_TRIAL_DISPDATA pDispData, MBool bTop2Down);
PLATFORM_EXPORT MVoid ADK_DrawTrialLogoEx(MByte* pData, MLong lW, MLong lH, MLong lLineBytes, MDWord dwPaf, MBool bTop2Down);
PLATFORM_EXPORT MBool ADK_TrialLogoGetRect(MLong lW, MLong lH, MBool bTop2Down, MRECT *rtLogo);


#else

#define ADK_DrawTrialLogo(pDispData, b)
#define ADK_DrawTrialLogoEx(pData, w, h, linebytes, paf, b)

#endif /*AM_ADK_TRIAL*/

#ifdef __cplusplus
}
#endif

#endif /*_ADKTRIAL_H_*/