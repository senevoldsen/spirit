SHELL := /bin/bash
PATH := $(HOME)/bin:$(PATH)
#^The above is build from:
##  https://github.com/gperftools/gperftools.git
HOW=clangxx3_9_pkg
HOW=gcc6_20151018
BENCH.main=csv
MAIN=trace-csv
trace-csv.opts_compile.cmd=-DBENCH_INC=x3_ns_rdef.hpp
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
	echo "nonius.inc:=$(nonius.inc)">>$@
	echo "#^See https://github.com/rmartinho/nonius.git">>$@
	echo "pprof.install:=$(pprof.install)">>$@
	echo "#^Built from https://github.com/gperftools/gperftools.git">>$@
	echo "boost.spirit.fork:=$(boost.spirit.fork)">>$@
	echo "#^develop branch of https://github.com/cppljevans/spirit.git">>$@
endif
LINK.nonius.opts=-L /lib64 -l pthread
  #The above from:
  #  http://stackoverflow.com/questions/25617839/undefined-reference-to-symbol-pthread-key-deleteglibc-2-2-5
  #to solve problem when previous version of this Makefile run.
  ###
COMPILE.gprof=-pg -g -static-libgcc -static-libstdc++
  #In the above:
  #  The -pg is required by:
  #    https://sourceware.org/binutils/docs/gprof/Compiling.html#Compiling    
  #  The -static-lib* is justified by:
  #      https://sourceware.org/binutils/docs/gprof/Compiling.html#Compiling
  #    to avoid empty output when gprof run with the --annotated-source
  #    and 0.0 times otherwise.
  #  The -g is to allow --annotated-source to work as explained here:
  #    https://sourceware.org/ml/binutils/2008-12/msg00068.html
  ###
INCS:= \
 -I$(boost.root) \
 -I$(nonius.inc) \
  $(INCS) \
  ###

.PHONY: show help compiler runs means

help:
	$(COMPILE.$(HOW)) -help

compiler:
	$(COMPILER.$(HOW)) --version

#-DBOOST_SPIRIT_X3_DEBUG
OPTS.spirit=-I$(boost.root)
OPTS.fork=-I$(boost.spirit.fork) $(OPTS.spirit)

define bench_make
nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe: nonius-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z -$(5) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) -I$(nonius.inc) $(LINK.nonius.opts) $$< -o $$@
nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).out: nonius-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe
	./$$< -o $$@
	cat $$@
chrono-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe: chrono-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z -$(5) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) $$< -o $$@
chrono-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).out: chrono-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe
	./$$< > $$@
	cat $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe: gprof-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z $(COMPILE.gprof) -$(5) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) $$< -o $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).gmon: gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe
	./$$<
	mv gmon.out $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).annot: gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).gmon
	gprof --demangle --line --annotated-source -x gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe $$< | c++filt > $$@
gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).flat_graph: gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).gmon
	gprof --demangle --line gprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe $$< | c++filt > $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe: pprof-$(BENCH.main).cpp $(1).hpp input.hpp
	$(COMPILER.$(HOW)) -std=c++1z -$(5) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  -I$(pprof.install)/include \
	  -L $(pprof.install)/lib -l profiler \
	  $(OPTS.$(2)) $$< -o $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).prof: pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe
	  ./$$< $$@
pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).text: \
  pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).exe\
  pprof-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).prof
	  $(pprof.install)/bin/pprof --text $$^ > $$@
