#ifndef BYTESTREAM_READER_H
#define BYTESTREAM_READER_H

#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

class BytestreamReader {
public:
  using data_type = unsigned char;
  BytestreamReader(const std::vector<data_type>&);
  size_t pos() const;
  size_t left() const;
  bool empty() const;
  bool end() const;
  bool seek(const size_t);
  bool read_str(std::string&, const size_t);
  bool read_u8(uint8_t&);
  bool read_u16(uint16_t&);
  bool read_u32(uint32_t&);
  bool read_bytes(data_type out[], const size_t);
private:
  template <
    typename T,
    std::enable_if_t<std::is_trivially_copyable<T>::value, int> = 0
  >
  bool read_num(T&);

  const std::vector<data_type>* m_data;
  size_t m_cursor;
};

// Reads a sequence of char as an integer type, assuming that chars are stored
// in little-endian byte-order and the system is little-endian as well
template <
  typename T,
  std::enable_if_t<std::is_trivially_copyable<T>::value, int>
>
bool BytestreamReader::read_num(T& out) {
  constexpr auto size = sizeof(T);

  if (size == 0 || m_cursor + size > m_data->size()) {
    return false;
  }

  std::memcpy(&out, m_data->data() + m_cursor, size);

  m_cursor += size;

  return true;
}

#endif
