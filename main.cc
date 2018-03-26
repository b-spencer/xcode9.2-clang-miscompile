#include "Thing.hh"

#include <memory>
#include <cstdio>

// This function pattern will crash trying to free the first allocated Thing a
// second time.  Any changes made to this pattern seem to avoid the crash.
std::unique_ptr<Thing>
broken()
{
  std::unique_ptr<Thing> thing;

  try
  {
    // Emulating std::make_unique().  Using reset() avoids the crash.
    thing = std::unique_ptr<Thing>(new Thing);

    // Removing this avoids the crash.
    thing->returns0();

    // Make a second instance and delete the first, in that order.
    thing = std::unique_ptr<Thing>(new Thing);

    // Raise the exception that leads to the crash.
    thing->throws2();
  }
  catch(long) // not the type thrown
  {
    // Without this catch block, the crash is avoided.
  }

  // Without this use, the crash is avoided.
  thing->returns0();

  // This never happens, because throws2() exception propagates instead.  The
  // program crashes while trying to free the memory used for the first Thing
  // instance (which was destroyed before the exception was raised).
  return thing;
}

void
caller()
{
  // We seem to need to throw and catch an exception first.
  try
  {
    throws1();
  }
  catch(int)
  {
    // This exception is caught and ignored.
  }
  
  try
  {
    broken();
  }
  catch(...) // crash happens before catch
  {
    printf("Pass\n");
  }
}

int
main()
{
  caller();
}
