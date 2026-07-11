#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

struct User {
	char un[30];
	char pass[24];
	int role;          //0 for user and 1 for admin
	int fileperm[10];
	int failedattempts;
};

typedef struct
{
	int day;
	int mon;
	int year;
	int hour;
	int min;
}timenow;

struct Logs {
    timenow t;
    char un[30];
    char action[40];
    char status[15];
};

struct File {
	char fn[30];
	char content[500];
};

struct Logs auditLogs[200];
int logCount = 0;

struct User database[10] = {
	{
		"hadeed", "kdghhg5338", 1, {2,2,2,2,0,0,0,0,0,0}, 0  //hadeed2005
	},
	{
		"hassan", "fdwv456", 0, {1,2,0,1,0,0,0,0,0,0}, 0  //cats123
	},
	{
		"dr.mazhar", "frpvdwv456", 1, {2,2,2,2,0,0,0,0,0,0}, 0  //comsats123
	},
	{
		"atif_aslam", "WxMddqhQd456", 0, {0,1,0,2,0,0,0,0,0,0}, 0  //TuJaaneNa123
	},
};

int totalUsers = 4;

struct File files[10] = {
    {"Salaries.txt", "Hadeed: 6000\nHassan: 5000\nDr.Mazhar: 500000\nAtif Aslam: 350000"}, 
    {"PfProjectHacks.txt", "There are no hacks, WORK HARD!!"},
    {"AdminNotes.txt", "System maintenance should be done ASAP or you will fail"}, 
    {"Public.txt", "Welcome to the system!\nProject made by hadeed and hassan"} 
};

int totalFiles = 4;

void displayMainMenu();
int login();
void adminDashboard(int);
void userDashboard(int);
void viewProfile(int);
int isPassValid(char []);
void changePass(int);
void encryptPass(char []);
void accessFiles(int);
void readFile(int, int);
void editFile(int, int);
void registerUser(int);
int isUsernameUnique(char []);
void viewAllUsers();
void delUser(int);
void makeAdmin(int);
void userUnlock(int);
void createFile(int);
void delFile(int);
void logActivity(char [], char [], char []);
void viewLogs();
timenow currentTime();
void saveData();
void saveFiles();
void saveLogs();
void loadData();
void loadFiles();
void loadLogs();


int main() {
	loadData();
	loadFiles();
	loadLogs();
	displayMainMenu();
	return 0;
}


void displayMainMenu() {
	int n, cu;
	while(1) {
		system("cls");
		printf("----------Main Menu----------\n");
		printf("[1]Login\n[2]Register\n[3]Exit\n");
		scanf("%d", &n);
		fflush(stdin);
		
		switch(n) {
			case 1:
				cu = login();
    
    			if (cu != -1) { 
        			if (database[cu].role == 1) {
            			adminDashboard(cu);
        			} 
					else {
            			userDashboard(cu);
        			}
    			}
    			break;
			case 2:
				registerUser(cu);
				break;
			case 3:
				exit(0);
			default:
				printf("Invalid Input\nPlease try again!\n");
		}
	}
}

int login() {
	int n, i;
	char inuser[30];
	char inpass[24];
	while(1) {
		system("cls");
		printf("----Enter your Username (or 0 to go back)----\n");
		fgets(inuser, 30, stdin);
		inuser[strcspn(inuser, "\n")] = 0;
		if (strcmp(inuser, "0") == 0) {
			return -1;
		}
		fflush(stdin);
		
		int found = -1;
		for (i = 0; i < totalUsers; i++) {
			if(strcmp(inuser, database[i].un) == 0) {
				found = i;
				break;
			}
		}
		if (found != -1) {
			if (database[found].failedattempts >= 3) {
				printf("Account Locked!\nSee Admin.\n");
				system("pause");
				return -1; 
			}
			
			printf("\n----Enter your Password----\n");
			
			// --- This is the Password Masking Feature ---

			int p = 0;
			char ch;
			while(1) {
				ch = _getch();
				
				if (ch == 13) {
					inpass[p] = '\0';
					break;
				} else if (ch == 8) { 
					if (p > 0) {
						p--;
						printf("\b \b"); 
					}
				} else if (p < 23) { 
					inpass[p] = ch;
					p++;
					printf("*");
				}
			}
			printf("\n");
			// --------------------------------

			encryptPass(inpass);

			if(strcmp(inpass, database[found].pass) == 0) {
				logActivity(database[found].un, "Login_Attempt", "Success");
				database[found].failedattempts = 0;
				return found;
			} else {
				printf("Password does not match!\n");
				database[found].failedattempts++;
				logActivity(database[found].un, "Login_Attempt", "Fail");
				system("pause");
			}
		} else {
			printf("Username does not exist!\nPlease try again\n");
			system("pause");
		}
	}
}
		
