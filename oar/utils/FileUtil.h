#ifndef __OAR_FILEUTIL_H__
#define __OAR_FILEUTIL_H__

#include <vector>
#include <string>
#include <iostream>


namespace oar {
  namespace FSUtil {
    void listAllFile(std::vector<std::string>& files
		     ,const std::string& path
		     ,const std::string& subfix,
		     int spaceNum = 0);
    /*    static bool mkdir(const std::string& dirname);
	  static bool isRunningPidfile(const std::string& pidfile);
	  static bool rm(const std::string& path);
	  static bool mv(const std::string& from, const std::string& to);
	  static bool realpath(const std::string& path, std::string& rpath);
	  static bool symlink(const std::string& frm, const std::string& to);
	  static bool unlink(const std::string& filename, bool exist = false);
	  static std::string dirName(const std::string& filename);
	  static std::string baseName(const std::string& filename);
	  static bool openForRead(std::ifstream& ifs, const std::string& filename
	  ,std::ios_base::openmode mode);
	  static bool openForWrite(std::ofstream& ofs, const std::string& filename
	  ,std::ios_base::openmode mode);
    */
  }
}

#endif
