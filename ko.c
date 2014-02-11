#include <stdlib.h>
#include <stdbool.h>
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
			if(bestillingOpp[i] || bestillingInne[i])
				return i;
		}

	}

	else
	{
		for(int j = etasje; j >= 0; j--)
		{
			if(bestillingNed[j] || bestillingInne[j])
				return j;
		}
	}

	return -1;
}

void leggTilOrdre(int etasje, bool inne, bool opp)
{
	if(inne == true)
		bestillingInne[etasje] = true;
	
	else if(inne == false && opp == true)
		bestillingOpp[etasje] = true;
	
	else if(inne == false && opp == false)
		bestillingNed[etasje] = true;

	settLys();
	nyOrdre();
}

bool finnestOrdreInniHeis()
{
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		if(bestillingInne[i] == true)
			return true;
	}
	return false;
}

void settLys()
{
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		elev_set_button_lamp(BUTTON_COMMAND, i, bestillingInne[i]);
		
		if(i != 3)
			elev_set_button_lamp(BUTTON_CALL_UP, i, bestillingOpp[i]);
		
		if(i !=0)
			elev_set_button_lamp(BUTTON_CALL_DOWN, i, bestillingNed[i]);
	}
}

bool erKoTom(){
	for(int i = 0; i < ANTALL_ETASJER; i++)
	{
		if(bestillingInne[i] || bestillingOpp[i] || bestillingNed[i])
			return false;
	}
	return true;
}
