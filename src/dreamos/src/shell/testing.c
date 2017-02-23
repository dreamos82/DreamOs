#include <commands.h>
#include <kernel.h>
#include <stddef.h>
#include <video.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <vfs.h>
#include <testing.h>
#include <fcntl.h>
#include <initrd.h>
#include <user_shell.h>
#include <thread.h>
#include <vm.h>
#include <timer.h>
#include <scheduler.h>

char * module_start;
file_descriptor_t fd_list[_SC_OPEN_MAX];

void try_strtok()
{
    char * s = "Hello World";
    char * p;

    printf("Stringa completa: %s\n"
               "Stringa spezzata: \n", s);

    p = strtok(s, " ");
    while (p != NULL)
    {
        printf("%s\n", p);
        p = strtok(NULL, " ");
    }
}

void try_kmalloc()
{
    uint32_t * b, * c, * d;
    uint32_t i = 0;

    printf("Kmalloc try: ... ");
    //print_heap_list (kheap->free_list);
    b = (uint32_t *) kmalloc(15 * sizeof(uint32_t));
    c = (uint32_t *) kmalloc(10 * sizeof(uint32_t));
    d = (uint32_t *) kmalloc(15 * sizeof(uint32_t));
    printf("Address obtained: 0x%x 0x%x 0x%x\n", b, c, d);

    while (i < 15)
    {
        b[i] = i * 2;
        if (i < 10) c[i] = i * 3;
        d[i] = i * 2;
        i++;
    }
    i = 0;

    while (i < 15)
    {
        printf("b[%d] = %d d[%d] = %d ", i, b[i], i, d[i]);
        if (i < 10) printf("c[%d] = %d\n", i, c[i]);
        else printf("\n");
        i++;
    }

    // TODO: Rework tests
    //printf("Navigating used list...\n");
    //print_heap_list (kheap->used_list);
    free(b);
    free(c);
    free(d);
}

void do_fault()
{
    int * prova;

    printf("Genero un pagefault scrivendo 10 nella locazione 0xa0000000...\n");
    prova = (int *) 0xa0000000;
    *prova = 10;
    printf(
        "Contenuto della locazione 0xa0000000 dopo l'intervento dell'handler: %d\n",
        *prova);
}

void try_printmem(void)
{
    //print_heap_list(kheap->used_list);
}

#ifdef LATEST
void try_newheap(){
    #ifdef NEW_HEAP_TEST
    unsigned int testA;
    unsigned int testB;
    unsigned int testC;

    printf("Second step: trying new_malloc...\n");
    testA=(unsigned int)new_malloc(8);
    printf("-Address of testA Hex: 0x%x\n", testA);
    testB=(unsigned int)new_malloc(8);
    printf("-Address of testB Hex: 0x%x\n", testB);
    //printf("testB - testA = %u", testB - testA);
    testC=new_malloc(8);
    printf("-Address of testC Hex: 0x%x\n", testC);
    n_free(testC);
    n_free(testB);
    unsigned int testD;
    testD = new_malloc(12);
    printf("-Address of testD Hex: 0x%x\n", testD);
    n_free(testA);
    n_free(testD);
    printf("DONE\n");
    #endif
}
#endif

void try_module()
{
    initrd_t * fs_head;
    initrd_file_t * headers;
    int i;
    i = 0;
    fs_head = (initrd_t *) module_start;
    headers = (initrd_file_t *) (module_start + sizeof(initrd_t));
    while (i < fs_head->nfiles)
    {
        printf("%s\t%d\n", headers[i].fileName, headers[i].length);
        i++;
    }
    printf("Total Files: %d\n", fs_head->nfiles);
    _kputs("\n");
}

void try_check()
{
    char test_name[50];
    memset(test_name, '\0', 50);
    printf("Please insert a username: ");
    scanf("%s", test_name);
    user_chk(test_name, test_name);
}

void try_open()
{
    char appoggio[50];
    char prova;
    int i;
    i = 0;
    while (i < 50)
    {
        appoggio[i] = '\0';
        i++;
    }
    printf("Please insert a path: ");

    scanf("%s", appoggio);
    i = open(appoggio, O_RDONLY, 42);
    printf("%d\n", i);
    if (i > -1)
    {
        int j = 0;
        while (read(i, &prova, 1) != 0)
        {
            putchar(prova);
            j++;
        }
        //printf("\n%s\n", prova);
        close(i);
    }
    printf("\n");
}

void try_ocreat()
{
    int fd = 0;
    //printf("Number of files present: %d\n", initfs_init());
    fd = open("pippo", O_RDWR | O_CREAT | O_APPEND);
    if (fd >= 0) write(fd, "buffo buffer", strlen("buffo buffer"));
    else printf("Error?\n");
    close(fd);
}

void try_syscall()
{
    int i;
    int var = -100;
    printf("Trying sysputch:\n");
    for (i = 'A'; i <= 'Z'; i++)
    {
        asm(
        "movl %0, %%ecx\n"
            "movl $0x0, %%eax\n"
            "int $80\n"
        : : "g"(i)
        );
    }
    printf("And before ending try to print a -100: %d\n", var);
    printf("\n");
}

void show_fd()
{
    int i = 0;
    while (i < _SC_OPEN_MAX)
    {
        if (fd_list[i].mountpoint_id == -1) printf("-1\n");
        else printf("+%d\n", fd_list[i].mountpoint_id);
        i++;
    }
    printf("\n");
}

void test_stat()
{
    struct stat * stats;
    printf("Testing stat functions on README\n");
    stats = (struct stat *) kmalloc(sizeof(struct stat));
    stat("README", stats);
    printf("Device_id: %d\n", stats->st_dev);
    printf("Size: %d\n", stats->st_size);
    printf("Uid: %d\n", stats->st_uid);
    free(stats);
}

void try_shadow()
{
    char stringa[30];
    memset(stringa, '\0', 30);
    set_shadow(ENABLED);
    scanf("%s", stringa);
    set_shadow(DISABLED);
    printf("%s\n", stringa);
}

void try_mapaddress()
{
    //uint32_t *tmp = (uint32_t *)kmalloc(sizeof(int));
    uint32_t va = 0x00c09000;
    uint32_t pa = 0x00010000;
    printf("\nMapping (virtual -> physical):\n(0x%x) -> (0x%x)\n\n", va, pa);

    uint32_t result_addr = 0;
    map(va, pa, 0);
    get_mapping(va, &result_addr);

    printf("Getting physical address: 0x%x\n", result_addr);

    unmap(va);
    //free (tmp);
}


int task_test_1(void * args)
{
    printf("I'm the task 1.\n");
    return 0;
}

int task_test_2(void * args)
{
    printf("I'm the task 2.\n");
    return 0;
}

void try_thread()
{
    printf("Testing task creation functions...\n");
    asm("cli;"); // Clear Interrupt Flag
    thread_t * thread1 = kernel_create_thread(task_test_1, "task_test_1", 0);
    printf("Task 1, pid: %d\n", thread1->id);
    thread_t * thread2 = kernel_create_thread(task_test_2, "task_test_2", 0);
    printf("Task 2, pid: %d\n", thread2->id);
    asm("sti;"); // Set Interrupt Flag
}

int sleeping_thread(void * args)
{
    sleep(10);
    return 0;
}

void try_thread_sleep()
{
    printf("Testing sleeping thread...\n");
    asm("cli;");
    int i = 0;
    for (; i < 5; ++i)
    {
        kernel_create_thread(sleeping_thread, "sleeping_thread", 0);
    }
    asm("sti;");
}
