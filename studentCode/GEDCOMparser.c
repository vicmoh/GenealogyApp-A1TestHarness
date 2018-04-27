/**
 * @file GEDCOMparser.c
 * @author Vicky Mohammad
 * @date 2018
 * @brief main parser functions
 */

//import lib
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

/***********************************************************
 * parser functions
 ***********************************************************/

GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
    if(DEBUG_PARSER)printf("********************************************************* INVOKING CREATE OBJECT\n");
    debug("*************************************** %s INVOKING CREATE OBJECT\n", fileName);
    *obj = NULL;

    //check the file name
    GEDCOMerror error = initError(OK, -1);
    error = checkFile(fileName);
    if(error.type != OK){
        return error;
    }//end if

    //dec vars
    int lineSize = 0;
    char** line = readFileByChar(fileName, &lineSize/*, 256*/);
    HelperVars vars = initVars(fileName, line, lineSize);//dont forget to free the object after

    //check the gedcom
    error = checkGedcom(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        debug("CHECK GEDCOM\n%s\n", printError(error));
        return error;
    }//end if

    error = checkIfLineNumbers(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        debug("CHECK LINE\n%s\n", printError(error));
        return error;
    }//end if
    
    //check the header
    error = checkHeader(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        debug("CHECK HEADER\n%s\n", printError(error));
        return error;
    }//end if*/

    //check the records 
    error = checkRecord(&vars);
    if(error.type != OK){
        clearList(&vars.indiRef);
        clearList(&vars.famRef);
        freeStringArray(line, lineSize);
        deleteGEDCOM(vars.object);
        *obj = NULL;
        debug("CHECK RECORD\n%s\n", printError(error));
        return error;
    }//end if*/

    //save the struct to the object
    *obj = vars.object;

    //debuging
    // if(strcmp("testFiles/valid/simpleValid1N1R.ged", fileName) == 0){
    //     debug("MINE\n%s", printGEDCOM(*obj));
    //     //debug("TEST\n%s", printGEDCOM(refObj));
    //     //testing the search for the individual
    //     Individual* tempIndi = initIndividual();
    //     tempIndi->givenName = setString("James");
    //     tempIndi->surname = setString("Smith");
    //     Individual* indiLocation = findPerson(vars.object,  comparePerson, tempIndi);
    //     Family* tempFam = getFromBack(indiLocation->families);
    //     //print family
    //     debug("PRINT FAMILY: %s\n", printFamily(tempFam));
    //     debug("NUMBER INDI: %d\n", getLength(vars.object->individuals));
    //     debug("NUMBER FAMILIES: %d\n", getLength(indiLocation->families));
    //     //print the descendant
    //     List testList = getDescendants(vars.object, tempIndi);
    //     debug("GET CHILDREN: FROM %sLIST:\n%s\n", printIndividualName(tempIndi), toString(testList) );
    //     //debug("Test Length %d\n", (int)strlen(refObj->header->source) );
    //     writeFile("./writeTest.ged", printGEDCOM(vars.object));
    // }//end if

    // if(strcmp("testFiles/valid/shakespeare.ged", fileName) == 0){
    //     Individual* tempIndi = initIndividual();
    //     tempIndi->givenName = setString("William");
    //     tempIndi->surname = setString("Shakespeare");
    //     List testList = getDescendants(vars.object, tempIndi);
    //     debug("GET CHILDREN GET CHILDREN GET CHILDREN GET CHILDREN GET CHILDREN %sLIST:\n%s\n", printIndividualName(tempIndi), toString(testList) );
    // }//end if
    
    //return
    freeStringArray(line, lineSize);
    // if(vars.foundGarbageFamily > 0){
    //     clearList(&vars.garbageFamily);
    // }//end if
    
    //clearList(&vars.garbageFamily);
    clearList(&vars.indiRef);
    clearList(&vars.famRef);
    char* printErrorString = printError(error);
    debug("%s\n", printErrorString);
    free(printErrorString);
    return error; 
}//end parser func

