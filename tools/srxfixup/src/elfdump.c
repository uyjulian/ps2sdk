
#include "all_include.h"

struct rellink 
{
	int rid; 
	elf_rel *rp; 
	elf_rel *mhrp;
};
struct name2num 
{
	srxfixup_const_char_ptr_t name; 
	int num;
};

static void search_rel_section(elf_file *elf, elf_section *scp, elf_rel **result, int *relentries, unsigned int *baseoff);
static void search_rel_data(elf_rel *rpbase, int relentries, unsigned int addr, struct rellink *result);
static void dumpb(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, uint8_t *data);
static void dumph(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, uint16_t *data);
static void dumpw(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, unsigned int *data);
static srxfixup_const_char_ptr_t num2name(struct name2num *table, int num);

void print_elf(elf_file *elf, int flag)
{
	if ( elf == NULL )
	{
		return;
	}
	print_elf_ehdr(elf, flag);
	print_elf_phdr(elf, flag);
	print_elf_sections(elf, flag);
	if ( (flag & 0x100) != 0 )
	{
		Elf_file_slot *order;

		order = build_file_order_list(elf);
		dump_file_order_list(elf, order);
		free(order);
	}
}

struct name2num Ei_class_name[] = { { "ELFCLASSNONE", 0 }, { "ELFCLASS32", 1 }, { "ELFCLASS64", 2 }, { NULL, 0 } };
struct name2num E_type_name[] =
{
	{ "ET_NONE", 0 },
	{ "ET_REL", 1 },
	{ "ET_EXEC", 2 },
	{ "ET_DYN", 3 },
	{ "ET_CORE", 4 },
	{ "ET_SCE_IOPRELEXEC", 0xFF80 },
	{ "ET_SCE_IOPRELEXEC2", 0xFF81 },
	{ "ET_SCE_EERELEXEC", 0xFF90 },
	{ "ET_SCE_EERELEXEC2", 0xFF91 },
	{ NULL, 0 }
};
struct name2num Ei_data_name[] =
{
	{ "ELFDATANONE", 0 },
	{ "ELFDATA2LSB", 1 },
	{ "ELFDATA2MSB", 2 },
	{ NULL, 0 }
};
struct name2num E_version_name[] = { { "EV_NONE", 0 }, { "EV_CURRENT", 1 }, { NULL, 0 } };
struct name2num E_machine_name[] =
{
	{ "EM_NONE", 0 },
	{ "EM_M32", 1 },
	{ "EM_SPARC", 2 },
	{ "EM_386", 3 },
	{ "EM_68K", 4 },
	{ "EM_88K", 5 },
	{ "EM_860", 7 },
	{ "EM_MIPS", 8 },
	{ "EM_MIPS_RS4_BE", 10 },
	{ "EM_SPARC64", 11 },
	{ "EM_PARISC", 15 },
	{ "EM_SPARC32PLUS", 18 },
	{ "EM_PPC", 20 },
	{ "EM_SH", 42 },
	{ NULL, 0 }
};

void print_elf_ehdr(elf_file *elf, int flag)
{
	if ( (flag & 1) == 0 )
	{
		return;
	}
	printf(" Elf header\n");
	printf(
		"   e_ident = 0x%02x+\"%c%c%c\", class = %s, encode = %s\n",
		elf->ehp->e_ident[0],
		elf->ehp->e_ident[1],
		elf->ehp->e_ident[2],
		elf->ehp->e_ident[3],
		num2name(Ei_class_name, elf->ehp->e_ident[4]),
		num2name(Ei_data_name, elf->ehp->e_ident[5]));
	printf("             version = %s\n", num2name(E_version_name, elf->ehp->e_ident[6]));
	printf(
		"   e_type = %s, e_machine = %s, e_version = %s\n",
		num2name(E_type_name, elf->ehp->e_type),
		num2name(E_machine_name, elf->ehp->e_machine),
		num2name(E_version_name, elf->ehp->e_version));
	printf(
		"   e_entry = 0x%08x,  e_phoff = 0x%08x, e_shoff = 0x%08x\n",
		elf->ehp->e_entry,
		elf->ehp->e_phoff,
		elf->ehp->e_shoff);
	printf("   e_flags = 0x%08x ", elf->ehp->e_flags);
	if ( (elf->ehp->e_flags & 1) != 0 )
		printf("EF_MIPS_NOREORDER ");
	if ( (elf->ehp->e_flags & 2) != 0 )
		printf("EF_MIPS_PIC");
	if ( (elf->ehp->e_flags & 4) != 0 )
		printf("EF_MIPS_CPIC");
	printf("\n");
	printf(
		"   e_ehsize = 0x%04x,  e_phentsize = 0x%04x, e_phnum = 0x%04x\n",
		elf->ehp->e_ehsize,
		elf->ehp->e_phentsize,
		elf->ehp->e_phnum);
	printf(
		"   e_shentsize = 0x%04x, e_shnum = 0x%04x, e_shstrndx = 0x%04x\n",
		elf->ehp->e_shentsize,
		elf->ehp->e_shnum,
		elf->ehp->e_shstrndx);
}

