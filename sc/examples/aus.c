/*
 *      RenderMan: Shading-Module 'txtplastic'
 */
#include <stdio.h>
#include <varargs.h>
#include "RiTypes.h"
#include "RiError.h"
#include "Shader.h"
#include "V.h"
#define EQ(a,b) (!strcmp((a),(b)))
typedef struct {
RtFloat Ks;
RtFloat Kd;
RtFloat Ka;
RtFloat roughness;
RtColor specularcolor;
RtString mapname;
} txtplastic$type;
static IOF iof = {1,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0};
/*
 * ShaderName: Liefert den Namen des Shaders.
 */
static char *
ShaderName()
{
  return "txtplastic" ;
}
/*
 * ShaderInfo: Druckt die Instanzen-Variablen des Shaders aus.
 */
static void
ShaderInfo(shader,stream)
Shader *shader;
FILE *stream;
{
  txtplastic$type *s = (txtplastic$type *)shader->data;
  fprintf(stream,"txtplastic:\n\
 Ks = %g\
,\n\
 Kd = %g\
,\n\
 Ka = %g\
,\n\
 roughness = %g\
,\n\
 specularcolor = (%g;%g;%g)\
,\n\
 mapname = \"%s\"\
"
,s->Ks
,s->Kd
,s->Ka
,s->roughness
,s->specularcolor[0],s->specularcolor[1],s->specularcolor[2]
,s->mapname
);
}
/*
 * ShaderSupport: Waehlt die benoetigten I/O-Variablen aus.
 */
static IOF *
ShaderSupport(shader)
Shader shader;
{
  return &iof;
}
static void
ShaderShade(level,shader,lightset,iob)
int level;
Shader *shader;
LightSet *lightset;
IOB *iob;
{
txtplastic$type *$s = (txtplastic$type *)shader->data;
RtFloat $f0,$f1,$f2,$f3,$f4,$f5,$f6;
RtColor $c0,$c1,$c2,$c3,$c4,$c5,$c6;
RtPoint $p0,$p1,$p2,$p3,$p4,$p5,$p6;
RtPoint Nf;
RtPoint V;
Rm_faceforward(
(
Rm_normalize(
(
iob->N)
,$p1),
$p1)
,
(
iob->I)
,$p0),
copyV($p0,Nf)
;
Rm_normalize(
(
mulVf(iob->I,-1.,$p1),
$p1)
,$p0),
copyV($p0,V)
;
if(
$f0=strcmp($s->mapname,""),
$f0){
Rm_textureV(
(
$s->mapname),(
0.000000e+00),
(
iob->s)
,
(
iob->t)
,$c0),
copyV($c0,iob->Ci)
;
}else{
copyV(iob->Cs,iob->Ci)
;
}
copyV(iob->Os,iob->Oi)
;
Rm_ambient(lightset
,$c0),
mulVf($c0,$s->Ka,$c1),
Rm_diffuse(lightset,iob->P,
(
Nf)
,$c0),
mulVf($c0,$s->Kd,$c2),
addVV($c1,$c2,$c0),
mulVV(iob->Ci,$c0,$c1),
mulVf($s->specularcolor,$s->Ks,$c0),
Rm_specular(lightset,iob->P,
(
Nf)
,
(
V)
,
(
$s->roughness)
,$c2),
mulVV($c0,$c2,$c3),
addVV($c1,$c3,$c0),
mulVV(iob->Os,$c0,$c1),
copyV($c1,iob->Ci)
;
}
static ShadeProcs txtplastic$progs = {
        ShaderName,
        ShaderInfo,
        ShaderSupport,
        ShaderShade
};
static Shader *
NEW_Shader(argp)
  va_list argp;
{
  RtToken token;
  txtplastic$type *$s;
  Shader   *S;
  RtFloat $f0,$f1,$f2,$f3,$f4,$f5,$f6;
  RtColor $c0,$c1,$c2,$c3,$c4,$c5,$c6;
  RtPoint $p0,$p1,$p2,$p3,$p4,$p5,$p6;
  $s=(txtplastic$type *)RmMalloc(sizeof(txtplastic$type));
  S=(Shader *)RmMalloc(sizeof(Shader));
$s->Ks=5.000000e-01
;
$s->Kd=5.000000e-01
;
$s->Ka=1.000000e+00
;
$s->roughness=1.000000e-01
;
setV(1.000000e+00,1.000000e+00,1.000000e+00,$s->specularcolor)
;
$s->mapname=""
;
token=va_arg(argp,RtToken);
while (token != RI_NULL) {
if
(EQ(token,"Ks"))
$s->Ks= *(RtFloat *)va_arg(argp,RtPointer);
else if
(EQ(token,"Kd"))
$s->Kd= *(RtFloat *)va_arg(argp,RtPointer);
else if
(EQ(token,"Ka"))
$s->Ka= *(RtFloat *)va_arg(argp,RtPointer);
else if
(EQ(token,"roughness"))
$s->roughness= *(RtFloat *)va_arg(argp,RtPointer);
else if
(EQ(token,"specularcolor"))
RmMemCpy(va_arg(argp,RtPointer),$s->specularcolor,sizeof(RtPoint));
else if
(EQ(token,"mapname"))
strcpy($s->mapname,va_arg(argp,RtPointer));
else {
sprintf(RmMsg,"[SurfaceShader.txtplastic] unknown or wrong token RI_%s (ignored)",token);
warning(2,RmMsg);
break;
}
token=va_arg(argp,RtToken);
}
  S->data=(char *)$s;
  S->procs= &txtplastic$progs;
  return S;
}
void
_init_txtplastic$shader()
{
  RmInstallNewShader("txtplastic",NEW_Shader);
}
