
#include <QFile>

#include "PEFile.h"

PEFile::PEFile(QObject *_parent)
    : QObject(_parent),
    data_directory_count_(0),
    section_count_(0),
    image_dos_header_(NULL),
    image_nt_headers_(NULL),
    image_file_header_(NULL),
    image_optional_header_(NULL)
{

}

PEFile::~PEFile(void)
{

}

void PEFile::read_the_pe_file(QString _file_name)
{
    QFile *_file = new QFile(_file_name);
    if (!_file->open(QFile::ReadOnly)) return;

    image_dos_header_ = new IMAGE_DOS_HEADER();
    image_nt_headers_ = new IMAGE_NT_HEADERS();
    image_file_header_ = new IMAGE_FILE_HEADER();
    image_optional_header_ = new IMAGE_OPTIONAL_HEADER();

    /////////////////////////// DOS HEADER //////////////////////////////
    // read the dos header
    int _cur_rva_offset = 0;
    int _cur_file_offset = 0;
    _file->seek(_cur_rva_offset);
    _file->read((char *)image_dos_header_, sizeof(IMAGE_DOS_HEADER));

    // check the file is dos file
    if (image_dos_header_->e_magic != IMAGE_DOS_SIGNATURE) return;
    
    /////////////////////////// NT HEADER //////////////////////////////
    // nt header include file header and optional header
    // optional header include sixteen data directories
    // read the nt header
    _cur_rva_offset += image_dos_header_->e_lfanew;
    _file->seek(_cur_rva_offset);
    _file->read((char *)image_nt_headers_, sizeof(IMAGE_NT_HEADERS));

    // check the file is pe file
    if (image_nt_headers_->Signature != IMAGE_NT_SIGNATURE) return;

    // read the file header
    _cur_rva_offset += 4;
    _file->seek(_cur_rva_offset);
    _file->read((char *)image_file_header_, sizeof(IMAGE_FILE_HEADER));

    // read the optional header
    _cur_rva_offset += sizeof(IMAGE_FILE_HEADER);
    _file->seek(_cur_rva_offset);
    _file->read((char *)image_optional_header_, sizeof(IMAGE_OPTIONAL_HEADER));
    
    // read the data directories
    data_directory_count_ = image_optional_header_->NumberOfRvaAndSizes;
    if (data_directory_count_ > 16) return;
    for (int i = 0; i != data_directory_count_; ++i)
    {
        IMAGE_DATA_DIRECTORY *_image_data_directory =
            &image_optional_header_->DataDirectory[i];
        image_data_directory_.push_back(_image_data_directory);
    }
    _cur_rva_offset += sizeof(IMAGE_OPTIONAL_HEADER);

    // read the section
    section_count_ = image_file_header_->NumberOfSections;
    _cur_rva_offset = 0;
    _cur_rva_offset += image_dos_header_->e_lfanew;
    _cur_rva_offset += 4;
    _cur_rva_offset += sizeof(IMAGE_FILE_HEADER);
    _cur_rva_offset += image_file_header_->SizeOfOptionalHeader;
    for (int i = 0; i != section_count_; ++i)
    {
        IMAGE_SECTION_HEADER *_image_section_header = new IMAGE_SECTION_HEADER();
        _file->read((char *)_image_section_header, sizeof(IMAGE_SECTION_HEADER));
        image_section_header_.push_back(_image_section_header);

        _cur_rva_offset += sizeof(IMAGE_SECTION_HEADER);
        _file->seek(_cur_rva_offset);
    }
    
    _file->close();
}