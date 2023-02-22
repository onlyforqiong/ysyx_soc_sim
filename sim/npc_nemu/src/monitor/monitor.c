#include <isa.h>
#include <memory/paddr.h>
#include <elf_read.h>
#include <disass.h>

void init_rand();
void init_log(const char *log_file);
void init_mem();
void init_difftest(char *ref_so_file, long img_size, int port);
void init_device();
void init_sdb();


void printf_kongge(int num) {
  for(int i = 0;i<num;i++) {
    printf(" ");
  }
}

static void welcome() {
  Log("Trace: %s", MUXDEF(CONFIG_TRACE, ASNI_FMT("ON", ASNI_FG_GREEN), ASNI_FMT("OFF", ASNI_FG_RED)));
  IFDEF(CONFIG_TRACE, Log("If trace is enabled, a log file will be generated "
        "to record the trace. This may lead to a large log file. "
        "If it is not necessary, you can disable it in menuconfig"));
  Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to %s-NEMU!\n", ASNI_FMT(str(__GUEST_ISA__), ASNI_FG_YELLOW ASNI_BG_RED));
  printf("For help, type \"help\"\n");
  //Log("Exercise: Please remove me in the source code and compile NEMU again.");
  assert(1);
}

#ifndef CONFIG_TARGET_AM
#include <getopt.h>

void sdb_set_batch_mode();

static char *log_file = NULL;




elf_list *elf_read_list = NULL;
static void elf_get(char * data) {
  if(elf_read_list == NULL) {
    elf_read_list = (elf_list *)malloc(sizeof(elf_list));
    elf_read_list -> has_use = 1;
    elf_read_list -> elf_file = data;
    elf_read_list -> next = NULL;
  }else {
    elf_list * p = elf_read_list;
    while(p->next != NULL) {
      p = p -> next;
    }
    p->next = (elf_list *)malloc(sizeof(elf_list));
    p->next->has_use = 1;
    p->next->elf_file  = data;

    p->next->next = NULL;
  }
  
}


static char *diff_so_file = NULL;
static char *img_file = NULL;
static int difftest_port = 1234;