gcov-$(BENCH.main)-$(1)-$(2)-GET$(3)-XRULE$(4)-Opt$(5)-$(HOW).dir: gcov-$(BENCH.main).cpp $(1).hpp input.hpp
	mkdir -p $$@
	-rm $$@/*.gcov $$@/gcov-$(BENCH.main).gcda $$@/gcov-$(BENCH.main).gcno
	cd $$@ ; \
	$(COMPILER.$(HOW)) -std=c++1z -fprofile-arcs -ftest-coverage -$(5) \
	  -DBENCH_INC=$(1).hpp \
	  -DBOOST_SPIRIT_GET_RHS_CRTP=$(3) \
	  -DBOOST_SPIRIT_ATTR_XFORM_IN_RULE=$(4) \
	  $(OPTS.$(2)) ../$$< -o gcov-$(BENCH.main).exe; \
	./gcov-$(BENCH.main).exe ; \
	gcov --demangled-names --object-file gcov-$(BENCH.main).exe ../$$< ; \
	rm gcov-$(BENCH.main).exe
endef

BENCH_INCS:=qi x3_ns_rdef
VERSIONS:=spirit fork
GET_RHS:=0 1
OPTIM:=O0 O1 O2 O3
XFORM_IN_RULE:=0 1
$(foreach inc,$(BENCH_INCS),\
$(foreach ver,$(VERSIONS),\
$(foreach get,$(GET_RHS),\
$(foreach xrule,$(XFORM_IN_RULE),\
$(foreach optim,$(OPTIM),\
$(eval $(call bench_make,$(inc),$(ver),$(get),$(xrule),$(optim))))))))

MAIN_FLAGS.bench=\
  $(BENCH.main)-qi-spirit-GET0-XRULE0-OptO0-$(HOW)\
  $(BENCH.main)-qi-spirit-GET0-XRULE0-OptO1-$(HOW)\
  $(BENCH.main)-qi-spirit-GET0-XRULE0-OptO2-$(HOW)\
  $(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-OptO0-$(HOW)\
  $(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-OptO1-$(HOW)\
  $(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-OptO2-$(HOW)\
  ###

BENCHS.nonius=$(addprefix nonius-,$(addsuffix .out,$(MAIN_FLAGS.bench)))
benchs.nonius: $(BENCHS.nonius)

summary.nonius: benchs.nonius
	grep -e '^mean: ' nonius-$(BENCH.main)-*-$(HOW).out > summary.nonius-$(BENCH.main)-$(HOW).out
	cat summary.nonius-$(BENCH.main)-$(HOW).out

BENCHS.chrono=$(addprefix chrono-,$(addsuffix .out,$(MAIN_FLAGS.bench)))
  ###
benchs.chrono: $(BENCHS.chrono)

summary.chrono: $(BENCHS.chrono)
	grep -e '^mean: ' chrono-$(BENCH.main)-*-$(HOW).out > summary.chrono-$(BENCH.main)-$(HOW).out
	cat summary.chrono-$(BENCH.main)-$(HOW).out

MAIN_FLAGS.profile=\
  $(BENCH.main)-qi-spirit-GET0-XRULE0-OptO1-$(HOW)\
  $(BENCH.main)-x3_ns_rdef-spirit-GET0-XRULE0-OptO1-$(HOW)\
  ###

FLAT_GRAPHS=$(addprefix gprof-,$(addsuffix .flat_graph,$(MAIN_FLAGS.profile)))
flat_graphs: $(FLAT_GRAPHS)

ANNOTS=$(addprefix gprof-,$(addsuffix .annot,$(MAIN_FLAGS.profile)))
annots: $(ANNOTS)

PPROFS=$(addprefix pprof-,$(addsuffix .text,$(MAIN_FLAGS.profile)))
pprofs: $(PPROFS)

GCOVS=$(addprefix gcov-,$(addsuffix .dir,$(MAIN_FLAGS.profile)))
gcovs: $(GCOVS)

test:
	$(COMPILER.$(HOW)) -DBENCH_INC=test.hpp -std=c++1z -pg test.cpp -o test.exe
	./test.exe
	#gprof test.exe > $@.gprof_out

prof_grep:
	perl filt_gprof_flat.pl < nonius-$(BENCH.main)-x3_ns_rdef.spirit.GET0.XRULE0-OptO0-$(HOW).gprof.out