char* printGEDCOM(const GEDCOMobject* obj){//its done
    //print if there is no object
    char* stringToPrint = setString("There is nothing to print\n");
    int stringSize = 399;
    int memSize = 999;
    char versionString[256];
    static char* encodingType[] = {
        "ANSEL", "UTF-8", "UNICODE", "ASCII"
    };//end array

    //check if object is null
    if(obj != NULL){
        //reset the stringtoprint so it doesnt show the prev string
        free(stringToPrint);
        stringToPrint = calloc(1, sizeof(char) * stringSize * memSize);

        //print the header
        strcat(stringToPrint, "********************((( HEADER )))********************\n");
        strcat(stringToPrint, "Header source: |");
        strcat(stringToPrint, obj->header->source);
        strcat(stringToPrint, "|\nHeader version: ");
        sprintf(versionString, "%0.2f", obj->header->gedcVersion);
        strcat(stringToPrint, versionString);
        strcat(stringToPrint, "\nEncoding: ");
        strcat(stringToPrint, encodingType[obj->header->encoding]);

        //print the other field of the header
        strcat(stringToPrint, "\nHeader other field:\n");
        char* headerString = toString(obj->header->otherFields);
        memSize = memSize + (strlen(headerString) * stringSize);
        stringToPrint = realloc(stringToPrint, sizeof(char*) * memSize);
        char* otherFieldString = toString(obj->header->otherFields);
        strcat(stringToPrint, otherFieldString);
        
        //print the family and the individual
        char* indiString = toString(obj->individuals);
        char* famString = toString(obj->families);
        memSize = memSize + strlen(indiString) + strlen(famString) + ((getLength(obj->individuals)*9) + (getLength(obj->families)*9)); 
        stringToPrint = realloc(stringToPrint, sizeof(char*)* memSize);
        strcat(stringToPrint, "********************((( INDIVIDUAL )))********************\n");
        //strcat(stringToPrint, "List indivdual:\n");
        strcat(stringToPrint, indiString);
        strcat(stringToPrint, "\n********************((( FAMILIES )))********************\n");
        //strcat(stringToPrint, "List families:\n");
        strcat(stringToPrint, famString);
        strcat(stringToPrint, "\n********************((( SUBMITTER )))********************\n");
        char* subString = printSubmitter(obj->submitter);
        strcat(stringToPrint, subString);
        strcat(stringToPrint, "********************((( TRAILER )))********************\n\0");

        //free
        free(headerString);
        free(otherFieldString);
        free(indiString);
        free(famString);
        free(subString);
    }//end if

    //return
    return stringToPrint;
}//end parser func

char* printError(GEDCOMerror err){
    //array for tags and enums etc
    static char* errorType[] = {
        "OK", "INV_FILE", "INV_GEDCOM",
        "INV_HEADER", "INV_RECORD", "OTHER"
    };//end array
    //print the error if there is error
    char* stringToPrint = calloc(1, sizeof(char)* (strlen(errorType[err.type])+99) );
    sprintf(stringToPrint, "Error code: %s\nError line: %d\n", errorType[err.type], err.line); 
    strcat(stringToPrint, "\0");
    //return the string
    return stringToPrint;
}//end func

void deleteGEDCOM(GEDCOMobject* obj){
    if(obj == NULL){
        return;
    }//end if
    deleteFamilyGarbageWrapper();
    deleteHeader(obj->header);
    clearList(&obj->families);
    clearList(&obj->individuals);
    free(obj);
}//end parser func*/

Individual* findPerson(const GEDCOMobject* familyRecord, bool (*compare)(const void* first, const void* second), const void* person){
    if(familyRecord == NULL || person == NULL){
        return NULL;
    }//end if

    Individual* persongBeingSearched = NULL;
    persongBeingSearched =  findElement(familyRecord->individuals, &comparePerson, person);
    return persongBeingSearched; 
}//end parser func

