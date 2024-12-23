#include "cjparse.h"

#pragma once
#ifndef CJPARSE_PRINT_H
#define CJPARSE_PRINT_H

class cjparse_json_print
{
  public:
    cjparse_json_print (cjparse::cjparse_json_value &JSON);

  public:
    void print_json_from_memory (cjparse::cjparse_json_value &JSON);
    void print_json_number (cjparse::json_number &num);

  private:
    int n_of_iteration = 0;
};

#endif
