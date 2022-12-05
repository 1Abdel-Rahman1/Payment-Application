#include "app.h"

int main(void)
{
	// Application Starts
	appStart();

	return 0;
}

void appStart(void)
{
	// A state variable to check the process
	int8_t state = -1;

	// Card Module Operation
	ST_cardData_t* newUser = malloc(sizeof(ST_cardData_t));
	if (newUser == NULL)
	{
		printf("Not Enough Memory.\n");
		free(newUser);
		return;
	}

	if (getCardHolderName(newUser) == WRONG_NAME)
	{
		printf("Invalid Cardholder's Name.\n");
		free(newUser);
		return;
	}
	if (getCardExpiryDate(newUser) == WRONG_EXP_DATE)
	{
		printf("Invalid Card Expiry Date.\n");
		free(newUser);
		return;
	}
	if (getCardPAN(newUser) == WRONG_PAN)
	{
		printf("Invalid Card's Primary Account Number.\n");
		free(newUser);
		return;
	}
	// Successful Card Module operation
	state = CARD_SUCCESS;

	// Terminal Module Operation
	ST_terminalData_t* newTransaction = malloc(sizeof(ST_terminalData_t));
	if (newTransaction == NULL)
	{
		printf("Not Enough Memory.\n");
		free(newUser);
		return;
	}

	if (state == CARD_SUCCESS)
	{
		if (getTransactionDate(newTransaction) == WRONG_DATE)
		{
			printf("Invalid Transaction Date.\n");
			free(newTransaction);
			free(newUser);
			return;
		}
		if (isCardExpired(newUser, newTransaction) == EXPIRED_CARD)
		{
			printf("Declined Expired Card.\n");
			free(newTransaction);
			free(newUser);
			return;
		}
		if (setMaxAmount(newTransaction) == INVALID_MAX_AMOUNT)
		{
			printf("Invalid Maximum Transaction Amount.\n");
			free(newTransaction);
			free(newUser);
			return;
		}
		if (getTransactionAmount(newTransaction) == INVALID_AMOUNT)
		{
			printf("Invalid Transaction Amount.\n");
			free(newTransaction);
			free(newUser);
			return;
		}
		if (isBelowMaxAmount(newTransaction, newTransaction->maxTransAmount) == EXCEED_MAX_AMOUNT)
		{
			printf("Declined Exceeded Maximum Amount.\n");
			free(newTransaction);
			free(newUser);
			return;
		}
		// Successful Terminal Module operation
		state = TERMINAL_SUCCESS;
	}

	// Server Module Operation
	ST_transaction_t* serverData = malloc(sizeof(ST_transaction_t));
	if (serverData == NULL)
	{
		printf("Not Enough Memory.\n");
		free(newTransaction);
		free(newUser);
		return;
	}

	if (state == TERMINAL_SUCCESS)
	{
		// Assign the data to the server
		serverData->cardHolderData = *newUser;
		serverData->terminalData = *newTransaction;

		if (receiveTransactionData(serverData) == FRAUD_CARD)
		{
			printf("Declined Fraud Card.\n");
			free(serverData);
			free(newTransaction);
			free(newUser);
			return;
		}
		if (receiveTransactionData(serverData) == DECLINED_INSUFFICIENT_FUND)
		{
			printf("Declined Insufficient Fund.\n");
			free(serverData);
			free(newTransaction);
			free(newUser);
			return;
		}
		if (receiveTransactionData(serverData) == DECLINED_STOLEN_CARD)
		{
			printf("Declined Stolen Card.\n");
			free(serverData);
			free(newTransaction);
			free(newUser);
			return;
		}
		if (receiveTransactionData(serverData) == APPROVED)
		{
			printf("Transaction Approved.\n");
			free(serverData);
			free(newTransaction);
			free(newUser);
			return;
		}
	}
}
