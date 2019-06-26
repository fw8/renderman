/*\subsection{AdminHash.c} \label{AdminHash.c}
 *	AdminHash : Verwaltung diverser Hash-Tabellen.
\begin{verbatim}*/

#include "RiTypes.h"
#include "Misc.h"
#include "AdminGlobals.h"
#include "RiError.h"

#define SHADER_HASH_SIZE 513
#define TEXTURE_HASH_SIZE 20 
#define TOKEN_HASH_SIZE 129
#define TRAFO_HASH_SIZE 63 

/*
 * Definintion der Tabellen-eintraege.
 */
typedef
struct hash_entry{ char *name;              /* key */
                   char *val;               /* value */
             struct hash_entry *hnxt;       /* next on collision chain */
} *HTab;

/*
 * Definition der Hash-Tabelle.
 */
typedef
struct hash_handle {
	int  size;	/* max # of entries in hash table */
	int  entries;	/* current # of entries in hash table */
	HTab *htab;	/* hash-table */
} *Hash;

static Hash ShaderHash, TextureHash, TokenHash, TrafoHash;


/*\end{verbatim} \label{AdminHash.c:InitHash} \begin{verbatim}
 * Initialisieren einer Hash-Tabelle.
 */
static void
InitHash(hash)
Hash hash;
{
  int i;
  for (i=0; i<hash->size; i++) hash->htab[i] = (HTab)0;
  hash->entries=0;
}

/*\end{verbatim} \label{AdminHash.c:CreateHash} \begin{verbatim}
 * Erzeugen einer Neuen Hash-Tabelle (der groesse 'size').
 */
static Hash
CreateHash(size)
int size;
{
  Hash new_hash=(Hash)RmMalloc(sizeof(struct hash_handle));

  new_hash->size=size;
  new_hash->htab=(HTab *)RmMalloc(sizeof(HTab)*size);
  InitHash(new_hash);
  return new_hash;
}

/*\end{verbatim} \label{AdminHash.c:DestroyHash} \begin{verbatim}
 * Freigabe des durch die Hash-Tabelle belegten Speichers.
 */
static void
DestroyHash(hash)
Hash hash;
{
  RmFree(hash->htab);
  RmFree(hash);
}

/*\end{verbatim} \label{AdminHash.c:CalcHashValue} \begin{verbatim}
 * Berechnung des Hash-Index
 */
static int
CalcHashValue(hash,name)
Hash hash;
char *name;
{
  int sum;
  for (sum = 0; *name != '\0'; name++) sum += (sum + *name);
  sum %= hash->size;                      /* take sum mod hashsize */
  if (sum < 0) sum += hash->size;         /* disallow negative hash value */
  return(sum);
}

/*\end{verbatim} \label{AdminHash.c:lookUpHash} \begin{verbatim}
 * finde 'name' in der Hash-Tabelle und gib seinen Wert zurueck.
 * wenn nicht gefunden gib RI_NULL zurueck.
 */
static char *
LookUpHash(hash,name)
Hash hash;
char *name;
{
  HTab cur;
  for (cur = hash->htab[CalcHashValue(hash,name)]; cur; cur = cur->hnxt) 
    if (strcmp(cur->name,name) == 0) return(cur->val);
  return(RI_NULL);
}


/*\end{verbatim} \label{AdminHash.c:FillHash} \begin{verbatim}
 * Eintrag eines Wertes in eine Hash-Table.
 */
static void
FillHash(hash,name,value)
Hash hash;
char *name;
char *value;
{
  int h;
  HTab cur;

  if (LookUpHash(hash,name)) {
     sprintf(RmMsg,"[AdminHash.FillHash] identifier %s appears twice",name);
     internal(1,RmMsg);
  }
  h = CalcHashValue(hash,name);
  cur = (HTab)RmMalloc(sizeof (struct hash_entry));
  cur->name = RmStrSave(name);
  cur->val = value;
  cur->hnxt = hash->htab[h];
  hash->htab[h] = cur;
  hash->entries++;
}


/*\end{verbatim} \label{AdminHash.c:RmInstallNewShader} \begin{verbatim}
 ***************** SHADER-HASH-SUPPORT
 */
