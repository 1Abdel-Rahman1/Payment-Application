/* app.h */
#ifndef APP_H
#define APP_H

#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

// An extra enum to define application's state
typedef enum EN_appState_t
{
	CARD_SUCCESS, TERMINAL_SUCCESS
}EN_appState_t;

void appStart(void);

#endif /* End of app.h */