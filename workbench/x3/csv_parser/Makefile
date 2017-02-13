SHELL := /bin/bash
PATH := $(HOME)/bin:$(PATH)
PPROF.install := $(HOME)/dwnlds/gperftools/build/install
HOW=clangxx3_9_pkg
#HOW=gcc6_20151018
BENCH.main=csv
ifdef USE_MACRO_INCLUDE
#invoke make with something like:
#  make USE_MACRO_INCLUDE=1
#to enable this branch of ifdef.
include MACRO_INCLUDE.imk
else
DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
include $(DIR.root)/root.imk

MACRO_INCLUDE.imk:
	echo "#Generated from Makefile with target:$@.">$@
	echo "#  Change values as appropriate for your environment.">>$@
	echo "COMPILER.$(HOW):=$(COMPILER.$(HOW))">>$@
	echo "boost.root:=$(boost.root)">>$@
	echo "nonius:=$(nonius)">>$@
	echo "boost.spirit.fork:=$(boost.spirit.fork)">>$@
endif
LINK.opts=-L /lib64 -l pthread
#The above from:
#  http://stackoverflow.com/questions/25617839/undefined-reference-to-symbol-pthread-key-deleteglibc-2-2-5
#to solve problem when previous version of this Makefile run.
INCS:= \
 -I$(boost.root) \
 -I$(nonius) \
  $(INCS) \
  ###

.PHONY: show help compiler runs means

show:
	@echo $(INCS)
help:
	$(COMPILE.$(HOW)) -help

compiler:
	$(COMPILER.$(HOW)) --version

OPTS.optim=-O3
#OPTS.optim=-g
#-DBOOST_SPIRIT_X3_DEBUG
OPTS.spirit=$(INCS)
OPTS.fork=-I$(boost.spirit.fork) $(INCS)

define bench_make
nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).exe: nonius-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z $(OPTS.optim) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) $(LINK.opts) nonius-$(BENCH.main).cpp -o $$@
nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).out: nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).exe
	./$$< -o $$@
	cat $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-prof.exe: gprof-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z -pg -O0 \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) $(LINK.opts) gprof-$(BENCH.main).cpp -o $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-gmon.out: gprof-$(BENCH.main)-$(1)-$(2).GET$(3)-XRULE$(4)-$(HOW)-prof.exe
	./$$<
	mv gmon.out gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-gmon.out
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-gprof.out: gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-gmon.out
	gprof --demangle gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW)-prof.exe ./$$< > $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).exe: pprof-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z -O0 \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  -I$(PPROF.install)/include \
	  -L $(PPROF.install)/lib -l profiler \
	  $(OPTS.$(2)) $(LINK.opts) pprof-$(BENCH.main).cpp -o $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).prof: pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).exe
	  ./$$< $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).text: \
  pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).exe\
  pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-$(HOW).prof
	  $(PPROF.install)/bin/pprof --text $$^ > $$@
endef

BENCH_INCS:=qi x3_ns_rdef x3_crtp
VERSIONS:=spirit fork
GET_RHS:=0 1
XFORM_IN_RULE:=0 1
$(foreach inc,$(BENCH_INCS),\
$(foreach ver,$(VERSIONS),\
$(foreach get,$(GET_RHS),\
$(foreach xrule,$(XFORM_IN_RULE),\
$(eval $(call bench_make,$(inc),$(ver),$(get),$(xrule)))))))

BENCHS=\
  nonius-$(BENCH.main)-qi-spirit-GET0-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-$(HOW)-bench.out\
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET0-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET0-XRULE1-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET1-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET1-XRULE1-$(HOW).out\
  nonius-$(BENCH.main)-x3_crtp-fork-GET1-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_crtp-fork-GET1-XRULE1-$(HOW).out\
  ###
BENCHS= \
  nonius-$(BENCH.main)-qi-spirit-GET0-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-$(HOW).out\
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET1-XRULE0-$(HOW).out \
  nonius-$(BENCH.main)-x3_ns_rdef-fork-GET1-XRULE1-$(HOW).out \
  nonius-$(BENCH.main)-x3_crtp-fork-GET1-XRULE1-$(HOW).out \
  ###
benchs: $(BENCHS)

GPROFS=\
  gprof-$(BENCH.main).x3_ns_rdef.fork.GET1.XRULE0-$(HOW).gprof.out\
  #gprof-$(BENCH.main).x3_ns_rdef.fork.GET1.XRULE1-$(HOW).gprof.out\
  ###
gprofs: $(GPROFS)

PPROFS=\
  pprof-csv-x3_ns_rdef-fork-GET1-XRULE0-$(HOW).text\
  pprof-csv-qi-spirit-GET0-XRULE0-$(HOW).text\
  ###
pprofs: $(PPROFS)

means: $(BENCHS)
	grep -e '^mean: ' nonius-$(BENCH.main)-*-$(HOW).out > means-$(BENCH.main).out
	cat means-$(BENCH.main).out

test:
	$(COMPILER.$(HOW)) -DBENCH_INC=test.hpp -std=c++1z -pg test.cpp -o test.exe
	./test.exe
	#gprof test.exe > $@.gprof_out

prof_grep:
	perl filt_gprof_flat.pl < nonius-$(BENCH.main)-x3_ns_rdef.spirit.GET0.XRULE0-$(HOW).gprof.out



