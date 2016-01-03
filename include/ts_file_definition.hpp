//
// Created by Karol Dzitkowski on 19.10.15.
//

#ifndef TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
#define TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H

#include <vector>

struct FileDefinition
{
    std::vector<std::string> Header;
    std::vector<DataType> Columns;
};

struct CSVFileDefinition : FileDefinition
{
    bool HasHeader = true;
    std::string Separator = ",";
};

struct BinaryFileDefinition : FileDefinition
{
};

#endif //TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
