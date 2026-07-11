# Secure File Management & Audit System

Hi there !! This is a console-based **File Management and Authentication System** built entirely in C.
This project originally started out as my **Programming Fundamentals Lab Final** at university. After finishing the basic requirements for class, I got really hooked on making it better, cleaner, and more secure.
I ended up developing it much further out of personal interest, diving deeper into C logic to add features like hidden password typing, account lockouts, and better ways to save data.


Features for this project:

**Password Masking :** Instead of showing your password on the screen when you type, the terminal hides it behind asterisks (`*`) just like a real login page. It also handles backspaces perfectly so you can fix typos.
**Two User Roles (Admin vs. Regular User):** The app behaves differently depending on who logs in. Admins have a special dashboard to manage users, unlock accounts, and create or delete files. Regular users get a clean dashboard to look at or edit files they have permission to access.
**Simple Password Encryption:** Passwords are never saved in plain text. The program uses a custom scrambling method to shift characters around, keeping user credentials safer before they are written to the disk.
**Account Lockout after 3 Strikes:** To prevent someone from guessing passwords over and over, the system automatically locks an account after 3 failed login attempts.
**Activity Logging (Audit Logs):** Every major action—like a successful login, a password change, or an admin deleting a user—gets written to an `auditlog.txt` file with a live timestamp so there is a history of everything that happened.
**Automatic Saving:** Everything you do; registering a user, changing a password, editing files, or tracking logs, gets completely saved to text files (`users.txt`, `auditlog.txt`) so your data is right there when you restart the program.


Important Notes & Environment:

**Language:** C (C99)
**Operating System:** Windows is required.
This project relies heavily on the Windows-specific `<conio.h>` library for the password masking loop and `system("cls")` to refresh the screen, so it needs a Windows environment to run properly.


How the Data is Kept:

The system manages its database using a mix of text files and internal memory structures:
`users.txt`: A text file that stores usernames, scrambled passwords, user roles, and their specific file access permissions.
**Internal File Structures (`struct File`):** The actual virtual files and their text contents are stored and updated inside a structured array directly within the program's memory.
`auditlog.txt`: A simple history log tracking all application events line-by-line with real-time data.
