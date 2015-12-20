#include "time_series_reader.hpp"
#include <gtest/gtest.h>
#include <time_series_writer.hpp>

//#define DDJ_USE_BOOST 1

TimeSeries GenerateFakeTimeSeries(std::string name, int N)
{
    TimeSeries result(name);
    result.init(std::vector<DataType> {DataType::d_time, DataType::d_double});

    size_t size = result.getRecordSize();
    char* data = new char[size];

    for(time_t i = 0; i < N; i++)
    {
        double value = (double)i * (i % 3);
        memcpy(data, &i, sizeof(time_t));
        memcpy(data+sizeof(time_t), &value, sizeof(double));
        result.addRecord(data);
    }

    return result;
}

TEST(TimeSeriesTest, ReadWrite_CSV_Data_ToFile)
{
    auto testFile = File::GetTempFile();
    TimeSeries fake = GenerateFakeTimeSeries("fake", 1000);
    CSVFileDefinition fileDefinition;
    TimeSeriesWriter().WriteToCSV(testFile, fake, fileDefinition);
    auto result = TimeSeriesReader().ReadFromCSV(testFile, fileDefinition);
    EXPECT_TRUE(result.compare(fake));
    testFile.Delete();
}

TEST(TimeSeries, ReadWrite_CSV_Data_FromFile)
{
    File realDataFile("../test/data/info.log");
    File testFile = File::GetTempFile();
    File testFile2 = File::GetTempFile();
    CSVFileDefinition fileDefinition;
    fileDefinition.Columns = std::vector<DataType> {
            DataType::d_time,
            DataType::d_float,
            DataType::d_float,
            DataType::d_float
    };
    auto data = TimeSeriesReader().ReadFromCSV(realDataFile, fileDefinition);
    TimeSeriesWriter().WriteToCSV(testFile, data, fileDefinition);
    auto data2 = TimeSeriesReader().ReadFromCSV(testFile, fileDefinition);
    TimeSeriesWriter().WriteToCSV(testFile2, data2, fileDefinition);
    EXPECT_TRUE(testFile.Compare(testFile2));
    testFile.Delete();
    testFile2.Delete();
}

TEST(TimeSeries, ReadWrite_Binary_Data_ToFile)
{
    auto testFile = File::GetTempFile();

    TimeSeries fake = GenerateFakeTimeSeries("fake", 1000);

    BinaryFileDefinition fileDefinition;
    fileDefinition.Header = std::vector<std::string> {"time", "value"};
    fileDefinition.Columns = std::vector<DataType> { DataType::d_time, DataType::d_float };
    TimeSeriesWriter().WriteToBinary(testFile, fake);
    auto result = TimeSeriesReader().ReadFromBinary(testFile, fileDefinition);
    EXPECT_TRUE(result.compare(fake));
    testFile.Delete();
}

TEST(TimeSeries, Read_CSV_Data_InMultipleParts_FromFile_CheckWithOnePartRead)
{
	File realDataFile("../test/data/info.log");
	CSVFileDefinition fileDefinition;
    fileDefinition.Columns = std::vector<DataType> {
            DataType::d_time,
            DataType::d_float,
            DataType::d_float,
            DataType::d_float
    };

	// read whole data 1000 rows
	int allRows = 1000;
	auto wholeData = TimeSeriesReader().ReadFromCSV(realDataFile, fileDefinition, allRows);

	// compare with reading 5 times 200 rows
	int partNo = 5;
	int partRowCnt = allRows / partNo;
	TimeSeriesReader reader;
	for(int i = 0; i < partNo; i++)
	{
		auto partData = reader.ReadFromCSV(realDataFile, fileDefinition, partRowCnt);
        EXPECT_EQ(partData.getRecordAsStrings(0)[0], wholeData.getRecordAsStrings(partRowCnt*i)[0]);
	}
}