void
RmInstallNewShader(name,proc)
char   *name;
RtPointer proc;
{
  if (LookUpHash(ShaderHash,name)) {
    error(1,"[AdminHash.RmInstallNewShader] duplicate shader (ignored !)");
  }else{
    FillHash(ShaderHash,name,proc);
  }
}
/*\end{verbatim} \label{AdminHash.c:RmLookUpShader} \begin{verbatim}
 */
RtFunc
RmLookUpShader(name)
char *name;
{
  return (RtFunc)LookUpHash(ShaderHash,name);
}

/*
 * TEXTURE-HASH-SUPPORT
 */
void
RmInstallNewTexture(name,map)
char   *name;
RtPointer  map;
{
  if (LookUpHash(TextureHash,name)) {
    error(1,"[AdminHash.RmInstallNewTexture] duplicate texture (ignored !)");
  }else{
    FillHash(TextureHash,name,map);
  }
}

RtPointer
RmLookUpTexture(name)
char *name;
{
  return (RtPointer)LookUpHash(TextureHash,name);
}


/*\end{verbatim} \label{AdminHash.c:RmDeclareToken} \begin{verbatim}
 ***************** TOKEN-HASH-SUPPORT
 */
RtToken
RmDeclareToken(name,type)
char *name;
char *type;
{
  RtToken token;

  if (LookUpHash(TokenHash,name)) {
    warning(1,"[AdminHash.RmDeclareToken] duplicate declaration (ignored !)");
  }else{
    FillHash(TokenHash,name,(void *)type);
  }
  return name;
}


/*\end{verbatim} \label{AdminHash.c:RmDeclareTrafo} \begin{verbatim}
 ***************** TRAFO-HASH-SUPPORT
 */
int
RmDeclareTrafo(name,trafo)
char *name;
transform *trafo;
{
  if (LookUpHash(TrafoHash,name)) {
    return 1; /* ERROR */
  }else{
    FillHash(TrafoHash,name,(void *)trafo);
  }
}
/*\end{verbatim} \label{AdminHash.c:RmLookUpTrafo} \begin{verbatim}
 */
transform *
RmLookUpTrafo(name)
char *name;
{
  return (transform *)LookUpHash(TrafoHash,name);
}

/*\end{verbatim} \label{AdminHash.c:_init_AdminHash} \begin{verbatim}
 */
void
_init_AdminHash()
{
  ShaderHash=CreateHash(SHADER_HASH_SIZE);
  TextureHash=CreateHash(TEXTURE_HASH_SIZE);
  TokenHash=CreateHash(TOKEN_HASH_SIZE);
  TrafoHash=CreateHash(TRAFO_HASH_SIZE);
}
/*\end{verbatim} \label{AdminHash.c:_begin_AdminHash} \begin{verbatim}
 */
_begin_AdminHash()
{
    RmDeclareToken("Ka","float");
    RmDeclareToken("Kd","float");
    RmDeclareToken("Ks","float");
    RmDeclareToken("roughness","float");

    RmDeclareToken("P", "varying point");
    RmDeclareToken("Pz","varying float");
    RmDeclareToken("Pw","varying float");
    RmDeclareToken("N", "varying point");
    RmDeclareToken("Np","uniform point");
    RmDeclareToken("Cs","varying color");
    RmDeclareToken("Os","varying color");
    RmDeclareToken("s", "varying float");
    RmDeclareToken("t", "varying float");
    RmDeclareToken("st","varying float");
}
/*\end{verbatim} \label{AdminHash.c:_end_AdminHash} \begin{verbatim}
 */
void
_end_AdminHash()
{
  InitHash(ShaderHash);
  InitHash(TextureHash);
  InitHash(TokenHash);
  InitHash(TrafoHash);
}
/*\end{verbatim} \label{AdminHash.c:_exit_AdminHash} \begin{verbatim}
 */
void
_exit_AdminHash()
{
  DestroyHash(ShaderHash);
  DestroyHash(TextureHash);
  DestroyHash(TokenHash);
  DestroyHash(TrafoHash);
}
/*\end{verbatim}*/
