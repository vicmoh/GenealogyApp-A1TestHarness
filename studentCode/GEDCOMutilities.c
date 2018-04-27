/**
 * @file GEDCOMutilities.c
 * @author Vicky Mohammad
 * @date 2018
 * @brief utilities functions
 */

/*import libraries*/
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

//a helper var for collecting pointer that needs to be freed
List LIST_OF_FAMILY_GARBAGE;

/***********************************************************
 * helper funtions to make c coding easier
 ***********************************************************/

char* setLowerCase(char* string){
    //loop through the char change to lower case
    for(int x=0; x <strlen(string)+1;x++){
        //to prevent conversion when its not letter
        if(UPPERCASE_RANGE(string[x]) || LOWERCASE_RANGE(string[x])){
            string[x] = tolower(string[x]);
        }//end if
    }//end if
    //return
    return string;
}//end func

char* setUpperCase(char* string){
    //loop through the char change to upper case
    for(int x=0; x <strlen(string)+1;x++){
        //to prevent conversion when its not letter
        if(UPPERCASE_RANGE(string[x]) || LOWERCASE_RANGE(string[x])){
            string[x] = toupper(string[x]);
        }//end if
    }//end if
    //return
    return string;
}//end func

char* setString(char* string){
    char* newString = calloc(1 ,sizeof(char)*(strlen(string)+9));
    strcpy(newString, string);
    return newString;
}//end func

char** split(char* stringToBeSplit, const char* delimiter, int* arraySize){
    //create string array
    int numOfSplits = 0;
    char** stringArray = calloc(1 ,sizeof(stringArray)*(numOfSplits+1));
    //tokenize through thw code
    char *token;
    char* stringBuffer = setString(stringToBeSplit);
    token = strtok(stringBuffer, delimiter);
    //iterate through the tokem
    while(token != NULL) {
        //copy the split token
        stringArray[numOfSplits] = setString(token);
        //keep going
        token = strtok(NULL, delimiter);
        //realloc more memory
        numOfSplits++;
        stringArray = realloc(stringArray, sizeof(stringArray)*(numOfSplits+1));
    }//end if 
    //return
    *arraySize = numOfSplits;
    free(stringBuffer);
    return stringArray;
}//end func

char** readFileByLine(char* fileName, int* arraySize, const int lineSize){
    //dec var
    FILE* filePointer = fopen(fileName, "r");
    char line[lineSize];
    char** stringArray = calloc(1 ,sizeof(stringArray));
    int numberOfLines = 0;

    //if file doesnt exist
    if(filePointer == NULL) {
        if(DEBUG_HELPER)printf("No such file.\n");
        free(stringArray);
        return NULL;
    }//end if

    if(DEBUG_HELPER)printf("Loading string...\n");
    //scan until end of file
    //while (fgets(line, sizeof(line), filePointer) != NULL) {
    while (fgets(line, sizeof(line), filePointer) != NULL) {
        //copy string
        if(DEBUG_HELPER)printf("line %d: %s", numberOfLines, line);
        removeLineEnding(line);
        line[strcspn(line, "\r\n")] = '\0';
        stringArray[numberOfLines] = setString(line); 
        //debug("String Length: %d\n", (int)strlen(line));
        numberOfLines++;
        //create string array from each line
        stringArray = realloc(stringArray, sizeof(stringArray)*(numberOfLines+1));
        //go to the next
    }//end loop
    if(DEBUG_HELPER)printf("\nString loaded...\n");
    
    //return
    *arraySize = numberOfLines; 
    fclose(filePointer);
    return stringArray;
}//end func

char** readFileByChar(char* fileName, int* arraySize){//wierd char fix
    //dec vars
    FILE* filePointer = fopen(fileName, "r");
    char** stringArray = calloc(1, sizeof(stringArray));
    char* stringBuffer = calloc(1, sizeof(stringBuffer));
    int charBuffer;
    int stringSize = 0;
    int lineSize = 0;

    //read the entire file to the string
    while(true){
        charBuffer = getc(filePointer);
        if(charBuffer == EOF){
            break;
        }//end if
        stringBuffer[stringSize] = (char)charBuffer;
        stringSize++;
        stringBuffer = realloc(stringBuffer, sizeof(stringBuffer)*(stringSize+1));
        stringBuffer[stringSize] = '\0';
    }//end if*/
    
    //remove the \r\n by tokenizing and adding to array
    char* token = strtok(stringBuffer, "\r\n");
    while(token != NULL){
        //stringArray[lineSize] = calloc(512, sizeof(char));
        stringArray[lineSize] = setString(token);
        strcpy(stringArray[lineSize], token);
        debug("Tokenized: %s\n", stringArray[lineSize]);
        lineSize++;
        stringArray = realloc(stringArray, sizeof(stringArray)*(lineSize+1));
        token = strtok(NULL, "\r\n");
    }//end while

    //debug("%s", stringBuffer);
    *arraySize = lineSize;
    free(stringBuffer);
    fclose(filePointer);
    return stringArray;
}//end func

void freeStringArray(char** array, int arraySize){
    for(int x = 0; x < arraySize; x++) {
        free(array[x]);
    }//end if
    free(array);
}//end func

bool checkEmail(char* string){
    //dec vars
    int foundAtSign = 0;
    int foundPeriod = 0;

    //if string size is greater than 5 it is impossible to be an email
    int stringLength = strlen(string);
    if(DEBUG_HELPER)printf("String length: %d\n", stringLength);
    if(stringLength < 5){
        return false;
    }//end if

    //go through the string and check
    for(int x=1; x<strlen(string)-1;x++){
        if (string[x-1] != '@' && string[x] == '@' && string[x+1] != '@'){
            foundAtSign++;
            if(DEBUG_HELPER)printf("Found '@'.\n");
        }//end if
        if( (string[x-1] != '@' && string[x-1] != '.') && string[x] == '.' && ALPHABET_RANGE(string[x+1]) && foundAtSign == 1 ){
            foundPeriod++;
            if(DEBUG_HELPER)printf("Found '.'.\n");
        }//end if
    }//end for loop

    //return
    if (foundAtSign == 1 && foundPeriod >= 1){
        return true;
    }//end if
    return false;
}//end func

int compareString(char* string1, char* string2){
    //set temp string for the strings to not be manipulated
    char* tempString1 = setString(string1);
    char* tempString2 = setString(string2);
    //compare with having both lower case
    int result = strcmp(setLowerCase(tempString1), setLowerCase(tempString2));
    //free and return
    free(tempString1);
    free(tempString2);
    return result;
}//end func

int searchStringArray(char* stringArray[], char* stringToBeSearched, int arraySize, char* type){
    //dec vars
    int index = -999;
    int foundString = 0;
    //loop through to find the string
    for (int x = 0; x < arraySize; x++){
        //if the string to be searched is found returned the size
        if (compareString(stringArray[x], stringToBeSearched) == 0){
            index = x; 
            foundString++;
        }//end if
    }//for loop
    //return based on the searched type
    if(compareString(type, "index") == 0){
        return index;
    }else{
        return foundString;
    }//end if
}//end func

char* combinedToken(char** token, int tokenSize, int tokenStartIndex){//untested = so far it works
    //prevent memory leaked
    if(tokenStartIndex > tokenSize){
        return NULL;
    }//end if
    //dec vars
    int mallocSize = 256 + strlen(token[tokenStartIndex]);
    char* combinedString = calloc(1 ,sizeof(char)*(mallocSize));
    int x = 0;
    //loop through and combined the string to one
    for(x=tokenStartIndex; x<tokenSize; x++){
        //start to combined the string
        char* newString = setString(token[x]);
        strcat(combinedString, newString);
        free(newString);
        if(x == tokenSize){
            strcat(combinedString, "\0");
            break;
        }//end if
        strcat(combinedString, " ");
        mallocSize = mallocSize + strlen(token[x]);
        combinedString = realloc(combinedString, sizeof(char)*mallocSize);
    }//end if
    combinedString[strlen(combinedString)-1] = '\0';
    
    if(DEBUG_UTILITIES)printf("combinedToken: %s\n", combinedString);
    return combinedString;
}//end func

void removeSpace(char* string){
    //remove the space of the white spaces of the string
    if(string == NULL){
        return;
    }//end if
    if(strlen(string) < 3){
        return;
    }//end if
    //right to left
    for(int x = strlen(string)-1; x > 0; x--){
        if(string[x] == ' '){
            string[x] = '\0';
        }else if(string[x] != ' '){
            break;
        }//end
    }//end if
    //left to right
    for(int x = 0; x < strlen(string)-1; x++){
        if(string[x] == ' '){
            string[x] = '\0';
        }else if(string[x] != ' '){
            break;
        }//end
    }//end if
}//end func

void removeLineEnding(char* string){
    for(int x=0; x<255; x++){
        if(string[x] == '\r' || string[x] == '\n'){
            string[x] = '\0';
            break;
        }else if(string[x] == '\0'){
            return;
        }//end if
    }//end for
}//end func

/***********************************************************
 * constructors for abstract data
 ***********************************************************/

Event* initEvent(){
    Event* newEvent = calloc(1, sizeof(Event));
    newEvent->type[0] = '\0';
    newEvent->date = NULL;
    newEvent->place = NULL;
    newEvent->otherFields = initializeList(printField, deleteField, compareFields);
    return newEvent;
}//end constructor

Field* initField(){
    Field* newField = calloc(1, sizeof(Field));
    newField->tag = NULL;
    newField->value = NULL;
    return newField;
}//end construtor

Submitter* initSubmitter(){
    Submitter* newSub = calloc(1, sizeof(Submitter) + sizeof(char [999]));
    newSub->address[0] = '\0';
    newSub->submitterName[0] = '\0'; 
    newSub->otherFields = initializeList(printField, deleteField, compareFields);
    return newSub;
}//end constructor

