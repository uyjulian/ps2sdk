/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003  Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include <kernel.h>
#include <stdio.h>
#include <stdint.h>
#include <stdnoreturn.h>  // noreturn
#include <string.h>
#include <startup.h>

extern char* _end;
extern char* _heap_size;
extern char* _fbss;
extern char* _stack;
extern char* _stack_size;

__attribute__((weak)) void _ps2sdk_memory_init();
__attribute__((weak)) void _init();
__attribute__((weak)) void _fini();

void _libcglue_args_parse(int argc, char** argv);
void _libcglue_init();
void _libcglue_deinit();
int main(int argc, char** argv);

static struct sargs args;
static struct sargs_start *args_start;

// We need to do a backup of args_start and _args because memset will wipe it up.
static void clear_bss_area()
{
	struct sargs args_backup;
	struct sargs_start args_start_backup;

	// Backup
	memcpy(&args_backup, &args, sizeof(struct sargs));
	memcpy(&args_start_backup, args_start, sizeof(struct sargs_start));

	// Wipe
	memset(&_fbss, 0, &_end - &_fbss);

	// Restore
	memcpy(&args, &args_backup, sizeof(struct sargs));
	memcpy(args_start, &args_start_backup, sizeof(struct sargs_start));
}

/*
 * Intermediate function between _start and main, stack can be used as normal.
 */
static void _main()
{
    int retval;
    struct sargs *pa;

    // clear bss area
    clear_bss_area();

    // initialize heap
    SetupHeap(&_end, (int)&_heap_size);

    // writeback data cache
    FlushCache(0);

    // Capability to override 32MiB on DESR/DVR models
    // NOTE: this call can restart the application
    if (_ps2sdk_memory_init)
        _ps2sdk_memory_init();
    
    // Use arguments sent through start if sent (by ps2link for instance)
    pa = &args;
    if (args.argc == 0 && args_start != NULL && args_start->args.argc != 0)
        pa = &args_start->args;

    // call libcglue argument parsing
	_libcglue_args_parse(pa->argc, pa->argv);

    // initialize libcglue
    _libcglue_init();

    // call global constructors (weak)
    if (_init)
        _init();

    // Initialize the kernel (Apply necessary patches).
    _InitSys();

    // Enable interruts
    EI();

    // call main
    retval = main(pa->argc, pa->argv);

    // call global destructors (weak)
    if (_fini)
        _fini();

    // uninitialize libcglue
    _libcglue_deinit();

    // Exit
    Exit(retval);
}

noreturn void _exit(int status)
{
	// call global destructors (weak)
    if (_fini)
        _fini();

    // uninitialize libcglue
    _libcglue_deinit();

    // Exit
    Exit(status);
}

/*
 * First function to be called by the loader
 * This function sets up the stack and heap.
 * DO NOT USE THE STACK IN THIS FUNCTION!
 */
void __start(struct sargs_start *pargs)
{
    asm volatile(
        "# Save first argument  \n"
        "la     $2, %0 \n"
        "sw     $4, ($2)        \n"
        "                       \n"
        "# SetupThread          \n"
        "la     $4, _gp         \n"
        "la     $5, _stack      \n"
        "la     $6, _stack_size \n"
        "la     $7, %1	        \n"
        "la     $8, ExitThread  \n"
        "move   $gp, $4         \n"
        "addiu  $3, $0, 60      \n"
        "syscall                \n"
        "move   $sp, $2         \n"
        "                       \n"
        "# Jump to _main      	\n"
        "j      %2           \n"
		: /* No outputs. */
		: "R"(args_start), "R"(args), "Csy"(_main));

    // Prevent gcc noreturn warning
    while(1){}
}