struct name2num P_type_name[] =
{
	{ "PT_NULL", 0 },
	{ "PT_LOAD", 1 },
	{ "PT_DYNAMIC", 2 },
	{ "PT_INTERP", 3 },
	{ "PT_NOTE", 4 },
	{ "PT_SHLIB", 5 },
	{ "PT_PHDR", 6 },
	{ "PT_MIPS_REGINFO", 0x70000000 },
	{ "PT_MIPS_RTPROC", 0x70000001 },
	{ "PT_SCE_IOPMOD", 0x70000080 },
	{ "PT_SCE_EEMOD", 0x70000090 },
	{ NULL, 0 }
};

void print_elf_phdr(elf_file *elf, int flag)
{
	int j;
	int i;

	if ( elf->php == NULL || (flag & 1) == 0 )
	{
		return;
	}
	printf(" Program header\n");
	for ( i = 0; i < elf->ehp->e_phnum; ++i )
	{
		printf(" %2d: p_type=%s, ", i, num2name(P_type_name, elf->php[i].phdr.p_type));
		printf(
			"p_offset=0x%06x, p_vaddr,p_paddr=0x%06x,0x%06x\n",
			elf->php[i].phdr.p_offset,
			elf->php[i].phdr.p_vaddr,
			elf->php[i].phdr.p_paddr);
		printf(
			"     p_filesz=0x%06x, p_memsiz=0x%06x, p_align=%d\n",
			elf->php[i].phdr.p_filesz,
			elf->php[i].phdr.p_memsz,
			(int)(elf->php[i].phdr.p_align));
		printf("     p_flags=0x%08x ( ", elf->php[i].phdr.p_flags);
		if ( (elf->php[i].phdr.p_flags & 1) != 0 )
			printf("PF_X ");
		if ( (elf->php[i].phdr.p_flags & 2) != 0 )
			printf("PF_W ");
		if ( (elf->php[i].phdr.p_flags & 4) != 0 )
			printf("PF_R ");
		printf(")\n");
		if ( elf->php[i].scp )
		{
			printf("     include sections = ");
			for ( j = 0; elf->php[i].scp[j]; ++j )
				printf("%s ", elf->php[i].scp[j]->name);
			printf("\n");
		}
	}
}

struct name2num S_type_name[] =
{
	{ "SHT_NULL", 0 },
	{ "SHT_PROGBITS", 1 },
	{ "SHT_SYMTAB", 2 },
	{ "SHT_STRTAB", 3 },
	{ "SHT_RELA", 4 },
	{ "SHT_HASH", 5 },
	{ "SHT_DYNAMIC", 6 },
	{ "SHT_NOTE", 7 },
	{ "SHT_NOBITS", 8 },
	{ "SHT_REL", 9 },
	{ "SHT_SHLIB", 10 },
	{ "SHT_DYNSYM", 11 },
	{ "SHT_MIPS_LIBLIST", 0x70000000 },
	{ "SHT_MIPS_CONFLICT", 0x70000002 },
	{ "SHT_MIPS_GPTAB", 0x70000003 },
	{ "SHT_MIPS_UCODE", 0x70000004 },
	{ "SHT_MIPS_DEBUG", 0x70000005 },
	{ "SHT_MIPS_REGINFO", 0x70000006 },
	{ "SHT_SCE_IOPMOD", 0x70000080 },
	{ "SHT_SCE_EEMOD", 0x70000090 },
	{ NULL, 0 }
};

