#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemberTag
{
    int id;
    char name[30];
    int age;
    char gender[10]; 
    int enrolled[4]; // Yoga, Cardio, Zumba and Weight lifting
    int amountPaid[4];
    struct MemberTag* next;
} Member;

typedef struct TrainerTag
{
    int id;
    char name[30];
    int offered[4];
    int remuneration;
    struct TrainerTag* next;
} Trainer;

typedef struct SlotTag
{
    int time;
    int memberID;
    int trainerID;
    int preferredTrainer;
    int program;
    char timestamp[15];
    struct SlotTag* next;
} Slot;

char programs[4][15] = {"Yoga", "Cardio", "Zumba", "Weight lifting"};
int fees[4] = {100, 200, 300, 400};

Trainer* partition(Trainer *lptr);
Trainer* merge(Trainer *lptr, Trainer *nptr, int state);
Trainer* mergeSort(Trainer *list_ptr, int state);


void printMemberNode(Member* member)
{
    
    printf("\nID: %d\nName: %s\nAge: %d\nGender: %s\n", member->id, member->name, member->age, member->gender);

    printf("Enrolled in programs and amount paid: \n");
    for(int i = 0; i < 4; i++)
    {
        if(member->enrolled[i])
        {
            printf("%s\t%d\n", programs[i], member->amountPaid[i]);
        }
    }
}

void printTrainerNode(Trainer* trainer)
{
    printf("\nID: %d\nName: %s\n", trainer->id, trainer->name);

    printf("Offered programs: \n");
    for(int i = 0; i < 4; i++)
    {
        if(trainer->offered[i])
        {
            printf("%s\n", programs[i]);
        }
    }
    
    printf("Remuneration: %d\n", trainer->remuneration);
}

void printSlotNode(Slot* assigned)
{
    
    printf("\nAllotted time: %d\nMember ID: %d\nPreferred Trainer %d\nAllotted trainer: %d\nChosen Program: %s\nTimestamp: %s\n", assigned->time, assigned->memberID, assigned->preferredTrainer, assigned->trainerID, programs[assigned->program], assigned->timestamp);
}

void printSlotList(Slot* shead)
{
    printf("Time is in 24 hour format\n");
    Slot* ptr = shead;
    while(ptr != NULL)
    {
        printSlotNode(ptr);
        ptr = ptr->next;
    }
}

int exists(Trainer* tptr, Slot* ashead)
{
    int found = 0;
    Slot* asptr = ashead;

    while(asptr != NULL)
    {
        if(asptr->trainerID == tptr->id)
            found = 1;
        asptr = asptr->next;
    }

    return found;
}

/*
  1. Add_member()
  Input: all the fields related to a member listed above (Member-ID should be randomly
  generated 4-digit number).
  Output: Display a message after successfully adding a member.
  Note: Records of members have to be maintained in a sorted manner based on key as their
  member-ID.
*/
void addMember(Member** mhead)
{
    
    Member* member = (Member*) malloc(sizeof(Member));

    member->id = (rand() % 9000) + 1000;

    getchar();
    
    printf("Enter name: \n");
    gets(member->name);
    
    printf("Enter age: \n");
    scanf("%d", &member->age);

    printf("Enter gender: \n");
    scanf("%s", member->gender);

    int choice;
    for(int i = 0; i < 4; i++)
    {
        printf("would you like to enroll in %s (0: no, 1: yes)\n", programs[i]);
        scanf("%d", &member->enrolled[i]);
    
        member->amountPaid[i] = fees[i] * member->enrolled[i];
    }

    Member* ptr = *mhead, * prev = NULL;
    while(ptr != NULL && ptr->id < member->id)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if(prev == NULL)
    {
        member->next = *mhead;
        *mhead = member;
    }
    else
    {
        prev->next = member;
        member->next = ptr;
    }    
}

