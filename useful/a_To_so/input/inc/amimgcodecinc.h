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
/*
 * amimgcodecinc.h
 *
 * Purpose:
 *      
 *
 * Code History:
 * 
 *
 * Code Review:
 *      
 *      
 * Copyright:  (C) 2008, ArcSoft Inc. All rights reserved
 *
 */

#ifndef __IMGCODEC_INC_H__
#define __IMGCODEC_INC_H__

#include "platform_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------properties ---------------------------------------*/
// properties for decoder & encoder
// Note: 
//   decoder supports the following propties:
//      AM_CODEC_PROP_IMGSIZE
//      AM_CODEC_PROP_RESIZE
//      AM_CODEC_PROP_PIXELFORMAT
//      AM_CODEC_PROP_IO_PIXELFORMAT
//      AM_CODEC_PROP_CROP
//      AM_CODEC_PROP_ROTATE
//      AM_CODEC_PROP_FLIP
//   
//   encoder supports the following propties:
//      AM_CODEC_PROP_IMGSIZE
//      AM_CODEC_PROP_PIXELFORMAT
//      AM_CODEC_PROP_IO_PIXELFORMAT
//      AM_CODEC_PROP_QUALITY
//      AM_CODEC_PROP_JPEG_EXIF_DATA
//      
//  for detailed description of the properties, see the following.
//

/**
 *	This property is used to get/set the resolution of file
 *	
 *	Parameter:
 *		pData		[in][out]	pointer to a MSIZE. [out] for decoder, [in] for encoder
 *		lDataSize	[in]		sizeof(MSIZE);
 *	Remark:
 *   decoder use this property to get resolution. encoder use it to set resolution. 
 *
*/
#define AM_CODEC_PROP_IMGSIZE			0x0002

/**
 *	This property is used to set target size for decoder
 *	
 *	Parameter:
 *		pData		[in]	   pointer to MSIZE.
 *		lDataSize	[in]		sizeof(MSIZE);
 *	Remark:
 *   this property only for jpg decoder.  MImgDecoder_Scanlines will base on this property .
 *
*/
#define AM_CODEC_PROP_RESIZE			0x1007

/**
 *	This property is used to get/set original pixel format stored in the file
 *	
 *	Parameter:
 *		pData		[in][out]	pointer to MDWord. [out] for decoder, [in] for encoder
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *   this property is only for JPG codec.  decoder use it to get original pixel format. encoder use it to set original pixel format. 
 *
*/
#define AM_CODEC_PROP_PIXELFORMAT		0x1008

/**
 *	This property is used to get/set input/output pixel format for the decoder/encoder
 *	
 *	Parameter:
 *		pData		[in][out]	pointer to MDWord. [out] for decoder, [in] for encoder
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *		The decoder&encoder should assign specified pixel array format.
 *
*/
#define AM_CODEC_PROP_IO_PIXELFORMAT	0x0003  //0x2002

/**
 *	This property is used to set YUV standard
 * (MPAF_BT601_YUV, MPAF_BT601_YCBCR, MPAF_BT709_YUV, MPAF_BT709_YCBCR)
 *	
 *	Parameter:
 *		pData		pointer to a MDWord
 *		lDataSize	sizeof(MDWord)
 */
#define AM_CODEC_PROP_YUV_BT_TYPE		0x0005

/**
 *	This property is used to set region in target size for decoder
 *	
 *	Parameter:
 *		pData		[in]	   pointer to MRECT.
 *		lDataSize	[in]		sizeof(MRECT);
 *	Remark:
 *   This property is optional, and only for decoder. MImgDecoder_Scanlines will base on this property.
 *
*/
#define AM_CODEC_PROP_CROP			    0x100A

/**
 *	This property is used to set rotate degree for decoder
 *	
 *	Parameter:
 *		pData		[in]	   pointer to MDWord. can be 0, 90, 180, 270. clockwise.
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *      1. This property is optional and only for decoder. MImgDecoder_Scanlines will base on this property .
 *      2. Unsupported currently
 *
*/
#define AM_CODEC_PROP_ROTATE			0x100C