void userDashboard(int cu) {
	int n;
	while(1) {
		system("cls");
		printf("--------Welcome back %s !--------\n",database[cu].un);
		printf("[1]View Profile\n[2]Change Pass\n[3]Access Files\n[4]Back\n");
		scanf("%d", &n);
		fflush(stdin);
		
		switch(n) {
			case 1:
				viewProfile(cu);
				break;
			case 2:
				changePass(cu);
				break;
			case 3:
				accessFiles(cu);
				break;
			case 4:
				return;
			default:
				printf("Invalid Input\nPlease try again!\n");	
		}
	}
}		

void viewProfile(int cu) {
	char displaypass[24];
	system("cls");
	printf("----Username----\n%s\n",database[cu].un);
	strcpy(displaypass, database[cu].pass);
	for(int i = 0; displaypass[i] != '\0'; i++) {
		displaypass[i] = displaypass[i] - 3;
	}
	printf("----Password----\n%s\n",displaypass);
	printf("----Role----\n");
	switch(database[cu].role) {
		case 0:
			printf("User\n");
			break;
		case 1:
			printf("Admin\n");
			break;
	}
	printf("----Failed Login Attempts----\n%d\n",database[cu].failedattempts);
	logActivity(database[cu].un, "Viewed_Profile", "Success");
	system("pause");
}

void encryptPass(char pass[]) {
    int i;
    for(i = 0; pass[i] != '\0'; i++) {
        pass[i] = pass[i] + 3;
    }
}

int isPassValid(char p[]) {
    int hasDigit = 0;

    if (strlen(p) < 5) {
        printf("Password must be at least 5 characters long!\n");
        return 0;
    }

    for (int i = 0; i < strlen(p); i++) {
        if (isdigit(p[i])) {
            hasDigit = 1;
            break;
        }
    }

    if (hasDigit == 0) {
        printf("Password must contain at least one number!\n");
        return 0;
    }
    return 1;
}

void changePass(int cu) {
	char oldp[30];
	char newp[30];
	char confp[30];
	while(1) {
		system("cls");
		printf("Enter your old password\n");
		
		fgets(oldp, 30, stdin);
		oldp[strcspn(oldp, "\n")] = 0;
		fflush(stdin);
		encryptPass(oldp);
		
		if(strcmp(oldp, database[cu].pass) != 0) {
			printf("Password does not match\nPlease try again\n");
			logActivity(database[cu].un, "Password_Updated", "Fail");
		} else {
			printf("Enter New Password\n");
			fgets(newp, 30, stdin);
			newp[strcspn(newp, "\n")] = 0;
			fflush(stdin);
			
			printf("Confirm New Password\n");
			fgets(confp, 30, stdin);
			confp[strcspn(confp, "\n")] = 0;
			fflush(stdin);
			
			if (strcmp(newp, confp) != 0) {
				printf("New passwords do not match!\n");
				system("pause");
				continue;
			}
			
			if(isPassValid(newp) == 1) {
				encryptPass(newp);
				strcpy(database[cu].pass, newp);
				logActivity(database[cu].un, "Password_Update", "Success");
				saveData();
				return;
			} else {
                printf("New password does not meet requirements!\n");
                logActivity(database[cu].un, "Password_Update", "Fail");
                system("pause");
			}
		}
	}
}
	
