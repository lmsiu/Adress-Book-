#include "address_book.h"
#include "address_book_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <malloc/malloc.h>

int get_option(int type, const char *msg){
	printf("%s", msg);
   
    switch (type){
        case NUM:{ //added {} to enclose cases
            int opt;
            scanf("%d", &opt);
            return opt;
        }
        case CHAR:{
            char opt;
            scanf("%c", &opt);
            return opt;
        }
        case NONE:
            return e_no_opt;
   }
   return -1; //added return statement
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (*str != '\0') //added * to beginning of str
	{
		printf("#######  %s\n", str);
	}
}

int add_menu(ContactInfo* newContact, /*int contactCreated,*/ int phoneNumCount, int emailCount){
    menu_header("Add Contact:\n");
    printf("0. Back\n");
    printf("1. Name: %s\n", newContact->name[0]);//(contactCreated==1) ? newContact->name[0]:"");
    printf("2. Phone Number 1: %s\n", (phoneNumCount>0) ? newContact->phone_numbers[0]:"");
    for(int i = 1; i<=phoneNumCount-1; i++){
      printf("   Phone Number %i: %s\n", i+1, newContact->phone_numbers[i]);
    }
    printf("3. Email Address 1: %s\n", (emailCount!=0) ? newContact->email_addresses[0]:"");
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

    menu:
    add_menu(&newContact, phoneNumCount, emailCount);

    searchOption = get_option(NUM, "Please select an option: ");

    if(searchOption == 0){
        printf("Go back!\n");//delete later
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
            sleep(1);//may need to delete if i cannot add the <unistd.h> library
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
         sleep(1);//may need to delete if i cannot add the <unistd.h> library
      }else{
         printf("Please enter email address %i: ", emailCount);
         scanf("%s", tempEmail);
         strcpy(newContact.email_addresses[emailCount-1], tempEmail);
      }
    } else{
        printf("Error! Please select a number from 0 to 3.\n");
        sleep(1);//may need to delete if i cannot add the <unistd.h> library
    }
    goto menu;

    quit:
    printf("Time to save\n");//delete later

    if(contactCreated == 0){
        printf("No contact was created.\n");
        return e_back;
    }
    printf("Contact created.\n");

    return e_success;
}

