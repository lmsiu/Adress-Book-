#include <stdio.h>
// #include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <Windows.h>
#define sleep Sleep
#define clear "cls"
#else
#include <unistd.h>
#define clear "clear"
#endif

// #include "abk_fileops.h"
// #include "abk_log.h"
// #include "abk_menus.h"
// #include "abk.h"
#include "address_book.h"
#include "address_book_menu.h"
#include "address_book_fops.h"

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

   ContactInfo * contactInfoPtr = address_book->list;
   ContactInfo * endPtr = contactInfoPtr + address_book->count;

   //allows for quiting later
   char quit;

    //print collumns
    for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++)
      printf("=");
   printf("\n: S.No : Name");
   for (int spaces = 0; spaces < 33 - 5; spaces++){
      printf(" ");
   }
   printf(": Phone No");
   for (int spaces = 0; spaces < 33 - 9; spaces++){
      printf(" ");
   }
   printf(": Email ID                     :\n");
   for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++){
      printf("=");
    }
    printf("\n");

    //print stuff
    for(contactInfoPtr; contactInfoPtr < endPtr; contactInfoPtr++){
        //print name and si_no
        int sino = contactInfoPtr->si_no;

        // printf("Name: %s\n", address_book->list[0].name[0]);
        printf(": %2d   : %s", sino, contactInfoPtr->name[0]);
        for(int spaces = 0; spaces < 32 - strlen(contactInfoPtr->name[0]); spaces++){
            printf(" ");
        }

        printf(": %s", contactInfoPtr->phone_numbers[0]);

        for(int spaces = 0; spaces < 32 - strlen(contactInfoPtr->phone_numbers[0]); spaces++){
            printf(" ");
        }

        printf(": %s", contactInfoPtr->email_addresses[0]);

        printf("\n");

         //print phone numbers and emails
        for(int i = 1; i < PHONE_NUMBER_COUNT; i++){
           for(int spaces = 0; spaces < 41; spaces++){
              printf(" ");
           }

           printf(": %s", contactInfoPtr->phone_numbers[i]);

           for(int spaces = 0; spaces < 32 - strlen(contactInfoPtr->phone_numbers[i]); spaces++){
               printf(" ");
            }

            printf(": %s", contactInfoPtr->email_addresses[i]);

            printf("\n");
        }

        for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++){
         printf("=");
        }

        printf("\n");
    }

    //allows users to see list until they hit 'q'
   
   while (quit != 'q')
   {
      printf("Enter 'q' when done\n");
      getchar();
      scanf("%c", &quit);
   }


	return e_success;
}

