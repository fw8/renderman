/* \subsection{hash.c} \label{hash.c}
 * RenderMan Shading Language Hash-Verwaltung.
\begin{verbatim} */

#include "hash.h"
#define NULL (void *)0L

/* entry points:
 *   clr_hash - L"oschen der Hash-Tabelle.
 *   enter_hash - Eintragen eines neuen Namens in die Hash-Tabelle.
 *   lkup_hash - Wert eines Namens in der Hash-Tabelle suchen und zur"uckgeben.
 *   change_hash - change a name's value in the symbol table.
 *
 */


/*
 * empty the hash table before using it...
 *
 */
void
clr_hash(htab)
HTAB htab[];
{
  int i;
  for (i=0; i<HASHSIZE; i++) htab[i] = (HTAB)0;
}

/*
 * compute the value of the hash for a symbol
 *
 */
static
int
hash(name)
char *name;
{
  int sum;
  for (sum = 0; *name != '\0'; name++) sum += (sum + *name);
  sum %= HASHSIZE;                      /* take sum mod hashsize */
  if (sum < 0) sum += HASHSIZE;         /* disallow negative hash value */
  return(sum);
}

/*
 * find name in the symbol table, return its value.  Returns NULL
 * if not found.
 *
 */
void *
lkup_hash(name,htab)
char *name;
HTAB htab[];
{
  HTAB cur;
  for (cur = htab[hash(name)]; cur != (HTAB)0; cur = cur->hnxt)
        if (strcmp(cur->name,name) == 0) return(cur->val);
  return(NULL);
}

/*
 * find name in the symbol table, change its value.  Returns 0
 * if not found.
 *
 */
int
change_hash(name,value,htab)
char *name;
void *value;
HTAB htab[];
{
  HTAB cur;
  for (cur = htab[hash(name)]; cur != (HTAB)0; cur = cur->hnxt)
        if (strcmp(cur->name,name) == 0) {cur->val=value; return(1);}
  return(0);
}

/*
 * enter ident name into the hash table
 *
 */
void
enter_hash(name,svalue,htab)
char *name;
void *svalue;
HTAB htab[];
{
  extern char *strsave();
  int h;
  HTAB cur;
  if (lkup_hash(name,htab) != NULL) {
     internal("identifier %s appears twice",name);
  }
  h = hash(name);
  cur = (HTAB)xmalloc(sizeof (struct hash_entry));
  cur->name = strsave(name);
  cur->val = svalue;
  cur->hnxt = htab[h];
  htab[h] = cur;
}
/*\end{verbatim}*/
