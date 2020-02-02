GCC ?=g++
AR ?=ar

CXX_FLAGS +=--std=c++17 -O3 -I. -I./include -I./include/argparsing -I./include/cdif -Wall -Werror -pedantic -Wextra
LD_FLAGS +=-pthread -lcurl -lcpr 
AR_FLAGS +=rcs

OBJDIR=./obj
LIBDIR=./lib

OBJECTS=
LIBS=${LIBDIR}/librequests.a ${LIBDIR}/libmulti-threading.a ${LIBDIR}/libparameters.a ${LIBDIR}/libsupport.a
EXE_LIB=${LIBDIR}/libdbpp.a

all: ${OBJDIR} ${LIBDIR} ${LIBS} ${OBJECTS} db++

db++: db++.cpp ${OBJECTS} ${EXE_LIB}
	$(GCC) ${CXX_FLAGS} -o $@ $< ${OBJECTS} ${EXE_LIB} ${LD_FLAGS}

.PRECIOUS: ${LIBDIR}/%.a

install: db++
	cp db++ /usr/bin/

${LIBDIR}:
	mkdir -p ${LIBDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${OBJDIR}/%.o: %.cpp %.h
	$(GCC) ${CXX_FLAGS} -c -o $@ $<

${EXE_LIB}: ${LIBS}
	for lib in ${LIBS}; do \
		${AR} -x $$lib; \
	done;
	${AR} ${AR_FLAGS} ${EXE_LIB} *.o
	rm *.o

${LIBDIR}/lib%.a: %
	make -C $<

clean:
	make -C parameters clean
	make -C multi-threading clean
	make -C requests clean
	make -C support clean
	rm -Rf ${OBJDIR}
	rm -Rf ${LIBDIR}
	rm -f db++
