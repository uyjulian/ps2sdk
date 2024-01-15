
#include "all_include.h"

static int is_in_range(unsigned int top, unsigned int size, unsigned int pos);
static void read_symtab(elf_file *elf, int sctindex, FILE *fp);
static void read_rel(elf_file *elf, int sctindex, FILE *fp);
static elf_mips_symbolic_data *read_mips_symbolic(FILE *fp);
static void renumber_a_symtab(elf_section *scp);
static void renumber_symtab(elf_file *elf);
static void write_symtab(elf_file *elf, int sctindex, FILE *fp);
static void write_rel(elf_file *elf, int sctindex, FILE *fp);
static void write_mips_symbolic(elf_mips_symbolic_data *sycb, int basepos, FILE *fp);
static void reorder_an_symtab(elf_file *elf, elf_section *scp);
static int search_string_table(srxfixup_const_char_ptr_t tbltop, int endindex, srxfixup_const_char_ptr_t str);
static void rebuild_a_symbol_name_strings(elf_section *scp);
static int comp_Elf_file_slot(const void *a1, const void *a2);

elf_file *read_elf(srxfixup_const_char_ptr_t filename)
{
	elf_proghead *v8;
	elf_section **v9;
	unsigned int v10;
	uint8_t *v11;
	uint8_t *v12;
	uint8_t *v13;
	uint8_t *mips_symbolic;
	uint8_t *v15;
	char *v16;
	unsigned int p_type;
	elf_section *v18;
	elf_section *v19;
	elf_section *v20;
	elf_section **v21;
	unsigned int sh_type;
	unsigned int p_filesz;
	unsigned int p_offset;
	int d;
	int s;
	signed int size;
	signed int count_1;
	signed int count_2;
	unsigned int pos_1;
	unsigned int pos_2;
	signed int pos_3;
	signed int pos_4;
	signed int pos_5;
	int i_1;
	int i_2;
	int i_3;
	int i_4;
	int i_5;
	int i_6;
	int i_7;
	int i_8;
	int ident;
	elf_file *elf;
	FILE *fp;

	fp = fopen(filename, "rb");
	if ( !fp )
	{
		fprintf(stderr, "`%s' can't open\n", filename);
		return 0;
	}
	elf = (elf_file *)calloc(1u, sizeof(elf_file));
	elf->ehp = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
	if ( fread(elf->ehp, sizeof(Elf32_Ehdr), 1u, fp) != 1 )
	{
		fprintf(stderr, "%s: Could not read ELF header\n", filename);
		exit(1);
	}
	swapmemory(elf->ehp, "ccccccccccccccccsslllllssssss", 1);
	ident = *(uint32_t *)elf->ehp->e_ident;
	swapmemory(&ident, "l", 1);
	if ( ident != 0x464C457F )
	{
		fprintf(stderr, "%s: not elf format\n", filename);
		free(elf->ehp);
		free(elf);
		fclose(fp);
		return 0;
	}
	if ( elf->ehp->e_ident[4] != ELFCLASS32 || elf->ehp->e_ident[5] != ELFDATA2LSB || elf->ehp->e_ident[6] != EV_CURRENT )
	{
		fprintf(stderr, "%s: Not 32-bit object or Not little endian or Invalid Elf version\n", filename);
		free(elf->ehp);
		free(elf);
		fclose(fp);
		return 0;
	}
	if ( elf->ehp->e_machine != EM_MIPS )
	{
		fprintf(stderr, "%s: not EM_MIPS\n", filename);
		free(elf->ehp);
		free(elf);
		fclose(fp);
		return 0;
	}
	if ( elf->ehp->e_phentsize && elf->ehp->e_phentsize != sizeof(Elf32_Phdr) )
	{
		fprintf(stderr, "%s: Unknown program header size\n", filename);
		free(elf->ehp);
		free(elf);
		fclose(fp);
		return 0;
	}
	if ( elf->ehp->e_shentsize && elf->ehp->e_shentsize != sizeof(Elf32_Shdr) )
	{
		fprintf(stderr, "%s: Unknown sectoin header size\n", filename);
		free(elf->ehp);
		free(elf);
		fclose(fp);
		return 0;
	}
	if ( elf->ehp->e_phnum && elf->ehp->e_phentsize )
	{
		count_1 = elf->ehp->e_phnum;
		pos_1 = elf->ehp->e_phoff;
		v8 = (elf_proghead *)calloc(count_1, sizeof(elf_proghead));
		elf->php = v8;
		fseek(fp, pos_1, 0);
		for ( i_1 = 0; count_1 > i_1; ++i_1 )
		{
			if ( fread(&elf->php[i_1], sizeof(Elf32_Phdr), 1u, fp) != 1 )
			{
				fprintf(stderr, "%s: Could not read ELF program header\n", filename);
				exit(1);
			}
			swapmemory(&elf->php[i_1], "llllllll", 1);
		}
	}
	if ( elf->ehp->e_shnum && elf->ehp->e_shentsize )
	{
		pos_2 = elf->ehp->e_shoff;
		count_2 = elf->ehp->e_shnum;
		v9 = (elf_section **)calloc(count_2 + 1, sizeof(elf_section *));
		elf->scp = v9;
		fseek(fp, pos_2, 0);
		for ( i_2 = 0; count_2 > i_2; ++i_2 )
		{
			elf->scp[i_2] = (elf_section *)calloc(1u, sizeof(elf_section));
			if ( fread(elf->scp[i_2], sizeof(Elf32_Shdr), 1u, fp) != 1 )
			{
				fprintf(stderr, "%s: Could not read ELF section header\n", filename);
				exit(1);
			}
			swapmemory(elf->scp[i_2], "llllllllll", 1);
		}
		for ( i_3 = 0; count_2 > i_3; ++i_3 )
		{
			switch ( elf->scp[i_3]->shr.sh_type )
			{
				case SHT_RELA:
				case SHT_REL:
					elf->scp[i_3]->info = elf->scp[elf->scp[i_3]->shr.sh_info];
				case SHT_SYMTAB:
				case SHT_HASH:
				case SHT_DYNAMIC:
				case SHT_DYNSYM:
					elf->scp[i_3]->link = elf->scp[elf->scp[i_3]->shr.sh_link];
					break;
				default:
					break;
			}
			if ( i_3 == elf->ehp->e_shstrndx )
				elf->shstrptr = elf->scp[i_3];
		}
		for ( i_4 = 0; ; ++i_4 )
		{
			if ( count_2 <= i_4 )
			{
				for ( i_5 = 0; count_2 > i_5; ++i_5 )
				{
					pos_4 = elf->scp[i_5]->shr.sh_offset;
					if ( (elf->scp[i_5]->shr.sh_type == SHT_SYMTAB || elf->scp[i_5]->shr.sh_type == SHT_DYNSYM)
						&& pos_4 > 0
						&& elf->scp[i_5]->shr.sh_size )
					{
						fseek(fp, pos_4, 0);
						read_symtab(elf, i_5, fp);
					}
				}
				for ( i_6 = 0; count_2 > i_6; ++i_6 )
				{
					pos_5 = elf->scp[i_6]->shr.sh_offset;
					if ( elf->scp[i_6]->shr.sh_type == SHT_REL && pos_5 > 0 && elf->scp[i_6]->shr.sh_size )
					{
						fseek(fp, pos_5, 0);
						read_rel(elf, i_6, fp);
					}
				}
				for ( i_7 = 0; count_2 > i_7; ++i_7 )
				{
					v16 = strdup((char *)&elf->shstrptr->data[elf->scp[i_7]->shr.sh_name]);
					elf->scp[i_7]->name = v16;
				}
				for ( i_8 = 0; i_8 < elf->ehp->e_phnum; ++i_8 )
				{
					p_type = elf->php[i_8].phdr.p_type;
					switch ( p_type )
					{
						case PT_LOAD:
							v21 = (elf_section **)calloc(count_2, sizeof(elf_section *));
							elf->php[i_8].scp = v21;
							s = 1;
							d = 0;
							while ( count_2 > s )
							{
								p_offset = elf->php[i_8].phdr.p_offset;
								p_filesz = elf->php[i_8].phdr.p_filesz;
								sh_type = elf->scp[s]->shr.sh_type;
								if ( sh_type == SHT_PROGBITS )
								{
									if ( is_in_range(p_offset, p_filesz, elf->scp[s]->shr.sh_offset)
										|| (!elf->scp[s]->shr.sh_size && elf->scp[s]->shr.sh_offset == p_filesz + p_offset) )
									{
										elf->php[i_8].scp[d++] = elf->scp[s];
									}
								}
								else if ( sh_type == SHT_NOBITS
											 && (is_in_range(elf->php[i_8].phdr.p_vaddr, elf->php[i_8].phdr.p_memsz, elf->scp[s]->shr.sh_addr)
												|| (!elf->scp[s]->shr.sh_size && elf->scp[s]->shr.sh_offset == p_filesz + p_offset)) )
								{
									elf->php[i_8].scp[d++] = elf->scp[s];
								}
								++s;
							}
							break;
						case PT_MIPS_REGINFO:
							elf->php[i_8].scp = (elf_section **)calloc(2u, sizeof(elf_section *));
							v18 = search_section(elf, SHT_MIPS_REGINFO);
							*elf->php[i_8].scp = v18;
							break;
						case PT_SCE_IOPMOD:
							elf->php[i_8].scp = (elf_section **)calloc(2u, sizeof(elf_section *));
							v19 = search_section(elf, SHT_SCE_IOPMOD);
							*elf->php[i_8].scp = v19;
							break;
						case PT_SCE_EEMOD:
							elf->php[i_8].scp = (elf_section **)calloc(2u, sizeof(elf_section *));
							v20 = search_section(elf, SHT_SCE_EEMOD);
							*elf->php[i_8].scp = v20;
							break;
						default:
							break;
					}
				}
				return elf;
			}
			pos_3 = elf->scp[i_4]->shr.sh_offset;
			size = elf->scp[i_4]->shr.sh_size;
			if ( pos_3 > 0 && size > 0 )
			{
				fseek(fp, pos_3, 0);
				v10 = elf->scp[i_4]->shr.sh_type;
				switch ( v10 )
				{
					case SHT_PROGBITS:
					case SHT_RELA:
					case SHT_HASH:
					case SHT_DYNAMIC:
					case SHT_MIPS_REGINFO:
						v11 = (uint8_t *)malloc(size);
						elf->scp[i_4]->data = v11;
						if ( fread(elf->scp[i_4]->data, size, 1u, fp) != 1 )
						{
							fprintf(stderr, "%s: Could not read ELF section contents\n", filename);
							exit(1);
						}
						swapmemory(elf->scp[i_4]->data, "l", (unsigned int)size >> 2);
						break;
					case SHT_SYMTAB:
					case SHT_NOBITS:
					case SHT_DYNSYM:
						break;
					case SHT_MIPS_DEBUG:
						mips_symbolic = (uint8_t *)read_mips_symbolic(fp);
						elf->scp[i_4]->data = mips_symbolic;
						break;
					case SHT_SCE_IOPMOD:
						v12 = (uint8_t *)malloc(size);
						elf->scp[i_4]->data = v12;
						if ( fread(elf->scp[i_4]->data, size, 1u, fp) != 1 )
						{
							fprintf(stderr, "%s: Could not read ELF section contents\n", filename);
							exit(1);
						}
						swapmemory(elf->scp[i_4]->data, "lllllls", 1);
						break;
					case SHT_SCE_EEMOD:
						v13 = (uint8_t *)malloc(size);
						elf->scp[i_4]->data = v13;
						if ( fread(elf->scp[i_4]->data, size, 1u, fp) != 1 )
						{
							fprintf(stderr, "%s: Could not read ELF section contents\n", filename);
							exit(1);
						}
						swapmemory(elf->scp[i_4]->data, "lllllllllls", 1);
						break;
					default:
						v15 = (uint8_t *)malloc(size);
						elf->scp[i_4]->data = v15;
						if ( fread(elf->scp[i_4]->data, size, 1u, fp) != 1 )
						{
							fprintf(stderr, "%s: Could not read ELF section contents\n", filename);
							exit(1);
						}
						break;
				}
			}
		}
	}
	fclose(fp);
	return elf;
}