/*
  2. Add_trainer()
  Input: all the fields related to a trainer listed above.
  Output: Display a message after successfully adding a trainer.
  Note: Records of trainers have to be maintained in a sorted manner based on key as their
  Trainer-ID.
*/
void addTrainer(Trainer** thead)
{    
    Trainer* trainer = (Trainer*) malloc(sizeof(Trainer));
    
    trainer->id = (rand() % 9000) + 1000;

    getchar();
    
    printf("Enter name: \n");
    gets(trainer->name);
    
    
    int choice;

    for(int i = 0; i < 4; i++)
    {
        printf("would you like to train %s (0: no, 1: yes)\n", programs[i]);
        scanf("%d", &trainer->offered[i]);
    
        trainer->remuneration = 0;
    }

    

    Trainer* ptr = *thead, * prev = NULL;
    while(ptr != NULL && ptr->id < trainer->id)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if(prev == NULL)
    {
        trainer->next = *thead;
        *thead = trainer;
    }
    else
    {
        prev->next = trainer;
        trainer->next = ptr;
    }
}

void schedule(Trainer* thead, Slot** rshead, Slot** ashead, Slot* rprev, Slot* rptr, int time)
{
    Slot* aptr = *ashead, * prevAS = NULL, * start = *ashead;
    Trainer* tptr = thead;

    Trainer* freeTrainer = NULL;

    int count = 0, unavailable = 0;
    
    
    while(aptr != NULL && aptr->time <= time)
    {
        if(aptr->time == time)
        {
            if(aptr->trainerID == rptr->trainerID)
                unavailable = 1;
            count++;
        }
        else
        {
            prevAS = aptr;
            start = start->next;
        }
        aptr = aptr->next;
    }

    if(count < 10)
    {
        if(!unavailable)
        {
            while(tptr != NULL && freeTrainer == NULL)
            {
                if(tptr->id == rptr->trainerID && tptr->offered[rptr->program] == 1)
                    freeTrainer = tptr;
                tptr = tptr->next;
            }
        }

        tptr = thead;

        while(tptr != NULL && freeTrainer == NULL)
        {
            if(tptr->offered[rptr->program] == 1 && !exists(tptr, start))
                freeTrainer = tptr;
            tptr = tptr->next;
        }

        if(freeTrainer != NULL)
        {
            printf("assigned trainer: %d\n", freeTrainer->id);

            aptr = start;

            while(aptr != NULL && aptr->time == time && aptr->program < rptr->program)
            {
                prevAS = aptr;
                aptr = aptr->next;
            }

            Slot* nptr;
            
            freeTrainer->remuneration += fees[rptr->program];
            nptr = rptr;
            nptr->trainerID = freeTrainer->id;
            rptr->time = time;
            nptr->time = time;
            
            if(rptr == *rshead)
            {
                *rshead = (*rshead)->next;
                rptr = *rshead;
            }
            else
            {
                rprev->next = rptr->next;
                rptr = rprev->next;
            }

            if(aptr == *ashead)
            {
                nptr->next = aptr;
                *ashead = nptr;
            }
            else
            {
                prevAS->next = nptr;
                nptr->next = aptr;
            }
            
            if(rptr != NULL)
            {
                schedule(thead, rshead, ashead, rprev, rptr, rptr->time);
            }
        }
        else if(time < 21)
        {
            schedule(thead, rshead, ashead, rprev, rptr, time + 1);
        }
        else if(rptr->next != NULL)
        {
            schedule(thead, rshead, ashead, rptr, rptr->next, rptr->next->time);
        }

    }
    else
    {
        schedule(thead, rshead, ashead, rprev, rptr, time + 1);
    }
}


Trainer* mergeTwoLists(Trainer* l1, Trainer* l2)
{
    
    if(l1 == NULL)
        return l2;
    else if(l2 == NULL)
        return l1;
    
    Trainer* p1 = l1, * p2 = l2, * l3 = NULL, * ptr = NULL;


    while(p1 != NULL && p2 != NULL)
    {
        
        if(ptr == NULL)
        {
            if(p1->id < p2->id)
            {
                ptr = p1;
                p1 = p1->next;
            }
            else
            {
                ptr = p2;
                p2 = p2->next;
            }
            l3 = ptr;
        }
        else
        {
            if(p1->id <= p2->id)
            {
                ptr->next = p1;
                ptr = p1;
                p1 = p1->next;
            }
            else
            {
                ptr->next = p2;
                ptr = p2;
                p2 = p2->next;
            }
        }
        
    }

    if(p1 != NULL)
        ptr->next = p1;
    else if(p2 != NULL)
        ptr->next = p2;
    return l3;
}

