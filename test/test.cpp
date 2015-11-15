#include "time_series.h"
#include <gtest/gtest.h>

//#define DDJ_USE_BOOST 1

TimeSeries<> GenerateFakeTimeSeries(std::string name, int size)
{
    TimeSeries<> result(name);
    for(int i = 0; i < size; i++)
    {
        result.InsertTime(i);
        result.InsertData((double)i * (i % 3));
    }
    return result;
}

TEST(TimeSeries, ReadWrite_CSV_Data_ToFile)
{
    auto testFile = File::GetTempFile();

    const TimeSeries<> ts1 = GenerateFakeTimeSeries("fake1", 1000);
    const TimeSeries<> ts2 = GenerateFakeTimeSeries("fake2", 1000);
    CSVFileDefinition fileDefinition;
    TimeSeries<>::WriteToCSV(testFile, std::vector<TimeSeries<>> {ts1, ts2}, fileDefinition);
    auto result = TimeSeries<>::ReadFromCSV(testFile, fileDefinition);

    EXPECT_TRUE(result[0].Equal(ts1));
    EXPECT_TRUE(result[1].Equal(ts2));
    testFile.Delete();
}

TEST(TimeSeries, ReadWrite_CSV_Data_FromFile)
{
    File realDataFile("../test/data/info.log");
    File testFile = File::GetTempFile();
    File testFile2 = File::GetTempFile();
    CSVFileDefinition fileDefinition;
    auto data = TimeSeries<>::ReadFromCSV(realDataFile, fileDefinition);
    TimeSeries<>::WriteToCSV(testFile, data, fileDefinition);
    auto data2 = TimeSeries<>::ReadFromCSV(testFile, fileDefinition);
    TimeSeries<>::WriteToCSV(testFile2, data2, fileDefinition);

    EXPECT_TRUE(testFile.Compare(testFile2));
    testFile.Delete();
}

TEST(TimeSeries, ReadWrite_Binary_Data_ToFile)
{
    auto testFile = File::GetTempFile();

    const TimeSeries<> ts1 = GenerateFakeTimeSeries("fake1", 1000);
    const TimeSeries<> ts2 = GenerateFakeTimeSeries("fake2", 1000);
    BinaryFileDefinition fileDefinition { 2 };
    TimeSeries<>::WriteToBinary(testFile, std::vector<TimeSeries<>> {ts1, ts2});
    auto result = TimeSeries<>::ReadFromBinary(testFile, fileDefinition);

    EXPECT_TRUE(result[0].Equal(ts1));
    EXPECT_TRUE(result[1].Equal(ts2));
    testFile.Delete();
}
