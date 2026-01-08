#!/bin/bash

# build -- CHB build script
#
# Copyright (C) 2021 - 2025 andres26
#
# This file is distributed under the terms of the MIT license.
#

build_err() { echo "$*" >&2; exit 2; }
build_warn() { echo "build warning: $*"; }
build_info() { echo "build info: $*"; }
need_arg() { if [ -z "$OPTARG" ]; then build_err "No arg for --$OPT option"; fi; }

build_help() {
	cat << EOF
'$0 -- build/clean system for CHB'
Usage: $0 [t] [h] [b] [d] [c] [?] [v] 

Configure CHB for build/clean

For use this program $0 detect if ./Makefile exists. Please note that to 
compile CHB only $0 is used because $0 sets up all initial variables and 
enables the interface for the cross compiler.

Initially to use $0 it is necessary to configure the program mode, which 
are:

* build -- compile CHB from source
* clean -- delete all CHB compiled files

In addition to this YOU CAN configure the prefix for both host compilers 
and cross compilers

It is recommended that the prefix be defined for the cross compiler because 
on certain types of hosts (e.g. cygwin, msys2 or some linux environment on 
windows) cross compilation is not allowed for ELF executables which is 
necessary to compile CHB correctly.

Notes:
* If you're going to compile CHB for an x86_64 machine, $0 automatically 
compiles it for i386 because CHB doesn't support long mode.

* Note that CHB only supports x86-based machines. (TODO: implement this.)

* If you plan to use the cross-compilation interface, you must have the 
GCC and binutils compilers installed on your system first.

Options:
    -t, --target=[TARGET_PREFIX]
        Sets the cross-compiler prefix so that the compiler you specified 
        is used when compiling.
		
        It is highly recommended to define the cross-compilation interface 
        because some systems do not support compiling to generate ELF format 
        files.
		
    -h, --host=[HOST_PREFIX]
        Set the host machine.
		
        Unlike --target, it is not necessary to specify the host because 
        the script automatically detects the host if it is not specified.
		
    -b, --build
        Build CHB from source.
		
    -c, --clean
        Clean compiled CHB files.
        
    -d, --docs=[MODE]
        build all CHB documentation with Sphinx, current modes supported are:
            - html      -- Generate HTML pages
            - latex     -- Generate .tex file
            - pdflatex  -- Convert .tex to pdf
            - htmlhelp  -- Generate windows CHM format 
            - text      -- Generate text file
            - clean     -- Clean documentation generated files
	
    -v, --version
        Print program version
	
    -?, --help
	    Print Help
EOF
	
}

TARGET=
HOST=
MODE=

while getopts \?t:h:bd:vc-: OPT; do
	if [[ "$OPT" = "-" ]]; then
		OPT="${OPTARG%%=*}"
		OPTARG="${OPTARG#"$OPT"}"
		OPTARG="${OPTARG#=}"
	fi
	
	case "$OPT" in
	t | target)
		need_arg;
		export TARGET="$OPTARG"
	;;
	h | host)
		need_arg;
		export HOST="$OPTARG"
	;;
	b | build)
		MODE=1
	;;
	c | clean)
		MODE=2
	;;
    d | docs)
		export DOCMODE="$OPTARG"
        case "$DOCMODE" in
            "html")
                ;;
            "latex")
                ;;
            "pdflatex")
                ;;
            "htmlhelp")
                ;;
            "text")
                ;;
            "clean")
                ;;
            *)
                build_err "Document mode not supported."
                ;;
        esac
        
        (cd ./ && make doc-$DOCMODE)
        exit 0
	;;
	\? | help)
		build_help;
		exit 2;
	;;
	v | version)
		echo "CHB $0 version 0.06-Prealpha";
	;;
	*)
		build_err "Illegal option --$OPT" ;;
	esac
done
shift $((OPTIND-1))

build_info "Configuring CHB build."

if [ -z $MODE ]; then 
	build_err "build error: program mode is not specified try with --build, --clean options";
fi

if [ -z $HOST ]; then
	build_info "checking host build type: local machine"
	export HOST_CC=gcc
	export HOST_LD=ld
	export HOST_AS=gcc
	export HOST_OBJCOPY=objcopy
	export HOST_OBJDUMP=objdump
	export HOST_AR=ar 
	export HOST_NM=nm