/**
 *	This property is used to set flip mode for decoder
 *	
 *	Parameter:
 *		pData		[in]	   pointer to MDWord. can be AM_FLIPMODE_NONE/AM_FLIPMODE_HORIZ/AM_FLIPMODE_VERT.
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *      1. This property is optional and only for decoder. MImgDecoder_Scanlines will base on this property .
 *      2. Unsupported currently
 *
*/
#define AM_CODEC_PROP_FLIP				0x100D

/**
 *	This property is used to get transparent information
 *	
 *	Parameter:
 *		pData		[in]	   pointer to MBool. MTrue: trans data exists. MFalse: trans data doesnot exist.
 *		lDataSize	[in]		sizeof(MBool);
 *	Remark:
 *      1. This property is optional and only for decoder. 
 *
*/
#define AM_CODEC_PROP_TRANS				0x100E

/**
 *	This property is used to set quality for JPG encoder
 *	
 *	Parameter:
 *		pData		[in]		pointer to MDWord. from 1~100
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *   this property is optional, and only for JPG encoder. 
 *
*/
#define AM_CODEC_PROP_QUALITY			0x2101

/**
 *	This property is used to set the EXIF information to the encoder
 *	
 *	Parameter:
 *		pData		[in]    a MByte* pointer to the exif data
 *		lDataSize	[in]    size of exif data length, in bytes.
 *	Remark:
 *   this property is optional, and only for JPG encoder. 
 *
 */
#define	AM_CODEC_PROP_JPEG_EXIF_DATA	0x2102

/**
 *	This property is used to query the number of frames 
 *	
 *	Parameter:
 *		pData		[in/out]	pointer to a MDWord.
 *		lDataSize	[in]		sizeof(MDWord);
 *	Remark:
 *   This property is optional and only for Animation imageing format, for exsample animation gif will use this property. 
*/
#define AM_CODEC_PROP_FRAME_NUM         0x1306

 /**
 *	This property is used to get/set the index of current frame
 *	
 *	Parameter:
 *		pData		[in/out]	pointer to a MLong.
 *		lDataSize	[in]		sizeof(MLong);
 *	Remark:
 *   This property is optional and only for Animation imageing format, for exsample animation gif will use this property. 
 */
#define AM_CODEC_PROP_FRAME_INDEX       0x1301

/**
 *	This property is used to get the loop times of animation image file
 *	
 *	Parameter:
 *		pData		pointer to an MLong.
 *		lDataSize	sizeof(MLong);
 *  Remark:
 *      if *pData is zero, no loop.
 *      if *pData is -1, infinite loop.
 *      if *PData in 1~65535, loop times.
 *   This property is optional and only for Animation imageing format, for exsample animation gif will use this property. 
 */
#define AM_CODEC_PROP_LOOP_NUM			0x1304

/**
 *	This property is used to get the current frame information of GIF file.
 *	
 *	Parameter:
 *		pData		pointer to an AM_GIF_FRAMEINFO.
 *		lDataSize	sizeof(AM_GIF_FRAMEINFO);
 *	Remark:
 *	    If user want to get a specified frame information, user should set the
 *      frame index firstly. 
 */
#define AM_CODEC_PROP_GIF_FRAMEINFO		0x1305

/**
 *	This property is used to set background bitmap for merging in A-GIF
 *	
 *	Parameter:
 *		pData		pointer to an MBITMAP.
 *		lDataSize	sizeof(MBITMAP);
 *	Remark:
 *	    This property should be set before decode scanlines.
 */
#define AM_CODEC_PROP_GIF_BACKGROUND	0x1306