int main(){
    AddressBook address_book;
    address_book.count = 0;
    printf("List address: %p\n", address_book.list);
    address_book.list = malloc(0); //possibly not needed, can possibly just use realloc without an initial malloc()
 
    add_contacts(&address_book);

    /*
    printf("Count: %d\n", address_book.count);
    printf("List address: %p\n", address_book.list);
    printf("List size BEFORE: %lu\n", sizeof(address_book.list));
    printf("List size BEFORE w/ &: %lu\n", sizeof(&address_book.list));
    printf("List size BEFORE w/ *: %lu\n", sizeof(*address_book.list));
    printf("List size w/ malloc_size: %zu\n", malloc_size(address_book.list));
    address_book.list = malloc(0);
    printf("List address: %p\n", address_book.list);
    printf("List size AFTER: %lu\n", sizeof(address_book.list));
    printf("List size AFTER w/ &: %lu\n", sizeof(&address_book.list));
    printf("List size AFTER w/ *: %lu\n", sizeof(*address_book.list));
    printf("List size AFTER w/ malloc_size: %zu\n", malloc_size(address_book.list));

    ContactInfo contact1;
    strcpy(contact1.name[0], "Abe");
    strcpy(contact1.phone_numbers[0], "818");
    strcpy(contact1.phone_numbers[1], "213");
    strcpy(contact1.email_addresses[0], "hey@");
    strcpy(contact1.email_addresses[1], "chu@");

    ContactInfo contact2;
    strcpy(contact2.name[0], "Wall");
    strcpy(contact2.phone_numbers[0], "909");
    strcpy(contact2.phone_numbers[1], "811");
    strcpy(contact2.email_addresses[0], "bleh@");
    strcpy(contact2.email_addresses[1], "kek@");
    
    address_book.list[0] = contact1;
    address_book.list[1] = contact2;
    printf("Name: %s\n", address_book.list[0].name[0]);
    printf("Phone 1: %s\n", address_book.list[0].phone_numbers[0]);
    printf("Phone 2: %s\n", address_book.list[0].phone_numbers[1]);
    printf("Email 1: %s\n", address_book.list[0].email_addresses[0]);
    printf("Email 2: %s\n", address_book.list[0].email_addresses[1]);

    printf("Name: %s\n", address_book.list[1].name[0]);
    printf("Phone 1: %s\n", address_book.list[1].phone_numbers[0]);
    printf("Phone 2: %s\n", address_book.list[1].phone_numbers[1]);
    printf("Email 1: %s\n", address_book.list[1].email_addresses[0]);
    printf("Email 2: %s\n", address_book.list[1].email_addresses[1]);


    //realloc copies data?? and realloc changes mem size to count * sizeof(CIptr)
    address_book.count = 3;
    address_book.list = realloc(address_book.list, (address_book.count)*sizeof(ContactInfo*));
    printf("Realloc list with malloc_size: %zu\n", malloc_size(address_book.list));
    
    //temp works to check null? temp erases list after function finishes?

    ContactInfo contact3;
    strcpy(contact3.name[0], "Egg");
    strcpy(contact3.phone_numbers[0], "000");
    strcpy(contact3.phone_numbers[1], "811");
    strcpy(contact3.phone_numbers[2], "4000");
    strcpy(contact3.phone_numbers[3], "811");
    strcpy(contact3.phone_numbers[4], "789");
    strcpy(contact3.email_addresses[0], "lic@");
    strcpy(contact3.email_addresses[1], "ori@");
    strcpy(contact3.email_addresses[2], "wew@");
    strcpy(contact3.email_addresses[3], "yiu@");
    strcpy(contact3.email_addresses[4], "kek@");
    
    address_book.list[address_book.count-1] = contact3;
    printf("Name: %s\n", address_book.list[0].name[0]);
    printf("Phone 1: %s\n", address_book.list[0].phone_numbers[0]);
    printf("Phone 2: %s\n", address_book.list[0].phone_numbers[1]);
    printf("Email 1: %s\n", address_book.list[0].email_addresses[0]);
    printf("Email 2: %s\n", address_book.list[0].email_addresses[1]);

    printf("Name: %s\n", address_book.list[1].name[0]);
    printf("Phone 1: %s\n", address_book.list[1].phone_numbers[0]);
    printf("Phone 2: %s\n", address_book.list[1].phone_numbers[1]);
    printf("Email 1: %s\n", address_book.list[1].email_addresses[0]);
    printf("Email 2: %s\n", address_book.list[1].email_addresses[1]);

    //deleting a contact at the end of list by decreasing reallocated size
    address_book.list = realloc(address_book.list, (2)*sizeof(ContactInfo*));
    printf("Realloc to delete: %zu\n", malloc_size(address_book.list));

    printf("Name: %s\n", address_book.list[2].name[0]);
    printf("Phone 1: %s\n", address_book.list[2].phone_numbers[0]);
    printf("Phone 2: %s\n", address_book.list[2].phone_numbers[1]);
    printf("Phone 3: %s\n", address_book.list[2].phone_numbers[2]);
    printf("Phone 4: %s\n", address_book.list[2].phone_numbers[3]);
    printf("Phone 5: %s\n", address_book.list[2].phone_numbers[4]);
    printf("Email 1: %s\n", address_book.list[2].email_addresses[0]);
    printf("Email 2: %s\n", address_book.list[2].email_addresses[1]);
    printf("Email 3: %s\n", address_book.list[2].email_addresses[2]);
    printf("Email 4: %s\n", address_book.list[2].email_addresses[3]);
    printf("Email 5: %s\n", address_book.list[2].email_addresses[4]);
    */

    free(address_book.list);

    return 0;
}