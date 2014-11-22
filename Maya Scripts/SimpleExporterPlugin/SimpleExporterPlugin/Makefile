C++      = g++
CFLAGS   = -O2 -D_BOOL -mcpu=pentiumpro -fno-gnu-keywords -fno-for-scope -DLINUX -DUNIX
INCLUDES = -I. -I/usr/aw/maya5.0/include  `pkg-config --cflags gtk+-2.0`
LIBS     = `pkg-config --cflags --libs gtk+-2.0`
LD       = ./mayald -shared 
COMPILE  = gcc -c $(CFLAGS) $(INCLUDES)
OUTPUT   = SimpleExporter.so
OBJECTS  = MayaMain.o MayaFileTranslator.o
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(LD) -o $(OUTPUT) $(OBJECTS) $(LIBS)
	
MayaFileTranslator.o: MayaFileTranslator.cpp MayaFileTranslator.h
	$(COMPILE) MayaFileTranslator.cpp 
	
MayaMain.o: MayaMain.cpp 
	$(COMPILE) MayaMain.cpp 

clean:
	rm -f $(OBJECTS) $(OUTPUT) 