/**
 *	This property is used to get/set the delay time of current frame 
 *	
 *	Parameter:
 *		pData		[in/out]	pointer to a MLong.
 *		lDataSize	[in]		sizeof(MLong);
 *	Remark:
 *   This property is optional and only for Animation imageing format, for exsample animation gif will use this property. 
 *   Delay time is the time to show on screen when playing, it should be millisecond. 
 */
#define AM_CODEC_PROP_DELAY_TIME		0x4001

/**
 *	This property is used to get the disposal method of current frame 
 *	
 *	Parameter:
 *		pData		[in/out]	pointer to a MLong.
 *		lDataSize	[in]		sizeof(MLong);
 *	Remark:
 *   This property is optional and only for Animation imageing format, for exsample animation gif will use this property. 
 *   The disposal method can be one of the following value: MGIF_DISPOSAL_TO_BACKGROUND,MGIF_DISPOSAL_TO_PREVIOUS,etc.
 */
#define AM_CODEC_PROP_DISPOSAL			0x4002

/**
 *	This property is used to set the reszie algorithm
 *	
 *	Parameter:
 *		pData		[in] MDWord.
 *		lDataSize	[in] sizeof(MDWord);    
 *	Remark:
 *		default MPP_RESIZE_NEAREST_NEIGHBOUR
 *		MPP_RESIZE_NEAREST_NEIGHBOUR		0x001	//Nearest Neighbour Interpolation, high performance and low quality
 *		MPP_RESIZE_BILINEAR					0x002	//Bilinear Interpolation, high quality and low performance
 *		MPP_RESIZE_BICUBIC					0x004	//Reserved, Bicubic Interpolation
 *		
 */
#define AM_CODEC_PROP_RESIZE_ALGORITHM		0x100B

/**
 *	This property is used to set invertion mode of output image data.
 *	
 *	Parameter:
 *		pData		pointer to an MBool. 
 *					MTrue: Need to invert the output data
 *					MFalse: Need not to invert the output data
 *		lDataSize	sizeof(MBool);    
 *	Remark:
 *		if the invertion mode is set to MTrue, the output image data's line order will
 *		become up to down. By default, the image data line order is the same order 
 *		with BMP definition(from down to up).
 */
#define AM_CODEC_PROP_BMP_INVERTION		0x1203

/**
 *	This property is used to get how many times should the PNG file should be decode.
 *	
 *	Parameter:
 *		pData		pointer to a MLong.
 *		lDataSize	sizeof(MLong);
 *	Remark: 
 *		The user should get this property before decode a PNG file, 
 *		then make decode the PNG file *pData times. 
 */
#define AM_CODEC_PROP_PNG_PASS_NUMBER	0x1401 

 /**
 *	This property is used to see the jpeg is progressive or not
 *	
 *	Parameter:
 *		pData		pointer to an MBool
 *		lDataSize	sizeof(MBool)
 */
#define	AM_CODEC_PROP_JPEG_IS_PROGRESSIVE	0x1506

 /**
 *	This property is used to see components of the jpeg is RGB or not
 *	
 *	Parameter:
 *		pData		pointer to an MBool
 *		lDataSize	sizeof(MBool)
 */
#define	AM_CODEC_PROP_JPEG_IS_RGB		0x1507

/**
 *	This property is used to get support transform or not
 *	
 *	Parameter:
 *		pData		pointer to an MBool
 *		lDataSize	sizeof(MBool)
 */
#define	AM_CODEC_PROP_JPEG_IS_SUPPORT_TRANSFORM		0x1512

/**
 *	This property is used to switch decoded stream from file/memory to memory/file stream,
 *  The file and memory streams should be the same jpeg stream, otherwise result is not predictable.
 *	
 *	Parameter:
 *		pData		pointer to a HMStream
 *		lDataSize	sizeof(HMStream)
 */
#define AM_CODEC_PROP_SWITCH_STREAM		0x1603

/**
 *	This property is used to set the external indexing information to jpeg decoder, 
 *  or get the indexing information generated by jpeg decoder internally. 
 *	
 *	Parameter:
 *		pData		pointer to a index buffer
 *		lDataSize	size of pData
 */
