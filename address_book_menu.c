#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "abk_fileops.h"
// #include "abk_log.h"
// #include "abk_menus.h"
// #include "abk.h"
#include "address_book.h"
#include "address_book_menu.h"

int get_option(int type, const char *msg)
{
	printf("%s", msg);
   switch (type)
   {
      case NUM:
      {
         int opt;
         scanf("%d", &opt);
         return opt;
      }
      case CHAR:
      {
         char opt;
         scanf("%c", &opt);
         return opt;
      }
      case NONE:
         return e_no_opt;
   }
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (*str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	struct ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	menu_header("Search Result:\n");

   //Print column names
   for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++)
      printf("=");
   printf("\n: S.No : Name");
   for (int spaces = 0; spaces < 33 - 5; spaces++)
      printf(" ");
   printf(": Phone No");
   for (int spaces = 0; spaces < 33 - 9; spaces++)
      printf(" ");
   printf(": Email ID                     :\n");
   for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++)
      printf("=");

   //Get start and end point for search
   struct ContactInfo * ptrToPeople = address_book->list;
   struct ContactInfo * endPtr = ptrToPeople + address_book->count;
   unsigned int foundPeople = 0;

   for (ptrToPeople; ptrToPeople < endPtr; ptrToPeople++)
   {
      if (compareFields(field, str, ptrToPeople) == 0)
      {
         foundPeople++;
         printf("\n: %d", ptrToPeople->si_no);

         //A little thing to keep the format nice
         short lengthOfSerial = 2; //Includes the one digit and one space
         int serial = ptrToPeople->si_no;
         while (serial > 10)
         {
            serial /= 10;
            lengthOfSerial++;
         }
         for (int spaces = 0; spaces < 6 - lengthOfSerial; spaces++)
            printf(" ");

         //Print name
         printf(": %s", ptrToPeople.name);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople.name); spaces++)
            printf(" ");
         
         //Print phone number 1
         printf(": %s", ptrToPeople.phone_numbers[0]);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople.phone_numbers[0]); spaces++)
            printf(" ");

         //Print email 1
         printf(": %s", ptrToPeople.email_addresses[0]);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople.email_addresses[0]); spaces++)
            printf(" ");
         printf(":\n");

         //Print remaining emails and phone numbers
         for (int infoLine = 1; infoLine < PHONE_NUMBER_COUNT; infoLine++) //Condition will need changing if PHONE_NUMBER_COUNT is ever not equal to EMAIL_ID_COUNT
         {
            printf(":      :                                 : %s", ptrToPeople.phone_numbers[infoLine]); // Will need changing if contacts can ever have multiple names/serial numbers
            for (int spaces = 0; spaces < 32 - strlen(ptrToPeople.phone_numbers[infoLine]); spaces++)
               printf(" ");
            printf(": %s", ptrToPeople.email_addresses[infoLine]);
            for (int spaces = 0; spaces < 32 - strlen(ptrToPeople.email_addresses[infoLine]); spaces++)
               printf(" ");
            printf(":\n");
         }
         for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++)
            printf("=");
      }
   }
   printf("%s", msg);
   if (foundPeople > 0)
      return e_success;
   else if (foundPeople < 0)
      return e_fail;
   return e_no_match;
}

//Allows for an easy comparison in search()
static int compareFields(int field, const char * toCheck, struct ContactInfo * contact)
{
   switch (field)
   {
      case NAME:
         return strcmp(toCheck, contact->name[0]);
      case NUMBER:
         for (int phone = 0; phone < PHONE_NUMBER_COUNT; phone++)
         {
            if (strcmp(toCheck, contact->phone_numbers[phone]) == 0)
               return 0;
         }
         return 1;
      case EMAIL:
         for (int email = 0; email < EMAIL_ID_COUNT; email++)
         {
            if (strcmp(toCheck, contact->email_addresses[email]) == 0)
               return 0;
         }
         return 1;
      case SERIAL:
         return atoi(toCheck) != contact->si_no;
   }
   return -1;
}

Status search_contact(AddressBook *address_book)
{
   char * quitMsg = "Press: [q] | Cancel: ";
   Status endStat;
   //Create a search menu
	menu_header("Search contact by:\n");
   printf("0. Back\n");
   printf("1. Name\n");
   printf("2. Phone No\n");
   printf("3. Email ID\n");
   printf("4. Serial No\n\n");

   //Get user's choice
   int searchOption = get_option(NUM, "Please select an option: ");

   //React to user choice
   if (searchOption == 0)
      return e_back;
   else if (searchOption == NAME)
   {
      printf("Enter the Name: ");
      char * name;
      scanf("%s", &name);
      endStat = search(name, address_book, 0, NAME, quitMsg, e_search);
   }
   else if (searchOption == NUMBER)
   {
      printf("Enter the Phone Number: ");
      char * number;
      scanf("%s", number);
      endStat = search(number, address_book, 0, NUMBER, quitMsg, e_search);
   }
   else if (searchOption == EMAIL)
   {
      printf("Enter the Email ID: ");
      char * email;
      scanf("%s", email);
      endStat = search(email, address_book, 0, EMAIL, quitMsg, e_search);
   }
   else if (searchOption == SERIAL)
   {
      printf("Enter the Serial Number: ");
      char * sno;
      scanf("%s", &sno);
      endStat = search(sno, address_book, 0, SERIAL, quitMsg, e_search);
   }
   else
      endStat = e_fail;
   char opt = get_option(CHAR, "");
   if (opt == 'q')
      return endStat;
}

