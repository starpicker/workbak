/*----------------------------------------------------------------------------------------------
*
 *This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and		
 *confidential information. 
 *
 *The information and code contained in this file is only for authorized ArcSoft employees 
 *to design, create, modify, or review.
 *
 *DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
 *
 *If you are not an intended recipient of this file, you must not copy, distribute, modify, 
 *or take any action in reliance on it. 
 *
 *If you have received this file in error, please immediately notify ArcSoft and 
 *permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/
/*
  *mdutils.h
 *
  *Reference:
 *
  *Description: Define some imaging utility APIs.
  *
 */

#ifndef _MDUTILS_H_
#define _MDUTILS_H_

#include "amplat.h"
#include "amdisplay.h"
#include "mddef.h"
#include "amimgcodec.h"
#include "amimgerror.h"

#include "platform_export.h"

#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdExifCreate( MHandle hAMCM,
                     HMSTREAM hFileStream,
                     MHandle *phExif );
PLATFORM_EXPORT MRESULT MdExifGetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MLong *plBufSize);

PLATFORM_EXPORT MRESULT MdExifSetFieldData( MHandle hExif,
                            MDWord dwFieldID,
                            MPVoid pDataBuf,
                            MLong lBufSize);

PLATFORM_EXPORT MRESULT MdGetExifFieldData(MHandle hAMCM,
                           HMSTREAM  hFileStream,
                           MDWord *pdwFieldID,
                           MPVoid *ppDataBuf,
                           MLong *plBufSize,
                           MLong lFieldCnt);

