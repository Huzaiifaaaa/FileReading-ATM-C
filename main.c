#include<stdio.h>
#include<stdlib.h>//including the respective header files, responsible for some basic function like printf,scanf...

//typedef client
//has 4 variables
//stores client data
//user defined data type
typedef struct client//client typedef, user defined types, stores client information
{
	char name[20];
	char email[20];
	char phone[20];
	int balance;
}client;//end


//user defined function declarations, tells compiler about functions their arguments & return types
//c requires function declaration like this
//function declared with arguments & return types
int input(client **c, char *fName, int *l);
void list(client *c, int l);
int transaction(client *c, int l);
int search(client *c, int l, char *nev);
int stringLength(char *n);
int newClient(client **c, int *l);
int emailCheck(char *eTemp);
int phoneCheck(char *pTemp);
void copy(char *fromHere, char *toHere);
void quit(client *c, char *fName, int l);
int overAgain();
int options();


//main menu, execution starts here
//no command line arguments
//asks user for file input name
//prints options
//selects case based on option
//returns int
int main()//main function, execution starts here
{
	client *clients = NULL;
    char fileName[100];
    int length = 0;
    int again;//variable declarations & initialization

	while(again = input(&clients, fileName, &length))
	{
 	    if(again == 2)
        {
             return 0;
        }
	}

	while(1)//looping infinite times
	{
		switch(options()) //switch case, depending upon return value from option function
		{
			case 1://if 1 returned
			    {
			        list(clients, length);//calling list function
                    break;//break, stops execution & jumps to line 80
			    }
			case 2://if 2 returned
			    {
			        while(transaction(clients, length));//calling transaction function
                    break;//break, stops execution & jumps to line 80
			    }
			case 3://if 3 returned
			    {
			        while(newClient(&clients, &length));//calling newClient function
                    break;//break, stops execution & jumps to line 80
			    }
			case 4://if 4 returned
			    {
			        quit(clients, fileName, length);//calling quit function
                    return 0;//returning zero
			    }
			default://if wrong value entered i.e., other then 1,2,3,4
			    {
			        printf("There are 4 options only: 1, 2, 3 or 4.\n");//printing message
			    }
		}	//end of switch case
	}//end of while
}//end of main


//takes no arguments
//prints menu/options
//ask for input
//clears console
//returns option selected(int)
int options()//function option, prints menu/options, takes nothing,returns int
{
    int option;//declaring variable

    printf("\nYou can choose the following options:\n\n");//printing on screen
    printf("1: client list / 2: transaction / 3: registration / 4: close the program\n\n");

    scanf("%d", &option);//taking user input

    system("clear");//clearing the screen/console

    return option;//returning option
}//end of function


//takes 3 arguments, pointer to c, fName & 1
//asks user to save changes or not
// if "yes->writes everything in file
//deallocates memory
//returns nothing
void quit(client *c, char *fName, int l)//quit function, prompts user for quitting, takes 3 arguments, returns nothing
{
	FILE *fp = NULL;//file pointer, points to file
    char save;
    int i;//creating & declaring variables

	printf("The client list may have changed.\n");//printing message
	printf("Do you want to save changes? (y / n) ");//prompting user
	scanf(" %c", &save);//taking user input

	if(save == 'y')//if "y" entered
    {
		fp = fopen(fName, "w");//opening file
		for(i = 0; i < l; i++)//writing in file
		{
			fprintf(fp, "%-20s%-20s%-20s\t%d\n", c[i].name, c[i].email, c[i].phone, c[i].balance);
		}

		fflush(fp);//clearing buffer
		fclose(fp);//closing the file
	}//end of if

	free(c);//deallocating memory

	printf("Program exits.\n");//printing message
}//end of function


//takes 2 arguments
//asks for email--> validates email
//ask for phone-->validates phone
//checks if storage available or not
//asks for name & balance
//store it in array
//returns int
int newClient(client **c, int *l)//function newClient, takes 2 arguments & returns int
{
	char emailTemp[20];
	char phoneTemp[20];//declaring variables(array size 20->MAX 20 Letters/numbers)

	printf("Email: ");//asking for email
    scanf("%s", emailTemp);//taking input

	if(emailCheck(emailTemp))//validating email
	{
        printf("ERROR: email address is invalid.\n");//if invalid email address entered
        if(overAgain())//again asking for input
        {
            return 1; //returning 1
        }
        else
        {
            return 0;//returning 0
        }
	}//end of outer if

	printf("Phone: ");//asking for phone
    scanf("%s", phoneTemp);//taking input

	if(phoneCheck(phoneTemp))//validating phone
	{
        printf("ERROR: phone number is invalid.\n");//if invalid phone entered
        if(overAgain())//again asking for input
        {
             return 1; //returning 1
        }
        else//returning 0
        {
            return 0;
        }
	}//end of outer if


	if(!(*c = (client *)realloc(*c, ++(*l)*sizeof(client))))
	{
        printf("ERROR: there is no memory enough\n");//if array full
        if(overAgain())//again asking for input
        {
             return 1;//returning 1
        }
        else
        {
             return 0; //returning 0
        }
	}//end of if

	copy(emailTemp, (*c)[*l-1].email);//calling copy function & passing arguments
	copy(phoneTemp, (*c)[*l-1].phone);//calling copy function & passing arguments

	printf("Name: ");//name prompt
    scanf("%s", (*c)[*l-1].name);//taking input & storing it in dynamic array

	printf("Balance: ");//balance prompt
    scanf("%d", &(*c)[*l-1].balance);//taking input & storing it in dynamic array

    return 0;//returning zero
}//end of function


