// https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/
#define BUFFER_LEN 1024
#define MOOGLETSL " \t\r\n\a"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

char *moogle_args[] = {"tree", "list", "path", "exit"};
// char *history[BUFFER_LEN] ;
// malloc(8 * sizeof(char *));
// void inithistory(){

// }
// for(int i = 0 ; i <100 ; i++){

//   *history[i] =  malloc(100 * sizeof(char *));
// }

char history[4][BUFFER_LEN];
char a[2][14];
int count = 0;
int historycount = 0;

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

// this function would take the input and put it in array and retun the pointer
char **Shell_input(char *input) {
  // printf("something");
  // allocates the requested memory and returns a pointer

  char **commands = malloc(8 * sizeof(char *));
  if (commands == NULL) {
    perror("malloc failed");
    exit(1);
  }
  char *space = " "; // space
  char *parsed_text;
  int index = 0;

  parsed_text = strtok(input, MOOGLETSL); // split the string by character space
  stpcpy(history[historycount], parsed_text);
  historycount++;
  //  history[historycount] = parsed_text;
  // printf("%d: %s\n", historycount, history[historycount]);
  // historycount++;

  // for (int i = 0; i < historycount; i++) {
  //   printf("%d: %s\n", i, history[i]); //
  // }

  while (parsed_text != NULL) {
    commands[index] = parsed_text;
    index++;
    parsed_text = strtok(NULL, space);
  }

  commands[index] = NULL;
  return commands;
}

int execute_command(char **command) {
  pid_t child;
  int stat_loc;
  child = fork();

  if (child < 0) {
    perror("Fork failed");
    exit(1);
  } else if (child == 0) {

    execvp(command[0], command);

  } else {
    waitpid(child, &stat_loc, WUNTRACED);
  }

  return 1;
}

int moogle_exit(char **args, int count) {
  printf("Previously Executed Commands: \n");

  int c = count;
  if (c < 4) {
    c = 0;
  } else {
    c -= 4;
  }
  for (int i = c; i < count; i++) {
    printf("%d: %s\n", i + 1, args[i]); // Prints
  }
  // printf("%d: %s\n", historycount, history[0]);
  // printf("%d: %s\n", historycount, history[1]);

  int pid = fork();
  if (pid < 0) { // Test For Fork Failed
    fprintf(stderr, "Fork Failed");
    exit(0);
  } else if (pid > 0) {
    // Parent
    wait(NULL);                                         // Wait For Child
    printf("\nHit the Return Key to Exit the Shell\n"); // Print
    getchar();
    FILE *f = fopen("file.txt", "wb"); // Open or Create file.txt
    for (int i = 0; i < count; i++) {  // From 0 to When Exit is Typed
      fprintf(f, "%s\n",
              args[i]); // Print Into file.txt of Each Command Written
    }
    fclose(f); // Close File
    printf("You've Successfully Exited, Check 'file.txt' For Command History:"
           "\n\n");
  } else if (pid == 0) {
    // Child
    char *myargs[3];                 // Define Arg (ls -l)
    myargs[0] = "ls";                // Perform ls
    myargs[1] = "-l";                // Perform -l
    myargs[2] = NULL;                // Marks End of Array
    execvp(myargs[0], myargs);       // Execute ls -l
    printf("This Shouldn't Run \n"); // In Case of Failure
  }
  return 0;
}

int moogle_path(char **args) { return 0; }

int moogle_list(char **args) {
  // clear screen
  system("clear");
  // list file
  system("ls -l");
  // output the list to t1.txt
  system("ls -l . > t1.txt");
  // rename it to tree
  system("mv t1.txt tree.txt");

  return 0;
}

// code for tree
int moogle_tree(char **args) { return 0; }

int moogle_commands(char **args) {

  // char *pointer = args[0];
  // char *second = "exit";
  bool commandfound = false;
  int i;
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < 4; i++) {

    if (strcmp(args[0], moogle_args[i]) == 0) {
      commandfound = true;
      switch (i) {
      case 0:
        // code for exit;
        moogle_tree(args);

      case 1:
        // code  for
        moogle_list(args);

        break;
      case 2: {
        moogle_path(args);

        break;
      }

      case 3: {
        for (int i = 0; i < historycount; i++) {
          printf("%d: %s\n", i + 1, history[i]); //
        }

        moogle_exit(args, count);
        break;
      }
      }
    }
  }
  return execute_command(args);
}
void moogle_init() {

  printf("******** **    ** *****  ****   ****  **    **    ******  "
         "*********\n");
  printf("******** **    ** ** ** **     **     **    **    **      "
         "*********\n");
  printf("         ** ** ** ** ** ** *** ** *** **    **    ******           "
         "\n");
  printf("******** **    ** ** ** **  ** **  ** **    **    **      "
         "*********\n");
  printf("******** **    ** *****   ***    ***  ***** ***** ******  "
         "*********\n");
}
int main() {
  char **command;
  char *input;
  int stat_loc;
  // history = malloc(8 * sizeof(char *));
  moogle_init();
  while (stat_loc) {
    printf("Moogle>>"); // Greeting shell during startup

    input = read_line();
    command = Shell_input(input);
    // copy the command used in the history
    //  strcpy(history[count++], command);

    // //  deallocate the memory
    stat_loc = moogle_commands(command);
    // moogle_commands(command ) ;
    free(input);
    free(command);
  }
  return 0;
}
