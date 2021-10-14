#include "address_book.h"
#include "address_book_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> //need for sleep()
#include <malloc/malloc.h>
#include <errno.h>

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

    if(contactCreated == 0){
        printf("No contact was created.\n");
        return e_back;
    }

    //increase count by 1
    address_book -> count += 1;

    //Assign serial id based on address_book->count variable
    newContact.si_no = address_book->count;

    printf("List size before realloc(): %zu\n", malloc_size(address_book->list));
    ContactInfo* tempPtr = realloc(address_book->list, (address_book->count)*sizeof(ContactInfo));
    if(tempPtr == NULL){
        printf("Memory reallocation failed! Please try to add contact again later.\n");
        return e_fail;
    }
    address_book->list = tempPtr;

    address_book->list[(address_book->count)-1] = newContact;

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


int main(){
    AddressBook address_book;
    address_book.count = 0; //need to init count = 0
    printf("List address: %p\n", address_book.list);
    address_book.list = malloc(0); //possibly not needed, can possibly just use realloc without an initial malloc()
 
    add_contacts(&address_book);
    add_contacts(&address_book);
    add_contacts(&address_book);

    for(int i = 0; i < address_book.count; i++){
        printf("Name %d: %s\n", i+1, address_book.list[i].name[0]);
        for(int k = 0; k < PHONE_NUMBER_COUNT; k++){
            printf("Phone %d: %s\n", k+1, address_book.list[i].phone_numbers[k]);
        }
        for(int j = 0; j < EMAIL_ID_COUNT; j++){
            printf("Email %d: %s\n", j+1, address_book.list[i].email_addresses[j]);
        }
        printf("Serial ID: %d\n", address_book.list[i].si_no);
        printf("\n");
    }

    save_file(&address_book);

    free(address_book.list);

    return 0;
}