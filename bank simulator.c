#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int flag = 1;
int n = 0;
int name_index = -1;
int ft_name_index = -1;
void login();
void reset_pwd();
void final_db();

void receipt(float bal, float new_bal)
{
	system("cls");
	printf("\t\t ACKNOWLEDGEMENT\n");
	printf("\t\t ---------------- \n\n");
	printf("INITIAL BALANCE: %.2f\n\n", bal);
	printf("UPDATED BALANCE: %.2f\n\n", new_bal);
}

float deposit(float bal, float amt)
{
	bal += amt;
	return bal;
}

float withdraw(float bal, float amt)
{
	if (amt <= bal)
	{
		bal -= amt;
		return bal;
	}
	else
	{
		printf("ERROR");
		return bal;
	}
}

struct details
{
	char *username;
	char *pwd;
	float bal;
};
struct details s[100];

void database()
{
	// struct details s[100];
	FILE *fp = fopen("database.csv", "r");
	if (!fp)
		printf("Can't open file\n");

	else
	{
		// Here we have taken size of
		// array 1024 you can modify it
		char buffer[1024];

		int row = 0;
		int column = 0;
		n = 0;
		while (fgets(buffer, 1024, fp))
		{
			column = 0;
			row++;
			if (row == 1)
				continue;

			// Splitting the data
			char *value = strtok(buffer, ", ");

			while (value)
			{
				// Column 1
				if (column == 0)
				{
					// username
					s[n].username = strdup(value);
					// strdup - used to duplicate a string
					// Returns a pointer
					// Different from strcpy(); strcpy()--- will not allocate memory space to copy
					// strdup()--- occupies itself the memory space for copying the string to
				}

				// Column 2
				if (column == 1)
				{
					// pwd
					s[n].pwd = strdup(value);
				}

				// Column 3
				if (column == 2)
				{
					// balance
					s[n].bal = atof(strdup(value));
				}
				value = strtok(NULL, ", ");
				column++;
			}
			n++;
		}
	}
}

int checking(char *name1, char ch) //,char *ft_name)
{
	int flag = -1;
	for (int i = 0; i < n; i++)
	{
		if (ch == 'u')
		{
			if (strcmp(name1, s[i].username) == 0)
			{
				flag = i;
			}
		}

		if (ch == 'p')
		{
			if (strcmp(name1, s[i].pwd) == 0)
			{
				flag = i;
			}
		}
	}
	if (flag >= 0)
		return flag;
	else
		return flag;
}

float fund_transfer(int a, int b)
{
	if (a >= 0 && b >= 0)
	{
		printf("SUCCESS\n");
		float amt, initial_bal = 0.0;
		printf("\nEnter the amount to transfer:\n");
		scanf("%f", &amt);
		initial_bal = s[a].bal;
		s[a].bal = withdraw(s[a].bal, amt);
		if (s[a].bal != initial_bal)
		{
			s[b].bal = deposit(s[b].bal, amt);
			printf("\nSUCCESSFULL");
		}
	}
	else
		printf("\nINVALID username, ft unsuccessfull");

	return s[a].bal;
}

float transact(float bal)
{
	char *name, *ft_name, name1[30], ft_name1[30];
	int ch, a = 0, b = 0;
	float amt, new_bal;
	printf("Logged in Successfully\n\n");
	printf("Press 1 to deposit\n");
	printf("Press 2 to withdraw\n");
	printf("Press 3 to fund transfer\n");
	printf("Enter your choice:\n");
	scanf("%d", &ch);

	switch (ch)
	{
	case 1:
		printf("\n\nEnter the amount:\n");
		scanf("%f", &amt);
		new_bal = deposit(bal, amt);
		break;

	case 2:
		printf("\n\nEnter the amount:\n");
		scanf("%f", &amt);
		new_bal = withdraw(bal, amt);
		break;

	case 3:
		fflush(stdin);
		printf("Please enter your username: ");
		gets(name1);
		name = name1;

		fflush(stdin);
		printf("Please enter the receiver's username: ");
		gets(ft_name1);
		ft_name = ft_name1;

		a = checking(name, 'u');
		b = checking(ft_name, 'u');

		new_bal = fund_transfer(a, b);
		break;

	default:
		flag++;
		if (flag < 6)
		{
			printf("\n\nINVALID CHOICE\n\n");
			transact(bal);
		}
		else
		{
			printf("\n******EXITTING******");
			exit(0);
		}
	}

	return new_bal;
}

void login_password()
{
	int d = 0, attempts = 1;
	char *lnop;
	char lnop1[30];
	printf("Password: ");
user_pwd:
	if (attempts <= 5)
	{
		gets(lnop1);
		lnop = lnop1;
		d = checking(lnop, 'p');
		if (d >= 0)
		{
			printf("Login Successful");
			system("cls");
		}
		else
		{
			attempts++;
			if (attempts <= 5)
				printf("\nInvalid Password, Try again\nPassword: ");
			goto user_pwd;
		}
	}
	else
	{
		printf("\n\nRan out of tries\n\n");
		printf("******EXITTING*******");
		exit(0);
	}
}

void login()
{
	char *lno, lno1[30];
	int c = 0, index = -1;
	int attempts = 1;
	float new_bal_amt = 0.0, bal_amt = 0.0;
	fflush(stdin);
	printf("Log into your account---\n\n");
	printf("\nUsername: ");
	gets(lno1);
	lno = lno1;
	c = checking(lno, 'u');
	if (c != -1)
	{
		login_password();
		bal_amt = s[c].bal;

		new_bal_amt = transact(bal_amt);
		s[c].bal = new_bal_amt;
		printf("GENERATING THE RECEIPT\n\n");
		receipt(bal_amt, new_bal_amt);
	}
	else
		printf("\ninvalid username");
}

void final_db()
{
	FILE *fp3 = fopen("database.csv", "w");
	for (int i = 0; i < n; i++)
	{
		fprintf(fp3, "\n%-20s,%-20s,%.2f", s[i].username, s[i].pwd, s[i].bal);
	}
	fclose(fp3);
}

void main()
{
	int a = 0;
	database();
	system("cls");
	printf("------Welcome to bank simulator------");
	printf("\n");
	login();

	final_db();
}