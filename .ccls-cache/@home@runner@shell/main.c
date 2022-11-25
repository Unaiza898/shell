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

int moogle_exit(char **args) {
  // somethign here;

  return 0;
}

int moogle_path(char **args) { return 0; }

int moogle_list(char **args) { return 0; }

// code for tree
int moogle_tree(char **args) { return 0; }

int moogle_nofound(char **args) {

  printf("command not found..");
  return 0;
}

int moogle_commands(char **args) {

  char *pointer = args[0];
  char *second = "exit";
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

      case 1:
        // code  for
        moogle_list(args);

        break;
      case 2:
        // code
        moogle_path(args);

      case 3:
        moogle_exit(args);
      }
    }
  }
  return execute_command(args);
}

int main() {
  char **command;
  char *input;
  int stat_loc;
  while (stat_loc) {
    printf("Moogle>>"); // Greeting shell during startup

    input = read_line();
    command = Shell_input(input);

    // //  deallocate the memory
    stat_loc = moogle_commands(command);
    // moogle_commands(command ) ;
    free(input);
    free(command);
  }
  return 0;
}
