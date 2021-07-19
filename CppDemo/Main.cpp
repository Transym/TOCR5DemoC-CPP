/*
 * Transym OCR Demonstration program
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * This program demonstrates calling TOCR version 5.0 from C++.
 * This program demonstrates the simple processing of a single file.
 * No real attempt is made to handle errors.
 *
 * Copyright (C) 2012 Transym Computer Services Ltd.
 *
 * TOCR4.0DemoC++ Issue1
 */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <malloc.h>

#pragma warning(disable : 4996)
#include "TOCRlangsel.h"

#ifdef	RESELLER1
#include "TOCRRdll.h"
#include "TOCRRuser.h"
#include "TOCRRerrs.h"
#else
#include "TOCRdll.h"
#include "TOCRuser.h"
#include "TOCRerrs.h"
#endif

// for PDF
#include "DL_TOCRPDF.h"

void Example1();
void Example2();
void Example3();
void Example4();
void Example5();
void Example6();
void Example7();
void Example8();
void Example9();
void Example10();

bool OCRWait(long JobNo, TOCRJOBINFO2 JobInfo2);
bool OCRWait(long JobNo, TOCRJOBINFO_EG JobInfoEg);
bool OCRWait_PDF(long JobNo, TOCRJOBINFO_EG& JobInfoEg, char* filename, TOCRPROCESSPDFOPTIONS_EG& pPdfOpts);
bool OCRPoll(long JobNo, TOCRJOBINFO2 JobInfo2);
bool GetResults(long JobNo, TOCRRESULTS **Results);
bool GetResults(long JobNo, TOCRRESULTSEX **Results);
bool GetResults(long JobNo, TOCRRESULTSEX_EG **Results);
bool FormatResults(TOCRRESULTS *Results, char *Msg);
bool FormatResults(TOCRRESULTSEX *Results, char *Msg);
bool FormatResults(TOCRRESULTSEX_EG *Results, wchar_t *Msg);
void WaitForKey();
HANDLE ConvertGlobalMemoryToMMF(HANDLE hMem);


#define SAMPLE_TIF_FILE "Sample.tif"
#define SAMPLE_BMP_FILE "Sample.bmp"
#define SAMPLE_PDF_FILE "Sample.pdf"
#define PDF_RESULTS_FILE "Result.pdf"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MessageBox(NULL, "TOCR5.0C++Demo", "Main", MB_TASKMODAL | MB_TOPMOST);
	Example1();		// Demonstrates how to OCR a file (as v4)
	Example2();		// Demonstrates how to OCR multiple files
	Example3();		// Demonstrates how to OCR an image using a memory mapped file created by TOCR
	Example4();		// Demonstrates how to OCR an image using a memory mapped file created here, Also demonstrates the use of TOCRRESULTSEX_EG
	Example5();		// Retrieve information on Job Slot usage
	Example6();		// Retrieve information on Job Slots
	Example7();		// Get images from a TWAIN compatible device
	Example8();		// Demonstrates TOCRSetConfig and TOCRGetConfig
	Example9();		// Demonstrates how to OCR a file (as v5) and place the results in a PDF document
	Example10();	// Demonstrates how to OCR a file (as v5) and place selected pages into a PDF document

	return 0;
} // main

// Demonstrates how to OCR a file
void Example1()
{
	TOCRJOBINFO2		JobInfo2;
	TOCRRESULTS			*Results = 0;
	long				Status;
	long				JobNo = 0;
	char				InputFile[MAX_PATH];
	char				Msg[10240];

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);
	//TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_LOG); // Log Errors
	//	TOCRGetJobDBInfo(&JobNo);

	memset(&JobInfo2, 0, sizeof(TOCRJOBINFO2));

	//strcpy(InputFile, SAMPLE_TIF_FILE);
	//JobInfo2.JobType = TOCRJOBTYPE_TIFFFILE;

	// or
	strcpy(InputFile,SAMPLE_BMP_FILE);
	JobInfo2.JobType = TOCRJOBTYPE_DIBFILE;

	JobInfo2.InputFile = InputFile;
	MessageBox(NULL, "TOCR5.0C++Demo Example 1 Load file - V4 interface", JobInfo2.InputFile, MB_TASKMODAL | MB_TOPMOST);

	Status = TOCRInitialise(&JobNo);

#ifdef _DEBUG
	MessageBox(NULL, "TOCRInitialise done - attach debugger now", "Example 1", MB_TASKMODAL | MB_TOPMOST);
#endif // _DEBUG

	if ( Status == TOCR_OK ) {
		if ( OCRWait(JobNo, JobInfo2) ) {
		//if ( OCRPoll(JobNo, JobInfo2) ) {
			if ( GetResults(JobNo, &Results) ) {

				// Display the results

				if (FormatResults(Results, Msg)) {
					MessageBox(NULL, Msg, "Example 1", MB_TASKMODAL | MB_TOPMOST);
				}

				free(Results);
			}
		}

		TOCRShutdown(JobNo);
	}
} // Example1()

// Demonstrates how to OCR multiple files
void Example2()
{
	TOCRJOBINFO_EG		JobInfo_EG;
	TOCRRESULTS			*Results = 0;
	long				Status;
	long				JobNo;
	char				InputFile[MAX_PATH];
	char				Msg[10240];
	long				CountDone = 0;

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);

	memset(&JobInfo_EG, 0, sizeof(TOCRJOBINFO_EG));

	JobInfo_EG.InputFile = InputFile;

	Status = TOCRInitialise(&JobNo);
	if (Status == TOCR_OK) {

		// 1st file
		strcpy(InputFile, SAMPLE_TIF_FILE);
		JobInfo_EG.JobType = TOCRJOBTYPE_TIFFFILE;
		if (OCRWait(JobNo, JobInfo_EG)) {
			if (GetResults(JobNo, &Results)) {

				// Process the results
				CountDone++;
				free(Results);
			}
		}

		// 2nd file
		strcpy(InputFile, SAMPLE_BMP_FILE);
		JobInfo_EG.JobType = TOCRJOBTYPE_DIBFILE;
		if (OCRWait(JobNo, JobInfo_EG)) {
			if (GetResults(JobNo, &Results)) {

				// Process the results
				CountDone++;
				free(Results);
			}
		}

		// 3rd file
		strcpy(InputFile, SAMPLE_PDF_FILE);
		JobInfo_EG.JobType = TOCRJOBTYPE_PDFFILE;
		if (OCRWait(JobNo, JobInfo_EG)) {
			if (GetResults(JobNo, &Results)) {

				// Process the results
				CountDone++;
				free(Results);
			}
		}

		TOCRShutdown(JobNo);
	}

	sprintf(Msg, "%d of 3 jobs done", CountDone);
	MessageBox(NULL, Msg, "Example 2", MB_TASKMODAL | MB_TOPMOST);
} // Example2()

