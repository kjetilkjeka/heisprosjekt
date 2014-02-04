#include <stdlib.h>
#include "heisstyring.h"
#include "elev.h"
#include "ko.h"

#define ANTALL_ETASJER 4

bool bestillingInne[ANTALL_ETASJER];
bool bestillingOpp[ANTALL_ETASJER];
bool bestillingNed[ANTALL_ETASJER];

void slettAlleOrdre()
{
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		bestillingInne[i] = false;
		bestillingOpp[i] = false;
		bestillingNed[i] = false;
	}

	settLys();
}

void slettOrdre(int etasje, bool opp)
{
	bestillingInne[etasje] = false;

	if (opp == true)
	{
		bestillingOpp[etasje] = false;
	}
	else
	{
		bestillingNed[etasje] = false;
	}

	settLys();
}

int hentNesteOrdre(int etasje, bool opp)
{
	if(opp == true)
	{
		for(int i = etasje; i < ANTALL_ETASJER; i++)
		{
			if(bestillingOpp[i] == true)
			{
				return i;
			}
		}

	}

	else if(opp == false)
	{
		for(int j = etasje; j > 0; j--)
		{
			if(bestillingNed[j] == true)
			{
				return j;
			}
		}
	}

	return 0;
}

void leggTilOrdre(int etasje, bool inne, bool opp)
{
	if(inne == true && opp == true)
	{
		bestillingInne[etasje] = true;
		bestillingOpp[etasje] = true;
	}
	
	else if(inne == true && opp == false)
	{
		bestillingInne[etasje] = true;
		bestillingNed[etasje] = true;
	}
	
	else if(inne == false && opp == true)
	{
		bestillingInne[etasje] = false;
		bestillingOpp[etasje] = true;
	}
	
	else if(inne == false && opp == false)
	{
		bestillingInne[etasje] = false;
		bestillingNed[etasje] = true;
	}

	settLys();
	//nyOrdre();
	//nyOrdreTomKo();
}

bool finnestOrdreInniHeis()
{
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		if(bestillingInne[i] == true)
		{
			return true;
		}
	}
	return false;
}

void settLys()
{
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		elev_set_button_lamp(BUTTON_COMMAND, i, bestillingInne[i]);

		elev_set_button_lamp(BUTTON_CALL_UP, i, bestillingOpp[i]);
		
		elev_set_button_lamp(BUTTON_CALL_DOWN, i, bestillingNed[i]);
	}
}
