#
# Makefile fuer die RenderMan Interface Bibliothek
#
#
# Die folgenden Pfade sind gegebenenfalls zu "andern.
#---------------------------------------------------------------------
INCLUDE = /mo2/ip/include
LIB = /mo2/ip/lib
#---------------------------------------------------------------------
CFLAGS = -g -DDEBUG -I$(INCLUDE)
OBJ =  Ri.o RiError.o RiTokens.o AdminGlobals.o AdminHash.o AdminMem.o AdminStack.o AdminTree.o Math.o MathTrafo.o MathVector.o Misc.o PrimCylinder.o PrimDisk.o PrimPoly.o PrimPolyNet.o PrimSphere.o Intersect.o Trace.o Screen.o Shader.ambientlight.o Shader.distantlight.o Shader.pointlight.o ShaderNoise.o ShaderTexture.o ShaderTools.o Pic.o Stat.o

all:libri.a
libri.a: $(OBJ)
	-rm -f init.c
	./munch $(OBJ) > init.c
	$(CC) -c init.c $(CFLAGS)
	-rm -f libri.a
	ar cq libri.a $(OBJ) init.o
	cp libri.a ../lib
	ranlib ../lib/libri.a
	cp ri.h ../include

install: libri.a
	cp libri.a $(LIB)
	ranlib $(LIB)/libri.a
	cp ri.h $(INCLUDE)

clean:
	rm -f $(OBJ) libri.a init.[co] core

Ri.o: RiTypes.h RiTokens.h RiError.h MathVector.h Admin.h Misc.h Screen.h AdminGlobals.h AdminHash.h AdminMem.h AdminStack.h AdminTree.h Shader.h Prim.h MathTrafo.h Ri.c
	$(CC) -c $(CFLAGS) Ri.c
RiError.o: RiTypes.h RiError.h RiError.c
	$(CC) -c $(CFLAGS) RiError.c
RiTokens.o: RiTypes.h RiTokens.c
	$(CC) -c $(CFLAGS) RiTokens.c
AdminGlobals.o: RiTokens.h MathTrafo.h AdminStack.h AdminGlobals.h Screen.h RiTypes.h Shader.h AdminTree.h Prim.h MathVector.h AdminGlobals.c
	$(CC) -c $(CFLAGS) AdminGlobals.c
AdminHash.o: RiTypes.h Misc.h AdminGlobals.h RiError.h Shader.h AdminTree.h AdminStack.h Prim.h MathVector.h MathTrafo.h AdminHash.c
	$(CC) -c $(CFLAGS) AdminHash.c
AdminMem.o: AdminMem.c
	$(CC) -c $(CFLAGS) AdminMem.c
AdminStack.o: Admin.h Misc.h AdminGlobals.h AdminHash.h AdminMem.h AdminStack.h AdminTree.h RiTypes.h Shader.h Prim.h MathVector.h MathTrafo.h AdminStack.c
	$(CC) -c $(CFLAGS) AdminStack.c
AdminTree.o: AdminGlobals.h RiError.h RiTypes.h Shader.h AdminTree.h AdminStack.h Prim.h MathVector.h MathTrafo.h AdminTree.c
	$(CC) -c $(CFLAGS) AdminTree.c
Math.o: RiTypes.h Math.c
	$(CC) -c $(CFLAGS) Math.c
MathTrafo.o: RiTypes.h Misc.h MathVector.h Math.h RiError.h MathTrafo.c
	$(CC) -c $(CFLAGS) MathTrafo.c
MathVector.o: MathVector.h RiTypes.h MathVector.c
	$(CC) -c $(CFLAGS) MathVector.c
Misc.o: RiTypes.h Misc.c
	$(CC) -c $(CFLAGS) Misc.c
PrimCylinder.o: Prim.h Math.h RiTypes.h Shader.h MathVector.h MathTrafo.h PrimCylinder.c
	$(CC) -c $(CFLAGS) PrimCylinder.c
PrimDisk.o: Prim.h Math.h RiTypes.h Shader.h MathVector.h MathTrafo.h PrimDisk.c
	$(CC) -c $(CFLAGS) PrimDisk.c
PrimPoly.o: RiTokens.h RiError.h Prim.h MathVector.h RiTypes.h Math.h Shader.h MathTrafo.h PrimPoly.c
	$(CC) -c $(CFLAGS) PrimPoly.c
PrimPolyNet.o: RiTokens.h RiError.h Prim.h Math.h Misc.h RiTypes.h Shader.h MathVector.h MathTrafo.h PrimPolyNet.c
	$(CC) -c $(CFLAGS) PrimPolyNet.c
PrimSphere.o: Prim.h Math.h RiTypes.h Shader.h MathVector.h MathTrafo.h PrimSphere.c
	$(CC) -c $(CFLAGS) PrimSphere.c
Intersect.o: Prim.h RiError.h Stat.h RiTypes.h Shader.h MathVector.h MathTrafo.h Intersect.c
	$(CC) -c $(CFLAGS) Intersect.c
Trace.o: RiTypes.h AdminGlobals.h MathVector.h MathTrafo.h Prim.h Shader.h RiError.h AdminTree.h AdminStack.h Trace.c
	$(CC) -c $(CFLAGS) Trace.c
Screen.o: Admin.h Misc.h Screen.h Math.h Stat.h AdminGlobals.h AdminHash.h AdminMem.h AdminStack.h AdminTree.h RiTypes.h Shader.h Prim.h MathVector.h MathTrafo.h Screen.c
	$(CC) -c $(CFLAGS) Screen.c
Shader.ambientlight.o: RiTypes.h RiError.h Shader.h MathVector.h Shader.ambientlight.c
	$(CC) -c $(CFLAGS) Shader.ambientlight.c
Shader.distantlight.o: RiTypes.h RiError.h AdminGlobals.h Shader.h MathVector.h AdminTree.h AdminStack.h Prim.h MathTrafo.h Shader.distantlight.c
	$(CC) -c $(CFLAGS) Shader.distantlight.c
Shader.pointlight.o: RiTypes.h RiError.h AdminGlobals.h Shader.h MathVector.h AdminTree.h AdminStack.h Prim.h MathTrafo.h Shader.pointlight.c
	$(CC) -c $(CFLAGS) Shader.pointlight.c
ShaderNoise.o: RiTypes.h AdminMem.h ShaderNoise.c
	$(CC) -c $(CFLAGS) ShaderNoise.c
ShaderTexture.o: RiTokens.h AdminGlobals.h AdminHash.h RiError.h RiTypes.h Shader.h AdminTree.h AdminStack.h Prim.h MathVector.h MathTrafo.h ShaderTexture.c
	$(CC) -c $(CFLAGS) ShaderTexture.c
ShaderTools.o: MathVector.h Math.h Misc.h Shader.h RiError.h AdminGlobals.h RiTypes.h AdminTree.h AdminStack.h Prim.h MathTrafo.h ShaderTools.c
	$(CC) -c $(CFLAGS) ShaderTools.c
Pic.o: RiTypes.h AdminGlobals.h AdminMem.h Misc.h RiError.h Shader.h AdminTree.h AdminStack.h Prim.h MathVector.h MathTrafo.h Pic.c
	$(CC) -c $(CFLAGS) Pic.c
Stat.o: Stat.h Stat.c
	$(CC) -c $(CFLAGS) Stat.c