// Demonstrates how to OCR an image using a memory mapped file created by TOCR
void Example3()
{
	TOCRJOBINFO_EG		JobInfo_EG;
	TOCRRESULTSEX		*Results = 0;
	long				Status;
	long				JobNo;
	HANDLE				hMMF = 0;
	char				Msg[10240];


	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);

	memset(&JobInfo_EG, 0, sizeof(TOCRJOBINFO_EG));

	Status = TOCRInitialise(&JobNo);
	if ( Status == TOCR_OK ) {
		//TIF
		//Status = TOCRConvertFormat(JobNo, SAMPLE_TIF_FILE, TOCRCONVERTFORMAT_TIFFFILE, &hMMF, TOCRCONVERTFORMAT_MMFILEHANDLE, 0);
		//PDF
		Status = TOCRConvertFormat(JobNo, SAMPLE_PDF_FILE, TOCRCONVERTFORMAT_PDFFILE, &hMMF, TOCRCONVERTFORMAT_MMFILEHANDLE, 0);
		if ( Status == TOCR_OK ) {

			JobInfo_EG.JobType = TOCRJOBTYPE_MMFILEHANDLE;
			JobInfo_EG.hMMF = hMMF;
			if ( OCRWait(JobNo, JobInfo_EG) ) {
				if ( GetResults(JobNo, &Results) ) {
					FormatResults(Results, Msg);
					MessageBox(NULL, Msg, "Example 3", MB_TASKMODAL | MB_TOPMOST);
					free(Results);
				}
			}
			CloseHandle(hMMF);
		}
		TOCRShutdown(JobNo);
	}

} // Example3()

// Demonstrates how to OCR an image using a memory mapped file created here
// Also demonstrates the use of TOCRRESULTSEX_EG
void Example4()
{
	TOCRJOBINFO_EG		JobInfo_EG;
	TOCRRESULTSEX_EG	*Results = 0;
	long				Status;
	long				JobNo;
	int					file = -1;
	HANDLE				hMap = NULL;
	void				*View = NULL;
	HANDLE				hMMF = NULL;
	long				numbytes;
	BITMAPFILEHEADER	bf;
	wchar_t				Msg[10240];

	// Create a memory mapped file from a DIB file
	// The contents of a memory mapped file for TOCR are everything in the bitmap file except
	// the bitmap file header.

	file = _open(SAMPLE_BMP_FILE, _O_RDONLY | _O_SEQUENTIAL | _O_BINARY);
	if ( file != -1 ) {

		// read the bitmap file header to find the size of the bitmap
		if ( _read(file, &bf, sizeof(bf)) > 0 ) {
			numbytes = bf.bfSize - sizeof(bf);
			hMMF = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, numbytes, "any unique name");
			if ( hMMF != NULL ) {
				View = (void *)MapViewOfFile(hMMF, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if ( View != NULL ) {

					// read the rest of the bitmap into the memory mapped file
					if ( _read(file, View, numbytes) > 0 ) {
						UnmapViewOfFile(View);
						View = NULL;
						_close(file);
						file = -1;
						JobInfo_EG.hMMF = hMap;
						// Do not close the handle to the memory mapped file here - otherwise the
						// object will be destroyed before the OCR engine can read it
					} else {
						UnmapViewOfFile(View);
						CloseHandle(hMMF);
						_close(file);
						return;
					}
				} else {
					CloseHandle(hMMF);
					_close(file);
					return;
				}
			} else {
				_close(file);
				return;
			}
		} else {
			_close(file);
		   return;
		}
	} else {
	   return;
	}

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);

	memset(&JobInfo_EG, 0, sizeof(TOCRJOBINFO_EG));

	Status = TOCRInitialise(&JobNo);
	if ( Status == TOCR_OK ) {
		//Status = TOCRConvertFormat(JobNo, SAMPLE_TIF_FILE, TOCRCONVERTFORMAT_TIFFFILE, &hMMF, TOCRCONVERTFORMAT_MMFILEHANDLE, 0);
		//Status = TOCRConvertFormat(JobNo, SAMPLE_PDF_FILE, TOCRCONVERTFORMAT_PDFFILE, &hMMF, TOCRCONVERTFORMAT_MMFILEHANDLE, 0);
		if ( Status == TOCR_OK ) {

			JobInfo_EG.JobType = TOCRJOBTYPE_MMFILEHANDLE;
			JobInfo_EG.hMMF = hMMF;
			if ( OCRWait(JobNo, JobInfo_EG) ) {
				if ( GetResults(JobNo, &Results) ) {

					if(FormatResults(Results, Msg)) {
						MessageBoxW(NULL, Msg, L"Example 4", MB_TASKMODAL | MB_TOPMOST);
					} else {
						MessageBox(NULL, NULL, "Example 4 - no results found", MB_TASKMODAL | MB_TOPMOST);
					}

					free(Results);
				}
			}
		}
		TOCRShutdown(JobNo);
	}

	CloseHandle(hMMF);

} // Example4()

