#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	//Max possible length for a contact, including commas
   unsigned int len = (NAME_COUNT * NAME_LEN) + (PHONE_NUMBER_COUNT * NUMBER_LEN) + (EMAIL_ID_COUNT * EMAIL_ID_LEN) + (EMAIL_ID_COUNT + PHONE_NUMBER_COUNT + NAME_COUNT);
   char contactBuff[len]; 

   //Try opening the file for reading
   FILE * fp = fopen(DEFAULT_FILE, "a+");

	if (!fp) //Make sure the pointer is not null
	{
      address_book->fp = fp;
      address_book->list = malloc(0);
      if (!address_book->list)
      {
         printf("Not able to allocate any memory.");
         return e_fail;
      }
      ContactInfo * currPerson = address_book->list;
      unsigned int count = 0;
      while (EOF != fscanf(fp, "%s", contactBuff)) //Input until end of file
      {
         ContactInfo * tempPtr = realloc(address_book->list, (count + 1) * sizeof(ContactInfo));
         if (!tempPtr)
         {
            printf("Not able to allocate more memory. %i people loaded.", count);
            return e_fail;
         }
         count++;
         address_book->list = tempPtr;                //Reassign list to new people
         currPerson = address_book->list + count - 1; //Start a new person
         unsigned int currChar = 0;                   //Read through the string one character at a time
         for (int currName = 0; currName < NAME_COUNT; currName++)
         { 
            while (contactBuff[currChar] != ',')         //Read name
            {
               currPerson->name[currName][currChar] = contactBuff[currChar];
               currChar++;
            }
            currChar++; //Move past comma
         }
         for (int phone = 0; phone < PHONE_NUMBER_COUNT; phone++) //Read each phone number
         {
            while (contactBuff[currChar] != ',')
            {
               currPerson->phone_numbers[phone][currChar] = contactBuff[currChar];
               currChar++;
            }
            currChar++;
         }
         for (int email = 0; email < EMAIL_ID_COUNT; email++)  //Read each email
         {
            while (contactBuff[currChar] != ',')
            {
               currPerson->email_addresses[email][currChar] = contactBuff[currChar];
               currChar++;
            }
            currChar++;
         }
         currPerson->si_no = count;  //Assign a serial number
         address_book->count = count; //Update the count
      }
	}
	/*else  //No file found
	{
      fp = fopen(DEFAULT_FILE, "w"); //Create a file
      address_book->count = 0;
      address_book->fp = fp;
	}*/
   address_book->fp = fp;
   fclose(fp);
	return e_success;
}

Status save_file(AddressBook *address_book)
{

	//open DEFAULT_FILE with the "w" file permission
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	//print name, phone numbers, email addresses, and serial ID's of all contacts to DEFAULT_FILE
	//each line = 1 contact
	//commas separate data fields
	//empty data is saved as "", as initialized in the add_contacts() function in the address_book_menu.c file
	for(int contactNum = 0; contactNum < address_book->count; contactNum++){
		fprintf(address_book->fp, "%s,",address_book->list[contactNum].name[0]);
		for(int phoneNum = 0; phoneNum < PHONE_NUMBER_COUNT; phoneNum++){
			fprintf(address_book->fp,"%s,",address_book->list[contactNum].phone_numbers[phoneNum]);
		}
		for(int emailCount = 0; emailCount < EMAIL_ID_COUNT; emailCount++){
			fprintf(address_book->fp, "%s,",address_book->list[contactNum].email_addresses[emailCount]);
		}
		fprintf(address_book->fp, "%d\n", address_book->list[contactNum].si_no);
	}

	//close file
	fclose(address_book->fp);

	return e_success;
}