void accessFiles(int cu) {
		system("cls");
		printf("--------Files Available--------\n");
		
		for(int i = 0; i < totalFiles; i++) {
    		if(database[cu].fileperm[i] > 0) { 
        	printf("[%d]%s\n", i + 1, files[i].fn);
    		}
		}
		
		printf("Enter number of the file you want to access\n");
		int n,m,x;
		scanf("%d", &n);
		fflush(stdin);
		
		if (n <= 0 || n > totalFiles) {
    		printf("Invalid file selection!\n");
    		logActivity(database[cu].un, "Accessed_File", "Fail");
    		system("pause");
    		return;
			}
			
		int cf = n - 1;
		
		while(1) {
			system("cls");
			if(database[cu].fileperm[cf] == 0) {
				printf("Access Denied\nChoose Another File");
				logActivity(database[cu].un, "Accessed_File", "Fail");
				system("pause");
				return;
			}
			else if(database[cu].fileperm[cf] == 1) {
				printf("Access Granted\n[1]Read\n[2]Back\n");
				logActivity(database[cu].un, "Accessed_File", "Success");
				scanf("%d", &x);
				
				switch(x) {
					case 1:
						readFile(cf, cu);
						break;
					case 2:
						return;
					default:
						printf("Invalid Input\nPlease try again!");	
				}
			}
			else if(database[cu].fileperm[cf] == 2) {
				printf("Access Granted\n[1]Read\n[2]Edit\n[3]Back\n");
				logActivity(database[cu].un, "Accessed_File", "Success");
				scanf("%d", &m);
				
				switch(m) {
					case 1:
						readFile(cf, cu);
						break;
					case 2:
						editFile(cf, cu);
						break;
					case 3:
						return;
					default:
						printf("Invalid Input\nPlease try again!");
				}
			}
		}
}

void readFile(int cf, int cu) {
	system("cls");
	if (files[cf].content[0] == '\0') {
        printf("File empty\n");
    } else {
        printf("%s\n", files[cf].content);
    }
    logActivity(database[cu].un, "File_Read", "Success");
    system("pause");
}

void editFile(int cf, int cu) {
	system("cls");
	char inp[500];
	int n;
	printf("[1]Overwrite\n[2]Add to the file\n[3]Back\n");
	
	while(1) {
		scanf("%d", &n);
		fflush(stdin);
		
		switch(n) {
			case 1:
				printf("\nEnter data:\n");
				fgets(inp, sizeof(inp), stdin);
				inp[strcspn(inp, "\n")] = 0;
				fflush(stdin);
				strcpy(files[cf].content, inp);
				logActivity(database[cu].un, "File_Overwrite", "Success");
				saveFiles();
				return;	
			case 2:
				printf("\nEnter data:\n");
				fgets(inp, sizeof(inp), stdin);
				inp[strcspn(inp, "\n")] = 0;
				fflush(stdin);
				strcat(files[cf].content, "\n");
				strcat(files[cf].content, inp);
				logActivity(database[cu].un, "Added_To_File", "Success");
				saveFiles();
				return;	
			case 3:
				return;	
			default:
				printf("Invalid Input\nPlease try again!");
		}
	}
}

void registerUser(int cu) {
	system("cls");
	char inpun[30];
	char inppass[24];
	char confpass[24];
	
	while(1) {
	if (totalUsers >= 10) {
		printf("User limit reached!\n");
		system("pause");
		return;
	} else {
		printf("Enter your username \n");
		fgets(inpun, sizeof(inpun), stdin);
		inpun[strcspn(inpun, "\n")] = 0;
		fflush(stdin);
		
		if (isUsernameUnique(inpun) == 1) {
			printf("Enter your password:\n");
			fgets(inppass, sizeof(inppass), stdin);
			inppass[strcspn(inppass, "\n")] = 0;
			fflush(stdin);
			
			printf("Confirm your password:\n");
                fgets(confpass, sizeof(confpass), stdin);
                confpass[strcspn(confpass, "\n")] = 0;
                fflush(stdin);
                
            if (strcmp(inppass, confpass) != 0) {
                    printf("Passwords do not match! Please try again!\n");
                    logActivity(database[cu].un, "Register_user", "Fail");
                    system("pause");
                    continue;
                }    
			
			if (isPassValid(inppass) == 1) {
				encryptPass(inppass);
				strcpy(database[totalUsers].pass, inppass);
                strcpy(database[totalUsers].un, inpun);
                database[totalUsers].role = 0;
                database[totalUsers].failedattempts = 0;

                for(int i = 0; i < 10; i++) {
                    database[totalUsers].fileperm[i] = 0;
                }
                database[totalUsers].fileperm[3] = 1;
                logActivity(database[totalUsers].un, "Register_user", "Success");
                totalUsers++;
                saveData();
                saveLogs();
				printf("Registration successful!\n");
                system("pause");
                return;
			} else {
				printf("Invalid Password\nTry Again\n");
				logActivity(database[cu].un, "Register_user", "Fail");
			}
		} else {
			printf("Username already exists!\nTry again\n");
			logActivity(database[cu].un, "Register_user", "Fail");
		}
	}
	}
}

