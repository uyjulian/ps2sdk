
#ifndef __ALL_INCLUDE_H__
#define __ALL_INCLUDE_H__

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

// Type definitions

typedef const char * srxfixup_const_char_ptr_t;

// srxfixup.c, elflib.c, elfdump.c, srxgen.c, readconf.c

typedef short unsigned int Elf32_Half;
typedef unsigned int Elf32_Addr;
typedef unsigned int Elf32_Off;
typedef unsigned int Elf32_Word;
typedef int Elf32_SWord;
typedef struct _Elf32_ehdr
{
	unsigned char e_ident[16];
	short unsigned int e_type;
	short unsigned int e_machine;
	unsigned int e_version;
	unsigned int e_entry;
	unsigned int e_phoff;
	unsigned int e_shoff;
	unsigned int e_flags;
	short unsigned int e_ehsize;
	short unsigned int e_phentsize;
	short unsigned int e_phnum;
	short unsigned int e_shentsize;
	short unsigned int e_shnum;
	short unsigned int e_shstrndx;
} Elf32_Ehdr;
typedef struct _Elf32_Phdr
{
	unsigned int p_type;
	unsigned int p_offset;
	unsigned int p_vaddr;
	unsigned int p_paddr;
	unsigned int p_filesz;
	unsigned int p_memsz;
	unsigned int p_flags;
	unsigned int p_align;
} Elf32_Phdr;
typedef struct _Elf32_Shdr
{
	unsigned int sh_name;
	unsigned int sh_type;
	unsigned int sh_flags;
	unsigned int sh_addr;
	unsigned int sh_offset;
	unsigned int sh_size;
	unsigned int sh_link;
	unsigned int sh_info;
	unsigned int sh_addralign;
	unsigned int sh_entsize;
} Elf32_Shdr;
typedef struct _Elf32_Sym
{
	unsigned int st_name;
	unsigned int st_value;
	unsigned int st_size;
	unsigned char st_info;
	unsigned char st_other;
	short unsigned int st_shndx;
} Elf32_Sym;
typedef struct _Elf_Note
{
	unsigned int namesz;
	unsigned int descsz;
	unsigned int type;
	char name[1];
} Elf_Note;
typedef struct _Elf32_Rel
{
	unsigned int r_offset;
	unsigned int r_info;
} Elf32_Rel;
typedef struct _Elf32_RegInfo
{
	unsigned int ri_gprmask;
	unsigned int ri_cprmask[4];
	int ri_gp_value;
} Elf32_RegInfo;
typedef struct _Elf32_IopMod
{
	unsigned int moduleinfo;
	unsigned int entry;
	unsigned int gp_value;
	unsigned int text_size;
	unsigned int data_size;
	unsigned int bss_size;
	short unsigned int moduleversion;
	char modulename[1];
} Elf32_IopMod;
typedef struct _Elf32_EeMod
{
	unsigned int moduleinfo;
	unsigned int entry;
	unsigned int gp_value;
	unsigned int text_size;
	unsigned int data_size;
	unsigned int bss_size;
	unsigned int erx_lib_addr;
	unsigned int erx_lib_size;
	unsigned int erx_stub_addr;
	unsigned int erx_stub_size;
	short unsigned int moduleversion;
	char modulename[1];
} Elf32_EeMod;
typedef struct _hdrr
{
	short int magic;
	short int vstamp;
	int ilineMax;
	int cbLine;
	int cbLineOffset;
	int idnMax;
	int cbDnOffset;
	int ipdMax;
	int cbPdOffset;
	int isymMax;
	int cbSymOffset;
	int ioptMax;
	int cbOptOffset;
	int iauxMax;
	int cbAuxOffset;
	int issMax;
	int cbSsOffset;
	int issExtMax;
	int cbSsExtOffset;
	int ifdMax;
	int cbFdOffset;
	int crfd;
	int cbRfdOffset;
	int iextMax;
	int cbExtOffset;
} hdrr;
typedef struct _fdr
{
	unsigned int adr;
	int rss;
	int issBase;
	int cbSs;
	int isymBase;
	int csym;
	int ilineBase;
	int cline;
	int ioptBase;
	int copt;
	short unsigned int ipdFirst;
	short int cpd;
	int iauxBase;
	int caux;
	int rfdBase;
	int crfd;
	unsigned int fdr_bits;
	int cbLineOffset;
	int cbLine;
} fdr;
typedef struct _symr
{
	int iss;
	int value;
	unsigned int sy_bits;
} symr;
typedef struct _extr
{
	short int reserved;
	short int ifd;
	symr asym;
} extr;
typedef struct _dnr
{
	unsigned int d_rfd;
	unsigned int d_index;
} dnr;
typedef struct _pdr
{
	unsigned int adr;
	int isym;
	int iline;
	int regmask;
	int regoffset;
	int iopt;
	int fregmask;
	int fregoffset;
	int frameoffset;
	short int framereg;
	short int pcreg;
	int lnLow;
	int lnHigh;
	unsigned int cbLineOffset;
} pdr;
typedef struct _tir
{
	unsigned int fBitfield;
	unsigned int continued;
	unsigned int bt;
	unsigned int tq4;
	unsigned int tq5;
	unsigned int tq0;
	unsigned int tq1;
	unsigned int tq2;
	unsigned int tq3;
} tir;
typedef struct _rndxr
{
	unsigned int rfd;
	unsigned int index;
} rndxr;
typedef union __anon_struct_32
{
	tir ti;
	rndxr rndx;
	int dnLow;
	int dnHigh;
	int isym;
	int iss;
	int width;
	int count;
} auxu;
typedef struct _optr
{
	unsigned int optr_bits;
	rndxr rndx;
	unsigned int offset;
} optr;
typedef int rfdt;
typedef struct _elf_mips_symbolic
{
	hdrr head;
	char *cbLine_Ptr;
	char *cbDn_Ptr;
	char *cbPd_Ptr;
	char *cbSym_Ptr;
	char *cbOpt_Ptr;
	char *cbAux_Ptr;
	char *cbSs_Ptr;
	char *cbSsExt_Ptr;
	char *cbFd_Ptr;
	char *cbRfd_Ptr;
	char *cbExt_Ptr;
} elf_mips_symbolic_data;
typedef struct _elf_section
{
	Elf32_Shdr shr;
	uint8_t *data;
	srxfixup_const_char_ptr_t name;
	int number;
	struct _elf_section *link;
	struct _elf_section *info;
	void *optdata;
} elf_section;
typedef struct _syment
{
	Elf32_Sym sym;
	srxfixup_const_char_ptr_t name;
	int number;
	int bind;
	int type;
	elf_section *shptr;
	int refcount;
} elf_syment;
typedef struct _rel
{
	Elf32_Rel rel;
	elf_syment *symptr;
	int type;
} elf_rel;
typedef struct _elf_proghead
{
	Elf32_Phdr phdr;
	elf_section **scp;
	void *optdata; // Not used
} elf_proghead;
typedef struct _elffile
{
	Elf32_Ehdr *ehp;
	elf_section *shstrptr;
	elf_proghead *php;
	elf_section **scp;
	void *optdata;
} elf_file;

