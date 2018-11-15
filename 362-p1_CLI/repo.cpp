#include "repo.h"
#include "manifest.h"
#include "creationOrder.h"
#include "mapfile.h"

// REFACTOR: all of this spagghetti
// TODO: Add label support
void Create(std::string source, std::string destination, std::string commands) {


  fs::path src_root(source);
  fs::path des_root(destination);

  // create the initial manifest file
  fs::path mInit("1.manifest");
  fs::path m(des_root / mInit);
  std::ofstream manifest(m.string());
  LogToManifest(commands, manifest);
  LogToManifest(timestamp(), manifest);

  update_version(destination);

  RepoifyDirectory(src_root, des_root, manifest);

  InitLabels(destination);

  manifest.close();
}

// TODO: Add label support
void CheckIn(std::string source, std::string destination, std::string commands) {


  fs::path src_root(source);
  fs::path des_root(destination);

  int latest_version = get_current_version(destination) + 1;
  update_version(des_root.string());

  std::stringstream manifest_file;
  manifest_file << latest_version << ".manifest";
  fs::path m_path(manifest_file.str());
  fs::path m(des_root / m_path);

  std::ofstream manifest(m.string());

  LogToManifest(commands, manifest);
  LogToManifest(timestamp(), manifest);

  RepoifyDirectory(src_root, des_root, manifest);

  manifest.close();
}

// TODO: Add label support, log checked out files to the manifest
void CheckOut(std::string source, std::string manifest, std::string destination, std::string commands) {

  manifest = getAliasIfExists(manifest);

  fs::path src_root(source);
  fs::path des_root(destination);
  fs::path manifest_name(manifest);
  fs::path manifest_path(src_root / manifest_name);

  // write the new checkout manifest
  int latest_version = get_current_version(source) + 1;
  update_version(src_root.string());

  std::stringstream new_manifest_name;
  new_manifest_name << latest_version << ".manifest";
  fs::path m_path(new_manifest_name.str());
  fs::path m(src_root / m_path);
  std::ofstream new_manifest(m.string());

  LogToManifest(commands, new_manifest);
  LogToManifest(timestamp(), new_manifest);

  std::vector<CreationObject> creation_order = GetCreationOrder(manifest_path.string());

  for (auto co : creation_order) {
    if (co.isFolder) {
      std::string proj_dest = co.destination;
      boost::replace_first(proj_dest, source, destination);
      fs::create_directories(proj_dest);
    }
    else {
      std::string proj_dest = co.destination;

      // add the file we took from the repo into the manifest
      LogToManifest(proj_dest, new_manifest);

      boost::replace_first(proj_dest, source, destination);

      fs::remove(proj_dest);    /* lol this hack, needed because the prior steps
                                   create the directory structure, but we can't
                                   copy_file into an existing directory name
                                   even if it is empty */

      fs::copy_file(co.source, proj_dest);
    }
  }
}

void RepoifyDirectory(fs::path src_root, fs::path des_root, std::ofstream& manifest) {

  for(fs::directory_entry& p: fs::recursive_directory_iterator(src_root)) {
    if(fs::is_regular_file(p.path())) {

      // get the current src path
      std::string src_path_string = p.path().string();

      // REFACTOR: CheckSum should take path instead of string
      fs::path checksum(CheckSum(src_path_string));

      // REFACTOR: build repo path for the file
      fs::path repo_path(src_path_string);
      std::string repo_string = repo_path.string();
      boost::replace_first(repo_string, src_root.string(), des_root.string());
      fs::path dest_path(repo_string);
      fs::path repo_final(dest_path / checksum);

      // fill the repository file structure
      fs::create_directories(dest_path);
      fs::copy_file(p.path(), repo_final, fs::copy_option::overwrite_if_exists);
      LogToManifest(repo_final.string(), manifest);

    }
  }
}

void InitLabels(std::string destination) {
  fs::path repo_root(destination);
  fs::path file_name(".labels");
  fs::path fullpath(repo_root / file_name);

  std::ofstream labels (fullpath.string());
  labels.close();
}

void LabelManifest(std::string repo, std::string manifest, std::string label) {
  // get manifest path
  std::stringstream m_path;
  m_path << repo << "/" << manifest;

  // get labels path
  std::stringstream l_path;
  l_path << repo << "/" << ".labels";

  // open labels file
  std::ofstream l_file(l_path.str());

  l_file << label << std::endl;
  l_file << manifest << std::endl;

  l_file.close();
}
