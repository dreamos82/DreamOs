#include <commands.h>
#include <video.h>
#include <timer.h>
#include <cpuid.h>
#include <stdio.h>
#include <string.h>
#include <language.h>
#include <version.h>
#include <sys/utsname.h>
#include <mouse.h>
#include <vfs.h>
#include <testing.h>
#include <fcntl.h>
#include <clock.h>
#include <scheduler.h>
#include <debug.h>
#include "irqflags.h"

void aalogo(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    video_set_color(BRIGHT_GREEN);
    printf("\t\t ____           _____          _____ _____\n");
    printf("\t\t|    \\ ___ ___ |  _  | _______|     |   __|\n");
    printf("\t\t|  |  |  _| -_|| |_| ||       |  |  |__   |\n");
    printf("\t\t|____/|_| |___||_| |_||_|\134_/|_|_____|_____|\n");
    video_set_color(WHITE);
    printf("\t\tVersion: \"%s %s.%s%s\"\n", NAME, VERSION, PATCHLEVEL,
           EXTRAVERSION);
    //video_set_color(WHITE);
    logo(1, NULL);
}

void logo(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    video_set_color(BRIGHT_BLUE);
    printf("\n");
    printf("\t\t\t The Dream Operating System \n"
                   "\t\t           v%s.%s%s %s      \n\n"
                   "\t\t\t   Welcome to DreamOS\n"
                   "\t  Where dreams don't become Reality and remain dreams. \n"
                   "\t          R.I.P - Rest in peace with dreamos ^_^        \n",
           VERSION, PATCHLEVEL, EXTRAVERSION, REV_NUM);

    printf("\n\n\n\n");
    video_set_color(WHITE);
}

void help(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    printf("Available commands:\n");
    int i = 0;
    for (i = 0; i < MAX_NUM_COM; ++i)
    {
        // Skip commands with undefined functions.
        if (shell_cmd[i].function == NULL) continue;
        printf("  %-10s %s\n",
               shell_cmd[i].cmdname,
               shell_cmd[i].cmddesc);
    }
}

void echo(int argc, char ** argv)
{
    int i = argc;
    int j = 0;
    if (argc == 1)
    {
        printf("");
    }
    else
    {
        while (--i > 0)
        {
            printf("%s ", argv[++j]);
        }
    }
    printf("\n");
}

void poweroff(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    printf("Poweroff...\n");

    __asm__ __volatile__("cli;");
    printf("Now you can shut down the PC.\n");
    while (1);
}

void uname_cmd(int argc, char ** argv)
{
    utsname_t utsname;
    uname(&utsname);
    if (argc != 2)
    {
        printf(utsname.sysname);
        return;
    }
    if (!(strcmp(argv[1], "-a")) || !(strcmp(argv[1], "--all")))
    {
        printf("%s %s.%s%s Hash: %s #1 CEST 2013 %s\n", utsname.sysname,
               utsname.version, utsname.release, EXTRAVERSION, REV_NUM,
               sinfo.cpu_vendor);
    }
    else if (!(strcmp(argv[1], "-r")) || !(strcmp(argv[1], "--rev")))
    {
        printf("%s.%s%s\n", utsname.version, utsname.release, EXTRAVERSION);
    }
    else if (!(strcmp(argv[1], "-h")) || !(strcmp(argv[1], "--help")))
    {
        printf(
                "Uname function allow you to see the kernel and system information.\n");
        printf("Function avaibles:\n");
        printf("1) -a   - Kernel version and processor type\n"
                       "2) -r   - Only the kernel version\n"
                       "3) -i   - All info of system and kernel\n");
    }
    else if (!(strcmp(argv[1], "-i")) || !(strcmp(argv[1], "--info")))
    {
        printf("\n:==========: :System info: :==========:\n\n");

        // Kernel info
        printf("Version: %s\n"
                       "Patchlevel: %s\n"
                       "Extraversion: %s\n"
                       "Name: %s\n"
                       "Revision: %s\n", VERSION, PATCHLEVEL, EXTRAVERSION,
               NAME,
               REV_NUM);

        // CPU Info
        video_puts(LNG_CPU);
        video_set_color(BRIGHT_RED);
        video_move_cursor(61, video_get_line());
        video_puts(sinfo.cpu_vendor);
        video_set_color(WHITE);
        printf("\n");

        // Memory RAM Info
        /*printf("Memory RAM: ");
        video_move_cursor(60, video_get_line());
        printf(" %d Kb\n", get_memsize()/1024);

        // Memory free RAM Info
        printf(LNG_FREERAM);
        video_move_cursor(60, video_get_line());
        printf(" %d Kb\n", get_numpages());

        printf("\n");

        // Bitmap Info
        printf("Number bitmap's elements: ");
        video_move_cursor(60, video_get_line());
        printf(" %d", get_bmpelements());
        video_move_cursor(60, video_get_line());*/

        // Mem_area Info
        //printf("\nSize of mem_area: ");
        //video_move_cursor(60, video_get_line());
        //printf(" %d\n", sizeof(mem_area));

        // Page Dir Info
        /*printf("Page Dir Entry n.0 is: ");
        video_move_cursor(60, video_get_line());
        printf(" %d\n", get_pagedir_entry(0));

        // Page Table Info
        printf("Page Table Entry n.4 in Page dir 0 is: ");
        video_move_cursor(60, video_get_line());
        printf(" %d\n", get_pagetable_entry(0,4));*/

        printf("\n:==========: :===========: :==========:\n\n");
    }
    else
    {
        printf("%s. For more info about this tool, please do 'uname --help'\n",
               utsname.sysname);
    }
}

