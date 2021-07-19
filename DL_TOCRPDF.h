#ifndef _DL_TOCRPDF_H_
#define _DL_TOCRPDF_H_

#include "TOCRPDF_API.h"
#include "TOCRPDF.h"
#include "windows.h" // For IN, OUT, LPCSTR
/*
#ifndef IN
#  define IN
#endif

#ifndef OUT
#  define OUT
#endif

#ifndef LPCSTR
#  define const char*
#endif
*/

// error checking the delay-load dll process
const char* TOCRPDF_GetLibName();
bool IsLoaded(LPCSTR lpModFileName, LPCSTR lpProcName);
bool LoadIfNeeded(LPCSTR lpModFileName, LPCSTR lpProcName);

/****************************** C interface *****************************/
/** Extractor **/
long __stdcall DL_PDFExtractorHandle_New(PDFExtractorHandle* pExtractor);
long __stdcall DL_PDFExtractorHandle_Delete(PDFExtractorHandle Extractor);

long __stdcall DL_PDFExtractorMemDocHandle_New(PDFExtractorMemDocHandle* pMemDoc);
long __stdcall DL_PDFExtractorMemDocHandle_Delete(PDFExtractorMemDocHandle MemDoc);

long __stdcall DL_PDFExtractorPageHandle_New(PDFExtractorPageHandle* pPage);
long __stdcall DL_PDFExtractorPageHandle_Delete(PDFExtractorPageHandle Page);

long __stdcall DL_PDFExtractor_Load(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc, const wchar_t* inFileName);
long __stdcall DL_PDFExtractor_GetPageCount(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc, long* pCount);
long __stdcall DL_PDFExtractor_FindPageSize(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage, double* pWidth, double* pHeight);
long __stdcall DL_PDFExtractor_GetTocrImageInfo(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage, OUT TOCRPDF_COLOUR_MODE* pColourMode, OUT double* pDpiX, OUT double* pDpiY);
long __stdcall DL_PDFExtractor_GetPage(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc, PDFExtractorPageHandle hPage, const long nPage);
long __stdcall DL_PDFExtractor_GetRecommendedDPIForPageSize(PDFExtractorHandle hExtractor, const double width, const double height, IN const TOCRPDF_COLOUR_MODE ColourMode, IN OUT double* pDpiX, IN OUT double* pDpiY);
long __stdcall DL_PDFExtractor_PageToDibMem(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage, CHARPTR_WITH_LEN* p_cpwl, const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, bool* pPageIsNotBlank);
long __stdcall DL_PDFExtractor_PageToDibFile(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage, const wchar_t* outputFileName, const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, const double dpiY, bool* pPageIsNotBlank);
long __stdcall DL_PDFExtractor_AddAppendix(PDFExtractorHandle hExtractor, const wchar_t* inputFileName, const wchar_t* outputFileName, const wchar_t* appendix);
long __stdcall DL_PDFExtractor_AddAppendixEx(PDFExtractorHandle hExtractor, const wchar_t* inputFileName, const wchar_t* outputFileName, const TOCRRESULTSEX_EG* pAppendix, const wchar_t* pszTitle, const double width, const double height, const double dpiX, const double dpiY, const TocrResultsInfo* pResultsInfo);
long __stdcall DL_PDFExtractor_GetLastExceptionText(PDFExtractorHandle hExtractor, const long err, CHARPTR_WITH_LEN* p_cpwl);

/** Archiver **/
long __stdcall DL_PDFArchiverHandle_New(OUT PDFArchiverHandle* pArchiver);
long __stdcall DL_PDFArchiverHandle_Delete(IN PDFArchiverHandle hArchiver);

long __stdcall DL_PDFArchiverMemDocHandle_New(OUT PDFArchiverMemDocHandle* pDoc);
long __stdcall DL_PDFArchiverMemDocHandle_Delete(IN PDFArchiverMemDocHandle hDoc);

long __stdcall DL_PDFArchiverHandle_Load(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc, 
									IN const wchar_t* inFileName);
long __stdcall DL_PDFArchiverHandle_Close(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc, 
									IN const wchar_t* inFileName);

long __stdcall DL_PDFArchiverHandle_SaveMonoImage(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
									IN const CHARPTR_WITH_ROW_COLUMN* mono_image, IN const float dpiX, 
									const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo* ResultsInfo);