#define AM_CODEC_PROP_INDEX_INFO		0x1604

/**
 *	This property is used to set the external indexing length to jpeg decoder, 
 *  or get the indexing length generated by jpeg decoder internally. 
 *	
 *	Parameter:
 *		pData		pointer to a index buffer
 *		lDataSize	sizeof(MLong)
 */
#define AM_CODEC_PROP_INDEX_INFO_SIZE   0x1605

/**
 *	This property is used to set the generating interval of MCU indexing in the encoder.
 *	
 *	Parameter:
 *		pData		pointer to a index interval, 0 - not to index, 1 - all to index, 2 - 1/2 MCUs to index, ...
 *		lDataSize	sizeof(MLong)
 */
#define AM_CODEC_PROP_INDEX_INTERVAL    0x1606 

/**
 *	This property is used to set memory size of the internally generated indexing information. 
 *	Note: This setting may overwrite Index interval which perhaps is specified during the decoder creation.
 *
 *	Parameter:
 *		pData		pointer to a MLong
 *		lDataSize	sizeof(MLong)
 */
#define AM_CODEC_PROP_INDEX_MEMSIZE		0x1607

/**
*	This property is used to internally get the MCU index information from JPEG file. 
*	In other words, this call will execute jpeg compression but not yield at all.
*	
*	Parameter:
*		pData		pointer to a MLong
*		lDataSize	sizeof(MLong)
*/
#define AM_CODEC_PROP_JPEG_INDEX		0x1608


/**
*	This property is used to set the attachability of encoded jpeg data. 
*
*	Parameter:
*		pData		pointer to a MLong, 0 - disable, 1 - enable
*		lDataSize	sizeof(MLong)
*/
#define	AM_CODEC_PROP_ENCODER_ATTACHABLE  0x1609

/**
 *	This property is used to set matrix for svg/swf,
 *  matrix is 1-dimension array with length 6.
 *	
 *	Parameter:
 *		pData		pointer to a MLong
 *		lDataSize	sizeof(MDouble)*6
 */
#define AM_CODEC_PROP_VIEW_MATRIX		0x1801

/**
 *	This property is used to get float size of image(svg/swf)
 *	
 *	Parameter:
 *		pData		pointer to a MDouble
 *		lDataSize	sizeof(MDouble)*2
 */
#define AM_CODEC_PROP_IMGSIZE_FLOAT     0x1802

/**
 *	This property is used to set several MBITMAPs to decoder
 *	
 *	Parameter:
 *		pData		pointer to a MLong
 *		lDataSize	sizeof(MBITMAP)*n
 * 
 *  Remarks:
 *  n is count of MBITMAPs for setting
 */
#define AM_CODEC_PROP_MBITMAP_BLOCKS    0x1901


/*----------------- definition for disposal method to animation image-----------------------------*/
#define MGIF_DISPOSAL_UNSPECIFIED   0
#define MGIF_DISPOSAL_NONE          1
#define MGIF_DISPOSAL_TO_BACKGROUND 2
#define MGIF_DISPOSAL_TO_PREVIOUS   3



//CODEC CAPABILITY:
#define AM_CODEC_CAPABILITY_FRAME		0x00000001
#define AM_CODEC_CAPABILITY_SCANLINE	0x00000002
#define AM_CODEC_CAPABILITY_RANDOMBLOCK	0x00000004
#define AM_CODEC_CAPABILITY_RESIZE      0x00000008
#define AM_CODEC_CAPABILITY_FLIP		0x00000010
#define AM_CODEC_CAPABILITY_QUALITY		0x00000020
#define AM_CODEC_CAPABILITY_ROTATE		0x00000040
#define AM_CODEC_CAPABILITY_EXIF		0x00000080


/*-------------------------------------------- capability ID -------------------------------------*/
//This capability ID is used to check the resizing decoding capability for the decoder
#define AM_CODEC_CAPID_RESIZE			AM_CODEC_CAPABILITY_RESIZE