// Retrieve information on Job Slot usage
void Example5()
{
	long				Status;
	long				NumSlots;
	long				*JobSlotInf;
	char				Msg[10240];
	char				Line[80];

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);
	
	// uncomment to see effect on usage
	//long				JobNo;
	//Status = TOCRInitialise(&JobNo);

	NumSlots = TOCRGetJobDBInfo(0);
	if ( NumSlots > 0 ) {
		JobSlotInf= (long *)malloc(NumSlots * sizeof(long));
		if ( JobSlotInf ) {
			Status = TOCRGetJobDBInfo(JobSlotInf);
			if (Status == TOCR_OK) {
				strcpy(Msg, "Slot usage is\n");
				for (long SlotNo = 0; SlotNo < NumSlots; SlotNo++) {
					sprintf(Line, "\nSlot %d is ", SlotNo);
					strcat(Msg, Line);
					switch (JobSlotInf[SlotNo])
					{
						case TOCRJOBSLOT_FREE:
							strcat(Msg, "free");
							break;
						case TOCRJOBSLOT_OWNEDBYYOU:
							strcat(Msg, "owned by you");
							break;
						case TOCRJOBSLOT_BLOCKEDBYYOU:
							strcat(Msg, "blocked by you");
							break;
						case TOCRJOBSLOT_OWNEDBYOTHER:
							strcat(Msg, "owned by another process");
							break;
						case TOCRJOBSLOT_BLOCKEDBYOTHER:
							strcat(Msg, "blocked by another process");
							break;
					}
				}
				MessageBox(NULL, Msg, "Example 5", MB_TASKMODAL | MB_TOPMOST);
			}
			free(JobSlotInf);
		} else
			MessageBox(NULL, "Failed to malloc for JobSlotInf", "Example 5", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
	} else 
		MessageBox(NULL, "No job slots", "Example 5", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);

	//TOCRShutdown(JobNo);


} // Example5()

// Retrieve information on Job Slots
void Example6()
{
	long				Status;
	long				NumSlots;
	unsigned char		Licence[20];
	long				Volume = 0;
	long				Time = 0;
	long				Remaining;
	long				Features;
	long				JobNo;
	char				Msg[10240];
	char				Line[80];

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);

	// comment to see effect on usage
	Status = TOCRInitialise(&JobNo);

	NumSlots = TOCRGetJobDBInfo(0);
	if ( NumSlots > 0 ) {
		strcpy(Msg, "Slot usage is\n");
		for (long SlotNo = 0; SlotNo < NumSlots; SlotNo++) {
			sprintf(Line, "\nSlot %d is ", SlotNo);
			strcat(Msg, Line);
			Status = TOCRGetLicenceInfoEx(SlotNo, (char *)Licence, &Volume, &Time, &Remaining, &Features);
			if ( Status == TOCR_OK ) {
				sprintf(Line, " %s", Licence);
				strcat(Msg, Line);
				switch (Features)
				{
					case TOCRLICENCE_STANDARD:
						strcat(Msg, " STANDARD licence");
						break;
					case TOCRLICENCE_EURO:
						if ( strcmp((char *)Licence, "154C-43BA-9421-C925") == 0 )
							strcat(Msg, " EURO TRIAL licence");
						else 
							strcat(Msg, " EURO licence");
						break;
					case TOCRLICENCE_EUROUPGRADE:
						strcat(Msg, " EURO UPGRADE licence");
						break;
					case TOCRLICENCE_V3SE:
						if ( strcmp((char *)Licence, "2E72-2B35-643A-0851") == 0 )
							strcat(Msg, " V3 SE TRIAL licence");
						else 
							strcat(Msg, " V3 licence");
						break;
					case TOCRLICENCE_V3SEUPGRADE:
						strcat(Msg, " V1/2 UPGRADE to V3 SE licence");
						break;
					case TOCRLICENCE_V3PRO:
						strcat(Msg, " V3 Pro/V4 licence");
						break;
					case TOCRLICENCE_V3PROUPGRADE:
						strcat(Msg, " V1/2 UPGRADE to V3 Pro/V4 licence");
						break;
					case TOCRLICENCE_V3SEPROUPGRADE:
						strcat(Msg, " V3 SE UPGRADE to V3 Pro/V4 licence");
						break;
				}
				if ( Volume != 0 || Time != 0 ) {
					sprintf(Line, " %d", Remaining);
					strcat(Msg, Line);
					if ( Time != 0 )
						strcat(Msg, " days");
					else
						strcat(Msg, " A4 pages");
					strcat(Msg, " remaining on licence");
				}
			} else
				strcat(Msg, "Failed to get information");	
		}
		MessageBox(NULL, Msg, "Example 6", MB_TASKMODAL | MB_TOPMOST);
	} else 
		MessageBox(NULL, "No job slots", "Example 6", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);

	TOCRShutdown(JobNo);

} // Example6()