Header* initHeader(){
    Header* newHeader = calloc(1, sizeof(Header));
    newHeader->source[0] = '\0';
    newHeader->gedcVersion = -1;
    newHeader->encoding = ASCII;//not sure if its ascii
    newHeader->submitter = NULL;//need to be init
    newHeader->otherFields = initializeList(printField, deleteField, compareFields);
    return newHeader;
}//end constructor

Individual* initIndividual(){
    Individual* newIndividual = calloc(1, sizeof(Individual));;
    newIndividual->givenName = NULL;
    newIndividual->surname = NULL;
    newIndividual->events = initializeList(printEvent, deleteEvent, compareEvents);
    newIndividual->families = initializeList(printFamily, dummyDelete, compareFamilies);
    newIndividual->otherFields = initializeList(printField, deleteField, compareFields);
    return newIndividual;
}//end constructor

Family* initFamily(){
    Family* newFamily = calloc(1, sizeof(Family));
    newFamily->wife = NULL;
    newFamily->husband =NULL;
    newFamily->children = initializeList(printIndividualName, dummyDelete, compareIndividuals);
    newFamily->events = initializeList(printEvent, deleteEvent, compareEvents);
    newFamily->otherFields = initializeList(printField, deleteField, compareFields);
    return newFamily;
}//end construstor

GEDCOMobject* initObject(){
    GEDCOMobject* newObject = calloc(1, sizeof(GEDCOMobject));
    newObject->header = initHeader();
    newObject->families = initializeList(printFamily, deleteFamily, compareFamilies);
    newObject->individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    newObject->submitter = NULL;//need to be init
    return newObject; 
}//end constructor

GEDCOMerror initError(ErrorCode errorCode, int line){
    #define DEBUG_ERROR false
    GEDCOMerror newError;
    newError.type = errorCode;
    newError.line = line;//not sure if it start with 1 or 0
    char* printString = printError(newError);
    if(DEBUG_ERROR)printf("%s\n", printString);
    free(printString);
    return newError;
}//end constructor

HelperVars initVars(char* fileName, char** stringArrayOfFile, int lineSize){
    //dec vars
    HelperVars newVars;
    newVars.object = initObject();
    newVars.indiRef = initializeList(printReference, deleteIndiRef, compareIndiRef);
    newVars.famRef = initializeList(printReference, deleteFamRef, compareFamRef);
    //newVars.garbageFamily = initializeList(printFamily, deleteGarbageFamily, compareFamilies);
    strcpy(newVars.fileName, fileName);
    //vars for the file
    newVars.lineLocation = 0;
    newVars.lineSize = lineSize;
    newVars.lineString = stringArrayOfFile;
    //dec found vars for GED_COM
    newVars.foundHead = 0;
    newVars.foundTrailer = 0;
    newVars.foundSubmitter = 0;
    newVars.foundRecord = 0;
    //dec vars for the header tags
    newVars.foundSourceTag = 0;
    newVars.foundGedcVersionTag = 0;
    newVars.foundGedcTag = 0;
    newVars.foundEncodingTag = 0;
    newVars.foundSubmitterTag = 0;
    newVars.foundEndHeader = 0;
    newVars.endHeaderLineLocation = 0;
    //for individual
    newVars.foundStartIndividual = 0;
    newVars.foundIndividual = 0;
    newVars.foundStartNameTag = 0;
    newVars.foundNameTag = 0;
    newVars.foundGivenNameTag = 0;
    newVars.foundSurnameTag = 0;
    //for family
    newVars.foundStartFamily = 0;
    newVars.foundFamily = 0;
    newVars.foundHusband = 0;
    newVars.foundWife = 0;
    newVars.foundChild = 0;
    //return the init helper vars
    newVars.counter = 0;
    newVars.foundGarbageFamily = 0;
    return newVars;
}//end constructor

IndividualReference* initIndiRef(char* ref, Individual* indi){
    IndividualReference* newIndi = calloc(1, sizeof(IndividualReference));
    newIndi->reference = setString(ref);
    newIndi->individual = indi;
    newIndi->found = false;
    newIndi->foundFAMS = false;
    newIndi->foundFAMC = false;
    newIndi->famcRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->famsRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newIndi;
}//end constructor

FamilyReference* initFamRef(char* ref, Family* fam){
    FamilyReference* newFam = calloc(1, sizeof(FamilyReference));
    newFam->reference = setString(ref);
    newFam->family = fam; 
    newFam->found = false;
    newFam->husbRef = NULL;
    newFam->wifeRef = NULL;
    newFam->childRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newFam;
}//end constructor

IndividualReference* initIndividualReference(){
    IndividualReference* newIndi = calloc(1, sizeof(IndividualReference));
    newIndi->reference = NULL;
    newIndi->individual = NULL;
    newIndi->found = false;
    newIndi->foundFAMS = false;
    newIndi->foundFAMC = false;
    newIndi->famcRef = initializeList(dummyPrint, freeObject, dummyCompare);
    newIndi->famsRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newIndi;
}//end constructor

FamilyReference* initFamilyReference(){
    FamilyReference* newFam = calloc(1, sizeof(FamilyReference));
    newFam->reference = NULL;
    newFam->family = NULL; 
    newFam->found = false;
    newFam->husbRef = NULL;
    newFam->wifeRef = NULL;
    newFam->childRef = initializeList(dummyPrint, freeObject, dummyCompare);
    return newFam;
}//end constructor

/***********************************************************
 * main checker helper functions
 ***********************************************************/

GEDCOMerror checkFile(char* fileName){//checked
    if(DEBUG_UTILITIES)printf("\n******Invoking checkFile function*****\n");
    //check if the file name is empty
    if(fileName == NULL){
        fprintf(stderr, "File name cannot be empty string.\n");
        return initError(INV_FILE, -1);
    }//end if

    //check if the file names end with ".ged"
    if (strstr(fileName, ".ged") == NULL) {
        fprintf(stderr, "File extension must be a (.ged).\n");
        return initError(INV_FILE, -1);
    }//end if

    //check if the file can be opened
    FILE* fileCheck = fopen(fileName, "r");
    if(fileCheck == NULL){
        fprintf(stderr, "File cannot be opened.\n");
        return initError(INV_FILE, -1);;
    }//end if
    fclose(fileCheck);

    //return
    return initError(OK, -1);
}//end file check

GEDCOMerror checkGedcom(HelperVars* vars){//worked, no leak
    LIST_OF_FAMILY_GARBAGE = initializeList(printFamily, deleteGarbageFamily, compareFamilies);
    if(DEBUG_UTILITIES)printf("\n******Invoking checkGedcom function*****\n");
    //check if there is error with gedcom
    for(int x=0; x<vars->lineSize;x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        static char* recordList[] = {
            //index 0
            "INDI", "FAM", "SUBM",
            //the for the main GEDCOM record
            "TRLR", "HEAD"
        };//end array
        printToken(token, tokenSize);

        //make sure that the token is at least 2
        if(tokenSize > 1){
            //check the head
            if(tokenSize == 2){
                if(strcmp(token[1], "HEAD") == 0){
                    vars->foundHead++;
                    if(DEBUG_UTILITIES)printf("foundHead: %d\n", vars->foundHead);
                }//end if
            }//end if
            
            //check if record exist
            if(tokenSize == 3){
                if(searchStringArray(recordList, token[2], RECORD_SIZE, "FOUND") > 0){
                    vars->foundRecord++;
                    if(DEBUG_UTILITIES)printf("foundRecord: %d\n", vars->foundRecord);
                    //check if it is SUBM record
                    if(strcmp(token[2], "SUBM") == 0){
                        vars->foundSubmitter++;
                        if(DEBUG_UTILITIES)printf("foundSubmitter: %d\n", vars->foundSubmitter);
                    }//end if
                }//end if
            }//end func

            //check for TRLR
            if(vars->foundHead == 1 && vars->foundSubmitter == 1 && vars->foundRecord >= 1 ){
                //make sure the token size is 2
                if(tokenSize == 2){
                    if(strcmp(token[1], "TRLR") == 0){
                        vars->foundTrailer++;
                        if(DEBUG_UTILITIES)printf("foundTrailer: %d\n", vars->foundTrailer);
                        freeStringArray(token, tokenSize);
                        break;
                    }//end if
                }//end if
            }else if(vars->foundHead > 1){
                if(DEBUG_UTILITIES)printf("Found duplicate heads\n");
                freeStringArray(token, tokenSize);
                return initError(INV_GEDCOM, -1);
            }//end if
        }//end if

        //free the token
        freeStringArray(token, tokenSize);
    }//end for

    //return the result
    debug("foundHead: %d, foundTrailer: %d, foundSubmitter: %d\n", vars->foundHead, vars->foundTrailer, vars->foundSubmitter);
    if(vars->foundHead == 1 && vars->foundTrailer == 1 && vars->foundSubmitter == 1){
        return initError(OK, -1);
    }else{
        return initError(INV_GEDCOM, -1);
    }//end if
}//end func

