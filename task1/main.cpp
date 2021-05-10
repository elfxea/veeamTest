// C++ 14 used.

#include <string>
#include <fstream>
#include <sys/stat.h>

// Import PugiXML
#include "/pugixml/pugiconfig.hpp"
#include "/pugixml/pugixml.hpp"
#include "/pugixml/pugixml.cpp"

// Config file location
#define CFG_LOCATION "config.xml"

using std::ios;

int main() {
    pugi::xml_document document;
    pugi::xml_parse_result file_loader = document.load_file(CFG_LOCATION);

    // If something went wrong while loading config
    if (file_loader.status != pugi::status_ok)
        throw std::runtime_error("Config file can't be reached.\n");

    // Getting root of config file
    pugi::xml_node config = document.child("config");

    for (pugi::xml_node file : config.children()) {
        // Reading file_i attributes
        std::string file_name = file.attribute("file_name").as_string();
        std::string source_path = file.attribute("source_path").as_string();
        std::string destination_path = file.attribute("destination_path").as_string();

        // Trying to create destination directory
        int status = mkdir(destination_path.c_str());

        // If status is not OK and directory does not exist
        if ((status < 0) && (errno != EEXIST))
            throw std::runtime_error("Unable to create destination path.\n");

        // Add file name to source/dest paths to open it with fstream
        source_path += file_name;
        destination_path += file_name;

        std::fstream source_file(source_path, ios::in | ios::binary);
        std::fstream dest_file(destination_path, ios::out | ios::binary);

        // if source/dest files are still unreachable
        if (!source_file.is_open() || !dest_file.is_open())
            throw std::runtime_error("Unable to open source/destination file.");

        // tmp_byte is necessary to avoid '\377' symbol at the end of dest file
        char tmp_byte = source_file.get();
        while (!source_file.eof()) {
            dest_file.put(tmp_byte);
            tmp_byte = source_file.get();
        }
    }


    return 0;
}