void menu_header(const char *str)
{
	//fflush(stdout);

	system(clear);

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
	ContactInfo backup;
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
				add_contacts(address_book);
				break;
			case e_search_contact:
				search_contact(address_book, e_search);
            printf("After search\n");
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
            list_contacts(address_book, "Contacts", 0, "Press: [q] | Cancel", e_list);
				break;
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

int add_menu(ContactInfo* newContact, int phoneNumCount, int emailCount){
   menu_header("Add Contact:\n");
   printf("0. Back\n");
   printf("1. Name: %s\n", newContact->name[0]);
   printf("2. Phone Number 1: %s\n", newContact->phone_numbers[0]);
   for(int i = 1; i<=phoneNumCount-1; i++){
     printf("   Phone Number %i: %s\n", i+1, newContact->phone_numbers[i]);
   }
   printf("3. Email Address 1: %s\n", newContact->email_addresses[0]);
   for(int k = 1; k<=emailCount-1; k++){
      printf("   Email Address %i: %s\n", k+1, newContact->email_addresses[k]);
   }
   return 0;
}

Status add_contacts(AddressBook *address_book){
   ContactInfo newContact; //can change to ptr but would need to change ptrs to vars
   int contactCreated = 0;
   char userName[NAME_LEN];
   int phoneNumCount = 0;
   char tempPhoneNum[NUMBER_LEN];
   int emailCount = 0;
   char tempEmail[EMAIL_ID_LEN];
   int searchOption;

   //initialize all contact data (name, phone numbers, and email addresses) as ""
   strcpy(newContact.name[0], "");
   for(int p = 0; p < PHONE_NUMBER_COUNT; p++){
      strcpy(newContact.phone_numbers[p],"");
   }
   for(int e = 0; e < EMAIL_ID_COUNT; e++){
      strcpy(newContact.email_addresses[e],"");
   }

   //display add menu for user
   menu:
   add_menu(&newContact, phoneNumCount, emailCount);

   //obtain user input
   searchOption = get_option(NUM, "Please select an option: ");

   //act based on user input
   if(searchOption == 0){
      goto quit;
   } else if(searchOption == NAME){
      printf("Please input the name of your new contact: ");
      scanf("%s", userName);
      strcpy(newContact.name[0], userName);
      contactCreated = 1;
   } else if(searchOption == NUMBER){
      phoneNumCount++;
      if(phoneNumCount > 5){
         printf("Error! You may only input up to 5 phone numbers for your new contact. \n");
         phoneNumCount--;
         sleep(1);
      }else{
         printf("Please enter phone number %i (without hyphens or parentheses): ", phoneNumCount);
         scanf("%s", tempPhoneNum);
         strcpy(newContact.phone_numbers[phoneNumCount-1], tempPhoneNum);
      }
   } else if(searchOption ==  EMAIL){
      emailCount++;
      if(emailCount > 5){
         printf("Error! You may only input up to 5 email addresses for your new contact. \n");
         emailCount--;
         sleep(1);
      }else{
         printf("Please enter email address %i: ", emailCount);
         scanf("%s", tempEmail);
         strcpy(newContact.email_addresses[emailCount-1], tempEmail);
      }
   } else{
      printf("Error! Please select a number from 0 to 3.\n");
      sleep(1);
   }
   goto menu;

   quit:

   //if no contact was created (contact does not have a name), do not add newContact struct to list variable
   if(contactCreated == 0){
      printf("No contact was created.\n");
      return e_back;
   }

   //increase count by 1
   address_book -> count += 1;
   
   //Assign serial id based on address_book->count variable
   newContact.si_no = address_book->count;

   ContactInfo* tempPtr = realloc(address_book->list, (address_book->count)*sizeof(ContactInfo));
   if(tempPtr == NULL){
      printf("Memory reallocation failed! Please try to add contact again later.\n");
      return e_fail;
   }
   address_book->list = tempPtr;
   //printf("List size after realloc(): %zu\n", malloc_size(address_book->list));

   address_book->list[(address_book->count)-1] = newContact;

   return e_success;
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
   ContactInfo * ptrToPeople = address_book->list;
   ContactInfo * endPtr = ptrToPeople + address_book->count;
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
         printf(": %s", ptrToPeople->name);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople->name[0]); spaces++)
            printf(" ");
         
         //Print phone number 1
         printf(": %s", ptrToPeople->phone_numbers[0]);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople->phone_numbers[0]); spaces++)
            printf(" ");

         //Print email 1
         printf(": %s", ptrToPeople->email_addresses[0]);
         for (int spaces = 0; spaces < 32 - strlen(ptrToPeople->email_addresses[0]); spaces++)
            printf(" ");
         printf(":\n");

         //Print remaining emails and phone numbers
         for (int infoLine = 1; infoLine < PHONE_NUMBER_COUNT; infoLine++) //Condition will need changing if PHONE_NUMBER_COUNT is ever not equal to EMAIL_ID_COUNT
         {
            printf(":      :                                 : %s", ptrToPeople->phone_numbers[infoLine]); // Will need changing if contacts can ever have multiple names/serial numbers
            for (int spaces = 0; spaces < 32 - strlen(ptrToPeople->phone_numbers[infoLine]); spaces++)
               printf(" ");
            printf(": %s", ptrToPeople->email_addresses[infoLine]);
            for (int spaces = 0; spaces < 32 - strlen(ptrToPeople->email_addresses[infoLine]); spaces++)
               printf(" ");
            printf(":\n");
         }
         for (int numOfEquals = 0; numOfEquals < 32 * 3 + 14; numOfEquals++)
            printf("=");
      }
   }
   printf("Right before msg\n");
   printf("%s", msg);
   printf("Right after msg\n");
   sleep(2);
   if (foundPeople > 0){
      printf("e_success\n");
      return e_success;
   }else if (foundPeople < 0){
      printf("e_fail\n");
      return e_fail;
   }
   printf("e_no_match\n");
   return e_no_match;
}

