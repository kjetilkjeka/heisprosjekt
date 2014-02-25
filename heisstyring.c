#include <stdbool.h>
#include <stdio.h>
#include "timer.h"

typedef enum
{
  INIT,
  IDLE,
  BEVEGER,
  KLAR,
  OBSTRUKSJON,
  APNE_DORER,
  NODSTOPP,
  OBSTRUKSJON_STOPP
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
bool apen = 0;

void foo1(event_t event)
{
    switch(tilstand1)
    {      
    case(INIT):
      initState(event);
      break;

    case(IDLE):
      idleState(event);
      break;

    case(BEVEGER):
      bevegerState(event);
      break;

    case(APNE_DORER):
      apneState(event);
      break;
    }

    elev_set_door_open_lamp(apen);


    printf("tilstand1 er: %d \n\r", tilstand1);
    printf("event er: %d \n\r", event);

}

void foo2(event_t event)
{
  switch(tilstand2)
    {
    case(KLAR):
      klarState(event);
      break;

    case(OBSTRUKSJON):
      obstruksjonState(event);
      break;
    	
    case(NODSTOPP):
      stoppState(event);
      break;
	
    case(OBSTRUKSJON_STOPP):
      obstruksjonStoppState(event);
      break;
    }
    
   
    
    printf("tilstand2 er: %d \n\r", tilstand2);
    printf("event er: %d \n\r", event);
}

void initState(event_t event)
{
      switch(event)
	{
	case(ETASJE_ANKOMMET):
	  elev_set_speed(0);
	  etasje = elev_get_floor_sensor_signal();
	  slettAlleOrdre();
	  tilstand1 = IDLE;
	  break;
	}
}

void idleState(event_t event)
{
	switch(event)
	{
	case(NY_ORDRE):
	    if(etasje == hentNesteOrdre(etasje, opp))
	    {
	      slettOrdre(etasje, opp);
	      apen = true;
	      tilstand1 = APNE_DORER;
	      startTimer();
	    } else {
		tilstand1 = BEVEGER;
		bevegerEntry();
	    }
	    break;	
	}
}


void klarEntry()
{
  if(tilstand1 == BEVEGER)
    bevegerEntry();
  if(tilstand1 == APNE_DORER)
    startTimer();
}

void klarState(event_t event)
{
  switch(event)
    {
    case(NOD_STOPP):	
      elev_set_speed(0);
      tilstand2 = NODSTOPP;
      break;
    case(OBSTRUKSJON_PA):
      elev_set_speed(0);
      tilstand2 = OBSTRUKSJON;
      break;
    }
}

void bevegerEntry()
{
  int neste = hentNesteOrdre(etasje, opp);
  printf("neste ordre er: %d\r\n", neste);
  
  if(neste == etasje) // dette har med obstruksjon å gjøre
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
}

      
void bevegerState(event_t event)
{	
	switch(event)
	{
	case(ETASJE_ANKOMMET):	
	  if(hentNesteOrdre(etasje, opp) == etasje)
	    {
	      slettOrdre(etasje, opp);
	      apen = true;
	      tilstand1 = APNE_DORER;
	      startTimer();
	    }
	}

	if(tilstand1 != BEVEGER)
	    elev_set_speed(0);
}

void obstruksjonState(event_t event)
{
	switch(event)
	{	
	case(NOD_STOPP):
	    tilstand2 = OBSTRUKSJON_STOPP;
	    break;

	case(OBSTRUKSJON_AV):
	  klarEntry();
	  tilstand2 = KLAR;
	}
}

void apneState(event_t event)
{
  if(tilstand2 != KLAR)
    return;

	switch(event)
	{
	case(TIME_OUT):
	  printf("hent neste ordre er %d \n\r", hentNesteOrdre(etasje, opp));
	  if(hentNesteOrdre(etasje, opp) == -1)
	    {
	      tilstand1 = IDLE;
	    } else if(hentNesteOrdre(etasje, opp) == etasje)
	    {	
	      slettOrdre(etasje, true);
	      slettOrdre(etasje, false);
	      apneState(TIME_OUT);
	      } else 
	      {
		bevegerEntry();
		tilstand1 = BEVEGER;
	      }
	
	    break;
	case(NY_ORDRE):
	  if(hentNesteOrdre(etasje, opp) == etasje)
	    {
	      slettOrdre(etasje, opp);
	      startTimer();	
	    }
	  //tilstand1 = APNE_DORER;
	  
	}


	if(tilstand1 != APNE_DORER && tilstand1 != NODSTOPP)
	  apen = false;
}

void stoppState(event_t event)
{
	switch(event)
	{
	case(NY_ORDRE):
	
	  if(finnestOrdreInniHeis() && !elev_get_stop_signal())
	    {
	      klarEntry();
	      tilstand2 = KLAR;
	    } else 
	    {
	      slettAlleOrdre();
	    }
	  break;
	case(OBSTRUKSJON_PA):
	  tilstand2 = OBSTRUKSJON_STOPP;
	}

	

	if(tilstand2 != NODSTOPP && tilstand2 != OBSTRUKSJON_STOPP)
	  elev_set_stop_lamp(false);


}


void obstruksjonStoppState(event_t event)
{
	switch(event)
	{
	case(NY_ORDRE):
	
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
		tilstand2 = OBSTRUKSJON;
	    else
	      slettAlleOrdre();

	break;
	case(OBSTRUKSJON_AV):
	    tilstand2 = NODSTOPP;	
	}

	if(tilstand2 != NODSTOPP && tilstand2 != OBSTRUKSJON_STOPP)
	  elev_set_stop_lamp(false);
}

void etasjeAnkommet(int e)
{
  etasje = e;
  foo1(ETASJE_ANKOMMET);
}

void nyOrdre()
{
    foo1(NY_ORDRE);
    foo2(NY_ORDRE);
}

void obstruksjonPa()
{
    foo2(OBSTRUKSJON_PA);
}

void obstruksjonAv()
{
    foo2(OBSTRUKSJON_AV);
}

void nodStopp()
{
  slettAlleOrdre();
  elev_set_stop_lamp(true);
  foo2(NOD_STOPP);
}

void timeOut()
{
  foo1(TIME_OUT);
}

void initHeisstyring()
{
  elev_set_speed(100);
}