// Get images from a TWAIN compatible device
void Example7()
{
	TOCRJOBINFO_EG		JobInfo_EG;
	TOCRRESULTS			*Results = 0;
	long				JobNo;
	long				Status;
	long				NumImages = 0;
	long				CntImages = 0;
	HANDLE				*hMems;
	HANDLE				hMMF = 0;
	char				Msg[10240];

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);

	memset(&JobInfo_EG, 0, sizeof(TOCRJOBINFO_EG));

	Status = TOCRInitialise(&JobNo);

	Status = TOCRTWAINSelectDS(); // select the TWAIN device
	if ( Status == TOCR_OK) {
		Status = TOCRTWAINShowUI(1);
		Status = TOCRTWAINAcquire(&NumImages);
		if ( NumImages > 0 ) {
			hMems = (HANDLE *)malloc(NumImages * sizeof (HANDLE));
			if ( hMems ) {
				memset(hMems, 0, NumImages * sizeof(HANDLE));
				Status = TOCRTWAINGetImages(hMems);
				for (long ImgNo = 0; ImgNo < NumImages; ImgNo++) {
					if ( hMems[ImgNo] ) {
						// convert the memory block to a Memory Mapped File
						hMMF = ConvertGlobalMemoryToMMF(hMems[ImgNo]);
						// free the global memory block to save space
						GlobalFree(hMems[ImgNo]);
						if ( hMMF ) {
						// uncomment to see OCRed results
							JobInfo_EG.JobType = TOCRJOBTYPE_MMFILEHANDLE;
							JobInfo_EG.hMMF = hMMF;
							if ( OCRWait(JobNo, JobInfo_EG) ) {
								if ( GetResults(JobNo, &Results) ) {
									if ( FormatResults(Results, Msg) ) {
										FormatResults(Results, Msg);
										MessageBox(NULL, Msg, "Example 7", MB_TASKMODAL | MB_TOPMOST);
										free(Results);
									}
								}
							}
							CntImages ++;
							CloseHandle(hMMF);
						}
					}
				}
				free(hMems);
			}
		}
	}
	TOCRShutdown(JobNo);

	sprintf(Msg, "%d images successfully acquired\n", CntImages);
	MessageBox(NULL, Msg, "Example 7", MB_TASKMODAL | MB_TOPMOST);

} // Example7()


// Demonstrates TOCRSetConfig and TOCRGetConfig
void Example8()
{
	long				Status;
	long				JobNo;
	long				Value;
	char				Msg[10240];
	char				Line[250];

	// Override the INI file settings for all new jobs
	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);
	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_SRV_ERRORMODE, TOCRERRORMODE_MSGBOX);

	TOCRGetConfigStr(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_LOGFILE, Line);
	sprintf(Msg, "Default Log file name %s", Line);
	MessageBox(NULL, Msg, "Example 8", MB_TASKMODAL | MB_TOPMOST);

	TOCRSetConfigStr(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_LOGFILE, "Loggederrs.lis");
	TOCRGetConfigStr(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_LOGFILE, Line);
	sprintf(Msg, "New default Log file name %s", Line);
	MessageBox(NULL, Msg, "Example 8", MB_TASKMODAL | MB_TOPMOST);

	Status = TOCRInitialise(&JobNo);
	TOCRSetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_NONE);

	TOCRGetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, &Value);
	sprintf(Msg, "Job %d DLL error mode %d", JobNo, Value);
	MessageBox(NULL, Msg, "Example 8", MB_TASKMODAL | MB_TOPMOST);

	TOCRGetConfig(JobNo, TOCRCONFIG_SRV_ERRORMODE, &Value);
	sprintf(Msg, "Job %d Service error mode %d", JobNo, Value);
	MessageBox(NULL, Msg, "Example 8", MB_TASKMODAL | MB_TOPMOST);

	// Cause an error - then check Loggederrs.lis
	TOCRSetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_LOG);
	TOCRSetConfig(JobNo, 1000, TOCRERRORMODE_LOG);

	TOCRShutdown(JobNo);

} // Example8()

// Demonstrates how to OCR a file and place the results in a PDF document
void Example9()
{
	TOCRPROCESSPDFOPTIONS_EG PDFOpts;
	TOCRJOBINFO_EG		JobInfoEg;
	TOCRRESULTSEX_EG	*Results = 0;
	long				Status;
	long				JobNo;
	char				InputFile[MAX_PATH];
	wchar_t*			pMsg = NULL;

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);
	// set the PDF options
	memset(&PDFOpts, 0, sizeof(TOCRPROCESSPDFOPTIONS_EG));

	PDFOpts.ResultsOn = true;
	PDFOpts.OriginalImageOn = true;
	PDFOpts.ProcessedImageOn = true;

	// set the job options as Example1()
	memset(&JobInfoEg, 0, sizeof(TOCRJOBINFO_EG));
	//Using a TIF file
	strcpy(InputFile, SAMPLE_TIF_FILE);
	JobInfoEg.JobType = TOCRJOBTYPE_TIFFFILE;

	// or using a BMP file
	//strcpy(InputFile,SAMPLE_BMP_FILE);
	//JobInfoEg.JobType = TOCRJOBTYPE_DIBFILE;

	// or using a PDF
	//strcpy(InputFile,SAMPLE_PDF_FILE);
	//JobInfoEg.JobType = TOCRJOBTYPE_PDFFILE;

	JobInfoEg.InputFile = InputFile;

	JobInfoEg.ProcessOptions.CGAlgorithm = TOCRJOBCG_HISTOGRAM;

	Status = TOCRInitialise(&JobNo);

	if ( Status == TOCR_OK ) {
		// Use the PDF version of the OCR process
		if ( OCRWait_PDF(JobNo, JobInfoEg, PDF_RESULTS_FILE, PDFOpts) ) {
			if ( GetResults(JobNo, &Results) ) {
				if (Results) {
					pMsg = (wchar_t *)malloc((1 + Results->Hdr.NumItems) * sizeof(wchar_t));
					if (pMsg) {
						// Display the results
						FormatResults(Results, pMsg);
						MessageBoxW(NULL, pMsg, L"Example 9 - V5 PDF test", MB_TASKMODAL | MB_TOPMOST);
						free(pMsg);
						pMsg = NULL;
					}
					free(Results);
				}
			}
		}

		TOCRShutdown(JobNo);
	}
} // Example9()

