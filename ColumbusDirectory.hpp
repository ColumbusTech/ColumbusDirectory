#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef _WIN64
	#include <windows.h>
#endif

#ifdef __linux
	#include <dirent.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

#include <string>
#include <vector>

namespace Columbus
{

	class Directory
	{
	public:
		Directory() {}

		static std::string getCurrent();
		static bool create(const std::string aPath);
		static bool remove(const std::string aPath);
		static std::vector<std::string> read(const std::string aPath);
		static std::vector<std::string> readCurrent();

		~Directory() {}
	};

	std::string Directory::getCurrent()
	{
		#ifdef __linux
			char dir[512];
			getcwd(dir, 512);
			return dir;
		#endif
	}

	bool Directory::create(const std::string aPath)
	{
		#ifdef __linux
			return mkdir(aPath.c_str(), 0777) ? false : true;
		#endif
	}

	bool Directory::remove(const std::string aPath)
	{
		#ifdef __linux
			return rmdir(aPath.c_str()) ? false : true;
		#endif
	}

	std::vector<std::string> Directory::read(const std::string aPath)
	{
		std::vector<std::string> ret;

		#ifdef __linux
			DIR* dir = opendir(aPath.c_str());
			if (dir == NULL) return ret;

			dirent* dptr = NULL;

			while ((dptr = readdir(dir)) != NULL)
			{
				ret.push_back(dptr->d_name);
			}

			closedir(dir);
		#endif

		return ret;
	}

	std::vector<std::string> Directory::readCurrent()
	{
		return read(getCurrent());
	}

}