void print_elf_sections(elf_file *elf, int flag)
{
	unsigned int sh_type;
	const char *v3;
	Elf32_RegInfo *v4;
	Elf32_IopMod *data;
	Elf32_EeMod *v6;
	int i;

	if ( elf->scp == NULL )
	{
		return;
	}
	if ( (flag & 1) != 0 )
		printf(" Section header\n");
	for ( i = 1; ; ++i )
	{
		if ( i >= elf->ehp->e_shnum )
			return;
		if ( (flag & 1) != 0 || ((flag & 2) != 0 && elf->scp[i]->shr.sh_type == 9) )
		{
			v3 = num2name(S_type_name, elf->scp[i]->shr.sh_type);
			printf(" %2d: %-12s sh_name=0x%04x sh_type=%s\n", i, elf->scp[i]->name, elf->scp[i]->shr.sh_name, v3);
			printf("        sh_flas=0x%08x ( ", elf->scp[i]->shr.sh_flags);
			if ( (elf->scp[i]->shr.sh_flags & 1) != 0 )
				printf("SHF_WRITE ");
			if ( (elf->scp[i]->shr.sh_flags & 2) != 0 )
				printf("SHF_ALLOC ");
			if ( (elf->scp[i]->shr.sh_flags & 4) != 0 )
				printf("SHF_EXECINSTR ");
			if ( (elf->scp[i]->shr.sh_flags & 0x10000000) != 0 )
				printf("SHF_MIPS_GPREL ");
			printf(")\n");
			printf(
				"        sh_addr,sh_offset,sh_size=0x%06x,0x%06x,0x%06x, sh_addralign=%2d\n",
				elf->scp[i]->shr.sh_addr,
				elf->scp[i]->shr.sh_offset,
				elf->scp[i]->shr.sh_size,
				(int)(elf->scp[i]->shr.sh_addralign));
			printf(
				"        sh_link=0x%08x, sh_info=0x%08x, sh_entsize=0x%02x(%d)\n",
				elf->scp[i]->shr.sh_link,
				elf->scp[i]->shr.sh_info,
				elf->scp[i]->shr.sh_entsize,
				(int)(elf->scp[i]->shr.sh_entsize));
		}
		sh_type = elf->scp[i]->shr.sh_type;
		switch ( sh_type )
		{
			case 2:
			case 11:
				print_elf_symtbl(elf->scp[i], flag);
				continue;
			case 9:
				print_elf_reloc(elf->scp[i], flag);
				continue;
			case 0x70000005:
				print_elf_mips_symbols((elf_mips_symbolic_data *)elf->scp[i]->data, flag);
				continue;
			case 0x70000080:
				if ( (flag & 1) != 0 )
				{
					data = (Elf32_IopMod *)elf->scp[i]->data;
					printf(
						"        moduleinfo=0x%08x, entry=0x%08x, gpvalue=0x%08x\n",
						data->moduleinfo,
						data->entry,
						data->gp_value);
					printf(
						"        text_size=0x%08x, data_size=0x%08x, bss_size=0x%08x\n",
						data->text_size,
						data->data_size,
						data->bss_size);
				}
				break;
			case 0x70000090:
				if ( (flag & 1) != 0 )
				{
					v6 = (Elf32_EeMod *)elf->scp[i]->data;
					printf("        moduleinfo=0x%08x, entry=0x%08x, gpvalue=0x%08x\n", v6->moduleinfo, v6->entry, v6->gp_value);
					printf(
						"        text_size=0x%08x, data_size=0x%08x, bss_size=0x%08x\n",
						v6->text_size,
						v6->data_size,
						v6->bss_size);
					printf("        erx_lib_addr=0x%08x, erx_lib_size=0x%08x\n", v6->erx_lib_addr, v6->erx_lib_size);
					printf("        erx_stub_addr=0x%08x, erx_stub_size=0x%08x\n", v6->erx_stub_addr, v6->erx_stub_size);
				}
				break;
			case 0x70000006:
				if ( (flag & 1) != 0 )
				{
					v4 = (Elf32_RegInfo *)elf->scp[i]->data;
					printf(
						"        gpmask=0x08x, cprmask=%08x,%08x,%08x,%08x\n",
						v4->ri_gprmask,
						v4->ri_cprmask[0],
						v4->ri_cprmask[1],
						v4->ri_cprmask[3]);
					printf("        gp=0x%08x\n", v4->ri_gp_value);
				}
				break;
			default:
				break;
		}
		if ( elf->scp[i]->data )
		{
			if ( (elf->scp[i]->shr.sh_flags & 4) != 0 && elf->ehp->e_machine == 8 )
			{
				if ( (elf->ehp->e_flags & 0xF0FF0000) == 0x20920000 )
					initdisasm(2, -1, 0, 0, 0);
				else
					initdisasm(1, -1, 0, 0, 0);
				print_elf_disasm(elf, elf->scp[i], flag);
			}
			else
			{
				print_elf_datadump(elf, elf->scp[i], flag);
			}
		}
	}
}

struct name2num R_MIPS_Type[] =
{
	{ "R_MIPS_NONE", 0 },
	{ "R_MIPS_16", 1 },
	{ "R_MIPS_32", 2 },
	{ "R_MIPS_REL32", 3 },
	{ "R_MIPS_26", 4 },
	{ "R_MIPS_HI16", 5 },
	{ "R_MIPS_LO16", 6 },
	{ "R_MIPS_GPREL16", 7 },
	{ "R_MIPS_LITERAL", 8 },
	{ "R_MIPS_GOT16", 9 },
	{ "R_MIPS_PC16", 10 },
	{ "R_MIPS_CALL16", 11 },
	{ "R_MIPS_GPREL32", 12 },
	{ "R_MIPS_GOTHI16", 21 },
	{ "R_MIPS_GOTLO16", 22 },
	{ "R_MIPS_CALLHI16", 30 },
	{ "R_MIPS_CALLLO16", 31 },
	{ "R_MIPS_DVP_11_PCREL", 120 },
	{ "R_MIPS_DVP_27_S4", 121 },
	{ "R_MIPS_DVP_11_S4", 122 },
	{ "R_MIPS_DVP_U15_S3", 123 },
	{ "R_MIPSSCE_MHI16", 250 },
	{ "R_MIPSSCE_ADDEND", 251 },
	{ NULL, 0 }
};

void print_elf_reloc(elf_section *scp, int flag)
{
	const char *name;
	elf_rel *rp;
	signed int entrise;
	signed int i;

	if ( (flag & 2) == 0 )
	{
		return;
	}
	entrise = scp->shr.sh_size / scp->shr.sh_entsize;
	if ( entrise > 0 )
	{
		printf("   ###: r_offset  r_type                r_sym\n");
		printf("   ---  --------  --------------------  -----------------------------------\n");
	}
	rp = (elf_rel *)scp->data;
	for ( i = 0; entrise > i; ++i )
	{
		const char *symshp;

		symshp = num2name(R_MIPS_Type, rp[i].type);
		printf("   %3d: 0x%06x  0x%02x %-16s ", i, rp[i].rel.r_offset, rp[i].type, symshp);
		if ( rp[i].symptr->type == 3 )
		{
			printf("0x%03x[%s]\n", rp[i].rel.r_info >> 8, rp[i].symptr->shptr->name);
		}
		else
		{
			name = rp[i].symptr->name;
			if ( !name )
				name = "";
			printf("0x%03x %s\n", rp[i].rel.r_info >> 8, name);
		}
	}
	printf("\n");
}

