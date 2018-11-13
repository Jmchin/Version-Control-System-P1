#include "repo.h"
#include "manifest.h"

// REFACTOR: all of this spagghetti
void Create(std::string source, std::string destination, std::string commands) {
  fs::path src_root(source);
  fs::path des_root(destination);
  fs::path mInit("1.manifest");
  fs::path m(des_root / mInit);


  // create the initial manifest file
  std::ofstream manifest(m.string());

  LogToManifest(commands, manifest);
  LogToManifest(timestamp(), manifest);

  update_version(destination);

  // folderify every leaf in the newly copied repo directory
  for(fs::directory_entry& p: fs::recursive_directory_iterator(src_root)) {
    if(fs::is_regular_file(p.path())) {

      // get the current src path
      std::string src_path_string = p.path().string();

      // REFACTOR: CheckSum should take path instead of string
      fs::path checksum(CheckSum(src_path_string));

      // build repo path for the file
      fs::path repo_path(src_path_string);
      std::string repo_string = repo_path.string();
      boost::replace_first(repo_string, source, destination);  // quick hack to replace src with dest
      fs::path dest_path(repo_string);
      fs::path repo_final(dest_path / checksum);

      // fill the repository file structure
      fs::create_directories(dest_path);
      fs::copy_file(p.path(), repo_final);
      LogToManifest(repo_final.string(), manifest);

    }
  }

  manifest.close();
}

void CheckIn(std::string source, std::string destination, std::string commands) {
  fs::path src_root(source);
  fs::path des_root(destination);
  fs::path mInit("2.manifest");
  fs::path m(des_root / mInit);


  // create the initial manifest file
  std::ofstream manifest(m.string());

  LogToManifest(commands, manifest);
  LogToManifest(timestamp(), manifest);

  // folderify every leaf in the newly copied repo directory
  for(fs::directory_entry& p: fs::recursive_directory_iterator(src_root)) {
    if(fs::is_regular_file(p.path())) {

      // get the current src path
      std::string src_path_string = p.path().string();

      // REFACTOR: CheckSum should take path instead of string
      fs::path checksum(CheckSum(src_path_string));

      // build repo path for the file
      fs::path repo_path(src_path_string);
      std::string repo_string = repo_path.string();
      boost::replace_first(repo_string, source, destination);  // quick hack to replace src with dest
      fs::path dest_path(repo_string);
      fs::path repo_final(dest_path / checksum);


      // fill the repository file structure
      fs::create_directories(dest_path);
      fs::copy_file(p.path(), repo_final, fs::copy_option::overwrite_if_exists);
      LogToManifest(repo_final.string(), manifest);

    }
  }

  manifest.close();
}

void CheckOut(std::string manifest, std::string destination) {
  std::cout << "Not implemented" << std::endl;

}
