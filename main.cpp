#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <youtube URL>" << std::endl;
    return 1;
  }

  std::string url = argv[1];

  std::string command = "yt-dlp --write-auto-sub --sub-lang en --skip-download " + url;
  int result = std::system(command.c_str());
  if (result != 0)
  {
    std::cerr << "Failed to fetch transcript." << std::endl;
    return 1;
  }

  std::string transcript_file;
  for (const auto &entry : fs::directory_iterator("."))
  {
    if (entry.path().extension() == ".vtt")
    {
      transcript_file = entry.path().filename().string();
      std::cout << "Found transcript file: " << transcript_file << std::endl;
      break;
    }
  }

  if (transcript_file.empty())
  {
    std::cerr << "Transcript file not found!" << std::endl;
    return 1;
  }

  std::ifstream vtt_file(transcript_file);
  std::ofstream txt_file("transcript.txt");

  std::string line;
  std::string prev_line;

  while (getline(vtt_file, line))
  {
    // .vtt metadata, a .vtt file starts like this.
    if (line.find("WEBVTT") != std::string::npos || line.find("Kind:") != std::string::npos || line.find("Language:") != std::string::npos)
      continue;

    if (line.empty() || line.find("-->") != std::string::npos || line == " ")
      continue;

    size_t pos = 0;
    while ((pos = line.find("<", pos)) != std::string::npos)
    {
      size_t end_pos = line.find(">", pos);
      if (end_pos == std::string::npos)
        break;
      line.erase(pos, end_pos - pos + 1);
    }

    if (line != prev_line)
    {
      txt_file << line << std::endl;
      prev_line = line;
    }
  }

  vtt_file.close();
  txt_file.close();

  std::remove(transcript_file.c_str());

  std::cout << "Transcript saved to transcript.txt!" << std::endl;
  return 0;
}