// elflib.c, elfdump.c, srxgen.c

typedef struct _elf_file_slot
{
	unsigned int offset;
	unsigned int size;
	int align;
	int type;
	union __anon_struct_47
	{
		elf_proghead *php;
		elf_section *scp;
	} d;
} Elf_file_slot;

// readconf.c, srxfixup.c, srxgen.c

typedef struct _segconf
{
	srxfixup_const_char_ptr_t name;
	int bitid;
	srxfixup_const_char_ptr_t *sect_name_patterns; 
	elf_section *empty_section; 
	int nsect; 
	elf_section **scp; 
	unsigned int addr; 
	unsigned int size;
} SegConf;
typedef struct _pheader_info
{
	int sw; 
	union __anon_struct_42
	{
		srxfixup_const_char_ptr_t section_name;
		SegConf **segment_list;
	} d;
} PheaderInfo;
typedef struct _sectconf
{
	srxfixup_const_char_ptr_t sect_name_pattern; 
	unsigned int flag; 
	int secttype; 
	int sectflag;
} SectConf;
typedef struct _crtsymconf
{
	srxfixup_const_char_ptr_t name; 
	int bind; 
	int type; 
	SegConf *segment; 
	srxfixup_const_char_ptr_t sectname; 
	int shindex; 
	int seflag;
} CreateSymbolConf;
typedef struct _srx_gen_table
{
	int target; 
	SegConf *segment_list; 
	CreateSymbolConf *create_symbols; 
	PheaderInfo *program_header_order; 
	SectConf *section_list; 
	srxfixup_const_char_ptr_t *removesection_list; 
	srxfixup_const_char_ptr_t *section_table_order; 
	srxfixup_const_char_ptr_t *file_layout_order;
} Srx_gen_table;