//This capability ID is used to check the region decoding capability for the decoder
#define AM_CODEC_CAPID_CROP				AM_CODEC_CAPABILITY_RANDOMBLOCK

//This capability ID is used to check the rotation decoding capability for the decoder
#define AM_CODEC_CAPID_ROTATE			AM_CODEC_CAPABILITY_ROTATE

//This capability ID is used to check the flip decoding capability for the decoder
#define AM_CODEC_CAPID_FLIP				AM_CODEC_CAPABILITY_FLIP

//This capability ID is used to check if the codec supports to set the encoding quality for the encoder
#define AM_CODEC_CAPID_QUALITY			AM_CODEC_CAPABILITY_QUALITY

//This capability ID is used to check if the codec supports the EXIF field data for the decoder/encoder
#define AM_CODEC_CAPID_EXIF				AM_CODEC_CAPABILITY_EXIF

//This capability ID is used to check if the codec supports the scanlines decoding/encoding
#define AM_CODEC_CAPID_SCANLINES		AM_CODEC_CAPABILITY_SCANLINE

#define AM_CODEC_CAPID_FRAME			AM_CODEC_CAPABILITY_FRAME
/*-------------------------------------------- capability ID -------------------------------------*/



/*--------------------------------------------------decoder---------------------------------------*/
/**
 *	MIDec_Create
 *		this function used to create decoder handle.
 *	
 *	Parameter:
 *		hAMCM			(in)	AMCM handle
 *		hImgStream		(in)	Handle to the stream
 *      dwFileFormat    (in)    file format for hImgStream, such as AM_JPG, AM_BMP and so on. It can be AM_UNKNOWN if don't knwon file format.
 *		phComponent		(in)	Pointer to component handle.
 *
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *		if dwCodecID is zero, Codec container will use default codec.
 */
PLATFORM_EXPORT
MRESULT MIDec_Create(MHandle hAMCM, HMSTREAM hImgStream, MDWord dwFileFormat, MHandle* phComponent);

/**
 *	MIDec_CreateWithFileName
 *		this function used to create decoder handle.
 *	
 *	Parameter:
 *		hAMCM			(in)	AMCM handle
 *		hImgStream		(in)	Handle to the stream
 *		pszFileName		(in)	file name
 *      dwFileFormat    (in)    file format for file name, such as AM_JPG, AM_BMP and so on. It can be AM_UNKNOWN if don't knwon file format.
 *		phComponent		(in)	Pointer to component handle.
 *
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *		if dwCodecID is zero, Codec container will use default codec.
 */
PLATFORM_EXPORT
MRESULT MIDec_CreateWithFileName(MHandle hAMCM, HMSTREAM hImgStream, MTChar *pszFileName, MDWord dwFileFormat, MHandle *phComponent);

/**
 *	MIDec_CreateEx
 *		this function used to create decoder handle.
 *	
 *	Parameter:
 *		hAMCM			(in)	AMCM handle
 *		hImgStream		(in)	Handle to the stream
 *      dwFileFormat    (in)    file format for hImgStream, such as AM_JPG, AM_BMP and so on. It can be AM_UNKNOWN if don't knwon file format.
 *      dwCapability    (in)    codec capability requested, can be AM_CODEC_CAPID_RESIZE,AM_CODEC_CAPID_CROP,etc.
 *      pdwUIDExcepted  (in)    codec UID excluded, these codec will be not selected in MIDec_CreateEx.
 *      lUIDCount       (in)    length of pdwUIDExcepted
 *		phComponent		(in)	Pointer to component handle.
 *
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *		if pdwUIDExcepted is MNull, all codec registered in AMCM will be candidate.
 */
PLATFORM_EXPORT
MRESULT MIDec_CreateEx(MHandle hAMCM, 
					   HMSTREAM hImgStream, 
					   MDWord dwFileFormat, 
					   MDWord dwCapability, 
					   MDWord* pdwUIDExcepted,
					   MLong lUIDCount,
					   MHandle* phComponent);

