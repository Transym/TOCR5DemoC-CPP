#ifndef _TOCRPDF_H
#define _TOCRPDF_H

#include <stdlib.h> // For _MAX_PATH.  Feel free to comment this out if your compiler does not have _MAX_PATH or does not like stdlib.h

// the previous line may be commented out so fit a default here
#ifndef _MAX_PATH
// from stdlib.h
#define _MAX_PATH   260 /* max. length of full pathname */
#endif // _MAX_PATH

#include "TOCRPDF_API.h"

typedef struct tagTOCRRESULTSEX_EG TOCRRESULTSEX_EG;

/*
 * TIFF is defined as an incomplete type to hide the
 * library's internal data structures from clients.
 */
typedef struct tiff TIFF;

typedef signed short		VBBOOL;	// in VB Boolean is a signed short True is -1
#define VB_TRUE -1
#define VB_FALSE 0

typedef enum tagTOCRPDF_Error
{
 TOCRPDF_ErrorOK = 0,					/** The default value indicating no error. */
 TOCRPDF_PoDoFo_Exception = 1,			/** PoDoFo::PdfError       */
 TOCRPDF_Standard_Exception = 2,		/** std::exception */
 TOCRPDF_Unknown_Error = 3,				/** catch(...) */
 TOCRPDF_Invalid_PDFExtractor = 4,		/** PDFExtractor is NULL */
 TOCRPDF_Invalid_PDFArchiver = 5,		/** PDFArchiver is NULL */
 TOCRPDF_DLL_not_loaded = 6,			/** function in a delay-loaded dll is NULL */
} TOCRPDF_Error;

typedef enum tagTOCRPDF_COLOUR_MODE
{
 COLOUR_MODE_MONO = 0,
 COLOUR_MODE_GREY8 = 1,
 COLOUR_MODE_RGB24 = 2,
} TOCRPDF_COLOUR_MODE;

typedef struct tagCHARPTR_WITH_LEN
{
	char*	charPtr;
	unsigned long	len;
} CHARPTR_WITH_LEN;

typedef struct tagCHARPTR_WITH_ROW_COLUMN
{
	char*	charPtr;
	unsigned long	row;
	unsigned long	column;
} CHARPTR_WITH_ROW_COLUMN;

typedef struct tagRGB24 {
	unsigned char    rgbtBlue;
    unsigned char    rgbtGreen;
    unsigned char    rgbtRed;
} RGB24;

typedef struct tagRGBPTR_WITH_LEN
{
	RGB24*	rgbPtr;
	unsigned long	len;
} RGBPTR_WITH_LEN;

typedef struct tagRGBPTR_WITH_ROW_COLUMN
{
	RGB24*	rgbPtr;
	unsigned long	row;
	unsigned long	column;
} RGBPTR_WITH_ROW_COLUMN;

#define PDFExtractor_DATA_STORE_LEN 200

typedef struct tagPDFExtractor_DataStore
{
	unsigned char DataStore[PDFExtractor_DATA_STORE_LEN];
} PDFExtractor_DataStore;

#define PDFArchiver_DATA_STORE_LEN 200

typedef struct tagPDFArchiver_DataStore
{
	unsigned char DataStore[PDFArchiver_DATA_STORE_LEN];
} PDFArchiver_DataStore;

#define PDFArchiverMemDoc_DATA_STORE_LEN 450

typedef struct tagPDFArchiverMemDoc_DataStore
{
	unsigned char DataStore[PDFArchiverMemDoc_DATA_STORE_LEN];
} PDFArchiverMemDoc_DataStore;

#define PDFExtractorMemDoc_DATA_STORE_LEN 176

typedef struct tagPDFExtractorMemDoc_DataStore
{
	unsigned char DataStore[PDFExtractorMemDoc_DATA_STORE_LEN];
} PDFExtractorMemDoc_DataStore;

#define PDFExtractorPage_DATA_STORE_LEN 8

typedef struct tagPDFExtractorPage_DataStore
{
	unsigned char DataStore[PDFExtractorPage_DATA_STORE_LEN];
} PDFExtractorPage_DataStore;

typedef struct tagPDFExtractorHandle
{
	PDFExtractor_DataStore* DataHandle;
} PDFExtractorHandle;