void print_elf_disasm(elf_file *elf, elf_section *scp, int flag)
{
	Disasm_result *v3;
	size_t v4;
	size_t v5;
	const char *v6;
	int v7;
	size_t v8;
	int v9;
	const char *v10;
	const char *v11;
	uint32_t rp_;
	uint32_t v13;
	int rpbase_;
	const char *name;
	const char *v16;
	elf_rel *rp;
	elf_rel *rpbase;
	struct rellink *rel;
	Disasm_result **dis;
	char pb[200];
	unsigned int baseoff;
	unsigned int addr;
	unsigned int *codes;
	int relentries;
	size_t steps;
	int d;
	int j;
	int i;

	if ( (flag & 8) == 0 )
	{
		return;
	}
	steps = scp->shr.sh_size >> 2;
	codes = (unsigned int *)scp->data;
	dis = (Disasm_result **)malloc(steps * sizeof(Disasm_result *));
	search_rel_section(elf, scp, &rpbase, &relentries, &baseoff);
	rel = (struct rellink *)calloc(steps, sizeof(struct rellink));
	i = 0;
	addr = 0;
	while ( (int)steps > i )
	{
		v3 = disassemble(addr, codes[i]);
		dis[i] = v3;
		gen_asmmacro(dis[i]);
		search_rel_data(rpbase, relentries, baseoff + addr, &rel[i++]);
		addr += 4;
	}
	for ( i = 0; (int)steps > i; ++i )
	{
		if ( rel[i].rp && rel[i].rp->type == 250 )
		{
			j = i;
			for ( d = SLOWORD(codes[i]); d; d = SLOWORD(codes[j]) )
			{
				j += d;
				rel[j].rid = rel[i].rid;
				rel[j].mhrp = rel[i].rp;
			}
		}
	}
	for ( i = 0; (int)steps > i; ++i )
	{
		format_disasm(dis[i], pb);
		if ( rel[i].rp || rel[i].mhrp )
		{
			for ( j = strlen(pb); j <= 47; ++j )
				pb[j] = 32;
			pb[48] = 0;
			rpbase_ = rel[i].rid;
			v4 = strlen(pb);
			sprintf(&pb[v4], "%3d:", rpbase_);
			if ( rel[i].rp )
			{
				rp = rel[i].rp;
				strcat(pb, " ");
			}
			else
			{
				rp = rel[i].mhrp;
				strcat(pb, ">");
			}
			if ( rp->symptr->type == 3 )
			{
				name = rp->symptr->shptr->name;
				rp_ = rp->rel.r_info >> 8;
				v10 = num2name(R_MIPS_Type, rp->type);
				v5 = strlen(pb);
				sprintf(&pb[v5], "  %s %d '%s'", v10, (int)rp_, name);
			}
			else
			{
				v6 = rp->symptr->name;
				if ( !v6 )
					v6 = "";
				v16 = v6;
				v13 = rp->rel.r_info >> 8;
				v11 = num2name(R_MIPS_Type, rp->type);
				if ( rp->symptr->bind )
				{
					if ( rp->symptr->bind == 1 )
					{
						v7 = 69;
					}
					else if ( rp->symptr->bind == 2 )
					{
						v7 = 119;
					}
					else
					{
						v7 = 32;
					}
				}
				else
				{
					v7 = 108;
				}
				v9 = v7;
				v8 = strlen(pb);
				sprintf(&pb[v8], "%c %s %d %s", v9, v11, (int)v13, v16);
			}
		}
		printf("    %s\n", pb);
		free(dis[i]);
	}
	printf("\n");
	free(dis);
	free(rel);
}

static void search_rel_section(elf_file *elf, elf_section *scp, elf_rel **result, int *relentries, unsigned int *baseoff)
{
	elf_section *relscp;
	int i;

	relscp = 0;
	for ( i = 1; i < elf->ehp->e_shnum; ++i )
	{
		relscp = elf->scp[i];
		if ( relscp->shr.sh_type == 9 && scp == relscp->info )
			break;
	}
	if ( i < elf->ehp->e_shnum )
	{
		*result = (elf_rel *)relscp->data;
		*relentries = relscp->shr.sh_size / relscp->shr.sh_entsize;
		*baseoff = 0;
		if ( elf->ehp->e_type == 0xFF80
			|| elf->ehp->e_type == 0xFF81
			|| elf->ehp->e_type == 0xFF90
			|| elf->ehp->e_type == 0xFF91 )
		{
			*baseoff = scp->shr.sh_addr;
		}
	}
	else
	{
		*result = 0;
		*relentries = 0;
		*baseoff = 0;
	}
}

static void search_rel_data(elf_rel *rpbase, int relentries, unsigned int addr, struct rellink *result)
{
	int j;

	result->rid = -1;
	result->rp = 0;
	for ( j = 0; relentries > j; ++j )
	{
		if ( rpbase[j].type != 251 && addr == rpbase[j].rel.r_offset )
		{
			result->rid = j;
			result->rp = &rpbase[j];
			return;
		}
	}
}