// anaarg.c, srxfixup.c

typedef struct _opt_strings 
{
	struct _opt_strings *next; 
	srxfixup_const_char_ptr_t string;
} Opt_strings;
typedef struct _opttable 
{
	srxfixup_const_char_ptr_t option; 
	int havearg; 
	char vartype; 
	void *var;
} Opttable;

// ring.c, anaarg.c, srxfixup.c

typedef struct _slink
{
	struct _slink *next;
} SLink;

// mipsdis.c, elfdump.c

typedef enum OperandTag
{
	OprTag_none = 0, 
	OprTag_reg = 1, 
	OprTag_c0reg_iop = 2, 
	OprTag_c0reg_ee = 3, 
	OprTag_czreg = 4, 
	OprTag_c1reg = 5, 
	OprTag_imm = 6, 
	OprTag_shamt = 7, 
	OprTag_jtarget = 8, 
	OprTag_regoffset = 9, 
	OprTag_code20 = 10, 
	OprTag_code25 = 11
} OperandTag;
typedef struct operand
{
	enum OperandTag tag; 
	unsigned char reg; 
	unsigned int data;
} Operand;
typedef struct disasm_result 
{
	unsigned int addr; 
	unsigned int data; 
	char mnemonic[16]; 
	Operand operands[4];
} Disasm_result;

// Function definitions

// anaarg.c
extern int analize_arguments(Opttable *dopttable, int argc, char **argv);

// elfdump.c
extern void print_elf(elf_file *elf, int flag);
extern void print_elf_ehdr(elf_file *elf, int flag);
extern void print_elf_phdr(elf_file *elf, int flag);
extern void print_elf_sections(elf_file *elf, int flag);
extern void print_elf_reloc(elf_section *scp, int flag);
extern void print_elf_disasm(elf_file *elf, elf_section *scp, int flag);
extern void print_elf_datadump(elf_file *elf, elf_section *scp, int flag);
extern void print_elf_symtbl(elf_section *scp, int flag);
extern void print_elf_mips_symbols(elf_mips_symbolic_data *symbol, int flag);

// elflib.c
extern elf_file *read_elf(srxfixup_const_char_ptr_t filename);
extern int layout_elf_file(elf_file *elf);
extern int write_elf(elf_file *elf, srxfixup_const_char_ptr_t filename);
extern void add_section(elf_file *elf, elf_section *scp);
extern elf_section *remove_section(elf_file *elf, int shtype);
extern elf_section *remove_section_by_name(elf_file *elf, srxfixup_const_char_ptr_t secname);
extern elf_section *search_section(elf_file *elf, int stype);
extern elf_section *search_section_by_name(elf_file *elf, srxfixup_const_char_ptr_t secname);
extern unsigned int *get_section_data(elf_file *elf, unsigned int addr);
extern elf_syment *search_global_symbol(srxfixup_const_char_ptr_t name, elf_file *elf);
extern int is_defined_symbol(elf_syment *sym);
extern elf_syment *add_symbol(elf_file *elf, srxfixup_const_char_ptr_t name, int bind, int type, int value, elf_section *scp, int st_shndx);
extern unsigned int get_symbol_value(elf_syment *sym, elf_file *elf);
extern void reorder_symtab(elf_file *elf);
extern int adjust_align(int value, int align);
extern void rebuild_section_name_strings(elf_file *elf);
extern void rebuild_symbol_name_strings(elf_file *elf);
extern Elf_file_slot *build_file_order_list(elf_file *elf);
extern void  shrink_file_order_list(Elf_file_slot *efs);
extern void  writeback_file_order_list(elf_file *elf, Elf_file_slot *efs);
extern void dump_file_order_list(elf_file *elf, Elf_file_slot *efs);

