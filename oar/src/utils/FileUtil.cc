#include "oar/utils/FileUtil.h"

#include "oar/utils/strutil.h"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
namespace oar {
namespace FSUtil {
    void listAllFile(std::vector<std::string>& files, const std::string& path, const std::string& subfix, int spaceNum)
    {
        if (access(path.c_str(), 0) != 0)
            return;

        DIR* dir;
        if ((dir = opendir(path.c_str())) == nullptr) {
            return;
        }

        struct dirent* de = nullptr;

        while ((de = readdir(dir))) {
            if (de->d_type == DT_DIR) {
                if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")
                    || !strcmp(de->d_name, "./") || !strcmp(de->d_name, "../")) {
                    continue;
                }
                files.emplace_back(std::string(de->d_name) + "[dir]");
                //	  files.emplace_back("├");
                listAllFile(files, path + "/" + de->d_name, subfix, spaceNum + 4);
            } else if (de->d_type == DT_REG) {
                std::string filename { de->d_name };
                if (subfix.empty())
                    files.emplace_back(std::string(spaceNum, ' ') + path + "/" + filename);
                else {
                    if (filename.size() < subfix.size()) {
                        std::cout << "subfix.size() = " << subfix.size() << std::endl;
                        continue;
                    }
                    if (filename.substr(filename.length() - subfix.size()) == subfix) {
                        files.emplace_back(std::string(spaceNum, ' ') + path + "/" + filename);
                    }
                }
            }
        }
        closedir(dir);
    }

    static int __linkstat(const char* filename, struct stat* st = nullptr)
    {
        struct stat _st;
        int ret = lstat(filename, &_st);
        if (st)
            *st = _st;
        return ret;
    }

    static bool __mkdir(const std::string& dirname)
    {
        if (access(dirname.c_str(), F_OK) == 0)
            return 0;
        return ::mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    bool mkdir(const std::string& dirname)
    {
        if (__linkstat(dirname.c_str()) == 0)
            return true;
        char* path = strdup(dirname.c_str());
        char* ptr = strchr(path + 1, '/');
        do {
            for (; ptr; *ptr = '/', ptr = strchr(ptr + 1, '/')) {
                *ptr = '\0';
                if (__mkdir(path) != 0)
                    break;
            }
            if (ptr != nullptr)
                break;
            else if (__mkdir(path) != 0)
                break;
            free(path);
            return true;
        } while (0);
        free(path);
        return false;
    }

    bool unlink(const std::string& filename, bool exist)
    {
        if (!exist && __linkstat(filename.c_str()))
            return true;
        return ::unlink(filename.c_str()) == 0;
    }

    bool rm(const std::string& path)
    {
        struct stat st;
        if (lstat(path.c_str(), &st))
            return true;
        if (!(st.st_mode & S_IFDIR))
            return FSUtil::unlink(path);
        DIR* dir = opendir(path.c_str());
        if (!dir)
            return false;
        bool ret = true;
        struct dirent* dent = nullptr;
        while (dent = readdir(dir)) {
            if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) {
                continue;
            }
            std::string dirname = path + "/" + dent->d_name;
            ret = FSUtil::rm(dirname);
        }
        closedir(dir);
        if (::rmdir(path.c_str()))
            ret = false;
        return ret;
    }

    bool mv(const std::string& from, const std::string& to)
    {
        if (!FSUtil::rm(to))
            return false;
        return ::rename(from.c_str(), to.c_str());
    }

    bool realPath(const std::string& path, std::string& rpath)
    {
        if (__linkstat(path.c_str()))
            return false;
        char* ptr = ::realpath(path.c_str(), nullptr);
        if (!ptr)
            return false;
        std::string(ptr).swap(rpath);
        free(ptr);
        return true;
    }

    std::string dirName(const std::string& filename)
    {
        if (filename.empty())
            return ".";
        auto pos = filename.rfind('/');
        if (pos == 0) {
            return "/";
        } else if (pos == std::string::npos) {
            return ".";
        } else {
            return filename.substr(0, pos);
        }
    }

    bool openForRead(std::ifstream& ifs, const std::string& filename, std::ios_base::openmode mode)
    {
        ifs.open(filename.c_str(), mode);
        return ifs.is_open();
    }

    bool openForWrite(std::ofstream& ofs, const std::string& filename, std::ios_base::openmode mode)
    {
        ofs.open(filename.c_str(), mode);
        if (!ofs.is_open()) {
            std::string dir = dirName(filename);
            FSUtil::mkdir(dir);
            ofs.open(filename.c_str(), mode);
        }
        return ofs.is_open();
    }

}
}
