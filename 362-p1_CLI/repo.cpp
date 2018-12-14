#include "repo.h"
#include "manifest.h"
#include "creationOrder.h"
#include "mapfile.h"

/**
   @param source - path to the project directory to create a repo from
   @param destination - path to the repository directory to initialize
   @param commands - the command line arguments to write to the manifest

   @return - none

   creates a version controlled repository from an initial project tree
*/
void Create(std::string source, std::string destination, std::string commands) {

  fs::path src_root(source);
  fs::path des_root(destination);

  // create the initial manifest file
  fs::path mInit("1.manifest");
  fs::path m(des_root / mInit);
  std::ofstream manifest(m.string());

  // log the command line arguments to manifest
  LogToManifest(commands, manifest);
  LogToManifest(timestamp(), manifest);

  // increment .latest_version
  update_version(destination);

  RepoifyDirectory(src_root, des_root, manifest);

  // create the initial .labels file
  InitLabels(destination);

  manifest.close();
}

/**
   @param source - path to the project directory to checkin from
   @param destination - path to the repository directory to checkin to
   @param commands - the command line arguments to write to the manifest

   @return - none

   checks-in a project tree into an existing repository
*/
void CheckIn(std::string source, std::string destination, std::string commands) {

  fs::path src_root(source);
  fs::path des_root(destination);

  // increment .latest_version.txt
  int latest_version = get_current_version(destination) + 1;
  update_version(des_root.string());

  // create checkin manifest
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

/**
    @param source - path to the source repository to checkout from
    @param manifest - filename/label specifying the manifest file to checkout from
    @param destination - path to the directory to checkout to
    @param commands - the command line arguments to log to the new checkout manifest

    @return - none
*/
void CheckOut(std::string source, std::string manifest, std::string destination, std::string commands) {

  // get the manifest file if given a label
  manifest = getAliasIfExists(manifest, source);

  fs::path src_root(source);
  fs::path des_root(destination);
  fs::path manifest_name(manifest);
  fs::path manifest_path(src_root / manifest_name);

  int latest_version = get_current_version(source) + 1;
  update_version(src_root.string());

  // write the new checkout manifest
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

/**
    @param src_root - filesystem path to project directory
    @param des_root - filesystem path to target repo directory
    @param manifest - filestream to the manifest file for the operation

    @return - none
*/
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

/**
    @param destination - path to root of a repo

    @return none

    initializes and writes an empty .labels file in the root of a repository
*/
void InitLabels(std::string destination) {

  // convert destination to a boost fs::path
  fs::path repo_root(destination);

  // the labels file
  fs::path file_name(".labels");

  // fully qualified path to .labels
  fs::path fullpath(repo_root / file_name);

  // create empty file at fullpath
  std::ofstream labels (fullpath.string());
  labels.close();
}

/**
    @param repo     - path to root of a repo
    @param manifest - filename/label specifying a version of the repo
    @param label    - the label to add to a manifest

    @return none

    adds a label to a specific manifest file so that the manifest can be specified
    using the label instead of the full filename

*/
void LabelManifest(std::string repo, std::string manifest, std::string label) {
  // build manifest path
  std::stringstream m_path;
  m_path << repo << "/" << manifest;

  // build labels path
  std::stringstream l_path;
  l_path << repo << "/" << ".labels";

  // open labels file
  std::ofstream l_file(l_path.str());

  // write label and associated manifest to .labels file
  l_file << label << std::endl;
  l_file << manifest << std::endl;

  l_file.close();
}