//Allows for an easy comparison in search()
int compareFields(int field, const char * toCheck, ContactInfo * contact)
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

Status search_contact(AddressBook *address_book, Modes comingFrom)
{
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
      char name[NAME_LEN];
      scanf("%s", name);
      endStat = search(name, address_book, 0, NAME, "", comingFrom);
   }
   else if (searchOption == NUMBER)
   {
      printf("Enter the Phone Number: ");
      char number[NUMBER_LEN];
      scanf("%s", &number);
      endStat = search(number, address_book, 0, NUMBER, "", comingFrom);
   }
   else if (searchOption == EMAIL)
   {
      printf("Enter the Email ID: ");
      char email[EMAIL_ID_LEN];
      scanf("%s", email);
      endStat = search(email, address_book, 0, EMAIL, "", comingFrom);
   }
   else if (searchOption == SERIAL)
   {
      printf("Enter the Serial Number: ");
      char sno[10];
      scanf("%s", &sno);
      endStat = search(sno, address_book, 0, SERIAL, "", comingFrom);
   }
   else
      endStat = e_fail;

   printf("Before char opt\n");
   sleep(2);
   fflush(stdin);
   char opt = get_option(CHAR, "Enter q to quit\n");
   printf("After char opt\n");
   sleep(2);
   if (opt == 'q'){
      printf("opt = q\n");
      printf("Endstat: %d\n", endStat);
      return endStat;
   }
   return e_back;
}

