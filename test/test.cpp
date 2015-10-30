#include "time_series.h"
#include <gtest/gtest.h>

//#define DDJ_USE_BOOST 1

TimeSeries<> GenerateFakeTimeSeries(std::string name, int size)
{
    TimeSeries<> result(name);
    for(int i = 0; i < size; i++)
    {
        result.InsertTime(i);
        result.InsertData((float)i * (i % 3));
    }
    return result;
}

TEST(TimeSeries, ReadWrite_Data_ToFile)
{
    auto testFile = File::GetTempFile();

    const TimeSeries<> ts1 = GenerateFakeTimeSeries("fake1", 1000);
    const TimeSeries<> ts2 = GenerateFakeTimeSeries("fake2", 1000);
    TSFileDefinition fileDefinition;
    TimeSeries<> ts;
    ts.WriteManyToFile(testFile, std::vector<TimeSeries<>> {ts1, ts2}, fileDefinition);
    auto result = ts.ReadManyFromFile(testFile, fileDefinition);

    EXPECT_TRUE(result[0].Equal(ts1));
    EXPECT_TRUE(result[1].Equal(ts2));
    testFile.Delete();
}

TEST(TimeSeries, ReadWrite_Data_FromFile)
{
    File realDataFile("../test/data/info.log");
    File testFile = File::GetTempFile();
    File testFile2 = File::GetTempFile();
    TSFileDefinition fileDefinition;
    TimeSeries<> ts;
    auto data = ts.ReadManyFromFile(realDataFile, fileDefinition);
    ts.WriteManyToFile(testFile, data, fileDefinition);
    auto data2 = ts.ReadManyFromFile(testFile, fileDefinition);
    ts.WriteManyToFile(testFile2, data2, fileDefinition);

    EXPECT_TRUE(testFile.Compare(testFile2));
    testFile.Delete();
}