long __stdcall DL_PDFArchiverHandle_SaveAppendix(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN const wchar_t* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
long __stdcall DL_PDFArchiverHandle_SaveResults(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN const TOCRRESULTSEX_EG* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo);
long __stdcall DL_PDFArchiverHandle_SaveTiff(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN TIFF* tfile, const UINT16 TiffImageNumber, IN const long ResultsPageNumber, IN const TocrResultsInfo* ResultsInfo);
long __stdcall DL_PDFArchiverHandle_SaveAllTiff(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
									TIFF* tfile, IN const long ResultsPageNumber);
long __stdcall DL_PDFArchiverHandle_SaveAllPdf(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN PDFArchiverMemDocHandle hDocIn, IN const long ResultsPageNumber, IN const TocrResultsInfo* ResultsInfo);
long __stdcall DL_PDFArchiverHandle_SaveRGB24(OUT PDFArchiverHandle hArchiver, 
									OUT PDFArchiverMemDocHandle hDoc, IN const RGBPTR_WITH_ROW_COLUMN* rgb_image,
									IN const float dpiX, IN const float dpiY, IN const long ResultsPageNumber);
long __stdcall DL_PDFArchiverHandle_SaveRGBPalette(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
									IN const CHARPTR_WITH_ROW_COLUMN* palette_image, IN const long BitsPerPixel,
									IN const long Width, IN const RGBPTR_WITH_LEN* RGB_palette, 
									IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo* ResultsInfo);

//long __stdcall DL_PDFArchiverHandle_SaveImageAndAppendix(PDFArchiverHandle hArchiver, 
//								PDFArchiverMemDocHandle hDoc, CHARPTR_WITH_LEN mono_image, const long ImagePageNumber,
//								const wchar_t* pAppendix, const long ResultsPageNumber);
long __stdcall DL_PDFArchiverHandle_GetLastExceptionText(PDFArchiverHandle hArchiver,
								const long err, CHARPTR_WITH_LEN* p_ErrText);

/****************************** C++ interface *****************************/

/** The DL_PDFExtractor class uses the PoDoFo lib to parse 
 *  a PDF document and to remove all text it finds
 *  in this PDF document so the remaining images 
 *  can be processed by TOCR.  The TOCR results can
 *  be used by functions here to produce an apendix
 *  to a PDF document.
 */

// forward declarations
class DL_PDFExtractorPage;
class DL_PDFExtractorMemDoc;

class DL_PDFExtractor
{
protected:
	PDFExtractorHandle sds;
	static const unsigned long errlen = 1024;
	char errmsg[errlen];

 public:
    DL_PDFExtractor();
    virtual ~DL_PDFExtractor();

	TOCRPDF_Error Load(OUT DL_PDFExtractorMemDoc& doc, IN const wchar_t* inFileName);

	TOCRPDF_Error GetPage(IN DL_PDFExtractorMemDoc& doc, OUT DL_PDFExtractorPage& buffer,
							IN const long pg);

	TOCRPDF_Error GetPageCount(IN DL_PDFExtractorMemDoc& doc, OUT long& nCount);

	TOCRPDF_Error FindPageSize(IN DL_PDFExtractorPage& page, OUT double& PageWidth, OUT double& PageHeight);

	TOCRPDF_Error GetTocrImageInfo(IN DL_PDFExtractorPage& page, OUT TOCRPDF_COLOUR_MODE& ColourMode,
							OUT double &dpiX, OUT double& dpiY);
	
	// populate dpiX and dpiY with current values if known (0 or less if not known)
	TOCRPDF_Error GetRecommendedDPIForPageSize(IN const double& width, IN const double& height, 
							IN const TOCRPDF_COLOUR_MODE ColourMode, IN OUT double& dpiX, IN OUT double& dpiY);

	TOCRPDF_Error PageToDib(IN DL_PDFExtractorPage& PageBuf, IN CHARPTR_WITH_LEN& cpwl, IN const TOCRPDF_COLOUR_MODE ColourMode,
							IN const double dpiX, IN const double dpiY, OUT bool& pageIsNotBlank);
	TOCRPDF_Error PageToDib(IN DL_PDFExtractorPage& PageBuf, const wchar_t* pageFileNameDib, IN const TOCRPDF_COLOUR_MODE ColourMode,
							IN const double dpiX, IN const double dpiY, OUT bool& pageIsNotBlank);

	TOCRPDF_Error AddAppendix(IN const wchar_t* inputFileName, IN const wchar_t* outputFileName,
		IN const wchar_t* appendix);

	TOCRPDF_Error AddAppendix(IN const wchar_t* inputFileName, IN const wchar_t* outputFileName,
		IN const TOCRRESULTSEX_EG& Appendix, IN const wchar_t* pszTitle, IN const double width,
							IN const double height, IN const double dpiX, IN const double dpiY,
							IN const TocrResultsInfo& ResultsInfo);

	const char* GetLastExceptionText(const TOCRPDF_Error& err);
};

class DL_PDFExtractorPage
{
public:
	DL_PDFExtractorPage();
	~DL_PDFExtractorPage();

protected:
	PDFExtractorPageHandle sds;
	friend DL_PDFExtractor;
};

class DL_PDFExtractorMemDoc
{
public:
	DL_PDFExtractorMemDoc();
	~DL_PDFExtractorMemDoc();

protected:
	PDFExtractorMemDocHandle sds;
	friend DL_PDFExtractor;
};


/** The DL_PDFArchiver class uses the PoDoFo lib to
 *  create a PDF file containing the mono image
 *  sent to TOCR and the results recieved from
 *  TOCR.
 */
// forward declarations
class DL_PDFArchiverMemDoc;

class DL_PDFArchiver
{
protected:
	PDFArchiverHandle sds;
	static const unsigned long errlen = 1024;
	char errmsg[errlen];

 public:
    DL_PDFArchiver();
    virtual ~DL_PDFArchiver();

	TOCRPDF_Error Load(OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* inFileName);
	TOCRPDF_Error Close(OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* inFileName);

	TOCRPDF_Error SaveMonoImage(IN OUT DL_PDFArchiverMemDoc& doc, IN const CHARPTR_WITH_ROW_COLUMN& mono_image,
		IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveAppendix(IN OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* appendix, IN const long ResultsPageNumber,
		IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveResults(IN OUT DL_PDFArchiverMemDoc& doc, IN const TOCRRESULTSEX_EG& appendix, IN const long ResultsPageNumber,
		IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveTiff(IN OUT DL_PDFArchiverMemDoc& doc, IN TIFF& tfile, const UINT16 TiffImageNumber,
		IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveAllTiff(IN OUT DL_PDFArchiverMemDoc& doc, IN TIFF& tfile,
									IN const long ResultsPageNumber);
	TOCRPDF_Error SaveAllPdf(IN OUT DL_PDFArchiverMemDoc& doc, DL_PDFArchiverMemDoc& docIn, 
		IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveRGB24(IN OUT DL_PDFArchiverMemDoc& doc, IN const RGBPTR_WITH_ROW_COLUMN& rgb_image, 
		IN const float dpiX, IN const float dpiY, IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveRGBPalette(IN OUT DL_PDFArchiverMemDoc& doc, IN const CHARPTR_WITH_ROW_COLUMN& palette_image, 
									IN const long BitsPerPixel, IN const long Width,
									IN const RGBPTR_WITH_LEN& RGB_palette, IN const float dpiX, 
									IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo);

	const char* GetLastExceptionText(const TOCRPDF_Error& err);
};

class DL_PDFArchiverMemDoc
{
public:
	DL_PDFArchiverMemDoc();
	~DL_PDFArchiverMemDoc();
		
	TOCRPDF_Error Load(OUT PDFArchiverHandle hArchiver, IN const wchar_t* inFileName);
	TOCRPDF_Error Close(OUT PDFArchiverHandle hArchiver, IN const wchar_t* inFileName);

	TOCRPDF_Error SaveMonoImage(OUT PDFArchiverHandle hArchiver, IN const CHARPTR_WITH_ROW_COLUMN& mono_image,
		IN const float dpiX, IN const float dpiY, IN const long ImagePageNumbe, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveAppendix(OUT PDFArchiverHandle hArchiver, IN const wchar_t* appendix,
		const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveResults(OUT PDFArchiverHandle hArchiver, IN const TOCRRESULTSEX_EG& appendix,
		const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveTiff(OUT PDFArchiverHandle hArchiver, IN TIFF& tfile, const UINT16 TiffImageNumber,
		IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveAllTiff(OUT PDFArchiverHandle hArchiver, IN TIFF& tfile, 
									IN const long ResultsPageNumber);
	TOCRPDF_Error SaveAllPdf(OUT PDFArchiverHandle hArchiver, IN DL_PDFArchiverMemDoc& hDocIn, 
		IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveRGB24(OUT PDFArchiverHandle hArchiver, IN const RGBPTR_WITH_ROW_COLUMN& rgb_image, 
		IN const float dpiX, IN const float dpiY, IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo);
	TOCRPDF_Error SaveRGBPalette(OUT PDFArchiverHandle hArchiver, IN const CHARPTR_WITH_ROW_COLUMN& palette_image, 
									IN const long BitsPerPixel, IN const long Width, 
									IN const RGBPTR_WITH_LEN& RGB_palette, IN const float dpiX, 
									IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo);

protected:
	PDFArchiverMemDocHandle sds;
	friend DL_PDFArchiver;
};

#endif // _DL_PDF_EXTRACTOR_H_