// Example10()
// 1) Allocate enough space for a wide version of the input and output filename
// 2) create a block of memory for the PDFExtractor to write to.
// 3) Allocate a block of memory to store the metadata
// 4) create a memory based pdf document for the extractor to work with called hDocIn.
// 5) create a memory based pdf document for archiver to write to called docOut.
// 6) take the narrow file name and expand it to wide
// 7) Use the pdf extractor to load the file into the ExtractorMemDoc called hDocIn
// 8) Load the original document into the archiver memory
// 9) clear the block of memory for the job
// 10) Connect to TOCRService
// 11) remove the text from each page and save an image of the rest of the page as a DIB
// 12) Extract the number of pages from the PDF
// 13) Create a memory mapped file i.e. reserve a large enough memory file space.
// 14) Add the file name to metadata
// 15) Record in the metadata that the following pages will be the original document.
// 16) Extract the entire input pdf and save it to docOut
// 17) Record in the metadata that the following pages will be the text returned by the OCR
// 18) Loop through all pages in the input file.
	// 18a) Initialise the Page store to only contain the page to be analysed
	// 18b) find the page size in inches
	// 18c) populate dpi values if known
	// 18d) Allocate memory for the dib file mmf process (i.e. reserve it from our memory mapped file created earlier)
	// 18e) Extract the mmf view (i.e. the memory allocated for the dib file mmf process) and interpret it as the extractors' memory block.
	// 18f) Create a DIB file from the page stored in memory, using the reccomended values storing the result in an mmf view
	// 18g) clear and return the memory used in the MMF process by the DIB
	// 18h) if the extractor has found something of OCR relevence on the page.
		// 18i) record for TOCR that we will be sending an MMF job
		// 18j) record for TOCR the address of the memory mapped file
		// 18k) adjust the Title string so that it contains the page number.
		// 18l) record in the metadata the original page number 
		// 18m) save the OCR results and metadata to a new PDF page at the end of the document in the archiver's memory
	// 18n) We're finished with the results so return the memory used.(Allocated by TOCR)
