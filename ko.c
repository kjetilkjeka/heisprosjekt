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
	

	// litt dirty sjekk om det finnest bedre måte senere
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

		for(int j = etasje; j < ANTALL_ETASJER; j++)
		{
		  if(bestillingNed[j])
		    return j;
		}

		for(int k = etasje; k >= 0; k--)
		{
			if(bestillingNed[k] || bestillingOpp[k] || bestillingInne[k])
				return k;
		}
	}

	else
	{
		for(int a = etasje; a >= 0; a--)
		{
			if(bestillingNed[a] || bestillingInne[a])
				return a;
		}

		for(int b = etasje; b >= 0; b--)
		{
			if(bestillingOpp[b])
				return b;
		}

		for(int c = etasje; c < ANTALL_ETASJER; c++)
		{
			if(bestillingNed[c] || bestillingOpp[c] || bestillingInne[c])
				return c;
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
