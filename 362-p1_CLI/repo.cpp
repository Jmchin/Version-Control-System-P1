#include "repo.h"
#include "manifest.h"

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

      boost::replace_first(proj_dest, source, destination);

      // add the file we took from the repo into the manifest
      LogToManifest(proj_dest, new_manifest);


      fs::remove(proj_dest);    /* lol this hack, needed because the prior steps
                                   create the directory structure, but we can't
                                   copy_file into an existing directory name
                                   even if it is empty */

      fs::copy_file(co.source, proj_dest);
    }
  }
}

/**
   @param source - the source repository
   @param manifest - the manifest/label specifying the version of the repo
   @param target - the project directory to merge into

   @return - none
 */
void Merge(std::string source, std::string manifest, std::string target, std::string commands) {

  std::string temp;

  // hack together a checkin commands line for the manifest LOL
  std::string str = "";
  char DELIMITER = ' ';
  std::stringstream lineOne(commands);
  std::vector<std::string> commandsVector;
  while (std::getline(lineOne, str, DELIMITER)) {
    commandsVector.push_back(str);
  }

  std::string checkin_commands;
  checkin_commands += commandsVector[0] + " checkin " + commandsVector[4] + " " + commandsVector[2];

  // checkin the target directory to the repo so we have a recent manifest to work with
  CheckIn(target, source, checkin_commands);

  // get the manifest file if given a label
  manifest = getAliasIfExists(manifest, source);

  fs::path src_root(source);
  fs::path manifest_name(manifest);
  fs::path manifest_path(src_root / manifest_name);

  // get the current working directory
  std::string cwd = fs::current_path().string();

  // build path to source manifest
  std::stringstream source_manifest_ss;
  source_manifest_ss << cwd << "/" << source << "/" << manifest_name.string();

  // build path to target manifest
  std::stringstream target_manifest_ss;
  target_manifest_ss << cwd << "/" << source << "/" << get_current_version(source) << ".manifest";

  // get target and source history
  std::vector<std::string> source_history = GetLinearHistory(source_manifest_ss.str(), source);
  std::vector<std::string> target_history = GetLinearHistory(target_manifest_ss.str(), source);

  // construct vector of target manifest files
  std::fstream target_manifest;
  target_manifest.open(target_manifest_ss.str());

  // throw away first two lines
  std::getline(target_manifest, temp);
  std::getline(target_manifest, temp);
  std::vector<std::string> target_manifest_lines;
  while (std::getline(target_manifest, temp))
  {
    target_manifest_lines.push_back(temp);
  }

  std::fstream source_manifest;
  source_manifest.open(manifest_path.string());

  // throw away first two lines
  std::getline(source_manifest, temp);
  std::getline(source_manifest, temp);

  // construct vector of source manifest files
  std::vector<std::string> source_manifest_lines;
  while (std::getline(source_manifest, temp))
  {
    source_manifest_lines.push_back(temp);
  }

  std::string sml_check = "";
  std::string sml_clean = "";
  std::string tml_check = "";
  std::string tml_clean = "";

  // loop over all source manifest files
  for (size_t i=0; i < source_manifest_lines.size(); i++)
  {
    bool exists = false;

    // process lines into filename path and artifactID
    sml_check = "";
    sml_clean = source_manifest_lines[i];

    for (int k = sml_clean.length() - 1; k >= 0; k--) {
      if (sml_clean[k] == '/') {
        break;
      }
      else {
        sml_check = sml_clean[k] + sml_check;
      }
    }

    sml_clean.erase(sml_clean.end() - sml_check.size(), sml_clean.end());

    // loop over all target manifest lines
    for (size_t j=0; j <target_manifest_lines.size(); j++)
    {
      // process lines into filename path and artifactID
      tml_check = "";
      tml_clean = target_manifest_lines[j];
      for (int m = tml_clean.length() - 1; m >= 0; m--) {
        if (tml_clean[m] == '/') {
          break;
        }
        else {
          tml_check = tml_clean[m] + tml_check;
        }
      }
      tml_clean.erase(tml_clean.end() - tml_check.size(), tml_clean.end());

      // file exists in both source and target
      if (sml_clean == tml_clean)
      {
        exists = true;

        // files are different
        if (sml_check != tml_check)
        {

          std::fstream ancestor_manifest;
          std::string common = getCommonAncestor(source_history, target_history);
          ancestor_manifest.open(common);

          // throw away first two lines
          std::getline(ancestor_manifest, temp);
          std::getline(ancestor_manifest, temp);
          std::vector<std::string> ancestor_manifest_lines;

          // get all ancestor file lines
          while (std::getline(target_manifest, temp))
          {
            ancestor_manifest_lines.push_back(temp);
          }

          // TODO REMOVE
          for (auto l : ancestor_manifest_lines){
            std::cout << l << std::endl;
          }
          //loop over ancestor files lines
          for (size_t i=0; i < ancestor_manifest_lines.size(); i++){
            std::string anc_check = "";
            std::string anc_clean = ancestor_manifest_lines[i];
            for (int k = anc_clean.length() - 1; k >= 0; k--) {
              if (anc_clean[k] == '/') {
                break;
              }
              else {
                anc_check = anc_clean[k] + anc_check;
              }
            }
            anc_clean.erase(anc_clean.end() - anc_check.size(), anc_clean.end());
            // loop over target file lines
            for (size_t j=0; target_manifest_lines.size(); j++)
            {
              std::string tml_check = "";
              std::string tml_clean = target_manifest_lines[j];
              for (int m = tml_clean.length() - 1; m >= 0; m--) {
                if (tml_clean[m] == '/') {
                  break;
                }
                else {
                  tml_check = tml_clean[m] + tml_check;
                }
              }
              tml_clean.erase(tml_clean.end() - tml_check.size(), tml_clean.end());

              //found a file that didnt exist inside target manifest, I think lol
              if (anc_check != tml_check)
              {
                std::cout << "GOT THIS FAR" << std::endl;
                std::stringstream destination_path;
                destination_path << tml_clean << boost::filesystem::extension(tml_check);

                fs::copy_file(ancestor_manifest_lines[i],destination_path.str(),fs::copy_option::overwrite_if_exists);
              }
            }
          }
        }

        // we found a match, so break out of the loop
        break;
      }
    }
    if (!exists)
    {
      // TODO: wut?
      std::cout << "lol i dont even know" << std::endl;
    }
  }

  // close our files
  target_manifest.close();
  source_manifest.close();

}




  // TODO: compare artifactIDs from source_manifest and new target_manifest
  // std::cout << getCommonAncestor(source_history, target_history) << std::endl;

  // TODO: if exists in source_manifest and not in target_manifest, copy file to target directory and LogToManifest

  // TODO: if not in source_manifest, and exists in target_manifest, do nothing

  // TODO: if the same, do nothing

  // TODO: if conflict:

  // TODO: find rightmost intersection of the two histories, this is the common ancestor

  // TODO: copy files from source and ancestor to target_directory  (e.g foo_MR.cpp, foo_MG.cpp)

  // TODO:rename file in target_directory that was different (e.g foo_MT.cpp)

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
