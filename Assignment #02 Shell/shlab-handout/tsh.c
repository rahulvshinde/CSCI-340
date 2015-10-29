/*******************************************************************************
NAME: Rahul V Shinde
Chico State ID: 006948956
Assignment # 02
Assignment Name: tsh - A tiny shell program with job control
References:
https://quip.com/ZbXoAEKlVPXW
http://condor.depaul.edu/glancast/374class/docs/slides/Apr09/slide9.html
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"



/* Global variables */

char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */



/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);



/*
* main - The shell's main routine
*/
int main(int argc, char **argv)
{
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1; /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
  * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
      case 'h':             /* print help message */
      usage();
      break;
      case 'v':             /* emit additional diagnostic info */
      verbose = 1;
      break;
      case 'p':             /* don't print a prompt */
      emit_prompt = 0;  /* handy for automatic testing */
      break;
      default:
      usage();
    }
  }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT,  sigint_handler);   /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler);

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1) {

    /* Read command line */
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
    app_error("fgets error");
    if (feof(stdin)) { /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  }

  exit(0); /* control never reaches here */
}

/*
* eval - Evaluate the command line that the user has just typed in
*
* If the user has requested a built-in command (quit, jobs, bg or fg)
* then execute it immediately. Otherwise, fork a child process and
* run the job in the context of the child. If the job is running in
* the foreground, wait for it to terminate and then return.  Note:
* each child process must have a unique process group ID so that our
* background children don't receive SIGINT (SIGTSTP) from the kernel
* when we type ctrl-c (ctrl-z) at the keyboard.
*/
/*******************************************************************************
Function Name: void eval(char *cmdline)
Description: This function parse commandline into argv array. It simply returns
             for black line. This function calls built-in() to check for and
             handle builtin commands like exit, jobs, fg and bg.
*******************************************************************************/

void eval(char *cmdline)
{
  /* Parse command line */
  //
  // The 'argv' vector is filled in by the parseline
  // routine below. It provides the arguments needed
  // for the execve() routine, which you'll need to
  // use below to launch a process.
  //
  char *argv[MAXARGS];

  //
  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
  //

  int bg = parseline(cmdline, argv);
  pid_t pid;
  int jid;
  sigset_t mask;  //represent a signal set
  int status = 0;

  if (argv[0] == NULL)
  return;   /* ignore empty lines */
  if(strcmp(argv[0],"quit")==0 || strcmp(argv[0],"jobs")==0
  || strcmp(argv[0],"bg")==0 || strcmp(argv[0],"fg")==0)
  {
    builtin_cmd(argv);
  }
  else
  {
    // printf("%d", bg);
    sigemptyset(&mask); //initialize and empty a signal set
    sigaddset(&mask,SIGCHLD); //add a signal to a signal set (adds signal
                              //specified by SIGCHLD)
    sigprocmask(SIG_BLOCK, &mask, NULL);  //examines and change blocked signals
    pid = fork(); //create a child process
    if(pid == 0)  // if the process is child process (default PID for child
                  //process is zero)
    {
      sigprocmask(SIG_UNBLOCK, &mask, NULL); //Unblock signals
      setpgid(0,0); //sets pgid for a process
      status = execve(argv[0], &argv[0], NULL); //It executes a program
                                                //specified in argv[0]
      if(status < 0)
      {
        printf("%s", argv[0]);
        printf(": Command not found\n");
        exit(0);
      }
    }

    if(!bg) //Check for foreground processes
    {
      addjob(jobs, pid, FG, cmdline); //adds job to list using jobs.c
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
      waitfg(pid);
    }
    else  //Check for background processes
    {
      addjob(jobs, pid, BG, cmdline);
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
      jid=pid2jid(pid);
      printf("[%d] (%d) %s",jid,pid,cmdline);
    }
  }
  //  exit(0);
  return;
}
/******************************************************************************/

/*******************************************************************************
Function Name: int builtin_cmd(char **argv)
Description:If the user has typed a built-in command then execute it immediately
            . The command name would be in argv[0] and is a C string. We've cast
             this to a C++ string type to simplify string comparisons; however,
             the do_bgfg routine will need to use the argv array as well to look
              for a job number.
*******************************************************************************/