// mipsdis.c
extern void  getrs(unsigned int data, Operand *opr);
extern void  getrt(unsigned int data, Operand *opr);
extern void  getrd(unsigned int data, Operand *opr);
extern void  getc0rd_iop(unsigned int data, Operand *opr);
extern void  getc0rd_ee(unsigned int data, Operand *opr);
extern void  getczrd(unsigned int data, Operand *opr);
extern void  getshamt(unsigned int data, Operand *opr);
extern void  getfs(unsigned int data, Operand *opr);
extern void  getft(unsigned int data, Operand *opr);
extern void  getfd(unsigned int data, Operand *opr);
extern void  getczfs(unsigned int data, Operand *opr);
extern void  getbroff(unsigned int addr, unsigned int data, Operand *opr);
extern void  Rs(Disasm_result *result);
extern void  Rd(Disasm_result *result);
extern void  Rdrs(Disasm_result *result);
extern void  Rsrt(Disasm_result *result);
extern void  Rtc0rd_iop(Disasm_result *result);
extern void  Rtc0rd_ee(Disasm_result *result);
extern void  Rtczrd(Disasm_result *result);
extern void  Rdrsrt(Disasm_result *result);
extern void  Rdrtrs(Disasm_result *result);
extern void  Rdrtshamt(Disasm_result *result);
extern void  Rsseimm(Disasm_result *result);
extern void  Rtrsseimm(Disasm_result *result);
extern void  Rtrsimm(Disasm_result *result);
extern void  Rdimm(Disasm_result *result);
extern void  Rsimm(Disasm_result *result);
extern void  Rtimm(Disasm_result *result);
extern void  Rsrtbroff(Disasm_result *result);
extern void  Rsbroff(Disasm_result *result);
extern void  Rtoffbase(Disasm_result *result);
extern void  Fdfs(Disasm_result *result);
extern void  Fsft(Disasm_result *result);
extern void  Fdfsft(Disasm_result *result);
extern void  Rtczfs(Disasm_result *result);
extern void  Code20(Disasm_result *result);
extern void  Jtarget(Disasm_result *result);
extern void  Cofun(Disasm_result *result);
extern void  Bcft(Disasm_result *result);
extern void  gen_asmmacro(Disasm_result *result);
extern void  initdisasm(int arch, int regform0, int regform1, int regform2, int regform3);
extern Disasm_result * disassemble(unsigned int addr, unsigned int data);
extern void  shex(char *buf, unsigned int data);
extern void  format_operand(Operand *opr, char *buf);
extern void  format_disasm(Disasm_result *dis, char *buf);

// readconf.c
extern Srx_gen_table * read_conf(srxfixup_const_char_ptr_t indata, srxfixup_const_char_ptr_t infile, int dumpopt);

// ring.c
extern SLink * add_ring_top(SLink *tailp, SLink *elementp);
extern SLink * add_ring_tail(SLink *tailp, SLink *elementp);
extern SLink * joint_ring(SLink *tailp, SLink *otherring);
extern SLink * ring_to_liner(SLink *tailp);

// srxfixup.c
extern void usage(srxfixup_const_char_ptr_t myname);
extern void stripusage(srxfixup_const_char_ptr_t myname);

// srxgen.c
extern int  convert_rel2srx(elf_file *elf, srxfixup_const_char_ptr_t entrysym, int needoutput, int cause_irx1);
extern int  layout_srx_file(elf_file *elf);
extern void  strip_elf(elf_file *elf);
extern SegConf * lookup_segment(Srx_gen_table *conf, srxfixup_const_char_ptr_t segname, int msgsw);
extern void  fixlocation_elf(elf_file *elf, unsigned int startaddr);
extern int  relocation_is_version2(elf_section *relsect);
extern void  dump_srx_gen_table(Srx_gen_table *tp);

// swapmem.c
extern void  swapmemory(void *aaddr, srxfixup_const_char_ptr_t format, int times);

// eefixconf.c
extern const char *ee_defaultconf;

// iopfixconf.c
extern const char *iop_defaultconf;

enum Ei_class_name_enum
{
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1,
	ELFCLASS64 = 2,
};

#define XEACH_Ei_class_name_enum() \
	X(ELFCLASSNONE) \
	X(ELFCLASS32) \
	X(ELFCLASS64)

enum E_type_name_enum
{
	ET_NONE = 0,
	ET_REL = 1,
	ET_EXEC = 2,
	ET_DYN = 3,
	ET_CORE = 4,
	ET_SCE_IOPRELEXEC = 0xFF80,
	ET_SCE_IOPRELEXEC2 = 0xFF81,
	ET_SCE_EERELEXEC = 0xFF90,
	ET_SCE_EERELEXEC2 = 0xFF91,
};

