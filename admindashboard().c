#include<stdio.h>
#include<string.h>
#include<time.h>

typedef struct
{
	int day;
	int mon;
	int year;
	int hour;
	int min;
}timenow;

void admindashboard(int cu);
void viewalluser();
void deluser();
void resetfailedattempts();
void createfile();
void delfile();
void logactivity(char un[], char action[], char status[]);
timenow currenttime();
void viewlog();


void admindashboard(int cu) {
    int n;
    while(1) {
        system("cls");
        printf("[1]View all User\n[2]Delete any User\n[3]Create File\n[4]Delete File\n[5]Reset any User Failed attempts\n[6]View Login Activity\n[7]Back");
        scanf("%d", &n);
        fflush(stdin);

        if (n == 0) {
        	return;
		}
		
        switch(n) {
            case 1: viewalluser();
				break;
            case 2: deluser();
				break;
            case 3: createfile();
				break;
            case 4: delfile(); 
				break;
            case 5: resetfailedattempts(); 
				break;
            case 6: viewlog(); 
				break;
            default: printf("Invalid choice\n");
			system("pause");
        }
    }
}

void viewalluser() {
	system("cls");
    printf("ID\tUsername\tRole\tFailedAttempts\tPermissions\n");
    printf("----------------------------------------------------------------------\n");
	for(int i = 0; i < totalusers; i++)
	{
		printf("[%d]\t%-15s\t", i + 1, database[i].un);

        if (database[i].role == 1) {
            printf("Admin\t");
        } else {
            printf("User\t");
        }
        printf("%d\t\t", database[i].failedAttempts);

        for (int j = 0; j < totalfiles; j++) {
            printf("%d ", database[i].fileperm[j]);
        }
        
        printf("\n"); 
    }
    printf("----------------------------------------------------------------------\n");
    system("pause");
}

void deluser() {
    int index;
    printf("Enter User ID to delete: ");
    scanf("%d", &index);
    index--; 

    if(index < 0 || index >= totalusers) {
        printf("Invalid ID\n");
        system("pause");
        return;
    }

    // Shifting the users left
    for(int i = index; i < totalusers - 1; i++) {
        database[i] = database[i+1];
    }
    totalusers--;
    printf("User deleted successfully\n");
    system("pause");
}

void resetfailedattempts() {
    int index;
    printf("Enter User ID to unlock: ");
    scanf("%d", &index);
    index--;

    if(index >= 0 && index < totalusers) {
        database[index].failedAttempts = 0;
        printf("User %s has been unlocked\n", database[index].un);
    } else {
        printf("Invalid User ID\n");
    }
    system("pause");
}

void createfile() {
    int n;
    if(totalfiles >= 10) {
        printf("The storage is full, delete a file first\n");
        system("pause");
        return;
	}   
    printf("Enter file name: ");
    fgets(allFiles[totalfiles].fn, 30, stdin);
    allFiles[totalfiles].fn[strcspn(allFiles[totalfiles].fn, "\n")] = 0;
    printf("[1] Write content now\n[2] Leave empty\nSelection: ");
    scanf("%d", &n);
    fflush(stdin);

    if(n == 1) {
        printf("Write content here:\n");
        fgets(allFiles[totalfiles].content, 500, stdin);
    } else {
        allFiles[totalfiles].content[0] = '\0';
    }
    
    database[0].fileperm[totalfiles] = 2; 
    totalfiles++;
    printf("File created successfully.\n");
    system("pause");
}

void delfile() {
    int index;
    printf("Enter File ID to delete: ");
    scanf("%d", &index);
    index--;

    if(index < 0 || index >= totalfiles) {
        printf("Invalid File ID\n");
        system("pause");
        return;
    }

    //shifting the file array
    for(int i = index; i < totalfiles - 1; i++) {
        allFiles[i] = allFiles[i + 1];
    }

    //shift permissions for all users
    for(int u = 0; u < totalusers; u++) {
        for(int p = index; p < totalfiles - 1; p++) {
            database[u].fileperm[p] = database[u].fileperm[p+1];
        }
        database[u].fileperm[totalfiles - 1] = 0;
    }

    totalfiles--;
    printf("File deleted and permissions updated\n");
    system("pause");
}

void logactivity(char un[30], char action[], char status[]) {
    if (logCount < 100) {
        auditLogs[logCount].t = currenttime();
        strcpy(auditLogs[logCount].un, un);
        strcpy(auditLogs[logCount].action, action);
        strcpy(auditLogs[logCount].status, status);

        logCount++;
    } else {
        printf("Log database full!\n");
    }
}

timenow currenttime() {
    timenow t;
    time_t now;
    struct tm *current;
    time(&now);
    current = localtime(&now);
    
    t.day = current->tm_mday;
    t.mon = current->tm_mon + 1;
    t.year = current->tm_year + 1900;
    t.hour = current->tm_hour;
    t.min = current->tm_min;
    return t;
}

void viewlog() {
	system("cls");
	printf("Time/Date\t\tUsername\t\tAction\t\tStatus\n");
	for(int i = 0; i < logCount; i++)
    {
        printf("%02d:%02d|%02d/%02d/%04d\t", l[i].t.hour, l[i].t.min, l[i].t.day, l[i].t.mon, l[i].t.year);
        printf("%-15s\t", l[i].un);
        printf("%-20s\t", l[i].action);
        printf("%-10s\n", l[i].status);
    }
    system("pause");
}