/**
 *	MIDec_CreateExWithFileName
 *		this function used to create decoder handle.
 *	
 *	Parameter:
 *		hAMCM			(in)	AMCM handle
 *		hImgStream		(in)	Handle to the stream
 *		pszFileName		(in)	file name
 *      dwFileFormat    (in)    file format for hImgStream, such as AM_JPG, AM_BMP and so on. It can be AM_UNKNOWN if don't knwon file format.
 *      dwCapability    (in)    codec capability requested, can be AM_CODEC_CAPID_RESIZE,AM_CODEC_CAPID_CROP,etc.
 *      pdwUIDExcepted  (in)    codec UID excluded, these codec will be not selected in MIDec_CreateEx.
 *      lUIDCount       (in)    length of pdwUIDExcepted
 *		phComponent		(in)	Pointer to component handle.
 *
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *		if pdwUIDExcepted is MNull, all codec registered in AMCM will be candidate.
 */
PLATFORM_EXPORT
MRESULT MIDec_CreateExWithFileName(MHandle hAMCM, 
					   HMSTREAM hImgStream,
					   MTChar *pszFileName,
					   MDWord dwFileFormat, 
					   MDWord dwCapability, 
					   MDWord* pdwUIDExcepted,
					   MLong lUIDCount,
					   MHandle* phComponent);

/**
 *	MIDec_GetProp
 *		this function is used to get property information from the Codec.
 *	
 *  MRESULT MIDec_GetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);
 *	Parameter:
 *		hComponent		(in)	Component handle
 *		dwPropID		(in)	Property identifier. please refer the amimgdecoder.h/amimgencoder.h
 *		pData			(out)	Property data address to receive the data specified by dwPropID.
 *		lDataSize		(in)	Property data size.
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *		1. The pData should be allocate by the caller with appropriate size. Please refer to the 
 *		   property identifier definition for the data and data size info. 
 *	See also:
 *		The property identifier definition. 
 *	
 */
PLATFORM_EXPORT
MRESULT MIDec_GetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);

/**
 *	MIDec_SetProp
 *		this function is used to set property information to the Codec.
 *
 *  MRESULT MIDec_SetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);
 *	
 *	Parameter:
 *		hComponent		(in)	Component handle
 *		dwPropID		(in)	Property identifier.
 *		pData			(in)	Property data address to store the data specified by dwPropID.
 *		lDataSize		(in)	Property data size.
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *	
 *	See also:
 *		The property identifier definition. 
 *
 */
PLATFORM_EXPORT
MRESULT MIDec_SetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);

/**
 *	MIDec_Scanlines
 *		this function used to decode scanline(s).
 *	
 *  MRESULT MIDec_Scanlines(MHandle hComponent, MByte** ppDataBuf, MLong* plLineBytes, MLong* plStartLine, MLong* plLines);
 *	Parameter:
 *		hComponent		(in)	decode component handle
 *		ppDataBuf		(out)	A pointer to the MByte*[]
 *		plLineBytes		(in)	A pointer to the MLong[]
 *		plStartLine		(in)	A pointer to the MLong
 *		plLines			(in/out)A pointer to the MLong, indicate lines.
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 */
PLATFORM_EXPORT
MRESULT MIDec_Scanlines(MHandle hComponent, MByte** ppDataBuf, MLong* plLineBytes, MLong* plStartLine, MLong* plLines);

/**
 *	MIDec_Destroy
 *		this function is used to destroy component handle.
 *	
 *  MRESULT MIDec_Destroy(MHandle hComponent);
 *	Parameter:
 *		hComponent		(in)	Component handle
 *
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *	
 */
PLATFORM_EXPORT
MRESULT MIDec_Destroy(MHandle hComponent);



