//
// Created by Karol Dzitkowski on 19.10.15.
//

#ifndef TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
#define TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H

struct CSVFileDefinition
{
    std::vector<std::string> Header;
    bool UseCustomHeader = false;
    int TimeValueIndex = 0;
    std::string Separator = ",";
};

struct BinaryFileDefinition
{
    int NoTimeSeries;
};

#endif //TIME_SERIES_DATA_READER_TS_FILE_DEFINITION_H
