#include "geometry.h"

void subdivideCatmull(list_t **geometries);
void subdivideLoop(list_t **geometries);
void subdivideKobbelt(list_t **geometries);
void subdivideButterfly(list_t **geometries, float w);