static long load_img() {
  if (img_file == NULL) {
    Log("No image is given. Use the default build-in image.");
    return 4096; // built-in image size
  }

  FILE *fp = fopen(img_file, "rb");
  Assert(fp, "Can not open '%s'", img_file);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  Log("The image is %s, size = %ld", img_file, size);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(guest_to_host(RESET_VECTOR), size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}






ftrace_type ftrace_loop[MAX_FTRACE_LOOP_DEPTH] = {};
int  ftrace_loop_index = 0;

static void ftrace_loop_push(ftrace_type * loop,ftrace_type instance , int* index) {

  loop[*index] = instance;
  (*index) ++;
  if(*index >= MAX_FTRACE_LOOP_DEPTH) {
    *index = 0;
  }
}
void printf_ftrace_type(int type,int *kongge_num) {
    printf_kongge(*kongge_num);
   if(type == func_call){
      *kongge_num = *kongge_num + 1;
      printf("func call");
    }else if(type == func_ret) {
      *kongge_num = *kongge_num - 1;
      printf("func ret ");
    }
}

void ftrace_loop_print(ftrace_type * loop,int index) {
  int i = index;
  int kongge_num = 0;
  elf_list * p = NULL;
  for(;i < MAX_FTRACE_LOOP_DEPTH;i++) {
    printf("    pc -> %08lx ",ftrace_loop[i].pc);
    printf_ftrace_type(ftrace_loop[i].type,&kongge_num);
    if(ftrace_loop[i].type == func_call) {
      p = ftrace_loop[i].elf_ptr;
      printf("@%08lx  %s\n",ftrace_loop[i].target_pc,&p->elf_string_table[p->elf_symbol_table[ftrace_loop[i].symbol_table_index].st_name]); 
    }else{
      printf("@%08lx \n",ftrace_loop[i].target_pc); 
    }

  }
  for(i = 0;i < index - 1;i++) {
     printf("    pc -> %08lx ",ftrace_loop[i].pc);
    printf_ftrace_type(ftrace_loop[i].type,&kongge_num);
  
    if(ftrace_loop[i].type == func_call) {
      p = ftrace_loop[i].elf_ptr;
      printf("@%08lx  %s\n",ftrace_loop[i].target_pc,&p->elf_string_table[p->elf_symbol_table[ftrace_loop[i].symbol_table_index].st_name]); 
    }else{
      printf("@%08lx \n",ftrace_loop[i].target_pc); 
    }
  }
     printf("    pc -> %08lx ",ftrace_loop[i].pc);
    printf_ftrace_type(ftrace_loop[i].type,&kongge_num);

    if(ftrace_loop[i].type == func_call) {
       p = ftrace_loop[i].elf_ptr;
      printf("@%08lx  %s\n",ftrace_loop[i].target_pc,&p->elf_string_table[p->elf_symbol_table[ftrace_loop[i].symbol_table_index].st_name]); 
    }else{
      printf("@%08lx \n",ftrace_loop[i].target_pc); 
    }
}

#ifdef ITRACE_COND
 void get_jal_type (size_t now_pc,size_t target_pc,uint8_t inst_type,uint8_t reg_source,uint32_t imm_value) {
    ftrace_type temp_ftrace = {};
  
    if(inst_type == jal_or_jalr) {
        if(reg_source == 1 && imm_value == 0) {
          // printf("ret ! @%lx\n",target_pc);
          
          temp_ftrace.pc = now_pc;
          temp_ftrace.target_pc = target_pc;
          temp_ftrace.type = func_ret;
          ftrace_loop_push(ftrace_loop,temp_ftrace , &ftrace_loop_index);
          return;
        }
        // printf("jal pc is %lx\n",now_pc);
        // printf("target  pc is %lx\n",target_pc);
        // printf("sec entry num is %lx\n",sec_entry_num);
        if(elf_read_list != NULL) {
          elf_list *p = elf_read_list;
          do{
            for(int i = 0;i < p->sec_entry_num ; i++) {
            // printf("target  pc is %lx\n",target_pc);
              if(p->elf_symbol_table[i].st_value  == target_pc && ELF64_ST_TYPE(p->elf_symbol_table[i].st_info) == STT_FUNC) {
                // ftrace_type temp_ftrace = {};
                  temp_ftrace.elf_ptr = p;
                  temp_ftrace.pc = now_pc;
                  temp_ftrace.target_pc = target_pc;
                  temp_ftrace.type = func_call;
                  // printf("i = %d\n",i);
                  temp_ftrace.symbol_table_index = i;
                  ftrace_loop_push(ftrace_loop,temp_ftrace , &ftrace_loop_index);
                  // printf("it is func!!! and pc = %lx  name = %s \n",target_pc,&elf_string_table[elf_symbol_table[i].st_name]);
                  return;
              }
            }
            p = p->next;
          }while(p != NULL);
          
        }
        
    }
}
#endif
static void load_elf() {
  if (elf_read_list == NULL) {
    printf("No elf is given. ftrace is not working.");
    return ;

  }
  elf_list * p = elf_read_list;
  do{
    char * elf_file = p -> elf_file;

    FILE *fp = fopen(elf_file, "rb");
    Assert(fp, "Can not open '%s'", elf_file);
    int answer = fread(&p->elf_header,sizeof(p->elf_header),1,fp); 

    p->elf_program_header = (Elf64_Phdr*)malloc(p->elf_header.e_phnum * p->elf_header.e_phentsize);
    p->elf_section_header = (Elf64_Shdr*)malloc(p->elf_header.e_shnum * p->elf_header.e_shentsize);

    answer = fseek(fp,p->elf_header.e_phoff,SEEK_SET);

    answer = fread(p->elf_program_header,p->elf_header.e_phnum * p->elf_header.e_phentsize,1,fp); 

    printf("answer = %d\n",answer);


    answer = fseek(fp,p->elf_header.e_shoff,SEEK_SET);
    answer = fread(p->elf_section_header,p->elf_header.e_shnum * p->elf_header.e_shentsize,1,fp); 

    for(int i = 0;i < p->elf_header.e_shnum;i++) {
      if(p->elf_section_header[i].sh_type == SHT_STRTAB) {

        answer = fseek(fp,p->elf_section_header[i].sh_offset,SEEK_SET);
      
        if(i != p->elf_header.e_shstrndx) {
          p->elf_string_table = (char*)malloc(sizeof(char) * p->elf_section_header[i].sh_size);
          answer = fread(p->elf_string_table,sizeof(char) * p->elf_section_header[i].sh_size ,1,fp); 
         
        }else if(i == p->elf_header.e_shstrndx) {
          p->elf_sec_name_table = (char*)malloc(sizeof(char) * p->elf_section_header[i].sh_size);
          answer = fread(p->elf_sec_name_table,sizeof(char) * p->elf_section_header[i].sh_size ,1,fp); 
        }
          
      }else if(p->elf_section_header[i].sh_type == SHT_SYMTAB) { 
        
        p->sec_entry_num = p->elf_section_header[i].sh_size /  p->elf_section_header[i].sh_entsize;
    
        p->elf_symbol_table = (Elf64_Sym*)malloc(p->elf_section_header[i].sh_size );

        answer = fseek(fp,p->elf_section_header[i].sh_offset,SEEK_SET);
        answer = fread(p->elf_symbol_table,p->elf_section_header[i].sh_size,1,fp); 
       
      }
    }
  

  
  fclose(fp);
  p = p->next;

  }while( p != NULL);

}
static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"port"     , required_argument, NULL, 'p'},
    {"ftrace"   , required_argument, NULL, 'f'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
 
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:f:", table, NULL)) != -1) {
    //  printf("o = %c\n",o) ;
    switch (o) {
      case 'b': sdb_set_batch_mode(); break;
      case 'p': sscanf(optarg, "%d", &difftest_port); break;
      case 'l': log_file     = optarg; break;
      case 'd': diff_so_file = optarg; break;
      case 'f': elf_get(optarg);  break;
      case 1:   img_file     = optarg; return optind - 1;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\t-f,--ftrace=FILE        input ELF file\n");
        printf("\n");
        exit(0);
    }
    // printf("\n\n\n\nftrace =  %s\n",elf_file);
  }
  
  return 0;
}

