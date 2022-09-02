OSTYPE := $(shell uname -s)

BINARIES= \
	lm_term


CC=gcc
LOCAL_CFLAGS=-g -Wall -fPIC
SHARED=-shared
BUILDDIR=./Build
SRCDIR=./src
TESTDIR=./test
NORTHBDIR=./src/northbound

LOCAL_INCLUDES= -I.

#pthread library must be included on linux.
# See http://sourceware.org/bugzilla/show_bug.cgi?id=10652
ifeq ("${OSTYPE}", "FreeBSD")
	LOCAL_LIBS= -pthread
LOCAL_CFLAGS+=-DNALPBSD
else
	LOCAL_LIBS= -ldl -pthread
endif

LMIWRAPOBJS=\
	$(BUILDDIR)/license_manager_interface.o

NINITWRAPOBJS= \
	$(BUILDDIR)/init.o

NSAWRAPOBJS=  \
	$(BUILDDIR)/dsoNSA.o

NSLWRAPOBJS= \
	$(BUILDDIR)/dsoNSL.o

# PSLWRAPOBJS= \
# 	$(BUILDDIR)/dsoPSL.o

ACTWRAPOBJS= \
	$(BUILDDIR)/dsoSHAFER.o

LMTWRAPOBJS= \
	$(BUILDDIR)/lm_term.o

SMWRAPOBJS= \
	$(BUILDDIR)/stringmapping.o

SWIG_LM_WRAPPNJS= \
	$(BUILDDIR)/license_manager_interface_wrap.o
# ACTPARSEOBJS= \
# 	$(BUILDDIR)/libHelper.o \
# 	$(BUILDDIR)/nalpArgs.o \
# 	$(BUILDDIR)/nalpHelper.o

# PASSPARSEOBJS= \
# 	$(BUILDDIR)/libHelper.o \
# 	$(BUILDDIR)/passArgs.o \
# 	$(BUILDDIR)/passHelper.o

# CKITOBJS = \
# 	$(BUILDDIR)/auxFuncs.o

all_common: $(BINARIES)

clean:
	-@rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)

#
# Executables
# ---------------------------------------------------------------------------- #
lm_term: $(LMTWRAPOBJS) $(BUILDDIR)/license_manager.so
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -g -o lm_term $(LMTWRAPOBJS) $(BUILDDIR)/license_manager.so $(LOCAL_LIBS)


#
# license manager .so file
# --------------------------------------------------------------------------------#
$(BUILDDIR)/license_manager.so: $(LMIWRAPOBJS) $(NSLWRAPOBJS) $(NSAWRAPOBJS) $(ACTWRAPOBJS) $(NINITWRAPOBJS) $(SMWRAPOBJS) $(SMWRAPOBJS)
	$(CC) $(SHARED) -g -o $(BUILDDIR)/license_manager.so $(LMIWRAPOBJS) $(NSLWRAPOBJS) $(NSAWRAPOBJS) $(ACTWRAPOBJS) $(NINITWRAPOBJS) $(SMWRAPOBJS)
#	cp $(BUILDDIR)/license_manager.so /usr/lib64/php/modules/
#
# Object modules
# ----------------------------------------------------------------------------
#
$(BUILDDIR)/license_manager_interface.o: $(SRCDIR)/LicenseMangerInterface/license_manager_interface.c $(SRCDIR)/LicenseMangerInterface/license_manager_interface.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/LicenseMangerInterface/license_manager_interface.c -g -o $(BUILDDIR)/license_manager_interface.o

$(BUILDDIR)/init.o: $(SMWRAPOBJS) $(NSLWRAPOBJS) $(NSAWRAPOBJS) $(ACTWRAPOBJS)
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/init.c -g -o $(BUILDDIR)/init.o

$(BUILDDIR)/dsoNSA.o: $(BUILDDIR)/libHelper.o
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/dso/dsoNSA.c -g -o $(BUILDDIR)/dsoNSA.o

$(BUILDDIR)/dsoNSL.o: $(BUILDDIR)/libHelper.o
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/dso/dsoNSL.c -g -o $(BUILDDIR)/dsoNSL.o

$(BUILDDIR)/dsoSHAFER.o: $(BUILDDIR)/libHelper.o
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/dso/dsoSHAFER.c -g -o $(BUILDDIR)/dsoSHAFER.o

$(BUILDDIR)/libHelper.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/utility/libHelper.c -g -o $(BUILDDIR)/libHelper.o

$(BUILDDIR)/stringmapping.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(NORTHBDIR)/utility/stringmapping.c -g -o $(BUILDDIR)/stringmapping.o

$(BUILDDIR)/lm_term.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(TESTDIR)/lm_term.c -g -o $(BUILDDIR)/lm_term.o

$(BUILDDIR)/license_manager_interface_wrap.o:
	swig -php7 $(TESTDIR)/license_manager_interface.i
#	$(CC) `php-config --includes` -fpic -c $(TESTDIR)/license_manager_interface_wrap.c -o $(BUILDDIR)/license_manager_interface_wrap.o