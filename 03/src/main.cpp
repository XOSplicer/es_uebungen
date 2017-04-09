/**
 * @author Felix Stegmaier
 */

#include "PreAllocString.h"
#include "debug.h"

int main() {
  CREATE(s_256, 256);
  DEBUG("size: " << s_256.SizeOf());
  DEBUG("length: " << s_256.GetLength());
  s_256.AddWhiteSpace();
  s_256.AddWhiteSpace();
  s_256.AddWhiteSpace();
  DEBUG("length: " << s_256.GetLength());
  s_256.Empty();
  DEBUG("length: " << s_256.GetLength());
  s_256.AddFormat("asd %s ;;", "hallo");
  DEBUG("length: " << s_256.GetLength());
  DEBUG("raw: " << s_256.raw());

/*
  DEBUG("");
  CREATE(s_512, 512);
  DEBUG("size: " << s_512.SizeOf());
  DEBUG("length: " << s_512.GetLength());

  //PreAllocString<128> s_128 = s_512; // type crash

  DEBUG("");
  //copy constructor
  PreAllocString<512> s = s_512;

  s.AddWhiteSpace();
  s.AddWhiteSpace();

  s_512.AddWhiteSpace();

  DEBUG("size: " << s.SizeOf());
  DEBUG("length: " << s.GetLength());

*/
  return 0;
}