#define XEACH_E_type_name_enum() \
	X(ET_NONE) \
	X(ET_REL) \
	X(ET_EXEC) \
	X(ET_DYN) \
	X(ET_CORE) \
	X(ET_SCE_IOPRELEXEC) \
	X(ET_SCE_IOPRELEXEC2) \
	X(ET_SCE_EERELEXEC) \
	X(ET_SCE_EERELEXEC2)

enum Ei_data_name_enum
{
	ELFDATANONE = 0,
	ELFDATA2LSB = 1,
	ELFDATA2MSB = 2,
};

#define XEACH_Ei_data_name_enum() \
	X(ELFDATANONE) \
	X(ELFDATA2LSB) \
	X(ELFDATA2MSB)

enum E_version_name_enum
{
	EV_NONE = 0,
	EV_CURRENT = 1,
};

#define XEACH_E_version_name_enum() \
	X(EV_NONE) \
	X(EV_CURRENT)

enum E_machine_name_enum
{
	EM_NONE = 0,
	EM_M32 = 1,
	EM_SPARC = 2,
	EM_386 = 3,
	EM_68K = 4,
	EM_88K = 5,
	EM_860 = 7,
	EM_MIPS = 8,
	EM_MIPS_RS4_BE = 10,
	EM_SPARC64 = 11,
	EM_PARISC = 15,
	EM_SPARC32PLUS = 18,
	EM_PPC = 20,
	EM_SH = 42,
};

#define XEACH_E_machine_name_enum() \
	X(EM_NONE) \
	X(EM_M32) \
	X(EM_SPARC) \
	X(EM_386) \
	X(EM_68K) \
	X(EM_88K) \
	X(EM_860) \
	X(EM_MIPS) \
	X(EM_MIPS_RS4_BE) \
	X(EM_SPARC64) \
	X(EM_PARISC) \
	X(EM_SPARC32PLUS) \
	X(EM_PPC) \
	X(EM_SH)

enum P_type_name_enum
{
	PT_NULL = 0,
	PT_LOAD = 1,
	PT_DYNAMIC = 2,
	PT_INTERP = 3,
	PT_NOTE = 4,
	PT_SHLIB = 5,
	PT_PHDR = 6,
	PT_MIPS_REGINFO = 0x70000000,
	PT_MIPS_RTPROC = 0x70000001,
	PT_SCE_IOPMOD = 0x70000080,
	PT_SCE_EEMOD = 0x70000090,
};

#define XEACH_P_type_name_enum() \
	X(PT_NULL) \
	X(PT_LOAD) \
	X(PT_DYNAMIC) \
	X(PT_INTERP) \
	X(PT_NOTE) \
	X(PT_SHLIB) \
	X(PT_PHDR) \
	X(PT_MIPS_REGINFO) \
	X(PT_MIPS_RTPROC) \
	X(PT_SCE_IOPMOD) \
	X(PT_SCE_EEMOD)

enum S_type_name_enum
{
	SHT_NULL = 0,
	SHT_PROGBITS = 1,
	SHT_SYMTAB = 2,
	SHT_STRTAB = 3,
	SHT_RELA = 4,
	SHT_HASH = 5,
	SHT_DYNAMIC = 6,
	SHT_NOTE = 7,
	SHT_NOBITS = 8,
	SHT_REL = 9,
	SHT_SHLIB = 10,
	SHT_DYNSYM = 11,
	SHT_MIPS_LIBLIST = 0x70000000,
	SHT_MIPS_CONFLICT = 0x70000002,
	SHT_MIPS_GPTAB = 0x70000003,
	SHT_MIPS_UCODE = 0x70000004,
	SHT_MIPS_DEBUG = 0x70000005,
	SHT_MIPS_REGINFO = 0x70000006,
	SHT_SCE_IOPMOD = 0x70000080,
	SHT_SCE_EEMOD = 0x70000090,
};

