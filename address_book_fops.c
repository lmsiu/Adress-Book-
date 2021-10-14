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