/*
  3. Schedule_slot()
  Input: list of members with their requested program, preferred time slot, preferred trainer
  (only 1) & the timestamp at which the request is made, and the list of trainers available in the
  club (note: All the trainers from the trainers' list may not be available).
  Output: list of members assigned to trainers along with their time slots during operating
  hours and the list of non-assigned members, if any.
  Note: The slots are assigned on a First-come-First-serve basis as per the request timestamp. If
  none of the trainers is free during the preferred time, then the member is assigned to the
  earliest available slot after the preferred time slot. As per the covid-norms set by the
  government and the floor capacity of the club, not more than 10 members can be present
  inside the club at any time. The trainers cannot be assigned for a program they don't train for.
  If a member requests for more than one program in a day, then the later request should be
  disallowed.
*/

void scheduleSlot(Trainer* thead, Slot** rshead, Slot** ashead)
{
    char choice;

    Slot* rptr = *rshead, * rprev = NULL, * aptr = *ashead;

    int freqs[9999-1000+1] = {0};

    // skip through existing records    
    while(rptr != NULL)
    {
        freqs[rptr->memberID - 1000] = 1;
        rprev = rptr;
        rptr = rptr->next;
    }
    while(aptr != NULL)
    {
        freqs[aptr->memberID - 1000] = 1;
        aptr = aptr->next;
    }
    
    printf("Enter the requests in increasing order of timestamp (earlier first): \n");
    do
    {
        Slot* nptr = (Slot*) malloc(sizeof(Slot));

        printf("Enter the member ID\n");
        scanf("%d", &nptr->memberID);

        
        printf("%d\n", freqs[nptr->memberID - 1000]);
        
        
        printf("Enter the preferred time (in 24 hour format ie between 6 and 21)\n");
        scanf("%d", &nptr->time);
        
        printf("Enter the preferred trainer's ID\n");
        scanf("%d", &nptr->trainerID);
        nptr->preferredTrainer = nptr->trainerID;

        printf("Enter the program (0: Yoga, 1: Cardio, 2: Zumba, 3: Weight lifting)\n");
        scanf("%d", &nptr->program);

        printf("Enter the timestamp (format: HH:MM:SS)\n");
        scanf("%s", nptr->timestamp);
        
        nptr->next = NULL;

        if(rprev == NULL && freqs[nptr->memberID - 1000] == 0)
        {
            freqs[nptr->memberID - 1000] = 1;
            *rshead = nptr;
            rprev = nptr;
        }
        else if(freqs[nptr->memberID - 1000] == 0)
        {
            freqs[nptr->memberID - 1000] = 1;
            rprev->next = nptr;
            rprev = nptr;
        }
        else
        {
            free(nptr);
        }

        getchar();
        printf("would you like to continue entering (y/n)\n");
        scanf("%c", &choice);
    } while(choice != 'n');

    Trainer* tptr = thead, * available = NULL, * tprev = NULL, * availptr = NULL;

    printf("Enter the ids of available trainers (in increasing order of IDs)\n");
    do
    {
        int id;
        printf("Enter the trainer id: \n");
        scanf("%d", &id);

        while(tptr->id < id)
        {
            tprev = tptr;
            tptr = tptr->next;
        }

        Trainer* nptr = tptr;

        if(tptr == thead)
        {
            thead = thead->next;
            tptr = thead;
        }
        else
        {
            tprev->next = tptr->next;
            tptr = tprev->next;
        }

        nptr->next = NULL;
        if(available == NULL)
        {
            available = nptr;
            availptr = nptr;
        }
        else
        {
            availptr->next = nptr;
            availptr = nptr;
        }

        getchar();
        printf("would you like to continue entering (y/n): ");
        scanf("%c", &choice);
    } while(choice != 'n');

    rprev = NULL;

    if(*rshead != NULL)
        schedule(available, rshead, ashead, rprev, *rshead, (*rshead)->time);
    
    thead = mergeTwoLists(available, thead);

}
/*
  4. Print_hour_wise_list():
  Input: No input
  Output: Hour/Slot-wise list of members & trainers assigned grouped by every program.
*/
void printHourWiseList(Slot* ashead)
{
    int currHour = -1, currProgram = -1;
    Slot* aptr = ashead;

    while(aptr != NULL)
    {
        if(aptr->time != currHour)
        {
            currHour = aptr->time;
            printf("\nHour: %d\n", currHour);
            currProgram = -1;
        }
        if(aptr->program != currProgram)
        {
            currProgram = aptr->program;
            printf("\nProgram: %s\n", programs[currProgram]);
        }
        printSlotNode(aptr);
        aptr = aptr->next;
    }
}


