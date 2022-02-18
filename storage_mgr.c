#include "storage_mgr.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include "test_helper.h"


void initStorageManager (void){
    //constructor
}

/* manipulating page files */
//Creates Page File and writes the entire file with '\0' bytes
RC createPageFile (char *fileName){

    FILE *fp;
    unsigned char buffer = '\0';

    fp = fopen(fileName, "w+");
    if(fp != NULL)
    {
        for (int i = 0; i < PAGE_SIZE; i++)
            fputc(buffer, fp);
        fclose(fp);
        return RC_OK;
    }

	return RC_FILE_NOT_FOUND;
}

//
RC openPageFile (char *fileName, SM_FileHandle *fHandle){

    FILE *fp = fopen(fileName, "r"); // file pointer to access the file.
    if(fp){ 
        struct stat fSt;
        stat(fileName, &fSt); // stat structure gets the statistics of file like file size.
        size_t fileSize = fSt.st_size;

        //Intialising the file handle object with file details.
        fHandle->fileName = fileName;
        fHandle->curPagePos = 0;
        fHandle->totalNumPages = fileSize / PAGE_SIZE;
        fHandle->mgmtInfo = fp; 

        return RC_OK;
    }    
    return RC_FILE_NOT_FOUND;
}

RC closePageFile (SM_FileHandle *fHandle)
{
    if(fHandle == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    //Closing the file with the exsiting pointer info in fHandle object.
    int closeStatus = fclose(fHandle->mgmtInfo);

    if(closeStatus != 0){
        return RC_FILE_NOT_FOUND;
    }
    return RC_OK;
}

RC destroyPageFile (char *fileName)
{
    //remove method is used to completely delete the file.
    if(remove(fileName) == 0){
        return RC_OK;
    }
    return RC_FILE_NOT_FOUND;
}

/* reading blocks from disc */

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if (fHandle == NULL) {
		return RC_FILE_HANDLE_NOT_INIT;
    }

    if(pageNum > fHandle->totalNumPages || pageNum < 0) //checking valid page number
    {
		return RC_READ_NON_EXISTING_PAGE;
    }

	if(fopen(fHandle->fileName,"r") != NULL) {

		fseek(fHandle->mgmtInfo , pageNum*PAGE_SIZE , SEEK_SET);//seeking to a particular block with respect to the page number

        fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo); //reading the entire file from the existing pointer location in fhandle object.

        fHandle->curPagePos = pageNum;

        return RC_OK;
    }
	return RC_FILE_NOT_FOUND;
}

int getBlockPos (SM_FileHandle *fHandle)
{
	if (fHandle == NULL)
		return RC_FILE_HANDLE_NOT_INIT;

	if(fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;

	return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(0, fHandle, memPage); // Using the readBloack method by assigning pageNum as 0 indicating the FirstBlock
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->curPagePos - 1, fHandle, memPage); // Using the readBloack method by assigning pageNum as curPagePos - 1 indicating the PreviousBlock
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->curPagePos, fHandle, memPage); // Using the readBloack method by assigning pageNum as curPagePos indicating the CurrentBlock
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->curPagePos + 1, fHandle, memPage); // Using the readBloack method by assigning pageNum as curPagePos + 1 indicating the NextBlock
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->totalNumPages - 1, fHandle, memPage); // Using the readBloack method by assigning pageNum as totalNumPages - 1 indicating the LastBlock
}

/* writing blocks to a page file */

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    //Checking all the validations 
    if (fHandle == NULL)
		return RC_FILE_HANDLE_NOT_INIT;

	if(fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;

	if(pageNum >= fHandle->totalNumPages || pageNum < 0)
        return RC_WRITE_FAILED;

	FILE *fp;
	fp = fopen(fHandle->fileName,"r+"); // pointer to access the file.
	fseek(fp , pageNum*PAGE_SIZE , SEEK_SET);//Moving the pointer to the beginning of the page specified by pageNum.fseek()

	if(fwrite(memPage,PAGE_SIZE,1,fp) != 1) // writing to the file.
		return RC_WRITE_FAILED;
	
	fp = fopen(fHandle->fileName,"r");

	fseek(fp, (pageNum+1)*PAGE_SIZE , SEEK_SET); //setting the pointer location to the end of the page.
    fclose(fp);

    //Populating the fHandle object with the updated values.
	fHandle->mgmtInfo = fp;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE);
	fHandle->totalNumPages = (ftell(fp)/PAGE_SIZE);

	return RC_OK;   
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
   return writeBlock(fHandle->curPagePos, fHandle, memPage);
}

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    int pageNum = fHandle->totalNumPages; //Get the total page numbers, and here is where we will write the empty block.
    
    FILE *fp;
    fp = fopen(fHandle->fileName, "r+"); //Open the file using r+ so we can append file.
    if(fp != NULL){
        fseek(fp, (pageNum * PAGE_SIZE), SEEK_END); //Moving the pointer to the start of the page given by pageNum.fseek() 
      
        for (int i = 0; i < PAGE_SIZE; i++)
            fputc('\0', fp);
        fclose(fp);

        fHandle->totalNumPages = fHandle -> totalNumPages + 1; //Total number of pages increased as new page added at the end.
        fclose(fp); 
        return RC_OK;
    }

    return RC_FILE_NOT_FOUND;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
    //Updating the capacity value by comparing totalNumPages with numberOfPages
    if(fHandle->totalNumPages < numberOfPages)
        fHandle->totalNumPages = numberOfPages;
    return RC_OK;
}