#include "FileUtil.h"


#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>

namespace oar {
  namespace FSUtil {
    void listAllFile(std::vector<std::string>& files
		     ,const std::string& path
		     ,const std::string& subfix,int spaceNum) {
      if (access(path.c_str(),0) != 0)
	return;

      DIR* dir;
      if ((dir = opendir(path.c_str())) == nullptr) {
	return;
      }

      struct dirent* de = nullptr;

      while ((de = readdir(dir))) {
	if (de->d_type == DT_DIR) {
	  if (!strcmp(de->d_name,".") || !strcmp(de->d_name,"..")
	      ||!strcmp(de->d_name,"./") || !strcmp(de->d_name,"../")) {
	    continue;
	  }
	  files.emplace_back(std::string(de->d_name) + "[dir]");
	  //	  files.emplace_back("├");
	  listAllFile(files, path + "/" + de->d_name, subfix, spaceNum+4);
	}
	else if (de->d_type == DT_REG) {
	  std::string filename {de->d_name};
	  if (subfix.empty())
	    files.emplace_back(std::string(spaceNum, ' ') + path + "/" + filename);
	  else {
	    if (filename.size() < subfix.size()) {
	      std::cout << "subfix.size() = " << subfix.size() << std::endl;
	      continue;
	    }
	    if (filename.substr(filename.length() - subfix.size()) == subfix)
	      {
		files.emplace_back(std::string(spaceNum, ' ') + path + "/" + filename);
	      } 
	  }
	}
	
      }
      closedir(dir);
    }
    //    static bool mkdir(const std::string& dirname);
    
  }
}