List getDescendants(const GEDCOMobject* familyRecord, const Individual* person){//dundo
    List listOfDescendant;
    //check if its is null and valid
    if(familyRecord == NULL || person == NULL){
        listOfDescendant = initializeList(printDescendants, dummyDelete, compareIndividuals);
        return listOfDescendant;
    }//end if
    listOfDescendant = initializeList(printDescendants, dummyDelete, compareIndividuals);

    //find the person and pass to the func to return the list
    Individual* searchPerson = findPerson(familyRecord, comparePerson, person);
    char* printIndi = printIndividual(searchPerson);
    if(DEBUG_UTILITIES)printf("print searchPerson: %s\n", printIndi);
    free(printIndi);
    if(searchPerson != NULL){
        listOfDescendant = getChildren(searchPerson);
    }else{
        if(DEBUG_PARSER)printf("searchPerson: NULL\n");
    }//end if 
    return listOfDescendant;
}//end parser func

/***********************************************************
 * list helper function functions
 ***********************************************************/

void deleteEvent(void* toBeDeleted){//checked
    //check if is null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete the event
    Event* object = (Event*)toBeDeleted;
    if(object->date != NULL ){
        free(object->date);
    }//end if
    if(object->place != NULL){
        free(object->place);
    }//end if
    clearList(&object->otherFields);
    free(object);
}//end func

int compareEvents(const void* first,const void* second){//checked
    //check if null
    if(first == NULL || second == NULL){
        return 0;
    }//end if
    /*static char* monthList[] = {
        "JAN", "FEB", "MAR", "APR",
        "MAY", "JUN", "JUL", "AUG",  
        "SEP", "OCT", "NOV", "DEC"
    };//end array*/
    if(DEBUG_PARSER)printf("comparingEvents invoked\n");

    //dec vars
    int result = 0;
    Event* object1 = (Event*)first;
    Event* object2 = (Event*)second;
    //check if there is a null
    if(object1->date == NULL || object2->date == NULL){
        return compareString(object1->type, object2->type);//type is mandatory
    }//end if
    return compareString(object1->type, object2->type);

    //dec vars
    int tokenSize1 = 0;
    char** token1 = split(object1->date, " ", &tokenSize1);
    int tokenSize2 = 0;
    char** token2 = split(object2->date, " ", &tokenSize2);

    //check the year
    if(tokenSize1 > 1 && tokenSize2 > 1){
        //if(DEBUG_PARSER)printf("comparing year %s = %s\n", token1[tokenSize1-1], token2[tokenSize2-1]);
        if(checkStringIfNumbers(token1[tokenSize1-1]) && checkStringIfNumbers(token2[tokenSize2-1])){
            if( atoi(token1[tokenSize1-1]) < atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return -1;
            }else if( atoi(token1[tokenSize1-1]) > atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return 1;
            }else if( atoi(token1[tokenSize1-1]) == atoi(token2[tokenSize2-1]) ){
                freeStringArray(token1, tokenSize1);
                freeStringArray(token2, tokenSize2);
                return strcmp(object1->type, object2->type);
            }//end if
        }//end if
    }//end if

    //check the month
    /*if(DEBUG_PARSER)printf("converting month %s = %s\n", token1[tokenSize1-2], token2[tokenSize2-2]);
    int month1 = searchStringArray(monthList, token1[tokenSize1-2], 12, "index");
    int month2 = searchStringArray(monthList, token2[tokenSize2-2], 12, "index");
    if(month1 < month2){
        freeStringArray(token1, tokenSize1);
        freeStringArray(token2, tokenSize2);
        return -1;
    }else if(month1 > month2){
        freeStringArray(token1, tokenSize1);
        freeStringArray(token2, tokenSize2);
        return 1;
    }//end if*/

    //check the date
    /*if(atoi(token1[tokenSize1-3]) < atoi(token2[tokenSize2-3])){
        freeStringArray(token1, tokenSize1);
        freeStringArray(token2, tokenSize2);
        return -1;
    }else if(atoi(token1[tokenSize1-3]) > atoi(token2[tokenSize2-3])){
        freeStringArray(token1, tokenSize1);
        freeStringArray(token2, tokenSize2);
        return 1;
    }//end if*/

    freeStringArray(token1, tokenSize1);
    freeStringArray(token2, tokenSize2);
    return result;
}//end func