#define XEACH_S_type_name_enum() \
	X(SHT_NULL) \
	X(SHT_PROGBITS) \
	X(SHT_SYMTAB) \
	X(SHT_STRTAB) \
	X(SHT_RELA) \
	X(SHT_HASH) \
	X(SHT_DYNAMIC) \
	X(SHT_NOTE) \
	X(SHT_NOBITS) \
	X(SHT_REL) \
	X(SHT_SHLIB) \
	X(SHT_DYNSYM) \
	X(SHT_MIPS_LIBLIST) \
	X(SHT_MIPS_CONFLICT) \
	X(SHT_MIPS_GPTAB) \
	X(SHT_MIPS_UCODE) \
	X(SHT_MIPS_DEBUG) \
	X(SHT_MIPS_REGINFO) \
	X(SHT_SCE_IOPMOD) \
	X(SHT_SCE_EEMOD)

enum R_MIPS_Type_enum
{
	R_MIPS_NONE = 0,
	R_MIPS_16 = 1,
	R_MIPS_32 = 2,
	R_MIPS_REL32 = 3,
	R_MIPS_26 = 4,
	R_MIPS_HI16 = 5,
	R_MIPS_LO16 = 6,
	R_MIPS_GPREL16 = 7,
	R_MIPS_LITERAL = 8,
	R_MIPS_GOT16 = 9,
	R_MIPS_PC16 = 10,
	R_MIPS_CALL16 = 11,
	R_MIPS_GPREL32 = 12,
	R_MIPS_GOTHI16 = 21,
	R_MIPS_GOTLO16 = 22,
	R_MIPS_CALLHI16 = 30,
	R_MIPS_CALLLO16 = 31,
	R_MIPS_DVP_11_PCREL = 120,
	R_MIPS_DVP_27_S4 = 121,
	R_MIPS_DVP_11_S4 = 122,
	R_MIPS_DVP_U15_S3 = 123,
	R_MIPSSCE_MHI16 = 250,
	R_MIPSSCE_ADDEND = 251,
};

#define XEACH_R_MIPS_Type_enum() \
	X(R_MIPS_NONE) \
	X(R_MIPS_16) \
	X(R_MIPS_32) \
	X(R_MIPS_REL32) \
	X(R_MIPS_26) \
	X(R_MIPS_HI16) \
	X(R_MIPS_LO16) \
	X(R_MIPS_GPREL16) \
	X(R_MIPS_LITERAL) \
	X(R_MIPS_GOT16) \
	X(R_MIPS_PC16) \
	X(R_MIPS_CALL16) \
	X(R_MIPS_GPREL32) \
	X(R_MIPS_GOTHI16) \
	X(R_MIPS_GOTLO16) \
	X(R_MIPS_CALLHI16) \
	X(R_MIPS_CALLLO16) \
	X(R_MIPS_DVP_11_PCREL) \
	X(R_MIPS_DVP_27_S4) \
	X(R_MIPS_DVP_11_S4) \
	X(R_MIPS_DVP_U15_S3) \
	X(R_MIPSSCE_MHI16) \
	X(R_MIPSSCE_ADDEND)

enum SymbolBinding_enum
{
	STB_LOCAL = 0,
	STB_GLOBAL = 1,
	STB_WEAK = 2,
};

#define XEACH_SymbolBinding_enum() \
	X(STB_LOCAL) \
	X(STB_GLOBAL) \
	X(STB_WEAK)

enum SymbolType_enum
{
	STT_NOTYPE = 0,
	STT_OBJECT = 1,
	STT_FUNC = 2,
	STT_SECTION = 3,
	STT_FILE = 4,
};

#define XEACH_SymbolType_enum() \
	X(STT_NOTYPE) \
	X(STT_OBJECT) \
	X(STT_FUNC) \
	X(STT_SECTION) \
	X(STT_FILE)

enum SymbolSpSection_enum
{
	SHN_UNDEF = 0,
	SHN_MIPS_ACOMMON = 0xFF00,
	SHN_MIPS_TEXT = 0xFF01,
	SHN_MIPS_DATA = 0xFF02,
	SHN_MIPS_SCOMMON = 0xFF03,
	SHN_MIPS_SUNDEFINED = 0xFF04,
	SHN_RADDR = 0xFF1F,
	SHN_ABS = 0xFFF1,
	SHN_COMMON = 0xFFF2,
};

#define XEACH_SymbolSpSection_enum() \
	X(SHN_UNDEF) \
	X(SHN_MIPS_ACOMMON) \
	X(SHN_MIPS_TEXT) \
	X(SHN_MIPS_DATA) \
	X(SHN_MIPS_SCOMMON) \
	X(SHN_MIPS_SUNDEFINED) \
	X(SHN_RADDR) \
	X(SHN_ABS) \
	X(SHN_COMMON)

