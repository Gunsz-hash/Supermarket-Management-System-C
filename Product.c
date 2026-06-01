#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"
#include "FileHelper.h"
#include "myMacros.h"

#define MIN_DIG 3
#define MAX_DIG 5

static const char* typeStr[eNofProductType] = { "Fruit Vegtable", "Fridge", "Frozen", "Shelf" };
static const char* typePrefix[eNofProductType] = { "FV", "FR", "FZ", "SH" };


void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	generateBarcode(pProduct);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	initDate(&pProduct->expiryDate);
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH);
		myGets(pProduct->name, sizeof(pProduct->name), stdin);
	} while (checkEmptyString(pProduct->name));
}

int		saveProductToFile(const Product* pProduct, FILE* fp)
{
	if (fwrite(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error saving product to file\n");
		return 0;
	}
	return 1;
}

int		loadProductFromFile(Product* pProduct, FILE* fp)
{
	if (fread(pProduct, sizeof(Product), 1, fp) != 1)
	{
		puts("Error reading product from file\n");
		return 0;
	}
	return 1;
}

int	compareProductsByName(const void* prod1, const void* prod2)
{
	const Product* pProd1 = *(Product**)prod1;
	const Product* pProd2 = *(Product**)prod2;

	return strcmp(pProd1->name, pProd2->name);
}

int	compareProductsByCount(const void* prod1, const void* prod2)
{
	const Product* pProd1 = *(Product**)prod1;
	const Product* pProd2 = *(Product**)prod2;

	return pProd1->count - pProd2->count;
}

int	compareProductsByPrice(const void* prod1, const void* prod2)
{
	const Product* pProd1 = *(Product**)prod1;
	const Product* pProd2 = *(Product**)prod2;

	if (pProd1->price > pProd2->price)
		return 1;
	if (pProd1->price < pProd2->price)
		return -1;
	return 0;
}

void printProduct(const Product* pProduct)
{
	RETURN_CHECK_0(pProduct);

	char* dateStr = getDateStr(&pProduct->expiryDate);
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %13d %7s %15s\n", typeStr[pProduct->type], pProduct->price, pProduct->count, " ", dateStr);
	free(dateStr);
}

void printProductPtr(void* v1)
{
	Product* pProduct = *(Product**)v1;
	printProduct(pProduct);
}

void generateBarcode(Product* pProd)
{
	RETURN_CHECK_0(pProd);

	char temp[BARCODE_LENGTH + 1];
	int barcodeNum;

	strcpy(temp, getProductTypePrefix(pProd->type));
	do {
		barcodeNum = MIN_BARCODE + rand() % (RAND_MAX - MIN_BARCODE + 1); //Minimum 5 digits
	} while (barcodeNum > MAX_BARCODE);

	sprintf(temp + strlen(temp), "%d", barcodeNum);

	strcpy(pProd->barcode, temp);
}

void getBarcodeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg, "Code should be of %d length exactly\n"
		"Must have %d type prefix letters followed by a %d digits number\n"
		"For example: FR20301",
		BARCODE_LENGTH, PREFIX_LENGTH, BARCODE_DIGITS_LENGTH);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts("Invalid barcode length");
			ok = 0;
		}
		else
		{
			//check first PREFIX_LENGTH letters are upper case and valid type prefix
			char* typeSubStr = (char*)malloc(PREFIX_LENGTH + 1);
			if (!typeSubStr)
				return;
			strncpy(typeSubStr, temp, PREFIX_LENGTH);
			typeSubStr[PREFIX_LENGTH] = '\0';
			int prefixOk = 0;
			int i;

			for (i = 0; i < eNofProductType; i++)
			{
				if (strcmp(typeSubStr, typePrefix[i]) == 0)
				{
					prefixOk = 1;
					break; //found valid type prefix
				}
			}

			free(typeSubStr); //free the allocated memory

			if (!prefixOk)
			{
				puts("Invalid type prefix");
				ok = 0;
			}
			else
			{
				for (i = PREFIX_LENGTH; i < BARCODE_LENGTH; i++)
				{
					if (!isdigit(temp[i]))
					{
						puts("Only digits after type prefix\n");
						puts(msg);
						ok = 0;
						break;
					}
					digCount++;
				}

				if (digCount != BARCODE_DIGITS_LENGTH)
				{
					puts("Incorrect number of digits");
					ok = 0;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

eProductType getProductType()
{
	int option;

	printf("\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);

	getchar();

	return (eProductType)option;
}

const char* getProductTypeStr(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

const char* getProductTypePrefix(eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typePrefix[type];
}

int isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

void updateProductCount(Product* pProduct)
{
	int count;

	do {
		printf("How many items to add to stock? ");
		scanf("%d", &count);
	} while (count < 1);

	pProduct->count += count;
}


void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}




/////////////////////////////

int compressPriceAndQuantity(int price, int quantity, char* file) {
	int shekels = price / 100;
	int cents = price % 100;

	if (shekels > 512 || cents > 99) {
		printf("Price out of range!\n");
		return 0;
	}
	BYTE data[3] = { 0 };
	data[0] = (quantity) & 0xFF;
	data[1] = (cents & 0x7F) | ((shekels & 0x03) << 7);
	data[2] = (shekels >> 2) & 0x7F;

	if (fwrite(data, sizeof(BYTE), 3, file) != 3) {
		fclose(file);
		return 0;  // Writing error
	}
	return 1;
}
int	saveProductToCompressedFile(char* file, const Product* product) {
	if (!file || !product)    //// is it nececcery ?
		return 0;
	/*	FILE* file = fopen(fileName, "wb");*/
	/*	if (!file)
			return 0;  */                            // AZ12345

	int nameLength = strlen(product->name);
	if (nameLength == 0) {
		return 0;  // Invalid name
	}

	BYTE data[4] = { 0 };
	data[0] = ((product->barcode[2]) & 0xF << 4) | product->barcode[3] & 0xF;
	data[1] = ((product->barcode[4]) & 0xF << 4) | product->barcode[5] & 0xF;
	data[2] = ((product->barcode[6] & 0xF) << 4) | ((product->type & 0x3) << 2) | ((product->name[0] >> 6) & 0x3);

	data[3] = (product->name[0] & 0x3) << 6;

	if (fwrite(data, sizeof(BYTE), 4, file) != 4) {
		fclose(file);
		return 0;  // Writing error
	}

	if (fwrite(product->name, sizeof(char), nameLength, file) != nameLength) {
		fclose(file);
		return 0;  // Writing error
	}

	compressPriceAndQuantity(product->price, product->count, file);

	compressDate(&(product->expiryDate), file);
	return 1;
}


int loadProductFromCompressedFile(FILE* file, Product* product) {
	BYTE dataP[4] = { 0 };

	if (fread(&dataP, sizeof(BYTE), 4, file) != 4)
		return 0;

	char digits[6] = { 0 };
	digits[0] = ((dataP[0] >> 4) & 0x0F) + '0'; // Barcode number 1
	digits[1] = (dataP[0] & 0x0F) + '0';       // Barcode number 2
	digits[2] = ((dataP[1] >> 4) & 0x0F) + '0'; // Barcode number 3
	digits[3] = (dataP[1] & 0x0F) + '0';       // Barcode number 4
	digits[4] = ((dataP[2] >> 4) & 0x0F) + '0'; // Barcode number 5



	int type = (dataP[2] >> 2) & 0x03;

	int nameLen = (dataP[3] >> 6) & 0x3 | (dataP[2] & 0x3) << 2;
	char* name = readStringFromFileWithLen(file, nameLen, "Error reading supermarket name\n");
	char* prefix = typePrefix[type];
	char* bcode = (char*)calloc(8, sizeof(char));
	if (!bcode)
		return 0;

	strcpy(bcode, prefix);
	strcat(bcode, digits);
	strcpy(product->barcode, bcode);
	strcpy(product->name, name);
	product->type = (eProductType)type;

	BYTE dataC[3] = { 0 };

	if (fread(&dataC, sizeof(BYTE), 3, file) != 3)
		return 0;
	product->count = dataC[0];
	product->price = (dataC[2] | ((dataC[1] & 0x1) << 8)) + (((float)(dataC[1] >> 1)) / 100);

	BYTE dataD[2] = { 0 };

	if (fread(&dataD, sizeof(BYTE), 2, file) != 2)
		return 0;

	product->expiryDate.day = dataD[0] >> 3;
	product->expiryDate.month = ((dataD[0] & 0x7) << 1) | (dataD[1] >> 7);
	product->expiryDate.year = ((dataD[1] >> 4) & 0x7) + 2024;

	return 1;
}
