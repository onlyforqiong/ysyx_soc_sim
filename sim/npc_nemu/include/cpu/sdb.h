#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  word_t last_value;
  char str[1000];

  /* TODO: Add more members if necessary */

} WP;
extern WP *head;
extern WP *free_ ;

word_t expr(char *e, bool *success);
void sdb_mainloop();
WP* new_wp();
bool delete_wp(int n);
extern bool stop_point_set ;
extern size_t stop_point_pc ;
#endif
