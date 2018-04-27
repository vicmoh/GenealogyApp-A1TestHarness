/**
 * @file GEDCOMutilities.h
 * @author Vicky Mohammad
 * @date 2018
 * @brief utilities
 */

#ifndef _GEDCOMUTILITIES_H
#define _GEDCOMUTILITIES_H

/*import libraries*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
//macros switches for debuging
#define BOOL_TYPE false
#define DEBUG_TYPE false
#define CHECKER_TYPE false
//define macros for debuging
#define debug if(DEBUG_TYPE)printf
#define DEBUG BOOL_TYPE
#define DEBUG_HELPER BOOL_TYPE
#define DEBUG_PARSER BOOL_TYPE
#define DEBUG_UTILITIES BOOL_TYPE
#define CHECK if(CHECKER_TYPE)printf("CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK\n")
//create helper macros for easy coding
#define GET_ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( *array )) //or array[0] instead of *array
#define UPPERCASE_RANGE( var ) (var >= 65 && var <= 90) 
#define LOWERCASE_RANGE( var ) (var >= 97 && var <= 122)
#define ALPHABET_RANGE( var ) ( (var >= 97 && var <= 122) || (var >= 65 && var <= 90) )
#define NUMBER_RANGE( var ) (var >= 48 && var <= 57)
#define VISIBLE_RANGE( var ) (var >= 33 && var <= 126)
//macros for the size
#define MONTH_SIZE 12
#define RECORD_SIZE 3
#define ALL_RECORD_SIZE 5
#define ENCODING_SIZE 4
#define INDI_EVENT_SIZE 23
#define FAM_EVENT_SIZE 12

//crate a struct reference individual
typedef struct{
    char* reference;
    Individual* individual;
    bool found;
    bool foundFAMS;
    bool foundFAMC;
    List famsRef;
    List famcRef;
}IndividualReference;

//create a struct reference families
typedef struct{
    char* reference;
    Family* family;
    bool found;
    char* husbRef;
    char* wifeRef;
    List childRef;
}FamilyReference;

//create struct for the used as a helper vars
typedef struct {
    //dec main vars
    GEDCOMobject* object;
    List indiRef;
    List famRef;
    //List garbageFamily;
    char fileName[99];
    //vars for the file
    int lineLocation;
    int lineSize;
    char** lineString;
    //dec found vars for the INV_GEDCOM
    int foundHead;
    int foundTrailer;
    int foundSubmitter;
    int foundRecord;//dif types recordds for INV_GEDCOM
    //create vars to for the headers data
    int foundSourceTag; 
    int foundGedcVersionTag;
    int foundGedcTag;
    int foundEncodingTag;
    int foundSubmitterTag;
    int foundEndHeader;
    int endHeaderLineLocation;
    //for individual
    int foundStartIndividual;
    int foundIndividual;
    int foundStartNameTag;
    int foundNameTag;
    int foundGivenNameTag;
    int foundSurnameTag;
    //for faimly
    int foundStartFamily;
    int foundFamily;
    int foundHusband;
    int foundWife;
    int foundChild;
    //other
    int foundGarbageFamily;
    int counter;
} HelperVars;

//helper funtions to make coding easier
char* setLowerCase(char * string);
char* setUpperCase(char * string);
char* setString(char* string);
char** split(char* stringToBeSplit, const char* delimiter, int* arraySize);
char** readFileByLine(char* fileName, int* arraySize, const int lineSize);
char** readFileByChar(char* fileName, int* arraySize);//there is a bug with wierd char
void freeStringArray(char** array, int arraySize);
bool checkEmail(char* string);
int compareString(char* string1, char* string2);
int searchStringArray(char* stringArray[], char* stringToBeSearched, int arraySize, char* type);
char* combinedToken(char** token, int tokenSize, int tokenStartIndex);
void removeSpace(char* string);
void removeLineEnding(char* string);
//constructor for inititilizing the abstract types
Event* initEvent();
Field* initField();
Submitter* initSubmitter();
Header* initHeader();
Individual* initIndividual();
Family* initFamily();
GEDCOMobject* initObject();
GEDCOMerror initError(ErrorCode errorCode, int line);
HelperVars initVars(char* fileName, char** stringArrayOfFile, int lineSize);
IndividualReference* initIndiRef(char* ref, Individual* indi);
FamilyReference* initFamRef(char* ref, Family* fam);
//main checker for the object
GEDCOMerror checkFile(char* fileName);
GEDCOMerror checkGedcom(HelperVars* vars);
GEDCOMerror checkHeader(HelperVars* vars);
GEDCOMerror checkRecord(HelperVars* vars);
//create a getter function
GEDCOMerror getSubmitter(char* reference, HelperVars* vars);
GEDCOMerror getIndividual(char* reference, HelperVars* vars);
GEDCOMerror getFamily(char* reference, HelperVars* vars);
GEDCOMerror getEvent(Event* tempEvent, void* object, int lineStartNumber, char* type, HelperVars* vars);
//create a setter function
void setErrorCode(GEDCOMerror* object, ErrorCode errorCode, int line);
//sub checker helper functions
void printToken(char** token, int tokenSize);
bool checkStringIfNumbers(char* string);
bool checkStringIfCaps(char* string);
bool checkIfReference(char* string);
bool checkIfValidLine(char* stringLine);
bool checkIfStartRecord(char* stringLine);
bool checkIfLastName(char* string);
GEDCOMerror checkIfLineNumbers(HelperVars* vars);
//helper functions for get descendent
List combinedIndividualList(List* list1, List* list2);
List getChildren(Individual* person);
//dummy linklist helper functions
void dummyDelete(void* toBePrinted);
int dummyCompare(const void* first,const void* second);
char* dummyPrint(void* toBePrinted);
void freeObject(void* toBeDeleted);
//other linklist helper functions
bool comparePerson(const void* first, const void* second);
char* printSubmitter(void* toBePrinted);
char* printIndividualName(void* toBePrinted);
void deleteIndiRef(void* toBeDeleted);
void deleteFamRef(void* toBeDeleted);
void deleteGarbageFamily(void* toBeDeleted);
char* printReference(void* toBePrinted);
int compareIndiRef(const void* first,const void* second);
int compareFamRef(const void* first,const void* second);
bool findFamRef(const void* first, const void* second);
bool findIndiRef(const void* first, const void* second);
void deleteSubmitter(Submitter* toBeDeleted);
void deleteHeader(Header* toBeDeleted);
char* printDescendants(void* toBePrinted);
char* printEncoding(CharSet encoding);
void deleteFamilyGarbageWrapper();
ErrorCode validateEvents(List* event);

/***********************************************************
 * functions for the writing gedcom
 ***********************************************************/

void writeFile(const char* fileName, char* string);
char* getFileString(List* famList, List* indiList, const GEDCOMobject* object);
char* getFileHeader(void* toBePrinted);
char* getFileSubmitter(void* toBePrinted);
char* getFileIndividual(List* indiList);
char* getFileFamily(List* famList);
char* getFileEvent(List* eventList);
void setReferences(List* famList, List* indiList, const GEDCOMobject* object);
void linkReferences(List* famList, List* indiList);
bool findIndividualPointer(const void* first, const void* second);

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj);
ErrorCode validateGEDCOM(const GEDCOMobject* obj); 

#endif
