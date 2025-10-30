#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
 
// Function declarations
int is_valid_name(char *name);
int is_valid_phone(char *phone);
int is_valid_email(char *email);
int is_unique_phone(AddressBook *addressBook, char *phone);
int is_unique_email(AddressBook *addressBook, char *email);

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0; // Set initial contact count to 0
}

void listContacts(AddressBook *addressBook) 
{

    // Print header
    printf("%-4s %-20s %-25s %-43s\n", "SL", "Name", "Phone", "Email");
    printf("----------------------------------------------------------------------------------\n");

    // Listing all contacts in a single line
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
       printf("%-4d %-20s %-25s %-43s\n", i + 1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }

}

void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook)
{
    char name[50], phone[11], email[50];
    int valid = 0;
 
    // Name validation
    do 
    {
        printf("Enter name : ");
        scanf("%s", name);
        valid = is_valid_name(name);

        if (!valid) 
        {
            printf("Invalid name. Only alphabets are allowed.\n");
        }
    } while (!valid);

    // Phone number validation and uniqueness check
    do 
    {
        printf("Enter phone number (10 digits): ");
        scanf("%s", phone);
        valid = is_valid_phone(phone) && is_unique_phone(addressBook, phone);

        if (!valid)
        {
            if (!is_valid_phone(phone)) 
            {
                printf("Invalid phone number. It must be 10 digits.\n");
            } 
            else 
            {
                printf("Phone number already exists.\n");
            }
        }
    } while (!valid);

    // Email validation and uniqueness check
    do 
    {
        printf("Enter email (lowercase letters only, must contain '@'): ");
        scanf("%s", email);
        valid = is_valid_email(email) && is_unique_email(addressBook, email);

        if (!valid) 
        {
            if (!is_valid_email(email)) 
            {
                printf("Invalid email format.\n");
            } else 
            {
                printf("Email already exists.\n");
            }
        }
    } while (!valid);

    // Add the contact to the address book
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);
    addressBook->contactCount++;

    printf("Contact created successfully!\n");
}

// Validation functions

int is_valid_name(char *name) 
{
    for (int i = 0; name[i] != '\0'; i++) 
    {
        if (!(name[i] >= 'a' && name[i] <= 'z') && !(name[i] >= 'A' && name[i] <= 'Z')) 
        {
            return 0;
        }
    }
    return 1;  // Valid name
}

int is_valid_phone(char *phone) 
{
    if (strlen(phone) != 10) 
    {
        return 0;
    }
    for (int i = 0; phone[i] != '\0'; i++) 
    {
        if (!(phone[i] >= '0' && phone[i] <= '9')) 
        {
            return 0;
        }
    }
    return 1;  // Valid phone number
}

int is_valid_email(char *email) 
{
    int atSymbol = 0;
    char *dotPos = NULL;

    for (int i = 0; email[i] != '\0'; i++) 
    {
        if (email[i] == '@') 
        {
            atSymbol = 1;  // Found '@' symbol
        } 
        if (email[i] == '.') 
        {
            dotPos = &email[i];  // Track the last '.' position
        }
        if (!((email[i] >= 'a' && email[i] <= 'z') || email[i] == '@' || email[i] == '.')) 
        {
            return 0;  // Invalid character found
        }
    }

    // Check if '@' is present and '.com' is the domain
    if (atSymbol && dotPos && strcmp(dotPos + 1, "com") == 0) 
    {
        return 1;  // Valid email
    }

    return 0;  // Invalid if no '@' or not ending with '.com'
}

// Check if phone number is unique
int is_unique_phone(AddressBook *addressBook, char *phone) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0) 
        {
            return 0;  // Phone number already exists
        }
    }
    return 1;  // Phone number is unique
}

// Check if email is unique
int is_unique_email(AddressBook *addressBook, char *email) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0) 
        {
            return 0;  // Email already exists
        }
    }
    return 1;  // Email is unique
}

