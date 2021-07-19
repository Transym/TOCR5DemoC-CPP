#include "DL_TOCRPDF.h"
#include "TOCRdll.h"
//#include "string.h"

#if defined(_WIN64) || defined(WIN64)
  #if defined(_DEBUG)
    #define DLLNAME "TOCRPDF64d.dll"
  #else
    #define DLLNAME "TOCRPDF64.dll"
  #endif
#elif defined(_WIN32) || defined(WIN32)
  #if defined(_DEBUG)
    #define DLLNAME "TOCRPDF32d.dll"
  #else
    #define DLLNAME "TOCRPDF32.dll"
  #endif
#endif

/*******************************************************************
** error checking the delay-load dll process
*******************************************************************/
bool IsLoaded(LPCSTR lpModFileName, LPCSTR lpProcName) {
	bool found = false;
	HMODULE mod = GetModuleHandle(lpModFileName);

	if(mod) {
		FARPROC proc = GetProcAddress(mod, lpProcName);

		if(proc) {
			found = true;
		}
	}

	return found;
}

bool LoadIfNeeded(LPCSTR lpModFileName, LPCSTR lpProcName) {
	bool found = IsLoaded(lpModFileName, lpProcName);

	if(!found) {
		LoadLibrary(lpModFileName);
		found = IsLoaded(lpModFileName, lpProcName);
	}
	
	return found;
}

const char* TOCRPDF_GetLibName() { return DLLNAME; }

void TOCRPDF_GetLastExceptionText(CHARPTR_WITH_LEN* p_cpwl) {
	if(p_cpwl) {
		if(p_cpwl->len > 0) {
			if(p_cpwl->len > 1) {
				const char* pMsg = "DLL did not load.";
				size_t len = strlen(pMsg);
				if(len > 0) {
					strncpy_s(p_cpwl->charPtr, p_cpwl->len, pMsg, len);

					if(len < p_cpwl->len) {
						p_cpwl->charPtr[len] = NULL;
					} else {
						p_cpwl->charPtr[p_cpwl->len - 1] = NULL;
					}
				} else { // len == 0
					p_cpwl->charPtr[0] = NULL;
				}
			} else { // p_cpwl->len == 1
				p_cpwl->charPtr[0] = NULL;
			}
		}
	}
}

/****************************** C interface *****************************/
/** Extractor **/
long __stdcall DL_PDFExtractorHandle_New(PDFExtractorHandle* pExtractor){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_New")) {
		errorid = PDFExtractorHandle_New(pExtractor);
	}

	return errorid;
}

long __stdcall DL_PDFExtractorHandle_Delete(PDFExtractorHandle hExtractor){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_Delete")) {
		errorid = PDFExtractorHandle_Delete(hExtractor);
	}

	return errorid;
}

long __stdcall DL_PDFExtractorMemDocHandle_New(PDFExtractorMemDocHandle* pMemDoc){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorMemDocHandle_New")) {
		errorid = PDFExtractorMemDocHandle_New(pMemDoc);
	}

	return errorid;
}

long __stdcall DL_PDFExtractorMemDocHandle_Delete(PDFExtractorMemDocHandle hMemDoc){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorMemDocHandle_Delete")) {
		errorid = PDFExtractorMemDocHandle_Delete(hMemDoc);
	}

	return errorid;
}

long __stdcall DL_PDFExtractorPageHandle_New(PDFExtractorPageHandle* pPage){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorPageHandle_New")) {
		errorid = PDFExtractorPageHandle_New(pPage);
	}

	return errorid;
}

long __stdcall DL_PDFExtractorPageHandle_Delete(PDFExtractorPageHandle hPage){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorPageHandle_Delete")) {
		errorid = PDFExtractorPageHandle_Delete(hPage);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_Load(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc,
	const wchar_t* inFileName) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_Load")) {
		errorid = PDFExtractorHandle_Load(hExtractor, hDoc, inFileName);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_GetPageCount(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc,
											long* pCount) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetPageCount")) {
		errorid = PDFExtractorHandle_GetPageCount(hExtractor, hDoc, pCount);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_FindPageSize(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage,
										double* pWidth, double* pHeight) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_FindPageSize")) {
		errorid = PDFExtractorHandle_FindPageSize(hExtractor, hPage, pWidth, pHeight);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_GetTocrImageInfo(PDFExtractorHandle hExtractor, PDFExtractorPageHandle hPage,
										 OUT TOCRPDF_COLOUR_MODE* pColourMode, OUT double* pDpiX, OUT double* pDpiY) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if (LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetTocrImageInfo")) {
		errorid = PDFExtractorHandle_GetTocrImageInfo(hExtractor, hPage, pColourMode, pDpiX, pDpiY);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_GetPage(PDFExtractorHandle hExtractor, PDFExtractorMemDocHandle hDoc,
								PDFExtractorPageHandle hPage, const long nPage) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetPage")) {
		errorid = PDFExtractorHandle_GetPage(hExtractor, hDoc, hPage, nPage);
	}

	return errorid;
}

