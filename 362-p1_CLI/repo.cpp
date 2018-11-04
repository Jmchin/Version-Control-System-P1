#include "repo.h"

void Create(std::string source, std::string destination) {

  fs::path src(source);
  fs::path des(destination);

  // make a copy of the source directory
  DeepCopyDir(src, des);

  // folderify every leaf in the newly copied repo directory
  for(fs::directory_entry& p: fs::recursive_directory_iterator(des)) {
    if(fs::is_regular_file(p.path())) {

      FolderifyLeaf(p.path().string());

      // TODO: log these operations to the manifest file somehow
    }
  }

}

void CheckIn(std::string source, std::string destination) {
  std::cout << "Not implemented" << std::endl;

}

void CheckOut(std::string manifest, std::string destination) {
  std::cout << "Not implemented" << std::endl;

}
