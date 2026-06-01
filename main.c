#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"
#include "SuperFile.h"

int main()
{
	SuperMarket	market;

	srand((unsigned int)time(NULL));
	printf("Welcome to our Super Market\n");
	printf("Do you want to load the data from a compressed file or regular?\n");
	printf("1 - compressed file\n");
	printf("2 - regular binary file\n");
	int filechoice;
	scanf("%d", &filechoice);
	if (filechoice == 1)
	{
		if (!loadSuperMarketFromFileCompressed(&market, SUPER_FILE_COMPRESSED_NAME, CUSTOMER_FILE_NAME))
		{
			printf("error loading Super Market\n");
			return 0;
		}
	}
	else {
		if (!initSuperMarket(&market, SUPER_FILE_NAME, CUSTOMER_FILE_NAME))
		{
			printf("error init Super Market");
			return 0;
		}
	}

	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if (!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerManageShoppingCart:
			if (!manageShoppingCart(&market))
				printf("Error in shopping cart managment\n");
			break;

		case eSortProducts:
			sortProducts(&market);
			break;

		case eSearchProduct:
			findProduct(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case EXIT:
			/*printf("Bye bye\n");*/
			printMessage("Thank", "You", "For", "Shopping", "With", "Us", NULL);
			printf("\n\n");

			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	handleCustomerStillShoppingAtExit(&market);

	int choice;
	printf("do you want to save the supermarket to a compressed file or regular binary file?: \n");
	printf("1 - compressed file\n");
	printf("2 - regular binary file\n");
	
	scanf("%d", &choice);
	if (choice == 1)
	{
		if (!saveNameAndLengthToBinaryFileCompressed(SUPER_FILE_COMPRESSED_NAME,&market))
			printf("Error saving supermarket to compressed file\n");
		else
		{
			printf("saved supermarket to compressed file successfully\n");
		}
	}
	else
	{
		if (!saveSuperMarketToFile(&market, SUPER_FILE_NAME, CUSTOMER_FILE_NAME))
			printf("Error saving supermarket to file\n");
		else
		{
			printf("saved supermarket to file successfully\n");
		}
	}
	printf("Goodbye\n");
	freeMarket(&market);

	return 1;
}

int menu()
{
	int option;
	printf("\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

