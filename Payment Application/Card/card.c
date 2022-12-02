#include "card.h"

// This function will ask for the cardholder's name and store it into card data.
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Enter Cardholder's Name: ");
	
	gets(cardData->cardHolderName);

	if ((strlen(cardData->cardHolderName) < 20) || (strlen(cardData->cardHolderName) > 24))
	{
		return WRONG_NAME;
	}
	for (uint8_t i = 0, n = strlen(cardData->cardHolderName); i < n; i++)
	{
		if (isalpha(cardData->cardHolderName[i]) == 0)
		{
			if (cardData->cardHolderName[i] != ' ')
			{
				return WRONG_NAME;
			}
		}
	}
	return CARD_OK;
}

// This function will ask for the card expiry date and store it in card data.
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	printf("Enter Card Expiry Date: ");
	
	gets(cardData->cardExpirationDate);

	if ((cardData->cardExpirationDate[2] != '/') || (strlen(cardData->cardExpirationDate) != 5))
	{
		return WRONG_EXP_DATE;
	}
	for (uint8_t i = 0; i < 2; i++)
	{
		if (isdigit(cardData->cardExpirationDate[i]) == 0)
		{
			return WRONG_EXP_DATE;
		}
	}
	for (uint8_t i = 3; i < 5; i++)
	{
		if (isdigit(cardData->cardExpirationDate[i]) == 0)
		{
			return WRONG_EXP_DATE;
		}
	}
	return CARD_OK;
}

// This function will ask for the card's Primary Account Number and store it in card data.
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Enter Card's Primary Account Number: ");
	
	gets(cardData->primaryAccountNumber);

	if ((strlen(cardData->primaryAccountNumber) < 16) || (strlen(cardData->primaryAccountNumber) > 19))
	{
		return WRONG_PAN;
	}
	for (uint8_t i = 0, n = strlen(cardData->primaryAccountNumber); i < n; i++)
	{
		if (isdigit(cardData->primaryAccountNumber[i]) == 0)
		{
			return WRONG_PAN;
		}
	}
	return CARD_OK;
}