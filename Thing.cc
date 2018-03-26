#include "Thing.hh"
#include <cstdio>

void
throws1()
{
  throw 1;
}

Thing::Thing()
  : a(0), b(0)
{
  printf("construct Thing=%p\n", this);
}

Thing::~Thing()
{
  printf("destroy   Thing=%p\n", this);
}

int
Thing::returns0()
{
  return 0;
}

void
Thing::prv_throws2()
{
  throw 2;
}
