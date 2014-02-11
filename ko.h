#ifndef KO_H
#define KO_H

#include <stdbool.h>

void slettAlleOrdre();

void slettOrdre(int etasje, bool opp);

int hentNesteOrdre(int etasje, bool opp);

void leggTilOrdre(int etasje, bool inne, bool opp);

bool finnestOrdreInniHeis();

void settLys();

bool erKoTom();

#endif