// 19) move on a page
// 20) If all has gone well, save the memory pdf document out to a file
void Example10()
{
	TOCRJOBINFO2		JobInfo2;
	TOCRRESULTSEX_EG	*Results = 0;
	long				JobNo;
	long				TocrStatus = TOCR_OK;
	long				PdfStatus = TOCRPDF_ErrorOK;
	long				NumImages = 0;
	long				CntImages = 0;
	bool pageIsNotBlank = false;
	TOCRPDF_COLOUR_MODE ColourMode = COLOUR_MODE_GREY8;  // do all processing as RGB (false => Grey8)
	wchar_t Msg[_MAX_PATH + 1000]; // for MsgBox use 
	wchar_t* pResultChars = NULL;

	HANDLE file = INVALID_HANDLE_VALUE;
	// a4 page at 300 dpi; x = 2479, y = 3508; x * y = 8697159 pixels
	// for RGB - times pixels by 3 bytes = 26091477 (26MB)
	const DWORD MaxSizeLow = 30000000;  // a4 RGB page at 300 dpi, no compression
	HANDLE				hMMF = 0;
	// 1) Allocate enough space for a wide version of the input and output filename
	wchar_t WideInFileName[_MAX_PATH + 1];
	wchar_t WideOutFileName[_MAX_PATH + 1];

	char ErrorStore[2000];
	// 2) create a block of memory for the PDFExtractor to write to.
	CHARPTR_WITH_LEN cpwl;
	void			*View = 0;
	// 3) Allocate a block of memory to store the metadata
	TocrResultsInfo ResultsInfo;
	double width = 0;
	double height = 0;
	double dpiX = 0;
	double dpiY = 0;

	PDFExtractorHandle hExtractor;
	PDFExtractorPageHandle hPage;
	PDFExtractorMemDocHandle hDocIn;
	PDFArchiverHandle		hArchiver;
	PDFArchiverMemDocHandle hDocAIn;
	PDFArchiverMemDocHandle hDocOut;

	DL_PDFExtractorHandle_New(&hExtractor);
	DL_PDFExtractorPageHandle_New(&hPage);
	
	// 4) create a memory based pdf document for the extractor to work with called hDocIn.	
	DL_PDFExtractorMemDocHandle_New(&hDocIn);
	
	DL_PDFArchiverHandle_New(&hArchiver);
	DL_PDFArchiverMemDocHandle_New(&hDocAIn);
	
	// 5) create a memory based pdf document for archiver to write to called docOut.
	DL_PDFArchiverMemDocHandle_New(&hDocOut);

	// 6) take the narrow file name and expand it to wide
	mbstowcs(WideInFileName, SAMPLE_PDF_FILE, _MAX_PATH);
	mbstowcs(WideOutFileName, PDF_RESULTS_FILE, _MAX_PATH);
	// 7) Use the pdf extractor to load the file (name held in WideInFileName)  into the ExtractorMemDoc hDocIn
	PdfStatus = DL_PDFExtractor_Load(hExtractor, hDocIn, WideInFileName);
	//Error Handling
	if (PdfStatus != TOCRPDF_ErrorOK) {
		TocrStatus = TOCRERR_FAILOPENFILE1;
		goto PdfErr;
	}
	// 8) Load the original document into the archiver memory
	PdfStatus = DL_PDFArchiverHandle_Load(hArchiver, hDocAIn, WideInFileName);
	//Error Handling
	if (PdfStatus != TOCRPDF_ErrorOK) {
		TocrStatus = TOCRERR_FAILOPENFILE2;
		goto PdfErr;
	}

	TOCRSetConfig(TOCRCONFIG_DEFAULTJOB, TOCRCONFIG_DLL_ERRORMODE, TOCRERRORMODE_MSGBOX);
	// 9) clear the block of memory for the job
	memset(&JobInfo2, 0, sizeof(TOCRJOBINFO2));
	// 10) Connect to TOCRService
	TocrStatus = TOCRInitialise(&JobNo);
	//Error Handling
	if (TocrStatus != TOCR_OK) {
		goto TocrErr;
	} // if ( TocrStatus != TOCR_OK)

	// 11) remove the text from each page and save an image of the rest of the page as a DIB
	// 12) Extract the number of pages from the PDF
	PdfStatus = DL_PDFExtractor_GetPageCount(hExtractor, hDocIn, &NumImages);
	//Error Handling
	if (PdfStatus != TOCRPDF_ErrorOK) {
		TocrStatus = TOCRERR_FAILREADFILE2;
		goto PdfErr;
	}
	// 13) Create a memory mapped file i.e. reserve a large enough memory file space.
	hMMF = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MaxSizeLow, 0);
	//Error Handling
	if (0 == hMMF) {
		TocrStatus = TOCRERR_FAILFILEMAP;
		goto PdfErr;
	}
	//Record that we intend to include original document in results
	ResultsInfo.bContainedInThisDocument = true;
	// 14) Add the file name to metadata
	wcscpy(ResultsInfo.OriginalFileName, WideInFileName);
	// 15) Record in the metadata that the following pages will be the original document.
	ResultsInfo.OutputStage = TRS_Original_Page;
	// 16) Extract the entire input pdf and save it to docOut, sets the Input file(docAIn) to be inserted at the end of the docOut
	PdfStatus = DL_PDFArchiverHandle_SaveAllPdf(hArchiver, hDocOut, hDocAIn, -2, &ResultsInfo);
	// 17) Record in the metadata that the following pages will be the text returned by the OCR
	ResultsInfo.OutputStage = TRS_Text;
	// 18) Loop through all pages in the input file.
	for (long ImgNo = 0; ImgNo < NumImages; ImgNo++) {
		// 18a) Initialise the Page store to only contain the page to be analysed
		PdfStatus = DL_PDFExtractor_GetPage(hExtractor, hDocIn, hPage, (ImgNo + 1));  // get page is 1 based but ImgNo is zero based
		//Error Handling
		if (PdfStatus != TOCRPDF_ErrorOK) {
			TocrStatus = TOCRERR_CANTFINDPAGE;
			goto PdfErr;
		}

		// 18b) find the page size in inches
		PdfStatus = DL_PDFExtractor_FindPageSize(hExtractor, hPage, &width, &height);
		//Error Handling
		if (PdfStatus != TOCRPDF_ErrorOK) {
			TocrStatus = TOCRERR_FAILREADFILE3;
			goto PdfErr;
		}
		width = (width < 0) ? (-1.0 * width) : width;
		height = (height < 0) ? (-1.0 * height) : height;
		// 18c) populate dpi values if known (0 or less if not known)
		PdfStatus = DL_PDFExtractor_GetRecommendedDPIForPageSize(hExtractor, width, height, ColourMode, &dpiX, &dpiY);
		//Error Handling
		if (PdfStatus != TOCRPDF_ErrorOK) {
			TocrStatus = TOCRERR_FAILREADFILE3;
			goto PdfErr;
		}
		// 18d) Allocate memory for the dib file mmf process (i.e. reserve it from our memory mapped file created earlier)
		View = (void *)MapViewOfFile(hMMF, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		//Error Handling
		if (NULL == View) {
			CloseHandle(hMMF);
			hMMF = NULL;
			TocrStatus = TOCRERR_FAILFILEVIEW;
			goto PdfErr;
		}
		// 18e) Extract the mmf view (i.e. the memory allocated for the dib file mmf process) and interpret it as the extractors' memory block.
		cpwl.charPtr = (char*)View;
		cpwl.len = MaxSizeLow;

		// 18f) Create a DIB file from the page stored in memory, using the recommended values storing the result in an mmf view (cpwl)
		PdfStatus = DL_PDFExtractor_PageToDibMem(hExtractor, hPage, &cpwl, ColourMode, dpiX, dpiY, &pageIsNotBlank);
		//A page may be processed as blank (even when there appears to be something there) 
		// because there is nothing on the page that is relevent to the OCR process.
		// 18g) clear and return the memory used in the MMF process by the DIB
		UnmapViewOfFile(View);
		cpwl.len = 0;
		cpwl.charPtr = NULL;
		//Error Handling
		if (PdfStatus != TOCRPDF_ErrorOK) {
			CloseHandle(hMMF);
			hMMF = NULL;
			TocrStatus = TOCRERR_FAILCREATEDIB;
			goto PdfErr;
		} // if ( PdfStatus != TOCRPDF_ErrorOK)
		// 18h) if the extractor has found something of OCR relevance on the page.
		if (pageIsNotBlank)
		{
			// 18i) record for TOCR that we will be sending an MMF job
			JobInfo2.JobType = TOCRJOBTYPE_MMFILEHANDLE;
			// 18j) record for TOCR the address of the memory mapped file
			JobInfo2.hMMF = hMMF;
			//JobInfo2.ProcessOptions.StructId = 1; //XPos and YPos refer to the positions on the original page 
			if (OCRWait(JobNo, JobInfo2)) {
				if (GetResults(JobNo, &Results)) {
					if (Results) {
						if (Results->Hdr.NumItems > 0)
						{
							pResultChars = (wchar_t *)malloc((1 + Results->Hdr.NumItems) * sizeof(wchar_t));
							if (pResultChars) {
								// Display the results
								FormatResults(Results, pResultChars);

								// 18k) adjust the Title string so that it contains the page number.
								//swprintf(pszTitle, L"APPENDIX - The text found by Transym OCR on page %d.", (ImgNo + 1));
								// 18l) record in the metadata the original page number 
								ResultsInfo.OriginalPageNumber = ImgNo;
								// 18m) save the OCR results and metadata to a new PDF page at the end of the document in the archiver's memory
								PdfStatus = DL_PDFArchiverHandle_SaveAppendix(hArchiver, hDocOut, pResultChars, -2, &ResultsInfo);
								//NP
								//PdfStatus = DL_PDFArchiverHandle_SaveResults(hArchiver, hDocOut, Results, -2, &ResultsInfo);
								//PdfStatus = DL_PDFArchiverHandle_SaveResults(hArchiver, hDocOut, Results, -2, pszTitle, width,
								//	height, dpiX, dpiY ResultsInfo);
								//PdfStatus = archiver.SaveResults(docOut, *Results, -2, ResultsInfo);
								//Error Handling
								if (PdfStatus != TOCRPDF_ErrorOK) {
									TocrStatus = TOCRERR_FAILOPENFILE3;
									goto PdfErr;
								}

								free(pResultChars);
								pResultChars = NULL;
							}
						} // if ( Results->Hdr.NumItems > 0 )
						// 18n) We're finished with the results so return the memory used.(Allocated by TOCR)
						free(Results);
						// 19) move on a page
						CntImages++;
					} // if(Results) 
				} // if ( GetResults(JobNo, &Results) )
			} // if ( OCRWait(JobNo, JobInfo2) )
		} // if(pageIsNotBlank)  // may have been set by the rendering engine
	} // for(ImgNo

	CloseHandle(hMMF);
	// 20) If all has gone well, save the memory pdf document out to a file
	if ((PdfStatus == TOCRPDF_ErrorOK) && (TocrStatus == TOCR_OK))
	{
		PdfStatus = DL_PDFArchiverHandle_Close(hArchiver, hDocOut, WideOutFileName);

		// notify the user
		swprintf(Msg, L"Success. Please see %s", WideOutFileName);
		MessageBoxW(NULL, Msg, L"Example10", MB_TASKMODAL | MB_TOPMOST | MB_ICONINFORMATION);
	}

PdfErr:
	if (PdfStatus != TOCRPDF_ErrorOK)
	{
		cpwl.charPtr = ErrorStore;
		cpwl.len = 2000;
		DL_PDFExtractor_GetLastExceptionText(hExtractor, PdfStatus, &cpwl);
		printf("\n%s", ErrorStore);
	}

	if (TocrStatus != TOCRERR_FAILOPENFILE1) {
		TOCRShutdown(JobNo);
	}

TocrErr:
	if (TocrStatus != TOCR_OK)
	{
		printf("\nTOCR Status %d\n", TocrStatus);
	}

} // Example10