int isUsernameUnique(char name[]) {
	for (int i = 0; i < totalUsers; i++) {
        if (strcmp(name, database[i].un) == 0) {
            return 0;
        }
    }
    return 1;
}

void adminDashboard(int cu) {
    int n;
    while(1) {
        system("cls");
		printf("[1]View all User\n[2]Delete any User\n[3]Make Admin\n[4]Create File\n[5]Delete File\n[6]Reset any User Failed attempts\n[7]View System Logs\n[8]Access Files\n[9]Back\n");
		scanf("%d", &n);
        fflush(stdin);
		
        switch(n) {
            case 1: viewAllUsers();
				break;
            case 2: delUser(cu);
				break;
			case 3: makeAdmin(cu);
				break;
            case 4: createFile(cu);
				break;
            case 5: delFile(cu);
				break;
            case 6: userUnlock(cu); 
				break;
            case 7: viewLogs(); 
				break;
			case 8: accessFiles(cu);
				break;	
			case 9: return;
            default: printf("Invalid choice\n");
			system("pause");
        }
    }
}

void viewAllUsers() {
	system("cls");
    printf("ID\tUsername\tRole\tFailedAttempts\tPermissions\n");
    printf("----------------------------------------------------------------------\n");
	for(int i = 0; i < totalUsers; i++)
	{
		printf("[%d]\t%-15s\t", i + 1, database[i].un);

        if (database[i].role == 1) {
            printf("Admin\t");
        } else {
            printf("User\t");
        }
        printf("%d\t\t", database[i].failedattempts);

        for (int j = 0; j < totalFiles; j++) {
            printf("%d ", database[i].fileperm[j]);
        }
        
        printf("\n"); 
    }
    printf("----------------------------------------------------------------------\n");
    system("pause");
}

void delUser(int cu) {
	system("cls");
	printf("ID\tUsername\tRole\n");
	printf("----------------------------------------------------------------------\n");
	for(int j=0 ;j < totalUsers;j++)
	{
		printf("[%d]\t%-15s\t",j + 1,database[j].un);
		
		if(database[j].role == 1)
		{ printf("Admin\t"); }
		else
		{ printf("User\t"); }
		printf("\n");
	}
	
    int index;
    printf("Enter User ID to delete: ");
    scanf("%d", &index);
    index--; 

    if(index < 0 || index >= totalUsers) {
        printf("Invalid ID\n");
        logActivity(database[cu].un, "Delete_User", "Fail");
        system("pause");
        return;
    }

    // Shifting the users left
    for(int i = index; i < totalUsers - 1; i++) {
        database[i] = database[i+1];
    }
    totalUsers--;
    printf("User deleted successfully\n");
    saveData();
    logActivity(database[cu].un, "Delete_User", "Success");
    system("pause");
}

