GCC ?=g++
AR ?=ar

CXX_FLAGS +=--std=c++17 -O3 -I.. -I../include -I../include/argparsing -I../include/cdif -Wall -Werror -pedantic -Wextra
AR_FLAGS +=rcs

OBJDIR=./obj
LIBDIR=../lib

OBJECTS=${OBJDIR}/logger.o
LIBS=${LIBDIR}/libmulti-threading.a

.PRECIOUS: ${OBJDIR}/%.o

all: ${OBJDIR} ${LIBS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${OBJDIR}/%.o: %.cpp %.h
	$(GCC) ${CXX_FLAGS} -c -o $@ $<

${LIBDIR}/lib%.a: ${OBJECTS}
	$(AR) ${AR_FLAGS} $@ ${OBJECTS}

clean:
	rm -Rf ${OBJDIR}
	rm -Rf ${LIBS}
