/* \subsection{AdminTree.c} \label{AdminTree.c}
 Alle Funktionen zur Verwaltung des Objektbaumes!
\begin{verbatim} */

#include <stdio.h>
#include "AdminGlobals.h"
#include "RiError.h"

/*\end{verbatim} \label{AdminTree.c:RmFillPrim} \begin{verbatim}
 * RmFillPrim: F"ullt die Objektstruktur voll auf.
 *
 *	INPUT : p = Zeiger auf das Objekt.
 */
void
RmFillPrim(p)
	Prim *p;
{
  RtMatrix WtoO;	/* world to object transformation */
  LightSet *li;
  int    i;

  /* WorldtoObject-Matrix erzeugen */
  RmInvertM44(RmGlobals.activattributes.trans.OtoW,WtoO);
  RmCpyM44(WtoO,p->WtoO);
  RmCpyM44(RmGlobals.activattributes.trans.OtoW,p->OtoW);
  /* Und alles eintragen, was in der Struktur wichtig ist! */
  p->shader = RmGlobals.activattributes.AllShader;
  p->lights = RmGlobals.lightnumber;
  copyV3(RmGlobals.activattributes.Cs,p->attrib.Cs);
  copyV3(RmGlobals.activattributes.Os,p->attrib.Os);
  p->attrib.s1 = RmGlobals.activattributes.s1;
  p->attrib.t1 = RmGlobals.activattributes.t1;
  p->attrib.s2 = RmGlobals.activattributes.s2;
  p->attrib.t2 = RmGlobals.activattributes.t2;
  p->attrib.s3 = RmGlobals.activattributes.s3;
  p->attrib.t3 = RmGlobals.activattributes.t3;
  p->attrib.s4 = RmGlobals.activattributes.s4;
  p->attrib.t4 = RmGlobals.activattributes.t4;
  p->attrib.sides = RmGlobals.activattributes.sides;
  p->attrib.ustep = RmGlobals.activattributes.ustep;
  p->attrib.vstep = RmGlobals.activattributes.vstep;
  /* Alle aktiven Lichtquellen einf"ugen */
  li=p->lightset=0;
  for(i=0;i<RmGlobals.outerlightpos+1;i++) {
	if(RmGlobals.activattributes.outerlightstate[i]) 
	  p->lightset = (LightSet *)RmMalloc(sizeof(LightSet));
          p->lightset->light=RmGlobals.outerlights[i];
          p->lightset->next=li;
          li=p->lightset;
  }
  for(i=0;i<RmGlobals.innerlightpos+1;i++) {
        if(RmGlobals.activattributes.innerlightstate[i])
          p->lightset = (LightSet *)RmMalloc(sizeof(LightSet));
          p->lightset->light=RmGlobals.innerlights[i];
          p->lightset->next=li;
          li=p->lightset;
  }
}

/*\end{verbatim} \label{AdminTree.c:RmChainPrim} \begin{verbatim}
 * RmChainPrim: Objekt in den Objektbaum einketten.
 *
 * 	INPUT :	p = Zeiger auf das Objekt.
 */
void
RmChainPrim(p)
	Prim *p;
{
  Topnode *tnode;
  Solid   *snode;
  Comp    *cnode;


  if (RmGlobals.solidblock.primflag) {      /* Wir sind auf Topknotenebene */

	/* Ist der momentane Knoten noch frei ? */
      if (RmGlobals.currenttopnode->csg_tree) { /* nein */
  	/* also neuen knoten erzeugen */
        tnode = (Topnode *)RmMalloc(sizeof(Topnode));
   	/* und f"ullen */
        tnode->csg_tree = (Comp *)p;
        tnode->next = 0;
        tnode->type = TOPNODE;
 	/* und einketten */
        RmGlobals.currenttopnode->next = tnode;
        RmGlobals.currenttopnode = tnode;
      } else {
	/* na dann nur einketten */
        RmGlobals.currenttopnode->csg_tree = (Comp *)p;
      }
  }

  if (RmGlobals.solidblock.solidflag) {      /* Wir sind in einem Solid ! */

	/* Ist der momentane Knoten noch frei ? */
      if (RmGlobals.solidblock.currentsolidnode->prim) { /* nein */
	/* also neuen knoten erzeugen */
        snode = (Solid *)RmMalloc(sizeof(Solid));
	/* und f"ullen */
        snode->prim = p;
        snode->next = 0;
        snode->type = SOLID;
	/* und einketten */
        RmGlobals.solidblock.currentsolidnode->next = snode;
        RmGlobals.solidblock.currentsolidnode = snode;
      } else {
	/* na dann nur einketten */
        RmGlobals.solidblock.currentsolidnode->prim = p;
      }
  }

  if (RmGlobals.solidblock.compflag) {      /* Wir sind in einem CSG ! */
    error(1,"[ChainPrim] No Primitives in CSG allowed!");
  }
}

