#include <cstdio>

#include "yajl.hpp"

class RefParser : public Yajl::Parser
{
public:
  void null() { _gen.null(); }
  void boolean(bool b) { _gen.boolean(b); }
  void number(const std::string& num) { _gen.number(num); }
  void string(const std::string& str) { _gen.string(str); }
  void map_open() { _gen.map_open();}
  void map_key(const std::string& key) { _gen.string(key); }
  void map_close() {_gen.map_close(); }
  void array_open() {_gen.array_open();}
  void array_close() { _gen.array_close();}

public:
  Yajl::Generator _gen;
};

int main()
{
  char buffer[65536];
  RefParser parser;

  try
    {

      for(;;)
	{
	  size_t rd = std::fread(buffer, 1, sizeof buffer - 1, stdin);
	  if( rd == 0 )
	    break;
	  buffer[rd] = 0;

	  parser.parse(buffer);

	  std::string s = parser._gen.get_incr_output();
	  puts(s.c_str());
	}

      parser.complete_parse();

    }
  catch( Yajl::ParseError e )
    {
      fprintf(stderr, "Error: %s\n", e._errtxt.c_str());
    }
}