void credits(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    video_set_color(BRIGHT_BLUE);
    video_puts("DreamOS Credits\n\n");
    video_puts("Main Developers:\n");
    video_set_color(GREEN);
    printf("Finarfin - Ivan Gualandri (Founder)\n\n");
    video_set_color(BRIGHT_BLUE);
    video_puts("Contributors of the past:\n");
    video_set_color(GREEN);
    video_puts("Lord Osiris - Diego Stamigni\n"
                       "N3m3s1s\n"
                       "Blackz\n"
                       "vinc94\n"
                       "tk0\n"
                       "DT\n"
                       "Celeron\n"
                       "Hamcha\n"
                       "m0nt0\n"
                       "and many others (3 or 4 :P)\n\n");
    video_set_color(WHITE);
}

void sleep_cmd(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("%s: missing operand.\n", argv[0]);
        printf("Try '%s --help' for more information.\n\n", argv[0]);
        return;
    }
    if (!strcmp(argv[1], "--help"))
    {
        printf("Usage: %s <seconds>\n\n", argv[0]);
        return;
    }
    int seconds = atoi(argv[1]);
    if (seconds <= 0)
    {
        printf("Error: You must provide a positive value (%d). \n\n",
               seconds);
        return;
    }
    sleep((time_t) seconds);
}

void cpuid_help(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    printf("CPUID help message\n"
                   "-v : shows verbose CPUID information\n");
}

/*
 * Call CPUID command and display information
 */