/*
  5. Delete_member():
  Input: Member-ID
  Output: Display a message after the successful deletion of the record.
*/
void deleteMember(Member** mhead)
{
    int id;

    printf("Enter the id to delete\n");
    scanf("%d", &id);

    Member* ptr = *mhead, * prev = NULL;

    while(ptr != NULL && ptr->id != id)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if(prev == NULL)
    {
        *mhead = (*mhead)->next;
        free(ptr);
    }
    else
    {
        prev->next = ptr->next;
        free(ptr);
    }
    
}

/*
  6. Delete_trainer():
  Input: Trainer-ID
  Output: Display a message after the successful deletion of the record.
*/
void deleteTrainer(Trainer** thead)
{
    int id;

    printf("Enter the id to delete\n");
    scanf("%d", &id);

    Trainer* ptr = *thead, * prev = NULL;

    while(ptr != NULL && ptr->id != id)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if(prev == NULL)
    {
        *thead = (*thead)->next;
        free(ptr);
    }
    else
    {
        prev->next = ptr->next;
        free(ptr);
    }
}


/*
  7. Search_member():
  Input: Key field to search and key-value
  Output: Display complete record based on key-value.
*/
void searchMember(Member* mhead)
{
    int choice;
    printf("Enter the field to search\n1. ID\n2. Name\n3. Age\n4.Gender\n5.Program enrolled\n");
    scanf("%d", &choice);

    if(choice == 1)
    {
        int id;
        
        printf("Enter the ID\n");
        scanf("%d", &id);

        Member* ptr = mhead;
        while(ptr != NULL && ptr->id != id)
        {
            ptr = ptr->next;
        }
        printMemberNode(ptr);
    }
    else if(choice == 2)
    {
        char name[15];

        getchar();
        printf("Enter the name: ");
        gets(name);

        Member* ptr = mhead;
        while(ptr != NULL)
        {
            if(strcmp(ptr->name, name) == 0)
            {
                printMemberNode(ptr);
            }
            ptr = ptr->next;
        }
        
    }
    else if(choice == 3)
    {
        int age;

        printf("Enter the age: \n");
        scanf("%d", &age);

        Member* ptr = mhead;
        while(ptr != NULL)
        {
            if(ptr->age == age)
            {
                printMemberNode(ptr);
            }
            ptr = ptr->next;
        }
    }
    else if(choice == 4)
    {
        char gender[15];

        printf("Enter the gender: ");
        scanf("%s", gender);

        Member* ptr = mhead;
        while(ptr != NULL)
        {
            if(strcmp(ptr->gender, gender) == 0)
            {
                printMemberNode(ptr);
            }
            ptr = ptr->next;
        }
    }
    else if(choice == 5)
    {
        int index;
        printf("Enter the program: ");
        for(int i = 0; i < 4; i++)
        {
            printf("%d. %s", i, programs[i]);
        }
        scanf("%d", &index);

        Member* ptr = mhead;
        while(ptr != NULL)
        {
            if(ptr->enrolled[index] == 1)
            {
                printMemberNode(ptr);
            }
            ptr = ptr->next;
        }
        
    }
    
}

/*
  8. Search_trainer():
  Input: Key field to search and key-value
  Output: Display complete record based on key-value.
*/
void searchTrainer(Trainer* thead)
{
    int choice;
    printf("Enter the field to search\n1. ID\n2. Name\n3. Program offered\n4. Remuneration earned\n");
    scanf("%d", &choice);

    if(choice == 1)
    {
        int id;

        
        printf("Enter the ID\n");
        scanf("%d", &id);

        Trainer* ptr = thead;
        while(ptr != NULL && ptr->id != id)
        {
            ptr = ptr->next;
        }
        printTrainerNode(ptr);
    }
    else if(choice == 2)
    {
        char name[15];

        getchar();
        printf("Enter the name: ");
        gets(name);

        Trainer* ptr = thead;
        while(ptr != NULL)
        {
            if(strcmp(ptr->name, name) == 0)
            {
                printTrainerNode(ptr);
            }
            ptr = ptr->next;
        }
        
    }
    else if(choice == 3)
    {
        int index;
        printf("Enter the program: ");
        for(int i = 0; i < 4; i++)
        {
            printf("%d. %s", i, programs[i]);
        }
        scanf("%d", &index);

        Trainer* ptr = thead;
        while(ptr != NULL)
        {
            if(ptr->offered[index] == 1)
            {
                printTrainerNode(ptr);
            }
            ptr = ptr->next;
        }
        
    }
    else if(choice == 4)
    {
        int remuneration;

        printf("Enter the remuneration: \n");
        scanf("%d", &remuneration);

        Trainer* ptr = thead;
        while(ptr != NULL)
        {
            if(ptr->remuneration == remuneration)
            {
                printTrainerNode(ptr);
            }
            ptr = ptr->next;
        }
    }
}

