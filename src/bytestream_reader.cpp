#include "bytestream_reader.h"

BytestreamReader::BytestreamReader(const std::vector<data_type>& data)
  : m_cursor(0)
{
  m_data = &data;
}

size_t BytestreamReader::pos() const {
  return m_cursor;
}

size_t BytestreamReader::left() const {
  return m_data->size() - m_cursor;
}

bool BytestreamReader::empty() const {
  return m_data->empty();
}

bool BytestreamReader::end() const {
  if (empty()) {
    return true;
  }
  return m_cursor >= m_data->size();
}

bool BytestreamReader::seek(const size_t cursor) {
  if (cursor > m_data->size()) {
    return false;
  }

  m_cursor = cursor;

  return true;
}

bool BytestreamReader::read_str(std::string& out, const size_t size) {
  if (size == 0 || m_cursor + size > m_data->size()) {
    return false;
  }

  auto begin = m_data->begin() + m_cursor;
  auto end = begin + size;

  out.assign(begin, end);

  m_cursor += size;

  return true;
}

bool BytestreamReader::read_u8(uint8_t& out) {
  return read_num(out);
}

bool BytestreamReader::read_u16(uint16_t& out) {
  return read_num(out);
}

bool BytestreamReader::read_u32(uint32_t& out) {
  return read_num(out);
}

bool BytestreamReader::read_bytes(data_type out[], const size_t size) {
  if (size == 0 || m_cursor + size > m_data->size()) {
    return false;
  }

  std::memcpy(out, m_data->data() + m_cursor, size);

  m_cursor += size;

  return true;
}