typedef struct tagPDFArchiverHandle
{
	PDFArchiver_DataStore* DataHandle;
} PDFArchiverHandle;

typedef struct tagPDFExtractorMemDocHandle
{
	PDFExtractorMemDoc_DataStore* DataHandle;
} PDFExtractorMemDocHandle;

typedef struct tagPDFArchiverMemDocHandle
{
	PDFArchiverMemDoc_DataStore* DataHandle;
} PDFArchiverMemDocHandle;

typedef struct tagPDFExtractorPageHandle
{
	PDFExtractorPage_DataStore* DataHandle;
} PDFExtractorPageHandle;

typedef enum tagTocrResultStage
{
	TRS_Extra_Text_Page = 500, // there was too much text to fit on one page so this page was created to accomodate the excess
	TRS_Text = 400,
	TRS_Processed_Image = 300,
	TRS_Input_Image = 200,
	TRS_Original_Page = 100,
} TocrResultStage;

typedef struct tagTocrResultsInfo
{
	wchar_t OriginalFileName[_MAX_PATH];
	long long OriginalPageNumber;
	TocrResultStage OutputStage;
	bool bContainedInThisDocument;
} TocrResultsInfo;

// C interface
/*********************** Extractor *********************************/
extern "C" TOCRPDF_API long __stdcall PDFExtractor_Constructor(PDFExtractor_DataStore* pExtractor);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_Destructor(PDFExtractor_DataStore* pExtractor);

extern "C" TOCRPDF_API long __stdcall PDFExtractorMemDoc_Constructor(PDFExtractorMemDoc_DataStore* pMemDoc);
extern "C" TOCRPDF_API long __stdcall PDFExtractorMemDoc_Destructor(PDFExtractorMemDoc_DataStore* pMemDoc);

extern "C" TOCRPDF_API long __stdcall PDFExtractorPage_Constructor(PDFExtractorPage_DataStore* pPage);
extern "C" TOCRPDF_API long __stdcall PDFExtractorPage_Destructor(PDFExtractorPage_DataStore* pPage);

extern "C" TOCRPDF_API long __stdcall PDFExtractor_Load(PDFExtractor_DataStore* pExtractor,
	PDFExtractorMemDoc_DataStore* pDoc, const wchar_t* inFileName);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_GetPageCount(PDFExtractor_DataStore* pExtractor,
								PDFExtractorMemDoc_DataStore* pDoc, long* pCount);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_FindPageSize(PDFExtractor_DataStore* pExtractor,
								 PDFExtractorMemDoc_DataStore* pDoc, const long nPage, double* pWidth,
								 double* pHeight);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_GetTocrImageInfo(PDFExtractor_DataStore* pExtractor,
								 PDFExtractorMemDoc_DataStore* pDoc, const long nPage, 
								 TOCRPDF_COLOUR_MODE* pColourMode, double* pDpiX, double* pDpiY);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_Init(PDFExtractor_DataStore* pExtractor,
								PDFExtractorMemDoc_DataStore* pDoc, PDFExtractorPage_DataStore* pPage,
								const long nPage, VBBOOL* pPageIsNotBlank);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_GetRecommendedDPIForPageSize(const double width, const double height,
								const TOCRPDF_COLOUR_MODE ColourMode, double* pDpiX, double* pDpiY);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_PageToDibMem(PDFExtractor_DataStore* pExtractor, 
								PDFExtractorPage_DataStore* pPage, CHARPTR_WITH_LEN* p_cpwl, 
								const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, 
								VBBOOL* pPageIsNotBlank);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_PageToDibFile(PDFExtractor_DataStore* pExtractor, 
								PDFExtractorPage_DataStore* pPage, const wchar_t* outputFileName,
								const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, 
								VBBOOL* pPageIsNotBlank);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_AddAppendix(PDFExtractor_DataStore* pExtractor, 
	const wchar_t* inputFileName, const wchar_t* outputFileName, const TOCRRESULTSEX_EG* pAppendix,
	const wchar_t* pszTitle, const double width, const double height, const double dpiX, const double dpiY);