void cpuid(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    /* List of features */
    const char * ecx_features[ECX_FLAGS_SIZE] = {
            "SSE3",
            "Reserved",
            "Reserved",
            "Monitor/MWAIT",
            "CPL Debug Store",
            "Virtual Machine",
            "Safer Mode",
            "Enhanced Intel SpeedStep Technology",
            "Thermal Monitor 2",
            "SSSE3",
            "L1 Context ID",
            "Reserved",
            "Reserved",
            "CMPXCHG16B",
            "xTPR Update Control",
            "Perfmon and Debug Capability",
            "Reserved",
            "Reserved",
            "DCA",
            "SSE4.1",
            "SSE4.2",
            "Reserved",
            "Reserved",
            "POPCNT"
    };
    const char * edx_features[EDX_FLAGS_SIZE] = {
            "x87 FPU",
            "Virtual 8086 Mode",
            "Debugging Extensions",
            "Page Size Extensions",
            "Time Stamp Counter",
            "RDMSR and WRMSR",
            "Physical Address Extensions",
            "Machine Check Exception",
            "CMPXCHG8B",
            "APIC On-chip",
            "Reserved",
            "SYSENTER and SYSEXIT",
            "Memory Type Range Registers",
            "PTE Global Bit",
            "Machine Check Architecture",
            "Conditional Move Instructions",
            "Page Attribute Table",
            "36-bit Page Size",
            "Processor Serial Number",
            "Reserved",
            "Debug Store",
            "Thermal Monitor and Clock Facilities",
            "Intel MMX",
            "FXSAVE and FXRSTOR",
            "SSE",
            "SSE2",
            "Self Snoop",
            "Multi-Threading",
            "TTC",
            "Reserved",
            "Pending Break Enable"
    };

    int i;
    int verbose = 0;

    /* Examine possible options */
    if (argv[1] != NULL)
    {
        if (strcmp(argv[1], "-v") == 0)
            verbose = 1;
        else
        {
            printf("Unknown option %s\n", argv[1]);
            cpuid_help(1, NULL);
            return;
        }
    }

    printf("----- CPU ID Information -----\n");
    if (strcmp(sinfo.brand_string, "Reserved") != 0)
        printf("%s\n", sinfo.brand_string);
    printf("Vendor: %s\n", sinfo.cpu_vendor);
    printf("Type: %s, Family: %x, Model: %x\n", sinfo.cpu_type,
           sinfo.cpu_family, sinfo.cpu_model);
    printf("Apic ID: %d\n", sinfo.apic_id);

    if (verbose == 1)
    {
        printf("\n--- Supported features ---\n");
        for (i = 0; i < ECX_FLAGS_SIZE; i++)
        {
            if (sinfo.cpuid_ecx_flags[i] == 1)
                printf("%s\n", ecx_features[i]);
        }
        for (i = 0; i < EDX_FLAGS_SIZE; i++)
        {
            if (sinfo.cpuid_edx_flags[i] == 1)
                printf("%s\n", edx_features[i]);
        }
        printf("---------------------------\n");
    }
}

void drv_load(int argc, char ** argv)
{
    if (argc < 2)
        printf(
                "No driver inserted or bad usage! Type %s --help for the usage.\n",
                argv[0]);

    else
    {
        if ((_kstrncmp(argv[1], "-r", 2) == 0))
        {
            if ((argv[2] != NULL))
            {
                if (_kstrncmp(argv[2], "mouse", 5) == 0)
                {
                    printf("Disattivamento %s in corso..\n", argv[2]);
                    mouse_disable();
                }
                else
                    printf("FATAL: Driver %s not found.\n", argv[2]);
            }
            else
                printf("Warning, no driver name inserted!\n");
        }
            // per ora lo impostiamo cos�, static,
            // tanto non abbiamo nessun altro modulo, per ora..
        else if (_kstrncmp(argv[1], "mouse", 5) == 0)
        {
            // enabling mouse
            mouse_install();
        }

        else if ((_kstrncmp(argv[1], "--help", 6) == 0) ||
                 (_kstrncmp(argv[1], "-h", 2) == 0))
        {
            printf("---------------------------------------------------\n"
                           "Driver tool to load and kill driver\n"
                           "Simple to use, just type:\n"
                           "\n"
                           "Usage: %s -<options> driver_name\n"
                           "\t-> %s module_name     - to load driver\n"
                           "\t-> %s -r module_name  - to kill driver\n"
                           "---------------------------------------------------\n",
                   argv[0], argv[0], argv[0]);
        }

        else
        {
            if ((_kstrncmp(argv[1], "-r", 2) == 0) &&
                (_kstrncmp(argv[2], "mouse", 5) == -1))
                printf("FATAL: Driver %s not found.\n", argv[2]);

            else
                printf("FATAL: Driver %s not found.\n", argv[1]);
        }

    }

}