static void dumpb(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, uint8_t *data)
{
	char cbuf[20];
	unsigned int addr;
	unsigned int off2;
	unsigned int off1;

	addr = address & 0xFFFFFFF0;
	off1 = 0;
	off2 = size;
	strcpy(cbuf, "........ ........");
	while ( off2 > off1 )
	{
		if ( (addr & 0xF) == 0 )
			printf("%s%08x: ", head, addr);
		if ( address > addr )
		{
			printf("-- ");
		}
		else
		{
			printf("%02x ", data[off1]);
			if ( data[off1] > 0x1Fu && data[off1] <= 0x7Eu )
			{
				unsigned int v4;
				char *v5;

				v4 = addr & 0xF;
				if ( v4 <= 7 )
					v5 = &cbuf[v4];
				else
					v5 = &cbuf[v4 + 1];
				*v5 = data[off1];
			}
		}
		if ( (((uint8_t)addr + 1) & 3) == 0 )
			printf(" ");
		if ( (addr & 0xF) == 15 )
		{
			printf("%s", cbuf);
			printf("\n");
			strcpy(cbuf, "........ ........");
		}
		if ( address <= addr )
			++off1;
		++addr;
	}
	while ( (addr & 0xF) != 0 )
	{
		printf("-- ");
		if ( (((uint8_t)addr + 1) & 3) == 0 )
			printf(" ");
		if ( (addr & 0xF) == 15 )
		{
			printf("%s", cbuf);
			printf("\n");
			strcpy(cbuf, "........ ........");
		}
		++addr;
	}
}

static void dumph(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, uint16_t *data)
{
	unsigned int addr;
	unsigned int off2;
	unsigned int off1;

	addr = address;
	LOBYTE(addr) = address & 0xF0;
	off2 = addr;
	off1 = 0;
	while ( size > off1 )
	{
		if ( (off2 & 0xF) == 0 )
			printf("%s%08x: ", head, off2);
		if ( address > off2 )
			printf("---- ");
		else
			printf("%04x ", data[off1 >> 1]);
		if ( (((uint8_t)off2 + 2) & 3) == 0 )
			printf("  ");
		if ( (off2 & 0xF) == 14 )
			printf("\n");
		if ( address <= off2 )
			off1 += 2;
		off2 += 2;
	}
	while ( (off2 & 0xF) != 0 )
	{
		printf("---- ");
		if ( (((uint8_t)off2 + 2) & 3) == 0 )
			printf("  ");
		if ( (off2 & 0xF) == 14 )
			printf("\n");
		LOBYTE(off2) = off2 + 2;
	}
}

static void dumpw(srxfixup_const_char_ptr_t head, unsigned int address, unsigned int size, unsigned int *data)
{
	unsigned int v4;
	unsigned int off1;
	unsigned int addr;

	v4 = address;
	LOBYTE(v4) = address & 0xF0;
	addr = v4;
	off1 = 0;
	while ( size > off1 )
	{
		if ( (addr & 0xF) == 0 )
			printf("%s%08x: ", head, addr);
		if ( address > addr )
			printf("--------  ");
		else
			printf("%08x  ", data[off1 >> 2]);
		if ( (addr & 0xF) == 12 )
			printf("\n");
		if ( address <= addr )
			off1 += 4;
		addr += 4;
	}
	while ( (addr & 0xF) != 0 )
	{
		printf("--------  ");
		if ( (addr & 0xF) == 12 )
			printf("\n");
		LOBYTE(addr) = addr + 4;
	}
}

void print_elf_datadump(elf_file *elf, elf_section *scp, int flag)
{
	unsigned int sh_type;
	unsigned int v4;
	const char *v5;
	srxfixup_const_char_ptr_t v6;
	int v7;
	const char *rpbase_;
	uint32_t rel_;
	uint32_t v10;
	const char *name;
	const char *v12;
	elf_rel *rpbase;
	struct rellink rel;
	int relentries;
	int i;
	unsigned int baseoff;
	unsigned int offset;
	unsigned int *dumpbuf;

	if ( (flag & 0xF0) == 0 )
		return;
	dumpbuf = (unsigned int *)calloc(1u, scp->shr.sh_size + 4);
	memcpy(dumpbuf, scp->data, scp->shr.sh_size);
	sh_type = scp->shr.sh_type;
	if ( sh_type == 1 || sh_type == 5 || sh_type == 6 || sh_type == 0x70000006 )
    {
        swapmemory(dumpbuf, (srxfixup_const_char_ptr_t)"l", (scp->shr.sh_size + 1) >> 2);
    }
	printf("\n");
	if ( (flag & 0x10) != 0 )
	{
		dumpb((srxfixup_const_char_ptr_t)"", 0, scp->shr.sh_size, (uint8_t *)dumpbuf);
		printf("\n");
	}
	if ( (flag & 0x20) != 0 )
	{
		dumph((srxfixup_const_char_ptr_t)"          ", 0, scp->shr.sh_size, (uint16_t *)dumpbuf);
		printf("\n");
	}
	if ( (flag & 0xC0) != 0 )
	{
		search_rel_section(elf, scp, &rpbase, &relentries, &baseoff);
		for ( offset = 0; scp->shr.sh_size > offset; offset += 16 )
		{
			if ( scp->shr.sh_size > offset + 16 )
				v4 = 16;
			else
				v4 = scp->shr.sh_size - offset;
			dumpw((srxfixup_const_char_ptr_t)"          ", offset, v4, &dumpbuf[offset / 4]);
			if ( (flag & 0x80u) != 0 )
			{
				for ( i = 0; i <= 15 && scp->shr.sh_size > offset + i; i += 4 )
				{
					search_rel_data(rpbase, relentries, baseoff + offset + i, &rel);
					if ( rel.rid >= 0 )
					{
						printf("                  +%02x:", i);
						printf(" [%3d]", rel.rid);
						if ( rel.rp->symptr->type == 3 )
						{
							name = rel.rp->symptr->shptr->name;
							rel_ = rel.rp->rel.r_info >> 8;
							v5 = num2name(R_MIPS_Type, rel.rp->type);
							printf("  %s %d '%s'", v5, (int)rel_, name);
						}
						else
						{
							v6 = rel.rp->symptr->name;
							if ( !v6 )
								v6 = (srxfixup_const_char_ptr_t)"";
							v12 = v6;
							v10 = rel.rp->rel.r_info >> 8;
							rpbase_ = num2name(R_MIPS_Type, rel.rp->type);
							if ( rel.rp->symptr->bind )
							{
								if ( rel.rp->symptr->bind == 1 )
								{
									v7 = 69;
								}
								else if ( rel.rp->symptr->bind == 2 )
								{
									v7 = 119;
								}
								else
								{
									v7 = 32;
								}
							}
							else
							{
								v7 = 108;
							}
							printf("%c %s %d %s", v7, rpbase_, (int)v10, v12);
						}
						printf("\n");
					}
				}
			}
		}
		printf("\n");
	}
	free(dumpbuf);
}

