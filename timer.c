#include <time.h>
#include <stdbool.h>


clock_t startTime = 0;
bool timedOut = true;


bool harTimetUt()
{
  clock_t tid = clock();

  if(tid - startTime > 3000000 && !timedOut)
    {
      timedOut = true;
      return true;
    }
  return false;
}

void startTimer()
{
  startTime = clock();
  timedOut = false;
  
}
