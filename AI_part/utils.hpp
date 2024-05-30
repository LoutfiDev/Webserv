#ifndef UTLS
#define UTLS

#include <string>
#include <vector>

#define NAMELENGTH 10

void handleError(const char *, int);
std::string trim(const std::string& str, std::string toTrim =" \t\n\r");
std::string fileName();
void toLower(std::string &);
std::string getUri(std::string &, std::string &);


#endif //!UTLS