char* printEvent(void* toBePrinted){//checked
    //check if null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //return
    Event* object = (Event*)toBePrinted;
    char* fieldString = toString(object->otherFields);
    char* stringToPrint = calloc(1, sizeof(char)* (99 + 99 + 256 + 256 + strlen(fieldString)));
    sprintf(stringToPrint, "|%s| |%s| |%s|\n", object->type, object->date, object->place);
    /*strcat(stringToPrint, "Other fields: ");
    strcat(stringToPrint, fieldString);
    strcat(stringToPrint, "\n\0");*/
    //free
    free(fieldString);
    return stringToPrint;
}//end func

/***********************************************************/

void deleteIndividual(void* toBeDeleted){//checked
    //check if null
    if(toBeDeleted == NULL){
        return;
    }//end if

    //delete indi
    Individual* object = (Individual*)toBeDeleted;
    if(object->givenName != NULL){
        free(object->givenName);
    }//end if
    if(object->surname != NULL){
        free(object->surname);
    }//end if
    clearList(&object->events);
    clearList(&object->families);
    clearList(&object->otherFields);
    free(object); 
}//end func

int compareIndividuals(const void* first,const void* second){//checked
    //check if its null
    if(first == NULL || second == NULL){
        return 0;
    }//end if
    
    //dec vars
    Individual* object1 = (Individual*)first;
    Individual* object2 = (Individual*)second;
    char individual1[256];
    char individual2[256];
    sprintf(individual1, "%s,%s", object1->surname, object1->givenName);
    sprintf(individual2, "%s,%s", object2->surname, object2->givenName);
    return compareString(individual1, individual2);
}//end func

char* printIndividual(void* toBePrinted){//checked
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
    strcat(stringToPrint, "\n\t---< Individual >---\nName: |");
    if(object->givenName != NULL){
        strcat(stringToPrint, object->givenName);
        strcat(stringToPrint, "| |");
    }//end if
    if(object->surname != NULL){
        strcat(stringToPrint, object->surname);
    }//end if
    strcat(stringToPrint, "|\nEvents: \n");
    strcat(stringToPrint, eventString);
    // strcat(stringToPrint, "Families: \n");
    // strcat(stringToPrint, famString);
    strcat(stringToPrint, "Other fields: \n");
    strcat(stringToPrint, fieldString);
    strcat(stringToPrint, "\0");

    /*free*/
    free(eventString);
    free(famString);
    free(fieldString);
    return stringToPrint;
}//end func

/***********************************************************/

void deleteFamily(void* toBeDeleted){//checked
    //check if null
    if(toBeDeleted == NULL){
        return;
    }//end if
    
    //delete family
    Family* object = (Family*)toBeDeleted;
    // if(object->wife != NULL){
    //     deleteIndividual(object->wife);
    // }//end if
    // if(object->husband != NULL){
    //     deleteIndividual(object->husband);
    // }//end if
    clearList(&object->events);
    clearList(&object->children);
    clearList(&object->otherFields);
    free(object);
}//end func