void
MergeBounds(b1,op,b2)
RtBound b1;
CompType op;
RtBound b2;
{
  switch (op) {

	case DIFFERENCE:
		break;

	case UNION:
		if (b2[0]<b1[0]) b1[0]=b2[0];
		if (b2[1]<b1[1]) b1[1]=b2[1];
		if (b2[2]<b1[2]) b1[2]=b2[2];
		if (b2[3]>b1[3]) b1[3]=b2[3];
		if (b2[4]>b1[4]) b1[4]=b2[4];
		if (b2[5]>b1[5]) b1[5]=b2[5];
		break;

	case INTERSECTION:
                if (b2[0]>b1[0]) b1[0]=b2[0];
                if (b2[1]>b1[1]) b1[1]=b2[1];
                if (b2[2]>b1[2]) b1[2]=b2[2];
                if (b2[3]<b1[3]) b1[3]=b2[3];
                if (b2[4]<b1[4]) b1[4]=b2[4];
                if (b2[5]<b1[5]) b1[5]=b2[5];
                break;
	default: internal(1,"[AdminTree.MergeBounds] unknown csg type");
  }
}


void
MakeBounds(level,node_adr,bound)
        int level;
	ObjType **node_adr;
        RtBound bound;
{
	RtBound tbound;
        Prim *pnode;
        Comp *cnode;
        Solid *snode;
	BBox *bnode;
        ObjType *node = *node_adr;
		RtPoint pt;
		int i,j,k;
		int obcnt;

  switch (*node) {

	case PRIMITIVE:
		pnode = (Prim *)node;

		(* pnode->procs->bound)(pnode,tbound);
                bound[0]=bound[1]=bound[2]=  RI_INFINITY;
		bound[3]=bound[4]=bound[5]= -RI_INFINITY;
		for (i=0;i<2;i++) for (j=0;j<2;j++) for (k=0;k<2;k++) {
			Vx(pt) = tbound[i*3];
			Vy(pt) = tbound[j*3+1];
			Vz(pt) = tbound[k*3+2];
			RmTransformPoint(pt,pnode->OtoW,pt);
			if (Vx(pt) < bound[0]) bound[0]=Vx(pt);
			if (Vy(pt) < bound[1]) bound[1]=Vy(pt);
			if (Vz(pt) < bound[2]) bound[2]=Vz(pt);
			if (Vx(pt) > bound[3]) bound[3]=Vx(pt);
			if (Vy(pt) > bound[4]) bound[4]=Vy(pt);
			if (Vz(pt) > bound[5]) bound[5]=Vz(pt);
                }
		break;

        case SOLID:
                snode = (Solid *)node;
		obcnt=0;

                if (snode->prim) MakeBounds(level+1,&(snode->prim),bound);
		else internal(1,"[AdminTree.MakeBounds] corrupted tree");

                while(snode->next) {
			obcnt++;
                        snode = snode->next;
                        if (snode->prim) {
				MakeBounds(level+1,&(snode->prim),tbound);
				MergeBounds(bound,UNION,tbound);
			}
                }
		if (obcnt>0) { /* more then only one object ? */
			bnode = (BBox *)RmMalloc(sizeof(BBox));
			bnode->type = BOUNDINGBOX;
			for (i=0;i<6;i++) bnode->bound[i] = bound[i];
			bnode->csg_tree = (Comp *)node;
                	*node_adr = (ObjType *)bnode;
		}
                break;

        case COMPOSITE:
                cnode = (Comp *)node;

                if (cnode->left) MakeBounds(level+1,&(cnode->left),bound);
		else internal(1,"[AdminTree.MakeBounds] corrupted tree");

                if (cnode->right) MakeBounds(level+1,&(cnode->right),tbound);
                else internal(1,"[AdminTree.MakeBounds] corrupted tree");

		MergeBounds(bound,cnode->op,tbound);

                if (level == 0) { /* build csg-bb only on toplevel */
                  bnode = (BBox *)RmMalloc(sizeof(BBox));
                  bnode->type = BOUNDINGBOX;
                  for (i=0;i<6;i++) bnode->bound[i] = bound[i];
                  bnode->csg_tree = (Comp *)node;
                  *node_adr = (ObjType *)bnode;
                }
                break;
	default: internal(1,"[AdminTree.MakeBounds] unknown node type");
  }
}

	   