GEDCOMerror checkHeader(HelperVars* vars){//worked no leak
    if(DEBUG_UTILITIES)printf("\n*****Invoking checkHeader function*****\n");
    //dec vars
    int lastLine = 0;
    bool otherField = true;
    bool foundHead = false;

    //check if the is an error in header
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        otherField = true;
        printToken(token, tokenSize);

        //check if there is a new record *****!!!!! not sure if it can end if "O TRLR" 
        if(tokenSize == 2){
            if(strcmp(token[1], "HEAD") == 0){
                foundHead = true;
                otherField = false;
                if(DEBUG_UTILITIES)printf("foundHead: %d\n", vars->foundHead);
                //check the correct line number
                if(strcmp(token[0], "0") != 0){
                    freeStringArray(token, tokenSize);
                    return initError(INV_HEADER, x+1);
                }//end if
            }//end if
        }else if(strcmp(token[0], "0") == 0){
            if(tokenSize == 3 && foundHead == true){
                vars->foundEndHeader++;
                vars->endHeaderLineLocation = x+1;
                lastLine = x;
                if(DEBUG_UTILITIES)printf("lastLine: %d\n", lastLine);
                freeStringArray(token, tokenSize);
                break;
            }//end if
        }//end if
        
        //check grammar
        if(checkIfValidLine(vars->lineString[x]) == false){
            freeStringArray(token, tokenSize);
            return initError(INV_RECORD, x+1);
        }//end if

        //make sure that the token is greater 1
        if(tokenSize > 1 && foundHead == true){
            //check the source
            if(tokenSize >= 2){
                if(strcmp(token[1], "SOUR") == 0){
                    otherField = false;
                    vars->foundSourceTag++;
                    if(DEBUG_UTILITIES)printf("foundSourceTag: %d\n", vars->foundSourceTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 || tokenSize == 2){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //assign the source to the object
                    char* combinedString = combinedToken(token, tokenSize, 2);
                    strcpy(vars->object->header->source, combinedString);
                    free(combinedString);
                    if(DEBUG_UTILITIES)printf("source: %s\n", vars->object->header->source);
                }//end if
            }//end if
         
            //check the gedc version
            if(tokenSize == 2){
                if(strcmp(token[1], "GEDC") == 0){
                    otherField = false;
                    vars->foundGedcTag++;
                    if(DEBUG_UTILITIES)printf("foundGedcTag: %d\n", vars->foundGedcTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                }//end if
            }//end if

            //check the important data in gedc
            if(tokenSize >= 2){
                //found vers inside gedc
                if(vars->foundGedcTag == 1 && strcmp(token[1], "VERS") == 0){
                    otherField = false;
                    vars->foundGedcVersionTag++;
                    if(DEBUG_UTILITIES)printf("foundGedcVersionTag: %d\n", vars->foundGedcVersionTag);
                    //check the correct line number
                    if(strcmp(token[0], "2") != 0 || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //set the gedc version
                    vars->object->header->gedcVersion = atof(token[2]);
                    if(DEBUG_UTILITIES)printf("gedcVersion: %f\n", vars->object->header->gedcVersion);
                }//end if
            }//end if

            //check the endcoding
            if(tokenSize >= 2){
                if(strcmp(token[1], "CHAR") == 0){
                    otherField = false;
                    vars->foundEncodingTag++;
                    if(DEBUG_UTILITIES)printf("foundEncodingTag: %d\n", vars->foundEncodingTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //check the encoding
                    CharSet encoding = ASCII;
                    if(strcmp("ANSEL", token[2]) == 0){
                        encoding = ANSEL;
                    }else if(strcmp("UTF-8", token[2]) == 0 || strcmp("UTF8", token[2]) == 0){
                        encoding = UTF8;
                    }else if(strcmp("UNICODE", token[2]) == 0){
                        encoding = UNICODE;
                    }//end if
                    //set the encoding
                    vars->object->header->encoding = encoding;
                    if(DEBUG_UTILITIES){
                        static char* encodingType[] = {
                            "ANSEL", "UTF-8", "UNICODE", "ASCII"
                        };//end array
                        printf("encoding: %s\n", encodingType[vars->object->header->encoding]);
                    }//end debug
                }//end if
            }//end if

            //check the reference for the submitter exist
            if(tokenSize >= 2){
                if(strcmp(token[1], "SUBM") == 0){
                    otherField = false;
                    vars->foundSubmitterTag++;
                    if(DEBUG_UTILITIES)printf("foundSubmitterTag: %d\n", vars->foundSubmitterTag);
                    //check the correct line number
                    if(strcmp(token[0], "1") != 0 /*|| tokenSize != 3*/){
                        freeStringArray(token, tokenSize);
                        return initError(INV_HEADER, x+1);
                    }//end if
                    //check if it is reference proper grammar
                    if(tokenSize == 3){
                        if(checkIfReference(token[2]) == false){
                            freeStringArray(token, tokenSize);
                            return initError(INV_HEADER, x+1);
                        }//end if
                    }//end if
                    //add submitter to the struct
                    getSubmitter(token[2], vars);
                }//end if
            }//end if

            //for the other field
            if(tokenSize > 1 && otherField == true){
                if(checkStringIfCaps(token[1])){
                    //set the other field
                    Field* tempField = initField();
                    tempField->tag = setString(token[1]);
                    if(tokenSize > 2){
                        tempField->value = combinedToken(token, tokenSize, 2);
                    }else{
                        tempField->value = setString("");
                    }//end if
                    insertFront(&vars->object->header->otherFields, (void*)tempField);
                    if(DEBUG_UTILITIES)printf("%s", printField(tempField));
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for
    
    //return
    if(vars->foundSourceTag != 1 || vars->foundGedcVersionTag != 1 || vars->foundEncodingTag != 1 || vars->foundSubmitterTag != 1){
        return initError(INV_HEADER, lastLine);
    }else{
        return initError(OK, -1);
    }//end if
}//end func*/

GEDCOMerror checkRecord(HelperVars* vars){//no leak
    if(DEBUG_UTILITIES)printf("\n*****Invoking checkRecord function*****\n");
    //dec vars
    GEDCOMerror error = initError(OK, -1);
    if(DEBUG_UTILITIES)printf("endHeaderLineLocation: %d\n", vars->endHeaderLineLocation-1);

    //get the indi
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
      
        if(checkIfValidLine(vars->lineString[x]) == false){
            freeStringArray(token, tokenSize);
            return initError(INV_RECORD, x+1);
        }//end if
        freeStringArray(token, tokenSize);
    }//end if

    //check if the is an error in record
    //IMPORTANT this loop may be using at the start of endHeader location!!!***
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        //dec needed vars
        //if(DEBUG_UTILITIES)printf("\n*****Save the record reference*****\n");
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        printToken(token, tokenSize);

        //make sure that the token is greater 1
        if(tokenSize > 1){
            //check the grammar for the record
            if(checkIfStartRecord(vars->lineString[x]) == true){
                //count as thae start of the record for the next
            
                //check if it is an individual
                if(strcmp(token[0], "0") == 0 && strcmp(token[2], "INDI") == 0){
                    vars->foundStartIndividual++;
                    if(DEBUG_UTILITIES)printf("foundStartIndividual: %d\n", vars->foundStartIndividual);
                    //error check the ref
                    if(checkIfReference(token[1]) == false || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_RECORD, x+1);
                    }//end if
                    //set the ref
                    //Individual* newIndi = initIndividual();
                    //newIndi->givenName = setString("FUCK");
                    //insertSorted(&vars->object->individuals, newIndi);
                    insertSorted(&vars->indiRef, initIndiRef(token[1], NULL));
                }//end if

                //check if it is a family
                if(strcmp(token[0], "0") == 0 && strcmp(token[2], "FAM") == 0){
                    vars->foundStartFamily++;
                    if(DEBUG_UTILITIES)printf("foundStartFamily: %d\n", vars->foundStartFamily);
                    //error hec the ref
                    if(checkIfReference(token[1]) == false || tokenSize != 3){
                        freeStringArray(token, tokenSize);
                        return initError(INV_RECORD, x+1);
                    }//end if
                    //set the fam
                    // Family* newFam = initFamily();
                    // insertSorted(&vars->object->families, newFam);
                    insertSorted(&vars->famRef, initFamRef(token[1], NULL));
                }//end if

                if((strcmp(token[0], "0") == 0 && strcmp(token[2], "SUBM") == 0) || tokenSize != 3){
                    if(DEBUG_UTILITIES)printf("foundSubmitter: %d\n", vars->foundSubmitter);
                }//end if

            }else if(checkIfValidLine(vars->lineString[x]) != true){
                //error if there is no number and less then 2 token
                freeStringArray(token, tokenSize);
                return initError(INV_RECORD, x+1);
            }else if(strcmp(token[0], "0") == 0 && strcmp(token[1], "TRLR") == 0){
                //stop loop when the program end
                freeStringArray(token, tokenSize);
                if(DEBUG_UTILITIES)printf("End of line from checkRecord\n");
                break;
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for

    //get the indi
    if(DEBUG_UTILITIES)printf("\n*****Invoking get function*****\n");
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        if(tokenSize == 3){
            if(strcmp(token[0], "0") == 0 && strcmp(token[2], "INDI") == 0){
                if(DEBUG_PARSER)printf("FOUND INDI: %s\n", token[1]);
                error = getIndividual(token[1], vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    return error;
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end if

    //get the fam
    for(int x=vars->endHeaderLineLocation-1; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        if(tokenSize == 3){
            if(strcmp(token[0], "0") == 0 && strcmp(token[2], "FAM") == 0){
                //if(DEBUG_PARSER)printf("FOUND FAM: %s\n", token[1]);
                error = getFamily(token[1], vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    return error;
                }//end if
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end if

    //return
    return error;
}//end func

/***********************************************************
 * getter functions for parsing
 ***********************************************************/

GEDCOMerror getSubmitter(char* reference, HelperVars* vars){//worked, called sub free, no leak
    Submitter* tempSubmitter = initSubmitter();
    int foundAddress = 0;
    int foundName = 0;
    bool foundStart = false;
    //loop through and get the submitter data
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " /", &tokenSize);
        
        //check where the start of the reference
        if(tokenSize > 1 && strcmp(token[1], reference) == 0){
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
                foundStart = true;
                if(tokenSize != 3){
                    deleteSubmitter(tempSubmitter);
                    freeStringArray(token, tokenSize);
                    return initError(INV_RECORD, x+1);
                }//end if
            }//end if
        }else if(strcmp(token[0], "0") == 0 && foundStart == true){
            //set to the object and break
            vars->object->header->submitter = tempSubmitter;
            vars->object->submitter = tempSubmitter;
            if(DEBUG_UTILITIES)printf("%s", printSubmitter(tempSubmitter));
            //name is a must in submitter
            if(foundName != 1){
                deleteSubmitter(tempSubmitter);
                freeStringArray(token, tokenSize);
                return initError(INV_RECORD, x+1);
            }//end
            freeStringArray(token, tokenSize);
            break;
        }//end if
            
        //check the body of the record
        if(foundStart == true){
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "NAME") == 0){
                //get the name
                foundName++;
                if(tokenSize <= 2){
                    deleteSubmitter(tempSubmitter);
                    freeStringArray(token, tokenSize);
                    return initError(INV_RECORD, x+1);
                }//end if
                char* name = combinedToken(token, tokenSize, 2);
                strcpy(tempSubmitter->submitterName, name);
                free(name);
                if(DEBUG_UTILITIES)printf("!!!!! submitterName: %s = %s\n", name, tempSubmitter->submitterName);
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "ADDR") == 0){
                //get the address
                foundAddress++;
                char* address = combinedToken(token, tokenSize, 2);
                if(address != NULL){
                    strcpy(tempSubmitter->address, address);
                }//end if 
                free(address);
            }else if(strcmp(token[0], "0") != 0){
                //get the other field
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempSubmitter->otherFields, otherField);
            }//end if
        }//end if
        freeStringArray(token, tokenSize);
    }//end for
    return initError(OK, -1);
}//end func

GEDCOMerror getIndividual(char* reference, HelperVars* vars){
    //dec vars
    Individual* tempIndividual = initIndividual();
    Event* tempEvent = NULL;
    GEDCOMerror error = initError(OK, -1);
    char pointerFAMS[256] = {"\0"};
    char pointerFAMC[256] = {"\0"};
    char pointerIndi[256] = {"\0"};
    bool foundFAMS = false;
    bool foundFAMC = false;
    //bool male = false;
    bool foundEvent = false;
    bool foundStart = false;
    int eventSize = 23;
    static char* eventTypes[] = {"BIRT", "CHR", "DEAT", "BURI", "CREM", "ADOP",
                               "BAPM", "BARM", "BASM", "BLES", "CHRA", "CONF", "FCOM", "ORDN",
                               "NATU", "EMIG", "IMMI", "CENS", "PROB", "WILL", "GRAD", "RETI", "EVEN"};

    //loop and get the data of the indi
    for(int x=0; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        printToken(token, tokenSize);

        //start and the end of record
        if(tokenSize > 1){
             //at the end
            if(strcmp(token[0], "0") == 0 && foundStart == true){
                //set the main to the indi struct
                IndividualReference* tempSearch = initIndiRef(pointerIndi, NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    if(foundFAMS == true){
                        indiLocation->foundFAMS = true;
                    }//end if
                    if(foundFAMC == true){
                        indiLocation->foundFAMC = true;
                    }//end if
                    indiLocation->individual = tempIndividual;
                    insertSorted(&vars->object->individuals, tempIndividual);
                }//end if
                deleteIndiRef(tempSearch);
                
                // link the parent
                // if(foundFAMS == true && male == true){
                //     //set the fam husband
                //     FamilyReference* tempSearch = initFamRef(pointerFAMS, NULL);
                //     FamilyReference* husbandLocation = findElement(vars->famRef, findFamRef, tempSearch);
                //     husbandLocation->family->husband = tempIndividual;
                //     deleteFamRef(tempSearch);
                // }else if(foundFAMS == true && male == false){
                //     //set the fam wife
                //     FamilyReference* tempSearch = initFamRef(pointerFAMS, NULL);
                //     FamilyReference* wifeLocation = findElement(vars->famRef, findFamRef, tempSearch);
                //     wifeLocation->family->wife = tempIndividual;
                //     deleteFamRef(tempSearch);
                // }//end if

                // //link the child
                // if(foundFAMC == true){
                //     FamilyReference* tempSearch = initFamRef(pointerFAMC, NULL);
                //     FamilyReference* childLocation = findElement(vars->famRef, findFamRef, tempSearch);
                //     insertSorted(&childLocation->family->children, tempIndividual);
                //     deleteFamRef(tempSearch);
                // }//end if

                // if(foundEvent == true){
                //     deleteEvent(tempEvent);
                // }//end if
                freeStringArray(token, tokenSize);
                break;
            }//end i

            //start
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
               foundStart = true; 
               strcpy(pointerIndi, token[1]);
            }//end if
        }//end if

        //check the body of the record
        if(foundStart == true && tokenSize > 1){
            //find the name
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "NAME") == 0){
                if(tokenSize > 2){
                    //get the string of the first name
                    char* nameValue = combinedToken(token, tokenSize, 2);
                    if(nameValue != NULL){
                        //split the string to get the first and last name
                        int nameTokenSize = 0;
                        char** nameToken = split(nameValue, "/", &nameTokenSize);
                        if(nameTokenSize < 2){
                            if(token[2][0] == '/'){
                                removeSpace(nameToken[0]);
                                tempIndividual->surname = setString(nameToken[0]);
                                tempIndividual->givenName = setString("");
                            }else if(nameTokenSize == 1){
                                removeSpace(nameToken[0]);
                                tempIndividual->surname = setString("");
                                tempIndividual->givenName = setString(nameToken[0]);
                            }//end if
                        }else if(nameTokenSize > 1){
                            removeSpace(nameToken[0]);
                            removeSpace(nameToken[1]);
                            tempIndividual->givenName = setString(nameToken[0]);
                            tempIndividual->surname = setString(nameToken[1]);
                        }//end if
                        freeStringArray(nameToken, nameTokenSize);
                    }//end if
                    free(nameValue);
                }//end if
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "SEX") == 0){
                //get the sex
                if(tokenSize > 2){
                    if(strcmp(token[2], "M")){
                        //male = true;
                    }//end if
                    if(tokenSize != 3){
                        error = initError(INV_RECORD, x+1);
                        freeStringArray(token, tokenSize);
                        deleteIndividual(tempIndividual);
                        deleteEvent(tempEvent);
                        break;
                    }//end if
                    Field* otherField = initField();
                    otherField->tag = setString(token[1]);
                    otherField->value = setString(token[2]);
                    insertSorted(&tempIndividual->otherFields, otherField);
                }else{
                    if(DEBUG_UTILITIES)printf("FOUND NO SEX VALUE!!!!!");
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "FAMS") == 0){
                //check the fam tags
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                foundFAMS = true;
                strcpy(pointerFAMS, token[2]);
            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "FAMC") == 0){
                //check the fam tags
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteIndividual(tempIndividual);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                foundFAMC = true;
                strcpy(pointerFAMC, token[2]);
            }else if(strcmp(token[0], "1") == 0 && searchStringArray(eventTypes, token[1], eventSize, "FOUND") > 0){
                //with temp indi assign the event
                foundEvent = true;
                tempEvent = initEvent();
                error = getEvent(tempEvent, tempIndividual, x, "Individual", vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempIndividual);
                    deleteEvent(tempEvent);
                    return error;
                }//end for
                strcpy(tempEvent->type, token[1]);
            }else if(strcmp(token[0], "0") != 0 && foundEvent == false && !(strcmp(token[1], "SURN") == 0 || strcmp(token[1], "GIVN") == 0)){
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempIndividual->otherFields, otherField);
                //if(DEBUG_UTILITIES)printf("other field: %s\n" , printField(otherField));
            }else if( (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 ) && foundEvent == true){
                foundEvent = false;
            }//end if

        }//end if
        freeStringArray(token, tokenSize);
    }//end for
    return error;
}//end func