int compareFamilies(const void* first,const void* second){//checked
    //dec vars
    Family* object1 = (Family*)first;
    Family* object2 = (Family*)second;
    int countFamily1 = 0;
    int countFamily2 = 0;

    //count the number of indi in the family for the first one
    if(object1->wife != NULL){
        countFamily1++;
    }//end if
    if(object1->husband != NULL){
        countFamily1++;
    }//end if
    countFamily1 = countFamily1 + getLength(object1->children);

    //check the number of person in the second family
    if(object2->wife != NULL){
        countFamily2++;
    }//end if
    if(object2->husband != NULL){
        countFamily2++;
    }//end if
    countFamily2 = countFamily2 + getLength(object2->children);

    //return the result
    if(countFamily1 < countFamily2){
        return -1;
    }else if(countFamily1 > countFamily2){
        return 1;
    }else{
        return 0;//means it is the same
    }//end if
}//end func

char* printFamily(void* toBePrinted){//checked
    //check if its null
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //return
    Family* object = (Family*)toBePrinted;
    char* stringEvent = toString(object->events);
    char* stringChildren = toString(object->children);
    int mallocSize = strlen(stringChildren) + strlen(stringEvent) + 99;
    char stringHusband[256];
    char stringWife[256];
    char* stringToPrint = calloc(1, sizeof(char) * (mallocSize+500));
    //set the husband
    if(object->husband != NULL){
        sprintf(stringHusband, "|%s| |%s|", object->husband->givenName, object->husband->surname);
    }else{
        strcpy(stringHusband, "");
    }//end if
    //set the wife
    if(object->wife != NULL){
        sprintf(stringWife, "|%s| |%s|", object->wife->givenName, object->wife->surname);
    }else{
        strcpy(stringWife, "");
    }//end if
    strcat(stringToPrint, "\n\t---< Family >---\nHusband: ");
    strcat(stringToPrint, stringHusband);
    strcat(stringToPrint, "\nWife: ");
    strcat(stringToPrint, stringWife);
    strcat(stringToPrint, "\nEvent: \n");
    strcat(stringToPrint, stringEvent);
    strcat(stringToPrint, "Chilren: \n");
    strcat(stringToPrint, stringChildren);
    strcat(stringToPrint, "\0");
    /*free*/
    free(stringEvent);
    free(stringChildren);
    return stringToPrint;
}//end func

/***********************************************************/

void deleteField(void* toBeDeleted){//checked
    //check if its null 
    if(toBeDeleted == NULL){
        return;
    }//end if
    
    //delete field
    Field* object = (Field*)toBeDeleted;
    if(object->tag != NULL){
        free(object->tag);
    }//end if
    if(object->value != NULL){
        free(object->value);
    }//end if
    free(object);
}//end func

int compareFields(const void* first,const void* second){//checked
    //check if null
    if(first == NULL || second == NULL){
        return 0;
    }//end if

    Field* object1 = (Field*)first;
    Field* object2 = (Field*)second;
    char* field1 = calloc(1, sizeof(char)* (strlen(object1->tag) + strlen(object1->value) + 99) );
    char* field2 = calloc(1, sizeof(char)* (strlen(object2->tag) + strlen(object2->value) + 99) );
    //set the string
    sprintf(field1, "%s %s", object1->tag, object1->value);
    sprintf(field2, "%s %s", object2->tag, object2->value);
    //comapre the too string
    int compareResult = strcmp(field1, field2);
    free(field1);
    free(field2);
    if(compareResult < 0){
        return -1;
    }else if(compareResult > 0){
        return 1;
    }else{
        return 0;
    }//end if
}//end func

char* printField(void* toBePrinted){
    if(toBePrinted == NULL){
        return setString("");
    }//end if

    //cast the object
    Field* object = (Field*)toBePrinted;

    //check the if its empty
    if(object->value == NULL){
        object->value = setString("");
    }//end
    
    //return
    char* stringToPrint = calloc(1, sizeof(char)* (strlen(object->tag) + strlen(object->value) + 99) );
    sprintf(stringToPrint, "\t|%s| |%s|\n", object->tag, object->value);
    return stringToPrint;
}//end func