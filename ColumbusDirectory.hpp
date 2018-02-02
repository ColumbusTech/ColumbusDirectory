#ifdef _WIN32
	#define COLUMBUS_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#ifdef _WIN64
	#define COLUMBUS_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#ifdef __linux
	#define COLUMBUS_PLATFORM_LINUX
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
		#ifdef COLUMBUS_PLATFORM_LINUX
			char dir[FILENAME_MAX];
			getcwd(dir, FILENAME_MAX);
			return dir;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			char dir[MAX_PATH];
			GetModuleFileName(NULL, dir, MAX_PATH);
			return dir;
		#endif
	}

	bool Directory::create(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return mkdir(aPath.c_str(), 0777) ? false : true;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			return CreateFolder(aPath.c_str(), 4555);
		#endif
	}

	bool Directory::remove(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return rmdir(aPath.c_str()) ? false : true;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			return DeleteFolder(aPath.c_str());
		#endif
	}

	std::vector<std::string> Directory::read(const std::string aPath)
	{
		std::vector<std::string> ret;

		#ifdef COLUMBUS_PLATFORM_LINUX
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

#undef COLUMBUS_PLATFORM_WINDOWS
#undef COLUMBUS_PLATFORM_LINUX