void makeAdmin(int cu) {
	system("cls");
	printf("ID\tUsername\tRole\n");
	printf("----------------------------------------------------------------------\n");
	for(int j=0 ;j < totalUsers;j++)
	{
		printf("[%d]\t%-15s\t",j + 1,database[j].un);
		
		if(database[j].role == 1)
		{ printf("Admin\t"); }
		else
		{ printf("User\t"); }
		printf("\n");
	}
	
    int index;
    printf("Enter User ID to make them Admin: ");
    scanf("%d", &index);
    index--; 

    if(index < 0 || index >= totalUsers) {
        printf("Invalid ID\n");
        logActivity(database[cu].un, "Gave_Admin_Permission", "Fail");
        system("pause");
        return;
    }
    
    if (database[index].role == 1) {
    	printf("This user is already an Admin\n");
    	logActivity(database[cu].un, "Gave_Admin_Permission", "Fail");
    	system("pause");
	} else {
		database[index].role = 1;
		for (int i = 0; i < 10; i++) {
			database[index].fileperm[i] = 2;
		}
    	printf("User %s has been made an Admin!\n", database[index].un);
    	saveData();
    	logActivity(database[cu].un, "Gave_Admin_Permission", "Success");
    	system("pause");
	}
}

void userUnlock(int cu) {
	system("cls");
	
	printf("ID\tUsername\tRole\nFailed Attempts");
	printf("----------------------------------------------------------------------\n");
	for(int j=0 ;j < totalUsers;j++)
	{
		printf("[%d]\t%-15s\t",j + 1,database[j].un);
		
		if(database[j].role == 1)
		{ printf("Admin\t"); }
		else
		{ printf("User\t"); }
		printf("%d\t",database[j].failedattempts);
		printf("\n");
	}
	
    int index;
    printf("Enter User ID to unlock: ");
    scanf("%d", &index);
    fflush(stdin);
    index--;

    if(index >= 0 && index < totalUsers) {
        database[index].failedattempts = 0;
        printf("User %s has been unlocked\n", database[index].un);
        saveData();
        logActivity(database[cu].un, "Unlock_User", "Success");
    } else {
        printf("Invalid User ID\n");
    }
    saveData();
    system("pause");
}

void createFile(int cu) {
	system("cls");
    int n;
    if(totalFiles >= 10) {
        printf("The storage is full, delete a file first\n");
        system("pause");
        return;
	}   
    printf("Enter file name: ");
    fgets(files[totalFiles].fn, 30, stdin);
    files[totalFiles].fn[strcspn(files[totalFiles].fn, "\n")] = 0;
    printf("[1] Write content now\n[2] Leave empty\nSelection: ");
    scanf("%d", &n);
    fflush(stdin);

    if(n == 1) {
        printf("Write content here:\n");
        fgets(files[totalFiles].content, 500, stdin);
    } else {
        files[totalFiles].content[0] = '\0';
    }
    
    database[0].fileperm[totalFiles] = 2; 
    totalFiles++;
    printf("File created successfully\n");
    saveFiles();
    logActivity(database[cu].un, "Create_File", "Success");

    system("pause");
}

void delFile(int cu) {
	system("cls");
    int i, index;
    
    printf("--- Current Files ---\n");
    for(i = 0; i < totalFiles; i++) {
        printf("[%d] %s\n", i + 1, files[i].fn);
    }
    printf("[0] Back\n");
    printf("Enter File ID to delete: ");
    scanf("%d", &index);
    fflush(stdin);
    index--;
    
    if(index == 0) {
    	return;
    index--;
	}
	
    if(index < 0 || index >= totalFiles) {
        printf("Invalid File ID\n");
        system("pause");
        return;
    }

    //shifting the file array
    for(i = index; i < totalFiles - 1; i++) {
        files[i] = files[i + 1];
    }

    //shift permissions for all users
    for(int u = 0; u < totalUsers; u++) {
        for(int p = index; p < totalFiles - 1; p++) {
            database[u].fileperm[p] = database[u].fileperm[p+1];
        }
        database[u].fileperm[totalFiles - 1] = 0;
    }

    totalFiles--;
    printf("File deleted and permissions updated\n");
    saveFiles();
    logActivity(database[cu].un, "Delete_File", "Success");
    system("pause");
}

void logActivity(char un[30], char action[], char status[]) {
    if (logCount < 200) {
        auditLogs[logCount].t = currentTime();
        strcpy(auditLogs[logCount].un, un);
        strcpy(auditLogs[logCount].action, action);
        strcpy(auditLogs[logCount].status, status);
        logCount++;
    } else {
        printf("Log database full\n");
        system("pause");
    }
    saveLogs();
}

