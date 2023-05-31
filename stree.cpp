#define _XOPEN_SOURCE 600
#include "./sources/Tree.hpp"
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <ftw.h>
#include <ctype.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions, plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>    /* Prototypes for many system calls */
#include <errno.h>     /* Declares errno and defines error constants */
#include <string.h>    /* Commonly used string-handling functions */
#define RESET "\033[0m"
#define BLACK "\033[30m"            /* Black */
#define RED "\033[31m"              /* Red */
#define GREEN "\033[32m"            /* Green */
#define YELLOW "\033[1;33m"         /* Yellow */
#define BLUE "\033[34m"             /* Blue */
#define MAGENTA "\033[35m"          /* Magenta */
#define CYAN "\033[36m"             /* Cyan */
#define WHITE "\033[37m"            /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */

using namespace std;
using namespace ariel;
Tree tree;
std::string *extractLastTwoNames(const std::string &path)
{
  std::istringstream iss(path);
  std::string *lastTwoNames = new std::string[2];
  std::string name;
  int count = 0;
  std::getline(iss, name, '/');
  lastTwoNames[0] = name;
  while (std::getline(iss, name, '/'))
  {
    if(!name.empty() && name[0]=='.')
    return nullptr;
    lastTwoNames[1] = lastTwoNames[0];
    lastTwoNames[0]=name;
  }

  return lastTwoNames;
}
static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
  // Initialize a buffer to store the permitions, name, level, user, group,size
  char permitions[256];
  char name[256];
  int level;
  char user[256];
  char group[256];
  char size[256];
  memset(permitions, 0, sizeof(permitions));
  memset(name, 0, sizeof(name));
  memset(user, 0, sizeof(user));
  memset(group, 0, sizeof(group));
  memset(size, 0, sizeof(size));

  if (type == FTW_NS)
  {
    sprintf(permitions, "?");
  }
  else
  {
    switch (sbuf->st_mode & S_IFMT)
    {
    case S_IFREG:
      sprintf(permitions, "-");
      break;
    case S_IFDIR:
      sprintf(permitions, "d");
      break;
    case S_IFCHR:
      sprintf(permitions, "c");
      break;
    case S_IFBLK:
      sprintf(permitions, "b");
      break;
    case S_IFLNK:
      sprintf(permitions, "l");
      break;
    case S_IFIFO:
      sprintf(permitions, "p");
      break;
    case S_IFSOCK:
      sprintf(permitions, "s");
      break;
    default:
      sprintf(permitions, "?");
      break;
    }
  }

  if (type != FTW_NS)
  {
    sprintf(permitions + strlen(permitions), "%s%s%s", sbuf->st_mode & S_IRUSR ? "r" : "-", sbuf->st_mode & S_IWUSR ? "w" : "-", sbuf->st_mode & S_IXUSR ? "x" : "-");
    sprintf(permitions + strlen(permitions), "%s%s%s", sbuf->st_mode & S_IRGRP ? "r" : "-", sbuf->st_mode & S_IWGRP ? "w" : "-", sbuf->st_mode & S_IXGRP ? "x" : "-");
    sprintf(permitions + strlen(permitions), "%s%s%s", sbuf->st_mode & S_IROTH ? "r" : "-", sbuf->st_mode & S_IWOTH ? "w" : "-", sbuf->st_mode & S_IXOTH ? "x" : "-");

    struct passwd *_user = getpwuid(sbuf->st_uid);
    struct group *_group = getgrgid(sbuf->st_gid);

    if (_user != NULL)
      sprintf(user, "%s", _user->pw_name);
    else
      sprintf(user, "%d", sbuf->st_uid);

    if (_group != NULL)
      sprintf(group, "%s", _group->gr_name);
    else
      sprintf(group, "%d", sbuf->st_gid);

    sprintf(size, "%ld", (long)sbuf->st_size);
  }

  level = ftwb->level;
  sprintf(name, "%s\n", &pathname[ftwb->base]);
  std::string *result = extractLastTwoNames(pathname);
  if (result != nullptr)
  {
    tree.add(result[0], level, permitions, user, group, size, result[1]);
    delete[] result;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  int flags = 0;

  if (nftw((argc != 2) ? "." : argv[1], dirTree, 10, flags) == -1)
  {
    perror("nftw");
    exit(EXIT_FAILURE);
  }
  cout << tree << endl;
  exit(EXIT_SUCCESS);
}