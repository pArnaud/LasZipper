// LasZipper

#include <iostream>
#include <string>
#include <vector>

#include <pdal/PointView.hpp>
#include <pdal/PointTable.hpp>
#include <pdal/Dimension.hpp>
#include <pdal/Options.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/io/BufferReader.hpp>
#include <pdal/io/LasReader.hpp>


#include <boost/filesystem.hpp>

void Help()
{
	std::cout << "LasZipper help." << std::endl;
	std::cout << "Usage :" << std::endl;
	std::cout << "\t-c [las files] : compress [las files]" << std::endl;
	std::cout << "\t-u [las files] : uncompress [las files]" << std::endl;

}
void OpenLas(pdal::LasReader *las_reader, const std::string lasName)
{
	pdal::Option las_opt("filename", lasName);
	pdal::Options las_opts;
	las_opts.add(las_opt);
	las_reader->setOptions(las_opts);
	pdal::PointTable table;
	las_reader->prepare(table);
	pdal::PointViewSet point_view_set = las_reader->execute(table);
}

void WriteLas(pdal::LasReader *las_reader, const std::string lasName)
{
	pdal::StageFactory factory;
	pdal::Stage *writer = factory.createStage("writers.las");
	pdal::Options options;
	options.add("filename", lasName);
	writer->setInput(*las_reader);
	writer->setOptions(options);

	pdal::PointTable table;
	writer->prepare(table);
	writer->execute(table);
}

boost::filesystem::path CompressPath(boost::filesystem::path path)
{
		bool fileOk(false);
	boost::filesystem::path compressPath;
	int counter(0);
	while(!fileOk)
	{
		std::string directory,strCounter;
		if (counter == 0)
		{
			strCounter = "";
		}
		else
		{			
			strCounter = std::to_string(counter);
		}

		if (path.parent_path().string().size() != 0)
		{
			directory = path.parent_path().string() + "/";
		}
		else
		{
			directory = "";
		}
		compressPath = boost::filesystem::path( directory + path.stem().string() + "_Compressed"+strCounter+".las");
		if(boost::filesystem::exists(compressPath))
		{
			++counter;
		}
		else
		{

			fileOk = true;
		}
	}
	
	return compressPath;
}
boost::filesystem::path UncompressPath(boost::filesystem::path path)
{
	bool fileOk(false);
	boost::filesystem::path uncompressPath;
	int counter(0);
	while(!fileOk)
	{
		std::string directory,strCounter;
		if (counter == 0)
		{
			strCounter = "";
		}
		else
		{			
			strCounter = std::to_string(counter);
		}

		if (path.parent_path().string().size() != 0)
		{
			directory = path.parent_path().string() + "/";
		}
		else
		{
			directory = "";
		}
		uncompressPath = boost::filesystem::path( directory + path.stem().string() + "_Uncompressed"+strCounter+".las");
		if(boost::filesystem::exists(uncompressPath))
		{
			++counter;
		}
		else
		{

			fileOk = true;
		}
	}
	
	return uncompressPath;
}

int main(int argc, char *argv[]) {


	if (argc < 3)
	{
		Help();
		return 0;
	}
	std::string option = argv[1];
	bool compress = false, uncompress = false;
	if (option == "-u")
	{
		//std::cout << "uncompress" << std::endl;
		uncompress = true;
	}
	else if (option == "-c")
	{
		//std::cout << "compress" << std::endl;
		compress = true;
	}
	else
	{
		Help();
		return 0;
	}


	std::vector<boost::filesystem::path> paths;
	for (int i = 2; i < argc; i++){
		//std::cout << "i = " << i << std::endl;
		paths.push_back(argv[i]);
	}

	for (std::vector<boost::filesystem::path>::const_iterator i = paths.begin(); i != paths.end(); ++i)
	{
		pdal::LasReader las_reader;
		OpenLas(&las_reader, i->string());

		pdal::LasHeader las_header = las_reader.header();

		if (compress)
		{
			if (las_header.compressed() == 1)
			{
				std::cout << "The las file " << i->string() << " is already compressed." << std::endl;
			}
			else if (las_header.compressed() == 0)
			{
				std::cout << "Compressing "<< i->string() << std::endl;
				WriteLas(&las_reader, CompressPath(*i).string());
				std::cout << "New file : " << CompressPath(*i).string() << std::endl;
			}
		}
		else if (uncompress)
		{
			if (las_header.compressed() == 1)
			{
				std::cout << "Decompressing "<< i->string() << std::endl;
				WriteLas(&las_reader, UncompressPath(*i).string());
				std::cout << "New file : " << UncompressPath(*i).string() << std::endl;

			}
			else if (las_header.compressed() == 0)
			{
				std::cout << "The las file " << i->string() << " is already uncompressed." << std::endl;
			}
		}		
		else
		{
			std::cout << "Error..." << std::endl;
		}
	}
	return 0;
}
