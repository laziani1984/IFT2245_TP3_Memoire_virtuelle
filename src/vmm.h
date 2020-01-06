#ifndef VMM_H
#define VMM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "conf.h"

//// -------------------------------------------------------------- ////

//// ==============+
//// Les Fonctions||
//// ==============+

void vmm_init (FILE *log);
char vmm_read (unsigned int logical_address);
void vmm_write (unsigned int logical_address, char);
void vmm_clean (void);
int pageFault(unsigned int page, int frame);

#endif

//// -------------------------------------------------------------- ////