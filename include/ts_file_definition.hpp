//
// Created by Karol Dzitkowski on 19.10.15.
//

#ifndef TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
#define TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H

#include <vector>

struct CSVFileDefinition
{
    bool HasHeader = true;
    std::string Separator = ",";

    std::vector<std::string> Header;
    std::vector<DataType> Columns;
};

struct BinaryFileDefinition
{
    std::vector<std::string> Header;
    std::vector<DataType> Columns;
};

#endif //TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
