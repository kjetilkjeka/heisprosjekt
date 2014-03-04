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
	
	if(etasje == ANTALL_ETASJER -1)
	  bestillingNed[etasje] = false;

	if(etasje == 0)
	  bestillingOpp[etasje] = false;

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

		for(int i = ANTALL_ETASJER-1; i >= 0; i--)
		{
		  if(bestillingNed[i] || bestillingInne[i])
		    return i;
		}

		for(int i = 0; i < etasje; i++)
		{
			if(bestillingOpp[i])
				return i;
		}
	}

	else
	{
	        for(int i = etasje; i >= 0; i--)
		{
			if(bestillingNed[i] || bestillingInne[i])
				return i;
		}

		for(int i = 0; i < ANTALL_ETASJER; i++)
		{
			if(bestillingOpp[i] || bestillingInne[i])
				return i;
		}

		for(int i = ANTALL_ETASJER; i >= etasje; i--)
		{
			if(bestillingNed[i])
				return i;
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