struct name2num SymbolBinding[] =
{
	{ "STB_LOCAL", 0 },
	{ "STB_GLOBAL", 1 },
	{ "STB_WEAK", 2 },
	{ NULL, 0 }
};
struct name2num SymbolType[] =
{
	{ "STT_NOTYPE", 0 },
	{ "STT_OBJECT", 1 },
	{ "STT_FUNC", 2 },
	{ "STT_SECTION", 3 },
	{ "STT_FILE", 4 },
	{ NULL, 0 }
};
struct name2num SymbolSpSection[] =
{
	{ "SHN_UNDEF", 0 },
	{ "SHN_MIPS_ACOMMON", 0xFF00 },
	{ "SHN_MIPS_TEXT", 0xFF01 },
	{ "SHN_MIPS_DATA", 0xFF02 },
	{ "SHN_MIPS_SCOMMON", 0xFF03 },
	{ "SHN_MIPS_SUNDEFINED", 0xFF04 },
	{ "SHN_RADDR", 0xFF1F },
	{ "SHN_ABS", 0xFFF1 },
	{ "SHN_COMMON", 0xFFF2 },
	{ NULL, 0 }
};

void print_elf_symtbl(elf_section *scp, int flag)
{
	elf_syment **syp;
	signed int entirse;
	int i;

	if ( (flag & 4) == 0 )
	{
		return;
	}
	entirse = scp->shr.sh_size / scp->shr.sh_entsize;
	syp = (elf_syment **)scp->data;
	if ( entirse > 1 )
	{
		printf("   ###: name        bind       type        st_value  st_size st_shndx\n");
		printf("   ---  ----------- ---------- ----------- ---------- ------ ------------------\n");
	}
	for ( i = 1; entirse > i; ++i )
	{
		printf("   %3d: ", i);
		if ( syp[i]->name && strlen(syp[i]->name) > 0xB )
		{
			printf("%s\n                    ", syp[i]->name);
		}
		else
		{
			const char *name;

			name = syp[i]->name;
			if ( !name )
				name = "<no name>";
			printf("%-11s ", name);
		}
		printf("%-10s %-11s 0x%08x 0x%04x ", num2name(SymbolBinding, syp[i]->bind), num2name(SymbolType, syp[i]->type), syp[i]->sym.st_value, syp[i]->sym.st_size);
		if ( syp[i]->shptr )
		{
			printf("%2d '%s'", syp[i]->sym.st_shndx, syp[i]->shptr->name);
		}
		else
		{
			int st_shndx;

			st_shndx = syp[i]->sym.st_shndx;
			printf("%s(0x%x)", num2name(SymbolSpSection, st_shndx), st_shndx);
		}
		if ( syp[i]->sym.st_other )
			printf("  st_other=0x%x", syp[i]->sym.st_other);
		printf("\n");
	}
	printf("\n");
}

static srxfixup_const_char_ptr_t num2name(struct name2num *table, int num)
{
	static char buf_28[30];

	while ( table->name )
	{
		if ( num == table->num )
			return table->name;
		++table;
	}
	memcpy(buf_28, "? ", 2);
	sprintf(&buf_28[2], "0x%x", num);
	return buf_28;
}