else
	build_info "checking host build type: $HOST"
	export HOST_CC=$HOST-gcc
	export HOST_LD=$HOST-ld
	export HOST_AS=$HOST-gcc
	export HOST_OBJCOPY=$HOST-objcopy
	export HOST_OBJDUMP=$HOST-objdump
	export HOST_AR=$HOST-ar 
	export HOST_NM=$HOST-nm 
fi

echo -n "Checking if $HOST_CC exists. "
if ! command -v $HOST_CC 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_CC not exist"
fi
echo "YES"

echo -n "Checking if $HOST_LD exists. "
if ! command -v $HOST_LD 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_LD not exist"
fi
echo "YES"

echo -n "Checking if $HOST_OBJCOPY exists. "
if ! command -v $HOST_OBJCOPY 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_OBJCOPY not exist"
fi
echo "YES"

echo -n "Checking if $HOST_OBJDUMP exists. "
if ! command -v $HOST_OBJDUMP 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_OBJDUMP not exist"
fi
echo "YES"

echo -n "Checking if $HOST_AR exists. "
if ! command -v $HOST_AR 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_AR not exist"
fi
echo "YES"

echo -n "Checking if $HOST_NM exists. "
if ! command -v $HOST_NM 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $HOST_NM not exist."
fi
echo "YES"

DEFAULTFLAGS='-nostdinc -Wall -Werror -Wextra -fno-builtin -ffunction-sections -fdata-sections -fno-stack-protector -fomit-frame-pointer -fno-exceptions -ffreestanding -fno-exceptions -mno-red-zone -nodefaultlibs -nostdlib'
DEFAULTLDFLAGS='-melf_i386 --gc-sections'
EMPTY=
case $TARGET in
  i[X]86*)
    export target_cpu=i386
	export TARGET_CFLAGS=$DEFAULTFLAGS
  ;;
  x86_64*)
	export target_cpu=x86_64
	export TARGET_CFLAGS=-m32 $DEFAULTFLAGS
	export TARGET_LDFLAGS=$DEFAULTLDFLAGS
  ;;
  *)
	# CHB assumes that $TARGET supports compiling ELF files for i386
	export target_cpu=
	export TARGET_CFLAGS=-m32 $DEFAULTFLAGS
	export TARGET_LDFLAGS=$DEFAULTLDFLAGS
  ;;
esac

# test if target is empty 
if [ -z $TARGET ]; then
	build_warn "Cross compiling is not enabled. Enable with --target option"
	build_info "checking target build type: local machine"
	export TARGET_CC=gcc
	export TARGET_LD=ld
	export TARGET_AS=gcc
	export TARGET_OBJCOPY=objcopy
	export TARGET_OBJDUMP=objdump
	export TARGET_AR=ar 
	export TARGET_NM=nm
else
	build_info "checking target build type: $TARGET"
	export TARGET_CC=$TARGET-gcc
	export TARGET_LD=$TARGET-ld
	export TARGET_AS=$TARGET-gcc
	export TARGET_OBJCOPY=$TARGET-objcopy
	export TARGET_OBJDUMP=$TARGET-objdump
	export TARGET_AR=$TARGET-ar 
	export TARGET_NM=$TARGET-nm 
fi

echo -n "Checking if $TARGET_CC exists. "
if ! command -v $TARGET_CC 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_CC not exist"
fi
echo "YES"

echo -n "Checking if $TARGET_LD exists. "
if ! command -v $TARGET_LD 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_LD not exist"
fi
echo "YES"

echo -n "Checking if $TARGET_OBJCOPY exists. "
if ! command -v $TARGET_OBJCOPY 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_OBJCOPY not exist"
fi
echo "YES"

echo -n "Checking if $TARGET_OBJDUMP exists. "
if ! command -v $TARGET_OBJDUMP 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_OBJDUMP not exist"
fi
echo "YES"

echo -n "Checking if $TARGET_AR exists. "
if ! command -v $TARGET_AR 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_AR not exist"
fi
echo "YES"

echo -n "Checking if $TARGET_NM exists. "
if ! command -v $TARGET_NM 2>&1 >/dev/null; then
	echo "NO"
	build_err "build error: $TARGET_NM not exist."
fi
echo "YES"

export MAKE=make

build_info "Configure finished!"

case $MODE in 
1)
	(cd ./ && $MAKE all)
	;;
2)
	(cd ./ && $MAKE clean)
	;;
*)
	build_err "mode $MODE is not supported"
	;;
esac