void ls(int argc, char ** argv)
{
//    dbg_print("Opened DIR : '%s'\n", current_user.cur_path);
    // Check if the user has provided the '-l' option.
    int flag = 0;
    if (argc > 1 && strcmp(argv[1], "-l") == 0)
    {
        flag = 1;
    }
    // Initialize the variables.
    int i = 0, j = 0;
    DIR * dirp;
    while (strcmp(mountpoint_list[i].mountpoint, ""))
    {
        j++;
        i++;
    }
    i = 0;
    dirp = opendir(current_user.cur_path);
    if (dirp != NULL)
    {
        int tot = 0;
        /*while(i<j){
            if(strcmp(mountpoint_list[i].mountpoint, current_user.cur_path)){

            }
            i++;
        }*/
        struct dirent * cur_dir_entry;
        cur_dir_entry = readdir(dirp);
        while (cur_dir_entry != NULL)
        {
            //struct stat
            if (cur_dir_entry->d_type == FS_DIRECTORY)
            {
                video_set_color(BRIGHT_CYAN);
            }
            if (cur_dir_entry->d_type == FS_MOUNTPOINT)
            {
                video_set_color(BRIGHT_GREEN);
            }
            if (flag == 1)
            {
                struct stat tmp_stat;
                if (stat(cur_dir_entry->d_name, &tmp_stat) != -1)
                {
                    printf("uid=%d(%s) - size:  %d ",
                           tmp_stat.st_uid,
                           current_user.username,
                           tmp_stat.st_size);
                    tot = tot + tmp_stat.st_size;
                }
            }
            printf("%s\n", cur_dir_entry->d_name);
            video_set_color(WHITE);
            cur_dir_entry = readdir(dirp);
        }
        closedir(dirp);
        printf("\n");
        if (flag == 1)
        {
            printf("Total: %d byte\n", tot);
        }
    }
    else
    {
        video_set_color(BRIGHT_BLUE);
        while (i < j)
        {
            if (flag)
            {
                printf("uid=%d(%s), gid=%d(%s) - %s\n",
                       mountpoint_list[i].uid,
                       current_user.username,
                       mountpoint_list[i].gid,
                       current_user.username,
                       mountpoint_list[i].mountpoint);
            }
            else
            {
                printf("%s ", mountpoint_list[i].mountpoint);
            }
            i++;
        }
        video_set_color(WHITE);
        printf("Total: %d\n", j);
    }
}

void more(int argc, char ** argv)
{
    if (argc == 1)
        printf("Usage:\n\t more filename\nfor read a file\n");
    else
    {
        if (argc < 3)
        {
            int i;
            char buf;
            //printf("File to open: %s\n", argv[1]);
            i = open(argv[1], O_RDONLY, 42);
            if (i > -1)
            {
                int j = 0;
                while (read(i, &buf, 1) != (int) NULL)
                {
                    putchar(buf);
                    j++;
                }
                putchar('\n');
                close(i);
            }
        }
        else printf("too many arguments\n");
    }
}

void cd(int argc, char ** argv)
{
    // Check the number of arguments.
    if (argc != 2)
    {
        printf("Bad usage. Try 'ls -l' and then 'cd dir'.\n");
        return;
    }
    char abspath[CURPATH_LEN];
    memset(abspath, '\0', CURPATH_LEN);

    DIR * dirp = NULL;

    int i = 0;

    if (argv[1][0] == '/')
    {
        i = get_mountpoint_id(argv[1]);
        strcpy(abspath, argv[1]);
        dirp = opendir(argv[1]);
    }
    else if (!strncmp(argv[1], "..", 2))
    {
        printf(".. option %s\n", argv[1]);
        return;
    }
    else if (argv[1][0] == '.')
    {
        //printf(". option\n");
        if (strlen(argv[1]) == 1)
        {
            return;
        }
        printf("str_len: %d\n", strlen(argv[1]));
    }
    else
    {
        // Copy the current path.
        strcpy(abspath, current_user.cur_path);
        // Get the absolute path.
        get_abs_path(abspath);
        // If the current directory is not the root, add a '/'
        if (strcmp(abspath, "/") != 0) strncat(abspath, "/", 1);
        // Concatenate the input dir.
        strncat(abspath, argv[1], strlen(argv[1]));
        // Open the directory.
        dirp = opendir(abspath);
    }
    if ((dirp == NULL) || (i == -1))
    {
        printf("cd: %s: No such file or directory\n\n", argv[1]);
        return;
    }
    closedir(dirp);
    strcpy(current_user.cur_path, abspath);
}

void whoami(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    printf("%s\n", current_user.username);
}

