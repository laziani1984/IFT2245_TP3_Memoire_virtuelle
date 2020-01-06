// Ce tp est présenté par Wael ABOU ALI, 20034365
/*
 * Version : 1.0
 * Utilisation : C'est un programme qui simule un gestionnaire de mémoire virtuelle par
 * pagination (paging) à l'aide d'un tlb. Le programme va lire et exécuter une liste des
 * commandes sur des adresses logiques, les convertir en adresses physiques à l'aide du tlb
 * et système de pagination et lire le contenu qui se trouve dans la mémoire physique.
 */

#include <stdint.h>
#include <stdio.h>

#include "tlb.h"

#include "conf.h"

//// -------------------------------------------------------------- ////

//// ==============+
//// Les variables||
//// ==============+

struct tlb_entry {
  unsigned int page_number;
  int frame_number;             /* Invalide si négatif.  */
  bool readonly : 1;
};

static FILE *tlb_log = NULL;
static struct tlb_entry tlb_entries[TLB_NUM_ENTRIES]; 

static unsigned int tlb_hit_count = 0;
static unsigned int tlb_miss_count = 0;
static unsigned int tlb_mod_count = 0;
unsigned int tlb_counter;
static unsigned int tlb_fifo_count = 0;


//// -------------------------------------------------------------- ////

//// ==============+
//// Les Fonctions||
//// ==============+

/* Initialise le TLB, et indique où envoyer le log des accès.  */
void tlb_init (FILE *log) {
  for (int i = 0; i < TLB_NUM_ENTRIES; i++) { tlb_entries[i].frame_number = -1; }
  tlb_log = log;
}

/******************** ¡ NE RIEN CHANGER CI-DESSUS !  ******************/

/* Teste la présence d'une page dans le TLB.
 * Renvoie true, si trouvé, ou false sinon. */
static bool tlb__verify (unsigned int page_number) {
  // TODO: COMPLÉTER CETTE FONCTION.
  for (unsigned int i = 0; i < TLB_NUM_ENTRIES; i++) {
    if (tlb_entries[i].page_number == page_number) {
      //Si la page est trouvé (TLB hit)
      return true;
    } else { tlb_counter += 1; }
  }
  return false;
}

    //// +------------------------------------------+ ////

/* Recherche dans le TLB.
 * Renvoie le `frame_number`, si trouvé, ou un nombre négatif sinon.  */
static int tlb__lookup(unsigned int page_number, bool write) {
  // TODO: COMPLÉTER CETTE FONCTION.

  tlb_counter = 0;

  //Recherche le TLB pour la page demandé
  if(tlb__verify(page_number)) {
    tlb_entries[tlb_counter].readonly = true;
    //Si l'acc
    if (write == false || tlb_entries[tlb_counter].readonly == true)
        { return tlb_entries[tlb_counter].frame_number; }
  }
  // sinon retourne faux.
  return -1;
}

    //// +------------------------------------------+ ////

/* Ajoute dans le TLB une entrée qui associe `frame_number` à
 * `page_number`.  */
static void tlb__add_entry(unsigned int page_number,
                           unsigned int frame_number, bool readonly) {
    // TODO: COMPLÉTER CETTE FONCTION.
    //Algorithme de remplacement LRU pout remplaces les pages
    tlb_fifo_count = tlb_fifo_count % TLB_NUM_ENTRIES;
    tlb_entries[tlb_fifo_count].page_number = page_number;
    tlb_entries[tlb_fifo_count].frame_number = frame_number;
    tlb_entries[tlb_fifo_count].readonly = readonly;
    tlb_fifo_count++;

}

    //// +------------------------------------------+ ////

/******************** ¡ NE RIEN CHANGER CI-DESSOUS !  ******************/

void tlb_add_entry (unsigned int page_number,
                    unsigned int frame_number, bool readonly) {
  tlb_mod_count++;
  tlb__add_entry(page_number, frame_number, readonly);
}

    //// +------------------------------------------+ ////

int tlb_lookup (unsigned int page_number, bool write) {
  int fn = tlb__lookup (page_number, write);
  (*(fn < 0 ? &tlb_miss_count : &tlb_hit_count))++;
  return fn;
}

    //// +------------------------------------------+ ////

bool tlb_verify (unsigned int page_number) { return tlb__verify (page_number); }

    //// +------------------------------------------+ ////

/* Imprime un sommaires des accès.  */
void tlb_clean (void) {

  fprintf (stdout, "TLB misses   : %3u\n", tlb_miss_count);
  fprintf (stdout, "TLB hits     : %3u\n", tlb_hit_count);
  fprintf (stdout, "TLB changes  : %3u\n", tlb_mod_count);
  fprintf (stdout, "TLB miss rate: %.1f%%\n",
           100 * tlb_miss_count
           /* Ajoute 0.01 pour éviter la division par 0.  */
           / (0.01 + tlb_hit_count + tlb_miss_count));

}

//// -------------------------------------------------------------- ////
