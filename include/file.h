//
// Created by Karol Dzitkowski on 19.10.15.
//

#ifndef TIME_SERIES_DATA_READER_FILE_H
#define TIME_SERIES_DATA_READER_FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>

class File
{
public:
    File(const char* path) : _path(path) {}
    File(std::string path) : _path(path) {}
    ~File(){}
    File(const File& other) : _path(other._path) {}
    File(File&& other) : _path(std::move(other._path)) {}

public:
    static File GetTempFile();

public:
    size_t GetSize() const;
    std::string GetPath() const;
    bool Compare(const File& other) const;
    bool Delete();

private:
    std::string _path;
};


#endif //TIME_SERIES_DATA_READER_FILE_H
