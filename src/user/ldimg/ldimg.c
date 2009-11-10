/**
 * \file ldimg.c
 *
 * \date 03.07.2009
 * \author kse
 */
#include "shell_command.h"
#include "fs/rootfs.h"
#include "fs/ramfs.h"
#include "fs/file.h"

#define COMMAND_NAME     "ldimg"
#define COMMAND_DESC_MSG "load image file"
#define HELP_MSG         "Usage: ldimg [-a addr] [-f filename] [-h]"
static const char *man_page =
	#include "ldimg_help.inc"
	;

DECLARE_SHELL_COMMAND(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG, man_page);

#define ENTRY_PTR 0x40000000

int copy_image(file_name) {

//    extern char _piggy_start, _piggy_end, _data_start;
//
//    char *src = &_piggy_start;
//    char *dst = (char*)ENTRY_PTR;
//
//    if (&_piggy_start == &_piggy_end) {
//        printf("No any images for ldimg in this build.\n");
//        return;
//    }
//
//
//    printf("\nCopy piggy image\n");
//    printf("from %08X to %08X size %d bytes\n", (unsigned)src, (unsigned)dst, (unsigned)(&_piggy_end) - (unsigned)(&_piggy_start));
//    while (src < &_piggy_end) {
//        *dst++ = *src++;
//    }
    void *romfile;
    void *ramfile;
    char romfname[0x40];
    char ramfname[0x40];
    char buff[0x1000];


    sprintf(romfname, "%s%s", "/ramfs/", file_name);
    if (NULL == (romfile = fopen(romfname, "r"))){
        printf ("Error:Can't open file %s\n", romfname);
        return -1;
    }
    sprintf(ramfname, "%s%s", romfname, "_loaded");
    if (NULL == (ramfile = fopen(ramfname,"w"))){
        printf ("Error:Can't create ramfs disk");
        return -1;
    }
    while(0 < fread(buff, sizeof(buff), 1, romfile)){
        fwrite(buff, sizeof(buff), 1, ramfile);
    }
}


static int exec(int argsc, char **argsv){
	RAMFS_CREATE_PARAM param;
	char *file_name;
	unsigned int base_addr;
	char ramfname[0x40];
	FSOP_DESCRIPTION *fsop;
        int nextOption;
        getopt_init();
        do {
                nextOption = getopt(argsc, argsv, "f:a:h");
                switch(nextOption) {
                case 'h':
                        show_help();
                        return 0;
                case 'f':
            		file_name = optarg;
            		break;
            	case 'a':
            		if ((optarg != NULL) && (!sscanf(optarg, "0x%x", &base_addr))
            		            && (!sscanf(optarg, "%d", (int *) &base_addr))) {
            		        LOG_ERROR("ERROR: hex value expected.\n");
            		        show_help();
            		        return -1;
            		}
            		break;
                case -1:
                        break;
                default:
                        return 0;
                }
        } while(-1 != nextOption);

	if (NULL == (fsop = rootfs_get_fsopdesc("/ramfs/"))){
		LOG_ERROR("Can't find ramfs disk");
		return -1;
	}

	param.size = 0x1000000;
        param.start_addr = (unsigned int )(base_addr);
        sprintf(param.name, "%s%s", file_name, "_loaded");
	if (-1 == fsop->create_file(&param)){
	    printf ("Error:Can't create ramfs disk");
	    return -1;
	}
	return copy_image(file_name);
}