void viewLogs() {
	system("cls");
	printf("Time/Date\t\tUsername\t\tAction\t\tStatus\n");
	for(int i = 0; i < logCount; i++) {
        printf("%02d:%02d|%02d/%02d/%04d\t", auditLogs[i].t.hour, auditLogs[i].t.min, auditLogs[i].t.day, auditLogs[i].t.mon, auditLogs[i].t.year);
        printf("%-15s\t", auditLogs[i].un);
        printf("%-20s\t", auditLogs[i].action);
        printf("%-10s\n", auditLogs[i].status);
    }
        if(logCount==0) {
      		printf("No Logs Available\n");
		}
    system("pause");
}

timenow currentTime() {
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

void saveData() {
	FILE *dptr = fopen("users.txt", "w");
	if (dptr != NULL) {
		fprintf(dptr, "%d\n", totalUsers);
		for (int i = 0; i < totalUsers; i++) {
			fprintf(dptr, "%s  %s  %d  %d\n", database[i].un, database[i].pass, database[i].role, database[i].failedattempts);
			for (int j = 0; j < 10; j++) {
				fprintf(dptr, "%d ", database[i].fileperm[j]);
			}
		}
		fprintf(dptr, "\n");
	}
	fclose(dptr);
}

void saveFiles() {
	FILE *fptr = fopen("files.txt", "w");
	
	if (fptr != NULL) {
		fprintf(fptr, "%d\n", totalFiles);
		for (int i = 0; i < totalFiles; i++) {
			fprintf(fptr, "%s\n%s\n|\n", files[i].fn, files[i].content);
		}
		fclose(fptr);
	}
}

void saveLogs() {
	FILE *lptr = fopen("auditlog.txt", "w");
	if (lptr != NULL) {
		for (int i = 0; i < logCount; i++) {
			fprintf(lptr, "%d %d %d %d %d %s %s %s\n", auditLogs[i].t.day, auditLogs[i].t.mon, auditLogs[i].t.year,
			auditLogs[i].t.hour, auditLogs[i].t.min, auditLogs[i].un, auditLogs[i].action, auditLogs[i].status);
		}
	fclose(lptr);
	}	
}

void loadData() {
	FILE *dptr = fopen("users.txt", "r");
	if(dptr == NULL) {
		return;
	}
	
	fscanf(dptr, "%d", &totalUsers);
	for (int i = 0; i < totalUsers; i++) {
		fscanf(dptr, "%s  %s  %d  %d", database[i].un, database[i].pass, &database[i].role, &database[i].failedattempts);
		for (int j = 0;  j < 10; j++) {
			fscanf(dptr, "%d ", &database[i].fileperm[j]);
		}
	}
	fclose(dptr);
}

void loadFiles() {
	FILE *fptr = fopen("files.txt", "r");
	if (fptr == NULL) {
		return;
	}
	
	char temp[500];
	fscanf(fptr, "%d\n", &totalFiles);
	
	for(int i = 0; i < totalFiles; i++) {
        files[i].content[0] = '\0';
        fgets(files[i].fn, 30, fptr);
        files[i].fn[strcspn(files[i].fn, "\n")] = 0;
        
        while(fgets(temp, sizeof(temp), fptr)) {
            if (strcmp(temp, "|\n") == 0 || strcmp(temp, "|") == 0) {
                break;
            }
            strcat(files[i].content, temp);
        }
    }
	fclose(fptr);
}

void loadLogs() {
	FILE *lptr = fopen("auditlog.txt", "r" );
	if (lptr == NULL) {
		return;
	}
	logCount = 0;
	
	while(logCount < 200 && fscanf(lptr, "%d %d %d %d %d %s %s %s\n", &auditLogs[logCount].t.day, &auditLogs[logCount].t.mon,
		&auditLogs[logCount].t.year, &auditLogs[logCount].t.hour, &auditLogs[logCount].t.min, auditLogs[logCount].un,
		auditLogs[logCount].action, auditLogs[logCount].status) != EOF) {
		logCount++;
	}
	fclose(lptr);
}