/*
  9. Print_member_list()
  Output: Display the records of all members.
*/
void printMemberList(Member* mhead)
{
    Member* ptr = mhead;

    while(ptr != NULL)
    {
        printMemberNode(ptr);
        ptr = ptr->next;
    }
}

/*
  10. Print_trainer_list()
  Output: Display the records of all trainers.
*/
void printTrainerList(Trainer* thead)
{
    Trainer* ptr = thead;

    while(ptr != NULL)
    {
        printTrainerNode(ptr);
        ptr = ptr->next;
    }

}

/*
  11. Print_sorted_remuneration_list():
  Output: Display the list of trainers in a sorted order according to their remuneration earned (highest to lowest) in a day.
*/
void printSortedRemunList(Trainer* thead)
{
    thead = mergeSort(thead, 1);
    printTrainerList(thead);
    thead = mergeSort(thead, 0);
}

/*
  12. Print_intersection_list():
  Input: list of members same as in function 3.
  Output: list of members who have been assigned their preferred trainer.
*/
void printIntersecList(Trainer* thead, Slot* rshead, Slot* ashead)
{
    Slot* aptr = ashead;
    while(aptr != NULL)
    {
        if(aptr->preferredTrainer == aptr->trainerID)
        {
            printSlotNode(aptr);
        }
        aptr = aptr->next;
    }
}


