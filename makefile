PROJECTNAME=lisp
DESCRIPTION="LISP interpreter based in JSON."
DEPENDS=g++

MAJOR=0
MINOR=1
PATCH=0

FLAGS=-std=c++17 -Wall -Werror -pedantic -O2
INCS=
LIBS=-lvar

OBJS= \
obj/Lisp.o \


#bin|exe|so|dll
TARGET=bin

#Don't change from this point below or you'll suffer :(
ifeq ($(TARGET), bin)
EXTENSION=
BINARY=$(PROJECTNAME)
PACKAGE=$(PROJECTNAME)
TOOLCHAIN=
ALL=$(OUTPUT)
OUTPUT=$(OUTPUTDIR)/$(PACKAGE)
endif

ifeq ($(TARGET), exe)
EXTENSION=.exe
BINARY=$(PROJECTNAME)
PACKAGE=$(PROJECTNAME)
TOOLCHAIN=x86_64-w64-mingw32-
ALL=$(OUTPUT)

endif


OUTPUTDIR=output/$(TARGET)


GCC=$(TOOLCHAIN)gcc
GPP=$(TOOLCHAIN)g++

.PHONY: all deb so dll clean

#General purpouse
all: $(OUTPUT)

clean:
	rm -rf obj $(OUTPUT) output dist

obj/%.o: src/%.cpp include/$(PROJECT)/%.hpp
	mkdir -p obj
	$(GPP) $< -I include $(FLAGS) -c -fPIC -o $@

$(OUTPUT): $(OBJS) lisp.cpp
	mkdir -p $(OUTPUTDIR)
	$(GPP) -I include lisp.cpp $(OBJS) $(LIBS) -o $(OUTPUT)

#Linux Packing
PACKAGEDEV=$(PACKAGE)-dev
DEPENDSDEV="$(PACKAGEDEV), $(DEPENDS)"

dist: deb

deb: $(OUTPUT)
	mkdir -p deb/bin
	cp $(OUTPUT) deb/bin

	mkdir -p deb/DEBIAN

	touch deb/DEBIAN/control
	cat unix/control.in | sed \
	 -e s/@PACKAGE/${PACKAGE}/g \
	 -e s/@DESCRIPTION/${DESCRIPTION}/g \
	 -e s/@MAJOR/${MAJOR}/g \
	 -e s/@MINOR/${MINOR}/g \
	 -e s/@PATCH/${PATCH}/g \
	 -e s/@DEPENDS/${DEPENDS}/g \
	 -e s/@SIZE/$(shell du -c --apparent-size so/* --exclude=DEBIAN | grep total | sed 's/[ \t]*total//')/g > deb/DEBIAN/control

	mkdir -p dist
	dpkg -b deb/ dist

	rm -rf deb
