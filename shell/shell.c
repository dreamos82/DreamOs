#include <commands.h>

#define NUM_COM 14

/*
 * Inserisce gli argomenti di un comando in un array di stringhe
 * argc = numero degli argomenti
 * argv[0] = nome del comando
 * argv[n] = opzioni successive
 */
void options(char *com)
{
  int i=0;
  argc=0;

  for (; *com; com++)
  {
    argv[argc] = (char *)kmalloc(sizeof(char) * 10);
    while (*com != ' ') {
      *(argv[argc] + i) = *com++;
      i++;
    }
    *(argv[argc] + i) = '\0';
    argc++;
    i=0;
  }
  argv[argc] = '\0';
}

/* corpo della shell */
void shell()
{
  char cmd[256];
  char *user = kmalloc(24);
  char *commands[NUM_COM+1] = { "help", "clear", "poweroff", "kmalloc", "do_fault", "aalogo", "uname", "printmem", "credits", "sleep", "cpuid", "date", "echo", "answer", NULL };
  void (*routines[NUM_COM])(void) = { help, _kclear, poweroff, kmalloc_try, do_fault, aalogo, uname, printmem, credits, sleep_cmd, cpuid, date, echo, answer };
  int i;

  memset(user, 0, strlen(user));
  printf(LNG_USER);
  scanf ("%23s",user);

  while (!strlen(user)) {		
    printf(LNG_USER);
    scanf ("%23s",user);
    printf(LNG_USER_R);
  }
  *(user + strlen(user)) = '\0';
	
  _kclear();
  aalogo();
  printf("\n\n\n\n");
  argc=1;

  for (;;)
  {
    printf("%s~# ",user);
    scanf("%254s",cmd);
        
    options (cmd);
    if (strlen(argv[0]) == 0)
      goto end;

    for (i=NUM_COM; i>=0; --i) {
      if(strcmp(argv[0], commands[i]) == 0) {
        (*routines[i])();
        break;
      }
    }
    if (i<0)
      printf(LNG_UNKNOWN_CMD " %s\n", argv[0]);

    end:
    memset (cmd, 0, strlen(cmd));
    for (--argc; argc>=0; argc--) {
      free (argv[argc]);
    }
  }
}

