GCC ?=g++
AR ?=ar

CXX_FLAGS +=--std=c++17 -I. -I./argparsing -I./argparsing/cdif -I../cpr/include -L../cpr/lib -Wall -Werror -pedantic -Wextra
LD_FLAGS +=-pthread -lcurl -lcpr 
AR_FLAGS +=rcs

OBJDIR=./obj
LIBDIR=./lib

OBJECTS=${OBJDIR}/requestfactory.o ${OBJDIR}/logger.o
LIBS=

db++: db++.cpp ${OBJDIR} ${LIBDIR} ${OBJECTS}
	$(GCC) ${CXX_FLAGS} -o $@ $< ${OBJECTS} ${LD_FLAGS} ${LIBS}

install: db++
	cp db++ /usr/bin/

${LIBDIR}:
	mkdir -p ${LIBDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${OBJDIR}/%.o: %.cpp %.h
	$(GCC) ${CXX_FLAGS} -c -o $@ $<

${LIBDIR}/lib%.a: %
	$(AR) ${AR_FLAGS} $@ %/${OBJDIR}/*

%: ./%/
	make -C $@

clean:
	rm -Rf ${OBJDIR}
	rm -Rf ${LIBDIR}
	rm -f db++
