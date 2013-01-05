#include "yajl.hpp"

namespace
{
  int callback_null(void* ctx)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->null();
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_boolean(void* ctx, int b)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->boolean(b);
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_number(void* (ctx), const char* s, size_t l)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->number( std::string(s, l) );
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_string(void* ctx, const unsigned char* s, size_t l)
  {
    try
      {
	const char* ss = reinterpret_cast<const char*>(s);
	(reinterpret_cast<Yajl::Parser*>(ctx))->string( std::string(ss, l) );
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_map_open(void* ctx)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->map_open();
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_map_key(void* ctx, const unsigned char* s, size_t l)
  {
    try
      {
	const char* ss = reinterpret_cast<const char*>(s);
	(reinterpret_cast<Yajl::Parser*>(ctx))->map_key( std::string(ss, l) );
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_map_close(void* ctx)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->map_close();
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_array_open(void* ctx)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->array_open();
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  int callback_array_close(void* ctx)
  {
    try
      {
	(reinterpret_cast<Yajl::Parser*>(ctx))->array_close();
      }
    catch(Yajl::Error e)
      {
	return 0;
      }
    return 1;
  }

  yajl_callbacks callbacks = {
    callback_null,
    callback_boolean,
    NULL,
    NULL,
    callback_number,
    callback_string,
    callback_map_open,
    callback_map_key,
    callback_map_close,
    callback_array_open,
    callback_array_close
  };

}

Yajl::Error::~Error()
{
}

Yajl::Parser::Parser()
{
  _parsehand = yajl_alloc(&callbacks, 0, this);
  yajl_config(_parsehand, yajl_allow_comments, 1);
}

Yajl::Parser::~Parser()
{
  yajl_free(_parsehand);
}

void Yajl::Parser::_checkerror(yajl_status s)
{
  if( s != yajl_status_ok )
    {
      unsigned char* errptr =
	yajl_get_error(_parsehand, 1,
		       reinterpret_cast<const unsigned char*>(_last_input.data()),
		       _last_input.size());
      std::string err( reinterpret_cast<char*>(errptr) );
      yajl_free_error(_parsehand, errptr);
      throw ParseError(s, err);
    }
}

void Yajl::Parser::parse(const std::string& str)
{
  _last_input = str;
  yajl_status s =
    yajl_parse(_parsehand,
	       reinterpret_cast<const unsigned char*>(_last_input.data()),
	       _last_input.size());
  _checkerror(s);
}

void Yajl::Parser::complete_parse()
{
  yajl_status s = yajl_complete_parse(_parsehand);
  _checkerror(s);
}

void Yajl::Parser::null()
{
}

void Yajl::Parser::boolean(bool b)
{
}

void Yajl::Parser::number(const std::string& num)
{
}

void Yajl::Parser::string(const std::string& str)
{
}

void Yajl::Parser::map_open()
{
}

void Yajl::Parser::map_key(const std::string& str)
{
}

void Yajl::Parser::map_close()
{
}

void Yajl::Parser::array_open()
{
}

void Yajl::Parser::array_close()
{
}

Yajl::Generator::Generator()
{
  _gen = yajl_gen_alloc(0);
  //yajl_gen_config(_gen, yajl_gen_beautify, 1);
  //yajl_gen_config(_gen, yajl_gen_validate_utf8, 1);
}

Yajl::Generator::~Generator()
{
  yajl_gen_free(_gen);
}

std::string Yajl::Generator::get_incr_output()
{
  const unsigned char* buf;
  size_t len;
  yajl_gen_get_buf(_gen, &buf, &len);
  std::string retn( reinterpret_cast<const char*>(buf), len );
  yajl_gen_clear(_gen);
  return retn;
}

void Yajl::Generator::null()
{
  yajl_gen_status s = yajl_gen_null(_gen);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::boolean(bool b)
{
  yajl_gen_status s = yajl_gen_bool(_gen, b);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::number(const std::string& num)
{
  yajl_gen_status s = yajl_gen_number(_gen, num.data(), num.size());
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::string(const std::string& str)
{
  yajl_gen_status s = yajl_gen_string( _gen,
				       reinterpret_cast<const unsigned char*>
				       (str.data()), str.size());
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::map_open()
{
  yajl_gen_status s = yajl_gen_map_open(_gen);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::map_close()
{
  yajl_gen_status s = yajl_gen_map_close(_gen);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::array_open()
{
  yajl_gen_status s = yajl_gen_array_open(_gen);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::array_close()
{
  yajl_gen_status s = yajl_gen_array_close(_gen);
  if (s != yajl_gen_status_ok)
    throw GeneratorError(s);
}

void Yajl::Generator::set_beautify(bool on) {
  yajl_gen_config(_gen, yajl_gen_beautify, on);
}
void Yajl::Generator::set_validate_utf8(bool on) {
  yajl_gen_config(_gen, yajl_gen_validate_utf8, on);
}
void Yajl::Generator::set_indent_string(const std::string& s) {
  yajl_gen_config(_gen, yajl_gen_indent_string, s.c_str());
}
void Yajl::Generator::set_escape_solidus(bool on) {
  yajl_gen_config(_gen, yajl_gen_escape_solidus, on);
}
