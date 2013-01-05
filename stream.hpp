#ifndef STREAM_HPP
#define STREAM_HPP

#include <string>
#include <cstdio>

class Stream
{
public:
  // open an existing file (standard fopen modes)
  Stream(const std::string& filename, const std::string& mode);
  // use an existing file
  Stream(FILE* f);
  // closes the file
  ~Stream();

  // return text up to a certain number of characters
  // if this is less than requested, this means no more is available
  std::string read_chars(size_t numchars);
  // write the characters given
  void write_chars(const std::string& s);

protected:
  FILE* _f;
};

// thrown if there is an error from the stream class
class StreamException
{
public:
  StreamException(const std::string& errortxt)
    : error(errortxt)
  {
  }
  const std::string error;
};

#endif
