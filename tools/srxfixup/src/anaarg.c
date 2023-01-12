
#include "all_include.h"

int analize_arguments(Opttable *dopttable, int argc, char **argv)
{
	size_t v5;
	SLink *v7;
	unsigned int v8;
	size_t v9;
	unsigned int v10;
	char v11;
	SLink *v13;
	char **v14;
	int v15;
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
					v13 = ring_to_liner(*(SLink **)opttable[i].var);
					*(SLink **)opttable[i].var = v13;
				}
			}
			return nargc;
		}
		if ( **argva == '-' )
			break;
		nargv[nargc++] = *argva;
LABEL_50:
		++argva;
	}
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
			v5 = strlen(opttable[i].option);
			if ( !strncmp(opttable[i].option, *argva, v5) )
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
				if ( (*argva)[strlen(opttable[i].option)] )
					goto LABEL_32;
				*(uint32_t *)opttable[i].var = 0;
				goto LABEL_50;
			case 'f':
				if ( (*argva)[strlen(opttable[i].option)] )
				{
LABEL_32:
					v15 = 16;
					v14 = 0;
					v9 = strlen(opttable[i].option);
					v10 = strtoul(&(*argva)[v9], v14, v15);
					*(uint32_t *)opttable[i].var = v10;
				}
				else
				{
					*(uint32_t *)opttable[i].var = 1;
				}
				goto LABEL_50;
			case 'h':
				v8 = strtoul(opt, 0, 16);
				*(uint32_t *)opttable[i].var = v8;
				goto LABEL_50;
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
					v11 = cp[1];
					if ( v11 == 110 )
					{
						opttable->havearg = 2;
					}
					else if ( v11 > 110 )
					{
						if ( v11 == 111 )
							opttable->havearg = 1;
					}
					else if ( v11 == 99 )
					{
						opttable->havearg = 4;
					}
				}
				goto LABEL_50;
			case 'l':
				optstr = (Opt_strings *)calloc(1u, sizeof(Opt_strings));
				optstr->string = opt;
				v7 = add_ring_tail(*(SLink **)opttable[i].var, (SLink *)optstr);
				*(SLink **)opttable[i].var = v7;
				goto LABEL_50;
			case 'n':
				goto LABEL_50;
			case 's':
				*(const char **)opttable[i].var = opt;
				goto LABEL_50;
			default:
				fprintf(stderr, "internal error\n");
				return -1;
		}
	}
	return -1;
}

