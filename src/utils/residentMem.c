#include <config.h> 
#include <string.h> 
#include <stdio.h>

#define to_string( s ) # s
#define STR_DEFINE(t, s) t = to_string(s)


double residentMem () {

  FILE* procresults;
  char buf[150];
  char* foo;
  long int myRSS, totRSS;
  char pscommand[150];
  char* psPath;

  STR_DEFINE(psPath, PSCOMMAND );

  strncpy(pscommand, psPath, strlen(psPath));

#if PSTYPE == BSD
  strcat(pscommand, " ax -o rss");
#else
  strcat(pscommand, " -ef -o rss");
#endif

  procresults = popen(pscommand, "r");

  totRSS = 0;
  while (!feof(procresults)) {
    fgets( buf, 150, procresults);
    if (!strcmp(buf, " RSS")) continue;
        
    foo = strtok(buf, " ,;\t");
    myRSS = atoi(foo);
    totRSS += myRSS;
  } 
  pclose(procresults);

  return(totRSS);

}
