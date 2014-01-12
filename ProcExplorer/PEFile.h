#ifndef PEFILE_H_
#define PEFILE_H_

#include <Windows.h>

#include <QObject>

class PEFile : public QObject
{
    Q_OBJECT

 public:
    PEFile(QObject *_parent = NULL);
    ~PEFile(void);
    void read_the_pe_file(QString _file_name);
 private:
    int data_directory_count_;
    int section_count_;
    IMAGE_DOS_HEADER *image_dos_header_;
    IMAGE_NT_HEADERS *image_nt_headers_;
    IMAGE_FILE_HEADER *image_file_header_;
    IMAGE_OPTIONAL_HEADER *image_optional_header_;
    QList<IMAGE_DATA_DIRECTORY *> image_data_directory_;
    QList<IMAGE_SECTION_HEADER *> image_section_header_;
};

#endif // PEFILE_H_