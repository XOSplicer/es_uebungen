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
  DEBUG("const char* cast: " << (const char*)s_256);
  DEBUG("indexing: " << s_256[2]);

  DEBUG("create");
  CREATE(s_128, 128);
  DEBUG("assign char");
  s_128 = 'a';
  DEBUG("const char* cast: " << (const char*)s_128);
  DEBUG("assign string");
  s_128 = "asdf";
  DEBUG("const char* cast: " << (const char*)s_128);
  DEBUG("add char");
  s_128 += 'g';
  DEBUG("const char* cast: " << (const char*)s_128);
  DEBUG("add string");
  s_128 += "hjkl";
  DEBUG("const char* cast: " << (const char*)s_128);
  DEBUG("const void* cast: " << (const void*) s_128);

  CREATE(s_64, 64);
  /* probably uses assignment with (const char*)s_128 as rhs */
  s_64 = s_128;
  s_64.AddFormat("%c", 'X');
  DEBUG("const char* cast: " << (const char*)s_64);

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
