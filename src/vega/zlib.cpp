#include "vega/zlib.h"
#include "vega/vega.h"

#include <zlib.h>

namespace vega {
  std::shared_ptr<std::stringstream> RFC1951::inflate(const std::string& compressed) {
    auto ss = std::make_shared<std::stringstream>();

    const size_t CHUNK = 16384;

    int inflate_code;
    z_stream zs;
    unsigned char output_buffer[CHUNK];

    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = 0;
    zs.next_in = Z_NULL;
    inflate_code = ::inflateInit2(&zs, -15);
    if (inflate_code != Z_OK) {
      throw vega::Exception("Could not initialize inflateInit2(): " + std::to_string(inflate_code));
    }

    zs.avail_in = (uInt)compressed.size();
    zs.next_in  = (Bytef*)compressed.c_str();

    do {
      zs.avail_out = CHUNK;
      zs.next_out = output_buffer;

      inflate_code = ::inflate(&zs, Z_NO_FLUSH);
      if (inflate_code < 0) throw vega::Exception("Encountered inflate() error, code: " + std::to_string(inflate_code));

      // Copy into string
      std::string tmp{(const char*)output_buffer, size_t(CHUNK - zs.avail_out)};
      *ss << tmp;
    }
    while(inflate_code != Z_STREAM_END);

    inflate_code = inflateEnd(&zs);
    if (inflate_code != Z_OK) {
      throw vega::Exception("Could not close inflateEnd(): " + std::to_string(inflate_code));
    }

    ss->seekg(0, ss->beg);
    return ss;
  }

  std::shared_ptr<std::stringstream> RFC1951::deflate(const std::string& uncompressed) {
    auto ss = std::make_shared<std::stringstream>();

    const size_t CHUNK = 16;

    int deflate_code;
    z_stream zs;
    unsigned char output_buffer[CHUNK];

    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = 0;
    zs.next_in = Z_NULL;
    deflate_code = ::deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    if (deflate_code != Z_OK) {
      throw vega::Exception("Could not initialize deflateInit2(): " + std::to_string(deflate_code));
    }

    zs.avail_in = (uInt)uncompressed.size();
    zs.next_in  = (Bytef*)uncompressed.c_str();

    do {
      zs.avail_out = CHUNK;
      zs.next_out = output_buffer;
      deflate_code = ::deflate(&zs, Z_FINISH);
      if (deflate_code < 0) throw vega::Exception("Encountered deflate() error, code: " + std::to_string(deflate_code));

      // Copy into string
      std::string tmp{(const char*)output_buffer, size_t(CHUNK - zs.avail_out)};
      *ss << tmp;
    } while (deflate_code != Z_STREAM_END);

    deflate_code = deflateEnd(&zs);
    if (deflate_code != Z_OK) {
      throw vega::Exception("Could not close deflateEnd(): " + std::to_string(deflate_code));
    }

    ss->seekg(0, ss->beg);
    return ss;
  }
}