void tester(int argc, char ** argv)
{
    int i = 0;
    struct devel testing[MAX_TEST] = {
            {"try_kmalloc",       "Test a basic kmalloc() function",               try_kmalloc},
            {"try_strtok",        "Test strtok() function in string.h",            try_strtok},
            {"try_printmem",      "Print used locations of memory",                try_printmem},
            {"try_ocreat",        "Test file creation",                            try_ocreat},
            {"try_open",          "Function to test open() & stdarg()",            try_open},
            {"try_syscall",       "Try some syscall functions",                    try_syscall},
            {"show_fd",           "Test file descriptors",                         show_fd},
            {"test_stat",         "Test stat function",                            test_stat},
            {"try_check",         "Test username if exist",                        try_check},
            {"try_shadow",        "Test shadow feature for text input",            try_shadow},
            {"try_mapaddress",    "Test map address function",                     try_mapaddress},
            {"try_process",       "Test multiple processs creation",               try_process},
            {"try_process_sleep", "Creates a process which sleeps for 10 seconds", try_process_sleep},
            {"try_queue",         "Tries the queue",                               try_queue},
    };
    if (argc != 2)
    {
        printf("Bad usage. Try '%s --help' for more info about the usage.\n",
               argv[0]);
        return;
    }
    if (!strcmp(argv[1], "--help"))
    {
        printf("Testing functions.. ");
        video_set_color(4);
        printf("Warning: for developers only!\n");
        video_set_color(7);
        for (i = 0; i < MAX_TEST; i++)
        {
            if (testing[i].func == NULL)
            {
                break;
            }
            printf("    [%-2d] %-20s%-20s\n", i,
                   testing[i].cmd_testname,
                   testing[i].cmd_description);
        }
        return;
    }
    int testid = atoi(argv[1]);
    for (i = 0; i < MAX_TEST; i++)
    {
        if (testid == i)
        {
            (testing[i].func)();
            break;
        }
    }
    if (i >= MAX_TEST)
    {
        printf("Error: tester %s not found.\n", argv[1]);
        printf("       You have to provide the test id.\n");
    }
}

void pwd(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    printf("%s\n", current_user.cur_path);
}

void newfile(int argc, char ** argv)
{
    if (argc != 2)
    {
        printf("Bad usage. Try newfile --help for more informations\n");
    }
    else
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf(
                    "newfile FILENAME - Make a new file, and prompt for it's content\n");
        }
        else
        {
            int fd;
            fd = open(argv[1], O_RDONLY, 0);
            if (fd >= 0) printf("-ERROR: %s File already exist\n", argv[1]);
            else
            {
                char text[256];
                printf("Filename: %s\n", argv[1]);
                close(fd);
                fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, 0);
                if (fd >= 0)
                {
                    printf(
                            "Type your text here, actually only one line available!!\n");
                    scanf("%s", text);
                    write(fd, text, strlen(text));
                    if (close(fd) == -1) printf("something went wrong\n");
                }
            }
        }
    }
}

void ps(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    // Print the header.
    printf("%-6s", "PID");
    printf("%-50s", "NAME");
    printf("\n");
    size_t nprocs = list_size(process_list);
    for (size_t i = 0; i < nprocs; ++i)
    {
        listnode_t * node = list_get_node_by_index(process_list, i);
        if (node)
        {
            process_t * process = node->val;
            if (process)
            {
                printf("%-6d", process->id);
                printf("%-50s", process->name);
                printf("\n");
            }
        }
    }
    printf("\n");
}

void date(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    // Disable the IRQs.
    irq_disable();
    printf("%s %x:%x:%x %s %s %02x %s %02x\n",
           LNG_TIMESTAMP,
           get_hour(), get_minute(), get_second(),
           LNG_TIMESTAMP3,
           get_day_lng(), get_day_m(), get_month_lng(), 0x2000 + get_year());
    // Re-Enable the IRQs.
    irq_enable();
}

void clear(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    video_clear();
}

void showpid(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    process_t * current_process = kernel_get_current_process();
    printf("pid %d\n", current_process->id);
}

void heapdump(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    print_heap();
}

