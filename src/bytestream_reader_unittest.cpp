#include <string>
#include <vector>

#include "../extern/catch/catch2.hpp"

#include "bytestream_reader.h"

TEST_CASE("empty") {
  const std::vector<BytestreamReader::data_type> buffer = {};

  BytestreamReader reader(buffer);

  REQUIRE(reader.empty() == true);
  REQUIRE(reader.end() == true);
  REQUIRE(reader.pos() == 0);
  REQUIRE(reader.left() == 0);

  SECTION("read string") {
    std::string out("foo");
    REQUIRE(reader.read_str(out, 4) == false);
    REQUIRE(reader.pos() == 0);
    REQUIRE(reader.left() == 0);
    REQUIRE(reader.end() == true);
    REQUIRE(out == "foo");
  }
}

TEST_CASE("filled with string") {
  const std::vector<BytestreamReader::data_type> buffer = {
    0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72, 0x62, 0x61, 0x7a // "foobarbaz"
  };

  BytestreamReader reader(buffer);

  REQUIRE(reader.empty() == false);
  REQUIRE(reader.end() == false);
  REQUIRE(reader.pos() == 0);

  SECTION("read full") {
    std::string out("tmp");

    REQUIRE(reader.read_str(out, 20) == false);
    REQUIRE(out == "tmp");
    REQUIRE(reader.pos() == 0);
    REQUIRE(reader.left() == 9);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_str(out, 9) == true);
    REQUIRE(out == "foobarbaz");
    REQUIRE(reader.pos() == 9);
    REQUIRE(reader.left() == 0);
    REQUIRE(reader.end() == true);
  }

  SECTION("read in chunks") {
    std::string out1("tmp");
    std::string out2("tmp");
    std::string out3("tmp");

    REQUIRE(reader.read_str(out1, 3) == true);
    REQUIRE(out1 == "foo");
    REQUIRE(reader.pos() == 3);
    REQUIRE(reader.left() == 6);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_str(out2, 0) == false);
    REQUIRE(out2 == "tmp");
    REQUIRE(reader.pos() == 3);
    REQUIRE(reader.left() == 6);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_str(out2, 5) == true);
    REQUIRE(out2 == "barba");
    REQUIRE(reader.pos() == 8);
    REQUIRE(reader.left() == 1);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_str(out3, 3) == false);
    REQUIRE(out3 == "tmp");
    REQUIRE(reader.pos() == 8);
    REQUIRE(reader.left() == 1);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_str(out3, 1) == true);
    REQUIRE(out3 == "z");
    REQUIRE(reader.pos() == 9);
    REQUIRE(reader.left() == 0);
    REQUIRE(reader.end() == true);
  }
}

TEST_CASE("numbers little-endian") {
  const std::vector<BytestreamReader::data_type> buffer = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 // "123456789"
  };

  BytestreamReader reader(buffer);

  REQUIRE(reader.empty() == false);
  REQUIRE(reader.end() == false);
  REQUIRE(reader.pos() == 0);

  SECTION("read uint8_t") {
    uint8_t out1 = 0, out2 = 0, out3 = 0;

    REQUIRE(reader.read_u8(out1) == true);
    REQUIRE(out1 == 1);
    REQUIRE(reader.pos() == 1);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u8(out2) == true);
    REQUIRE(out2 == 2);
    REQUIRE(reader.pos() == 2);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u8(out3) == true);
    REQUIRE(out3 == 3);
    REQUIRE(reader.pos() == 3);
    REQUIRE(reader.end() == false);
  }

  SECTION("read uint16_t") {
    uint16_t out1 = 0, out2 = 0, out3 = 0;

    REQUIRE(reader.read_u16(out1) == true);
    REQUIRE(out1 == 513);
    REQUIRE(reader.pos() == 2);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u16(out2) == true);
    REQUIRE(out2 == 1027);
    REQUIRE(reader.pos() == 4);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u16(out3) == true);
    REQUIRE(out3 == 1541);
    REQUIRE(reader.pos() == 6);
    REQUIRE(reader.end() == false);
  }

  SECTION("read uint32_t") {
    uint32_t out1 = 0, out2 = 0, out3 = 0;

    REQUIRE(reader.read_u32(out1) == true);
    REQUIRE(out1 == 67305985);
    REQUIRE(reader.pos() == 4);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u32(out2) == true);
    REQUIRE(out2 == 134678021);
    REQUIRE(reader.pos() == 8);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.read_u32(out3) == false);
    REQUIRE(out3 == 0);
    REQUIRE(reader.pos() == 8);
    REQUIRE(reader.end() == false);
  }
}

TEST_CASE("seeking") {
  const std::vector<BytestreamReader::data_type> buffer = {
    0x66, 0x6f, 0x6f, 0x62, 0x61, 0x72 // "foobar"
  };

  BytestreamReader reader(buffer);

  REQUIRE(reader.pos() == 0);
  REQUIRE(reader.left() == 6);
  REQUIRE(reader.end() == false);

  SECTION("seek past end") {
    REQUIRE(reader.seek(10) == false);
    REQUIRE(reader.pos() == 0);
    REQUIRE(reader.left() == 6);
    REQUIRE(reader.end() == false);
  }

  SECTION("seek normal") {
    REQUIRE(reader.seek(3) == true);
    REQUIRE(reader.pos() == 3);
    REQUIRE(reader.left() == 3);
    REQUIRE(reader.end() == false);

    REQUIRE(reader.seek(5) == true);
    REQUIRE(reader.pos() == 5);
    REQUIRE(reader.left() == 1);
    REQUIRE(reader.end() == false);
  }
}

TEST_CASE("bytes") {
  const std::vector<BytestreamReader::data_type> buffer = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 // "123456789"
  };

  BytestreamReader reader(buffer);

  REQUIRE(reader.pos() == 0);

  SECTION("outside range") {
    auto bytes = std::make_unique<unsigned char[]>(5);

    REQUIRE(reader.read_bytes(bytes.get(), 20) == false);
    REQUIRE(reader.pos() == 0);
  }

  SECTION("inside range") {
    auto bytes = std::make_unique<unsigned char[]>(5);

    REQUIRE(reader.read_bytes(bytes.get(), 5) == true);
    REQUIRE(reader.pos() == 5);
    REQUIRE(bytes[0] == 1);
    REQUIRE(bytes[1] == 2);
    REQUIRE(bytes[2] == 3);
    REQUIRE(bytes[3] == 4);
    REQUIRE(bytes[4] == 5);
  }
}