int main()
{
    srand(time(NULL));

    
    int choice;

    Member* mhead = NULL, * mptr = NULL, * prevMember = NULL;
    Trainer* thead = NULL, * tptr = NULL, * prevTrainer = NULL;
    Slot* ashead = NULL, * aptr = NULL, * prevAssigned = NULL;
    Slot* rshead = NULL, * rptr = NULL, * prevRequest = NULL;

    FILE* fp;

    char ch;
    
    fp = fopen("members.txt", "r");
    if (fp != NULL)
    {
        ch = fgetc(fp);

        while (ch != EOF)
        {
            int len;
            Member* nptr = (Member*) malloc(sizeof(Member));
            fscanf(fp, "%d", &nptr->id);
            ch = fgetc(fp);

            fgets(nptr->name, 30, fp);
            len = strlen(nptr->name);
            if (nptr->name[len - 1] == '\n')
                nptr->name[len - 1] = '\0';

            fscanf(fp, "%d", &nptr->age);
            ch = fgetc(fp);
            
            fgets(nptr->gender, 10, fp);
            len = strlen(nptr->gender);
            if (nptr->gender[len - 1] == '\n')
                nptr->gender[len - 1] = '\0';

            for(int i = 0; i < 4; i++)
                fscanf(fp, "%d", &nptr->enrolled[i]);
            ch = fgetc(fp);
            
            for(int i = 0; i < 4; i++)
                fscanf(fp, "%d", &nptr->amountPaid[i]);
            ch = fgetc(fp);
            
            if (prevMember == NULL)
            {
                mhead = nptr;
                prevMember = nptr;
            }
            else
            {
                prevMember->next = nptr;
                prevMember = nptr;
            }
            ch = getc(fp);
            ch = getc(fp);
        }
    }

    printf("\nMEMBERS: \n");
    printMemberList(mhead);

    fp = fopen("trainers.txt", "r");
    if (fp != NULL)
    {
        ch = fgetc(fp);

        while (ch != EOF)
        {
            int len;
            Trainer* nptr = (Trainer*) malloc(sizeof(Trainer));
            fscanf(fp, "%d", &nptr->id);
            ch = fgetc(fp);

            fgets(nptr->name, 30, fp);
            len = strlen(nptr->name);
            if (nptr->name[len - 1] == '\n')
                nptr->name[len - 1] = '\0';

            
            for(int i = 0; i < 4; i++)
                fscanf(fp, "%d", &nptr->offered[i]);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->remuneration);
            ch = fgetc(fp);
            
            if (prevTrainer == NULL)
            {
                thead = nptr;
                prevTrainer = nptr;
            }
            else
            {
                prevTrainer->next = nptr;
                prevTrainer = nptr;
            }

            ch = getc(fp);
        }
    }

    printf("\nTRAINERS: \n");
    printTrainerList(thead);

    fp = fopen("requests.txt", "r");
    if (fp != NULL)
    {
        ch = fgetc(fp);

        while (ch != EOF)
        {
            int len;
            Slot* nptr = (Slot*) malloc(sizeof(Slot));
            fscanf(fp, "%d", &nptr->time);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->memberID);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->trainerID);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->preferredTrainer);
            ch = fgetc(fp);
            
            fscanf(fp, "%d", &nptr->program);
            ch = fgetc(fp);
            
            fgets(nptr->timestamp, 15, fp);
            len = strlen(nptr->timestamp);
            if (nptr->timestamp[len - 1] == '\n')
                nptr->timestamp[len - 1] = '\0';

            
            if (prevRequest == NULL)
            {
                rshead = nptr;
                prevRequest = nptr;
            }
            else
            {
                prevRequest->next = nptr;
                prevRequest = nptr;
            }

            ch = getc(fp);
        }
    }

    printf("\nREQUESTS: \n");
    printSlotList(rshead);

    fp = fopen("assigned.txt", "r");
    if (fp != NULL)
    {
        ch = fgetc(fp);

        while (ch != EOF)
        {
            int len;
            Slot* nptr = (Slot*) malloc(sizeof(Slot));
            fscanf(fp, "%d", &nptr->time);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->memberID);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->trainerID);
            ch = fgetc(fp);

            fscanf(fp, "%d", &nptr->preferredTrainer);
            ch = fgetc(fp);
            
            fscanf(fp, "%d", &nptr->program);
            ch = fgetc(fp);
            
            fgets(nptr->timestamp, 15, fp);
            len = strlen(nptr->timestamp);
            if (nptr->timestamp[len - 1] == '\n')
                nptr->timestamp[len - 1] = '\0';

            if (prevAssigned == NULL)
            {
                ashead = nptr;
                prevAssigned = nptr;
            }
            else
            {
                prevAssigned->next = nptr;
                prevAssigned = nptr;
            }

            ch = getc(fp);
        }
    }

    printf("\nASSIGNED: \n");
    printSlotList(ashead);
    
    do
    {
        printf("\nMake a choice\n");
        printf("1. Add member\n2. Add trainer\n3. Schedule slot\n4. Print hour-wise list\n5. Delete member\n6. Delete trainer\n7. Search member\n8. Search trainer\n9. Print member list\n10. Print trainer list\n11. Print sorted remuneration list\n12. Print intersection list\n13. Quit\n");
        scanf("%d", &choice);

        if (choice == 1)
            addMember(&mhead);
        else if (choice == 2)
            addTrainer(&thead);
        else if (choice == 3)
            scheduleSlot(thead, &rshead, &ashead);
        else if (choice == 4)
            printHourWiseList(ashead);
        else if (choice == 5)
            deleteMember(&mhead);
        else if (choice == 6)
            deleteTrainer(&thead);
        else if(choice == 7)
            searchMember(mhead);
        else if (choice == 8)
            searchTrainer(thead);
        else if (choice == 9)
            printMemberList(mhead);
        else if (choice == 10)
            printTrainerList(thead);
        else if (choice == 11)
            printSortedRemunList(thead);
        else if(choice == 12)
            printIntersecList(thead, rshead, ashead);
    } while(choice != 13);

    fp = fopen("members.txt", "w");
    mptr = mhead;
    while (mptr != NULL)
    {
        fprintf(fp, "\n%d\n", mptr->id);
        fprintf(fp, "%s\n", mptr->name);
        fprintf(fp, "%d\n", mptr->age);
        fprintf(fp, "%s\n", mptr->gender);
        
        for(int i = 0; i < 4; i++)
            fprintf(fp, "%d ", mptr->enrolled[i]);
        fprintf(fp, "\n");
        
        for(int i = 0; i < 4; i++)
            fprintf(fp, "%d ", mptr->amountPaid[i]);
        fprintf(fp, "\n");
        
        mptr = mptr->next;
    }

    fp = fopen("trainers.txt", "w");
    tptr = thead;
    while (tptr != NULL)
    {
        fprintf(fp, "\n%d\n", tptr->id);
        fprintf(fp, "%s\n", tptr->name);
        
        for(int i = 0; i < 4; i++)
            fprintf(fp, "%d ", tptr->offered[i]);
        fprintf(fp, "\n");
        
        fprintf(fp, "%d\n", tptr->remuneration);
        
        tptr = tptr->next;
    }

    fp = fopen("trainers.txt", "w");
    tptr = thead;
    while (tptr != NULL)
    {
        fprintf(fp, "\n%d\n", tptr->id);
        fprintf(fp, "%s\n", tptr->name);
        
        for(int i = 0; i < 4; i++)
            fprintf(fp, "%d ", tptr->offered[i]);
        fprintf(fp, "\n");
        
        fprintf(fp, "%d\n", tptr->remuneration);
        
        tptr = tptr->next;
    }

    fp = fopen("requests.txt", "w");
    rptr = rshead;
    while (rptr != NULL)
    {
        fprintf(fp, "\n%d\n", rptr->time);
        fprintf(fp, "%d\n", rptr->memberID);
        fprintf(fp, "%d\n", rptr->trainerID);
        fprintf(fp, "%d\n", rptr->preferredTrainer);
        fprintf(fp, "%d\n", rptr->program);
        fprintf(fp, "%s\n", rptr->timestamp);
        
        rptr = rptr->next;
    }

    fp = fopen("assigned.txt", "w");
    aptr = ashead;
    while (aptr != NULL)
    {
        fprintf(fp, "\n%d\n", aptr->time);
        fprintf(fp, "%d\n", aptr->memberID);
        fprintf(fp, "%d\n", aptr->trainerID);
        fprintf(fp, "%d\n", aptr->preferredTrainer);
        fprintf(fp, "%d\n", aptr->program);
        fprintf(fp, "%s\n", aptr->timestamp);
        
        aptr = aptr->next;
    }
    
    fclose(fp);

    mptr = mhead;
    while (mptr != NULL)
    {
        prevMember = mptr;
        mptr = mptr->next;
        free(prevMember);
    }

    tptr = thead;
    while (tptr != NULL)
    {
        prevTrainer = tptr;
        tptr = tptr->next;
        free(prevTrainer);
    }

    rptr = rshead;
    while (rptr != NULL)
    {
        prevRequest = rptr;
        rptr = rptr->next;
        free(prevRequest);
    }

    aptr = ashead;
    while (aptr != NULL)
    {
        prevAssigned = aptr;
        aptr = aptr->next;
        free(prevAssigned);
    }
    
}