// Function to search a contact in the AddressBook
void searchContact(AddressBook *addressBook) 
{
    char searchTerm[50];
    int found = 0; // Flag to track if any contact was found

    printf("Enter name, phone number, or email to search: ");
    scanf("%s", searchTerm); // Get the search term from the user

    // Print header
    printf("%-10s %-15s %-30s\n", "Name", "Phone", "Email");
    printf("--------------------------------------------------------\n");

    // Iterate through all contacts to find matches
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        // Check if the search term matches any of the contact details
        if (strcmp(addressBook->contacts[i].name, searchTerm) == 0 || 
            strcmp(addressBook->contacts[i].phone, searchTerm) == 0 || 
            strcmp(addressBook->contacts[i].email, searchTerm) == 0) 
        {
            // Print the matched contact details
            printf("%-10s %-15s %-30s\n", 
                addressBook->contacts[i].name, 
                addressBook->contacts[i].phone, 
                addressBook->contacts[i].email);
            found = 1; // Set found flag to true
        }
    }

    if (!found) 
    {
        printf("No contact found matching '%s'.\n", searchTerm);
    }
}

//edit option validation

void editContact(AddressBook *addressBook)
{
    char searchTerm[50];
    int foundIndexes[50];  // To store indexes of matching contacts
    int foundCount = 0;

    printf("Enter the name, phone, or email of the contact to edit: ");
    scanf("%s", searchTerm);

    // Search for all contacts that match the search term
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].name, searchTerm) == 0 ||
            strcmp(addressBook->contacts[i].phone, searchTerm) == 0 ||
            strcmp(addressBook->contacts[i].email, searchTerm) == 0) 
        {
            foundIndexes[foundCount] = i;
            foundCount++;
        }
    }

    if (foundCount == 0) 
    {
        printf("No contact found matching '%s'.\n", searchTerm);
        return;
    }

    // If multiple contacts are found, ask the user to choose which one to edit
    int selectedContactIndex;
    if (foundCount > 1) 
    {
        printf("Multiple contacts found. Please choose which one to edit:\n");
        for (int i = 0; i < foundCount; i++) 
        {
            int index = foundIndexes[i];
            printf("%d. %-10s %-15s %-30s\n", i + 1, 
                addressBook->contacts[index].name, 
                addressBook->contacts[index].phone, 
                addressBook->contacts[index].email);
        }

        int choice;
        do 
        {
            printf("Enter the number of the contact you want to edit (1-%d): ", foundCount);
            scanf("%d", &choice);
        } while (choice < 1 || choice > foundCount);

        // Now, 'choice' is the index of the selected contact in foundIndexes[]
        selectedContactIndex = foundIndexes[choice - 1];
    } 
    else 
    {
        // If only one contact is found, directly proceed with editing
        selectedContactIndex = foundIndexes[0];
    }

    printf("Editing contact %s...\n", addressBook->contacts[selectedContactIndex].name);

    // Ask the user if they want to edit all fields or specific fields
    int editAllFields;
    printf("Do you want to edit all fields (name, phone, and email)? (1 for Yes, 0 for No): ");
    scanf("%d", &editAllFields);

    char newName[50], newPhone[11], newEmail[50];
    int valid;

    if (editAllFields) 
    {
        // Edit all fields
        // Name validation
        do 
        {
            printf("Enter new name : ");
            scanf("%s", newName);
            valid = is_valid_name(newName);

            if (!valid) 
            {
                printf("Invalid name. Only alphabets are allowed.\n");
            }
        } while (!valid);

        // Phone number validation and uniqueness check
        do 
        {
            printf("Enter new phone number (10 digits): ");
            scanf("%s", newPhone);
            valid = is_valid_phone(newPhone) && is_unique_phone(addressBook, newPhone);

            if (!valid) 
            {
                if (!is_valid_phone(newPhone)) 
                {
                    printf("Invalid phone number. It must be 10 digits.\n");
                } 
                else 
                {
                    printf("Phone number already exists.\n");
                }
            }
        } while (!valid);

        // Email validation and uniqueness check
        do 
        {
            printf("Enter new email : ");
            scanf("%s", newEmail);
            valid = is_valid_email(newEmail) && is_unique_email(addressBook, newEmail);

            if (!valid) 
            {
                if (!is_valid_email(newEmail)) 
                {
                    printf("Invalid email format.\n");
                } 
                else 
                {
                    printf("Email already exists.\n");
                }
            }
        } while (!valid);

        // Update all fields
        strcpy(addressBook->contacts[selectedContactIndex].name, newName);
        strcpy(addressBook->contacts[selectedContactIndex].phone, newPhone);
        strcpy(addressBook->contacts[selectedContactIndex].email, newEmail);

        printf("All fields updated successfully!\n");
    } 
    else 
    {
        // Ask which field the user wants to edit (for individual field editing)
        int fieldChoice;
        printf("Which field do you want to edit?\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        do 
        {
            printf("Enter the number of the field to edit (1-3): ");
            scanf("%d", &fieldChoice);
        } while (fieldChoice < 1 || fieldChoice > 3);

        switch (fieldChoice) 
        {
            case 1:  // Edit name
                // Name validation
                do 
                {
                    printf("Enter new name : ");
                    scanf("%s", newName);
                    valid = is_valid_name(newName);

                    if (!valid) 
                    {
                        printf("Invalid name. Only alphabets are allowed.\n");
                    }
                } while (!valid);

                // Update name
                strcpy(addressBook->contacts[selectedContactIndex].name, newName);
                printf("Name updated successfully!\n");
                break;

            case 2:  // Edit phone
                // Phone number validation and uniqueness check
                do 
                {
                    printf("Enter new phone number (10 digits): ");
                    scanf("%s", newPhone);
                    valid = is_valid_phone(newPhone) && is_unique_phone(addressBook, newPhone);

                    if (!valid) 
                    {
                        if (!is_valid_phone(newPhone)) 
                        {
                            printf("Invalid phone number. It must be 10 digits.\n");
                        } 
                        else 
                        {
                            printf("Phone number already exists.\n");
                        }
                    }
                } while (!valid);

                // Update phone number
                strcpy(addressBook->contacts[selectedContactIndex].phone, newPhone);
                printf("Phone number updated successfully!\n");
                break;

            case 3:  // Edit email
                // Email validation and uniqueness check
                do 
                {
                    printf("Enter new email : ");
                    scanf("%s", newEmail);
                    valid = is_valid_email(newEmail) && is_unique_email(addressBook, newEmail);

                    if (!valid) 
                    {
                        if (!is_valid_email(newEmail)) 
                        {
                            printf("Invalid email format.\n");
                        } 
                        else 
                        {
                            printf("Email already exists.\n");
                        }
                    }
                } while (!valid);

                // Update email
                strcpy(addressBook->contacts[selectedContactIndex].email, newEmail);
                printf("Email updated successfully!\n");
                break;

            default:
                printf("Invalid choice.\n");
                return;
        }
    }
}

