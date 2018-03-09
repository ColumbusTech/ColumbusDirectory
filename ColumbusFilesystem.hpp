#ifdef _WIN32
	#define COLUMBUS_PLATFORM_WINDOWS
	#include <windows.h>
#endif

#ifdef __linux
	#define COLUMBUS_PLATFORM_LINUX
	#include <dirent.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

namespace Columbus
{

	class Filesystem
	{
	public:
		Filesystem() {}

		static std::string getCurrent();
		static bool createDirectory(const std::string aPath);
		static bool createFile(const std::string aPath);
		static bool removeDirectory(const std::string aPath);
		static bool removeFile(const std::string aPath);
		static bool rename(const std::string aOld, const std::string aNew);
		static std::vector<std::string> read(const std::string aPath);
		static std::vector<std::string> readCurrent();

		~Filesystem() {}
	};

	std::string Filesystem::getCurrent()
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			char dir[4096];
			getcwd(dir, 4096);
			return dir;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			wchar_t wdir[MAX_PATH];
			char dir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, wdir);
			wcstombs(dir, wdir, MAX_PATH);
			return dir;
		#endif

		return "";
	}

	bool Filesystem::createDirectory(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return mkdir(aPath.c_str(), 0777) == 0;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			wchar_t* name = new wchar_t[aPath.size()];
			mbstowcs(name, aPath.c_str(), aPath.size() * 2);
			return CreateDirectory(name, NULL);
		#endif

		return false;
	}

	bool Filesystem::createFile(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return open(aPath.c_str(), O_CREAT | O_EXCL | O_RDWR | O_CLOEXEC, S_IRWXU) >= 0;
		#endif

		return false;
	}

	bool Filesystem::removeDirectory(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return rmdir(aPath.c_str()) == 0;
		#endif

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			wchar_t* name = new wchar_t[aPath.size()];
			mbstowcs(name, aPath.c_str(), aPath.size() * 2);
			return RemoveDirectory(name);
		#endif

		return false;
	}

	bool Filesystem::removeFile(const std::string aPath)
	{
		#ifdef COLUMBUS_PLATFORM_LINUX
			return remove(aPath.c_str()) == 0;
		#endif

		return false;
	}

	//INTERNAL USE
	static int rnm(const std::string aOld, const std::string aNew)
	{
		return rename(aOld.c_str(), aNew.c_str());
	}

	bool Filesystem::rename(const std::string aOld, const std::string aNew)
	{
		return rnm(aOld, aNew) == 0;
	}

	std::vector<std::string> Filesystem::read(const std::string aPath)
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

		#ifdef COLUMBUS_PLATFORM_WINDOWS
			HANDLE hFind;
			WIN32_FIND_DATA data;

			wchar_t* path = new wchar_t[aPath.size()];
			char file[MAX_PATH];
			mbstowcs(path, aPath.c_str(), aPath.size() * 2);

			std::wstring str = path;
			str += L"/*.*";

			hFind = FindFirstFile(str.c_str(), &data);

			if (hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					wcstombs(file, data.cFileName, MAX_PATH);
					ret.push_back(file);
				} while (FindNextFile(hFind, &data));
				FindClose(hFind);
			}
		#endif

		return ret;
	}

	std::vector<std::string> Filesystem::readCurrent()
	{
		return read(getCurrent());
	}

}

#undef COLUMBUS_PLATFORM_WINDOWS
#undef COLUMBUS_PLATFORM_LINUX




