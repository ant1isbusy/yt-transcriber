#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


std::string getVideoTitle(std::string url)
{
  std::string command = "yt-dlp --get-title " + url;
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe)
  {
    std::cerr << "Failed to get video title!" << std::endl;
    exit(1); // safe to exit here, we have not alloced any memory.
  }

  std::string vid_title;
  char buff[128];
  while (fgets(buff, sizeof(buff), pipe) != NULL)
  {
    vid_title += buff;
  }

  pclose(pipe);
  std::cout << vid_title << std::endl;
  return vid_title;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << "<youtube URL>" << std::endl;
    return 1;
  }

  std::string url = argv[1];
  std::string vid_title = getVideoTitle(url);

  
  return 0;
}
