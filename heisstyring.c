#include <stdbool.h>
#include <stdio.h>
#include "timer.h"

typedef enum
{
    INIT, //0
    IDLE, //1
    BEVEGER, //2
    KLAR,//3
    OBSTRUKSJON, //4
    APNE_DORER, //5
    NODSTOPP, //6
    OBSTRUKSJON_STOPP //7
} tilstand_t;

typedef enum
{
  NY_ORDRE_TOM_KO = 1,
  TIME_OUT = 2,
  NOD_STOPP = 3,
  OBSTRUKSJON_PA = 4,
  OBSTRUKSJON_AV = 5,
  ETASJE_ANKOMMET = 6,
  NY_ORDRE = 7
} event_t;

tilstand_t tilstand1 = INIT;
tilstand_t tilstand2 = KLAR;
bool opp = true;
int etasje = 0;

void tilstandsMaskin1(event_t event)
{
    if(tilstand2 != KLAR)
	return;
    
    switch(tilstand1)
    {      	
    case(INIT):
	switch(event)
	{
	case(ETASJE_ANKOMMET):
	    slettAlleOrdre();
	    idleEntry();
	    break;
	}
	break;
	
    case(IDLE):
	switch(event)
	{
	case(NY_ORDRE):
	    if(etasje == hentNesteOrdre(etasje, opp))
	    {
		dorerEntry();
	    } else 
	    {	
		bevegerEntry();
	    }
	    break;	
	}
	break;
	
    case(BEVEGER):
	switch(event)
	{
	case(ETASJE_ANKOMMET):	
	    if(hentNesteOrdre(etasje, opp) == etasje)
	    {
		dorerEntry();
	    }
	}
	break;
	
    case(APNE_DORER):
	
	switch(event)
	{
	case(TIME_OUT):
	    if(hentNesteOrdre(etasje, opp) == -1)
	    {
		idleEntry();
	    } else if(hentNesteOrdre(etasje, opp) == etasje) 
	    {	
		slettOrdre(etasje, true);
		slettOrdre(etasje, false);
		dorerEntry();
	    } else 
	    {
		bevegerEntry();
	    }
	    
	    break;
	case(NY_ORDRE):
	    if(hentNesteOrdre(etasje, opp) == etasje)
	    {
		dorerEntry();
	    }
	}	
	break;
    }
}

void tilstandsMaskin2(event_t event)
{
    switch(tilstand2)
    {
    case(KLAR):
	switch(event)
	{
	case(NOD_STOPP):	
	    nodstopEntry();
	    break;
	case(OBSTRUKSJON_PA):
	    obstruksjonEntry();
	    break;
	}
	break;
	
    case(OBSTRUKSJON):
	switch(event)
	{	
	case(NOD_STOPP):
	    obstruksjonStoppEntry();
	    break;
	    
	case(OBSTRUKSJON_AV):
	    klarEntry(); 
	}
	break;
    	
    case(NODSTOPP):
	switch(event)
	{
	case(NY_ORDRE):	
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
	    {
		klarEntry();
	    } else 
	    {
		nodstopEntry();
	    }
	    break;
	case(OBSTRUKSJON_PA):
	    obstruksjonStoppEntry();
	}
	break;
	
    case(OBSTRUKSJON_STOPP):
	switch(event)
	{
	case(NY_ORDRE):
	    
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
		obstruksjonEntry();
	    else
		obstruksjonStoppEntry();
	    break;
	    
	case(OBSTRUKSJON_AV):
	    nodstopEntry();	
	}
	
	if(tilstand2 != NODSTOPP && tilstand2 != OBSTRUKSJON_STOPP)
	    elev_set_stop_lamp(false);
	
	break;
    }    
}

void idleEntry()
{
    elev_set_door_open_lamp(false);
    elev_set_speed(0);
    etasje = elev_get_floor_sensor_signal();
    tilstand1 = IDLE;
}

void dorerEntry()
{
    elev_set_door_open_lamp(true);
    elev_set_speed(0);
    slettOrdre(etasje, opp);
    startTimer();
    tilstand1 = APNE_DORER;
}

void klarEntry()
{
    elev_set_stop_lamp(false);
    if(tilstand1 == BEVEGER)
	bevegerEntry();
    if(tilstand1 == APNE_DORER)
	startTimer();
    tilstand2 = KLAR;
}

void nodstopEntry()
{
    elev_set_stop_lamp(true);
    slettAlleOrdre();
    elev_set_speed(0);
    tilstand2 = NODSTOPP;
}

void obstruksjonEntry()
{
    elev_set_stop_lamp(false);
    elev_set_speed(0);
    tilstand2 = OBSTRUKSJON;
}

void bevegerEntry()
{
    elev_set_door_open_lamp(false);
    int neste = hentNesteOrdre(etasje, opp);
    
    if(neste == etasje)
    {
	if(opp)
	{
	    opp = false;
	    elev_set_speed(-300);
	} else
	{
	    opp = true;
	    elev_set_speed(300);
	}
    } else if(neste > etasje)
    {
	elev_set_speed(300);
	opp = true;
    } else
    {
	elev_set_speed(-300);
	opp = false;
    }
    
    tilstand1 = BEVEGER;
}

void obstruksjonStoppEntry()
{
    slettAlleOrdre();
    elev_set_stop_lamp(true);
    tilstand2 = OBSTRUKSJON_STOPP;
}

void etasjeAnkommet(int e)
{
    etasje = e;
    tilstandsMaskin1(ETASJE_ANKOMMET);
}

void nyOrdre()
{
    tilstandsMaskin1(NY_ORDRE);
    tilstandsMaskin2(NY_ORDRE);
}

void obstruksjonPa()
{
    tilstandsMaskin2(OBSTRUKSJON_PA);
}

void obstruksjonAv()
{
    tilstandsMaskin2(OBSTRUKSJON_AV);
}

void nodStopp()
{
    tilstandsMaskin2(NOD_STOPP);
}

void timeOut()
{
    tilstandsMaskin1(TIME_OUT);
}

void initHeisstyring()
{
    elev_set_speed(100);
}