void init_monitor(int argc, char *argv[]) {
  /* Perform some global initialization. */

  /* Parse arguments. */
  parse_args(argc, argv);

  load_elf();

  /* Set random seed. */   
  init_rand();

  /* Open the log file. */ 

  init_log(log_file);


  /* Initialize memory. */
  init_mem();

  /* Initialize devices. */
  IFDEF(CONFIG_DEVICE, init_device());

  /* Perform ISA dependent initialization. */
  init_isa();

  /* Load the image to memory. This will overwrite the built-in image. */
  long img_size = load_img();

  /* Initialize differential testing. */
  init_difftest(diff_so_file, img_size, difftest_port);

  /* Initialize the simple debugger. */
  init_sdb();

  IFDEF(CONFIG_ITRACE, init_disasm(
    MUXDEF(CONFIG_ISA_x86,     "i686",
    MUXDEF(CONFIG_ISA_mips32,  "mipsel",
    MUXDEF(CONFIG_ISA_riscv32, "riscv32",
    MUXDEF(CONFIG_ISA_riscv64, "riscv64", "bad")))) "-pc-linux-gnu"
  ));

  /* Display welcome message. */
  welcome();
}
#else // CONFIG_TARGET_AM
static long load_img() {
  extern char bin_start, bin_end;
  size_t size = &bin_end - &bin_start;
  Log("img size = %ld", size);
  memcpy(guest_to_host(RESET_VECTOR), &bin_start, size);
  return size;
}

void am_init_monitor() {
  init_rand();
  init_mem();
  init_isa();
  load_img();
  IFDEF(CONFIG_DEVICE, init_device());
  welcome();
}
#endif
