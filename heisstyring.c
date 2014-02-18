#include <stdbool.h>
#include <stdio.h>
#include "main.h"

typedef enum
{
  INIT,
  IDLE,
  BEVEGER,
  OBSTRUKSJON,
  APNE_DORER,
  NODSTOPP,
  DOR_OBSTRUKSJON,
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


tilstand_t tilstand = INIT;
bool opp = true;
int etasje = 0;

void foo(event_t event)
{
    switch(tilstand)
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

    case(OBSTRUKSJON):
      obstruksjonState(event);
      break;
    
    case(APNE_DORER):
      apneState(event);
      break;
	
    case(NODSTOPP):
      stoppState(event);
      break;
	
    case(DOR_OBSTRUKSJON):
      dorObstruksjonState(event);
      break;

    case(OBSTRUKSJON_STOPP):
      obstruksjonStoppState(event);
      break;
    }
    printf("tilstand er: %d \n\r", tilstand);
    printf("event er: %d \n\r", event);
}

void initState(event_t event)
{
      switch(event)
	{
	case(NOD_STOPP):
	  break;
	
	case(OBSTRUKSJON_PA):
	  break;
	
	case(OBSTRUKSJON_AV):
	  break;

	case(ETASJE_ANKOMMET):
	  elev_set_speed(0);
	  etasje = elev_get_floor_sensor_signal(); // må ta hennsyn her
	  tilstand = IDLE;
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
	      elev_set_door_open_lamp(1);
	      tilstand = APNE_DORER;
	      startTimer();
	      //apneState(TIME_OUT); // hack
	      //do stuff
	    } else {
		tilstand = BEVEGER;
		bevegerEntry();
	    }
	    break;	
	}
}

void bevegerEntry()
{
  int neste = hentNesteOrdre(etasje, opp);
  
  if(neste > etasje)
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
	case(NOD_STOPP):	
	    tilstand = NODSTOPP;
	    break;
	case(OBSTRUKSJON_PA):	
	    tilstand = OBSTRUKSJON;
	    break;

	case(ETASJE_ANKOMMET):	
	  if(hentNesteOrdre(etasje, opp) == etasje) // clean up
	    {
	      slettOrdre(etasje, opp);
	      elev_set_door_open_lamp(1);
	      tilstand = APNE_DORER;
	      startTimer();
	      //apneState(TIME_OUT); // hack
	      // timer
	    }
	}

	//exit
	if(tilstand != BEVEGER)
	    elev_set_speed(0);

}

void obstruksjonState(event_t event)
{
	switch(event)
	{	
	case(NOD_STOPP):
	    tilstand = OBSTRUKSJON_STOPP;
	    break;

	case(OBSTRUKSJON_AV):
	    tilstand = BEVEGER;
	    
	}
}

void apneState(event_t event)
{
	//entry
	    // start timer
	switch(event)
	{
	case(TIME_OUT):
	  printf("hent neste ordre er %d \n\r", hentNesteOrdre(etasje, opp));
	  if(hentNesteOrdre(etasje, opp) == -1)
	    {
	      tilstand = IDLE;
	    } else if(hentNesteOrdre(etasje, opp) == etasje) // dette er surrete kan hende det er mye enklere
	    {	
	      slettOrdre(etasje, true);
	      slettOrdre(etasje, false);
	      apneState(TIME_OUT);
	      } else 
	      {
		bevegerEntry();
		tilstand = BEVEGER;
	      }
	
	    break;
	case(OBSTRUKSJON_PA):	
	    tilstand = DOR_OBSTRUKSJON;
	    break;

	case(NY_ORDRE):
	  // if ny ordre er i etasjen den er i
	  slettOrdre(etasje, opp);
	  tilstand = APNE_DORER;
	  startTimer();
	
	}

	if(tilstand != APNE_DORER)
	  elev_set_door_open_lamp(0);

}

void stoppState(event_t event)
{

	//entry
	slettAlleOrdre();

	switch(event)
	{
	case(NY_ORDRE_TOM_KO):
	
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
	    {
		tilstand = BEVEGER;
	    } else if(finnestOrdreInniHeis() || elev_get_stop_signal())
	    {
		tilstand = NODSTOPP; // vil kalle entry på nytt ;)
	    }

	    break;
	case(OBSTRUKSJON_PA):
	
	    tilstand = OBSTRUKSJON_STOPP;
	}
}

void dorObstruksjonState(event_t event)
{
	switch(event)
	{
	case(OBSTRUKSJON_AV):
	    tilstand = APNE_DORER;
	}
}

void obstruksjonStoppState(event_t event)
{

	//entry
	slettAlleOrdre();
	
	switch(event)
	{
	case(NY_ORDRE_TOM_KO):
	
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
		tilstand = OBSTRUKSJON;
	    else if(finnestOrdreInniHeis || elev_get_stop_signal)
		tilstand = OBSTRUKSJON_STOPP; // vil kalle entry på nytt ;)

	break;
	case(OBSTRUKSJON_AV):
	    tilstand = NODSTOPP;
	
	}
}

void etasjeAnkommet(int e) // clean up
{
  etasje = e;
  foo(ETASJE_ANKOMMET);
}

void nyOrdre()
{
    foo(NY_ORDRE);
}

void obstruksjonPa()
{
    foo(OBSTRUKSJON_PA);
}

void obstruksjonAv()
{
    foo(OBSTRUKSJON_AV);
}

void nodStopp()
{
    foo(NOD_STOPP);
}


void timeOut()
{
  foo(TIME_OUT);
}

void initHeisstyring()
{
  elev_set_speed(100);
}
