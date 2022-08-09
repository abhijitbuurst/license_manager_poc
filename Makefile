OSTYPE := $(shell uname -s)

BINARIES= \
	license_manager_interface


CC=gcc
LOCAL_CFLAGS=-g -Wall
BUILDDIR=./Build
SRCDIR=./src
TESTDIR=./test

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
# lm_term: 


#
# license manager .so file
# --------------------------------------------------------------------------------#
license_manager_interface: $(LMIWRAPOBJS) $(NSLWRAPOBJS) $(NSAWRAPOBJS) $(ACTWRAPOBJS) $(NINITWRAPOBJS)
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -o license_manager_interface $(LMIWRAPOBJS) $(NSLWRAPOBJS) $(NSAWRAPOBJS) $(ACTWRAPOBJS) $(NINITWRAPOBJS) $(LOCAL_LIBS)
#
# Object modules
# ----------------------------------------------------------------------------
#
$(BUILDDIR)/license_manager_interface.o: $(SRCDIR)/LicenseMangerInterface/license_manager_interface.c $(SRCDIR)/LicenseMangerInterface/license_manager_interface.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/LicenseMangerInterface/license_manager_interface.c -o $(BUILDDIR)/license_manager_interface.o

$(BUILDDIR)/init.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/northbound/init.c -o $(BUILDDIR)/init.o

$(BUILDDIR)/dsoNSA.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/northbound/dsoNSA.c -o $(BUILDDIR)/dsoNSA.o

$(BUILDDIR)/dsoNSL.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/northbound/dsoNSL.c -o $(BUILDDIR)/dsoNSL.o

$(BUILDDIR)/dsoSHAFER.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/northbound/dsoSHAFER.c -o $(BUILDDIR)/dsoSHAFER.o

$(BUILDDIR)/libHelper.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(SRCDIR)/northbound/libHelper.c -o $(BUILDDIR)/libHelper.o

$(BUILDDIR)/lm_term.o:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_INCLUDES) -c $(TESTDIR)/lc_term.c -o $(BUILDDIR)/lc_term.o