struct name2num SymbolTypes[] =
{
	{ "stNil", 0 },
	{ "stGlobal", 1 },
	{ "stStatic", 2 },
	{ "stParam", 3 },
	{ "stLocal", 4 },
	{ "stLabel", 5 },
	{ "stProc", 6 },
	{ "stBlock", 7 },
	{ "stEnd", 8 },
	{ "stMember", 9 },
	{ "stTypedef", 10 },
	{ "stFile", 11 },
	{ "stRegReloc", 12 },
	{ "stForward", 13 },
	{ "stStaticProc", 14 },
	{ "stConstant", 15 },
	{ "stStaParam", 16 },
	{ "stStruct", 26 },
	{ "stUnion", 27 },
	{ "stEnum", 28 },
	{ "stIndirect", 34 },
	{ "stStr", 60 },
	{ "stNumber", 61 },
	{ "stExpr", 62 },
	{ "stType", 63 },
	{ "stMax", 64 },
	{ NULL, 0 }
};
struct name2num StorageClasse[] =
{
	{ "scNil", 0 },
	{ "scText", 1 },
	{ "scData", 2 },
	{ "scBss", 3 },
	{ "scRegister", 4 },
	{ "scAbs", 5 },
	{ "scUndef", 6 },
	{ "scUndefined", 6 },
	{ "scCdbLocal", 7 },
	{ "scBits", 8 },
	{ "scCdbSystem", 9 },
	{ "scDbx", 9 },
	{ "scRegImage", 10 },
	{ "scInfo", 11 },
	{ "scUserStruct", 12 },
	{ "scSData", 13 },
	{ "scSBss", 14 },
	{ "scRData", 15 },
	{ "scVar", 16 },
	{ "scCommon", 17 },
	{ "scSCommon", 18 },
	{ "scVarRegister", 19 },
	{ "scVariant", 20 },
	{ "scSUndefined", 21 },
	{ "scInit", 22 },
	{ "scBasedVar", 23 },
	{ "scXData", 24 },
	{ "scPData", 25 },
	{ "scFini", 26 },
	{ "scRConst", 27 },
	{ "scMax", 32 },
	{ NULL, 0 }
};

