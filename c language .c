#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    char blood_type[5];
    char last_donation_date[15];
    int available_units;
} Donor;

typedef struct {
    char name[50];
    char blood_type[5];
    int required_units;
    int fulfilled;
} Recipient;

// ----------------------------------------
// BLOOD COMPATIBILITY CHECKER
// ----------------------------------------
int isCompatible(char recipient[], char donor[]) {

    if (strcmp(recipient, "AB+") == 0)
        return 1;  // Universal recipient

    if (strcmp(recipient, "O-") == 0)
        return strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "O+") == 0)
        return strcmp(donor, "O+") == 0 || strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "A-") == 0)
        return strcmp(donor, "A-") == 0 || strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "A+") == 0)
        return strcmp(donor, "A+") == 0 || strcmp(donor, "A-") == 0 ||
               strcmp(donor, "O+") == 0 || strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "B-") == 0)
        return strcmp(donor, "B-") == 0 || strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "B+") == 0)
        return strcmp(donor, "B+") == 0 || strcmp(donor, "B-") == 0 ||
               strcmp(donor, "O+") == 0 || strcmp(donor, "O-") == 0;

    if (strcmp(recipient, "AB-") == 0)
        return strcmp(donor, "AB-") == 0 || strcmp(donor, "A-") == 0 ||
               strcmp(donor, "B-") == 0 || strcmp(donor, "O-") == 0;

    return 0;
}

// ----------------------------------------
// ALLOCATE BLOOD TO RECIPIENTS
// ----------------------------------------
void allocateBlood(Donor *donors, int dCount, Recipient *recipients, int rCount) {
    for (int i = 0; i < rCount; i++) {
        int unitsNeeded = recipients[i].required_units;

        for (int j = 0; j < dCount && unitsNeeded > 0; j++) {

            if (isCompatible(recipients[i].blood_type, donors[j].blood_type)) {

                int canGive = donors[j].available_units >= unitsNeeded
                              ? unitsNeeded
                              : donors[j].available_units;

                donors[j].available_units -= canGive;
                unitsNeeded -= canGive;
            }
        }

        recipients[i].fulfilled = (unitsNeeded == 0);
    }
}

// ----------------------------------------
// DAILY SUMMARY REPORT
// ----------------------------------------
void dailySummary(Donor *donors, int dCount, Recipient *recipients, int rCount) {

    printf("\n\n--- DAILY BLOOD BANK SUMMARY ---\n");

    printf("\nAvailable Units After Allocation:\n");
    for (int i = 0; i < dCount; i++) {
        printf("%s (%s): %d units remaining\n",
               donors[i].name, donors[i].blood_type, donors[i].available_units);
    }

    printf("\nRecipient Status:\n");
    for (int i = 0; i < rCount; i++) {
        printf("%s (%s): Required %d units --> %s\n",
               recipients[i].name,
               recipients[i].blood_type,
               recipients[i].required_units,
               recipients[i].fulfilled ? "Fulfilled" : "Pending");
    }
}

// ----------------------------------------
// MAIN FUNCTION
// ----------------------------------------
int main() {
    int dCount, rCount;

    printf("Enter number of donors: ");
    scanf("%d", &dCount);
    getchar();

    Donor *donors = (Donor *)malloc(dCount * sizeof(Donor));

    printf("\nEnter Donor Details:\n");
    for (int i = 0; i < dCount; i++) {
        printf("\nDonor %d Name: ", i + 1);
        fgets(donors[i].name, 50, stdin);
        donors[i].name[strcspn(donors[i].name, "\n")] = '\0';

        printf("Blood Type: ");
        scanf("%s", donors[i].blood_type);

        printf("Last Donation Date: ");
        scanf("%s", donors[i].last_donation_date);

        printf("Available Units: ");
        scanf("%d", &donors[i].available_units);

        getchar();
    }

    printf("\nEnter number of recipients: ");
    scanf("%d", &rCount);
    getchar();

    Recipient *recipients = (Recipient *)malloc(rCount * sizeof(Recipient));

    printf("\nEnter Recipient Details:\n");
    for (int i = 0; i < rCount; i++) {
        printf("\nRecipient %d Name: ", i + 1);
        fgets(recipients[i].name, 50, stdin);
        recipients[i].name[strcspn(recipients[i].name, "\n")] = '\0';

        printf("Blood Type: ");
        scanf("%s", recipients[i].blood_type);

        printf("Required Units: ");
        scanf("%d", &recipients[i].required_units);

        getchar();
    }

    // Perform allocation
    allocateBlood(donors, dCount, recipients, rCount);

    // Generate summary
    dailySummary(donors, dCount, recipients, rCount);

    // Free memory
    free(donors);
    free(recipients);

    return 0;
}