extern "C" TOCRPDF_API long __stdcall PDFExtractor_GetLastExceptionText(PDFExtractor_DataStore* pExtractor, 
								const TOCRPDF_Error err, CHARPTR_WITH_LEN* p_cpwl);

/*********************** Archiver *********************************/
extern "C" TOCRPDF_API long __stdcall PDFArchiver_Constructor(PDFArchiver_DataStore* pArchiver);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_Destructor(PDFArchiver_DataStore* pArchiver);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveMonoImage(PDFArchiver_DataStore* pArchiver, 
								const wchar_t* outputFileName, const CHARPTR_WITH_ROW_COLUMN* mono_image, 
								const float dpiX, const float dpiY, const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveAppendix(PDFArchiver_DataStore* pArchiver,
	const wchar_t* outputFileName, const wchar_t* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveResults(PDFArchiver_DataStore* pArchiver,
	const wchar_t* outputFileName, const TOCRRESULTSEX_EG* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveTiff(PDFArchiver_DataStore* pArchiver, PDFArchiverMemDoc_DataStore* pDoc,
	TIFF* tfile, const unsigned short TiffImageNumber, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveAllTiff(PDFArchiver_DataStore* pArchiver, 
								PDFArchiverMemDoc_DataStore* pDoc, TIFF* tfile, 
								const long ResultsPageNumber);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveAllPdf(PDFArchiver_DataStore* pArchiver, 
								PDFArchiverMemDoc_DataStore* pDoc, PDFArchiverMemDoc_DataStore* pDocIn, 
								const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveRGB24(PDFArchiver_DataStore* pArchiver, 
								const wchar_t* outputFileName, const RGBPTR_WITH_ROW_COLUMN* rgb24_image, 
								const float dpiX, const float dpiY, const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_SaveRGBPalette(PDFArchiver_DataStore* pArchiver,
								const wchar_t* outputFileName, const CHARPTR_WITH_ROW_COLUMN* palette_image,
								const long BitsPerPixel, const long Width, 
								const RGBPTR_WITH_LEN* RGB_palette, const float dpiX,
								const float dpiY, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long __stdcall PDFArchiver_GetLastExceptionText(PDFArchiver_DataStore* pArchiver, 
								const TOCRPDF_Error err, CHARPTR_WITH_LEN* p_cpwl);


// VB interface
#ifdef _WIN64
// for 64 bit build __stdcall works
#define NO_DECORATION __stdcall
#elif defined _WIN

// for 32 bit build __stdcall doesn't work (VB cant find the fn due to the _ on the front) trying __cdecl
// __cdecl works but then VB debug complains that the stack is unbalanced and some fns dont work
#define NO_DECORATION __stdcall
#else
#define NO_DECORATION
#endif // _WIN64

/*********************** Extractor *********************************/
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_New(PDFExtractorHandle* pExtractor);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_Delete(PDFExtractorHandle Extractor);

extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorMemDocHandle_New(PDFExtractorMemDocHandle* pMemDoc);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorMemDocHandle_Delete(PDFExtractorMemDocHandle MemDoc);

extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorPageHandle_New(PDFExtractorPageHandle* pPage);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorPageHandle_Delete(PDFExtractorPageHandle Page);

extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_Load(PDFExtractorHandle Extractor,
	PDFExtractorMemDocHandle Doc, const wchar_t* inFileName);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_GetPageCount(PDFExtractorHandle Extractor,
								PDFExtractorMemDocHandle Doc, long* pCount);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_FindPageSize(PDFExtractorHandle Extractor,
								 PDFExtractorPageHandle Page, double* pWidth, double* pHeight);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_GetTocrImageInfo(PDFExtractorHandle Extractor,
	PDFExtractorPageHandle Page, TOCRPDF_COLOUR_MODE* pColourMode, double* pDpiX, double* pDpiY);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_GetPage(PDFExtractorHandle Extractor,
								PDFExtractorMemDocHandle Doc, PDFExtractorPageHandle Page, const long nPage);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_GetRecommendedDPIForPageSize(PDFExtractorHandle Extractor,
								const double width, const double height, const TOCRPDF_COLOUR_MODE ColourMode, 
								double* pDpiX, double* pDpiY);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_PageToDibMem(PDFExtractorHandle Extractor, 
								PDFExtractorPageHandle Page, CHARPTR_WITH_LEN* p_DIB, 
								const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, VBBOOL* pPageIsNotBlank);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_PageToDibFile(PDFExtractorHandle Extractor, 
								PDFExtractorPageHandle Page, const wchar_t* outputFileName,
								const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, VBBOOL* pPageIsNotBlank);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_AddAppendix(PDFExtractorHandle Extractor, 
	const wchar_t* inputFileName, const wchar_t* outputFileName, const wchar_t* appendix);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_AddAppendixEx(PDFExtractorHandle Extractor, 
	const wchar_t* inputFileName, const wchar_t* outputFileName, const TOCRRESULTSEX_EG* pAppendix,
	const wchar_t* pszTitle, const double width, const double height, const double dpiX,
								const double dpiY, const TocrResultsInfo* pResultsInfo);
extern "C" TOCRPDF_API long NO_DECORATION PDFExtractorHandle_GetLastExceptionText(PDFExtractorHandle Extractor,
								const long err, CHARPTR_WITH_LEN* p_ErrText);

/*********************** Archiver *********************************/
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_New(PDFArchiverHandle* pArchiver);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_Delete(PDFArchiverHandle Archiver);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverMemDocHandle_New(PDFArchiverMemDocHandle* pMemDoc);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverMemDocHandle_Delete(PDFArchiverMemDocHandle MemDoc);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_Load(PDFArchiverHandle Archiver,
								PDFArchiverMemDocHandle Doc, const wchar_t* inFileName);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_Close(PDFArchiverHandle Archiver,
								PDFArchiverMemDocHandle Doc, const wchar_t* outFileName);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveMonoImage(PDFArchiverHandle Archiver, 
								PDFArchiverMemDocHandle Doc, const CHARPTR_WITH_ROW_COLUMN* mono_image, 
								const float dpiX, const float dpiY, const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveAppendix(PDFArchiverHandle Archiver,
	PDFArchiverMemDocHandle Doc, const wchar_t* appendix,
	const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveResults(PDFArchiverHandle Archiver,
	PDFArchiverMemDocHandle Doc, const TOCRRESULTSEX_EG* appendix,
	const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveTiff(PDFArchiverHandle Archiver, 
	PDFArchiverMemDocHandle Doc, TIFF* tfile, const unsigned short TiffImageNumber,
								const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveAllTiff(PDFArchiverHandle Archiver, 
								PDFArchiverMemDocHandle Doc, TIFF* tfile, 
								const long ImagePageNumber);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveAllPdf(PDFArchiverHandle Archiver, 
								PDFArchiverMemDocHandle Doc, PDFArchiverMemDocHandle DocIn, 
								const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveRGB24(PDFArchiverHandle Archiver, 
								PDFArchiverMemDocHandle Doc, const RGBPTR_WITH_ROW_COLUMN* rgb_image, 
								const float dpiX, const float dpiY, const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);
extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_SaveRGBPalette(PDFArchiverHandle Archiver, 
								PDFArchiverMemDocHandle Doc, const CHARPTR_WITH_ROW_COLUMN* palette_image,
								const long BitsPerPixel, const long Width, 
								const RGBPTR_WITH_LEN* RGB_palette, const float dpiX, 
								const float dpiY, const long ImagePageNumber, const TocrResultsInfo* ResultsInfo);

extern "C" TOCRPDF_API long NO_DECORATION PDFArchiverHandle_GetLastExceptionText(PDFArchiverHandle Archiver,
								const long err, CHARPTR_WITH_LEN* p_ErrText);

// may throw exceptions - please use PDFArchiverHandle_Load
extern "C" TOCRPDF_API long __stdcall PDFArchiverMemDoc_Load(PDFArchiverMemDoc_DataStore* pMemDoc, const wchar_t* inFileName);
// may throw exceptions - please use PDFArchiverHandle_Close
extern "C" TOCRPDF_API long __stdcall PDFArchiverMemDoc_Close(PDFArchiverMemDoc_DataStore* pMemDoc, const wchar_t* outFileName);

#endif // _TOCRPDF_H