PLATFORM_EXPORT MRESULT MdExifDestroy( MHandle hExif );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdGetImgFileInfo( MHandle hAMCM, HMSTREAM hImgStream, LPMDIMGFILEINFO pImgInfo );
PLATFORM_EXPORT MRESULT MdGetImgFileFormat( MHandle hAMCM, HMSTREAM hImgStream, MDWord *pdwFormat );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapAlloc( MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapFree( MBITMAP *pBitmap );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapMerge( MHandle hAMCM, MBITMAP *pBitmap, 
                      MBITMAP *pBitmapFore, PMPOINT pPtForePos, 
                      MBITMAP *pBitmapMask, PMPOINT pPtMaskPos, MLong lOpacity );
PLATFORM_EXPORT MRESULT MdBitmapFillColor( MHandle hAMCM, MBITMAP *pBitmap,
                          MCOLORREF clrFill, PMRECT prtFillRect,
                          MBITMAP *pMask, MLong lOpacity); 
PLATFORM_EXPORT MRESULT MdBitmapResample( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap );
PLATFORM_EXPORT MRESULT MdBitmapCrop( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MRECT *prtCrop );
PLATFORM_EXPORT MRESULT MdBitmapRotate( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MLong lDegree );
PLATFORM_EXPORT MRESULT MdBitmapFlip( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, MDWord dwFlipMode );

//////////////////////////////////////////////////////////////////////////
PLATFORM_EXPORT MRESULT MdBitmapLoad( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoad2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapLoadWithAlphaTrans( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap,MDWord dwAlphaTrans );
PLATFORM_EXPORT MRESULT MdBitmapLoad2WithAlphaTrans( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap,MDWord dwAlphaTrans );
PLATFORM_EXPORT MRESULT MdBitmapSave( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdBitmapSave2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );

PLATFORM_EXPORT MRESULT MdFileResize( MHandle hAMCM, 
                      MVoid *pInputFile, 
                      MLong lWidth, 
                      MLong lHeight, 
                      MDWord dwMaxFileSize, 
                      MVoid *pOutputFile);

PLATFORM_EXPORT MRESULT MdBitmapColorConvert( MHandle hAMCM, MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap );

PLATFORM_EXPORT MRESULT MdBitmapCropRotFlipResample( MHandle hAMCM, 
                            MBITMAP *pSrcBitmap, MBITMAP *pDstBitmap, 
                            MRECT *prtSrc, MRECT *prtDst,
                            MLong lRotateDegree, MDWord dwFlipMode );

PLATFORM_EXPORT MRESULT MdBitmapLoadFast(MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP* pBitmap);
PLATFORM_EXPORT MRESULT MdBitmapLoadFast2(MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP* pBitmap);

//////////////////////////////////////////////////////////////////////////

#define MD_VTEXT_DIRECTION_LEFT_TO_RIGHT			0x00000000
#define MD_VTEXT_DIRECTION_RIGHT_TO_LEFT			0x00000001

#define MD_VTEXT_ALIGN_LEFT							0x00000000
#define MD_VTEXT_ALIGN_RIGHT						0x00000002
#define MD_VTEXT_ALIGN_CENTER_HORZ					0x00000004

#define MD_VTEXT_ALIGN_TOP							0x00000000
#define MD_VTEXT_ALIGN_BOTTOM						0x00000008
#define MD_VTEXT_ALIGN_CENTER_VERT					0x00000010

#define MD_VTEXT_SHAPE_CONVERT						0x00000000
#define MD_VTEXT_SHAPE_ORIGINAL						0x00000020

#define MD_VTEXT_ORDER_REORDER						0x00000000
#define MD_VTEXT_ORDER_ORIGINAL						0x00000040

#define MD_VTEXT_FONT_STYLE_NORMAL					0x00000000
#define MD_VTEXT_FONT_STYLE_ITALIC					0x00000080

#define MD_VTEXT_FONT_WEIGHT_100					0x00000100
#define MD_VTEXT_FONT_WEIGHT_200					0x00000200
#define MD_VTEXT_FONT_WEIGHT_300					0x00000300
#define MD_VTEXT_FONT_WEIGHT_400					0x00000400
#define MD_VTEXT_FONT_WEIGHT_500					0x00000500
#define MD_VTEXT_FONT_WEIGHT_600					0x00000600
#define MD_VTEXT_FONT_WEIGHT_700					0x00000700
#define MD_VTEXT_FONT_WEIGHT_800					0x00000800
#define MD_VTEXT_FONT_WEIGHT_900					0x00000900

#define MD_VTEXT_DECORATION_NONE					0x00000000
#define MD_VTEXT_DECORATION_UNDERLINE				0x00001000
#define MD_VTEXT_DECORATION_OVERLINE				0x00002000
#define MD_VTEXT_DECORATION_LINETHROUGH				0x00004000

#define MD_VTEXT_FONT_WEIGHT_NORMAL					MD_VTEXT_FONT_WEIGHT_400
#define MD_VTEXT_FONT_WEIGHT_BOLD					MD_VTEXT_FONT_WEIGHT_700
//////////////////////////////////////////////////////////////////////////
typedef struct  _tagMDVTEXTINFO
{
	MWChar*		pszText;
	MCOLORREF	clrTxt;
	MLong		lFontSize;
	MPVoid		pDataTTF;
    MDWord      dwDSTypeTTF;
	MDWord		dwFormatTTF;
	MPVoid		pDataBubble;
    MDWord      dwDSTypeBubble;
	MDWord		dwFormatBubble;
	MCOLORREF	clrBubble;
	MBool       bAutoLine;
	MLong       lLineWidth;
	MDWord		dwTextFlag;
}MDVTEXTINFO, *LPMDVTEXTINFO;

PLATFORM_EXPORT MRESULT MdPoint2Mask(MHandle hAMCM, MSIZE sizeSrc, MPOINT *pptPoint, MDWord dwPtNum, MBITMAP *pbmpDstMask, MBool bReverse); 
PLATFORM_EXPORT MRESULT MdEllipse2Mask(MHandle hAMCM, MSIZE sizeSrc, MRECT *prtEllipse, MBITMAP *pbmpDstMask, MBool bReverse); 
PLATFORM_EXPORT MRESULT MdEllipse2Mask2(MHandle hAMCM, MSIZE sizeSrc, MRECT *prtEllipse, 
					   MBITMAP *pbmpDstMask, MRECT *prtMask, MBool bReverse);

PLATFORM_EXPORT MRESULT MdCreateMagicWand(MHandle hAMCM, MBITMAP *pbmpData, MHandle *phMagicWand);
PLATFORM_EXPORT MRESULT MdGetMagicMask(MHandle hMagicWand,  MPOINT *pPoint, MLong lTolerance, MBool bReverse, 
					   MBITMAP **ppbmpMask, MPOINT **ppptPoint, MLong **ppPolyPtNum, MLong *plPolyCount);
PLATFORM_EXPORT MRESULT MdDestroyMagicWand(MHandle hMagicWand);

PLATFORM_EXPORT MRESULT MdDrawShadowWithRect(MBITMAP *pbmpData, MRECT *prtRect, MBITMAP *pbmpShadow, MLong lOpacity, MBool bReverse,
							 AM_FNPROGRESS fnProgress, MVoid *pUserData);
PLATFORM_EXPORT MRESULT MdDrawShadowWithMask(MBITMAP *pbmpData, MBITMAP *pbmpMask, MBITMAP *pbmpShadow, MLong lOpacity,
							 AM_FNPROGRESS fnProgress, MVoid *pUserData);
PLATFORM_EXPORT MRESULT MdUtilsJpgAttachStream(MByte* pHead, MLong *plHeadLen, MBool bHeadAttach, MByte* pTail, MLong lTailLen);

MRESULT MdUtilsAGIFEncode_Create(MHandle hAMCM, HMSTREAM hStream, MSIZE sizeDst, MHandle *hEncode);
MRESULT MdUtilsAGIFEncode_Destroy(MHandle hEncode);
MRESULT MdUtilsAGIFEncode_AddFrame(MHandle hEncode, MBITMAP *pbmpFrame,MPOINT ptOffset);
MRESULT MdUtilsAGIFEncode_SetDurationTime(MHandle hEncode, MLong lDurationTime);
MRESULT MdUtilsAGIFEncode_SetLoopNum(MHandle hEncode, MLong lLoopNum);

MRESULT MdUtilsAGIFDecode_Create(MHandle hAMCM, HMSTREAM hStream, MDWord dwDstPixFormat, MSIZE *psizeDst, MLong *plFrameNum,
								 MHandle *hDecode);
MRESULT MdUtilsAGIFDecode_Destroy(MHandle hDecode);
MRESULT MdUtilsAGIFDecode_DecodeFrame(MHandle hDecode, MBITMAP *pbmpFrame);

PLATFORM_EXPORT MRESULT MdUtilsLoad( MHandle hAMCM, HMSTREAM hFileStream, MDWord dwImgFormat, MBITMAP *pBitmap );
PLATFORM_EXPORT MRESULT MdUtilsLoad2( MHandle hAMCM, MVoid *pFile, MDWord dwImgFormat, MBITMAP *pBitmap );

PLATFORM_EXPORT MRESULT MdMask_Create( MHandle *hMask );
PLATFORM_EXPORT MRESULT MdMask_SetPoint( MHandle hMask,MPOINT *pptPoint );
PLATFORM_EXPORT MRESULT MdMask_Apply( MHandle hMask, MBITMAP *pBitmapMask );
PLATFORM_EXPORT MRESULT MdMask_Destroy( MHandle hMask );

PLATFORM_EXPORT MRESULT MdUtils_FeatherMask(  MBITMAP *pBitmapMask,MLong lHalfRadius );


#ifdef __cplusplus
}
#endif

#endif	/*#ifndef _MDUTILS_H_*/

/*End of file */



