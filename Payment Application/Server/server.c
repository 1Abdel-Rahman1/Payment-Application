#include "server.h"

#define DB_SIZE	10

uint8_t transactionCounter = 0;
int8_t accountIndex = -1;
ST_accountsDB_t* referencePtr = 0;

ST_accountsDB_t accountsDB[255] = {
	{2000.0, RUNNING, "8989374615436851"},
	{100000.0, BLOCKED, "5807007076043875"},
	{7500.0, RUNNING, "4716961258588441"},
	{3000.0, BLOCKED, "4539539167569762"},
	{1000.0, RUNNING, "4532709416977390"},
	{5000.0, BLOCKED, "4678173989174907"},
	{90000.0, RUNNING, "4716360493317213"},
	{11500.0, RUNNING, "4916545416147030"},
	{60000.0, BLOCKED, "4006657966722657"},
	{40000.0, RUNNING, "4916999120208804"}
};

ST_transaction_t transactionDB[255];	// Uninitialized global array is initially filled with Zeros.

// This function will take all transaction data and validate its data, it contains all server logic.
EN_transState_t receiveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t reference;
	
	if (isValidAccount(&transData->cardHolderData, &reference) == ACCOUNT_NOT_FOUND)
	{
		//transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}
	if (isAmountAvailable(&transData->terminalData, &reference) == LOW_BALANCE)
	{
		//transData->transState = DECLINED_INSUFFICIENT_FUND;
		return DECLINED_INSUFFICIENT_FUND;
	}
	if (isBlockedAccount(&reference) == BLOCKED_ACCOUNT)
	{
		//transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		//transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}

	//transData->transState = APPROVED;
//	accountsDB[accountIndex].balance -= transData->terminalData.transAmount;
	return APPROVED;
}

// This function will take card data and validate if the account related to this card exists or not.
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference)
{
	for (uint8_t i = 0; i < DB_SIZE; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			*accountReference = accountsDB[i];
			return SERVER_OK;
		}
	}
	//accountReference = NULL;
	return ACCOUNT_NOT_FOUND;
}

// This function takes a reference to the account into the database and verifies if it is blocked or not.
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountReference)
{
	if (accountReference->state == RUNNING)
	{
		return SERVER_OK;
	}
	else
	{
		return BLOCKED_ACCOUNT;
	}
}

// This function will take terminal data and a reference to the account in the database and check if the account has a sufficient amount to withdraw or not.
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountReference)
{
	if (termData->transAmount > accountReference->balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

// This function will store all transaction data in the transactions database.
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	if (transactionCounter < 255)
	{
		transactionDB[transactionCounter].cardHolderData = transData->cardHolderData;
		transactionDB[transactionCounter].terminalData = transData->terminalData;
		transactionDB[transactionCounter].transState = transData->transState;
		transactionDB[transactionCounter].transactionSequenceNumber = transactionCounter;
		transactionCounter++;
		return SERVER_OK;
	}
	else
	{
		return SAVING_FAILED;
	}
}

// This function prints all transactions found in the transactions DB.
void listSavedTransactions(void)
{
	printf("#########################\n");
	printf("Transaction Sequence Number: %d\n", transactionDB->transactionSequenceNumber);
	printf("Transaction Date: %s\n", transactionDB->terminalData.transactionDate);
	printf("Transaction Amount: %.2f\n", transactionDB->terminalData.transAmount);
	if (transactionDB->transState == APPROVED)
	{
		printf("Transaction State: APPROVED\n");
	}
	else if (transactionDB->transState == DECLINED_INSUFFICIENT_FUND)
	{
		printf("Transaction State: DECLINED_INSUFFICIENT_FUND\n");
	}
	else if (transactionDB->transState == DECLINED_STOLEN_CARD)
	{
		printf("Transaction State: DECLINED_STOLEN_CARD\n");
	}
	else if (transactionDB->transState == FRAUD_CARD)
	{
		printf("Transaction State: FRAUD_CARD\n");
	}
	else if (transactionDB->transState == INTERNAL_SERVER_ERROR)
	{
		printf("Transaction State: INTERNAL_SERVER_ERROR\n");
	}
	printf("Terminal Max Amount: %.2f\n", transactionDB->terminalData.maxTransAmount);
	printf("Cardholder Name: %s\n", transactionDB->cardHolderData.cardHolderName);
	printf("PAN: %s\n", transactionDB->cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", transactionDB->cardHolderData.cardExpirationDate);
	printf("#########################\n");
}