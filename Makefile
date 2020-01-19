GCC ?=g++
AR ?=ar

CXX_FLAGS +=--std=c++17 -I. -I./include -I./include/argparsing -I./include/cdif -I../cpr/include -L../cpr/lib -Wall -Werror -pedantic -Wextra
LD_FLAGS +=-pthread -lcurl -lcpr 
AR_FLAGS +=rcs

OBJDIR=./obj
LIBDIR=./lib

OBJECTS=${OBJDIR}/fileio.o
LIBS=${LIBDIR}/librequests.a ${LIBDIR}/libmulti-threading.a ${LIBDIR}/libparameters.a

all: ${OBJDIR} ${LIBDIR} ${LIBS} ${OBJECTS} db++

db++: db++.cpp ${OBJECTS} ${LIBS}
	$(GCC) ${CXX_FLAGS} -o $@ $< ${OBJECTS} ${LIBS} ${LD_FLAGS}

install: db++
	cp db++ /usr/bin/

${LIBDIR}:
	mkdir -p ${LIBDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${OBJDIR}/%.o: %.cpp %.h
	$(GCC) ${CXX_FLAGS} -c -o $@ $<

${LIBDIR}/lib%.a: %
	make -C $<

clean:
	make -C parameters clean
	make -C multi-threading clean
	make -C requests clean
	rm -Rf ${OBJDIR}
	rm -Rf ${LIBDIR}
	rm -f db++
