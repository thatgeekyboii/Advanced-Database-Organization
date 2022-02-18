# Advanced-Database-Organization
<h1>Storage Manager.</h1>

<hr>

<b>GROUP 17</b> <br>
<b>Team Members </b> <br>
1. Ramya Konkala  (A20497022).<br>
2. Vaibhav Patil  (A20492152).<br>
3. Saipavan Kunda (A20496516).<br>

<hr>

<h3>Aim: </h3>
The goal of this assignment is to implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE SIZE). In addition to reading and writing pages from a file,we have also implemented functions to CREATE,OPEN,CLOSE,READ,WRITE and APPEND pages in the file.

<hr>

<h3>Project Files</h3>

<b>C Source Files </b> </br> 
dberror.c <br>
storage_mgr.c <br>
test_assign.c <br>
<br>
<b>Header Files </b> </br> 
dberror.h <br>
storage_mgr.h <br>
test_helper.h <br>
<br>
<b>Run File</b><br>
Makefile <br>

<hr>


<h3>Instructions to run the code.</h3>
Step 1-  $ make <br>
Step 2-  $ ./test_assign1

Use clean command to delete all object files.
$ make clean <br>
<hr>

<h3>Functions Used. </h3>

<b>Manipulating Page File.</b><br>
<br>
<b>createPageFile():</b><br>
      - Creates a new page file of 1 page(PAGE_SIZE) and fills it with '\0' bytes.<br>
      - In this function we are using fopen() function to create the file.<br>
      - The file is opened in the write mode so that we can fill it with '\0' bytes.<br>

<b>openPageFile():</b><br>
      - Checks if the input file is present.<br>
      - If file is present then opens the existing file or else throws an error RC_FILE_NOT_FOUND.<br>
      - We are calculating the total number of pages by pointing the file pointer to end and then dividing it by the page size.<br> 
      - After that we are updating the file handle with file name, total number of pages and current page position.<br>
      
<b>closePageFile():</b><br>
      - Closes the already opened page file.<br>
      - Returns RC_OK if file closes successfully else returns RC_FILE_NOT_FOUND.<br>

<b>destroyPageFile():</b><br>
      - Destroys already created page file.<br>
      - Using remove() function to destroy the file.<br>
      - Returns RC_OK if file destroyed successfully else returns RC_FILE_NOT_FOUND.<br>
      
<hr>

<b>Read Functions.</b><br>

<b>readBlock():</b><br>
      - Checks if the pageNum is greater than the total number of pages.<br>
      - If greater then throws an error.<br>
      - Else  will set the pointer position to the page which we have to read and read the block.<br>
      - Set the current page position to the page number we read.<br>

<b>getBlockPos():</b> <br>
      - Return current page position using fHandle.<br>
      
<b>readFirstBlock():</b> <br>
      - Using the readBloack method by assigning pageNum as 0 indicating the FirstBlock. <br>
      
<b>readPreviousBlock():</b><br>
      - Using the readBloack method by assigning pageNum as curPagePos - 1 indicating the PreviousBlock. <br>
      
<b>readCurrentBlock():</b><br>
      - Using the readBloack method by assigning pageNum as curPagePos indicating the CurrentBlock.<br>
      
<b>readNextBlock():</b> <br>
      - Using the readBloack method by assigning pageNum as curPagePos + 1 indicating the NextBlock<br>
      
<b>readLastBlock():</b> <br>
      - Using the readBloack method by assigning pageNum as totalNumPages - 1 indicating the LastBlock.<br>

<hr>

<b> Write Function </b> <br>

<b>writeBlock():</b><br>
      - Creating a fp pointer to access the file.<br>
      - Moving the pointer to the beginning of the page specified by pageNum.fseek().<br>
      - Writing to the file.<br>
      - Setting the pointer location to the end of the page.<br>
      - Populating the fHandle object with the updated values.<br>
      
      
<b>writeCurrentBlock():</b><br>
      - Get the current page position.<br>
      - Call the writeBlock() function by passing the current position in its arguments.<br>
      - If current block is successfully written,return RC_OK.<br>
      - Else return RC_WRITE_FAILED.<br>
      
<b>appendEmptyBlock():</b><br>
      - Get the total page numbers, and here is where we will write the empty block.<br>
      - Open the file using r+ so we can append file.<br>
      - Moving the pointer to the start of the page given by pageNum.fseek() <br>
      - Total number of pages increased as new page added at the end.<br>
      


<b>ensureCapacity():</b> </br>
      - Updating the capacity value by comparing totalNumPages with numberOfPages.


