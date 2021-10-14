#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{
	int ret;

	/* 
	 * Check for file existance
	 */

	if (ret == 0)
	{
		/* 
		 * Do the neccessary step to open the file
		 * Do error handling
		 */ 
	}
	else
	{
		/* Create a file for adding entries */
	}

	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	//ContactInfo* tempListPtr = address_book->list;
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

	/* 
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 

	fclose(address_book->fp);

	return e_success;
}
