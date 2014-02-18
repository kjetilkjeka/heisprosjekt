// REPLACE THIS FILE WITH YOUR OWN CODE.
// READ ELEV.H FOR INFORMATION ON HOW TO USE THE ELEVATOR FUNCTIONS.

#include "elev.h"
#include <stdio.h>
#include <stdbool.h>
#include "ko.h"
#include "heisstyring.h"
#include <time.h>

clock_t startTime = 0;
bool timedOut = true;

int main()
{
    // Initialize hardware
    // Initialize hardware
    if (!elev_init()) {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }

    int forgje = 0;
    

    printf("Press STOP button to stop elevator and exit program.\n");
    slettAlleOrdre();
    elev_set_speed(0);    
    initHeisstyring();

    while (1) {
      // poll buttons
      for(int i = 0; i < N_FLOORS; i++) // poller masse, fix
	{
	  if(i != N_FLOORS-1 && elev_get_button_signal(BUTTON_CALL_UP, i))
	    leggTilOrdre(i, false, true);
	  
	  if(i != 0 && elev_get_button_signal(BUTTON_CALL_DOWN, i))
	    leggTilOrdre(i, false, false);
	  
	  if(elev_get_button_signal(BUTTON_COMMAND, i))
	    leggTilOrdre(i, true, true);
	  
	}

      int etasje = elev_get_floor_sensor_signal();

  
      if (etasje != -1 && etasje != forgje)
	{
	  elev_set_floor_indicator(etasje);
	  etasjeAnkommet(etasje);
	  forgje = etasje;
	}

      clock_t time = clock();
      if(time - startTime > 3000000 && !timedOut)
	{
	  timeOut();
	  timedOut = true;
	  printf("timed out bitches!\n\r");
	}

      
    // obstruksjon og nødstopp

    }


    return 0;
}

void startTimer()
{
  startTime = clock();
  timedOut = false;
  
}