Trainer* partition(Trainer* lptr)
{
    Trainer* nptr, * slow, * fast;
    slow = lptr;
    fast = lptr->next->next;
    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL)
            fast = fast->next;
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}

Trainer* merge(Trainer* lptr, Trainer* nptr, int state)
{
    Trainer* merged, * ptr1, * ptr2, * ptr3;
    ptr1 = lptr;
    ptr2 = nptr;
    if ((state == 0 && ptr1->id < ptr2->id) || (state == 1 && ptr1->remuneration > ptr2->remuneration))
    {
        merged = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        merged = ptr2;
        ptr2 = ptr2->next;
    }
    
    ptr3 = merged;
    while (ptr1 != NULL && ptr2 != NULL)
    {
        if ((state == 0 && ptr1->id < ptr2->id) || (state == 1 && ptr1->remuneration > ptr2->remuneration))
        {
            ptr3 = ptr3->next = ptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            ptr3 = ptr3->next = ptr2;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1 != NULL)
        ptr3->next = ptr1;
    else
        ptr3->next = ptr2;
    return merged;
}

Trainer* mergeSort(Trainer* lptr, int state)
{
    Trainer* nptr;
    if (lptr != NULL && lptr->next != NULL)
    {
        nptr = partition(lptr);
        lptr = mergeSort(lptr, state);
        nptr = mergeSort(nptr, state);
        lptr = merge(lptr, nptr, state);
    }
    return lptr;
}