void print_elf_mips_symbols(elf_mips_symbolic_data *symbol, int flag)
{
	symr *syp_1;
	pdr *pdrp_2;
	unsigned int *ip_1;
	unsigned int *ip_2;
	symr *syp_2;
	extr *ep;
	const char *issBase;
	fdr *fdrp;
	int ifd;

	if ( (flag & 0x200) == 0 || symbol == NULL )
	{
		return;
	}
	printf(" Symbol header\n");
	printf("      magic=0x%x      vstamp=0x%x\n", symbol->head.magic, symbol->head.vstamp);
	printf(
		"      ilineMax= %4d   cbLine=       %6d    cbLineOffset=%d(0x%05x)\n",
		symbol->head.ilineMax,
		symbol->head.cbLine,
		symbol->head.cbLineOffset,
		symbol->head.cbLineOffset);
	printf(
		"      idnMax=   %4d   cbDnOffset=   %6d(0x%05x)\n",
		symbol->head.idnMax,
		symbol->head.cbDnOffset,
		symbol->head.cbDnOffset);
	printf(
		"      ipdMax=   %4d   cbPdOffset=   %6d(0x%05x)\n",
		symbol->head.ipdMax,
		symbol->head.cbPdOffset,
		symbol->head.cbPdOffset);
	printf(
		"      isymMax=  %4d   cbSymOffset=  %6d(0x%05x)\n",
		symbol->head.isymMax,
		symbol->head.cbSymOffset,
		symbol->head.cbSymOffset);
	printf(
		"      ioptMax=  %4d   cbOptOffset=  %6d(0x%05x)\n",
		symbol->head.ioptMax,
		symbol->head.cbOptOffset,
		symbol->head.cbOptOffset);
	printf(
		"      iauxMax=  %4d   cbAuxOffset=  %6d(0x%05x)\n",
		symbol->head.iauxMax,
		symbol->head.cbAuxOffset,
		symbol->head.cbAuxOffset);
	printf(
		"      issMax=   %4d   cbSsOffset=   %6d(0x%05x)\n",
		symbol->head.issMax,
		symbol->head.cbSsOffset,
		symbol->head.cbSsOffset);
	printf(
		"      issExtMax=%4d   cbSsExtOffset=%6d(0x%05x)\n",
		symbol->head.issExtMax,
		symbol->head.cbSsExtOffset,
		symbol->head.cbSsExtOffset);
	printf(
		"      ifdMax=   %4d   cbFdOffset=   %6d(0x%05x)\n",
		symbol->head.ifdMax,
		symbol->head.cbFdOffset,
		symbol->head.cbFdOffset);
	printf(
		"      crfd=     %4d   cbRfdOffset=  %6d(0x%05x)\n",
		symbol->head.crfd,
		symbol->head.cbRfdOffset,
		symbol->head.cbRfdOffset);
	printf(
		"      iextMax=  %4d   cbExtOffset=  %6d(0x%05x)\n",
		symbol->head.iextMax,
		symbol->head.cbExtOffset,
		symbol->head.cbExtOffset);
	printf("\n");
	if ( symbol->head.issMax > 0 )
	{
		int i_1;
		const char *cp_1;
		const char *cpend_1;

		cp_1 = symbol->cbSs_Ptr;
		cpend_1 = &cp_1[symbol->head.issMax];
		printf("  Local strings\n");
		i_1 = 0;
		while ( cpend_1 > cp_1 )
		{
			printf("   %3d(%5d): %s\n", i_1++, (int)(cp_1 - symbol->cbSs_Ptr), cp_1);
			cp_1 += strlen(cp_1) + 1;
		}
		printf("\n");
	}
	if ( symbol->head.issExtMax > 0 )
	{
		int i_2;
		const char *cp_2;
		const char *cpend_2;

		cp_2 = symbol->cbSsExt_Ptr;
		cpend_2 = &cp_2[symbol->head.issExtMax];
		printf("  External strings\n");
		i_2 = 0;
		while ( cpend_2 > cp_2 )
		{
			printf("   %3d(%5d): %s\n", i_2++, (int)(cp_2 - symbol->cbSsExt_Ptr), cp_2);
			cp_2 += strlen(cp_2) + 1;
		}
		printf("\n");
	}
	if ( symbol->head.iextMax > 0 )
	{
		int i_3;

		ep = (extr *)symbol->cbExt_Ptr;
		printf("  External symbols\n");
		for ( i_3 = 0; symbol->head.iextMax > i_3; ++i_3 )
		{
			unsigned int idx_1;
			unsigned int class_1;
			unsigned int type_1;

			type_1 = ep->asym.sy_bits & 0x3F;
			class_1 = (ep->asym.sy_bits >> 6) & 0x1F;
			idx_1 = ep->asym.sy_bits >> 12;
			printf("   %3d: res,ifd=%04x,%04hx", i_3, ep->reserved, (unsigned short)(ep->ifd));
			printf(", 0x%08x", ep->asym.value);
			printf(", %8s:%-7s 0x%05x ", num2name(SymbolTypes, type_1), num2name(StorageClasse, class_1), idx_1);
			printf("%s\n", &symbol->cbSsExt_Ptr[ep->asym.iss]);
			++ep;
		}
		printf("\n");
	}
	printf("  Local informations\n");
	fdrp = (fdr *)symbol->cbFd_Ptr;
	for ( ifd = 0; symbol->head.ifdMax > ifd; ++ifd )
	{
		issBase = &symbol->cbSs_Ptr[fdrp->issBase];
		printf("  %3d: %-40s addr=0x%08x \n", ifd, &issBase[fdrp->rss], fdrp->adr);
		if ( fdrp->csym > 0 )
		{
			int i_4;

			syp_1 = (symr *)&symbol->cbSym_Ptr[12 * fdrp->isymBase];
			printf("      Local symbols\n");
			for ( i_4 = 0; fdrp->csym > i_4; ++i_4 )
			{
				unsigned int idx_2;
				unsigned int class_2;
				unsigned int type_2;

				type_2 = syp_1->sy_bits & 0x3F;
				class_2 = (syp_1->sy_bits >> 6) & 0x1F;
				idx_2 = syp_1->sy_bits >> 12;
				printf("      %3d: 0x%08x", i_4, syp_1->value);
				printf(", %10s:%-10s 0x%05x ", num2name(SymbolTypes, type_2), num2name(StorageClasse, class_2), idx_2);
				printf("%s\n", &issBase[syp_1->iss]);
				++syp_1;
			}
		}
		if ( fdrp->cline > 0 )
		{
			printf("      Line numbers\n");
			printf("        ilinBase = %d, cline= %d\n", fdrp->ilineBase, fdrp->cline);
		}
		if ( fdrp->copt > 0 )
		{
			printf("      Optimization entries\n");
			printf("        ioptBase = %d, copt = %d\n", fdrp->ioptBase, fdrp->copt);
		}
		if ( fdrp->cpd > 0 )
		{
			int i_5;

			syp_2 = (symr *)&symbol->cbSym_Ptr[12 * fdrp->isymBase];
			pdrp_2 = (pdr *)&symbol->cbPd_Ptr[52 * fdrp->ipdFirst];
			printf("      Procedures\n");
			for ( i_5 = 0; i_5 < fdrp->cpd; ++i_5 )
			{
				printf("      %3d: 0x%08x  %s\n", i_5, pdrp_2->adr, &issBase[syp_2[pdrp_2->isym].iss]);
				printf(
					"           regmask=0x%08x, regoffset=%d, fregmask=0x%08x, fregoffset=%d\n",
					pdrp_2->regmask,
					pdrp_2->regoffset,
					pdrp_2->fregmask,
					pdrp_2->fregoffset);
				printf(
					"           iopt=%d, frameoffset=%d, framereg=%d, pcreg=%d, line %d..%d\n",
					pdrp_2->iopt,
					pdrp_2->frameoffset,
					pdrp_2->framereg,
					pdrp_2->pcreg,
					pdrp_2->lnLow,
					pdrp_2->lnHigh);
				printf("           iline=%d, cbLineOffset=%d\n", pdrp_2->iline, (int)(pdrp_2->cbLineOffset));
				++pdrp_2;
			}
		}
		if ( fdrp->caux > 0 )
		{
			int i_6;

			printf("      Auxillary symbol entries\n");
			ip_1 = (unsigned int *)&symbol->cbAux_Ptr[4 * fdrp->iauxBase];
			for ( i_6 = 0; fdrp->caux > i_6; ++i_6 )
				printf("      %3d: 0x%08lx \n", i_6, (unsigned long)(*ip_1++));
		}
		if ( fdrp->crfd > 0 )
		{
			int i_7;

			printf("      File indirect entries\n");
			ip_2 = (unsigned int *)&symbol->cbRfd_Ptr[4 * fdrp->rfdBase];
			for ( i_7 = 0; fdrp->crfd > i_7; ++i_7 )
				printf("      %3d: 0x%08lx \n", i_7, (unsigned long)(*ip_2++));
		}
		printf("\n");
		++fdrp;
	}
	printf("\n");
}


































