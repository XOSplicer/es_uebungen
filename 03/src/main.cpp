/**
 * @author Felix Stegmaier
 */

#include "PreAllocString.h"
#include "debug.h"

int main() {
  CREATE(s_256, 256);
  DEBUG("size: " << s_256.SizeOf());
  DEBUG("length: " << s_256.GetLength());

  CREATE(s_512, 512);
  DEBUG("size: " << s_512.SizeOf());
  DEBUG("length: " << s_512.GetLength());

  //PreAllocString<128> s_128 = s_512; /* type crash */

  //copy constructor
  PreAllocString<512> s = s_512;

  DEBUG("size: " << s.SizeOf());
  DEBUG("length: " << s.GetLength());


  return 0;
}