GEDCOMerror getFamily(char* reference, HelperVars* vars){
    #define DEBUG_FAMILY false
    if(DEBUG_FAMILY)printf("\n*****Invoking checkFamily function*****\n");
    GEDCOMerror error = initError(OK, -1);
    Event* tempEvent = NULL;
    Family* tempFamily = initFamily();

    // Family* famCopy = calloc(1, sizeof(Family));
    // Family* middleChildFam = calloc(1, sizeof(Family));
    // insertSorted(&vars->garbageFamily, famCopy);
    // insertSorted(&vars->garbageFamily, middleChildFam);

    char pointerFam[256] = {"\0"};
    char pointerHusband[256] = {"\0"};
    char pointerWife[265] = {"\0"};
    bool foundHusband = false;
    bool foundWife = false;
    bool foundEvent = false;
    bool foundStart = false;
    int foundChild = 0;
    int eventSize = 12;
    static char* eventTypes[] = {"ANUL", "CENS", "DIV", "DIVF", "ENGA", "MARB", "MARC",
                        "MARR", "MARL", "MARS", "RESI", "EVEN"};

    for(int x=0; x<vars->lineSize; x++){
        //dec vars
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);

        //start of the records
        if(tokenSize > 1){
            //at the end of the record
            if(strcmp(token[0], "0") == 0 && foundStart == true){
                FamilyReference* tempSearch = initFamRef(pointerFam, NULL);
                FamilyReference* famLocation = findElement(vars->famRef, findFamRef, tempSearch);
                if(DEBUG_UTILITIES)printf("end of family: \t\t\t\t%s %d\n", pointerFam, vars->counter);
                famLocation->family = tempFamily;
                insertSorted(&vars->object->families, tempFamily);
                //if(DEBUG_UTILITIES)printf("\nfamLocation: %s\n\n",  printFamily( tempFamily));
                freeStringArray(token, tokenSize);
                deleteFamRef(tempSearch);

                // set the husb and wife family linkage
                // if(foundHusband == true){
                //     if(DEBUG_UTILITIES)printf("pointerHusband: %s\n", pointerHusband);
                //     //link the husband
                //     IndividualReference* husbandSearch = initIndiRef(pointerHusband, NULL);
                //     IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, husbandSearch);
                //     if(indiLocation != NULL && indiLocation->found == false){
                //         tempFamily->husband = indiLocation->individual;
                //         indiLocation->found = true;
                //         insertSorted(&indiLocation->individual->families, tempFamily); 
                //     }//end if
                //     deleteIndiRef(husbandSearch);
                // }//end if

                // if(foundWife == true){
                //     if(DEBUG_UTILITIES)printf("pointerWife: %s\n", pointerWife);
                //     IndividualReference* wifeSearch = initIndiRef(pointerWife, NULL);
                //     IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, wifeSearch);
                //     if(indiLocation != NULL && indiLocation->found == false){
                //         tempFamily->wife = indiLocation->individual;
                //         indiLocation->found = true;
                //         insertSorted(&indiLocation->individual->families, tempFamily);  
                //     }//end if
                //     deleteIndiRef(wifeSearch);
                // }//end if
                
                // if(foundEvent == true){
                //     deleteEvent(tempEvent);
                // }//end if
                break;
            }//end if

            //check where the start of the reference
            if(strcmp(token[0], "0") == 0 && strcmp(token[1], reference) == 0){
                foundStart = true;
                strcpy(pointerFam, token[1]);
                vars->counter++;
                if(DEBUG_UTILITIES)printf("initFamily: %d\n", vars->counter);
            }//end if 
        }//end if

        //body of the record
        if(foundStart == true && tokenSize > 1){
            if(strcmp(token[0], "1") == 0 && strcmp(token[1], "HUSB") == 0){
                //check husband
                vars->foundHusband++;
                //foundHusband = true;
                strcpy(pointerHusband, token[2]);
                if(DEBUG_FAMILY)printf("foundHusband: %d\n", vars->foundHusband);
                //error checked
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                // link the husband
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    ///if(indiLocation->foundFAMS == true && indiLocation->foundFAMC == false /*&& indiLocation->found == false*/){
                        if(DEBUG_UTILITIES)printf("HUSBAND NAME:\t%s %s\t\t%s\t%d\n", indiLocation->individual->givenName, indiLocation->individual->surname, token[2], vars->counter);
                        foundHusband = true;
                        indiLocation->found = true;
                        indiLocation->foundFAMS = true;
                        tempFamily->husband = indiLocation->individual;
                        insertSorted(&indiLocation->individual->families, tempFamily); 
                    //}//end if 
                }//end if
                deleteIndiRef(tempSearch);

            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "WIFE") == 0){
                //check wife
                vars->foundWife++;
                //foundWife = true;
                strcpy(pointerWife, token[2]);
                //error check
                if(DEBUG_FAMILY)printf("foundWife: %d\n", vars->foundWife);
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                // link the wife
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    //if(indiLocation->foundFAMS == true && indiLocation->foundFAMC == false /*&& indiLocation->found == false*/){
                        if(DEBUG_UTILITIES)printf("WIFE NAME:\t%s %s\t\t%s\t%d\n", indiLocation->individual->givenName, indiLocation->individual->surname,token[2], vars->counter);
                        foundWife = true;
                        indiLocation->found = true;
                        indiLocation->foundFAMS = true;
                        tempFamily->wife = indiLocation->individual;
                        insertSorted(&indiLocation->individual->families, tempFamily);
                    //}//end if 
                }//end if
                deleteIndiRef(tempSearch);

            }else if(strcmp(token[0], "1") == 0 && strcmp(token[1], "CHIL") == 0){
                //check chil
                vars->foundChild++;
                foundChild++;
                //error check
                if(DEBUG_FAMILY)printf("foundChild: %d\n", vars->foundChild);
                if(tokenSize != 3){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if
                if(checkIfReference(token[2]) != true){
                    error = initError(INV_RECORD, x+1);
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    break;
                }//end if

                //line the child 
                IndividualReference* tempSearch = initIndiRef(token[2], NULL);
                IndividualReference* indiLocation = findElement(vars->indiRef, findIndiRef, tempSearch);
                if(indiLocation != NULL){
                    //if(indiLocation->foundFAMC == true && indiLocation->foundFAMS == false /*&& indiLocation->found == false*/){

                        //for fixing the leave tree and event linking (nuclearFam)
                        if(indiLocation->foundFAMC == true && indiLocation->foundFAMS == false){
                            //indiLocation->found == true;
                            // Individual* tempIndi = initIndividual();
                            // tempIndi->givenName = indiLocation->individual->givenName;
                            // tempIndi->surname = indiLocation->individual->surname;
                            // tempIndi->events = indiLocation->individual->events;
                            //insertSorted(&famCopy->children, tempIndi);
                            vars->foundGarbageFamily++;
                            Family* famCopy = calloc(1, sizeof(Family));

                            famCopy->events = tempFamily->events;
                            if(foundHusband == true){
                                famCopy->husband = tempFamily->husband;
                            }//end if
                            if(foundWife == true){
                                famCopy->wife = tempFamily->wife;
                            }//end if
                            
                            insertSorted(&LIST_OF_FAMILY_GARBAGE, famCopy);
                            insertSorted(&indiLocation->individual->families, famCopy);//there is segfault here, be carefull
                        }//end if

                        //fixing simpleValid2GenE1 when child in the middle
                        if(indiLocation->foundFAMC == true && indiLocation->foundFAMS == true){
                            //indiLocation->found == true;
                            vars->foundGarbageFamily++;
                            Family* middleChildFam = calloc(1, sizeof(Family));
                            
                            middleChildFam->events = tempFamily->events;
                            if(foundHusband == true){
                                middleChildFam->husband = tempFamily->husband;
                            }//end if 
                            if(foundWife == true){
                                middleChildFam->wife = tempFamily->wife;
                            }//end if
                            
                            insertSorted(&LIST_OF_FAMILY_GARBAGE, middleChildFam);                            
                            insertSorted(&indiLocation->individual->families, middleChildFam);
                        }//end if

                        // debug("DEBUG NUMBER OF INDI IN REF: %d\n", getLength(vars->indiRef));
                        // debug("DEBUG INDI PASS FAM: %sFAM LENGTH: %d\n", printIndividualName(indiLocation->individual), getLength(indiLocation->individual->families));
                        // debug("DEBUG INDI PASS FAM PRINT FAM: %s\n", toString(indiLocation->individual->families));
                        //indiLocation->found = true;
                    //}//end if
                }//end if
                deleteIndiRef(tempSearch);
                insertSorted(&tempFamily->children, indiLocation->individual);
                
            }else if(strcmp(token[0], "1") == 0 && searchStringArray(eventTypes, token[1], eventSize, "FOUND") > 0){
                //with temp indi assign the event
                foundEvent = true;
                tempEvent = initEvent();
                error = getEvent(tempEvent, tempFamily, x, "Family", vars);
                if(error.type != OK){
                    freeStringArray(token, tokenSize);
                    deleteFamily(tempFamily);
                    deleteEvent(tempEvent);
                    return error;
                }//end for
                strcpy(tempEvent->type, token[1]);
            }else if(strcmp(token[0], "0") != 0 && foundEvent == false){
                Field* otherField = initField();
                otherField->tag = setString(token[1]);
                //get the value for the field
                if(tokenSize > 2){
                    otherField->value = combinedToken(token, tokenSize, 2);
                }else{
                    otherField->value = setString("");
                }//end if
                insertSorted(&tempFamily->otherFields, otherField);
            }else if( (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 ) && foundEvent == true){
                foundEvent = false;
            }//end if
        }//end if

        freeStringArray(token, tokenSize);
    }//end for
    
    //return
    return error;
}//end func

