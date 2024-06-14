#ifndef UTLS
#define UTLS

#include <string>
#include <vector>

#define NAMELENGTH 10

void handleError(const char *, int);
std::string trim(const std::string& str, std::string toTrim =" \t\n\r");
std::string generateFileName(int gen=1);
void toLower(std::string &);
std::string getUri(std::string &, std::string );
std::string removeLastChar(std::string); // remove last '/'
std::string to_String(long long);
std::string getExtension(const std::string &);

#endif //!UTLS