enum SymbolTypes_enum
{
	stNil = 0,
	stGlobal = 1,
	stStatic = 2,
	stParam = 3,
	stLocal = 4,
	stLabel = 5,
	stProc = 6,
	stBlock = 7,
	stEnd = 8,
	stMember = 9,
	stTypedef = 10,
	stFile = 11,
	stRegReloc = 12,
	stForward = 13,
	stStaticProc = 14,
	stConstant = 15,
	stStaParam = 16,
	stStruct = 26,
	stUnion = 27,
	stEnum = 28,
	stIndirect = 34,
	stStr = 60,
	stNumber = 61,
	stExpr = 62,
	stType = 63,
	stMax = 64,
};

#define XEACH_SymbolTypes_enum() \
	X(stNil) \
	X(stGlobal) \
	X(stStatic) \
	X(stParam) \
	X(stLocal) \
	X(stLabel) \
	X(stProc) \
	X(stBlock) \
	X(stEnd) \
	X(stMember) \
	X(stTypedef) \
	X(stFile) \
	X(stRegReloc) \
	X(stForward) \
	X(stStaticProc) \
	X(stConstant) \
	X(stStaParam) \
	X(stStruct) \
	X(stUnion) \
	X(stEnum) \
	X(stIndirect) \
	X(stStr) \
	X(stNumber) \
	X(stExpr) \
	X(stType) \
	X(stMax)

enum StorageClasse_enum
{
	scNil = 0,
	scText = 1,
	scData = 2,
	scBss = 3,
	scRegister = 4,
	scAbs = 5,
	scUndef = 6,
	scUndefined = 6,
	scCdbLocal = 7,
	scBits = 8,
	scCdbSystem = 9,
	scDbx = 9,
	scRegImage = 10,
	scInfo = 11,
	scUserStruct = 12,
	scSData = 13,
	scSBss = 14,
	scRData = 15,
	scVar = 16,
	scCommon = 17,
	scSCommon = 18,
	scVarRegister = 19,
	scVariant = 20,
	scSUndefined = 21,
	scInit = 22,
	scBasedVar = 23,
	scXData = 24,
	scPData = 25,
	scFini = 26,
	scRConst = 27,
	scMax = 32,
};

#define XEACH_StorageClasse_enum() \
	X(scNil) \
	X(scText) \
	X(scData) \
	X(scBss) \
	X(scRegister) \
	X(scAbs) \
	X(scUndef) \
	X(scUndefined) \
	X(scCdbLocal) \
	X(scBits) \
	X(scCdbSystem) \
	X(scDbx) \
	X(scRegImage) \
	X(scInfo) \
	X(scUserStruct) \
	X(scSData) \
	X(scSBss) \
	X(scRData) \
	X(scVar) \
	X(scCommon) \
	X(scSCommon) \
	X(scVarRegister) \
	X(scVariant) \
	X(scSUndefined) \
	X(scInit) \
	X(scBasedVar) \
	X(scXData) \
	X(scPData) \
	X(scFini) \
	X(scRConst) \
	X(scMax)



// Some convenience macros to make partial accesses nicer
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#define LOW_IND(x,part_type)   0
// first unsigned macros:
#define BYTEn(x, n)   (*((uint8_t*)&(x)+n))
#define WORDn(x, n)   (*((uint16_t*)&(x)+n))
#define DWORDn(x, n)  (*((uint32_t*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,uint8_t))
#define LOWORD(x)  WORDn(x,LOW_IND(x,uint16_t))
#define LODWORD(x) DWORDn(x,LOW_IND(x,uint32_t))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,uint8_t))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,uint16_t))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,uint32_t))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

// now signed macros (the same but with sign extension)
#define SBYTEn(x, n)   (*((int8_t*)&(x)+n))
#define SWORDn(x, n)   (*((int16_t*)&(x)+n))
#define SDWORDn(x, n)  (*((int32_t*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8_t))
#define SLOWORD(x)  SWORDn(x,LOW_IND(x,int16_t))
#define SLODWORD(x) SDWORDn(x,LOW_IND(x,int32_t))
#define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,int8_t))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16_t))
#define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,int32_t))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)


#endif