GEDCOMerror getEvent(Event* tempEvent, void* object, int lineStartNumber, char* type, HelperVars* vars){
    //dec vars
    GEDCOMerror error = initError(OK, -1);
    bool foundDate = false;
    bool foundPlace = false;
    bool foundEvent = true;
    bool foundEventBody = false;
    Family* tempFamily = NULL;
    Individual* tempIndividual = NULL;

    //cast based on type
    if(compareString(type, "Family") == 0){
        tempFamily = (Family*)object;
    }else{
        tempIndividual = (Individual*)object;
    }//end if
    
    for(int x=lineStartNumber; x<vars->lineSize; x++){
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);

        //when found event
        if(tokenSize > 1){
            //when found event
            if(DEBUG_UTILITIES)printf("found event\n");
            //check if there is the data for event
            if(strcmp(token[0], "2") == 0 && tokenSize > 2){
                foundEventBody = true;
                if(strcmp(token[1], "DATE") == 0){
                    foundDate = true;
                    tempEvent->date = combinedToken(token, tokenSize, 2);
                }else if(strcmp(token[1], "PLAC") == 0){
                    foundPlace = true;
                    tempEvent->place = combinedToken(token, tokenSize, 2);
                }else{
                    Field* tempField = initField();
                    tempField->tag = setString(token[1]);
                    char* tempValue = combinedToken(token, tokenSize, 2);
                    if(tempValue == NULL){
                        tempField->value = setString("");
                        free(tempValue);
                    }else{
                        tempField->value = tempValue;
                    }//end if
                    insertSorted(&tempEvent->otherFields, tempField);
                }//end if
            }else if(foundEvent == true && foundEventBody == true && (strcmp(token[0], "0") == 0 || strcmp(token[0], "1") == 0 )){
                //end of event
                if(foundPlace == false){
                    tempEvent->place = setString("");
                }else if(foundDate == false){
                    tempEvent->date = setString("");
                }//end if
                foundDate = false;
                foundPlace = false;
                foundEvent = false;
                foundEventBody = false;
                //cast based on type
                if(compareString(type, "Family") == 0){
                    insertSorted(&tempFamily->events, tempEvent);
                }else{
                    insertSorted(&tempIndividual->events, tempEvent);
                }//end if
                //at the end of event
                freeStringArray(token, tokenSize);
                break;
            }//end if
        }//end if*/

        freeStringArray(token, tokenSize);
    }//end for
    return error;
}//end func

