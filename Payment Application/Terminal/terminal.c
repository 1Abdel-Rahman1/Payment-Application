#include "terminal.h"

// This function will ask for the transaction date and store it in terminal data.
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Enter Transaction Date: ");
	
	gets(termData->transactionDate);

	if ((strlen(termData->transactionDate) != 10) || (termData->transactionDate[2] != '/') || (termData->transactionDate[5] != '/'))
	{
		return WRONG_DATE;
	}
	for (uint8_t i = 0; i < 2; i++)
	{
		if (isdigit(termData->transactionDate[i]) == 0)
		{
			return WRONG_DATE;
		}
	}
	for (uint8_t i = 3; i < 5; i++)
	{
		if (isdigit(termData->transactionDate[i]) == 0)
		{
			return WRONG_DATE;
		}
	}
	for (uint8_t i = 6; i < 10; i++)
	{
		if (isdigit(termData->transactionDate[i]) == 0)
		{
			return WRONG_DATE;
		}
	}
	return TERMINAL_OK;
}

// This function compares the card expiry date with the transaction date.
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	uint8_t transYear[3] = { termData->transactionDate[8], termData->transactionDate[9], '\0' };
	uint32_t tYear = atoi(transYear);

	uint8_t expYear[3] = { cardData->cardExpirationDate[3], cardData->cardExpirationDate[4], '\0' };
	uint32_t eYear = atoi(expYear);

	uint8_t transMonth[3] = { termData->transactionDate[3], termData->transactionDate[4], '\0' };
	uint32_t tMonth = atoi(transMonth);

	uint8_t expMonth[3] = { cardData->cardExpirationDate[0], cardData->cardExpirationDate[1], '\0' };
	uint32_t eMonth = atoi(expMonth);

	if (tYear < eYear)
	{
		return TERMINAL_OK;
	}
	else if (tYear > eYear)
	{
		return EXPIRED_CARD;
	}
	else
	{
		if (tMonth <= eMonth)
		{
			return TERMINAL_OK;
		}
		else
		{
			return EXPIRED_CARD;
		}
	}
}

// This function asks for the transaction amount and saves it into terminal data.
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	//uint8_t input[20];

	printf("Enter Transaction Amount: ");

	//gets(input);
	//termData->transAmount = atof(input);
	scanf_s("%f", &(termData->transAmount));

	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

// This function takes the maximum allowed amount and stores it into terminal data.
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	//uint8_t input[20];
	//gets(input);
	//termData->maxTransAmount = atof(input);

	printf("Set Maximum Transaction Amount: ");

	scanf_s("%f", &(termData->maxTransAmount));

	if (termData->maxTransAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

// This function compares the transaction amount with the terminal max allowed amount.
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	//maxAmount = termData->maxTransAmount;

	if (termData->transAmount > maxAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}