int builtin_cmd(char **argv)
{
  char* choice = argv[0];
  if(strcmp(choice,"quit")==0)
  {
    exit(0);
  }
  else if(strcmp(choice,"jobs")==0)
  {
    listjobs(jobs);
    return 1;
  }
  else if(strcmp(choice,"fg")==0)
  {
    do_bgfg(argv);
    return 1;
  }
  else
  {
    do_bgfg(argv);
    return 1;
  }
  return 0;     /* not a builtin command */
}
/******************************************************************************/

/*******************************************************************************
Function Name: void do_bgfg(char **argv)
Description: Execute the builtin bg and fg commands
*******************************************************************************/

void do_bgfg(char **argv)
{
  struct job_t *jobp = NULL;

  if (argv[1] == NULL)  //If no argument with command
  {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }

  if (isdigit(argv[1][0])) //Parse the required PID or %JID arg
  {
    pid_t pid = atoi(argv[1]);  //convert argv[1] from string to int and stores
                                //in PID
    if (!(jobp = getjobpid(jobs, pid)))
    {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%')
  {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid)))
    {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }
  else
  {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
  if (strcmp(argv[0],"bg") == 0) //if the input command is bg
  {
    if (jobp->state == ST)  //if the job state is stopped
    {
      jobp->state = BG; //change job state to BG
      kill((-jobp->pid), SIGCONT);  // Run a stopped process
      char *cmd_ptr = (char *)&(jobp->cmdline); //retrieve command line from job
      printf("[%d] (%d) %s", jobp->jid, jobp->pid, cmd_ptr); //display move to
                                                             //background
    }
  }
  else  // if the process is running in background
  {
    if (jobp->state == BG || jobp->state == ST) //make sure process is stopped
                                                //or in background
    {
      jobp->state = FG;
      kill((-jobp->pid), SIGCONT);  //Run a stopped process
      waitfg(jobp->pid);
    }
  }
  return;
}
/******************************************************************************/

/*******************************************************************************
Function Name: void waitfg(pid_t pid)
Description: Block until process pid is no longer the foreground process
*******************************************************************************/

void waitfg(pid_t pid)
{
  while(pid == fgpid(jobs)) //loop as long as process state is foreground
  {
    sleep(0);
  }
  return;
}
/******************************************************************************/

/*******************************************************************************
Function Name: void sigchld_handler(int sig)
Description: The kernel sends a SIGCHLD to the shell whenever a child job
             terminates (becomes a zombie), or stops because it received a
             SIGSTOP or SIGTSTP signal. The handler reaps all available zombie
             children, but doesn't wait for any other currently running
             children to terminate.
*******************************************************************************/

void sigchld_handler(int sig)
{
  int status;
  pid_t pid;
  int jid;
  while((pid=waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
  {
    struct job_t *job = getjobpid(jobs,pid);
    if(WIFSTOPPED(status)!=0)
    {
      job->state = ST;
      jid = pid2jid(pid);
      printf("Job [%d] (%d) stopped by signal %d\n", jid,pid,WSTOPSIG(status));
    }
    else if(WIFSIGNALED(status)!=0 ||(WIFEXITED(status)!=0 ))
    {
      if(WIFSIGNALED(status)!=0)
      {
        job->state = ST;
        jid = pid2jid(pid);
        printf("Job [%d] (%d) terminated by signal %d\n", jid,pid,WTERMSIG(status));
      }
      deletejob(jobs,pid);
    }
  }
  return;
}
/******************************************************************************/

/*******************************************************************************
Function Name: void sigint_handler(int sig)
Description: The kernel sends a SIGINT to the shell whenver the user types
             ctrl-c at the keyboard.  Catch it and send it along to the
             foreground job.
*******************************************************************************/

void sigint_handler(int sig)
{
  pid_t fgid = fgpid(jobs);
  if(fgid)
  {
    kill(-fgid, SIGINT); //sends SIGINT signal to the shell
  }
  return;
}
/******************************************************************************/

/*******************************************************************************
Function Name: void sigtstp_handler(int sig)
Description: The kernel sends a SIGTSTP to the shell whenever the user types
             ctrl-z at the keyboard. Catch it and suspend the foreground job by
             sending it a SIGTSTP.
*******************************************************************************/

void sigtstp_handler(int sig)
{
  pid_t fgid = fgpid(jobs);
  if(fgid)
  {
    kill(-fgid, SIGTSTP); // Stops the foreground process
  }
  return;
}
/******************************************************************************/

/*********************
* End signal handlers
*********************/
