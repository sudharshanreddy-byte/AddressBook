#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

#define FILE_NAME "contacts.txt"

// Save contacts to file
void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) 
    {
        printf("Error opening file for saving contacts.\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        fprintf(file, "%s %s %s\n", 
            addressBook->contacts[i].name, 
            addressBook->contacts[i].phone, 
            addressBook->contacts[i].email);
    }

    fclose(file);
    
}

// Load contacts from file
void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) 
    {
        printf("No existing contacts found.\n");
        return;
    }

    char name[50], phone[11], email[50];
    addressBook->contactCount = 0;

    while (fscanf(file, "%s %s %s", name, phone, email) == 3) 
    {
        strcpy(addressBook->contacts[addressBook->contactCount].name, name);
        strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
        strcpy(addressBook->contacts[addressBook->contactCount].email, email);
        addressBook->contactCount++;
    }

    fclose(file);
    
}
