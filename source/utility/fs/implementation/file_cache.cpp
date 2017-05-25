#include "file_cache.h"

#include "fast_text_writer.h"

#include "utility/convertion/convert.h"
#include "utility/fs/files.h"


using utility::convertion::convert;
using std::string;
using std::vector;
using std::remove;
using std::make_shared;


namespace {


string const MARKER_REMOVED_INDEX    = "#ri#";
string const MARKER_CACHED_LINE      = "#cl#";


bool checkMarker(string const &line, string const &marker) {
    return line.size() > marker.size() && line.substr(0, marker.size()) == marker; // ----->
}


} // unnamed


namespace utility {
namespace fs {
namespace implementation {


CFileCache::CFileCache(std::string const &file_name)
:
    m_file_name     (file_name),
    m_text_writer   (CFastTextWriter::create(file_name))
{}


void CFileCache::initialize() {
    if (!m_lines.empty())
        return; // ----->

    vector<size_t>  indexes_for_remove;
    vector<string>  items;

    if (fs::checkFileExistence(m_file_name)) {
        vector<string> text_file = fs::readTextFile(m_file_name, false);

        string item;
        for (auto const &line : text_file) {
            if (line.empty())
                continue; // <---

            if (checkMarker(line, MARKER_CACHED_LINE)) {
                if (!item.empty())
                    items.push_back(item);
                item = line.substr(MARKER_CACHED_LINE.size(), string::npos);
                continue; // <---
            }

            if (checkMarker(line, MARKER_REMOVED_INDEX)) {
                if (!item.empty()) {
                    items.push_back(item);
                    item.clear();
                }
                indexes_for_remove.push_back(convert<size_t>(line.substr(MARKER_REMOVED_INDEX.size(), string::npos)));
                continue; // <---
            }

            if (item.empty())
                throw std::runtime_error("parsing cache file '" + m_file_name + "' error: line '" + line + "', expects tag"); // ----->

            item += "\n" + line;
        }
        if (!item.empty())
            items.push_back(item);

        fs::remove(m_file_name);
    }

    m_text_writer->initialize();

    // todo: move file_name
    m_lines.clear();
    for (size_t i = 0; i < items.size(); i++) {
        bool is_removed_index = false;
        for (auto const &ii : indexes_for_remove) {
            if (i == ii) {
                is_removed_index = true;
                break; // --->
            }
        }
        if (!is_removed_index)
            push(items[i]);
    }
    m_text_writer->flush();
}


void CFileCache::finalize() {
    m_text_writer->finalize();
    m_lines.clear();
}


size_t CFileCache::push(string const &line) {
    m_text_writer->writeLine(MARKER_CACHED_LINE + line);
    m_lines.push_back(make_shared<string>(line));
    return m_lines.size();
}


string CFileCache::get(size_t const &id) const {
    if (id < m_lines.size())
        if (m_lines[id])
            return *m_lines[id]; // ----->
        else
            throw std::runtime_error("file cache error: line id " + convert<string>(id) + " is empty"); // ----->
    else
        throw std::runtime_error("file cache error: line id " + convert<string>(id) + ", cache size " + convert<string>(m_lines.size())); // ----->
}


void CFileCache::remove(size_t const &id) {
    if (id < m_lines.size()) {
        if (m_lines[id]) {
            m_text_writer->writeLine(MARKER_REMOVED_INDEX + convert<string>(id));
            m_lines[id].reset();
        }
    } else
        throw std::runtime_error("file cache error: line id " + convert<string>(id) +", cache size " + convert<string>(m_lines.size())); // ----->
}


void CFileCache::clean() {
    m_lines.clear();
    m_text_writer->finalize();
    fs::remove(m_file_name);
}


size_t CFileCache::size() const {
    return m_lines.size(); // ----->
}


bool CFileCache::checkExistense(size_t const &id) const {
    return id < m_lines.size() && m_lines[id]; // ----->
}


void CFileCache::flush() {
    m_text_writer->flush();
}


} // implementation
} // fs
} // utility
