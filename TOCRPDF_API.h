// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TOCRPDF_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TOCRPDF_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TOCRPDF_EXPORTS
	//#define TOCRPDF_API __declspec(dllexport)  // now using .def file
	#define TOCRPDF_API
#elif defined TOCRPDF_STATIC
	#define TOCRPDF_API
#elif defined TOCRPDF_CUSTOM
	#define TOCRPDF_API TOCRPDF_CUSTOM
#else
	#define TOCRPDF_API __declspec(dllimport)
#endif

/*
// Example of a class exported from the TOCRPDF.dll
class TOCRPDF_API CTOCRPDF {
public:
	CTOCRPDF(void);
	// TODO: add your methods here.
};
*/

// example of a variable exported from the TOCRPDF.dll
//extern TOCRPDF_API int nTOCRPDF;

// example of a variable exported from the TOCRPDF.dll
//TOCRPDF_API int fnTOCRPDF(void);
