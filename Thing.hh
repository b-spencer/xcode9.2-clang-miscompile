// Throws int 1.
void throws1();

struct Thing
{
  Thing();

  ~Thing();

  // Returning bool instead of int here avoids the bug.
  int returns0();

  // Throws int 2.  Removing this indirection avoids the bug.
  void throws2()
  { prv_throws2(); }
  void prv_throws2();

  // Fewer data members avoids the bug.
  int a;
  int b;
};