// populate dpiX and dpiY with current values if known (0 or less if not known)
long __stdcall DL_PDFExtractor_GetRecommendedDPIForPageSize(PDFExtractorHandle hExtractor,
			const double width, const double height, IN const TOCRPDF_COLOUR_MODE ColourMode, IN OUT double* pDpiX, IN OUT double* pDpiY) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetRecommendedDPIForPageSize")) {
		errorid = PDFExtractorHandle_GetRecommendedDPIForPageSize(hExtractor, width, height, ColourMode, pDpiX, pDpiY);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_PageToDibMem(PDFExtractorHandle hExtractor,
						PDFExtractorPageHandle hPage, CHARPTR_WITH_LEN* p_cpwl, 
						const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX,  
						const double dpiY, bool* pPageIsNotBlank) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_PageToDibMem")) {
		VBBOOL vbPageIsNotBlank;
		errorid = PDFExtractorHandle_PageToDibMem(hExtractor, hPage, p_cpwl, ColourMode, dpiX, dpiY, &vbPageIsNotBlank);

		if(TOCRPDF_ErrorOK == errorid) {
			if(pPageIsNotBlank) {
				if(VB_FALSE == vbPageIsNotBlank) {
					*pPageIsNotBlank = false;
				} else {
					*pPageIsNotBlank = true;
				}
			}
		}
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_PageToDibFile(PDFExtractorHandle hExtractor,
	PDFExtractorPageHandle hPage, const wchar_t* pageFileNameDib,
						const TOCRPDF_COLOUR_MODE ColourMode, const double dpiX, 
						const double dpiY, bool* pPageIsNotBlank) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_PageToDibFile")) {
		VBBOOL vbPageIsNotBlank;
		errorid = PDFExtractorHandle_PageToDibFile(hExtractor, hPage, pageFileNameDib, ColourMode, dpiX, dpiY, &vbPageIsNotBlank);

		if(TOCRPDF_ErrorOK == errorid) {
			if(pPageIsNotBlank) {
				if(VB_FALSE == vbPageIsNotBlank) {
					*pPageIsNotBlank = false;
				} else {
					*pPageIsNotBlank = true;
				}
			}
		}
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_AddAppendix(PDFExtractorHandle hExtractor, 
	const wchar_t* inputFileName, const wchar_t* outputFileName,
	const wchar_t* appendix) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_AddAppendix")) {
		errorid = PDFExtractorHandle_AddAppendix(hExtractor, inputFileName, outputFileName, appendix);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_AddAppendixEx(PDFExtractorHandle hExtractor, 
	const wchar_t* inputFileName, const wchar_t* outputFileName,
	const TOCRRESULTSEX_EG* pAppendix, const wchar_t* pszTitle, const double width,
								const double height, const double dpiX, const double dpiY, const TocrResultsInfo* pResultsInfo) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_AddAppendixEx")) {
		errorid = PDFExtractorHandle_AddAppendixEx(hExtractor, inputFileName, outputFileName, 
			pAppendix, pszTitle, width, height, dpiX, dpiY, pResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFExtractor_GetLastExceptionText(PDFExtractorHandle hExtractor, 
								const long err, CHARPTR_WITH_LEN* p_cpwl) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if((LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetLastExceptionText")) && (TOCRPDF_DLL_not_loaded != err)) {
		errorid = PDFExtractorHandle_GetLastExceptionText(hExtractor, err, p_cpwl);
	}
	else {
		TOCRPDF_GetLastExceptionText(p_cpwl);
	}

	return errorid;
}

/** Archiver **/
long __stdcall DL_PDFArchiverHandle_New(PDFArchiverHandle* pArchiver){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_New")) {
		errorid = PDFArchiverHandle_New(pArchiver);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_Delete(PDFArchiverHandle hArchiver){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_Delete")) {
		errorid = PDFArchiverHandle_Delete(hArchiver);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverMemDocHandle_New(PDFArchiverMemDocHandle* pDoc){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverMemDocHandle_New")) {
		errorid = PDFArchiverMemDocHandle_New(pDoc);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverMemDocHandle_Delete(PDFArchiverMemDocHandle hDoc){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverMemDocHandle_Delete")) {
		errorid = PDFArchiverMemDocHandle_Delete(hDoc);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_Load(PDFArchiverHandle hArchiver, 
								PDFArchiverMemDocHandle hDoc, const wchar_t* inFileName){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_Load")) {
		errorid = PDFArchiverHandle_Load(hArchiver, hDoc, inFileName);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_Close(PDFArchiverHandle hArchiver, 
								PDFArchiverMemDocHandle hDoc, const wchar_t* outFileName){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_Close")) {
		errorid = PDFArchiverHandle_Close(hArchiver, hDoc, outFileName);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveMonoImage(PDFArchiverHandle hArchiver, 
								PDFArchiverMemDocHandle hDoc, const CHARPTR_WITH_ROW_COLUMN* mono_image, 
								IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo* ResultsInfo){
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveMonoImage")) {
		errorid = PDFArchiverHandle_SaveMonoImage(hArchiver, hDoc, mono_image, dpiX, dpiY, ImagePageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveAppendix(PDFArchiverHandle hArchiver,
	PDFArchiverMemDocHandle hDoc, const wchar_t* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo){
	long errorid = TOCRPDF_DLL_not_loaded;

	if (LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveAppendix")) {
		errorid = PDFArchiverHandle_SaveAppendix(hArchiver, hDoc, appendix, ResultsPageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveResults(PDFArchiverHandle hArchiver,
	PDFArchiverMemDocHandle hDoc, const TOCRRESULTSEX_EG* appendix, const long ResultsPageNumber, const TocrResultsInfo* ResultsInfo){
	long errorid = TOCRPDF_DLL_not_loaded;

	if (LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveResults")) {
		errorid = PDFArchiverHandle_SaveResults(hArchiver, hDoc, appendix, ResultsPageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveTiff(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN TIFF* tfile, const UINT16 TiffImageNumber, IN const long ResultsPageNumber, IN const TocrResultsInfo* ResultsInfo)
{
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveTiff")) {
		errorid = PDFArchiverHandle_SaveTiff(hArchiver, hDoc, tfile, TiffImageNumber, ResultsPageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveAllTiff(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
												IN TIFF* tfile, IN const long ResultsPageNumber)
{
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveAllTiff")) {
		errorid = PDFArchiverHandle_SaveAllTiff(hArchiver, hDoc,  tfile, ResultsPageNumber);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveAllPdf(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
	IN PDFArchiverMemDocHandle hDocIn, IN const long ResultsPageNumber, IN const TocrResultsInfo* ResultsInfo)
{
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveAllPdf")) {
		errorid = PDFArchiverHandle_SaveAllPdf(hArchiver, hDoc, hDocIn, ResultsPageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveRGB24(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
											  IN const RGBPTR_WITH_ROW_COLUMN* rgb_image,
											  IN const float dpiX, IN const float dpiY, 
											  IN const long ResultsPageNumber, IN const TocrResultsInfo* ResultsInfo)
{
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveRGB24")) {
		errorid = PDFArchiverHandle_SaveRGB24(hArchiver, hDoc, rgb_image, dpiX, dpiY, ResultsPageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_SaveRGBPalette(OUT PDFArchiverHandle hArchiver, OUT PDFArchiverMemDocHandle hDoc,
									IN const CHARPTR_WITH_ROW_COLUMN* palette_image, IN const long BitsPerPixel,
									IN const long Width, IN const RGBPTR_WITH_LEN* RGB_palette, 
									IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo* ResultsInfo)
{
	long errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_SaveRGBPalette")) {
		errorid = PDFArchiverHandle_SaveRGBPalette(hArchiver, hDoc, palette_image, BitsPerPixel, Width, RGB_palette, dpiX, dpiY, ImagePageNumber, ResultsInfo);
	}

	return errorid;
}

long __stdcall DL_PDFArchiverHandle_GetLastExceptionText(PDFArchiverHandle hArchiver,
								const long err, CHARPTR_WITH_LEN* p_cpwl) {
	long errorid = TOCRPDF_DLL_not_loaded;

	if((LoadIfNeeded(DLLNAME, "PDFArchiverHandle_GetLastExceptionText")) && (TOCRPDF_DLL_not_loaded != err)) {
		errorid = PDFArchiverHandle_GetLastExceptionText(hArchiver, err, p_cpwl);
	}
	else {
		TOCRPDF_GetLastExceptionText(p_cpwl);
	}

	return errorid;
}


/****************************** C++ interface *****************************/


/*******************************************************************
** PDFExtractorPage
*******************************************************************/

DL_PDFExtractorPage::DL_PDFExtractorPage() {
	DL_PDFExtractorPageHandle_New(&sds);
}

DL_PDFExtractorPage::~DL_PDFExtractorPage() {
	DL_PDFExtractorPageHandle_Delete(sds);
}

/*******************************************************************
** PDFExtractorMemDoc
*******************************************************************/

DL_PDFExtractorMemDoc::DL_PDFExtractorMemDoc() {
	// run constructor on data store
	DL_PDFExtractorMemDocHandle_New(&sds);
}

DL_PDFExtractorMemDoc::~DL_PDFExtractorMemDoc() {
	// run destructor on data store
	DL_PDFExtractorMemDocHandle_Delete(sds);
}

/*******************************************************************
** PDFExtractor
*******************************************************************/
DL_PDFExtractor::DL_PDFExtractor() {
	// run constructor on data store
	DL_PDFExtractorHandle_New(&sds);
}

DL_PDFExtractor::~DL_PDFExtractor() {
	// run destructor on data store
	DL_PDFExtractorHandle_Delete(sds);
}

// populate dpiX and dpiY with current values if known (0 or less if not known)
TOCRPDF_Error DL_PDFExtractor::GetRecommendedDPIForPageSize(IN const double& width, 
				IN const double& height, IN const TOCRPDF_COLOUR_MODE ColourMode, IN OUT double& dpiX, IN OUT double& dpiY) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_GetRecommendedDPIForPageSize(sds, width, height, ColourMode, &dpiX, &dpiY);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::Load(OUT DL_PDFExtractorMemDoc& doc, IN const wchar_t* inFileName) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_Load(sds, doc.sds, inFileName);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::GetPage(IN DL_PDFExtractorMemDoc& doc, OUT DL_PDFExtractorPage& buffer,
	IN const long pg) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_GetPage(sds, doc.sds, buffer.sds, pg);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::FindPageSize(IN DL_PDFExtractorPage& page,
						OUT double& PageWidth, OUT double& PageHeight) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_FindPageSize(sds, page.sds, &PageWidth, &PageHeight);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::GetTocrImageInfo(IN DL_PDFExtractorPage& page,
						OUT TOCRPDF_COLOUR_MODE& ColourMode, OUT double &dpiX, OUT double& dpiY) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_GetTocrImageInfo(sds, page.sds, &ColourMode, &dpiX, &dpiY);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::AddAppendix(IN const wchar_t* inputFileName, IN const wchar_t* outputFileName,
	IN const wchar_t* appendix) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_AddAppendix(sds, inputFileName, outputFileName, appendix);

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::AddAppendix(IN const wchar_t* inputFileName, IN const wchar_t* outputFileName,
	IN const TOCRRESULTSEX_EG& Appendix, IN const wchar_t* pszTitle, IN const double width,
							IN const double height, IN const double dpiX, IN const double dpiY, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_AddAppendixEx(sds, inputFileName, outputFileName, 
			&Appendix, pszTitle, width, height, dpiX, dpiY, &ResultsInfo);

	return errorid;
}


TOCRPDF_Error DL_PDFExtractor::GetPageCount(IN DL_PDFExtractorMemDoc& doc, OUT long& nCount) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFExtractor_GetPageCount(sds, doc.sds, &nCount);

	return errorid;
}

const char* DL_PDFExtractor::GetLastExceptionText(const TOCRPDF_Error& err) {
	CHARPTR_WITH_LEN msg_box;
	msg_box.charPtr = errmsg;
	msg_box.len = errlen;
	TOCRPDF_Error errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_GetLastExceptionText")) {
		errorid = (TOCRPDF_Error)PDFExtractorHandle_GetLastExceptionText(sds, err, &msg_box);

		if(TOCRPDF_ErrorOK != errorid) {
			return "Could not find a description for error.";
		}
		else {
			return msg_box.charPtr;
		}
	}
	else {
		return "DLL did not load.";
	}
}

TOCRPDF_Error DL_PDFExtractor::PageToDib(IN DL_PDFExtractorPage& PageBuf, IN CHARPTR_WITH_LEN& cpwl, 
	IN const TOCRPDF_COLOUR_MODE ColourMode, IN const double dpiX, IN const double dpiY, OUT bool& pageIsNotBlank) {
	TOCRPDF_Error errorid = TOCRPDF_DLL_not_loaded;

	VBBOOL vbPageIsNotBlank;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_PageToDibMem")) {
		errorid = (TOCRPDF_Error)PDFExtractorHandle_PageToDibMem(sds, PageBuf.sds, &cpwl, ColourMode, dpiX, dpiY, &vbPageIsNotBlank);

		if(TOCRPDF_ErrorOK == errorid) {
			if(VB_FALSE == vbPageIsNotBlank) {
				pageIsNotBlank = false;
			} else {
				pageIsNotBlank = true;
			}
		}
	}

	return errorid;
}

TOCRPDF_Error DL_PDFExtractor::PageToDib(IN DL_PDFExtractorPage& PageBuf, const wchar_t* pageFileNameDib,
	IN const TOCRPDF_COLOUR_MODE ColourMode, IN const double dpiX, IN const double dpiY, OUT bool& pageIsNotBlank) {
	TOCRPDF_Error errorid = TOCRPDF_DLL_not_loaded;

	VBBOOL vbPageIsNotBlank;

	if(LoadIfNeeded(DLLNAME, "PDFExtractorHandle_PageToDibFile")) {
		errorid = (TOCRPDF_Error)PDFExtractorHandle_PageToDibFile(sds, PageBuf.sds, pageFileNameDib, ColourMode, dpiX, dpiY, &vbPageIsNotBlank);

		if(TOCRPDF_ErrorOK == errorid) {
			if(VB_FALSE == vbPageIsNotBlank) {
				pageIsNotBlank = false;
			} else {
				pageIsNotBlank = true;
			}
		}
	}

	return errorid;
}


/*******************************************************************
** PDFArchiverMemDoc
*******************************************************************/

DL_PDFArchiverMemDoc::DL_PDFArchiverMemDoc() {
	// run constructor on data store
	DL_PDFArchiverMemDocHandle_New(&sds);
}

DL_PDFArchiverMemDoc::~DL_PDFArchiverMemDoc() {
	// run destructor on data store
	DL_PDFArchiverMemDocHandle_Delete(sds);
}

TOCRPDF_Error DL_PDFArchiverMemDoc::Load(OUT PDFArchiverHandle hArchiver, IN const wchar_t* inFileName) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_Load(hArchiver, sds, inFileName);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::Close(OUT PDFArchiverHandle hArchiver, IN const wchar_t* outFileName) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_Close(hArchiver, sds, outFileName);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveMonoImage(OUT PDFArchiverHandle hArchiver, IN const CHARPTR_WITH_ROW_COLUMN& mono_image,
	IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveMonoImage(hArchiver, sds, &mono_image, dpiX, dpiY, ImagePageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveAppendix(OUT PDFArchiverHandle hArchiver, IN const wchar_t* appendix,
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveAppendix(hArchiver, sds, appendix, ResultsPageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveResults(OUT PDFArchiverHandle hArchiver, IN const TOCRRESULTSEX_EG& appendix,
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveResults(hArchiver, sds, &appendix, ResultsPageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveTiff(OUT PDFArchiverHandle hArchiver, IN TIFF& tfile, const UINT16 TiffImageNumber, 
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveTiff(hArchiver, sds, &tfile, TiffImageNumber, ResultsPageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveAllTiff(OUT PDFArchiverHandle hArchiver, IN TIFF& tfile, 
										   IN const long ResultsPageNumber) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveAllTiff(hArchiver, sds, &tfile, ResultsPageNumber);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveAllPdf(OUT PDFArchiverHandle hArchiver, IN DL_PDFArchiverMemDoc& hDocIn, 
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveAllPdf(hArchiver, sds, hDocIn.sds, ResultsPageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveRGB24(OUT PDFArchiverHandle hArchiver, IN const RGBPTR_WITH_ROW_COLUMN& rgb_image, 
	IN const float dpiX, IN const float dpiY, IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveRGB24(hArchiver, sds, &rgb_image, dpiX, dpiY, ResultsPageNumber, &ResultsInfo);

	return errorid;
}

TOCRPDF_Error DL_PDFArchiverMemDoc::SaveRGBPalette(OUT PDFArchiverHandle hArchiver, IN const CHARPTR_WITH_ROW_COLUMN& palette_image, 
									IN const long BitsPerPixel, IN const long Width, 
									IN const RGBPTR_WITH_LEN& RGB_palette, IN const float dpiX,
									IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo) {
	TOCRPDF_Error errorid = (TOCRPDF_Error)DL_PDFArchiverHandle_SaveRGBPalette(hArchiver, sds, &palette_image, 
		BitsPerPixel, Width, &RGB_palette, dpiX, dpiY, ImagePageNumber, &ResultsInfo);

	return errorid;
}


/*******************************************************************
** PDFArchiver
*******************************************************************/
DL_PDFArchiver::DL_PDFArchiver() {
	// run constructor on data store
	DL_PDFArchiverHandle_New(&sds);
}

DL_PDFArchiver::~DL_PDFArchiver() {
	// run destructor on data store
	DL_PDFArchiverHandle_Delete(sds);
}

TOCRPDF_Error DL_PDFArchiver::Load(OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* inFileName) {
	return doc.Load(sds, inFileName);
}

TOCRPDF_Error DL_PDFArchiver::Close(OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* inFileName) {
	return doc.Close(sds, inFileName);
}

TOCRPDF_Error DL_PDFArchiver::SaveMonoImage(OUT DL_PDFArchiverMemDoc& doc, IN const CHARPTR_WITH_ROW_COLUMN& mono_image,
	IN const float dpiX, IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveMonoImage(sds, mono_image, dpiX, dpiY, ImagePageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveAppendix(OUT DL_PDFArchiverMemDoc& doc, IN const wchar_t* appendix,
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveAppendix(sds, appendix, ResultsPageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveResults(OUT DL_PDFArchiverMemDoc& doc, IN const TOCRRESULTSEX_EG& appendix,
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveResults(sds, appendix, ResultsPageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveTiff(OUT DL_PDFArchiverMemDoc& doc, IN TIFF& tfile, const UINT16 TiffImageNumber,
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveTiff(sds, tfile, TiffImageNumber, ResultsPageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveAllTiff(OUT DL_PDFArchiverMemDoc& doc, IN TIFF& tfile, 
										   IN const long ResultsPageNumber) {
	return doc.SaveAllTiff(sds, tfile, ResultsPageNumber);
}

TOCRPDF_Error DL_PDFArchiver::SaveAllPdf(OUT DL_PDFArchiverMemDoc& doc, IN DL_PDFArchiverMemDoc& docIn, 
	IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveAllPdf(sds, docIn, ResultsPageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveRGB24(OUT DL_PDFArchiverMemDoc& doc, IN const RGBPTR_WITH_ROW_COLUMN& rgb_image, 
	IN const float dpiX, IN const float dpiY, IN const long ResultsPageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveRGB24(sds, rgb_image, dpiX, dpiY, ResultsPageNumber, ResultsInfo);
}

TOCRPDF_Error DL_PDFArchiver::SaveRGBPalette(OUT DL_PDFArchiverMemDoc& doc, IN const CHARPTR_WITH_ROW_COLUMN& palette_image, 
									IN const long BitsPerPixel, IN const long Width, 
									IN const RGBPTR_WITH_LEN& RGB_palette, IN const float dpiX, 
									IN const float dpiY, IN const long ImagePageNumber, IN const TocrResultsInfo& ResultsInfo) {
	return doc.SaveRGBPalette(sds, palette_image, BitsPerPixel, Width, RGB_palette, dpiX, dpiY, ImagePageNumber, ResultsInfo);
}

const char* DL_PDFArchiver::GetLastExceptionText(const TOCRPDF_Error& err) {
	CHARPTR_WITH_LEN msg_box;
	msg_box.charPtr = errmsg;
	msg_box.len = errlen;
	TOCRPDF_Error errorid = TOCRPDF_DLL_not_loaded;

	if(LoadIfNeeded(DLLNAME, "PDFArchiverHandle_GetLastExceptionText")) {
		errorid = (TOCRPDF_Error)PDFArchiverHandle_GetLastExceptionText(sds, err, &msg_box);

		if(TOCRPDF_ErrorOK != errorid) {
			return "Could not find a description for error.";
		}
		else {
			return msg_box.charPtr;
		}
	}
	else {
		return "DLL did not load.";
	}
}