void
RmMakeBounds()
{
        Topnode *root = RmGlobals.ModelRoot;
	RtBound worldext;
	RtBound tbound;

   if (root->csg_tree) MakeBounds(0,&(root->csg_tree),worldext);
   else internal(1,"[AdminTree.RmMakeBounds] empty tree");

   while(root->next) {
     root = root->next;
     if (root->csg_tree) MakeBounds(0,&(root->csg_tree),tbound);
     else internal(1,"[AdminTree.RmMakeBounds] empty tree");
     MergeBounds(worldext,UNION,tbound);
  }
}
  
    
   

/*\end{verbatim} \label{AdminTree.c:RmPrintNode} \begin{verbatim}
 */
void
RmPrintNode(node)
	ObjType *node;
{
	BBox *bnode;
	Prim *pnode;
	Comp *cnode;
	Topnode *tnode;
	Solid *snode;

  switch (*node) {

	case BOUNDINGBOX:
		bnode = (BBox *)node;
		fprintf(stderr,"{");
		if (bnode->csg_tree) RmPrintNode(bnode->csg_tree);
		else fprintf(stderr,"empty");
		fprintf(stderr,"}");
		break;

	case PRIMITIVE:
		pnode = (Prim *)node;
		fprintf(stderr,"%s",(* pnode->procs->name)());
		break;

	case TOPNODE:
		tnode = (Topnode *)node;
		fprintf(stderr,"\nroot\n");
 		if (tnode->csg_tree) RmPrintNode(tnode->csg_tree);
		else fprintf(stderr,"empty");

		while(tnode->next) {
			tnode = tnode->next;
			fprintf(stderr,"\n");
			if (tnode->csg_tree) RmPrintNode(tnode->csg_tree);
			else fprintf(stderr,"empty");
		}
		fprintf(stderr,"\n");
		fflush(stderr);
		break; 

	case SOLID:
		snode = (Solid *)node;
		fprintf(stderr,"[");
 		if (snode->prim) RmPrintNode(snode->prim); 
		else fprintf(stderr,"empty");

		while(snode->next) {
			snode = snode->next;
			fprintf(stderr,",");
			if (snode->prim) RmPrintNode(snode->prim);
			else fprintf(stderr,"empty");
                }
		fprintf(stderr,"]");
                break;

	case COMPOSITE:
		cnode = (Comp *)node;
		fprintf(stderr,"(");

		if (cnode->left) RmPrintNode(cnode->left);
		else fprintf(stderr,"empty");

		switch (cnode->op) {
			case INTERSECTION: fprintf(stderr," & "); break;
			case UNION:        fprintf(stderr," + "); break;
			case DIFFERENCE:   fprintf(stderr," - "); break;
			default:           fprintf(stderr," <%d> ",cnode->op);
		}

		if (cnode->right) RmPrintNode(cnode->right);
		else fprintf(stderr,"empty");

		fprintf(stderr,")");
                break;	
  }
}

/*\end{verbatim} \label{AdminTree.c:RmPrintModelRoot} \begin{verbatim}
 * Print the whole Model Root Object Tree
 */
void
RmPrintModelRoot()
{
  RmPrintNode(RmGlobals.ModelRoot);
}
/*\end{verbatim}*/