// V4 Version
bool OCRWait(long JobNo, TOCRJOBINFO2 JobInfo2)
{
	long				Status;
	long				JobStatus;
	long				ErrorMode;
	char				Msg[TOCRJOBMSGLENGTH];

	Status = TOCRDoJob2(JobNo, &JobInfo2);
	if (Status == TOCR_OK) {
		Status = TOCRWaitForJob(JobNo, &JobStatus);
	}

	if (Status == TOCR_OK && JobStatus == TOCRJOBSTATUS_DONE)
	{
		return true;
	}
	else {
		// If something has gone wrong display a message
		// (Check that the OCR engine hasn't already displayed a message)
		TOCRGetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, &ErrorMode);
		if (ErrorMode == TOCRERRORMODE_NONE) {
			TOCRGetJobStatusMsg(JobNo, Msg);
			MessageBox(NULL, Msg, "OCRWait", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
		}
		return false;
	}
} // OCRWait()

// V5 version
bool OCRWait(long JobNo, TOCRJOBINFO_EG JobInfoEg)
{
	long				Status;
	long				JobStatus;
	long				ErrorMode;
	char				Msg[TOCRJOBMSGLENGTH];

	Status = TOCRDoJob_EG(JobNo, &JobInfoEg);
	if (Status == TOCR_OK) {
		Status = TOCRWaitForJob(JobNo, &JobStatus);
	}

	if (Status == TOCR_OK && JobStatus == TOCRJOBSTATUS_DONE)
	{
		return true;
	}
	else {
		// If something has gone wrong display a message
		// (Check that the OCR engine hasn't already displayed a message)
		TOCRGetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, &ErrorMode);
		if (ErrorMode == TOCRERRORMODE_NONE) {
			TOCRGetJobStatusMsg(JobNo, Msg);
			MessageBox(NULL, Msg, "OCRWait", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
		}
		return false;
	}
} // OCRWait()

bool OCRWait_PDF(long JobNo, TOCRJOBINFO_EG& JobInfoEg, char* filename, TOCRPROCESSPDFOPTIONS_EG& PdfOpts)
{
	long				Status;
	long				JobStatus;
	long				ErrorMode;
	char				Msg[TOCRJOBMSGLENGTH];

	Status = TOCRDoJobPDF_EG(JobNo, &JobInfoEg, filename, &PdfOpts);
	if (Status == TOCR_OK) {
		Status = TOCRWaitForJob(JobNo, &JobStatus);
	}
	
	if (Status == TOCR_OK && JobStatus == TOCRJOBSTATUS_DONE)
	{
		return true;
	} else {
		// If something has gone wrong display a message
		// (Check that the OCR engine hasn't already displayed a message)
		TOCRGetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, &ErrorMode);
		if ( ErrorMode == TOCRERRORMODE_NONE ) {
			TOCRGetJobStatusMsg(JobNo, Msg);
			MessageBox(NULL, Msg, "OCRWait", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
		}
		return false;
	}
} // OCRWait_PDF()