Status edit_contact(AddressBook *address_book)
{
		/* Add the functionality for edit contacts here */
   char stringToChange[32] = "";
   int sino;
   char yn;

   struct ContactInfo * contactToEdit;
   int indexToChange;

   struct ContactInfo ci; //tester

   char * quitMsg = "Press: [q] | Cancel: ";

   printf("Which contact would you like to edit?\n");
   search_contact(address_book); //uses search contact to search and print contact

   printf("Please enter the serial number of the contact\n");
   scanf("%d", sino);

   // contactToEdit= getContactAddress(address_book, sino);
   //need to have this be a pointer to a ContactInfo
   contactToEdit = &ci;

   menu_header("Edit contact by:\n");
   printf("0. Back\n");
   printf("1. Name\n");
   printf("2. Phone No\n");
   printf("3. Email ID\n");

   //Get user's choice
   int searchOption = get_option(NUM, "Please select an option: ");

   //React to user choice
   if (searchOption == 0)
      return e_back;
   else if (searchOption == NAME)
   {
      printf("What would you like to change the name to?\n ");
      scanf("%s", &stringToChange);
      strcpy(contactToEdit->name[0], stringToChange);
      printf("New name: %s", contactToEdit->name[0]);

   } else if (searchOption == NUMBER) {

      printf("Would you like to change a phone number? (y/n)\n");
      scanf("%c", &yn);

      while(yn == 'y' || yn =='Y'){
      printf("Which phone number would you like to change?\n");
      for(int i = 0; i<PHONE_NUMBER_COUNT; i++){
         printf("Name: %s, Phone number %d: %s\n", contactToEdit->name, i+1, contactToEdit->phone_numbers[i]);
      }
      scanf("%d", &indexToChange); //input will be 1 greater than the index

      printf("What would you like to change the phone number to?\n");
      scanf("%s", stringToChange);

      strcpy(contactToEdit->phone_numbers[indexToChange - 1], stringToChange);

      printf("Updated phone numbers: \n");
      for(int i = 0; i<PHONE_NUMBER_COUNT; i++){
         printf("Name: %s, Phone number %d: %s\n", contactToEdit->name, i+1, contactToEdit->phone_numbers[i]);
      }

      printf("New string %s\n", stringToChange);

      printf("Would you like to change another phone number? (y/n)\n");

      getchar();
      scanf("%c%*c", &yn);

      }

   } else if (searchOption == EMAIL) {
      printf("Would you like to change an email address? (y/n)\n");
      scanf("%c", &yn);

      while(yn == 'y' || yn =='Y'){
      printf("Which email address would you like to change?\n");
      for(int i = 0; i<EMAIL_ID_COUNT; i++){
         printf("Name: %s, Email address %d: %s\n", contactToEdit->name[0], i+1, contactToEdit->email_addresses[i]);
      }
      scanf("%d", &indexToChange); //input will be 1 greater than the index

      printf("What would you like to change the email address to?\n");
      scanf("%s", stringToChange);

      strcpy(contactToEdit->email_addresses[indexToChange - 1], stringToChange);

      printf("Updated phone numbers: \n");
      for(int i = 0; i<EMAIL_ID_COUNT; i++){
         printf("Name: %s, Email address %d: %s\n", contactToEdit->name[0], i+1, contactToEdit->email_addresses[i]);
      }

      printf("New string %s\n", stringToChange);

      printf("Would you like to change another email address? (y/n)\n");

      getchar();
      scanf("%c%*c", &yn);
      }

   }
   else{
      return e_fail;
   }

   char opt = get_option(CHAR, "");

   if (opt == 'q'){
      return e_back;
   }

   return e_success;


}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}

//Gets the pointer to a contact, useful for deleting and editing
struct ContactInfo * getContactAddress(AddressBook *addBook, int sno)
{
   struct ContactInfo * ptr = addBook->list;
   struct ContactInfo * endPtr = addBook->list + addBook->count;
   for (; ptr < endPtr; ptr++)
   {
      if (ptr.si_no == sno)
         return ptr;
   }
   return NULL;
}