/***********************************************************
 * getter and setter as helper functions
 ***********************************************************/

char* getNameOfIndividual(Individual* individual){
    static char string[256];
    sprintf(string, "%s %s", individual->givenName, individual->surname);
    return string;
}//end func

void setErrorCode(GEDCOMerror* object, ErrorCode errorCode, int line){
    object->type = errorCode;
    object->line = line;
}//end func

/***********************************************************
 * sub checkers helper functions
 ***********************************************************/

void printToken(char** token, int tokenSize){
    for(int x=0; x<tokenSize; x++){
        if(DEBUG_UTILITIES)printf("\tToken %d: %s\n", x, token[x]);
    }//end if
}//end func

bool checkStringIfNumbers(char* string){
    //make sure it doesnt start at 0
    if(strlen(string) > 1 && string[0] == '0'){
        return false;
    }//end if

    //loop through each char and check if its number
    for(int x=0; x<strlen(string); x++){
        if(!NUMBER_RANGE(string[x])){
            return false;
        }//end if
    }//end for
    return true;
}//end func

bool checkStringIfCaps(char* string){
    //loop through each char and check if its upper case
    for(int x=0; x<strlen(string); x++){
        if(!UPPERCASE_RANGE(string[x])){
            return false;
        }//end if
    }//end for
    return true;
}//end func

bool checkIfReference(char* string){
    #define DEBUG_FUNC false
    //chec if the @ at the start 
    if(string[0] != '@' || string[strlen(string)-1] != '@'){
        return false;
    }//end if
    if(DEBUG_FUNC)printf("Ref1: %c, Ref2: %c\n", string[0], string[strlen(string)-1]);

    //check if inside @ is not alpha or number
    if(DEBUG_FUNC)printf("Inside the ref: ");
    for(int x=1; x<strlen(string)-1; x++){
        if(DEBUG_FUNC)printf("%c", string[x]);
        if( !(ALPHABET_RANGE(string[x]) || NUMBER_RANGE(string[x])) ){
            return false;
        }//end if
    }//end for
    if(DEBUG_FUNC)printf("\n");

    //check pass
    return true;
}//end func

