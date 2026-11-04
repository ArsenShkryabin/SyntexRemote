//
// Aspia Project
// Copyright (C) 2016-2022 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include "base/files/file_util.h"

#include <fstream>

namespace base {

namespace {

template <class Container>
bool readFileT(const std::filesystem::path& filename, Container* buffer)
{
    std::ifstream stream;
    stream.open(filename, std::ifstream::binary | std::ifstream::in);
    if (!stream.is_open())
        return false;

    buffer->clear();

    stream.seekg(0, stream.end);
    auto tell_pos = stream.tellg();
    stream.seekg(0);

    if (tell_pos > 0 && tell_pos != static_cast<std::streampos>(-1))
    {
        size_t size = static_cast<size_t>(tell_pos);
        buffer->resize(size);
        stream.read(reinterpret_cast<char*>(buffer->data()), size);
        return !stream.fail();
    }

    // Fallback for /proc, /sys and virtual files where seekg tells -1 or 0
    char chunk[4096];
    while (stream.read(chunk, sizeof(chunk)))
    {
        buffer->insert(buffer->end(), chunk, chunk + stream.gcount());
    }
    if (stream.gcount() > 0)
    {
        buffer->insert(buffer->end(), chunk, chunk + stream.gcount());
    }

    return true;
}

} // namespace

bool writeFile(const std::filesystem::path& filename, const void* data, size_t size)
{
    std::ofstream stream;
    stream.open(filename, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
    if (!stream.is_open())
        return false;

    stream.seekp(0);
    stream.write(reinterpret_cast<const char*>(data), size);

    return !stream.fail();
}

bool writeFile(const std::filesystem::path& filename, const ByteArray& buffer)
{
    return writeFile(filename, buffer.data(), buffer.size());
}

bool writeFile(const std::filesystem::path& filename, std::string_view buffer)
{
    return writeFile(filename, buffer.data(), buffer.size());
}

bool readFile(const std::filesystem::path& filename, ByteArray* buffer)
{
    return readFileT(filename, buffer);
}

bool readFile(const std::filesystem::path& filename, std::string* buffer)
{
    return readFileT(filename, buffer);
}

} // namespace base
