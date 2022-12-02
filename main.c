#define BUFFER_LEN 1024
#define MOOGLETSL " \t\r\n\a"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *moogle_args[] = {"tree", "list", "exit"}; // argument used in the terminal
char history[4][BUFFER_LEN];                    // save the last 4 command used
int count = 0;
int historycount = 0;
char *arg = "";

/*
 * intializing the shell and the printing out the start of the shell
 */
void moogle_init() {

  printf("******** **      ** *****  ****   ****  **    **    ******  "
         "*********\n");
  printf("******** ** *  * ** ** ** **     **     **    **    **      "
         "*********\n");
  printf("         **  **  ** ** ** ** *** ** *** **    **    ******           "
         "\n");
  printf("******** **      ** ** ** **  ** **  ** **    **    **      "
         "*********\n");
  printf("******** **      ** *****   ***    ***  ***** ***** ******  "
         "*********\n");
}
// this function reads the input from the command line
char *read_line(void) {
  char *line = NULL;
  ssize_t buffersize = 0; // have getline allocate a buffer for us

  if (getline(&line, &buffersize, stdin) == -1) {
    // checking the end of file
    if (feof(stdin)) {
      exit(EXIT_SUCCESS); // We recieved an EOF
    } else {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

// this function would take the input and put it in array and return the pointer
char **Shell_input(char *input) {
  // allocates the requested memory and returns a pointer
  char *space = " "; // space
  char **commands = malloc(8 * sizeof(char *));
  char *something = " ";

  if (commands == NULL) {
    perror("malloc failed");
    exit(1);
   
  }
  char *parsed_text ="";
  int index = 0;

  parsed_text = strtok(input, MOOGLETSL); // split the string by character space

  if(parsed_text != NULL){
    stpcpy(history[historycount], parsed_text);
  }

  while (parsed_text != NULL) {
    commands[index] =
        parsed_text; // safes the parsed text into the pointer array
    index++;
    parsed_text = strtok(NULL, space);
   
    
  }

  commands[index] = NULL;
  return commands;
}

int execute_command(char **command) {
  pid_t child;
  int stat_loc;
  child = fork(); // creating a child

  if (child < 0) {
    perror("Fork failed");
    exit(1); // if fork()) failed exit
  } else if (child == 0) {

    execvp(command[0], command); // execute the command

  } else {
    waitpid(child, &stat_loc, WUNTRACED);
  }
  historycount++; // increasing the history count after the command is executed
  return 1;
}

// code for tree
int moogle_tree(char **args) {
  int pid;
  pid = fork();
  if (pid < 0) { // error message
    fprintf(stderr, "ERROR: Fork failed");
    exit(-1);
  } else if (pid == 0) { // forking to make dir
    printf("Making dir0 with all access permission...\n");
    sleep(2);
    mkdir("dir0", 0777); // dir0 using all access modifier
    exit(0);             // terminate child
  } else { //parent process 
    wait(NULL); // wait for child to complete
    printf("Changing directory to dir0...\n");
    sleep(2);
    chdir("dir0"); // change directory internally

    printf("Creating t1.txt...\n");
    sleep(1);
    FILE *textfile1 = fopen("t1.txt", "w");//write access modifier 
    fclose(textfile1);

    printf("Creating t2.txt...\n");
    sleep(1);
    FILE *textfile2 = fopen("t2.txt", "w");
    fclose(textfile2); // resource your program is using such as memory it no
                       // longer needs, so should be closed after being used

    printf("Creating t3.txt...\n");
    sleep(1);
    FILE *textfile3 = fopen("t3.txt", "w");
    fclose(textfile3);
    mkdir("dir1", 0777); // dir1 using 0777 (all access)
  }
}

int moogle_list(char **args) {
  // clear screen
  printf("Clearing screen...\n");
  system("clear");

  // list file
  printf("List file...\n");
  system("ls -l");

  // output the list to t1.txt
  printf("Output ...\n");
  system("ls -l . > t1.txt");

  // rename it to tree
  printf("Renaming to tree...\n");
  system("mv t1.txt tree.txt");

  return 0;
}

int moogle_exit(char **args, int count) {
  // print out the last 4 command used
  if (historycount != 0) {
    printf("Previously Executed Commands: \n");
    for (int i = 0; i < historycount; i++) {
      printf("%d: %s\n", i + 1, history[i]);
    }
  } else {
    printf("there is no command history \n");
  }
  printf("List files...\n");
  
  int pid = fork();
  if (pid < 0) { // error message
    fprintf(stderr, "ERROR: Fork failed");
    exit(-1);
  }
  else if (pid > 0) {
    // Parent
    wait(NULL);                                         // Wait For Child
    printf("\nReturn Key to Exit the Shell\n"); // Print
    getchar();
    if (historycount != 0) {
      FILE *f = fopen("file.txt", "w");       // Open or Create file.txt
      for (int i = 0; i < historycount; i++) { // From 0 to When Exit is Typed
        fprintf(f, "%s\n",
                history[i]); // Print Into file.txt of Each Command Written
      }
      fclose(f); // Close File
      printf("Successful Exit, Check 'file.txt' For Command History"
             "\n\n");
      exit(0); // exit to the original shell
    }

  } else if (pid == 0) { 
  system("ls -l"); 
    // char *myargs[3]; // Define Arg (ls -l)
    // 	myargs[0] = "ls";   // Perform ls
    //     myargs[1] = "-l"; // Perform -l
    //     myargs[2] =  NULL; // Marks End of Array
    //     execvp(myargs[0], myargs); // Execute ls -l
    } 
  return 0;

   }

int moogle_commands(char **args) {
  bool commandfound = false;
  int i;
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < 3; i++) {

    if (strcmp(args[0], moogle_args[i]) == 0) {
      commandfound = true;
      switch (i) {
      case 0:
        // code for exit;
        moogle_tree(args);
        break;
      case 1:
        // code  for
        moogle_list(args);
        break;
      case 2: {
        moogle_exit(args, count);
        exit(0);
        break;
      }
      }
    }
  }
  return execute_command(args);
}
int main() {
  char **command;
  char *input;
  int status;
  moogle_init();
  while (status) {      // keeps running the shell
    printf("Moogle>>"); // Greeting shell during startup
    input = read_line(); // taking the input from the user



    command = Shell_input(input); // parsing the input
    
    status = moogle_commands(command); // run the command function
    free(input);   //  deallocate the memory
    free(command); //  deallocate the memory
  }
  return 0;
}
