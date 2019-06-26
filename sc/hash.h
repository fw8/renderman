/* \subsection{hash.h} \label{hash.h}
 * entry points:
 *   clr\_hash - empty hash table.
 *   enter\_hash - enter a name into the symbol table
 *   lkup\_hash - find a name's value in the symbol table.
 *   change\_hash - change a name's value in the symbol table.
 *
\begin{verbatim} */

#define HASHSIZE 101                    /* # of entries in hash table */
typedef
struct hash_entry{ char *name;                /* symbol name */
                   void *val;                   /* value */
             struct hash_entry *hnxt; }        /* next on collision chain */
* HTAB;

extern
void
clr_hash();

extern
void *
lkup_hash();

extern
int
change_hash();

extern
void
enter_hash();
/*\end{verbatim}*/
