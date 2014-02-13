#include <stdbool.h>
#include <stdio.h>

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
    
	switch(event)
	{
	case(NY_ORDRE_TOM_KO):
	
	
	case(TIME_OUT):
	
	
	case(NOD_STOPP):
	
	
	case(OBSTRUKSJON_PA):
	
	
	case(OBSTRUKSJON_AV):
	
	
	case(NY_ORDRE):
	    printf("lol");
	
	}
	
	
    
    case(IDLE):
	switch(event)
	{
	case(NY_ORDRE_TOM_KO):
	    if(etasje == hentNesteOrdre(etasje, opp))
	    {
		tilstand = APNE_DORER;
		//do stuff
	    } else {
		tilstand = BEVEGER;
		//do stuff
	    }
	    
	
	}
	
    
    case(BEVEGER):
    
	//entry
	(opp ? elev_set_speed(100) : elev_set_speed(-100)); // denne er litt loka (blir kalla for ofte)
	
	switch(event)
	{
	case(NOD_STOPP):	
	    tilstand = NODSTOPP;
	
	case(OBSTRUKSJON_PA):	
	    tilstand = OBSTRUKSJON;
	

	case(ETASJE_ANKOMMET):	
	    if(hentNesteOrdre(etasje, opp) == etasje)
	    {
		tilstand = APNE_DORER;
	    }
	
	}

	//exit
	if(tilstand != BEVEGER)
	    elev_set_speed(0);

	
    
    case(OBSTRUKSJON):
    
	switch(event)
	{	
	case(NOD_STOPP):
	    tilstand = OBSTRUKSJON_STOPP;
	
	case(OBSTRUKSJON_AV):
	    tilstand = BEVEGER;
	
	}
	
    
    case(APNE_DORER):
    
	//entry
	    // start timer
	switch(event)
	{
	case(TIME_OUT):
	    if(hentNesteOrdre() == -1)
		tilstand = IDLE;
	    else
		tilstand = BEVEGER;
	
	
	case(OBSTRUKSJON_PA):	
	    tilstand = DOR_OBSTRUKSJON;
     
	case(NY_ORDRE):
	    printf("starte timer på ny");
	    //start timer på ny
	
	}
	
    
    case(NODSTOPP):
    
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

	case(OBSTRUKSJON_PA):
	
	    tilstand = OBSTRUKSJON_STOPP;
	}

	
    
    case(DOR_OBSTRUKSJON):
	switch(event)
	{
	case(OBSTRUKSJON_AV):
	    tilstand = APNE_DORER;
	}
	

    
    case(OBSTRUKSJON_STOPP):
    
	//entry
	slettAlleOrdre();
	
	switch(event)
	{
	case(NY_ORDRE_TOM_KO):
	
	    if(finnestOrdreInniHeis() && !elev_get_stop_signal())
		tilstand = OBSTRUKSJON;
	    else if(finnestOrdreInniHeis || elev_get_stop_signal)
		tilstand = OBSTRUKSJON_STOPP; // vil kalle entry på nytt ;)

       
	
	case(OBSTRUKSJON_AV):
	    tilstand = NODSTOPP;
	
	}

    
    }
}
