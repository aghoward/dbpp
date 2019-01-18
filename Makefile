GCC ?=g++
AR ?=ar

CXX_FLAGS +=--std=c++17 -I. -I./argparsing -I./argparsing/cdif -I../cpr/include -L../cpr/lib -Wall -Werror -pedantic -Wextra
LD_FLAGS +=-pthread -lcurl -lcpr 
AR_FLAGS +=rcs

OBJDIR=./obj
LIBDIR=./lib

OBJECTS=
LIBS=

db++: db++.cpp
	$(GCC) ${CXX_FLAGS} -o $@ $< ${LD_FLAGS} ${LIBS}

${LIBDIR}:
	mkdir -p ${LIBDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${OBJDIR}/%.o: %.cpp %.h ${OBJDIR}
	$(GCC) ${CXX_FLAGS} -c -o $@ $<

${LIBDIR}/lib%.a: % ${LIBDIR}
	$(AR) ${AR_FLAGS} $@ %/${OBJDIR}/*

%: ./%/
	make -C $@

clean:
	for lib in ${LIBS}
	do
		make -C ${lib} clean
	done

	rm -Rf ${OBJDIR}
	rm -Rf ${LIBDIR}
