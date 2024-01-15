
#include "all_include.h"

int analize_arguments(const Opttable *dopttable, int argc, char **argv)
{
	Opt_strings *optstr;
	const char *opt;
	Opttable *otp;
	Opttable *igadd;
	Opttable *opttable;
	char *cp;
	char **argvp;
	char **nargv;
	int nargc;
	int i;
	int argca;
	char **argva;

	for ( i = 0; dopttable[i].option; ++i )
		;
	opttable = (Opttable *)__builtin_alloca((argc + i) * sizeof(Opttable));
	memset(opttable, 0, (argc + i) * sizeof(Opttable));
	memcpy(opttable, dopttable, i * sizeof(Opttable));
	igadd = &opttable[i];
	nargv = (char **)__builtin_alloca((argc + 1) * sizeof(char *));
	memset(nargv, 0, (argc + 1) * sizeof(char *));
	argvp = argv;
	*nargv = *argv;
	nargc = 1;
	argva = argv + 1;
	for ( argca = argc - 1; ; --argca )
	{
		if ( argca <= 0 )
		{
			for ( i = 0; i < nargc + 1; ++i )
				argvp[i] = nargv[i];
			for ( i = 0; opttable[i].option; ++i )
			{
				if ( opttable[i].vartype == 108 )
				{
					*(SLink **)opttable[i].var = ring_to_liner(*(SLink **)opttable[i].var);
				}
			}
			return nargc;
		}
		if ( **argva == '-' )
		{
			opt = 0;
			for ( i = 0; opttable[i].option; ++i )
			{
				if ( opttable[i].havearg == 3 )
				{
					if ( !strcmp(opttable[i].option, *argva) )
						break;
				}
				else
				{
					if ( !strncmp(opttable[i].option, *argva, strlen(opttable[i].option)) )
						break;
				}
			}
			if ( !opttable[i].option )
				return -1;
			if ( opttable[i].havearg && opttable[i].havearg != 3 )
			{
				if ( opttable[i].havearg == 4 || (*argva)[strlen(opttable[i].option)] )
				{
					opt = &(*argva)[strlen(opttable[i].option)];
				}
				else if ( argca > 1 )
				{
					opt = argva[1];
					++argva;
					--argca;
				}
			}
			if ( opttable[i].havearg != 2 || opt )
			{
				switch ( opttable[i].vartype )
				{
					case 'F':
					case 'f':
						if ( (*argva)[strlen(opttable[i].option)] )
						{
							*(uint32_t *)opttable[i].var = strtoul(&(*argva)[strlen(opttable[i].option)], NULL, 16);
						}
						else
						{
							*(uint32_t *)opttable[i].var = (opttable[i].vartype == 'f') ? 1 : 0;
						}
						break;
					case 'h':
						*(uint32_t *)opttable[i].var = strtoul(opt, 0, 16);
						break;
					case 'i':
						for ( otp = igadd; opttable < otp; --otp )
							*otp = otp[-1];
						++igadd;
						opttable->option = opt;
						opttable->vartype = 110;
						opttable->havearg = 3;
						cp = index(opttable->option, ':');
						if ( cp )
						{
							*cp = 0;
							switch ( cp[1] )
							{
								case 'c':
									opttable->havearg = 4;
									break;
								case 'n':
									opttable->havearg = 2;
									break;
								case 'o':
									opttable->havearg = 1;
									break;
								default:
									break;
							}
						}
						break;
					case 'l':
						optstr = (Opt_strings *)calloc(1u, sizeof(Opt_strings));
						optstr->string = opt;
						*(SLink **)opttable[i].var = add_ring_tail(*(SLink **)opttable[i].var, (SLink *)optstr);
						break;
					case 'n':
						break;
					case 's':
						*(const char **)opttable[i].var = opt;
						break;
					default:
						fprintf(stderr, "internal error\n");
						return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			nargv[nargc++] = *argva;
		}
		++argva;
	}
}