bool checkIfValidLine(char* stringLine){
    //dec vars
    int tokenSize = 0;
    char** token = split(stringLine, " ", &tokenSize);
    bool validTag = false;

    //chec for line length
    if(strlen(stringLine) > 200){
        return false;
    }

    //check if token is greater than 2
    if(tokenSize < 2){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //check if it is a number and a valid tag
    if(checkStringIfNumbers(token[0]) == true /*&& checkStringIfCaps(token[1]) == true*/){
        validTag = true;
    }//end if
    /*
    if(checkStringIfNumbers(token[0]) == true && checkIfReference(token[1]) == true){
        validTag = true;
    }//end if*/

    //check if the tag is valid
    if(validTag == false){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //valid line
    freeStringArray(token, tokenSize);
    return true;
}//end func

bool checkIfStartRecord(char* stringLine){
    //dec vars
    int tokenSize = 0;
    char** token = split(stringLine, " ", &tokenSize);

    //check if it is exactly 3 token
    if(tokenSize != 3){
        freeStringArray(token, tokenSize);
        return false;
    }//end if
    
    //check the if is record format
    if(strcmp(token[0], "0") != 0){
        freeStringArray(token, tokenSize);
        return false;
    }//end if

    //free and return
    freeStringArray(token, tokenSize);
    return true;
}//end fund

bool checkIfLastName(char* string){
    //check if it is the right format for the last name
    if(strlen(string) < 2){
        return false;
    }//end if

    if(string[0] != '/' || string[strlen(string)-1] != '/'){
        return false;
    }//end if

    //return
    return true;
}//end func

GEDCOMerror checkIfLineNumbers(HelperVars* vars){
    //loop until pass or found error
    for(int x=0; x < vars->lineSize-1; x++){
        if(checkIfValidLine(vars->lineString[x]) == false){
            return initError(INV_RECORD, x+1);
        }//end if

        //dec vars
        int tokenSize = 0;
        char** token = split(vars->lineString[x], " ", &tokenSize);
        int postTokenSize = 0;
        char** postToken = split(vars->lineString[x+1], " ", &postTokenSize);
        
        //make sure that line number exist before converting
        if(checkStringIfNumbers(token[0]) == false){
            freeStringArray(token, tokenSize);
            freeStringArray(postToken, postTokenSize);
            return initError(INV_RECORD, x+1);
        }//end if

        //convert to number
        int lineNum = atoi(token[0]);
        int postLineNum = atoi(postToken[0]);
        int lineDistance = postLineNum - lineNum;

        //check if line numbers is correct
        if(lineDistance > 1){
            freeStringArray(token, tokenSize);
            freeStringArray(postToken, postTokenSize);
            return initError(INV_RECORD, x+2);
        }//end if

        //free
        freeStringArray(token, tokenSize);
        freeStringArray(postToken, postTokenSize);
    }//end for
    return initError(OK, -1);
}//end func

/***********************************************************
 * other get children / get descendances helper functions
 ***********************************************************/

List combinedIndividualList(List* list1, List* list2){
    //dec vars
    List combinedList = initializeList(printDescendants, dummyDelete, compareIndividuals);
    ListIterator iterList1 = createIterator(*list1);
    ListIterator iterList2 = createIterator(*list2);
    //loop through the firsst list
    for(int x = 0; x < getLength(*list1); x++){
        Individual* tempIndi = nextElement(&iterList1);
        insertBack(&combinedList, tempIndi);
    }//end for
    //loop through the second list
    for(int x = 0; x < getLength(*list2); x++){
        Individual* tempIndi = nextElement(&iterList2);
        insertBack(&combinedList, tempIndi);
    }//end for
    clearList(list1);
    clearList(list2);
    return combinedList;
}//end func

List getChildren(Individual* person){
    if(DEBUG_UTILITIES)printf("calling get children\n");
    // char* printIndiInFam = toString(person->families);
    // if(DEBUG_UTILITIES)printf("\nprinting persons fam ***%s\n", printIndiInFam);
    // free(printIndiInFam);
    //dec vars
    List descendantsList = initializeList(printDescendants, dummyDelete, compareIndividuals);
    //check if its empty
    if(person != NULL){
        ListIterator iterateFam = createIterator(person->families);
        if(DEBUG_UTILITIES)printf("getLength: %d\n", getLength(descendantsList));
        for(int x = 0; x <= getLength(descendantsList)+1; x++){
            Family* family = nextElement(&iterateFam);
            if(family == NULL){
                continue;
            }//end if
            char* printFam = printFamily(family);
            if(DEBUG_UTILITIES)printf("printFam: %s\n", printFam);
            free(printFam);
            
            //get the each child
            ListIterator iterateChild = createIterator(family->children);
            for(int i = 0; i <= getLength(family->children)+1; i++){
                if(DEBUG_UTILITIES)printf("in children loop: %d\n", i);
                Individual* individual = nextElement(&iterateChild);
                if(individual == NULL){
                    continue;
                }//end if
                insertSorted(&descendantsList, individual);
                List tempChildList = getChildren(individual);
                descendantsList = combinedIndividualList(&descendantsList, &tempChildList);
            }//end for 
        }//end for
    }else{
        if(DEBUG_UTILITIES)printf("person is null\n");
    }//end if
    //return
    if(DEBUG_UTILITIES)printf("return the list of children\n");
    return descendantsList;
}//end funcs

/***********************************************************
 * dummy functions
 ***********************************************************/

void dummyDelete(void* toBeDeleted){
    //does nothing
}//end func

int dummyCompare(const void* first,const void* second){
    return 0;
}//end func

char* dummyPrint(void* toBePrinted){
    return NULL;
}//end func

void freeObject(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    //check if null
    free(toBeDeleted);
}//end func

/***********************************************************
 * other linklist helper functions
 ***********************************************************/

bool comparePerson(const void* first, const void* second){
    //declare vars
    Individual* person1 = (Individual*)first;
    Individual* person2 = (Individual*)second;
    if(compareIndividuals(person1, person2) == 0){
        return true;
    }else{
        return false;
    }//end if
}//end func

char* printSubmitter(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if
    
    //print the object
    Submitter* object = (Submitter*)toBePrinted;
    char* stringField = toString(object->otherFields);
    int stringSize = 99 + 256 + strlen(object->submitterName) + strlen(stringField);
    char* stringToPrint = calloc(1, sizeof(char)*stringSize);
    //assign the print
    strcat(stringToPrint, "submiter name: |");
    strcat(stringToPrint, object->submitterName);
    strcat(stringToPrint, "|\nOther fields:\n");
    strcat(stringToPrint, stringField);
    strcat(stringToPrint, "Address: |");
    strcat(stringToPrint, object->address);
    strcat(stringToPrint, "|\n\0");
    //free
    free(stringField);
    return stringToPrint;
}//end func

char* printIndividualName(void* toBePrinted){//checked
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if
    
    Individual* object = (Individual*)toBePrinted;
    char* eventString = toString(object->events);
    char* famString = toString(object->families);
    char* fieldString = toString(object->otherFields);
    int mallocSize = 99 + strlen(eventString) + strlen(famString) + strlen(fieldString);
    char* stringToPrint = calloc(1, sizeof(char)* mallocSize );
    //make sure you dont combine the 
    strcat(stringToPrint, "Name: |");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, "| |");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if
    /**strcat(stringToPrint, "\nEvents: \n");
    strcat(stringToPrint, eventString);
    strcat(stringToPrint, "\nFamilies: \n");
    strcat(stringToPrint, famString);
    strcat(stringToPrint, "Other fields: \n");
    strcat(stringToPrint, fieldString);*/
    strcat(stringToPrint, "|\n\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

void deleteIndiRef(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if

    //free mem
    IndividualReference* object = (IndividualReference*)toBeDeleted;
    //clearList(&object->individual->families);
    free(object->reference);
    clearList(&object->famcRef);
    clearList(&object->famsRef);
    free(object);
}//end func

void deleteFamRef(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if

    //free mem
    FamilyReference* object = (FamilyReference*)toBeDeleted;
    free(object->reference);
    if(object->husbRef != NULL){
        free(object->husbRef);
    }//end if
    if(object->wifeRef != NULL){
        free(object->wifeRef);
    }//end if
    clearList(&object->childRef);
    free(object);
}//end func

void deleteGarbageFamily(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    Family* object = (Family*)toBeDeleted;
    //clearList(&object->events);
    //clearList(&object->otherFields);
    free(object);
}//end func

char* printReference(void* toBePrinted){
    //just a dummy for now
    return setString("printReference has not been setup\n");
}//end func

int compareIndiRef(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if
    
    //dec vars
    IndividualReference* object1 = (IndividualReference*)first;
    IndividualReference* object2 = (IndividualReference*)second;
    char individual1[256];
    char individual2[256];
    strcpy(individual1, object1->reference);
    strcpy(individual2, object2->reference);
    return strcmp(individual1, individual2);
}//end func

int compareFamRef(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if
    
    //dec vars
    FamilyReference* object1 = (FamilyReference*)first;
    FamilyReference* object2 = (FamilyReference*)second;
    char family1[256];
    char family2[256];
    strcpy(family1, object1->reference);
    strcpy(family2, object2->reference);
    return strcmp(family1, family2);
}//end func

bool findIndiRef(const void* first, const void* second){
    //declare vars
    IndividualReference* person1 = (IndividualReference*)first;
    IndividualReference* person2 = (IndividualReference*)second;
    if(strcmp(person1->reference, person2->reference) == 0){
        //if(DEBUG_UTILITIES)printf("FOUND INDI REF\n");
        return true;
    }else{
        return false;
    }//end if
}//end func

bool findFamRef(const void* first, const void* second){
    //declare vars
    FamilyReference* fam1 = (FamilyReference*)first;
    FamilyReference* fam2 = (FamilyReference*)second;
    if(strcmp(fam1->reference, fam2->reference) == 0){
        //if(DEBUG_UTILITIES)printf("FOUND FAM REF\n");
        return true;
    }else{
        return false;
    }//end if
}//end func

void deleteSubmitter(Submitter* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }//end if
    //free the field
    clearList(&toBeDeleted->otherFields);
    free(toBeDeleted);
}//end func

void deleteHeader(Header* toBeDeleted){
    deleteSubmitter(toBeDeleted->submitter);
    clearList(&toBeDeleted->otherFields);
    free(toBeDeleted);
}//end func

char* printDescendants(void* toBePrinted){
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if
    
    Individual* object = (Individual*)toBePrinted;
    char* eventString = toString(object->events);
    char* famString = toString(object->families);
    char* fieldString = toString(object->otherFields);
    int mallocSize = 99 + strlen(eventString) + strlen(famString) + strlen(fieldString);
    char* stringToPrint = calloc(1, sizeof(char)* mallocSize );
    //make sure you dont combine the 
    strcat(stringToPrint, "Name: ");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, " ");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if
    // strcat(stringToPrint, "\nEvents: \n");
    // strcat(stringToPrint, eventString);
    // /*strcat(stringToPrint, "\nFamilies: \n");
    // strcat(stringToPrint, famString);*/
    // strcat(stringToPrint, "Other fields: \n");
    // strcat(stringToPrint, fieldString);
    strcat(stringToPrint, "\n\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

char* printEncoding(CharSet encoding){
    //array for tags and enums etc
    static char* charType[] = {
        "ANSEL", "UTF-8", "UNICODE", "ASCII"
    };//end array
    //print the error if there is error
    char* stringToPrint = calloc(1, sizeof(char)* 99);
    sprintf(stringToPrint, "%s", charType[encoding]); 
    strcat(stringToPrint, "\0");
    //return the string
    return stringToPrint;
}//end func

void deleteFamilyGarbageWrapper(){
    clearList(&LIST_OF_FAMILY_GARBAGE);
}//end func

/***********************************************************
 * Helper function for the writing gedcom
 ***********************************************************/

void writeFile(const char* fileName, char* string){
    FILE* filePointer = fopen(fileName, "w");
    //check if there is an error opening a file
    if(filePointer == NULL){
        debug("Error opening a file for writing\n");
    }//end if
    fprintf(filePointer, "%s", string);
    fclose(filePointer);
}//end func

char* getFileString(List* famList, List* indiList, const GEDCOMobject* object){
    char* headerString = getFileHeader(object->header);
    char* submitterString = getFileSubmitter(object->submitter);
    char* individualString = getFileIndividual(indiList);
    char* familyString = getFileFamily(famList);
    char* string = calloc(1, sizeof(char)* (strlen(headerString)+strlen(submitterString)+strlen(individualString)+strlen(familyString)+256) );
    strcat(string, "0 HEAD\n");
    strcat(string, headerString);
    strcat(string, submitterString);
    strcat(string, individualString);
    strcat(string, familyString);
    strcat(string, "0 TRLR\n\0");
    
    free(headerString);
    free(submitterString);
    free(individualString);
    free(familyString);
    return string;
}//end func

char* getFileHeader(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //setup the mem and vars
    Header* object = (Header*)toBePrinted;
    char* string = calloc(1, sizeof(char)*999);
    char* encodingPointer = printEncoding(object->encoding);

    //set the string
    sprintf(string, "1 SOUR %s\n1 CHAR %s\n1 GEDC\n2 VERS %0.2lf\n2 FORM LINEAGE-LINKED\n", 
        object->source, encodingPointer, object->gedcVersion);
    strcat(string, "1 SUBM @SUB1@\n");
    free(encodingPointer);
    return string;
}//end func

char* getFileSubmitter(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if
    
    //print the object
    Submitter* object = (Submitter*)toBePrinted;
    int stringSize = 99 + 256 + 999 + strlen(object->submitterName);
    char* stringToPrint = calloc(1, sizeof(char)*stringSize);

    //name and address
    char* name = calloc(1, sizeof(char)* (strlen(object->submitterName)+99));
    char* address = calloc(1, sizeof(char)* (strlen(object->address)+99) );
    sprintf(name, "0 @SUB1@ SUBM\n1 NAME %s\n", object->submitterName);
    if(strlen(object->address) > 1){
        sprintf(address, "1 ADDR %s\n", object->address);
    }//end if
    
    //assign the print
    strcat(stringToPrint, name);
    //strcat(stringToPrint, address);
    free(name);
    free(address);
    return stringToPrint;
}//end func

char* getFileIndividual(List* indiList){
    //dec vars
    ListIterator indiIterator = createIterator(*indiList);
    int memSize = 256*3;
    char surname[265];
    char givenName[265];
    char refBuffer[265];
    char* stringToBeReturned = calloc(1, sizeof(char)*((memSize*INDI_EVENT_SIZE)*getLength(*indiList)));
    
    //for loop to to the whole indi list
    for(int x=0; x<getLength(*indiList); x++){
        //set the vars
        IndividualReference* tempIndi = nextElement(&indiIterator);
        memSize = memSize + (256*3) + (256*3 * getLength(tempIndi->individual->events) );
        char* stringBuffer = calloc(1, sizeof(char)*(memSize));
       
        //for the name tag
        sprintf(stringBuffer, "0 %s INDI\n1 NAME %s /%s/\n", tempIndi->reference, tempIndi->individual->givenName, tempIndi->individual->surname);
        
        //for givenname tag
        if(strlen(tempIndi->individual->givenName) > 1){
            sprintf(givenName, "1 GIVN %s\n", tempIndi->individual->givenName);
            strcat(stringBuffer, givenName);
        }//end if
        
        //for the surname tag
        if(strlen(tempIndi->individual->surname) > 1){
            sprintf(surname, "1 SURN %s\n", tempIndi->individual->surname);
            strcat(stringBuffer, surname);
        }//end if

        if(getLength(tempIndi->individual->events) != 0){
            char* eventBuffer = getFileEvent(&tempIndi->individual->events);
            strcat(stringBuffer, eventBuffer);
            free(eventBuffer);
        }//end if

        // if(getLength(tempIndi->individual->otherFields) != 0){
        //     char* fieldBuffer;
        // }//end if
        
        //for the fams ref
        ListIterator famsIterator = createIterator(tempIndi->famsRef);
        for(int y=0; y<getLength(tempIndi->famsRef) ;y++){
            char* famsString = nextElement(&famsIterator);
            sprintf(refBuffer, "1 FAMS %s\n", famsString);
            strcat(stringBuffer, refBuffer);
        }//end for

        //for the famc ref
        ListIterator famcIterator = createIterator(tempIndi->famcRef);
        for(int y=0; y<getLength(tempIndi->famcRef) ;y++){
            char* famcString = nextElement(&famcIterator);
            sprintf(refBuffer, "1 FAMC %s\n", famcString);
            strcat(stringBuffer, refBuffer);
        }//end for
        
        strcat(stringToBeReturned, stringBuffer); 
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

char* getFileFamily(List* famList){
    //dec vars
    ListIterator famIterator = createIterator(*famList);
    int memSize = 256*3;
    char refBuffer[256];
    char* stringToBeReturned = calloc(1, sizeof(char)*( (memSize*FAM_EVENT_SIZE)*getLength(*famList)) );
    
    //loop through the whole fam list
    for(int x=0; x<getLength(*famList); x++){
        //set the vars
        FamilyReference* tempFam = nextElement(&famIterator);
        memSize = memSize + 256 + (256 * getLength(*famList) + getLength(tempFam->family->events));
        char* stringBuffer = calloc(1, sizeof(char)*memSize);

        //for the record fam tag
        sprintf(stringBuffer, "0 %s FAM\n", tempFam->reference);

        //for the husb tag
        if(tempFam->husbRef != NULL){
            sprintf(refBuffer, "1 HUSB %s\n", tempFam->husbRef);
            strcat(stringBuffer, refBuffer);
        }//end if

        //for the wife tag
        if(tempFam->wifeRef != NULL){
            sprintf(refBuffer, "1 WIFE %s\n", tempFam->wifeRef);
            strcat(stringBuffer, refBuffer);
        }//end if

        if(getLength(tempFam->family->events) != 0){
            char* eventBuffer = getFileEvent(&tempFam->family->events);
            strcat(stringBuffer, eventBuffer);
            free(eventBuffer);
        }//end if

        //for the child tags
        ListIterator childIterator = createIterator(tempFam->childRef);
        for(int y=0; y<getLength(tempFam->childRef) ;y++){
            char* childString = nextElement(&childIterator);
            sprintf(refBuffer, "1 CHIL %s\n", childString);
            strcat(stringBuffer, refBuffer);
        }//end for
        
        strcat(stringToBeReturned, stringBuffer); 
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

char* getFileEvent(List* eventList){
    //dec vars
    ListIterator eventIterator = createIterator(*eventList);
    int memSize = 256*3;
    char dateString[256];
    char placeString[256];
    char* stringToBeReturned = calloc(1, sizeof(char)*(memSize*getLength(*eventList)));
    //for loop event list unti
    for(int x=0; x<getLength(*eventList) ;x++){
        //set the vars
        Event* tempEvent = nextElement(&eventIterator);
        memSize = memSize + 256 + (256 * getLength(*eventList));
        char* stringBuffer = calloc(1, sizeof(char)*memSize);

        //for the event type tag
        sprintf(stringBuffer, "1 %s\n", tempEvent->type);

        if(strlen(tempEvent->date) > 1 ){
            sprintf(dateString, "2 DATE %s\n", tempEvent->date);
            strcat(stringBuffer, dateString);
        }//end if

        if(strlen(tempEvent->place) > 1 ){
            sprintf(placeString, "2 PLAC %s\n", tempEvent->place);
            strcat(stringBuffer, placeString);
        }//end if
        
        strcat(stringToBeReturned, stringBuffer); 
        free(stringBuffer);
    }//end for

    return stringToBeReturned;
}//end func

void setReferences(List* famList, List* indiList, const GEDCOMobject* object){
    //create iterator for the list of family and indi
    ListIterator famIterator = createIterator(object->families);
    ListIterator indiIterator = createIterator(object->individuals);

    //creata a vars for counter the code
    for(int x=1; x<=getLength(object->individuals); x++){
        char referenceCode[99];
        sprintf(referenceCode, "@I%d@", x);
        IndividualReference* indiRef = initIndiRef(referenceCode, nextElement(&indiIterator));
        insertBack(indiList, indiRef);
    }//end for

    for(int x=1; x<=getLength(object->families); x++){
        char referenceCode[99];
        sprintf(referenceCode, "@F%d@", x);
        FamilyReference* famRef = initFamRef(referenceCode, nextElement(&famIterator));
        insertBack(famList, famRef);
    }//end for
}//end func

void linkReferences(List* famList, List* indiList){
    //loop through the all the fam and link with the indi
    ListIterator famIterator = createIterator(*famList);
    for(int x=0; x<getLength(*famList); x++){
        FamilyReference* famRef = nextElement(&famIterator);
        IndividualReference* searchIndividual = initIndividualReference();

        //searched for the husband for the family pointer
        searchIndividual->individual = famRef->family->husband;
        IndividualReference* husbSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
        if(husbSearchedResult != NULL){
            //if(husbSearchedResult->famsRef != NULL){
                insertBack(&husbSearchedResult->famsRef, setString(famRef->reference));
                famRef->husbRef = setString(husbSearchedResult->reference); 
            //}//end if
        }//end if

        //searched for the wife for the family pointer
        searchIndividual->individual = famRef->family->wife;
        IndividualReference* wifeSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
        if(wifeSearchedResult != NULL){
            //if(wifeSearchedResult->famsRef != NULL){
                insertBack(&wifeSearchedResult->famsRef, setString(famRef->reference));
                famRef->wifeRef = setString(wifeSearchedResult->reference);
            //}//end if
        }//end if
        
        //searched for the child for familly pointer
        ListIterator childIterator = createIterator(famRef->family->children);
        for(int y=0; y<getLength(famRef->family->children); y++){
            Individual* currentChildren = nextElement(&childIterator);

            //search the child
            searchIndividual->individual = currentChildren;
            IndividualReference* childSearchedResult = findElement(*indiList, findIndividualPointer, searchIndividual);
            if(childSearchedResult != NULL){
                // if(childSearchedResult->famcRef != NULL){
                    insertBack(&childSearchedResult->famcRef, setString(famRef->reference));
                    insertBack(&famRef->childRef, setString(childSearchedResult->reference));
                //}//end if
            }//end if
        }//end for

        deleteIndiRef(searchIndividual);
    }//end for
}//end func

bool findIndividualPointer(const void* first, const void* second){
    //error check
    if(first == NULL || second == NULL){
        return false;
    }//end if
    //cast objects
    IndividualReference* object1 = (IndividualReference*)first;
    IndividualReference* object2 = (IndividualReference*)second;
    //declare vars
    if(object1->individual == object2->individual){
        return true;
    }else{
        return false;
    }//end if
}//end func

ErrorCode validateEvents(List* event){
    ErrorCode error = OK;
    
    //check event
    ListIterator eventIterator = createIterator(*event);
    for(int y=0; y<getLength(*event); y++){
        Event* currentEvent = nextElement(&eventIterator);
        
        //check if the event is not null
        if(currentEvent == NULL){
            return INV_RECORD;
        }//end if

        //check the type
        if(strlen(currentEvent->type) > 4){
            return INV_RECORD;
        }//end if

        //check the date
        if(currentEvent->date == NULL){
            return INV_RECORD;
        }//end if
        if(strlen(currentEvent->date) > 200){
            return INV_RECORD;
        }//end if

        //check the place
        if(currentEvent->place == NULL){
            return INV_RECORD;
        }//end if
        if(strlen(currentEvent->place) > 200){
            return INV_RECORD;
        }//end if

    }//end for
    return error;
}//end func

/***********************************************************
 * mains functions for the writing gedcom
 ***********************************************************/

GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj){
    //check the file name
    GEDCOMerror error = initError(OK, -1);

    //validate the object
    error.type = validateGEDCOM(obj);
    if(error.type != OK){
        return error;
    }//end if

    //create a list of family and indi
    List famList = initializeList(printReference, deleteFamRef, compareFamRef);
    List indiList = initializeList(printReference, deleteIndiRef, compareIndiRef);
    //set the reference pointer
    setReferences(&famList, &indiList, obj);
    linkReferences(&famList, &indiList);

    //write to the file
    char* fileOutput = getFileString(&famList, &indiList, obj);
    writeFile(fileName, fileOutput);
    clearList(&famList);
    clearList(&indiList);
    free(fileOutput);
    return error;
}//func

ErrorCode validateGEDCOM(const GEDCOMobject* obj){
    //dec vars
    ErrorCode error = OK;

    //checking the invalid gedcom
    if(obj == NULL){
        debug("object is null\n");
        return INV_GEDCOM;
    }//end if
    if(obj->header == NULL || obj->submitter == NULL){
        debug("header and subm is null\n");
        return INV_GEDCOM;
    }//end if

    //checking the invalid header !!! not sure if source is mandatory
    Header* header = obj->header;
    if(strlen(header->source) == 0 || strlen(header->source) > 200 || (header->gedcVersion < 1) || header->submitter == NULL){
        debug("invalid header struct\n");
        return INV_HEADER;
    }//end if

    //checking the submitter name
    Submitter* submitter = obj->submitter;
    if(strlen(submitter->submitterName) == 0 || strlen(submitter->submitterName) > 60){
        debug("invalid subm name\n");
        return INV_RECORD;
    }//end if

    //check indi records
    ListIterator indiIterator = createIterator(obj->individuals);
    for(int x=0; x<getLength(obj->individuals) ;x++){
        Individual* currentIndi = nextElement(&indiIterator);
        
        //check if indi is null
        if(currentIndi == NULL){
            debug("indi is null\n");
            return INV_RECORD;
        }//end if

        //check for the first name
        if(currentIndi->givenName == NULL){
            debug("given name is null\n");
            return INV_RECORD;
        }//end if
        if(strlen(currentIndi->givenName) > 200){
            debug("given name is over 200\n");
            return INV_RECORD;
        }//end if

        //check for the last name
        if(currentIndi->surname == NULL){
            debug("surname is null\n");
            return INV_RECORD;
        }//end if
        if(strlen(currentIndi->surname) > 200){
            debug("surname is over 200\n");
            return INV_RECORD;
        }//end if

        //check for the events
        error = validateEvents(&currentIndi->events);
        if(error != OK){
            debug("invalid indi events\n");
            return INV_RECORD;
        }//end if

        //check families?

    }//end for

    //check indi records
    ListIterator famIterator = createIterator(obj->families);
    for(int x=0; x<getLength(obj->families) ;x++){
        Family* currentFam = nextElement(&famIterator);
        
        //check if fam is null
        if(currentFam == NULL){
            debug("fam is null\n");
            return INV_RECORD;
        }//end if

        //check children?

        error = validateEvents(&currentFam->events);
        if(error != OK){
            debug("invalid fam events\n");
            return INV_GEDCOM;
        }//end if

    }//end for

    return error;
}//end func
