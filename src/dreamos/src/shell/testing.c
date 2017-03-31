#include <commands.h>
#include <kernel.h>
#include <video.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <vfs.h>
#include <testing.h>
#include <fcntl.h>
#include <initrd.h>
#include <process.h>
#include <vm.h>
#include <timer.h>
#include <queue.h>
#include <mutex.h>
#include <scheduler.h>
#include <spinlock.h>

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
    kfree(b);
    kfree(c);
    kfree(d);
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
    fs_head = (initrd_t *) module_start;
    headers = (initrd_file_t *) (module_start + sizeof(initrd_t));
    uint32_t it = 0;
    while (it < fs_head->nfiles)
    {
        printf("%s\t%d\n", headers[it].fileName, headers[it].length);
        ++it;
    }
    printf("Total Files: %d\n", fs_head->nfiles);
    video_puts("\n");
}

void try_check()
{
    credentials_t credentials;
    init_credentials(&credentials);
    printf("Please insert a username: ");
    scanf("%50s", credentials.username);
    printf("Please insert a password: ");
    scanf("%50s", credentials.password);
    if (check_credentials(&credentials))
    {
        printf("[Correct]\n");
    }
    else
    {
        printf("[Wrong]\n");
    }
    printf("\n");
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
    fd = open("pippo", O_RDWR | O_CREAT | O_APPEND, 0);
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
        __asm__(
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
    kfree(stats);
}

void try_shadow()
{
    printf("Now try to write something:");
    char str[30];
    memset(str, '\0', 30);
    keyboard_set_shadow(true);
    scanf("%s", str);
    keyboard_set_shadow(false);
    printf("You have written: %s\n", str);
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

spinlock_t spinlock;

int task_test_1(void * args)
{
    (void) args;
    printf("Task 1: I'm waiting for the spinlock...\n");
    spinlock_lock(&spinlock);
    printf("Task 1: I'm in control of the spinlock...\n");
    spinlock_unlock(&spinlock);
    return 0;
}

int task_test_2(void * args)
{
    (void) args;
    printf("Task 2: I'm waiting for the spinlock...\n");
    spinlock_lock(&spinlock);
    printf("Task 2: I'm in control of the spinlock...\n");
    spinlock_unlock(&spinlock);
    return 0;
}

void try_process()
{
    printf("Testing task creation functions...\n");
    spinlock_init(&spinlock);
    spinlock_lock(&spinlock);
    // Disable the IRQs.
    irq_disable();
    process_t * process1 = kernel_create_process(task_test_1,
                                              "task_test_1",
                                              "task_test_1",
                                              0);
    printf("Task 1, pid: %d\n", process1->id);
    process_t * process2 = kernel_create_process(task_test_2,
                                              "task_test_2",
                                              "task_test_2",
                                              0);
    printf("Task 2, pid: %d\n", process2->id);
    // Re-Enable the IRQs.
    irq_enable();
    sleep(5);
    spinlock_unlock(&spinlock);
}

int sleeping_process(void * args)
{
    (void) args;
    sleep(4);
    return 0;
}

void try_process_sleep()
{
    printf("Testing sleeping process...\n");
    // Disable the IRQs.
    irq_disable();
    int i = 0;
    for (; i < 5; ++i)
    {
        kernel_create_process(sleeping_process,
                             "sleeping_process",
                             "sleeping_process",
                             0);
    }
    // Re-Enable the IRQs.
    irq_enable();
}

void try_queue()
{
    queue_t queue = queue_create(sizeof(int));
    if (queue == NULL)
    {
        printf("Error while creating the queue.\n");
        return;
    }
    printf("Enqueuing...");
    int a = 1;
    int b = 2;
    int c = 3;
    queue_enqueue(queue, &a);
    queue_enqueue(queue, &b);
    queue_enqueue(queue, &c);

    printf("Dequeue : ");
    while (!queue_is_empty(queue))
    {
        int value;
        if (queue_front_and_dequeue(queue, &value))
        {
            printf("%d ", value);
        }
    }
    printf("\n");
    queue_destroy(queue);
}