Status edit_contact(AddressBook *address_book)
{
   char stringToChange[32] = "";
   int sino;
   char yn;

   ContactInfo * contactToEdit;
   int indexToChange;

   char * quitMsg = "Press: [q] | Cancel: ";

   printf("Which contact would you like to edit?\n");
   Status check = search_contact(address_book, e_edit); //uses search contact to search and print contact
   //check back
   if(check == e_back){
      return e_back;
   }


   printf("Please enter the serial number of the contact\n");
   scanf("%d", &sino);

   contactToEdit= getContactAddress(address_book, sino);

   menu_header("Edit contact by:\n");
   printf("0. Back\n");
   printf("1. Name\n");
   printf("2. Phone No\n");
   printf("3. Email ID\n");

   //Get user's choice
   int searchOption = get_option(NUM, "Please select an option: ");

   //React to user choice
   if (searchOption == 0){
      return e_back;
   }else if (searchOption == NAME)
   {
      printf("What would you like to change the name to?\n ");
      scanf("%s", &stringToChange);
      strcpy(contactToEdit->name[0], stringToChange);
      // printf("New name: %s", contactToEdit->name[0]);

   } else if (searchOption == NUMBER) {

      printf("Would you like to change a phone number? (y/n)\n");
      getchar();
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

      // printf("New string %s\n", stringToChange);

      printf("Would you like to change another phone number? (y/n)\n");

      getchar();
      scanf("%c%*c", &yn);

      }

   } else if (searchOption == EMAIL) {
      printf("Would you like to change an email address? (y/n)\n");
      getchar();
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

      printf("Updated email adresses: \n");
      for(int i = 0; i<EMAIL_ID_COUNT; i++){
         printf("Name: %s, Email address %d: %s\n", contactToEdit->name[0], i+1, contactToEdit->email_addresses[i]);
      }

      // printf("New string %s\n", stringToChange);

      printf("Would you like to change another email address? (y/n)\n");

      getchar();
      scanf("%c", &yn);
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
   int userChoice;
   char userOpt;
   char userOpt2;
   int chosenSNO;
   int finalSNO;
   Status endStat2;
   char * Msg = "";

   start:
   /*resets values for when user come back to the start*/
   userChoice = NULL;
   userOpt = NULL;
   userOpt2 = NULL;

   /* Prints to console options*/
   printf("Search Contact to Delete by:\n");
   printf("0. Back\n");
   printf("1. Name\n");
   printf("2. Phone No\n");
   printf("3. Email ID\n");
   printf("4. Serial No\n\n");
   redo:

   userChoice = get_option(NUM, "Please select an option: ");
   /*Switch used to decide who to look up and how using users input*/
   switch(userChoice){
      case NONE:
      goto end;   //ends function returning to main menu
      break;
      case NAME :
         /* searches by name */
         printf("Enter the Name: ");
         char * name;
         scanf("%s", &name);
         endStat2 = search(name, address_book, 0, NAME, Msg, e_search);
         break;
      case NUMBER :
         /* searches by number */
         printf("Enter the Phone Number: ");
         char * number;
         scanf("%s", number);
         endStat2 = search(number, address_book, 0, NUMBER, Msg, e_search);
         break;
      case EMAIL :
         /* searches by email */
         printf("Enter the Email ID: ");
         char * email;
         scanf("%s", email);
         endStat2 = search(email, address_book, 0, EMAIL, Msg, e_search);
         break;
      case SERIAL :
         /* searches by serial number */
         printf("Enter the Serial Number: ");
         char * sno;
         scanf("%s", &sno);
         endStat2 = search(sno, address_book, 0, SERIAL, Msg, e_search);
         break;
      default :
         /* didn't choose a given option */
         printf("Please choose a valid option. \n");
         goto redo; //reattempts to ask user for a valid option
   } 

   /*Switch used to decide whether to go back to the very start, continue with deletion, or prompt user for a valid option*/
   while (userOpt != 's' && userOpt != 'q')
   {
      fflush(stdin);
      userOpt = get_option(CHAR, "\nPress: [s] = Select. [q] | Cancel:\n");
   switch(userOpt) {
      case 'q' :
         printf("Returning to delete menu\n");
         goto start; //goes back to very start of function
         break;
      case 's' :
         chosenSNO = get_option(NUM, "Select a Serial Number (S.No) to Delete: ");
         break;
      default:
         printf("Please choose a valid option.\n");
      } 
   }
   fflush(stdin);

   userOpt2 = get_option(CHAR, "Enter 'Y' to delete. [Press any other key to ignore]: ");
   /* Deletes person from array if chosen, otherwise goes back to start */
   if(userOpt2 == 'Y')
   {
      /* Find final Serial Number */
      int i = 0;
      while(address_book->list[i].si_no != 0 && address_book->list[i].si_no != NULL)
      {
         i++;
      }
      int finalSNO = i;

      /* Shifts every contact over by one if set to be deleted contact is not the last contact*/
      if(chosenSNO < finalSNO)
      {
         do
         {
            address_book->list[chosenSNO - 1] = address_book->list[chosenSNO];
            address_book->list[chosenSNO - 1].si_no--;
            chosenSNO++;
         } while (chosenSNO < finalSNO);
      }
      /* Reallocates size of list to delete the final contact*/
      ContactInfo* newSize = realloc(address_book->list, (chosenSNO)*sizeof(ContactInfo));
      address_book->list = newSize;
      address_book->count--;
      printf("Successfully deleted Contact.\n");
   }
   printf("Returning to delete menu\n");
   goto start;
   
   end:
   return e_success;
}

//Gets the pointer to a contact, useful for deleting and editing
ContactInfo* getContactAddress(AddressBook *addBook, int sno)
{
   ContactInfo * ptr = addBook->list;
   ContactInfo * endPtr = addBook->list + addBook->count;
   for (; ptr < endPtr; ptr++)
   {
      if (ptr->si_no == sno)
         return ptr;
   }
   return NULL;
}