void deleteContact(AddressBook *addressBook)
{
    char searchTerm[50];
    int foundIndexes[50];  // To store indexes of matching contacts
    int foundCount = 0;

    printf("Enter the name, phone, or email of the contact to delete: ");
    scanf("%s", searchTerm);

    // Search for all contacts that match the search term
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].name, searchTerm) == 0 ||
            strcmp(addressBook->contacts[i].phone, searchTerm) == 0 ||
            strcmp(addressBook->contacts[i].email, searchTerm) == 0) 
        {
            foundIndexes[foundCount] = i;
            foundCount++;
        }
    }

    if (foundCount == 0) 
    {
        printf("No contact found matching '%s'.\n", searchTerm);
        return;
    }

    // If multiple contacts are found, ask the user to choose which one to delete
    if (foundCount > 1) 
    {
        printf("Multiple contacts found. Please choose which one to delete:\n");
        for (int i = 0; i < foundCount; i++) 
        {
            int index = foundIndexes[i];
            printf("%d. %-10s %-15s %-30s\n", i + 1, 
                addressBook->contacts[index].name, 
                addressBook->contacts[index].phone, 
                addressBook->contacts[index].email);
        }

        int choice;
        do 
        {
            printf("Enter the number of the contact you want to delete (1-%d): ", foundCount);
            scanf("%d", &choice);
        } while (choice < 1 || choice > foundCount);

        // Get the selected contact index
        int selectedContactIndex = foundIndexes[choice - 1];

        // Shift all subsequent contacts up by one position
        for (int i = selectedContactIndex; i < addressBook->contactCount - 1; i++) 
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
        addressBook->contactCount--;  // Decrease the total number of contacts

        printf("Contact deleted successfully!\n");
    } 
    else 
    {
        // If only one contact is found, delete it directly
        int selectedContactIndex = foundIndexes[0];

        // Shift all subsequent contacts up by one position
        for (int i = selectedContactIndex; i < addressBook->contactCount - 1; i++) 
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
        addressBook->contactCount--;  // Decrease the total number of contacts

        printf("Contact deleted successfully!\n");
    }

      
}