//Takes  2 arguments,both pointers
//copies data from one memory address to other
//copies from fromHere to toHere
//done by looping
//returns nothing
void copy(char *fromHere, char *toHere)//function copy, moves data from one location to other, takes 2 arguments, returns nothing
{
	int i = 0;//declaring & initializing variable
	while(fromHere[i])//looping
    {
        toHere[i] = fromHere[i];
        i++;//incrementing
    }
	toHere[i] = 0;
}//end of function


//Takes  1 argument, pointer
//checks for character "." & "@" in the input
//counts "." & "@"
//returns 1 if ".@ comes before "@"
//also checks some other things as well
//returns 1-> if valid
//returns 0-> if invalid
//returns int
int emailCheck(char *eTemp)//function to validate email, takes 1 argument, returns int
{
	int i = 0;
	int dotIndex;
	int atIndex;
	int numberOfDots = -1;
	int numberOfAts = -1;
	int diff;//declaring & initializing variables

	while(eTemp[i])//looping
	{
		if(eTemp[i] == '.')//checking for "." in email input
        {
            dotIndex = i;//setting dot index
            numberOfDots++;//counting dots
        }
		if(eTemp[i] == '@')//checking for "@" in email input
        {
            atIndex = i;//setting "@"t index
            numberOfAts++;//counting @'s
        }
		i++;//incrementing
	}//end of while

	if((diff = dotIndex - atIndex) < 0)//checking which "." & "@" comes first
    {
         return 1; //returning 1
    }
	if(numberOfDots || numberOfAts)
    {
        return 1;//returning 1
    }
	if(!dotIndex || !atIndex || dotIndex == i-1 || atIndex == i-1 || diff == 1)
    {
        return 1; //returning 1
    }

	return 0;//returning 0
}//end of function


//Takes  1 argument, pointer
//checks for character in the input
//returns 1-> if valid
//returns 0-> if invalid
//returns int
int phoneCheck(char *pTemp)//function to validate phone, tales phone number, returns int
{
	int i = 0;//declaring variables

	while(pTemp[i])//looping
	{
		if(pTemp[i] < '0' ||	pTemp[i] > '9')//validating that only numbers/digits are entered
        {
            return 1;
        }
		i++;//incrementing
	}//end of while

	return 0;//returning 0
}//end of function


//Takes 1 arguments, i.e, pointer
//Returns int
//counts for character in input array
//returns length
int stringLength(char *n)//function to get string length, gets string, returns int
{
	int i = 0;//declaring variable

	while(n[i])
    {
         i++; //incementing
    }
	return i;//returning length
}//end of function


//Takes 2 arguments, i.e, pointer,1 ,name
//Returns int
//looks for user in array
//if found returns index
//else -1
//compares input name with the names available in array
//returns value
int search(client *c, int l, char *name)//function search, takes 3 arguments, returns int
{
	int i;
	int j;
	int nameLength = stringLength(name);//declaring & initializing variables

	for(i = 0; i < l; i++)//looping
	{
		if(nameLength == stringLength(c[i].name))//matching the name length
		{
			j = 0;//declaring variable
			while(j < nameLength && name[j] == c[i].name[j])//comparing corresponding characters
			{
				j++;//incrementing
			}
			if(j == nameLength)//if length is same
            {
                return i;//returning index
            }
		}//end of outer if
	}//end of for

	return -1;//returning -1
}//end of function


