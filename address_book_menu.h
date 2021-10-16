#ifndef ABK_MENUS_H
#define ABK_MENUS_H

#include "address_book.h"

#define	NONE							0
#define	NUM								1
#define	CHAR							2

#define WINDOW_SIZE						5

//For use as fields
#define NAME   1
#define NUMBER 2
#define EMAIL  3
#define SERIAL 4

void menu_header(const char *str);
void main_menu(void);
Status menu(AddressBook *address_book);
Status save_prompt(AddressBook *address_book);
Status add_contacts(AddressBook *address_book);
Status search_contact(AddressBook *address_book, Modes comingFrom);
ContactInfo* getContactAddress(AddressBook *addBook, int sno);
int compareFields(int field, const char * toCheck, ContactInfo * contact);
Status edit_contact(AddressBook *address_book);
Status delete_contact(AddressBook *address_book);
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode);


#endif