static int is_in_range(unsigned int top, unsigned int size, unsigned int pos)
{
	if ( pos >= top && pos < size + top )
		return 1;
	return 0;
}

static void read_symtab(elf_file *elf, int sctindex, FILE *fp)
{
	elf_syment **result;
	elf_section *sp_x;
	int entrise;
	int i;

	sp_x = elf->scp[sctindex];
	entrise = sp_x->shr.sh_size / sp_x->shr.sh_entsize;
	result = (elf_syment **)calloc(entrise, sizeof(elf_syment *));
	sp_x->data = (uint8_t *)result;
	for ( i = 0; entrise > i; ++i )
	{
		result[i] = (elf_syment *)calloc(1u, sizeof(elf_syment));
		if ( fread(result[i], sp_x->shr.sh_entsize, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF symbol table\n");
			exit(1);
		}
		swapmemory(result[i], "lllccs", 1);
		result[i]->bind = result[i]->sym.st_info >> 4;
		result[i]->type = result[i]->sym.st_info & 0xF;
		result[i]->name = 0;
		if ( result[i]->sym.st_name )
			result[i]->name = strdup((char *)&sp_x->link->data[result[i]->sym.st_name]);
		if ( result[i]->sym.st_shndx && result[i]->sym.st_shndx <= 0xFEFFu )
			result[i]->shptr = elf->scp[result[i]->sym.st_shndx];
		else
			result[i]->shptr = 0;
	}
}

static void read_rel(elf_file *elf, int sctindex, FILE *fp)
{
	elf_syment **symp;
	elf_rel *result;
	elf_section *sp_x;
	int entrise;
	int i;

	sp_x = elf->scp[sctindex];
	entrise = sp_x->shr.sh_size / sp_x->shr.sh_entsize;
	result = (elf_rel *)calloc(entrise, sizeof(elf_rel));
	sp_x->data = (uint8_t *)result;
	symp = (elf_syment **)sp_x->link->data;
	for ( i = 0; entrise > i; ++i )
	{
		if ( fread(&result[i], sp_x->shr.sh_entsize, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF relocations\n");
			exit(1);
		}
		swapmemory(&result[i], "ll", 1);
		result[i].type = result[i].rel.r_info & 0xFF;
		result[i].symptr = symp[result[i].rel.r_info >> 8];
		++result[i].symptr->refcount;
	}
}

static elf_mips_symbolic_data *read_mips_symbolic(FILE *fp)
{
	elf_mips_symbolic_data *sycb;

	sycb = (elf_mips_symbolic_data *)malloc(sizeof(elf_mips_symbolic_data));
	if ( fread(sycb, sizeof(hdrr), 1u, fp) != 1 )
	{
		fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
		exit(1);
	}
	swapmemory(sycb, "sslllllllllllllllllllllll", 1);
	if ( sycb->head.cbLineOffset > 0 )
	{
		size_t size_1;

		size_1 = sycb->head.cbLine;
		sycb->cbLine_Ptr = (char *)malloc(size_1);
		fseek(fp, sycb->head.cbLineOffset, 0);
		if ( fread(sycb->cbLine_Ptr, size_1, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
	}
	if ( sycb->head.cbDnOffset > 0 )
	{
		int size_2;

		size_2 = 8 * sycb->head.idnMax;
		sycb->cbDn_Ptr = (char *)malloc(size_2);
		fseek(fp, sycb->head.cbDnOffset, 0);
		if ( fread(sycb->cbDn_Ptr, size_2, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbDn_Ptr, "ll", sycb->head.idnMax);
	}
	if ( sycb->head.cbPdOffset > 0 )
	{
		int size_3;

		size_3 = 52 * sycb->head.ipdMax;
		sycb->cbPd_Ptr = (char *)malloc(size_3);
		fseek(fp, sycb->head.cbPdOffset, 0);
		if ( fread(sycb->cbPd_Ptr, size_3, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbPd_Ptr, "lllllllllsslll", sycb->head.ipdMax);
	}
	if ( sycb->head.cbSymOffset > 0 )
	{
		int size_4;

		size_4 = 12 * sycb->head.isymMax;
		sycb->cbSym_Ptr = (char *)malloc(size_4);
		fseek(fp, sycb->head.cbSymOffset, 0);
		if ( fread(sycb->cbSym_Ptr, size_4, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbSym_Ptr, "lll", sycb->head.isymMax);
	}
	if ( sycb->head.cbOptOffset > 0 )
	{
		int size_5;

		size_5 = 12 * sycb->head.ioptMax;
		sycb->cbOpt_Ptr = (char *)malloc(size_5);
		fseek(fp, sycb->head.cbOptOffset, 0);
		if ( fread(sycb->cbOpt_Ptr, size_5, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbOpt_Ptr, "lll", sycb->head.ioptMax);
	}
	if ( sycb->head.cbAuxOffset > 0 )
	{
		int size_6;

		size_6 = 4 * sycb->head.iauxMax;
		sycb->cbAux_Ptr = (char *)malloc(size_6);
		fseek(fp, sycb->head.cbAuxOffset, 0);
		if ( fread(sycb->cbAux_Ptr, size_6, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbAux_Ptr, "l", sycb->head.iauxMax);
	}
	if ( sycb->head.cbSsOffset > 0 )
	{
		size_t size_7;

		size_7 = sycb->head.issMax;
		sycb->cbSs_Ptr = (char *)malloc(size_7);
		fseek(fp, sycb->head.cbSsOffset, 0);
		if ( fread(sycb->cbSs_Ptr, size_7, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
	}
	if ( sycb->head.cbSsExtOffset > 0 )
	{
		size_t size_8;

		size_8 = sycb->head.issExtMax;
		sycb->cbSsExt_Ptr = (char *)malloc(size_8);
		fseek(fp, sycb->head.cbSsExtOffset, 0);
		if ( fread(sycb->cbSsExt_Ptr, size_8, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
	}
	if ( sycb->head.cbFdOffset > 0 )
	{
		int size_9;

		size_9 = 72 * sycb->head.ifdMax;
		sycb->cbFd_Ptr = (char *)malloc(size_9);
		fseek(fp, sycb->head.cbFdOffset, 0);
		if ( fread(sycb->cbFd_Ptr, size_9, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbFd_Ptr, "llllllllllsslllllll", sycb->head.ifdMax);
	}
	if ( sycb->head.cbRfdOffset > 0 )
	{
		int size_A;

		size_A = 4 * sycb->head.crfd;
		sycb->cbRfd_Ptr = (char *)malloc(size_A);
		fseek(fp, sycb->head.cbRfdOffset, 0);
		if ( fread(sycb->cbRfd_Ptr, size_A, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbRfd_Ptr, "l", sycb->head.crfd);
	}
	if ( sycb->head.cbExtOffset > 0 )
	{
		int size_B;

		size_B = 16 * sycb->head.iextMax;
		sycb->cbExt_Ptr = (char *)malloc(size_B);
		fseek(fp, sycb->head.cbExtOffset, 0);
		if ( fread(sycb->cbExt_Ptr, size_B, 1u, fp) != 1 )
		{
			fprintf(stderr, "elflib: Could not read ELF debug info contents\n");
			exit(1);
		}
		swapmemory(sycb->cbExt_Ptr, "sslll", sycb->head.iextMax);
	}
	return sycb;
}

int layout_elf_file(elf_file *elf)
{
	Elf_file_slot *order;

	reorder_symtab(elf);
	rebuild_section_name_strings(elf);
	rebuild_symbol_name_strings(elf);
	order = build_file_order_list(elf);
	shrink_file_order_list(order);
	writeback_file_order_list(elf, order);
	free(order);
	return 0;
}

int write_elf(elf_file *elf, srxfixup_const_char_ptr_t filename)
{
	unsigned int sh_type;
	unsigned int size;
	int count_1;
	int count_2;
	signed int pos;
	int i_1;
	int i_2;
	int i_3;
	int i_4;
	int i_5;
	FILE *fp;

	fp = fopen(filename, "wb");
	if ( !fp )
	{
		perror(filename);
		return 1;
	}
	renumber_symtab(elf);
	if ( elf->ehp->e_shnum && elf->ehp->e_shentsize )
	{
		for ( i_1 = 0; i_1 < elf->ehp->e_shnum; ++i_1 )
			elf->scp[i_1]->number = i_1;
		for ( i_2 = 0; i_2 < elf->ehp->e_shnum; ++i_2 )
		{
			switch ( elf->scp[i_2]->shr.sh_type )
			{
				case SHT_RELA:
				case SHT_REL:
					elf->scp[i_2]->shr.sh_info = elf->scp[i_2]->info->number;
				case SHT_SYMTAB:
				case SHT_HASH:
				case SHT_DYNAMIC:
				case SHT_DYNSYM:
					elf->scp[i_2]->shr.sh_link = elf->scp[i_2]->link->number;
					break;
				default:
					break;
			}
			if ( elf->scp[i_2] == elf->shstrptr )
				elf->ehp->e_shstrndx = elf->scp[i_2]->number;
		}
	}
	swapmemory(elf->ehp, "ccccccccccccccccsslllllssssss", 1);
	fwrite(elf->ehp, 0x34u, 1u, fp);
	swapmemory(elf->ehp, "ccccccccccccccccsslllllssssss", 1);
	if ( elf->ehp->e_phnum && elf->ehp->e_phentsize )
	{
		count_1 = elf->ehp->e_phnum;
		fseek(fp, elf->ehp->e_phoff, 0);
		for ( i_3 = 0; count_1 > i_3; ++i_3 )
		{
			swapmemory(&elf->php[i_3], "llllllll", 1);
			fwrite(&elf->php[i_3], 0x20u, 1u, fp);
			swapmemory(&elf->php[i_3], "llllllll", 1);
		}
	}
	if ( elf->ehp->e_shnum && elf->ehp->e_shentsize )
	{
		count_2 = elf->ehp->e_shnum;
		fseek(fp, elf->ehp->e_shoff, 0);
		for ( i_4 = 0; count_2 > i_4; ++i_4 )
		{
			swapmemory(elf->scp[i_4], "llllllllll", 1);
			fwrite(elf->scp[i_4], 0x28u, 1u, fp);
			swapmemory(elf->scp[i_4], "llllllllll", 1);
		}
		for ( i_5 = 0; count_2 > i_5; ++i_5 )
		{
			pos = elf->scp[i_5]->shr.sh_offset;
			size = elf->scp[i_5]->shr.sh_size;
			if ( pos > 0 && (int)size > 0 )
			{
				fseek(fp, pos, 0);
				sh_type = elf->scp[i_5]->shr.sh_type;
				switch ( sh_type )
				{
					case SHT_PROGBITS:
					case SHT_RELA:
					case SHT_HASH:
					case SHT_DYNAMIC:
					case SHT_MIPS_REGINFO:
						swapmemory(elf->scp[i_5]->data, "l", size >> 2);
						fwrite(elf->scp[i_5]->data, size, 1u, fp);
						swapmemory(elf->scp[i_5]->data, "l", size >> 2);
						break;
					case SHT_SYMTAB:
					case SHT_DYNSYM:
						write_symtab(elf, i_5, fp);
						break;
					case SHT_NOBITS:
						break;
					case SHT_REL:
						write_rel(elf, i_5, fp);
						break;
					case SHT_MIPS_DEBUG:
						write_mips_symbolic((elf_mips_symbolic_data *)elf->scp[i_5]->data, pos, fp);
						break;
					case SHT_SCE_IOPMOD:
						swapmemory(elf->scp[i_5]->data, "lllllls", 1);
						fwrite(elf->scp[i_5]->data, size, 1u, fp);
						swapmemory(elf->scp[i_5]->data, "lllllls", 1);
						break;
					case SHT_SCE_EEMOD:
						swapmemory(elf->scp[i_5]->data, "lllllllllls", 1);
						fwrite(elf->scp[i_5]->data, size, 1u, fp);
						swapmemory(elf->scp[i_5]->data, "lllllllllls", 1);
						break;
					default:
						fwrite(elf->scp[i_5]->data, size, 1u, fp);
						break;
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

static void renumber_a_symtab(elf_section *scp)
{
	elf_syment **syp;
	signed int entrise;
	int i;

	entrise = scp->shr.sh_size / scp->shr.sh_entsize;
	syp = (elf_syment **)scp->data;
	for ( i = 0; entrise > i; ++i )
		syp[i]->number = i;
}

static void renumber_symtab(elf_file *elf)
{
	int sc;

	for ( sc = 1; sc < elf->ehp->e_shnum; ++sc )
	{
		if ( elf->scp[sc]->shr.sh_type == SHT_SYMTAB || elf->scp[sc]->shr.sh_type == SHT_DYNSYM )
			renumber_a_symtab(elf->scp[sc]);
	}
}

static void write_symtab(elf_file *elf, int sctindex, FILE *fp)
{
	Elf32_Sym sym;
	elf_syment **syp;
	elf_section *sp_x;
	int entrise;
	int i;

	sp_x = elf->scp[sctindex];
	entrise = sp_x->shr.sh_size / sp_x->shr.sh_entsize;
	syp = (elf_syment **)sp_x->data;
	fseek(fp, sp_x->shr.sh_offset, 0);
	for ( i = 0; entrise > i; ++i )
	{
		memcpy(&sym, syp[i], sizeof(sym));
		if ( syp[i]->shptr )
			sym.st_shndx = syp[i]->shptr->number;
		swapmemory(&sym, "lllccs", 1);
		fwrite(&sym, 0x10u, 1u, fp);
	}
}

static void write_rel(elf_file *elf, int sctindex, FILE *fp)
{
	Elf32_Rel rel;
	elf_rel *rp;
	elf_section *sp_x;
	int entrise;
	int i;

	sp_x = elf->scp[sctindex];
	entrise = sp_x->shr.sh_size / sp_x->shr.sh_entsize;
	rp = (elf_rel *)sp_x->data;
	fseek(fp, sp_x->shr.sh_offset, 0);
	for ( i = 0; entrise > i; ++i )
	{
		memcpy(&rel, &rp[i], sizeof(rel));
		if ( rp[i].symptr->number == -1 )
		{
			fprintf(stderr, "Internal error !!\n");
			fprintf(stderr, " relocation entry have no symbol\nabort\n");
			exit(1);
		}
		rel.r_info = (rp[i].symptr->number << 8) + (rp[i].type & 0xFF);
		swapmemory(&rel, "ll", 1);
		fwrite(&rel, 8u, 1u, fp);
	}
}

static void write_mips_symbolic(elf_mips_symbolic_data *sycb, int basepos, FILE *fp)
{
	int pos;

	pos = basepos + 96;
	if ( sycb->head.cbLineOffset > 0 )
	{
		int size_1;

		size_1 = sycb->head.cbLine;
		sycb->head.cbLineOffset = pos;
		pos += size_1;
		fseek(fp, sycb->head.cbLineOffset, 0);
		fwrite(sycb->cbLine_Ptr, size_1, 1u, fp);
	}
	if ( sycb->head.cbDnOffset > 0 )
	{
		int size_2;

		size_2 = 8 * sycb->head.idnMax;
		sycb->head.cbDnOffset = pos;
		pos += size_2;
		fseek(fp, sycb->head.cbDnOffset, 0);
		swapmemory(sycb->cbDn_Ptr, "ll", sycb->head.idnMax);
		fwrite(sycb->cbDn_Ptr, size_2, 1u, fp);
		swapmemory(sycb->cbDn_Ptr, "ll", sycb->head.idnMax);
	}
	if ( sycb->head.cbPdOffset > 0 )
	{
		int size_3;

		size_3 = 52 * sycb->head.ipdMax;
		sycb->head.cbPdOffset = pos;
		pos += size_3;
		fseek(fp, sycb->head.cbPdOffset, 0);
		swapmemory(sycb->cbPd_Ptr, "lllllllllsslll", sycb->head.ipdMax);
		fwrite(sycb->cbPd_Ptr, size_3, 1u, fp);
		swapmemory(sycb->cbPd_Ptr, "lllllllllsslll", sycb->head.ipdMax);
	}
	if ( sycb->head.cbSymOffset > 0 )
	{
		int size_4;

		size_4 = 12 * sycb->head.isymMax;
		sycb->head.cbSymOffset = pos;
		pos += size_4;
		fseek(fp, sycb->head.cbSymOffset, 0);
		swapmemory(sycb->cbSym_Ptr, "lll", sycb->head.isymMax);
		fwrite(sycb->cbSym_Ptr, size_4, 1u, fp);
		swapmemory(sycb->cbSym_Ptr, "lll", sycb->head.isymMax);
	}
	if ( sycb->head.cbOptOffset > 0 )
	{
		int size_5;

		size_5 = 12 * sycb->head.ioptMax;
		pos += size_5;
		fseek(fp, sycb->head.cbOptOffset, 0);
		swapmemory(sycb->cbOpt_Ptr, "lll", sycb->head.ioptMax);
		fwrite(sycb->cbOpt_Ptr, size_5, 1u, fp);
		swapmemory(sycb->cbOpt_Ptr, "lll", sycb->head.ioptMax);
	}
	if ( sycb->head.cbAuxOffset > 0 )
	{
		int size_6;

		size_6 = 4 * sycb->head.iauxMax;
		sycb->head.cbAuxOffset = pos;
		pos += size_6;
		fseek(fp, sycb->head.cbAuxOffset, 0);
		swapmemory(sycb->cbAux_Ptr, "l", sycb->head.iauxMax);
		fwrite(sycb->cbAux_Ptr, size_6, 1u, fp);
		swapmemory(sycb->cbAux_Ptr, "l", sycb->head.iauxMax);
	}
	if ( sycb->head.cbSsOffset > 0 )
	{
		int size_7;

		size_7 = sycb->head.issMax;
		sycb->head.cbSsOffset = pos;
		pos += size_7;
		fseek(fp, sycb->head.cbSsOffset, 0);
		fwrite(sycb->cbSs_Ptr, size_7, 1u, fp);
	}
	if ( sycb->head.cbSsExtOffset > 0 )
	{
		int size_8;

		size_8 = sycb->head.issExtMax;
		sycb->head.cbSsExtOffset = pos;
		pos += size_8;
		fseek(fp, sycb->head.cbSsExtOffset, 0);
		fwrite(sycb->cbSsExt_Ptr, size_8, 1u, fp);
	}
	if ( sycb->head.cbFdOffset > 0 )
	{
		int size_9;

		size_9 = 72 * sycb->head.ifdMax;
		sycb->head.cbFdOffset = pos;
		pos += size_9;
		fseek(fp, sycb->head.cbFdOffset, 0);
		swapmemory(sycb->cbFd_Ptr, "llllllllllsslllllll", sycb->head.ifdMax);
		fwrite(sycb->cbFd_Ptr, size_9, 1u, fp);
		swapmemory(sycb->cbFd_Ptr, "llllllllllsslllllll", sycb->head.ifdMax);
	}
	if ( sycb->head.cbRfdOffset > 0 )
	{
		int size_A;

		size_A = 4 * sycb->head.crfd;
		sycb->head.cbRfdOffset = pos;
		pos += size_A;
		fseek(fp, sycb->head.cbRfdOffset, 0);
		swapmemory(sycb->cbRfd_Ptr, "l", sycb->head.crfd);
		fwrite(sycb->cbRfd_Ptr, size_A, 1u, fp);
		swapmemory(sycb->cbRfd_Ptr, "l", sycb->head.crfd);
	}
	if ( sycb->head.cbExtOffset > 0 )
	{
		int size_B;

		size_B = 16 * sycb->head.iextMax;
		sycb->head.cbExtOffset = pos;
		fseek(fp, sycb->head.cbExtOffset, 0);
		swapmemory(sycb->cbExt_Ptr, "sslll", sycb->head.iextMax);
		fwrite(sycb->cbExt_Ptr, size_B, 1u, fp);
		swapmemory(sycb->cbExt_Ptr, "sslll", sycb->head.iextMax);
	}
	fseek(fp, basepos, 0);
	swapmemory(sycb, "sslllllllllllllllllllllll", 1);
	fwrite(sycb, 0x60u, 1u, fp);
	swapmemory(sycb, "sslllllllllllllllllllllll", 1);
}

void add_section(elf_file *elf, elf_section *scp)
{
	Elf32_Ehdr *ehp;
	int count;

	ehp = elf->ehp;
	count = ++ehp->e_shnum;
	elf->scp = (elf_section **)realloc(elf->scp, (count + 1) * sizeof(elf_section *));
	elf->scp[count - 1] = scp;
	elf->scp[count] = 0;
	add_symbol(elf, 0, 0, 3, 0, scp, 0);
}

elf_section *remove_section(elf_file *elf, int shtype)
{
	elf_section *rmsec;
	int s;

	rmsec = 0;
	for ( s = 1; s < elf->ehp->e_shnum; ++s )
	{
		if ( shtype == elf->scp[s]->shr.sh_type )
		{
			--elf->ehp->e_shnum;
			rmsec = elf->scp[s];
			break;
		}
	}
	while ( s < elf->ehp->e_shnum )
	{
		elf->scp[s] = elf->scp[s + 1];
		++s;
	}
	return rmsec;
}

elf_section *remove_section_by_name(elf_file *elf, srxfixup_const_char_ptr_t secname)
{
	elf_section *rmsec;
	int s;

	rmsec = 0;
	for ( s = 1; s < elf->ehp->e_shnum; ++s )
	{
		if ( !strcmp(elf->scp[s]->name, secname) )
		{
			--elf->ehp->e_shnum;
			rmsec = elf->scp[s];
			break;
		}
	}
	while ( s < elf->ehp->e_shnum )
	{
		elf->scp[s] = elf->scp[s + 1];
		++s;
	}
	return rmsec;
}

elf_section *search_section(elf_file *elf, int stype)
{
	int i;

	for ( i = 1; i < elf->ehp->e_shnum; ++i )
	{
		if ( stype == elf->scp[i]->shr.sh_type )
			return elf->scp[i];
	}
	return 0;
}

elf_section *search_section_by_name(elf_file *elf, srxfixup_const_char_ptr_t secname)
{
	int i;

	for ( i = 1; i < elf->ehp->e_shnum; ++i )
	{
		if ( !strcmp(elf->scp[i]->name, secname) )
			return elf->scp[i];
	}
	return 0;
}

unsigned int *get_section_data(elf_file *elf, unsigned int addr)
{
	int i;

	for ( i = 1; i < elf->ehp->e_shnum; ++i )
	{
		if ( elf->scp[i]->shr.sh_type == SHT_PROGBITS
			&& addr >= elf->scp[i]->shr.sh_addr
			&& addr < elf->scp[i]->shr.sh_size + elf->scp[i]->shr.sh_addr )
		{
			return (unsigned int *)&elf->scp[i]->data[addr - elf->scp[i]->shr.sh_addr];
		}
	}
	return 0;
}

elf_syment *search_global_symbol(srxfixup_const_char_ptr_t name, elf_file *elf)
{
	int entrise;
	int i;
	elf_syment **syp;
	elf_section *scp;

	scp = search_section(elf, SHT_SYMTAB);
	if ( !scp )
		return 0;
	entrise = scp->shr.sh_size / scp->shr.sh_entsize;
	syp = (elf_syment **)scp->data;
	for ( i = 1; entrise > i; ++i )
	{
		if ( syp[i]->name && syp[i]->bind == STB_GLOBAL && !strcmp(syp[i]->name, name) )
			return syp[i];
	}
	return 0;
}

int is_defined_symbol(const elf_syment *sym)
{
	if ( !sym )
		return 0;
	if ( !sym->sym.st_shndx )
		return 0;
	if ( sym->sym.st_shndx <= 0xFEFFu )
		return 1;
	return sym->sym.st_shndx == SHN_ABS;
}

elf_syment *add_symbol(elf_file *elf, srxfixup_const_char_ptr_t name, int bind, int type, int value, elf_section *scp, int st_shndx)
{
	int entrise;
	elf_syment *sym;
	elf_syment **newtab;
	elf_section *symtbl;

	symtbl = search_section(elf, SHT_SYMTAB);
	if ( !symtbl )
		return 0;
	entrise = symtbl->shr.sh_size / symtbl->shr.sh_entsize;
	newtab = (elf_syment **)realloc(symtbl->data, (entrise + 1) * sizeof(elf_syment *));
	sym = (elf_syment *)calloc(1u, sizeof(elf_syment));
	newtab[entrise] = sym;
	symtbl->shr.sh_size += symtbl->shr.sh_entsize;
	symtbl->data = (uint8_t *)newtab;
	if ( name )
		sym->name = strdup(name);
	sym->bind = bind;
	sym->type = type;
	sym->sym.st_info = (type & 0xF) + 16 * bind;
	sym->sym.st_value = value;
	sym->shptr = scp;
	if ( scp )
		sym->sym.st_shndx = 1;
	else
		sym->sym.st_shndx = st_shndx;
	return sym;
}

unsigned int get_symbol_value(const elf_syment *sym, const elf_file *elf)
{
	if ( !is_defined_symbol(sym) )
		return 0;
	if ( sym->sym.st_shndx != SHN_ABS && elf->ehp->e_type == ET_REL )
		return sym->shptr->shr.sh_addr + sym->sym.st_value;
	return sym->sym.st_value;
}

static void reorder_an_symtab(elf_file *elf, elf_section *scp)
{
	int sections;
	elf_syment **oldtab;
	elf_syment **newtab;
	signed int entrise;
	int sc;
	unsigned int d;
	int i;
	int j;
	int k;
	int m;

	sections = elf->ehp->e_shnum;
	entrise = scp->shr.sh_size / scp->shr.sh_entsize;
	oldtab = (elf_syment **)malloc(entrise * sizeof(elf_syment *));
	memcpy(oldtab, (elf_syment **)scp->data, entrise * sizeof(elf_syment *));
	newtab = (elf_syment **)calloc(entrise, sizeof(elf_syment *));
	scp->data = (uint8_t *)newtab;
	*newtab = *oldtab;
	d = 1;
	for ( sc = 1; sections > sc; ++sc )
	{
		for ( i = 1; entrise > i; ++i )
		{
			if ( oldtab[i]
				&& oldtab[i]->type == 3
				&& !oldtab[i]->name
				&& oldtab[i]->shptr
				&& !strcmp(oldtab[i]->shptr->name, elf->scp[sc]->name) )
			{
				newtab[d++] = oldtab[i];
				oldtab[i] = 0;
				break;
			}
		}
	}
	for ( j = 1; entrise > j; ++j )
	{
		if ( oldtab[j] && oldtab[j]->type == 3 && !oldtab[j]->name )
			oldtab[j] = 0;
	}
	for ( k = 1; entrise > k; ++k )
	{
		if ( oldtab[k] && !oldtab[k]->bind )
		{
			newtab[d++] = oldtab[k];
			oldtab[k] = 0;
		}
	}
	scp->shr.sh_info = d;
	for ( m = 1; entrise > m; ++m )
	{
		if ( oldtab[m] )
		{
			newtab[d++] = oldtab[m];
			oldtab[m] = 0;
		}
	}
	scp->shr.sh_size = scp->shr.sh_entsize * d;
	free(oldtab);
}

void reorder_symtab(elf_file *elf)
{
	int s;

	for ( s = 1; s < elf->ehp->e_shnum; ++s )
	{
		if ( elf->scp[s]->shr.sh_type == SHT_SYMTAB || elf->scp[s]->shr.sh_type == SHT_DYNSYM )
			reorder_an_symtab(elf, elf->scp[s]);
	}
}

int adjust_align(int value, int align)
{
	return ~(align - 1) & (align + value - 1);
}

void rebuild_section_name_strings(elf_file *elf)
{
	unsigned int offset;
	size_t namesize;
	int i_1;
	int i_2;

	namesize = 1;
	if ( elf->scp == NULL )
	{
		return;
	}
	for ( i_1 = 1; i_1 < elf->ehp->e_shnum; ++i_1 )
		namesize += strlen(elf->scp[i_1]->name) + 1;
	if ( elf->shstrptr->data )
		free(elf->shstrptr->data);
	elf->shstrptr->data = (uint8_t *)calloc(1u, namesize);
	elf->shstrptr->shr.sh_size = namesize;
	i_2 = 1;
	offset = 1;
	while ( i_2 < elf->ehp->e_shnum )
	{
		strcpy((char *)&elf->shstrptr->data[offset], elf->scp[i_2]->name);
		elf->scp[i_2]->shr.sh_name = offset;
		offset += strlen(elf->scp[i_2++]->name) + 1;
	}
}

static int search_string_table(srxfixup_const_char_ptr_t tbltop, int endindex, srxfixup_const_char_ptr_t str)
{
	int idx;

	for ( idx = 1; endindex > idx; idx += strlen(&tbltop[idx]) + 1 )
	{
		if ( !strcmp(str, &tbltop[idx]) )
			return idx;
	}
	return 0;
}

static void rebuild_a_symbol_name_strings(elf_section *scp)
{
	size_t v1;
	elf_section *strtab;
	elf_syment **syp;
	int offset;
	size_t namesize;
	signed int entrise;
	int i_1;
	int i_2;

	entrise = scp->shr.sh_size / scp->shr.sh_entsize;
	strtab = scp->link;
	syp = (elf_syment **)scp->data;
	namesize = 1;
	for ( i_1 = 1; entrise > i_1; ++i_1 )
	{
		if ( syp[i_1]->name )
			v1 = strlen(syp[i_1]->name) + namesize + 1;
		else
			v1 = namesize;
		namesize = v1;
	}
	if ( strtab->data )
		free(strtab->data);
	strtab->data = (uint8_t *)calloc(1u, namesize);
	i_2 = 1;
	offset = 1;
	while ( entrise > i_2 )
	{
		if ( syp[i_2]->name )
		{
			syp[i_2]->sym.st_name = search_string_table((char *)strtab->data, offset, syp[i_2]->name);
			if ( !syp[i_2]->sym.st_name )
			{
				strcpy((char *)&strtab->data[offset], syp[i_2]->name);
				syp[i_2]->sym.st_name = offset;
				offset += strlen(syp[i_2]->name) + 1;
			}
		}
		++i_2;
	}
	strtab->shr.sh_size = offset;
}

void rebuild_symbol_name_strings(elf_file *elf)
{
	int sc;

	if ( elf->scp == NULL )
	{
		return;
	}
	for ( sc = 1; sc < elf->ehp->e_shnum; ++sc )
	{
		if ( elf->scp[sc]->shr.sh_type == SHT_SYMTAB || elf->scp[sc]->shr.sh_type == SHT_DYNSYM )
			rebuild_a_symbol_name_strings(elf->scp[sc]);
	}
}

static int comp_Elf_file_slot(const void *a1, const void *a2)
{
	const Elf_file_slot *p1;
	const Elf_file_slot *p2;

	p1 = a1;
	p2 = a2;

	if ( p1->type == 1 && p2->type == 1 )
		return 0;
	if ( p1->type == 1 )
		return -1;
	if ( p2->type == 1 )
		return 1;
	if ( p1->type == 2 && p2->type == 2 )
		return 0;
	if ( p1->type == 2 )
		return -1;
	if ( p2->type == 2 )
		return 1;
	if ( !p1->type && !p2->type )
		return 0;
	if ( !p1->type )
		return 1;
	if ( !p2->type )
		return -1;
	if ( p1->type == 100 && p2->type == 100 )
		return 0;
	if ( p1->type == 100 )
		return 1;
	if ( p2->type == 100 )
		return -1;
	if ( p1->type == 3 && p2->type == 4 )
		return -1;
	if ( p1->type == 4 && p2->type == 3 )
		return 1;
	if ( p2->offset == p1->offset )
		return 0;
	if ( p2->offset >= p1->offset )
		return -1;
	return 1;
}

Elf_file_slot *build_file_order_list(const elf_file *elf)
{
	int sections;
	elf_section **scp;
	Elf_file_slot *resolt;
	int s_2;
	int d_1;
	size_t d_2;
	int maxent;

	sections = elf->ehp->e_shnum;
	scp = (elf_section **)calloc(sections + 1, sizeof(elf_section *));
	memcpy(scp, elf->scp, sections * sizeof(elf_section *));
	maxent = elf->ehp->e_shnum + 2;
	if ( elf->ehp->e_phnum )
		maxent = elf->ehp->e_phnum + elf->ehp->e_shnum + 3;
	resolt = (Elf_file_slot *)calloc(maxent, sizeof(Elf_file_slot));
	resolt->type = 1;
	resolt->offset = 0;
	resolt->size = 52;
	resolt->align = 4;
	d_1 = 1;
	if ( elf->ehp->e_phnum )
	{
		int seg;

		resolt[1].type = 2;
		resolt[1].offset = resolt->size;
		resolt[1].size = 32 * elf->ehp->e_phnum;
		resolt[1].align = 4;
		seg = 0;
		d_1 = 2;
		while ( seg < elf->ehp->e_phnum )
		{
			elf_section **phdscp;

			resolt[d_1].type = 3;
			resolt[d_1].d.php = &elf->php[seg];
			resolt[d_1].offset = elf->php[seg].phdr.p_offset;
			resolt[d_1].size = elf->php[seg].phdr.p_filesz;
			resolt[d_1].align = elf->php[seg].phdr.p_align;
			for ( phdscp = elf->php[seg].scp; *phdscp; ++phdscp )
			{
				int s_1;

				for ( s_1 = 0; sections > s_1; ++s_1 )
				{
					if ( *phdscp == scp[s_1] )
					{
						scp[s_1] = 0;
						break;
					}
				}
			}
			++seg;
			++d_1;
		}
	}
	resolt[d_1].type = 4;
	if ( elf->ehp->e_shoff )
		resolt[d_1].offset = elf->ehp->e_shoff;
	else
		resolt[d_1].offset = -256;
	resolt[d_1].size = 40 * elf->ehp->e_shnum;
	resolt[d_1].align = 4;
	d_2 = d_1 + 1;
	for ( s_2 = 1; sections > s_2; ++s_2 )
	{
		if ( scp[s_2] )
		{
			resolt[d_2].type = 5;
			resolt[d_2].d.scp = scp[s_2];
			resolt[d_2].offset = scp[s_2]->shr.sh_offset;
			resolt[d_2].size = scp[s_2]->shr.sh_size;
			resolt[d_2].align = scp[s_2]->shr.sh_addralign;
			scp[s_2] = 0;
			++d_2;
		}
	}
	resolt[d_2].type = 100;
	free(scp);
	qsort(resolt, d_2, sizeof(Elf_file_slot), comp_Elf_file_slot);
	return resolt;
}

void  shrink_file_order_list(Elf_file_slot *efs)
{
	int slot;

	slot = 0;
	while ( efs->type != 100 )
	{
		unsigned int foffset;

		foffset = adjust_align(slot, efs->align);
		efs->offset = foffset;
		slot = efs->size + foffset;
		++efs;
	}
}

void  writeback_file_order_list(elf_file *elf, Elf_file_slot *efs)
{
	elf_section **scp;
	unsigned int segoffset;
	int i;

	while ( efs->type != 100 )
	{
		switch ( efs->type )
		{
			case 2:
				elf->ehp->e_phoff = efs->offset;
				break;
			case 3:
				efs->d.php->phdr.p_offset = efs->offset;
				scp = efs->d.php->scp;
				segoffset = efs->offset;
				(*scp)->shr.sh_offset = efs->offset;
				for ( i = 1; scp[i]; ++i )
				{
					if ( scp[i]->shr.sh_type != SHT_NOBITS )
						segoffset = scp[i]->shr.sh_addr + efs->offset - (*scp)->shr.sh_addr;
					scp[i]->shr.sh_offset = segoffset;
					if ( scp[i]->shr.sh_type != SHT_NOBITS )
						segoffset += scp[i]->shr.sh_size;
				}
				break;
			case 4:
				elf->ehp->e_shoff = efs->offset;
				break;
			case 5:
				efs->d.php->phdr.p_filesz = efs->offset;
				break;
			default:
				break;
		}
		++efs;
	}
}

void dump_file_order_list(const elf_file *elf, const Elf_file_slot *efs)
{
	int offset;
	int sh_offset;
	unsigned int size_tmp;
	unsigned int offset_tmp;
	char tmp[100];
	const char *name;
	elf_section **scp;
	const Elf_file_slot *slot;
	int i;

	offset_tmp = efs->offset;
	printf("\n");
	for ( slot = efs; slot->type != 100; ++slot )
	{
		int oldend_1;
		int size_1;
		signed int startpos_1;

		oldend_1 = slot->size;
		startpos_1 = slot->offset;
		if ( oldend_1 <= 0 )
			offset = slot->offset;
		else
			offset = oldend_1 + startpos_1 - 1;
		size_1 = offset;
		switch ( slot->type )
		{
			case 1:
				name = "[Elf header]";
				break;
			case 2:
				name = "[Proram Header Table]";
				break;
			case 3:
				sprintf(tmp, "[Proram Header entry %d]", (int)(0xCCCCCCCD * ((char *)slot->d.php - (char *)elf->php)) >> 3);
				name = tmp;
				break;
			case 4:
				name = "[Section Header Table]";
				break;
			case 5:
				sprintf(tmp, "%s data", slot->d.scp->name);
				name = tmp;
				break;
			default:
				name = "internal error Unknown EFS type !!!!";
				break;
		}
		if ( startpos_1 > (int)(offset_tmp + 1) )
			printf("%36s = %08x-%08x (%06x)\n", "**Blank**", offset_tmp + 1, startpos_1 - 1, startpos_1 - offset_tmp - 1);
		offset_tmp = size_1;
		printf("%36s = %08x-%08x (%06x)\n", name, startpos_1, size_1, oldend_1);
		if ( slot->type == 3 )
		{
			scp = slot->d.php->scp;
			(*scp)->shr.sh_offset = slot->offset;
			size_tmp = slot->offset;
			for ( i = 0; scp[i]; ++i )
			{
				int oldend_2;
				int size_2;
				signed int startpos_2;

				if ( scp[i]->shr.sh_type == SHT_NOBITS )
					oldend_2 = 0;
				else
					oldend_2 = scp[i]->shr.sh_size;
				startpos_2 = scp[i]->shr.sh_offset;
				if ( oldend_2 <= 0 )
					sh_offset = scp[i]->shr.sh_offset;
				else
					sh_offset = oldend_2 + startpos_2 - 1;
				size_2 = sh_offset;
				sprintf(tmp, "(%s)", scp[i]->name);
				name = tmp;
				if ( startpos_2 > (int)(size_tmp + 1) )
					printf("%36s | %08x-%08x (%06x)\n", "**Blank**", size_tmp + 1, startpos_2 - 1, startpos_2 - size_tmp - 1);
				size_tmp = size_2;
				printf("%36s | %08x-%08x (%06x)\n", name, startpos_2, size_2, scp[i]->shr.sh_size);
			}
		}
	}
}