//Takes 2 arguments, i.e, pointer & 1...
//Returns int
//Asks user for his name(sender)
//looks for his name in array
//if not found-->asks again
//Asks user for receipient name(receiver)
//looks for his name in array
//if not found-->asks again
//takes amount input
//compares with the available balance
//if less--> asks again
//else transfers to receipient account
//returns value
int transaction(client *c, int l)//function transaction, takes 2 arguments, returns int
{
	char transactorName[20];
	char recipientName[20];
    int transactorIndex;
    int recipientIndex;
    int amount;//declaring variables

	printf("Transactor's name: ");//printing message
    scanf("%s", transactorName);//taking input

	if((transactorIndex = search(c, l, transactorName)) == -1)//checking if already exists
	{
        printf("ERROR: name cannot be found.\n");//printing message
        if(overAgain())//calling function
        {
            return 1; //returning 1
        }
        else
        {
             printf("Program exits.\n");//printing message
             return 2;//returning 2
        }
	}//end of outer if

	printf("Recipient's name: ");//printing message
    scanf("%s", recipientName);//taking input

	if((recipientIndex = search(c, l, recipientName)) == -1)//checking if receipt exists
	{
        printf("ERROR: name cannot be found.\n");//printing message
	         if(overAgain())//calling function
        {
            return 1; //returning 1
        }
        else
        {
             printf("Program exits.\n");//printing message
             return 2;//returning 2
        }
	}//end of outer if

	printf("Amount: ");//printing message
    scanf("%d", &amount);//user input

	if(c[transactorIndex].balance < amount)//checking if account has enough balance
	{
        printf("ERROR: funds is not enough.\n");//less balance
        if(overAgain())//calling function to ask again
        {
            return 1; //returning 1
        }
        else
        {
             printf("Program exits.\n");//printing message
             return 2;//returning 2
        }
	}//end of outer if

	c[transactorIndex].balance -= amount;//detecting amount from senders account
	c[recipientIndex].balance += amount;//adding amount to recipient account

	printf("Amount has been transferred.\n");//printing message
	return 0;//returning zero
}//end of function


//Takes 2 argument, i.e,  pointer...
//Returns nothing/void
//reads from array
//prints on screen
//looped 1 times
//returns nothing
void list(client *c, int l)//function list, takes 2 arguments, returns nothing
{
	int i;//declaring variable
	for(i = 0; i < l; i++)//printing single line, loops 1 time
	{
		printf("Name: %-20s email: %-20s phone: %-20s balance: %d\n", c[i].name, c[i].email, c[i].phone, c[i].balance);//printing
	}
}//end of function


//Takes 3 argument, i.e, typedef struct, pointer to file ...
//Returns int
//Asks user for file name
//opens the file
//if not opened--> file does not exist-->asks again
//reads file till END OF FILE
//checks if array is full or not
//reads from file
//closes the file
//returns value
int input(client **c, char *fName, int *l)//function input, takes 3 arguments, returns int, reads file
{
	FILE *fp = NULL;//file pointer
    char ch;
    int i;//declaring variables

	printf("Type the name of the text file you want to open: ");//asking for file
	 scanf("%s", fName);//taking iput

	if(!(fp = fopen(fName, "r")))//file not opened, i.e file pointer returns NULL
	{
        printf("ERROR: file cannot be opened. (Its path may be wrong).\n");//printing message
        if(overAgain())//calling function
        {
            return 1; //returning 1
        }
        else
        {
             printf("Program exits.\n");//printing message
             return 2;//returning 2
        }
	}//end of outer if

	while((ch = fgetc(fp)) != EOF)//reading till end of file
	{
		if(ch == '\n')//new line(checking or escape character)
        {
             (*l)++;//increment
        }
	}

	if(!(*c = (client *)malloc(*l*sizeof(client))))//checking if dynamic memory is empty or not
	{
	    printf("ERROR: there is no memory enough.\n");//printing message
        if(overAgain())//calling function
        {
            return 1; //returning 1
        }
        else
        {
             printf("Program exits.\n");//printing message
             return 2;//returning 2
        }
	}//end of outer if

	rewind(fp);//setting  the file position to the beginning of the file for the stream pointed to by stream

	for(i = 0; i < *l; i++)//reading from filr
	{
		fscanf(fp, "%s%s%s%d", (*c)[i].name, (*c)[i].email, (*c)[i].phone, &(*c)[i].balance);
	}

	fclose(fp);//closing file
	printf("Data have been read into the list.\n");//printing mesage

	return 0;//returning 0
}//end of function


//Takes No argument
//Returns int
//Asks user for repeating the process & takes input
//If "y", entered-->repeat the process-->returns 1
//else--> does not repeat-->returns 0
int overAgain()//function over again. takes nothing, returns int
{
    char again;//declaring variable
    printf("Do you want to try again? (y / n) ");//printing message
    scanf(" %c", &again);//taking user input

    if(again == 'y')//if "y" entered
    {
        return 1; //returning 1
    }
    else
    {
        return 0;//returning 0
    }
}//end of function