/*--------------------------------------------------encoder---------------------------------------*/
/**
 *	MIEnc_Create
 *		this function used to create the encoder handle.
 *	
 *	Parameter:
 *		hAMCM			(in)	AMCM handle
 *		hImgStream	    (in)	HMSTREAM Handle for dest file.
 *		dwFileFormat	(in)	File format such as AM_JPG, AM_BMP and so on.
 *      szImageSize     (in)    File size for source data
 *      dwFilePixFormat (in)    Pixel format for output file
 *		phComponent		(out)	Pointer to component handle.
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *		When dwCodecID is zero, container will use a preferential CODEC.
 */
PLATFORM_EXPORT
MRESULT MIEnc_Create (MHandle hAMCM, HMSTREAM hImgStream, MDWord dwFileFormat,
					  MSIZE szImageSize, MDWord dwFilePixFormat, MHandle* phComponent);

PLATFORM_EXPORT
MRESULT MIEnc_CreateEx(MHandle hAMCM, 
					   HMSTREAM hImgStream,
					   MDWord dwFileFormat,
					   MSIZE	szImageSize,
					   MDWord dwFilePixFormat, 
					   MDWord dwCapability, 
					   MDWord* pdwUIDExcepted,
					   MLong lUIDCount,
					   MHandle* phComponent);
/**
 *	MIEnc_GetProp
 *		this function is used to get property information from the Codec.
 *	
 *  MRESULT MIEnc_GetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);
 *
 *	Parameter:
 *		hComponent		(in)	Component handle
 *		dwPropID		(in)	Property identifier. please refer the amimgdecoder.h/amimgencoder.h
 *		pData			(out)	Property data address to receive the data specified by dwPropID.
 *		lDataSize		(in)	Property data size.
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *		1. The pData should be allocate by the caller with appropriate size. Please refer to the 
 *		   property identifier definition for the data and data size info. 
 *	See also:
 *		The property identifier definition. 
 *	
 */
PLATFORM_EXPORT
MRESULT MIEnc_GetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);

/**
 *	MIEnc_SetProp
 *		this function is used to set property information to the Codec.
 *	
 *  MRESULT MIEnc_SetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);
 *	Parameter:
 *		hComponent		(in)	Component handle
 *		dwPropID		(in)	Property identifier.
 *		pData			(in)	Property data address to store the data specified by dwPropID.
 *		lDataSize		(in)	Property data size.
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *	
 *	See also:
 *		The property identifier definition. 
 *
 */
PLATFORM_EXPORT
MRESULT MIEnc_SetProp(MHandle hComponent, MDWord dwPropID, MPVoid pData, MLong lDataSize);

/**
 *	MIEnc_Scanlines
 *		this function used to encode scanline(s).
 *	
 *  MRESULT MIEnc_Scanlines(MHandle hComponent, MByte** ppDataBuf, MLong* plLineBytes, MLong* plEncLines);
 *	Parameter:
 *		hComponent		(in)	decode component handle
 *		ppDataBuf		(out)	A pointer to the MByte*[]
 *		plLineBytes		(in)	A pointer to the MLong[]
 *		plEncLines		(in/out)A pointer to the MLong, indicate lines.
 *	Return:
 *		Returns MOK if successful, or an error value otherwise
 *
 *	Remark:
 *
 */
PLATFORM_EXPORT
MRESULT MIEnc_Scanlines(MHandle hComponent, MByte** ppDataBuf, MLong* plLineBytes, MLong* plEncLines);

/**
 *	MIEnc_Destroy
 *		this function is used to destroy component handle.
 *	
 *  MRESULT MIEnc_Destroy(MHandle hComponent);
 *	Parameter:
 *		hComponent		(in)	Component handle
 *
 *	Return:
 *		return MOK if successful, error code otherwise.
 *
 *	Remark:
 *	
 */
PLATFORM_EXPORT
MRESULT MIEnc_Destroy(MHandle hComponent);


#if defined( __cplusplus )
}
#endif


#endif //__IMGCODEC_INC_H__

// End of file
