#include "elev.h"
#include <stdio.h>
#include <stdbool.h>
#include "ko.h"
#include "heisstyring.h"
#include "timer.h"

bool obstruksjon = false;
bool stopp = false;

int main()
{
    if (!elev_init()) {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }


    slettAlleOrdre();
    elev_set_speed(0);    
    initHeisstyring();

    int forgjeEtasje = 0;
    int etasje;
    bool forgjeStopp;
    bool stopp;
    bool forgjeObstruksjon;
    bool obstruksjon;
    bool forgjeOpp[4];
    bool opp[4];
    bool forgjeInne[4];
    bool inne[4];
    bool forgjeNed[4];
    bool ned[4];

    while(1)
    {
	stopp = elev_get_stop_signal();
	obstruksjon = elev_get_obstruction_signal();
	etasje = elev_get_floor_sensor_signal();
	
	for(int i = 0; i < N_FLOORS; i++) // poller masse, fix
	{
	    if(i != N_FLOORS-1)
		opp[i] = elev_get_button_signal(BUTTON_CALL_UP, i);
    	    
	    if(i != 0)
		ned[i] = elev_get_button_signal(BUTTON_CALL_DOWN, i);
	    
	    inne[i] = elev_get_button_signal(BUTTON_COMMAND, i);
	}

	for(int i = 0; i < N_FLOORS; i++)
	{
	    if(opp[i] && !forgjeOpp[i])
		leggTilOrdre(i, false, true);
	    
	    if(ned[i] && !forgjeNed[i])
		leggTilOrdre(i, false, false);
	    
	    if(inne[i] && !forgjeInne[i])
		leggTilOrdre(i, true, true);
	    
	    forgjeOpp[i] = opp[i];
	    forgjeNed[i] = ned[i];
	    forgjeInne[i] = inne[i];
	}

	if(harTimetUt())
	    timeOut();

	if (etasje != -1 && etasje != forgjeEtasje)
	{
	    elev_set_floor_indicator(etasje);
	    etasjeAnkommet(etasje);
	    forgjeEtasje = etasje;
	}

	if(obstruksjon && !forgjeObstruksjon)
	{
	    forgjeObstruksjon = obstruksjon;
	    forgjeEtasje = -1;
	    if(obstruksjon)
		obstruksjonPa();
	    else
		obstruksjonAv();
	}
	
	if(stopp && !forgjeStopp)
	{
	    forgjeStopp = stopp;
	    forgjeEtasje = -1;
	    if(stopp)
		nodStopp();
	}
    }	
    return 0;
}
