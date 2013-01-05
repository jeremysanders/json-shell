#include <cstring>
#include <cerrno>
#include <vector>
#include "stream.hpp"

namespace
{
  void _raiseerror(const std::string& prefix)
  {
    const std::string msg = prefix + ": " + strerror(errno);
    throw StreamException(msg);
  }
}

Stream::Stream(const std::string& filename, const std::string& mode)
{
  _f = fopen(filename.c_str(), mode.c_str());
  if(_f == 0)
    _raiseerror("open");
}

Stream::Stream(FILE* f)
{
  _f = f;
}

Stream::~Stream()
{
  const int s = fclose(_f);
  if( s != 0 )
    _raiseerror("close");
}

std::string Stream::read_chars(size_t numchars)
{
  std::vector<char> buffer(numchars);

  const size_t r = fread(&buffer[0], sizeof(char), numchars, _f);
  if( r != numchars && ferror(_f) )
    _raiseerror("read");

  return std::string(&buffer[0], r);
}

void Stream::write_chars(const std::string& s)
{
  const size_t w = fwrite(s.data(), sizeof(char), s.size(), _f);
  if( w != s.size() )
    _raiseerror("write");
}

int main()
{
  try
    {
      Stream fin(stdin);
      Stream fout(stdout);

      for(;;)
	{
	  std::string s( fin.read_chars(2) );
	  if( s.empty() )
	    break;
	  fout.write_chars( s + "x" );
	}
    }
  catch( StreamException e )
    {
      fprintf(stderr, "%s\n", e.error.c_str());
    }
}
