#ifndef GEN_TEMPS_H
#define GEN_TEMPS_H

char* gen_new_temp();
int is_temporary(const char* addr);
void gen_release_temp(char *temp_name);
void free_temps();

#endif