bool OCRPoll(long JobNo, TOCRJOBINFO2 JobInfo2)
{
	long				Status;
	long				Jobstatus;
	long				ErrorMode;
	char				Msg[TOCRJOBMSGLENGTH];

	Status = TOCRDoJob2(JobNo, &JobInfo2);
	if (Status == TOCR_OK) {
		do {
			Sleep(100);
			Status = TOCRGetJobStatus(JobNo, &Jobstatus);
			if ( Status != TOCR_OK ) {
				sprintf(Msg, "OCRPoll failed - %d\n", Status);
				MessageBox(NULL, Msg, "OCRPoll", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
				return false;
			}
		} while ( !Jobstatus );
	}
	
	if (Status == TOCR_OK && Jobstatus == TOCRJOBSTATUS_DONE)
	{
		return true;
	} else {
		// If something hass gone wrong display a message
		// (Check that the OCR engine hasn't already displayed a message)
		TOCRGetConfig(JobNo, TOCRCONFIG_DLL_ERRORMODE, &ErrorMode);
		if ( ErrorMode == TOCRERRORMODE_NONE ) {
			TOCRGetJobStatusMsg(JobNo, Msg);
			MessageBox(NULL, Msg, "OCRPoll", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
		}
		return false;
	}
} // OCRPoll()

// Get the results from TOCR
bool getresults(long JobNo, long mode, void **Results)
{
	long				Status;
	long				ResultsInf;
	char				Msg[TOCRJOBMSGLENGTH];

	if ( mode == TOCRGETRESULTS_NORMAL_EG || mode == TOCRGETRESULTS_EXTENDED_EG ) {
		Status = TOCRGetJobResultsEx_EG(JobNo, mode, &ResultsInf, 0);
		if ( Status != TOCR_OK ) {
			sprintf(Msg, "TOCRGetJobResultsEx_EG failed - %d\n", Status);
		}
	} else {
		Status = TOCRGetJobResultsEx(JobNo, mode, &ResultsInf, 0);
		if ( Status != TOCR_OK ) {
			sprintf(Msg, "TOCRGetJobResultsEx failed - %d\n", Status);
		}
	}
	if ( Status != TOCR_OK ) {
		MessageBox(NULL, Msg, "getresults", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
		return false;
	}
	if ( ResultsInf > 0 ) {
		// Allocate memory for results

		*Results = (void *)malloc(ResultsInf * sizeof(unsigned char));
		if ( *Results ) {

			// Retrieve the results
			if ( mode == TOCRGETRESULTS_NORMAL_EG || mode == TOCRGETRESULTS_EXTENDED_EG ) {
				Status = TOCRGetJobResultsEx_EG(JobNo, mode, &ResultsInf, *Results);
				if ( Status != TOCR_OK ) {
					sprintf(Msg, "TOCRGetJobResultsEx_EG failed - %d\n", Status);
				}
			} else {
				Status = TOCRGetJobResultsEx(JobNo, mode, &ResultsInf, *Results);
				if ( Status != TOCR_OK ) {
					sprintf(Msg, "TOCRGetJobResultsEx failed - %d\n", Status);
				}
			}
			if ( Status != TOCR_OK ) {
				MessageBox(NULL, Msg, "getresults", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
				free(*Results);
				*Results = 0;
				return false;
			}
		} else {
			MessageBox(NULL, "Failed to allocate memory for results\n", "getresults", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
			return false;
		}
	} else {
		MessageBox(NULL, "No results found\n", "getresults", MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP);
	}
	
	return true;
} // getresults()

// Get normal results
bool GetResults(long JobNo, TOCRRESULTS **Results)
{
	return getresults(JobNo, TOCRGETRESULTS_NORMAL, (void **)Results);
} // GetResults()

// Get extended results
bool GetResults(long JobNo, TOCRRESULTSEX **Results)
{
	return getresults(JobNo, TOCRGETRESULTS_EXTENDED, (void **)Results);
} // GetResults()

// Get extended eg results
bool GetResults(long JobNo, TOCRRESULTSEX_EG **Results)
{
	return getresults(JobNo, TOCRGETRESULTS_EXTENDED_EG, (void **)Results);
} // GetResults()


// Convert results to a string
bool FormatResults(TOCRRESULTS *Results, char *Msg)
{
	long			ItemNo;
	long			APos = 0;
	bool			Status = false;

	if ( Results->Hdr.NumItems > 0 ) {
		for (ItemNo = 0; ItemNo < Results->Hdr.NumItems; ItemNo ++ ) {
			if ( Results->Item[ItemNo].OCRCha == '\r' )
				Msg[APos] = '\n';
			else
				Msg[APos] = (char)Results->Item[ItemNo].OCRCha;
			APos ++;
		}
		Msg[APos] = 0;
		Status = true;
	}

	return Status;

} // FormatResults()

// Convert extended results to a string
bool FormatResults(TOCRRESULTSEX *Results, char *Msg)
{
	long			ItemNo;
	long			APos = 0;
	bool			Status = false;

	if ( Results->Hdr.NumItems > 0 ) {
		for (ItemNo = 0; ItemNo < Results->Hdr.NumItems; ItemNo ++ ) {
			if ( Results->Item[ItemNo].OCRCha == '\r' )
				Msg[APos] = '\n';
			else
				Msg[APos] = (char)Results->Item[ItemNo].OCRCha;
			APos ++;
		}
		Msg[APos] = 0;
		Status = true;
	}

	return Status;
} // FormatResults()

// Convert extended results to a string
bool FormatResults(TOCRRESULTSEX_EG *Results, wchar_t *Msg)
{
	long			ItemNo;
	long			APos = 0;
	bool			Status = false;

	if ( Results->Hdr.NumItems > 0 ) {
		for (ItemNo = 0; ItemNo < Results->Hdr.NumItems; ItemNo ++ ) {
			if ( Results->Item[ItemNo].OCRCharWUnicode == L'\r' )
				Msg[APos] = L'\n';
			else
				Msg[APos] = Results->Item[ItemNo].OCRCharWUnicode;
			APos ++;
		}
		Msg[APos] = 0;
		Status = true;
	}

	return Status;
} // FormatResults()


HANDLE ConvertGlobalMemoryToMMF(HANDLE hMem)
{
	void				*View = 0;
	HANDLE				hMMF = NULL;
	void				*ucImg = NULL;
	long				Status;

	Status = TOCR_OK + 1;

	hMMF = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)GlobalSize((void *)hMem), 0);
	if ( hMMF != NULL ) {
		View = (void *)MapViewOfFile(hMMF, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if ( View != NULL ) {
			ucImg = GlobalLock((void *)hMem);
			if ( ucImg ) {
				memcpy(View, ucImg, GlobalSize((void *)hMem) * sizeof(unsigned char));
				GlobalUnlock((void *)hMem);
				Status = TOCR_OK;
			}
			UnmapViewOfFile(View);
		}
	}

	if ( Status == TOCR_OK )
		return hMMF;
	else {
		if ( hMMF )
			CloseHandle(hMMF);
		return NULL;
	}

} // ConvertGlbalMemoryToMMF()
